#!/bin/bash
 
clear 
echo "This script will uninstall this fantastic libary :(  "
echo "Are you sure ?"
echo
echo -n "                Do you want to proceed (Y/N)?"
read answer
if test "$answer" != "Y" -a "$answer" != "y";
then 
 echo "A wise choice :)"
 exit 0;
fi

 
#first step remove the executables by hand

  if [ -e "/usr/lib/AdvancedImageSearch/libAdvancedImageSearchLib.so"  ]
  then
    sudo rm  "/usr/lib/AdvancedImageSearch/libAdvancedImageSearchLib.so"  
  else  
    echo "No AdvancedImageSearch Library detected "
  fi
   
  if [ -e "/usr/bin/lsimg"  ]
  then
    sudo rm  /usr/bin/lsimg  
  else  
    echo "No lsimg executable detected "
  fi
  
#second step remove the directories automagically 

if [ -d "/usr/lib/AdvancedImageSearch" ]; then
  echo "Removing library directory  " 
  sudo rm -rf "/usr/lib/AdvancedImageSearch" 
else 
  echo "No Installed Program Libraries found"
fi 

 
  echo "Done" 
exit 0
