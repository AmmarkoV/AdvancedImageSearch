#!/bin/bash

#Ok this is kind of lame , due to CBP2MAKE difficulties I have a manual make script for installations
#so that things will point in the right directions , I should probably make a proper make script some time :P
#or learn how to configure CBP2MAKE

 

red=$(printf "\033[31m")
green=$(printf "\033[32m")
yellow=$(printf "\033[33m")
blue=$(printf "\033[34m")
magenta=$(printf "\033[35m")
cyan=$(printf "\033[36m")
white=$(printf "\033[37m")
normal=$(printf "\033[m")
 
normalChars=$(printf "\033[0m")
boldChars=$(printf "\033[1m")
underlinedChars=$(printf "\033[4m")
blinkingChars=$(printf "\033[5m") 



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
 echo "OpenCV will not be used"   
else   
  OPENCVSTUFF="`pkg-config --cflags --libs opencv` /usr/local/lib/libopencv_nonfree.so" 
fi

CPU="-s -O3 -fexpensive-optimizations -march=native -mtune=native"
ALLTHELIBS="$IMAGE_LIBS $OPENCVSTUFF $PATTERNSTUFF"


cd src/lsimg
gcc main.c -L.  /usr/lib/AdvancedImageSearch/libAdvancedImageSearch.so $CPU $ALLTHELIBS -o lsimg
cd ..
cd ..

cd src/cmpimg
gcc cmpimg.c -L.  /usr/lib/AdvancedImageSearch/libAdvancedImageSearch.so $CPU $ALLTHELIBS -o cmpimg
cd ..
cd ..


exit 0
