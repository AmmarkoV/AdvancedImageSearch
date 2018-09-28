![Advanced Image Search for Linux](https://raw.githubusercontent.com/AmmarkoV/AdvancedImageSearch/master/data/logoSml.jpg)

# Advanced Image Search for Linux
## Enabling content aware image search for linux using computer vision
------------------------------------------------------------------ 
The purpose of this library is to provide command line tools that enable content-aware image browsing from the CLI..

lsimg for example ( which is the first implemented application ) should be able to return images which are larger than a specific resolution , that contain faces , that are similar to another one , that are bright ( histograms ) , that have been taken in a specific Longitude/Latitude ( EXIF ) or the same content as some other image file etc..

You can view implemented functionality by issuing "lsimg - help" 


To submit bug or feature tickets you can use the github issue system
https://github.com/AmmarkoV/AdvancedImageSearch/issues 


## Getting the project
------------------------------------------------------------------ 
To get the project issue 
```
git clone git://github.com/AmmarkoV/AdvancedImageSearch.git
cd AdvancedImageSearch
```

You can also get the project as a zip file by clicking here
https://nodeload.github.com/AmmarkoV/AdvancedImageSearch/zip/master
or 

```
wget https://nodeload.github.com/AmmarkoV/AdvancedImageSearch/zip/master
unzip master
cd AdvancedImageSearch-master
```


## Library Dependencies
------------------------------------------------------------------ 
The program requires libjpeg , libpng , opencv , darknet and gcc to enable all features! It can also operate without problems without darknet and/or opencv but some advanced functionality will not be available.
It also requires exif tool for JPG exif functionality that greatly speeds up some operations when just scanning JPEG headers.
 

To install them issue :
```
sudo apt-get install cmake build-essential git g++ gcc exif libjpeg-dev libpng12-dev
```

Libraries that are not available on repositories can be installed using the following instructions. OpenCV should be a version after version 3.0 which went under a very large refactoring and also contain the nonfree/contrib components. This library is not available in repositories and thus has to be downloaded and built from source something that might be tricky for some users. The semantic search features ( which is a really nice feature ) will also require Darknet to be installed in order to be enabled.

Luckily, all of these dependencies can be installed using the script provided in 3dparty/get_third_party_libs.sh which asks you and then downloads, configures and tries to auto-build the dependencies. 

In order to run the script and assuming you are on the main directory issuing the command :
```
3dparty/get_third_party_libs.sh
```
will download these dependencies and guide you through the procedure, you should also enable the correct CMake switches afterwards. This can be done using console ( cmake -DENABLE_DARKNET=ON -DENABLE_FACEDETECTION=ON -DENABLE_OPENCV=ON -DENABLE_PATTERNRECOGNITION=ON .. )  or a GUI like cmake-gui 


## Building
------------------------------------------------------------------ 
In order to make the project go to the Project root directory and simply issue
```
mkdir build && cd build &&  cmake .. && make
```
It will create a local copy of the binaries, if you wish to make a system persistent installation just issue

```
mkdir build && cd build &&  cmake .. && sudo make install
```




## Examples
------------------------------------------------------------------ 
Sample usage of lsimg :

It will move all images smaller than 640x480 to the subdir small!
```
 mv `lsimg --maxDims 640 480 /home/me/Pictures/Wallpapers/Raw` Small/
```
 
It will move all images suitable for wallpaper ( larger than 1680x1050 ) to the wallpaper dir!
```
 mv `lsimg --minDims 1680 1050 /home/me/Downloads/` /home/me/Pictures/Wallpapers/Raw/
```
 
It will move all images suitable for wallpaper that are "red" ( larger than 1680x1050 ) to the red wallpaper dir!
```
 mv `lsimg --color red --minDims 1680 1050 /home/me/Downloads/` /home/me/Pictures/Wallpapers/Red/
```

Find all images like ~/me.jpg in ~/Downloads direcotry 
```
lsimg --like ~/me.jpg ~/Downloads/
```


Find all images containing the same contents as ~/camel.jpg in ~/Downloads direcotry 
```
lsimg --semantics ~/camel.jpg 10 ~/Downloads/
```

Open all the "green" images with gpicview
```
cd ~/Downloads
lsimg --color green ./ | xargs -n 1 gpicview #
```
 



