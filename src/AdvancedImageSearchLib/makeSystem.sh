#!/bin/bash

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
  CPU="-fPIC -march=corei7-avx -mtune=corei7-avx -ftree-vectorizer-verbose=1 "
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
#Codecs
#Future Addition gcc  $Optimizations $CPU -c codecs/jpgExiforient_embed.c -o jpgExiforient_embed.o 
gcc  $Optimizations $CPU -c codecs/codecs.c -o codecs.o 
gcc  $Optimizations $CPU -c codecs/jpgExifexternal.c -o jpgExifexternal.o 
gcc  $Optimizations $CPU -c codecs/jpgInput.c -o jpgInput.o 
gcc  $Optimizations $CPU -c codecs/pngInput.c -o pngInput.o 
gcc  $Optimizations $CPU -c codecs/ppmInput.c -o ppmInput.o 
 
echo "Done with Object Files"

FILESTOLINK="main.o string_extension_scanner.o timers.o parameter_parser.o faceDetection.o filters.o findPatterns.o histograms.o imageComparison.o imageResizer.o codecs.o jpgExifexternal.o jpgInput.o pngInput.o ppmInput.o "

g++ -shared $FILESTOLINK -o $TARGET $ALLTHELIBS 

rm $FILESTOLINK

 
if [ -e $TARGET ]
then
  echo "Success building $TARGET.."
else
  echo "Failure building $TARGET.."
fi

exit 0
