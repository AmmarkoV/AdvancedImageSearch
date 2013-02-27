#!/bin/bash
echo "Copying to system bin directory"
echo "Installation needs root"
sudo echo "Thank you"

make

if [ -e "src/AdvancedImageSearchLib/AdvancedImageSearchLib.so" ]; then
   echo "fixing temporary cbp2make bug"
   cp "src/AdvancedImageSearchLib/AdvancedImageSearchLib.so" "src/AdvancedImageSearchLib/libAdvancedImageSearchLib.so" 
fi

 

if [ -e "src/AdvancedImageSearchLib/libAdvancedImageSearchLib.so" ]; then
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

sudo cp src/AdvancedImageSearchLib/libAdvancedImageSearchLib.so /usr/lib/AdvancedImageSearch/libAdvancedImageSearchLib.so

scripts/makeSystem.sh

sudo cp src/lsimg/lsimg /usr/bin/lsimg 
exit 0
