#include "histograms.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void cleanHistogram ( struct Histogram * hist )
{
   //memset(hist , 0 , sizeof(struct Histogram));
    unsigned int i =0;
    for (i=0; i<255; i++) { hist->channel[0].intensity[i]=0;  }
    for (i=0; i<255; i++) { hist->channel[1].intensity[i]=0;  }
    for (i=0; i<255; i++) { hist->channel[2].intensity[i]=0;  }
}



struct Histogram *  generateHistogram(char * rgb , unsigned int width , unsigned int height , unsigned int channels )
{

   char * rgbPTR = rgb;
   char * rgbLimit = rgb+width*height*channels;
   struct Histogram *  hist =  (struct Histogram * ) malloc (sizeof(struct Histogram));
   if (hist==0) { return 0; }
   cleanHistogram(hist);


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
    unsigned int overlaps = 0;
    unsigned int rgbPTRIndex=0;
    while (rgbPTR<rgbLimit)
     {
        rgbPTRIndex = 0 + (unsigned int) *rgbPTR;
        if (rgbPTRIndex>255) { rgbPTRIndex=255; ++overlaps;  }
        hist->channel[0].intensity[rgbPTRIndex]+=1;

        ++rgbPTR;
        rgbPTRIndex = 0 + (unsigned int) *rgbPTR;
        if (rgbPTRIndex>255) { rgbPTRIndex=255; ++overlaps; }
        hist->channel[1].intensity[rgbPTRIndex]+=1;

        ++rgbPTR;
        rgbPTRIndex = 0 + (unsigned int) *rgbPTR;
        if (rgbPTRIndex>255) { rgbPTRIndex=255; ++overlaps;  }
        hist->channel[2].intensity[rgbPTRIndex]+=1;

        ++rgbPTR;
     }

     if (overlaps>0) { fprintf(stderr,"Had %u overlaps\n",overlaps); }

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
  unsigned char minR = R; unsigned char maxR = R;
  unsigned char minG = G; unsigned char maxG = G;
  unsigned char minB = B; unsigned char maxB = B;

  if (minR>=Deviation) { minR-=Deviation; } else { minR=0; }
  if (minG>=Deviation) { minG-=Deviation; } else { minG=0; }
  if (minB>=Deviation) { minB-=Deviation; } else { minB=0; }

  if (maxR+Deviation<255) { maxR+=Deviation; } else { maxR=255; }
  if (maxG+Deviation<255) { maxG+=Deviation; } else { maxG=255; }
  if (maxB+Deviation<255) { maxB+=Deviation; } else { maxB=255; }

  unsigned long thresR = 0;
  unsigned long thresG = 0;
  unsigned long thresB = 0;

  unsigned char i=0;
  for (i=0; i<255; i++) { if ( ( minR<=i ) && (i<=maxR) ) { thresR+=hist->channel[0].intensity[i]; }  }
  for (i=0; i<255; i++) { if ( ( minG<=i ) && (i<=maxG) ) { thresG+=hist->channel[1].intensity[i]; }  }
  for (i=0; i<255; i++) { if ( ( minB<=i ) && (i<=maxB) ) { thresB+=hist->channel[2].intensity[i]; }  }


  if (thresR+thresG+thresB == 0) { return 0; }

  float ourPercentage = (float) imageSize / ((float) thresR+thresG+thresB/3) ;

  fprintf(stderr,"Our Percentage %0.2f , target %0.2f \n",ourPercentage,targetPercentage);
  if (ourPercentage>targetPercentage) { fprintf(stderr,"Success\n"); return 1; }

  return 0;
}


