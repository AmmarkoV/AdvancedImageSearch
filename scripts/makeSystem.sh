#!/bin/bash

IMAGE_LIBS="-ljpeg -lpng"


OPENCV_USED=`cat src/AdvancedImageSearchLib/configuration.h | grep "#define USE_OPENCV_FACEDETECTION 1"` 
OPENCVSTUFF=" "
if [ -z "$OPENCV_USED" ] 
then
 echo "OpenCV not configured for use "   
else
 OPENCVSTUFF="`pkg-config --cflags --libs opencv`"
fi

ALLTHELIBS="$OPENCVSTUFF $IMAGE_LIBS"


cd src/lsimg
gcc main.c -L.  /usr/lib/AdvancedImageSearch/libAdvancedImageSearch.so $ALLTHELIBS -o lsimg
cd ..
cd ..

exit 0
