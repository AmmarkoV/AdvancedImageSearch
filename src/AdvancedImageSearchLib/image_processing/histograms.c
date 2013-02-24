#include "histograms.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int generateHistogram(char * rgb , unsigned int width , unsigned int height , unsigned int channels , struct Histogram * hist )
{
   char * rgbPTR = rgb;
   char * rgbLimit = rgb+width*height*channels;

   memset(hist , 0 , sizeof(struct Histogram));

   if (channels==1)
   {
    while (rgbPTR<rgbLimit)
     {
        ++hist->channel[0].intensity[*rgbPTR];
        ++rgbPTR;
     }
   } else
   if (channels==3)
   {
    while (rgbPTR<rgbLimit)
     {
        ++hist->channel[0].intensity[*rgbPTR];
        ++rgbPTR;
        ++hist->channel[1].intensity[*rgbPTR];
        ++rgbPTR;
        ++hist->channel[2].intensity[*rgbPTR];
        ++rgbPTR;
     }
   }
    else
    {
       fprintf(stderr,"Cannot make histogram for %u channels \n",channels);
       return 0;
    }

   return 1;
}
