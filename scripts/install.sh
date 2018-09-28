#!/bin/bash

red=$(printf "\033[31m")
green=$(printf "\033[32m")
yellow=$(printf "\033[33m")
blue=$(printf "\033[34m")
magenta=$(printf "\033[35m")
cyan=$(printf "\033[36m")
white=$(printf "\033[37m")
normal=$(printf "\033[m")
 
normalChars=$(printf "\033[0m")
boldChars=$(printf "\033[1m")
underlinedChars=$(printf "\033[4m")
blinkingChars=$(printf "\033[5m") 

apologize() 
{
   echo "$red The library $1 could not be built $normal , please consider running ./get-dependencies.sh to solve build issues"
   echo "If the problem presists create an issue ticket at https://github.com/AmmarkoV/AdvancedImageSearch/issues" 
   echo "Thank you , and sorry for the inconvinience"
   exit 1 
}


echo "Copying to system bin directory"
echo "Installation needs root"
sudo echo "Thank you"




make
 
if [ -e "src/AdvancedImageSearchLib/AdvancedImageSearch.so" ]; then
   echo "cbp2make bug detected!"
    if [ -e "src/AdvancedImageSearchLib/libAdvancedImageSearch.so" ]; then
     rm "src/AdvancedImageSearchLib/libAdvancedImageSearch.so"
    fi
   mv "src/AdvancedImageSearchLib/AdvancedImageSearch.so" "src/AdvancedImageSearchLib/libAdvancedImageSearch.so" 
fi
if [ -e "src/PatternRecognition/PatternRecognition.so" ]; then
   echo "cbp2make bug detected!"
    if [ -e "src/PatternRecognition/libPatternRecognition.so" ]; then
     rm "src/PatternRecognition/libPatternRecognition.so"
    fi
   mv "src/PatternRecognition/PatternRecognition.so" "src/PatternRecognition/libPatternRecognition.so" 
fi
 

if [ -e "src/AdvancedImageSearchLib/libAdvancedImageSearch.so" ]; then
   echo "$green Main Library is OK $normal" 
else
   apologize "Main Library"
fi

if [ -e "src/PatternRecognition/libPatternRecognition.so" ]; then
   echo "$green Pattern Library is OK $normal" 
else
   apologize "Pattern Library" 
fi


if [ -d "/usr/lib/AdvancedImageSearch" ]; then
echo "AdvancedImageSearch Library path detected patching it up :)"
else
echo "Installing AdvancedImageSearch in the system.. :)"
  sudo mkdir /usr/lib/AdvancedImageSearch
fi

sudo cp src/PatternRecognition/libPatternRecognition.so /usr/lib/AdvancedImageSearch/libPatternRecognition.so

sudo cp data/haarcascade_frontalface_alt.xml /usr/lib/AdvancedImageSearch/haarcascade_frontalface_alt.xml


#Make a /usr/lib specific .so after sattelite sos are copied:(
CURDIR=`pwd`
cd src/AdvancedImageSearchLib
./makeSystem.sh
cd $CURDIR 

if [ -e "src/AdvancedImageSearchLib/libAdvancedImageSearch.so" ]; then
   echo "$green libAdvancedImageSearch.so is OK $normal" 
else 
   apologize "libAdvancedImageSearch.so"
fi



sudo cp src/AdvancedImageSearchLib/libAdvancedImageSearch.so /usr/lib/AdvancedImageSearch/libAdvancedImageSearch.so


#make a system specific lsimg and cmpimg
scripts/makeSystem.sh

if [ -e "src/lsimg/lsimg" ]; then
   echo "$green lsimg is OK $normal" 
else 
   apologize "lsimg"
fi

if [ -e "src/cmpimg/cmpimg" ]; then
   echo "$green cmpimg is OK $normal" 
else 
   apologize "cmpimg"
fi

#everything is ok , copy to system 
 
sudo cp src/lsimg/lsimg /usr/bin/lsimg 
sudo cp src/cmpimg/cmpimg /usr/bin/cmpimg

 
exit 0
