#!/bin/bash 
echo "This script is supposed to solve all dependencies using pacman"
echo "This script supposes a ARch distro with apt-get as a package manager!"

echo
echo -n "                Do you want to proceed (Y/N)?"
read answer
if test "$answer" != "Y" -a "$answer" != "y";
then exit 0;
fi
 
echo "Starting downloads " 

sudo pacman -s gcc libpng libjpeg-turbo
 
  

exit 0
