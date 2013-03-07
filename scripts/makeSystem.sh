#!/bin/bash

IMAGE_LIBS="-ljpeg -lpng"

FACEDETECTOR_USED=`cat src/AdvancedImageSearchLib/configuration.h | grep "#define USE_OPENCV_FACEDETECTION 1"` 
PATTERN_RECO_USED=`cat src/AdvancedImageSearchLib/configuration.h | grep "#define USE_PATTERN_RECOGNITION 1"` 
LINKOPENCV=""

PATTERNSTUFF=" "
OPENCVSTUFF=" "


if [ -z "$FACEDETECTOR_USED" ] 
then
 echo "No Face Detection configuration"   
else
 LINKOPENCV="YES"
fi

if [ -z "$PATTERN_RECO_USED" ] 
then
 echo "No Object Detection configuration"   
else
 LINKOPENCV="YES"
 PATTERNSTUFF="/usr/lib/AdvancedImageSearch/libPatternRecognition.so"
fi

if [ -z "$LINKOPENCV" ] 
then
 echo "OpenCV not configured for use "   
else   
  OPENCVSTUFF="`pkg-config --cflags --libs opencv` /usr/local/lib/libopencv_nonfree.so" 
fi

 
ALLTHELIBS="$IMAGE_LIBS $OPENCVSTUFF $PATTERNSTUFF"


cd src/lsimg
gcc main.c -L.  /usr/lib/AdvancedImageSearch/libAdvancedImageSearch.so $ALLTHELIBS -o lsimg
cd ..
cd ..

cd src/cmpimg
gcc cmpimg.c -L.  /usr/lib/AdvancedImageSearch/libAdvancedImageSearch.so $ALLTHELIBS -o cmpimg
cd ..
cd ..

exit 0
