#include "histograms.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG_HISTOGRAMS 1

void cleanHistogram ( struct Histogram * hist )
{
    memset(hist , 0 , sizeof(struct Histogram));
    return ;
    unsigned int i =0;
    for (i=0; i<255; i++) { hist->channel[0].intensity[i]=0; }
    for (i=0; i<255; i++) { hist->channel[1].intensity[i]=0; }
    for (i=0; i<255; i++) { hist->channel[2].intensity[i]=0; }
}



struct Histogram *  generateHistogramPTRS(char * rgb , unsigned int width , unsigned int height , unsigned int channels )
{
   if (rgb==0) { return 0; }
   unsigned char * rgbPTR = (unsigned char* )rgb;
   unsigned char * rgbLimit = rgbPTR + width*height*channels;
   struct Histogram *  hist =  (struct Histogram * ) malloc (sizeof(struct Histogram));
   if (hist==0) { return 0; }
   cleanHistogram(hist);

   if (channels==3)
   {
    unsigned int overlaps = 0;
    unsigned int rgbPTRIndex=0;
    while (rgbPTR<rgbLimit)
     {
        rgbPTRIndex = 0;
        rgbPTRIndex = (unsigned int) (*rgbPTR);
        if (rgbPTRIndex>255) { rgbPTRIndex=255; ++overlaps;  }
        hist->channel[0].intensity[rgbPTRIndex]+=1;

        ++rgbPTR;
        rgbPTRIndex = 0;
        rgbPTRIndex = (unsigned int) (*rgbPTR);
        if (rgbPTRIndex>255) { rgbPTRIndex=255; ++overlaps; }
        hist->channel[1].intensity[rgbPTRIndex]+=1;

        ++rgbPTR;
        rgbPTRIndex = 0;
        rgbPTRIndex = (unsigned int) (*rgbPTR);
        if (rgbPTRIndex>255) { rgbPTRIndex=255; ++overlaps;  }
        hist->channel[2].intensity[rgbPTRIndex]+=1;

        ++rgbPTR;
     }

     if (overlaps>0) {
                       #if DEBUG_HISTOGRAMS
                        fprintf(stderr,"Had %u overlaps\n",overlaps);
                       #endif
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



struct Histogram *  generateHistogram(char * rgb , unsigned int width , unsigned int height , unsigned int channels )
{
   if (rgb==0) { return 0; }
   struct Histogram *  hist =  (struct Histogram * ) malloc (sizeof(struct Histogram));
   if (hist==0) { return 0; }
   cleanHistogram(hist);

   if (channels==3)
   {
    unsigned int overlaps = 0;
    unsigned int rgbPTRIndex=0;
    unsigned int rgbPTRIndexLimit = width*height*channels;

    struct histogramChannel * rHist = &hist->channel[0];
    struct histogramChannel * gHist = &hist->channel[1];
    struct histogramChannel * bHist = &hist->channel[2];


    while ( rgbPTRIndex< rgbPTRIndexLimit)
     {

        //*(rgb+rgbPTRIndex);
        unsigned int tmpR =  rgb[rgbPTRIndex];
        //tmpR gets values like 4294967294
        if (tmpR>=255) { tmpR=255; ++overlaps;  }
        rHist->intensity[ tmpR ]=rHist->intensity[ tmpR ]+1;

        unsigned int tmpG =  rgb[rgbPTRIndex];
        if (tmpG>=255) { tmpG=255; ++overlaps;  }
        gHist->intensity[ tmpG ]=gHist->intensity[ tmpG ]+1;

        unsigned int tmpB =  rgb[rgbPTRIndex];
        if (tmpB>=255) { tmpB=255; ++overlaps;  }
        bHist->intensity[ tmpB ]=bHist->intensity[ tmpB ]+1;

        ++rgbPTRIndex;
     }

     if (overlaps>0) {
                       #if DEBUG_HISTOGRAMS
                        fprintf(stderr,"Had %u overlaps\n",overlaps);
                       #endif
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
  if (hist==0) { return 0; }
  if (imageSize==0) { return 0; }

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



  unsigned int imageSizePerChannel = (unsigned int) imageSize;
  if (imageSizePerChannel == 0) { return 0; }

  float ourR_Percentage = (float) 100 * thresR/imageSizePerChannel ;
  float ourG_Percentage = (float) 100 * thresG/imageSizePerChannel ;
  float ourB_Percentage = (float) 100 * thresB/imageSizePerChannel ;

  #if DEBUG_HISTOGRAMS
    fprintf(stderr,"Our R %0.2f%% G %0.2f%% B %0.2f%% , target %0.2f \n",ourR_Percentage,ourG_Percentage,ourB_Percentage,targetPercentage);
  #endif

  if (
       (ourR_Percentage>=targetPercentage) &&
       (ourG_Percentage>=targetPercentage) &&
       (ourB_Percentage>=targetPercentage)
     ) { return 1; }

  return 0;
}


