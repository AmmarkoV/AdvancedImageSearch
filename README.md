![Advanced Image Search for Linux](https://raw.githubusercontent.com/AmmarkoV/AdvancedImageSearch/master/data/logoSml.jpg)

#Advanced Image Search for Linux
##Enabling content aware image search for linux using computer vision
------------------------------------------------------------------ 
The purpose of this library are command line tools that enable content-aware image browsing from the CLI..

lsimg for example ( which is the first implemented application ) should be able to return images which are larger than a specific resolution , that contain faces , that are similar to another one , that are bright ( histograms ) , that have been taken in a specific Longitude/Latitude ( EXIF ) etc..

Of course some of this functionality is theoretical for now , you can view implemented functionality by issuing "lsimg - help" 


To submit bug or feature tickets you can use the github issue system
https://github.com/AmmarkoV/AdvancedImageSearch/issues 


##Getting the project
------------------------------------------------------------------ 
To get the project issue 
git clone git://github.com/AmmarkoV/AdvancedImageSearch.git
cd AdvancedImageSearch

You can also get the project as a zip file by clicking here
https://nodeload.github.com/AmmarkoV/AdvancedImageSearch/zip/master



##Library Dependencies
------------------------------------------------------------------ 
Requires libjpeg , libpng , opencv and gcc to be compiled!
OpenCV should be a version after 2.4 since nonfree components of it got seperated from the main library and we use them in this lib..
In case you run into problems with lib dependencies or don't want them at all in your system consider deactivating the features you don't want by switching them off from src/AdvancedImageSearchLib/configuration.h

Requires exif tool ( sudo apt-get install exif ) for JPG exif functionality
The project contains a script to solve dependencies using pacman or apt-get as package managers
You can perform automatic download of required dependencies by issuing : ./get-dependencies.sh



## Building
------------------------------------------------------------------ 
In order to make the project simply issue
"mkdir build && cd build &&  cmake .. && make" from the root directory
It will create a local copy of the binaries



##Installation of current version
------------------------------------------------------------------ 
In order to install lsimg and the AdvancedImageSearch to your system you can do it
issuing 
./install.sh
your password is required.. 

In order to remove the project you can use the uninstall script by issuing
./uninstall.sh


##Installation of OpenCV3.2 and Darknet
------------------------------------------------------------------ 
In order to install dependencies you can use the script 3dparty/get_third_party_libs.sh 


##Examples
------------------------------------------------------------------ 
Sample usage of lsimg :

It will move all images smaller than 640x480 to the subdir small!
 mv `lsimg --maxDims 640 480 /home/me/Pictures/Wallpapers/Raw` Small/
 
It will move all images suitable for wallpaper ( larger than 1680x1050 ) to the wallpaper dir!
 mv `lsimg --minDims 1680 1050 /home/me/Downloads/` /home/me/Pictures/Wallpapers/Raw/
 
It will move all images suitable for wallpaper that are "red" ( larger than 1680x1050 ) to the red wallpaper dir!
 mv `lsimg --color red --minDims 1680 1050 /home/me/Downloads/` /home/me/Pictures/Wallpapers/Red/

Find all images like ~/me.jpg in ~/Downloads direcotry 
lsimg --like ~/me.jpg ~/Downloads/


Find all images containing the same contents as ~/camel.jpg in ~/Downloads direcotry 
lsimg --semantics ~/camel.jpg 10 ~/Downloads/

Open all the "green" images with gpicview
cd ~/Downloads
lsimg --color green ./ | xargs -n 1 gpicview #
 



