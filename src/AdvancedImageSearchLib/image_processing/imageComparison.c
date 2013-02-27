#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imageComparison.h"

int imagesAreSimilar(struct Image * img1,struct Image * img2,unsigned int maxDifferencePerChannel,float differencePercent)
{
 if ( (img1==0) || (img2==0) ) { fprintf(stderr,"Cannot compare images that have not been properly allocated \n"); return 0; }
 if ( (img1->pixels==0) || (img2->pixels==0) ) { fprintf(stderr,"Cannot compare images that do not have a body allocated \n"); return 0; }
 if ( (img1->width!=img2->width) || (img1->height!=img2->height) ) { fprintf(stderr,"Cannot compare images that have different dimensions\n"); return 0; }


 //We have two images of the same size
 unsigned char *rgb1 = img1->pixels;
 unsigned char *rgb1Limit = rgb1 + img1->width * img1->height * img1->depth;
 unsigned char *rgb2 = img2->pixels;

 unsigned int maxDifferentPixels =(unsigned int) (img1->width * img1->height *  (differencePercent/100));
 unsigned int differentPixels=0;
 unsigned int diffR=0;
 unsigned int diffG=0;
 unsigned int diffB=0;
 while (rgb1<rgb1Limit)
   {
     if (*rgb1>=*rgb2) { diffR=*rgb1-*rgb2; } else { diffR=*rgb2-*rgb1; }
     ++rgb1; ++rgb2;
     if (*rgb1>=*rgb2) { diffG=*rgb1-*rgb2; } else { diffG=*rgb2-*rgb1; }
     ++rgb1; ++rgb2;
     if (*rgb1>=*rgb2) { diffB=*rgb1-*rgb2; } else { diffB=*rgb2-*rgb1; }
     ++rgb1; ++rgb2;

    if ( (diffR>=maxDifferencePerChannel) ||
         (diffG>=maxDifferencePerChannel) ||
         (diffB>=maxDifferencePerChannel) )
                                             { ++differentPixels; }
   }



  if (differentPixels<maxDifferentPixels) { return 1; }
  return 0;
}

