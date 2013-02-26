#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imageResizer.h"

#define DEBUG_RESIZING 1

unsigned char * resizeImageInternal3Bytes(unsigned char * rgb, unsigned int originalWidth ,unsigned int originalHeight , unsigned int resizeWidth,unsigned int resizeHeight)
{
  if ( ( resizeHeight>originalHeight ) || (resizeWidth>originalWidth ) ) { fprintf(stderr,"Can not increase image size\n"); return rgb; }
  if ( ( resizeHeight == 0 ) || (resizeWidth == 0 ) ) { fprintf(stderr,"Will not resize to 0  \n"); return rgb; }
  unsigned char * output = (unsigned char *) malloc(resizeWidth*resizeHeight*3);
  memset(output,0,resizeWidth*resizeHeight*3);

  if ( output == 0 ) { fprintf(stderr,"Could not allocate image for resizing \n"); return rgb; }

  unsigned int xBlockCount = originalWidth  / resizeWidth;
  unsigned int yBlockCount = originalHeight / resizeHeight;


  #if DEBUG_RESIZING
  fprintf(stderr,"Resizing a %ux%u image to %ux%u \n",originalWidth,originalHeight,resizeWidth,resizeHeight);
  fprintf(stderr,"We have %ux%u blocks\n",xBlockCount,yBlockCount);
  #endif

  unsigned int xBlock=0;
  unsigned int yBlock=0;

  unsigned char * rgbOut = output;
  unsigned char * rgbIn = rgb;

  for (yBlock = 0 ;  yBlock<yBlockCount; yBlock++)
  {
    for (xBlock = 0 ;  xBlock<xBlockCount; xBlock++)
    {

      //Sample a group of pixels that will become one pixel!
      unsigned int valueR = 0;
      unsigned int valueG = 0;
      unsigned int valueB = 0;

      unsigned int repeatTimes = yBlockCount;
      unsigned int i=0;

      unsigned char * tmpRGB = rgbIn;
      unsigned char * tmpRGBLimit = rgbIn + xBlockCount*3;

      #if DEBUG_RESIZING
        fprintf(stderr,"Sum on block : %ux%u \n",xBlock,yBlock);
     #endif

      for (i=0; i<repeatTimes; i++)
      {
       #if DEBUG_RESIZING
        fprintf(stderr,"Scanline %u - %p -> %p \n",i,tmpRGB,tmpRGBLimit);
       #endif

        while (tmpRGB<tmpRGBLimit)
        {
         valueR+= (unsigned int) *tmpRGB; ++tmpRGB;
         valueG+= (unsigned int) *tmpRGB; ++tmpRGB;
         valueB+= (unsigned int) *tmpRGB; ++tmpRGB;
        }

       #if DEBUG_RESIZING
        fprintf(stderr,"raw Sum : R %u G %u B %u \n",valueR,valueG,valueB);
       #endif

        tmpRGB+=originalWidth*3;
        tmpRGBLimit=tmpRGB + xBlockCount*3 ;

      }
      //Finished

      valueR/=xBlockCount*yBlockCount;
      if (valueR>255) { valueR = 255; }
      valueG/=xBlockCount*yBlockCount;
      if (valueG>255) { valueG = 255; }
      valueB/=xBlockCount*yBlockCount;
      if (valueB>255) { valueB = 255; }

      #if DEBUG_RESIZING
        fprintf(stderr," R %u G %u B %u \n",valueR,valueG,valueB);
     #endif

      *rgbOut = (unsigned char) valueR; ++rgbOut;
      *rgbOut = (unsigned char) valueG; ++rgbOut;
      *rgbOut = (unsigned char) valueB; ++rgbOut;

      rgbIn+= xBlockCount*3;
    }

    rgbIn+= originalWidth*3*yBlockCount;
  }

  return output;
}




struct Image * resizeImage(struct Image * img,unsigned int resizeWidth , unsigned int resizeHeight )
{
 struct Image * smallerImg  = (struct Image *) malloc(sizeof (struct Image));
 if (smallerImg==0) { fprintf(stderr,"Could not allocate space for smaller image \n"); return img; }

 smallerImg->width = resizeWidth;
 smallerImg->height = resizeHeight;
 smallerImg->depth = img->depth;

 smallerImg->pixels = resizeImageInternal3Bytes(img->pixels, img->width, img->height, smallerImg->width , smallerImg->height);

 return smallerImg;
}


