#!/bin/bash


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



TARGET="libAdvancedImageSearch.so"

if [ -e $TARGET ]
then
  rm $TARGET
fi 
  

IMAGE_LIBS="-ljpeg -lpng"

FACEDETECTOR_USED=`cat configuration.h | grep "#define USE_OPENCV_FACEDETECTION 1"` 
PATTERN_RECO_USED=`cat configuration.h | grep "#define USE_PATTERN_RECOGNITION 1"` 
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
  OPENCVSTUFF="`pkg-config --libs opencv` /usr/local/lib/libopencv_nonfree.so" 
fi

 
ALLTHELIBS="$IMAGE_LIBS $OPENCVSTUFF $PATTERNSTUFF"

 

OpenCVStuff="`pkg-config --cflags opencv` `pkg-config --libs opencv`"
Optimizations="-O3 -fexpensive-optimizations -s"
#CPU="-mfpmath=sse -mtune=core2 -msse -msse2 -msse3"
#CPU="-march=corei7-avx"
CPU=" "


if cat /proc/cpuinfo | grep avx
then
  echo "We appear to be running on an AVX enabled CPU , lets use it"
  CPU="-fPIC -march=corei7-avx -mtune=corei7-avx "
else
  CPU="-fPIC -march=native -mtune=native" 
fi
 

gcc  $Optimizations $CPU -c main.c -o main.o 
#Tools
gcc  $Optimizations $CPU -c tools/string_extension_scanner.c -o string_extension_scanner.o
gcc  $Optimizations $CPU -c tools/timers.c -o timers.o
gcc  $Optimizations $CPU -c tools/parameter_parser.c -o parameter_parser.o
#Image Processing
gcc  $Optimizations $CPU $OpenCVStuff -c image_processing/faceDetection.c -o faceDetection.o
gcc  $Optimizations $CPU -c image_processing/filters.c -o filters.o
gcc  $Optimizations $CPU $PATTERNSTUFF -c image_processing/findPatterns.c -o findPatterns.o
gcc  $Optimizations $CPU -c image_processing/histograms.c -o histograms.o
gcc  $Optimizations $CPU -c image_processing/imageComparison.c -o imageComparison.o
gcc  $Optimizations $CPU -c image_processing/imageResizer.c -o imageResizer.o 
#Image Processing
gcc  $Optimizations $CPU -c caching/resultbank.c -o resultbank.o
#Codecs
#Future Addition gcc  $Optimizations $CPU -c codecs/jpgExiforient_embed.c -o jpgExiforient_embed.o 
gcc  $Optimizations $CPU -c codecs/codecs.c -o codecs.o 
gcc  $Optimizations $CPU -c codecs/jpgExifexternal.c -o jpgExifexternal.o 
gcc  $Optimizations $CPU -c codecs/jpgInput.c -o jpgInput.o 
gcc  $Optimizations $CPU -c codecs/pngInput.c -o pngInput.o 
gcc  $Optimizations $CPU -c codecs/ppmInput.c -o ppmInput.o 
 

FILESTOLINK="main.o string_extension_scanner.o timers.o parameter_parser.o faceDetection.o filters.o findPatterns.o histograms.o imageComparison.o imageResizer.o resultbank.o codecs.o jpgExifexternal.o jpgInput.o pngInput.o ppmInput.o "

g++ -shared $FILESTOLINK -o $TARGET $ALLTHELIBS 

rm $FILESTOLINK

 
if [ -e $TARGET ]
then
  echo "$green Success building $TARGET.. $normal"
else
  echo "$red Failure building $TARGET.. $normal"
fi

exit 0
