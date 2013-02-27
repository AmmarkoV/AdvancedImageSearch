#!/bin/bash

cd src/lsimg
gcc main.c -L.  /usr/lib/AdvancedImageSearch/libAdvancedImageSearch.so  -ljpeg -o lsimg
cd ..
cd ..

exit 0
