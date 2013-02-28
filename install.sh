#!/bin/bash
echo "Copying to system bin directory"
echo "Installation needs root"
sudo echo "Thank you"

make

if [ -e "src/AdvancedImageSearchLib/AdvancedImageSearch.so" ]; then
   echo "fixing temporary cbp2make bug"
   cp "src/AdvancedImageSearchLib/AdvancedImageSearch.so" "src/AdvancedImageSearchLib/libAdvancedImageSearch.so" 
fi

 

if [ -e "src/AdvancedImageSearchLib/libAdvancedImageSearch.so" ]; then
   echo "Library is OK" 
else
   echo "The library could not be built , please consider running ./get-dependencies.sh to solve build issues"
   echo "If the problem presists create an issue ticket at https://github.com/AmmarkoV/AdvancedImageSearch/issues" 
   echo "Thank you , and sorry for the inconvinience"
   exit 1
fi
 
if [ -e "src/lsimg/lsimg" ]; then
   echo "lsimg is OK" 
else
   echo "The lsimg binary could not be built , please consider running ./get-dependencies.sh to solve build issues"
   echo "If the problem presists create an issue ticket at https://github.com/AmmarkoV/AdvancedImageSearch/issues" 
   echo "Thank you , and sorry for the inconvinience"
   exit 1 
fi

 
if [ -d "/usr/lib/AdvancedImageSearch" ]; then
echo "AdvancedImageSearch Library path detected patching it up :)"
else
echo "Installing AdvancedImageSearch in the system.. :)"
  sudo mkdir /usr/lib/AdvancedImageSearch
fi

sudo cp src/AdvancedImageSearchLib/libAdvancedImageSearch.so /usr/lib/AdvancedImageSearch/libAdvancedImageSearch.so

sudo cp data/haarcascade_frontalface_alt.xml /usr/lib/AdvancedImageSearch/haarcascade_frontalface_alt.xml


scripts/makeSystem.sh

sudo cp src/lsimg/lsimg /usr/bin/lsimg 
exit 0
