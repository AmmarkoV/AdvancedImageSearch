#!/bin/bash

IMAGE_LIBS="-ljpeg -lpng"


cd src/lsimg
gcc main.c -L.  /usr/lib/AdvancedImageSearch/libAdvancedImageSearch.so $IMAGE_LIBS -o lsimg
cd ..
cd ..

exit 0
