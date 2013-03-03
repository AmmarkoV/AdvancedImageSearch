#include "findObjectsSURF.h"
#include "../configuration.h"
#include "../tools/timers.h"

#define USE_OPENCV_SURF_DETECTOR 1


#if USE_OPENCV_SURF_DETECTOR
#include <cv.h>
#include <cxcore.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct pair
{
    float p1;
    float p2;
};

struct pairList
{
   unsigned int maxItems;
   unsigned int currentItems;
   struct pair * item;
};


struct pairList * growPairList(struct pairList * pairs,unsigned int addedItems)
{
  if (pairs==0) { return 0; }
  unsigned int itemsByteSize=sizeof(struct pairList) * ( addedItems  + pairs->maxItems);
  struct pairList * newPair = (struct pairList *) realloc(pairs,itemsByteSize);
  if (newPair!=0) { pairs->maxItems+=addedItems; }
  return newPair;
};


/* ------------------------------------------------------------------------------------------------- */
struct pairList * initializePairList(struct pairList * pairs,unsigned int initialItems)
{
  unsigned int initialItemsByteSize=sizeof(struct pairList) * initialItems;
  struct pairList * newPair = 0 ;
  if (pairs==0)
     {
       newPair= ( struct pairList * ) malloc(initialItemsByteSize);
       if (newPair == 0 ) { fprintf(stderr,"Could not initialize new pair list"); return 0; }
       newPair->maxItems = initialItems;
     } else
     {
        if (initialItems>newPair->maxItems)
        {
          newPair=( struct pairList * ) realloc(pairs,initialItemsByteSize);
          if (newPair == 0 ) { fprintf(stderr,"Could not reallocate new pair list"); return 0; }
          newPair->maxItems = initialItems;
        } else
        {
          //we have more space initialized that the initialItems
          //we don't care
        }
     }
  pairs->currentItems=0;
  return newPair;
}


int clearPairList(struct pairList * pairs)
{
  if (pairs==0) { return 0; }
  pairs->currentItems=0;
  return 1;
}

int destroyPairList(struct pairList ** pairs)
{
  if (*pairs==0) { return 0; }
  free(*pairs);
  *pairs = 0;
  return 1;
}
/* ------------------------------------------------------------------------------------------------- */



double
compareSURFDescriptors( const float* d1, const float* d2, double best, int length )
{
    double total_cost = 0;
    if ( length % 4 != 0 ) { fprintf(stderr,"Cannot compare SURF descriptors \n"); return 0.0; }
    int i = 0;
    for( i = 0; i < length; i += 4 )
    {
        double t0 = d1[i  ] - d2[i  ];
        double t1 = d1[i+1] - d2[i+1];
        double t2 = d1[i+2] - d2[i+2];
        double t3 = d1[i+3] - d2[i+3];
        total_cost += t0*t0 + t1*t1 + t2*t2 + t3*t3;
        if( total_cost > best )
            break;
    }
    return total_cost;
}


int
naiveNearestNeighbor( const float* vec, int laplacian,
                      const CvSeq* model_keypoints,
                      const CvSeq* model_descriptors )
{
    int length = (int)(model_descriptors->elem_size/sizeof(float));
    int i, neighbor = -1;
    double d, dist1 = 1e6, dist2 = 1e6;
    CvSeqReader reader, kreader;
    cvStartReadSeq( model_keypoints, &kreader, 0 );
    cvStartReadSeq( model_descriptors, &reader, 0 );

    for( i = 0; i < model_descriptors->total; i++ )
    {
        const CvSURFPoint* kp = (const CvSURFPoint*)kreader.ptr;
        const float* mvec = (const float*)reader.ptr;
    	CV_NEXT_SEQ_ELEM( kreader.seq->elem_size, kreader );
        CV_NEXT_SEQ_ELEM( reader.seq->elem_size, reader );
        if( laplacian != kp->laplacian )
            continue;
        d = compareSURFDescriptors( vec, mvec, dist2, length );
        if( d < dist1 )
        {
            dist2 = dist1;
            dist1 = d;
            neighbor = i;
        }
        else if ( d < dist2 )
            dist2 = d;
    }
    if ( dist1 < 0.6*dist2 )
        return neighbor;
    return -1;
}

void
findPairs( const CvSeq* objectKeypoints, const CvSeq* objectDescriptors,
           const CvSeq* imageKeypoints, const CvSeq* imageDescriptors, struct pairList * ptpairs )
{
    CvSeqReader reader, kreader;
    cvStartReadSeq( objectKeypoints, &kreader , 0 );
    cvStartReadSeq( objectDescriptors, &reader , 0 );


    struct pairList * pairs = initializePairList(ptpairs,objectDescriptors->total);
    if (pairs==0) { fprintf(stderr,"Could not allocate enough memory for pointer pairs\n"); return ; }

    int i;
    for( i = 0; i < objectDescriptors->total; i++ )
    {
        const CvSURFPoint* kp = (const CvSURFPoint*)kreader.ptr;
        const float* descriptor = (const float*)reader.ptr;
        CV_NEXT_SEQ_ELEM( kreader.seq->elem_size, kreader );
        CV_NEXT_SEQ_ELEM( reader.seq->elem_size, reader );
        int nearest_neighbor = naiveNearestNeighbor( descriptor, kp->laplacian, imageKeypoints, imageDescriptors );
        if( nearest_neighbor >= 0 )
        {
            pairs->item[i].p1 = i;
            pairs->item[i].p2 = nearest_neighbor;
        }
    }

    destroyPairList(&pairs);
}



/* a rough implementation for object location */
int
locatePlanarObject( const CvSeq* objectKeypoints, const CvSeq* objectDescriptors,
                    const CvSeq* imageKeypoints, const CvSeq* imageDescriptors,
                    const CvPoint src_corners[4], CvPoint dst_corners[4] )
{

    double h[9];
    CvMat _h = cvMat(3, 3, CV_64F, h);
    // vector<int> ptpairs;
    // vector<CvPoint2D32f> pt1, pt2;
    struct pairList * ptpairs = 0;
    CvMat _pt1, _pt2;
    int i, n;

    findPairs( objectKeypoints, objectDescriptors, imageKeypoints, imageDescriptors, ptpairs );

    n = (int)(ptpairs->currentItems);
    if( n < 4 )
        return 0;

   struct pair * pairA = (struct pair *) malloc(sizeof(struct pair) * n );
   struct pair * pairB = (struct pair *) malloc(sizeof(struct pair) * n );

    for( i = 0; i < n; i+=2 )
    {
       //   pairA[i].p1 = ptpairs[i].p1; //  ((CvSURFPoint*)cvGetSeqElem(objectKeypoints,ptpairs[i*2]))->pt;
       //   pairA[i].p2 = ptpairs[i].p2; //  ((CvSURFPoint*)cvGetSeqElem(objectKeypoints,ptpairs[i*2]))->pt;

       //   pairB[i].p1 = ptpairs[i+1].p1; //  ((CvSURFPoint*)cvGetSeqElem(imageKeypoints,ptpairs[i*2+1]))->pt;
       //   pairB[i].p2 = ptpairs[i+1].p2; //  ((CvSURFPoint*)cvGetSeqElem(imageKeypoints,ptpairs[i*2+1]))->pt;
    }

    _pt1 = cvMat(1, n, CV_32FC2, pairA );
    _pt2 = cvMat(1, n, CV_32FC2, pairB );
    if( !cvFindHomography( &_pt1, &_pt2, &_h, CV_RANSAC, 5 , 0 ))  { return 0; }


    for( i = 0; i < 4; i++ )
    {
        double x = src_corners[i].x, y = src_corners[i].y;
        double Z = 1./(h[6]*x + h[7]*y + h[8]);
        double X = (h[0]*x + h[1]*y + h[2])*Z;
        double Y = (h[3]*x + h[4]*y + h[5])*Z;
        dst_corners[i] = cvPoint(cvRound(X), cvRound(Y));
    }

    return 1;
}

int openCV_SURFDetector(struct Image * pattern,struct Image * img)
{

   StartTimer(FIND_OBJECTS_DELAY);

    IplImage  * image = cvCreateImage( cvSize(img->width,img->height), IPL_DEPTH_8U, img->depth);
    char * opencvImagePointerRetainer = image->imageData; // UGLY HACK
    image->imageData = (char*) img->pixels; // UGLY HACK
    cvCvtColor( image, image, CV_RGB2GRAY);

    IplImage  * object = cvCreateImage( cvSize(pattern->width,pattern->height), IPL_DEPTH_8U, pattern->depth);
    char * opencvObjectPointerRetainer = object->imageData; // UGLY HACK
    object->imageData = (char*) pattern->pixels; // UGLY HACK
    cvCvtColor( object, object, CV_RGB2GRAY);


    CvMemStorage* storage = cvCreateMemStorage(0);
    static CvScalar colors[] = { {{0,0,255}}, {{0,128,255}}, {{0,255,255}}, {{0,255,0}}, {{255,128,0}}, {{255,255,0}}, {{255,0,0}}, {{255,0,255}}, {{255,255,255}} };

    IplImage* object_color = cvCreateImage(cvGetSize(object), 8, 3);
    cvCvtColor( object, object_color, CV_GRAY2BGR );

    CvSeq* objectKeypoints = 0, *objectDescriptors = 0;
    CvSeq* imageKeypoints = 0, *imageDescriptors = 0;
    int i;
    CvSURFParams params = cvSURFParams(500, 1);

    double tt = (double)cvGetTickCount();

    cvExtractSURF( object, 0, &objectKeypoints, &objectDescriptors, storage, params , 0 );
    //printf("Object Descriptors: %d\n", objectDescriptors->total);

    cvExtractSURF( image, 0, &imageKeypoints, &imageDescriptors, storage, params , 0 );
    //printf("Image Descriptors: %d\n", imageDescriptors->total);
    tt = (double)cvGetTickCount() - tt;

    //printf( "Extraction time = %gms\n", tt/(cvGetTickFrequency()*1000.));

    CvPoint src_corners[4] = {{0,0}, {object->width,0}, {object->width, object->height}, {0, object->height}};
    CvPoint dst_corners[4];

    //IplImage* correspond = cvCreateImage( cvSize(image->width, object->height+image->height), 8, 1 );
    //cvSetImageROI( correspond, cvRect( 0, 0, object->width, object->height ) );
    //cvCopy( object, correspond , 0 );
    //cvSetImageROI( correspond, cvRect( 0, object->height, correspond->width, correspond->height ) );
    //cvCopy( image, correspond , 0 );
    //cvResetImageROI( correspond );

    if( locatePlanarObject( objectKeypoints, objectDescriptors, imageKeypoints, imageDescriptors, src_corners, dst_corners ))
    {
        for( i = 0; i < 4; i++ )
        {
            CvPoint r1 = dst_corners[i%4];
            CvPoint r2 = dst_corners[(i+1)%4];
            //cvLine( correspond, cvPoint(r1.x, r1.y+object->height ), cvPoint(r2.x, r2.y+object->height ), colors[8] , 1 ,8 ,0  );
        }
    }

    struct pairList * ptpairs = 0;
    findPairs( objectKeypoints, objectDescriptors, imageKeypoints, imageDescriptors, ptpairs );


    printf(" Found %u pairs \n",(int) ptpairs->currentItems);

    image->imageData = opencvImagePointerRetainer; // UGLY HACK
    cvReleaseImage( &image );

    image->imageData = opencvObjectPointerRetainer; // UGLY HACK
    cvReleaseImage( &object );

    EndTimer(FIND_OBJECTS_DELAY);
/*

    for( i = 0; i < (int)ptpairs->currentItems; i++ )
    {
        CvSURFPoint* r1 = (CvSURFPoint*)cvGetSeqElem( objectKeypoints, ptpairs->item[i].p1; );
        CvSURFPoint* r2 = (CvSURFPoint*)cvGetSeqElem( imageKeypoints,  ptpairs->item[i].p2; );
        cvLine( correspond, cvPointFrom32f(r1->pt),
            cvPoint(cvRound(r2->pt.x), cvRound(r2->pt.y+object->height)), colors[8] );
    }
    cvShowImage( "Object Correspond", correspond );
    for( i = 0; i < objectKeypoints->total; i++ )
    {
        CvSURFPoint* r = (CvSURFPoint*)cvGetSeqElem( objectKeypoints, i );
        CvPoint center;
        int radius;
        center.x = cvRound(r->pt.x);
        center.y = cvRound(r->pt.y);
        radius = cvRound(r->size*1.2/9.*2);
        cvCircle( object_color, center, radius, colors[0], 1, 8, 0 );
    }
    cvShowImage( "Object", object_color );

    cvWaitKey(0);

    cvDestroyWindow("Object");
    cvDestroyWindow("Object Correspond");
*/
    return 1;
}

#endif


int findPatternInImage(struct Image * pattern,struct Image * img)
{
    #if USE_OPENCV_SURF_DETECTOR
      return openCV_SURFDetector(pattern,img);
    #endif
    return 0;
}



