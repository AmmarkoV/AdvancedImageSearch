#!/bin/bash
echo "Copying to system bin directory"

sudo mkdir /usr/lib/AdvancedImageSearch
sudo cp src/AdvancedImageSearchLib/libAdvancedImageSearchLib.so /usr/lib/AdvancedImageSearch/libAdvancedImageSearchLib.so

./makeSystem.sh

sudo cp src/lsimg/lsimg /bin/lsimg 
exit 0
