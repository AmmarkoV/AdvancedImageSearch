#include "faceDetection.h"
#include "../configuration.h"



#if USE_OPENCV_FACEDETECTION
#include <cv.h>
#include <cxcore.h>


CvHaarClassifierCascade *cascade=0;
CvMemStorage            *storage=0;
int faceDetectorInitialized=0;


int openCVFaceDetector(struct Image * img)
{
   if (!faceDetectorInitialized)
   {
      char filename[1024]={0};
      strcpy (filename,"/usr/lib/AdvancedImageSearch/haarcascade_frontalface_alt.xml");

      // load the classifier note that I put the file in the same directory with this code */
      cascade = ( CvHaarClassifierCascade* ) cvLoad( filename, 0, 0, 0 );

      // setup memory buffer; needed by the face detector
      storage = cvCreateMemStorage( 0 );
   }


    IplImage  * image = cvCreateImage( cvSize(img->width,img->height), IPL_DEPTH_8U, img->depth);
    char * opencv_pointer_retainer = image->imageData; // UGLY HACK
    image->imageData=(char*) img->pixels; // UGLY HACK

    CvSeq *faces = cvHaarDetectObjects
           (
            image,
            cascade,
            storage,
            1.1,
            3,
            0 //CV_HAAR_DO_CANNY_PRUNNING
            , cvSize( 40, 40 )
            , cvSize( 50, 50 ) // <--- This might have to be commented out if compiled with C++ :P
            );


    image->imageData = opencv_pointer_retainer; // UGLY HACK
    cvReleaseImage( &image );

    return faces->total;

    /* for each face found, draw a red box
    int i;
    for( i = 0 ; i < ( faces ? faces->total : 0 ) ; i++ )
    {
        CvRect *r = ( CvRect* )cvGetSeqElem( faces, i );
       // r->x , r->y  r->width , r->height
    }*/
}
#endif



int imageHasNFaces(struct Image * img)
{
   #if USE_OPENCV_FACEDETECTION
     return openCVFaceDetector(img);
   #endif // USE_OPENCV

   return 0;
}
