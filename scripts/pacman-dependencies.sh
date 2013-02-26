#!/bin/bash
echo "This script is a stub.. if someone has arch please tell me the deps of the project to include them"
exit 1


echo "This script is supposed to solve all dependencies "
echo "This script supposes a ARch distro with apt-get as a package manager!"

echo
echo -n "                Do you want to proceed (Y/N)?"
read answer
if test "$answer" != "Y" -a "$answer" != "y";
then exit 0;
fi
 
echo "Starting downloads " 

sudo pacman -s gcc libjpeg-dev libpng-dev
 
  

exit 0
