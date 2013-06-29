#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imageResizer.h"
#include "../tools/timers.h"

#define DEBUG_RESIZING 0

unsigned char * resizeImageInternal3Bytes(unsigned char * rgb, unsigned int originalWidth ,unsigned int originalHeight , unsigned int resizeWidth,unsigned int resizeHeight)
{
  if (rgb==0) { fprintf(stderr,"Can not resize null image \n");  return 0; }
  if ( ( resizeHeight>originalHeight ) || (resizeWidth>originalWidth ) ) { /*fprintf(stderr,"Can not increase image size\n");*/ return 0; }
  if ( ( resizeHeight == 0 ) || (resizeWidth == 0 ) ) { fprintf(stderr,"Will not resize to 0  \n"); return 0; }


  unsigned int xBlockCount = (unsigned int) originalWidth  / resizeWidth;
  unsigned int yBlockCount = (unsigned int) originalHeight / resizeHeight;
  unsigned int allBlocks = xBlockCount * yBlockCount;
  unsigned int blockScanline = xBlockCount * 3;

  if ( (xBlockCount==0) || (yBlockCount==0)  )  { fprintf(stderr,"Can not resize with a null block size\n"); return 0; }


  unsigned char * output = (unsigned char *) malloc(resizeWidth*resizeHeight*3);
  if ( output == 0 ) { fprintf(stderr,"Could not allocate image for resizing \n"); return 0; }
  memset(output,0,resizeWidth*resizeHeight*3);


  unsigned int originalScanline = originalWidth * 3;


  #if DEBUG_RESIZING
  fprintf(stderr,"Resizing a %ux%u image to %ux%u \n",originalWidth,originalHeight,resizeWidth,resizeHeight);
  fprintf(stderr,"We have %ux%u blocks\n",xBlockCount,yBlockCount);
  #endif

  unsigned int x=0;
  unsigned int y=0;

  unsigned char * rgbOut = output;
  unsigned char * rgbIn = rgb;
  unsigned char * rgbInLimit = rgb + originalWidth*originalScanline;

  for (y = 0 ;  y<resizeHeight; y++)
  {
    rgbIn = rgb+ (y*yBlockCount) * originalScanline;

    for (x = 0 ;  x<resizeWidth; x++)
    {
      //Sample a group of pixels that will become one pixel!
      unsigned int valueR = 0;
      unsigned int valueG = 0;
      unsigned int valueB = 0;

      unsigned int i=0;
      unsigned int repeatTimes = yBlockCount;

      unsigned char * tmpRGB = rgbIn;
      unsigned char * tmpRGBLimit = rgbIn + blockScanline;

      #if DEBUG_RESIZING
        fprintf(stderr,"Sum on block : %ux%u \n",x,y);
     #endif

      for (i=0; i<repeatTimes; i++)
      {
       #if DEBUG_RESIZING
        fprintf(stderr,"Scanline %u - %p -> %p \n",i,tmpRGB,tmpRGBLimit);
       #endif

        while (tmpRGB<tmpRGBLimit)
        {
          if (rgbInLimit>tmpRGB+3)
          {
           valueR+= (unsigned int) *tmpRGB; ++tmpRGB;
           valueG+= (unsigned int) *tmpRGB; ++tmpRGB;
           valueB+= (unsigned int) *tmpRGB; ++tmpRGB;
          } else
          {
            tmpRGB+=3;
          }

        }

       #if DEBUG_RESIZING
        fprintf(stderr,"raw Sum : R %u G %u B %u \n",valueR,valueG,valueB);
       #endif

        tmpRGB+=originalScanline;
        tmpRGBLimit=tmpRGB + blockScanline;
      }
      //Finished

      valueR/=allBlocks;
      if (valueR>255) { valueR = 255; }

      valueG/=allBlocks;
      if (valueG>255) { valueG = 255; }

      valueB/=allBlocks;
      if (valueB>255) { valueB = 255; }

      #if DEBUG_RESIZING
        fprintf(stderr," R %u G %u B %u \n",valueR,valueG,valueB);
     #endif

      *rgbOut = (unsigned char) valueR; ++rgbOut;
      *rgbOut = (unsigned char) valueG; ++rgbOut;
      *rgbOut = (unsigned char) valueB; ++rgbOut;

      rgbIn+= blockScanline;
    }

    //rgbIn+= originalScanline  * yBlockCount;
  }

  return output;
}




struct Image * resizeImage(struct Image * img,unsigned int resizeWidth , unsigned int resizeHeight )
{
 struct Image * smallerImg  = (struct Image *) malloc(sizeof (struct Image));
 if (smallerImg==0) { fprintf(stderr,"Could not allocate memory for resized image structure\n"); return 0; }

 if (img->channels==3)
 {
   StartTimer(IMAGE_RESIZE_DELAY);
   smallerImg->width = resizeWidth;
   smallerImg->height = resizeHeight;
   smallerImg->channels = img->channels;
   smallerImg->bitsperpixel = img->bitsperpixel;

   smallerImg->pixels = resizeImageInternal3Bytes(img->pixels, img->width, img->height, smallerImg->width , smallerImg->height);

   if (smallerImg->pixels == 0) { free(smallerImg); smallerImg=0; }
   EndTimer(IMAGE_RESIZE_DELAY);
 } else
 {
   free(smallerImg);
   smallerImg=0;
 }

 return smallerImg;
}
