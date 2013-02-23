#!/bin/bash

cd src/lsimg
gcc main.c -L.  /usr/lib/AdvancedImageSearch/libAdvancedImageSearchLib.so  -ljpeg -o lsimg


exit 0
