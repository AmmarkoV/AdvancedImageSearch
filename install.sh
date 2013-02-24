#!/bin/bash
echo "Copying to system bin directory"
echo "Installation needs root"
sudo echo "Thank you"

make

if [ -d "/usr/lib/AdvancedImageSearch" ]; then
echo "AdvancedImageSearch Library path detected patching it up :)"
else
echo "Installing AdvancedImageSearch in the system.. :)"
  sudo mkdir /usr/lib/AdvancedImageSearch
fi

sudo cp src/AdvancedImageSearchLib/libAdvancedImageSearchLib.so /usr/lib/AdvancedImageSearch/libAdvancedImageSearchLib.so

./makeSystem.sh

sudo cp src/lsimg/lsimg /usr/bin/lsimg 
exit 0
