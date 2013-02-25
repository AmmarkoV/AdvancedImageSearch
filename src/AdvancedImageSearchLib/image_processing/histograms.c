#include "histograms.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG_HISTOGRAMS 0

float HISTOGRAM_SUCCESS_PERCENTAGE = 50;//%

void cleanHistogram ( struct Histogram * hist )
{
    memset(hist , 0 , sizeof(struct Histogram));
    return ;
    unsigned int i =0;
    for (i=0; i<255; i++) { hist->channel[0].intensity[i]=0; }
    for (i=0; i<255; i++) { hist->channel[1].intensity[i]=0; }
    for (i=0; i<255; i++) { hist->channel[2].intensity[i]=0; }
}



struct Histogram *  generateHistogramPTRS(unsigned char * rgb , unsigned int width , unsigned int height , unsigned int channels )
{
   fprintf(stderr,"This code compiles incorrectly :P \n");
   return 0;
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



struct Histogram *  generateHistogram(unsigned char * rgb , unsigned int width , unsigned int height , unsigned int channels )
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

    unsigned int tmpR = 0;
    unsigned int tmpG = 0;
    unsigned int tmpB = 0;

    while ( rgbPTRIndex< rgbPTRIndexLimit)
     {
        tmpR = rgb[rgbPTRIndex];
        #if DEBUG_HISTOGRAMS
         if (tmpR>255) { tmpR=255; ++overlaps;  }
        #endif
        rHist->intensity[ tmpR ]+=1;

         ++rgbPTRIndex;
        tmpG = rgb[rgbPTRIndex];
        #if DEBUG_HISTOGRAMS
         if (tmpG>255) { tmpG=255; ++overlaps;  }
        #endif
        gHist->intensity[ tmpG ]+=1;

         ++rgbPTRIndex;
        tmpB = rgb[rgbPTRIndex];
        #if DEBUG_HISTOGRAMS
         if (tmpB>255) { tmpB=255; ++overlaps;  }
        #endif
        bHist->intensity[ tmpB ]+=1;

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
       #if DEBUG_HISTOGRAMS
        fprintf(stderr,"Cannot make histogram for %u channels \n",channels);
       #endif
       free(hist);
       return 0;
    }

   return hist;
}












int histogramIsCloseToColor(struct Histogram * hist,unsigned char R,unsigned char G,unsigned char B,unsigned char Deviation,unsigned int imageSize,float targetPercentage)
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



  unsigned int imageSizePerChannel = (unsigned int) imageSize/3;

  float ourR_Percentage = (float) 100 * ((float) thresR/imageSizePerChannel) ;
  float ourG_Percentage = (float) 100 * ((float) thresG/imageSizePerChannel) ;
  float ourB_Percentage = (float) 100 * ((float) thresB/imageSizePerChannel) ;

  #if DEBUG_HISTOGRAMS
    fprintf(stderr,"Our R %0.2f%% G %0.2f%% B %0.2f%% , target %0.2f \n",ourR_Percentage,ourG_Percentage,ourB_Percentage,targetPercentage);
  #endif

  if (
       (ourR_Percentage>=HISTOGRAM_SUCCESS_PERCENTAGE) &&
       (ourG_Percentage>=HISTOGRAM_SUCCESS_PERCENTAGE) &&
       (ourB_Percentage>=HISTOGRAM_SUCCESS_PERCENTAGE)
     ) { return 1; }

  return 0;
}


