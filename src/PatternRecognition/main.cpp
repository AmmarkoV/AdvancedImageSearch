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

 #define USE_PATTERN_RECOGNITION 1

#include <iostream>
#include <vector>
#include <stdio.h>

#if USE_PATTERN_RECOGNITION
#include "opencv2/core.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/core/ocl.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/xfeatures2d.hpp"
#endif

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


using namespace cv;
using namespace cv::xfeatures2d;

const int LOOP_NUM = 10;
const int GOOD_PTS_MAX = 50;
const float GOOD_PORTION = 0.15f;

int64 work_begin = 0;
int64 work_end = 0;

static void workBegin()
{
    work_begin = getTickCount();
}

static void workEnd()
{
    work_end = getTickCount() - work_begin;
}

static double getTime()
{
    return work_end /((double)getTickFrequency() )* 1000.;
}

struct SURFDetector
{
    Ptr<Feature2D> surf;
    SURFDetector(double hessian = 800.0)
    {
        surf = SURF::create(hessian);
    }
    template<class T>
    void operator()(const T& in, const T& mask, std::vector<cv::KeyPoint>& pts, T& descriptors, bool useProvided = false)
    {
        surf->detectAndCompute(in, mask, pts, descriptors, useProvided);
    }
};

template<class KPMatcher>
struct SURFMatcher
{
    KPMatcher matcher;
    template<class T>
    void match(const T& in1, const T& in2, std::vector<cv::DMatch>& matches)
    {
        matcher.match(in1, in2, matches);
    }
};

static Mat drawGoodMatches(
    const Mat& img1,
    const Mat& img2,
    const std::vector<KeyPoint>& keypoints1,
    const std::vector<KeyPoint>& keypoints2,
    std::vector<DMatch>& matches,
    std::vector<Point2f>& scene_corners_
    )
{
    //-- Sort matches and preserve top 10% matches
    std::sort(matches.begin(), matches.end());
    std::vector< DMatch > good_matches;
    double minDist = matches.front().distance;
    double maxDist = matches.back().distance;

    const int ptsPairs = std::min(GOOD_PTS_MAX, (int)(matches.size() * GOOD_PORTION));
    for( int i = 0; i < ptsPairs; i++ )
    {
        good_matches.push_back( matches[i] );
    }
    std::cout << "\nMax distance: " << maxDist << std::endl;
    std::cout << "Min distance: " << minDist << std::endl;

    std::cout << "Calculating homography using " << ptsPairs << " point pairs." << std::endl;

    // drawing the results
    Mat img_matches;

    drawMatches( img1, keypoints1, img2, keypoints2,
                 good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
                 std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS  );

    //-- Localize the object
    std::vector<Point2f> obj;
    std::vector<Point2f> scene;

    for( size_t i = 0; i < good_matches.size(); i++ )
    {
        //-- Get the keypoints from the good matches
        obj.push_back( keypoints1[ good_matches[i].queryIdx ].pt );
        scene.push_back( keypoints2[ good_matches[i].trainIdx ].pt );
    }
    //-- Get the corners from the image_1 ( the object to be "detected" )
    std::vector<Point2f> obj_corners(4);
    obj_corners[0] = Point(0,0);
    obj_corners[1] = Point( img1.cols, 0 );
    obj_corners[2] = Point( img1.cols, img1.rows );
    obj_corners[3] = Point( 0, img1.rows );
    std::vector<Point2f> scene_corners(4);

    Mat H = findHomography( obj, scene, RANSAC );
    perspectiveTransform( obj_corners, scene_corners, H);

    scene_corners_ = scene_corners;

    //-- Draw lines between the corners (the mapped object in the scene - image_2 )
    line( img_matches,
          scene_corners[0] + Point2f( (float)img1.cols, 0), scene_corners[1] + Point2f( (float)img1.cols, 0),
          Scalar( 0, 255, 0), 2, LINE_AA );
    line( img_matches,
          scene_corners[1] + Point2f( (float)img1.cols, 0), scene_corners[2] + Point2f( (float)img1.cols, 0),
          Scalar( 0, 255, 0), 2, LINE_AA );
    line( img_matches,
          scene_corners[2] + Point2f( (float)img1.cols, 0), scene_corners[3] + Point2f( (float)img1.cols, 0),
          Scalar( 0, 255, 0), 2, LINE_AA );
    line( img_matches,
          scene_corners[3] + Point2f( (float)img1.cols, 0), scene_corners[0] + Point2f( (float)img1.cols, 0),
          Scalar( 0, 255, 0), 2, LINE_AA );
    return img_matches;
}

////////////////////////////////////////////////////
// This program demonstrates the usage of SURF_OCL.
// use cpu findHomography interface to calculate the transformation matrix
int main(int argc, char* argv[])
{
    const char* keys =
        "{ h help     |                  | print help message  }"
        "{ l left     | box.png          | specify left image  }"
        "{ r right    | box_in_scene.png | specify right image }"
        "{ o output   | SURF_output.jpg  | specify output save path }"
        "{ m cpu_mode |                  | run without OpenCL }";

    CommandLineParser cmd(argc, argv, keys);
    if (cmd.has("help"))
    {
        std::cout << "Usage: surf_matcher [options]" << std::endl;
        std::cout << "Available options:" << std::endl;
        cmd.printMessage();
        return EXIT_SUCCESS;
    }
    if (cmd.has("cpu_mode"))
    {
        ocl::setUseOpenCL(false);
        std::cout << "OpenCL was disabled" << std::endl;
    }

    UMat img1, img2;

    std::string outpath = cmd.get<std::string>("o");

    std::string leftName = cmd.get<std::string>("l");
    imread(leftName, IMREAD_GRAYSCALE).copyTo(img1);
    if(img1.empty())
    {
        std::cout << "Couldn't load " << leftName << std::endl;
        cmd.printMessage();
        return EXIT_FAILURE;
    }

    std::string rightName = cmd.get<std::string>("r");
    imread(rightName, IMREAD_GRAYSCALE).copyTo(img2);
    if(img2.empty())
    {
        std::cout << "Couldn't load " << rightName << std::endl;
        cmd.printMessage();
        return EXIT_FAILURE;
    }

    double surf_time = 0.;

    //declare input/output
    std::vector<KeyPoint> keypoints1, keypoints2;
    std::vector<DMatch> matches;

    UMat _descriptors1, _descriptors2;
    Mat descriptors1 = _descriptors1.getMat(ACCESS_RW),
        descriptors2 = _descriptors2.getMat(ACCESS_RW);

    //instantiate detectors/matchers
    SURFDetector surf;

    SURFMatcher<BFMatcher> matcher;

    //-- start of timing section

    for (int i = 0; i <= LOOP_NUM; i++)
    {
        if(i == 1) workBegin();
        surf(img1.getMat(ACCESS_READ), Mat(), keypoints1, descriptors1);
        surf(img2.getMat(ACCESS_READ), Mat(), keypoints2, descriptors2);
        matcher.match(descriptors1, descriptors2, matches);
    }
    workEnd();
    std::cout << "FOUND " << keypoints1.size() << " keypoints on first image" << std::endl;
    std::cout << "FOUND " << keypoints2.size() << " keypoints on second image" << std::endl;

    surf_time = getTime();
    std::cout << "SURF run time: " << surf_time / LOOP_NUM << " ms" << std::endl<<"\n";


    std::vector<Point2f> corner;
    Mat img_matches = drawGoodMatches(img1.getMat(ACCESS_READ), img2.getMat(ACCESS_READ), keypoints1, keypoints2, matches, corner);

    //-- Show detected matches

    namedWindow("surf matches", 0);
    imshow("surf matches", img_matches);
    imwrite(outpath, img_matches);

    waitKey(0);
    return EXIT_SUCCESS;
}




int detectPatternOpenCV(struct ptrnImage * pattern,struct ptrnImage * img)
{
    if (img==0) {  return 0; }
    if (img->pixels==0) {  return 0; }
    if (pattern==0) { return 0; }
    if (pattern->pixels==0) {  return 0; }



    if ( (img->width < pattern->width) && (img->height < pattern->height) ) { return 0; }


    int result = 0;

    //cv::initModule_nonfree();

    monochrome(img);
    /*
    IplImage  * image = cvCreateImage( cvSize(img->width,img->height), IPL_DEPTH_8U, img->depth);
    char * opencvImagePointerRetainer = image->imageData; // UGLY HACK
    image->imageData = (char*) img->pixels; // UGLY HACK
    */
    cv::Mat imgMat(img->height,img->width,CV_8UC3,(void*) img->pixels , img->depth * img->width * sizeof(unsigned char));

    monochrome(pattern);
    /*
    IplImage  * object = cvCreateImage( cvSize(pattern->width,pattern->height), IPL_DEPTH_8U, pattern->depth);
    char * opencvObjectPointerRetainer = object->imageData; // UGLY HACK
    object->imageData = (char*) pattern->pixels; // UGLY HACK
    */
    cv::Mat patternMat(pattern->height,pattern->width,CV_8UC3,(void*) pattern->pixels , pattern->depth * pattern->width * sizeof(unsigned char));



}






#endif


int detectPattern(struct ptrnImage * pattern,struct ptrnImage * img)
{
  #if USE_PATTERN_RECOGNITION
    return detectPatternOpenCV(pattern,img);
  #endif
  return 0;
}
