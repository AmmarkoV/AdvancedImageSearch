#include "histograms.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Histogram *  generateHistogram(char * rgb , unsigned int width , unsigned int height , unsigned int channels )
{

   char * rgbPTR = rgb;
   char * rgbLimit = rgb+width*height*channels;
   struct Histogram *  hist =  (struct Histogram * ) malloc (sizeof(struct Histogram));
   if (hist==0) { return 0; }
   memset(hist , 0 , sizeof(struct Histogram));

   if (channels==1)
   {
    while (rgbPTR<rgbLimit)
     {
        ++hist->channel[0].intensity[(unsigned int) *rgbPTR];
        ++rgbPTR;
     }
   } else
   if (channels==3)
   {
    while (rgbPTR<rgbLimit)
     {
        unsigned int rgbPTRIndex = *rgbPTR;
        fprintf(stderr,"Accessing %u ",rgbPTRIndex);
        hist->channel[0].intensity[rgbPTRIndex]+=1;

        ++rgbPTR;
        rgbPTRIndex = *rgbPTR;
        fprintf(stderr,"Accessing %u ",rgbPTRIndex);
        hist->channel[1].intensity[rgbPTRIndex]+=1;

        ++rgbPTR;
        rgbPTRIndex = *rgbPTR;
        fprintf(stderr,"Accessing %u ",rgbPTRIndex);
        hist->channel[2].intensity[rgbPTRIndex]+=1;

        ++rgbPTR;
     }
   }
    else
    {
       fprintf(stderr,"Cannot make histogram for %u channels \n",channels);
       free(hist);
       return 0;
    }

   return hist;
}


int histogramIsCloseToColor(struct Histogram * hist,char R,char G,char B,char Deviation,unsigned int imageSize,float targetPercentage)
{
  unsigned char minR = R;
  unsigned char maxR = R;
  unsigned char minG = G;
  unsigned char maxG = G;
  unsigned char minB = B;
  unsigned char maxB = B;

  if (minR>=Deviation) { minR-=Deviation; } else { minR=0; }
  if (minG>=Deviation) { minG-=Deviation; } else { minG=0; }
  if (minB>=Deviation) { minB-=Deviation; } else { minB=0; }

  if (maxR+Deviation<255) { maxR+=Deviation; } else { maxR=255; }
  if (maxG+Deviation<255) { maxG+=Deviation; } else { maxG=255; }
  if (maxB+Deviation<255) { maxB+=Deviation; } else { maxB=255; }

  unsigned long thresR = 0;
  unsigned long thresG = 0;
  unsigned long thresB = 0;

  unsigned int i=0;
   fprintf(stderr,"histogramIsCloseToColor \n");
  for (i=0; i<255; i++) { if ( ( minR<=i ) && (i<=maxR) ) { thresR+=hist->channel[0].intensity[i]; }  }
  for (i=0; i<255; i++) { if ( ( minG<=i ) && (i<=maxG) ) { thresG+=hist->channel[1].intensity[i]; }  }
  for (i=0; i<255; i++) { if ( ( minB<=i ) && (i<=maxB) ) { thresB+=hist->channel[2].intensity[i]; }  }

   fprintf(stderr,"survived loops \n");

  if (thresR+thresG+thresB == 0) { return 0; }

  float ourPercentage = (float) imageSize / ((float) thresR+thresG+thresB/3) ;

   fprintf(stderr,"the end\n");
  if (ourPercentage>targetPercentage) { return 1; }

  return 0;
}


