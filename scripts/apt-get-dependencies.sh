#!/bin/bash
echo "This should install all packages that are needed by this program , if you dont have Ubuntu/Debian - apt-get you should install them manualy!"


#Build Dependencies
BUILD_PROJECT="gcc libjpeg-dev libpng12-dev"
 
OPENCV_USED=`cat src/AdvancedImageSearchLib/configuration.h | grep "#define USE_OPENCV_FACEDETECTION 1"` 
FACE_DETECTION=" "
if [ -z "$OPENCV_USED" ] 
then
 echo "OpenCV not configured for use "   
else
 FACE_DETECTION="libcv-dev"
fi 
#FACE_DETECTION="libopencv-dev libcv-dev libcvaux-dev libhighgui-dev"

#Runtime Dependencies
EXIF_FUNCTIONALITY="exif" 

sudo apt-get install $BUILD_PROJECT $EXIF_FUNCTIONALITY $FACE_DETECTION 
  
exit 0
