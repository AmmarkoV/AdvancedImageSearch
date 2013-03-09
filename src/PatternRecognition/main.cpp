// The functions contained in this file are pretty dummy
// and are included only as a placeholder. Nevertheless,
// they *will* get included in the shared library if you
// don't remove them :)
//
// Obviously, you 'll have to write yourself the super-duper
// functions to include in the resulting library...
// Also, it's not necessary to write every function in this file.
// Feel free to add more files in this project. They will be
// included in the resulting library.
/*
 * A Demo to OpenCV Implementation of SURF
 * Further Information Refer to "SURF: Speed-Up Robust Feature"
 * Author: Liu Liu
 * liuliu.1987+opencv@gmail.com
 */

//This is the main configuration file which controls USE_PATTERN_RECOGNITION switch
 #include "../AdvancedImageSearchLib/configuration.h"


#if USE_PATTERN_RECOGNITION
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#endif

#include <iostream>
#include <vector>
#include <stdio.h>
#include "PatternRecognition.h"

using namespace std;

typedef unsigned char BYTE;

int monochrome(struct ptrnImage * img)
{
 if (img==0) { fprintf(stderr,"Function Monochrome given Null Image\n"); return 0; }
 if (img->pixels==0) { fprintf(stderr,"Function Monochrome given Null Image\n"); return 0; }
 if (img->depth==1) { /*Already Monochrome */return 1; }
 if (img->depth!=3) { fprintf(stderr,"Function Monochrome assumes 3byte array\n"); return 0; }


 BYTE * input_frame = img->pixels;
 unsigned int col_med;
 unsigned int image_size= img->width * img->height * img->depth;

 register BYTE *out_px = (BYTE *) input_frame;
 register BYTE *end_px = (BYTE *) input_frame + image_size;
 register BYTE *px = (BYTE *) input_frame;
 register BYTE *r;
 register BYTE *g;
 register BYTE *b;

 while ( px < end_px)
 {
       r = px++; g = px++; b = px++;
       col_med= ( *r + *g + *b )/3;
       *out_px= (BYTE)col_med ;
       ++out_px;
 }
 img->depth = 1;
 return 1;
}


#if USE_PATTERN_RECOGNITION

// define whether to use approximate nearest-neighbor search
#define USE_FLANN

#ifdef USE_FLANN
static void
flannFindPairs( const CvSeq*, const CvSeq* objectDescriptors,
           const CvSeq*, const CvSeq* imageDescriptors, vector<int>& ptpairs )
{
    int length = (int)(objectDescriptors->elem_size/sizeof(float));

    cv::Mat m_object(objectDescriptors->total, length, CV_32F);
    cv::Mat m_image(imageDescriptors->total, length, CV_32F);


    // copy descriptors
    CvSeqReader obj_reader;
    float* obj_ptr = m_object.ptr<float>(0);
    cvStartReadSeq( objectDescriptors, &obj_reader );
    for(int i = 0; i < objectDescriptors->total; i++ )
    {
        const float* descriptor = (const float*)obj_reader.ptr;
        CV_NEXT_SEQ_ELEM( obj_reader.seq->elem_size, obj_reader );
        memcpy(obj_ptr, descriptor, length*sizeof(float));
        obj_ptr += length;
    }
    CvSeqReader img_reader;
    float* img_ptr = m_image.ptr<float>(0);
    cvStartReadSeq( imageDescriptors, &img_reader );
    for(int i = 0; i < imageDescriptors->total; i++ )
    {
        const float* descriptor = (const float*)img_reader.ptr;
        CV_NEXT_SEQ_ELEM( img_reader.seq->elem_size, img_reader );
        memcpy(img_ptr, descriptor, length*sizeof(float));
        img_ptr += length;
    }

    // find nearest neighbors using FLANN
    cv::Mat m_indices(objectDescriptors->total, 2, CV_32S);
    cv::Mat m_dists(objectDescriptors->total, 2, CV_32F);
    cv::flann::Index flann_index(m_image, cv::flann::KDTreeIndexParams(4));  // using 4 randomized kdtrees
    flann_index.knnSearch(m_object, m_indices, m_dists, 2, cv::flann::SearchParams(64) ); // maximum number of leafs checked

    int* indices_ptr = m_indices.ptr<int>(0);
    float* dists_ptr = m_dists.ptr<float>(0);
    for (int i=0;i<m_indices.rows;++i) {
        if (dists_ptr[2*i]<0.6*dists_ptr[2*i+1]) {
            ptpairs.push_back(i);
            ptpairs.push_back(indices_ptr[2*i]);
        }
    }
}
#else

static double
compareSURFDescriptors( const float* d1, const float* d2, double best, int length )
{
    double total_cost = 0;
    assert( length % 4 == 0 );
    for( int i = 0; i < length; i += 4 )
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

static int
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

static void
findPairs( const CvSeq* objectKeypoints, const CvSeq* objectDescriptors,
           const CvSeq* imageKeypoints, const CvSeq* imageDescriptors, vector<int>& ptpairs )
{
    int i;
    CvSeqReader reader, kreader;
    cvStartReadSeq( objectKeypoints, &kreader );
    cvStartReadSeq( objectDescriptors, &reader );
    ptpairs.clear();

    for( i = 0; i < objectDescriptors->total; i++ )
    {
        const CvSURFPoint* kp = (const CvSURFPoint*)kreader.ptr;
        const float* descriptor = (const float*)reader.ptr;
        CV_NEXT_SEQ_ELEM( kreader.seq->elem_size, kreader );
        CV_NEXT_SEQ_ELEM( reader.seq->elem_size, reader );
        int nearest_neighbor = naiveNearestNeighbor( descriptor, kp->laplacian, imageKeypoints, imageDescriptors );
        if( nearest_neighbor >= 0 )
        {
            ptpairs.push_back(i);
            ptpairs.push_back(nearest_neighbor);
        }
    }
}
#endif

/* a rough implementation for object location */
static int
locatePlanarObject( const CvSeq* objectKeypoints, const CvSeq* objectDescriptors,
                    const CvSeq* imageKeypoints, const CvSeq* imageDescriptors,
                    const CvPoint src_corners[4], CvPoint dst_corners[4] )
{
    double h[9];
    CvMat _h = cvMat(3, 3, CV_64F, h);
    vector<int> ptpairs;
    vector<CvPoint2D32f> pt1, pt2;
    CvMat _pt1, _pt2;
    int i, n;

#ifdef USE_FLANN
    flannFindPairs( objectKeypoints, objectDescriptors, imageKeypoints, imageDescriptors, ptpairs );
#else
    findPairs( objectKeypoints, objectDescriptors, imageKeypoints, imageDescriptors, ptpairs );
#endif

    n = (int)(ptpairs.size()/2);
    if( n < 4 )
        return 0;

    pt1.resize(n);
    pt2.resize(n);
    for( i = 0; i < n; i++ )
    {
        pt1[i] = ((CvSURFPoint*)cvGetSeqElem(objectKeypoints,ptpairs[i*2]))->pt;
        pt2[i] = ((CvSURFPoint*)cvGetSeqElem(imageKeypoints,ptpairs[i*2+1]))->pt;
    }

    _pt1 = cvMat(1, n, CV_32FC2, &pt1[0] );
    _pt2 = cvMat(1, n, CV_32FC2, &pt2[0] );
    if( !cvFindHomography( &_pt1, &_pt2, &_h, CV_RANSAC, 5 ))
        return 0;

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

int detectPatternOpenCV(struct ptrnImage * pattern,struct ptrnImage * img)
{
    if (img==0) {  return 0; }
    if (img->pixels==0) {  return 0; }
    if (pattern==0) { return 0; }
    if (pattern->pixels==0) {  return 0; }



    if ( (img->width < pattern->width) && (img->height < pattern->height) ) { return 0; }


    int result = 0;

    cv::initModule_nonfree();

    monochrome(img);
    IplImage  * image = cvCreateImage( cvSize(img->width,img->height), IPL_DEPTH_8U, img->depth);
    char * opencvImagePointerRetainer = image->imageData; // UGLY HACK
    image->imageData = (char*) img->pixels; // UGLY HACK

    monochrome(pattern);
    IplImage  * object = cvCreateImage( cvSize(pattern->width,pattern->height), IPL_DEPTH_8U, pattern->depth);
    char * opencvObjectPointerRetainer = object->imageData; // UGLY HACK
    object->imageData = (char*) pattern->pixels; // UGLY HACK



    CvMemStorage* storage = cvCreateMemStorage(0);

    //cvNamedWindow("Object", 1);
    //cvNamedWindow("Object Correspond", 1);

    static CvScalar colors[] =
    {
        {{0,0,255}},
        {{0,128,255}},
        {{0,255,255}},
        {{0,255,0}},
        {{255,128,0}},
        {{255,255,0}},
        {{255,0,0}},
        {{255,0,255}},
        {{255,255,255}}
    };

    IplImage* object_color = cvCreateImage(cvGetSize(object), 8, 3);
    cvCvtColor( object, object_color, CV_GRAY2BGR );

    CvSeq* objectKeypoints = 0, *objectDescriptors = 0;
    CvSeq* imageKeypoints = 0, *imageDescriptors = 0;


    CvSURFParams params = cvSURFParams(500, 1);

    double tt = (double)cvGetTickCount();
    cvExtractSURF( object, 0, &objectKeypoints, &objectDescriptors, storage, params );
   // printf("Object Descriptors: %d\n", objectDescriptors->total);

    cvExtractSURF( image, 0, &imageKeypoints, &imageDescriptors, storage, params );
    //printf("Image Descriptors: %d\n", imageDescriptors->total);
    tt = (double)cvGetTickCount() - tt;

    //printf( "Extraction time = %gms\n", tt/(cvGetTickFrequency()*1000.));

    CvPoint src_corners[4] = {{0,0}, {object->width,0}, {object->width, object->height}, {0, object->height}};
    CvPoint dst_corners[4];

    //IplImage* correspond = cvCreateImage( cvSize(image->width, object->height+image->height), 8, 1 );
    //cvSetImageROI( correspond, cvRect( 0, 0, object->width, object->height ) );
    //cvCopy( object, correspond );
    //cvSetImageROI( correspond, cvRect( 0, object->height, correspond->width, correspond->height ) );
    //cvCopy( image, correspond );
    //cvResetImageROI( correspond );

#ifdef USE_FLANN
    //printf("Using approximate nearest neighbor search\n");
#endif

    if( locatePlanarObject( objectKeypoints, objectDescriptors, imageKeypoints,
        imageDescriptors, src_corners, dst_corners ))
    {
        result=1;
       /* for( i = 0; i < 4; i++ )
        {
            CvPoint r1 = dst_corners[i%4];
            CvPoint r2 = dst_corners[(i+1)%4];
            cvLine( correspond, cvPoint(r1.x, r1.y+object->height ),
                cvPoint(r2.x, r2.y+object->height ), colors[8] );
        }*/
    }
    vector<int> ptpairs;
#ifdef USE_FLANN
    flannFindPairs( objectKeypoints, objectDescriptors, imageKeypoints, imageDescriptors, ptpairs );
#else
    findPairs( objectKeypoints, objectDescriptors, imageKeypoints, imageDescriptors, ptpairs );
#endif



/*
    for( i = 0; i < (int)ptpairs.size(); i += 2 )
    {
        CvSURFPoint* r1 = (CvSURFPoint*)cvGetSeqElem( objectKeypoints, ptpairs[i] );
        CvSURFPoint* r2 = (CvSURFPoint*)cvGetSeqElem( imageKeypoints, ptpairs[i+1] );
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

    image->imageData = opencvImagePointerRetainer; // UGLY HACK
    cvReleaseImage( &image );

    object->imageData = opencvObjectPointerRetainer; // UGLY HACK
    cvReleaseImage( &object );


    return result;
}

#endif


int detectPattern(struct ptrnImage * pattern,struct ptrnImage * img)
{
  #if USE_PATTERN_RECOGNITION
    return detectPatternOpenCV(pattern,img);
  #endif
  return 0;
}
