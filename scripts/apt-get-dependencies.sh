#!/bin/bash
echo "This should install all packages that are needed by this program , if you dont have Ubuntu/Debian - apt-get you should install them manualy!"


#Build Dependencies
BUILD_PROJECT="gcc libjpeg-dev libpng12-dev"

#face detection not used yet
FACE_DETECTION=" "
#FACE_DETECTION="libopencv-dev libcv-dev libcvaux-dev libhighgui-dev"

#Runtime Dependencies
EXIF_FUNCTIONALITY="exif" 

sudo apt-get install $BUILD_PROJECT $EXIF_FUNCTIONALITY $FACE_DETECTION 
  
exit 0
