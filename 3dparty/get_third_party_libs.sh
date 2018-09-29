#!/bin/bash

STARTDIR=`pwd`
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$DIR"

#BINARIES_THAT_NEED_LIBS="grabber viewer grabber_mux grabber_segment acquisitionBroadcast editor redist openni1_acquisition_shared_library openni2_acquisition_shared_library libfreenect_acquisition_shared_library "


ONIFOLDER64="x64-Release"
ONIFOLDER="x86-Release"
MACHINE_TYPE=`uname -m`
if [ ${MACHINE_TYPE} == 'x86_64' ]; then
echo "Will pick 64bit binaries"
ONIFOLDER=$ONIFOLDER64
else
echo "Will pick 32bit binaries"
#CUDA_VER SHOULD ALREADY BE SET TO $CUDA_VER32
fi


# ---------------------------------------------------------------------
if [ -d darknet ]
then
echo "darknet appears to already exist .."
else
  echo "Do you want to download darknet ? " 
  echo
  echo -n " (Y/N)?"
  read answer
  if test "$answer" != "N" -a "$answer" != "n";
  then  
      cd "$DIR"
      git clone https://github.com/pjreddie/darknet
      cd darknet
      wget https://pjreddie.com/media/files/darknet19.weights 
      ln -s data/imagenet.shortnames.list data/imagenet.names
      
      make
      
      cd ..
  fi
fi
 
# ---------------------------------------------------------------------
if [ -d opencv-3.2.0 ]
then
echo "OpenCV3.2 appears to already exist .."
else
  echo "Do you want to download OpenCV3.2 ? " 
  echo
  echo -n " (Y/N)?"
  read answer
  if test "$answer" != "N" -a "$answer" != "n";
  then  
      cd "$DIR"
       ./getOpenCV3.2.sh
      cd ..
  fi
fi

 
cd "$STARTDIR"

exit 0
