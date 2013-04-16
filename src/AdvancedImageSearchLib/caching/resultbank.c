#include "resultbank.h"

#include "../image_processing/findPatterns.h"
#include "../image_processing/faceDetection.h"
#include "../image_processing/histograms.h"



/* djb2
this algorithm (k=33) was first reported by dan bernstein many years ago in comp.lang.c. another version of this algorithm (now favored by bernstein) uses xor: hash(i) = hash(i - 1) * 33 ^ str[i]; the magic of number 33 (why it works better than many other constants, prime or not) has never been adequately explained.
*/
unsigned long hash(unsigned char *str)
    {
        unsigned long hash = 5381;
        int c;

        while (c = *str++)
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

        return hash;
    }




int request_findPatternInImage(char * filename , char * patternname,  struct Image * pattern,struct Image * img)
{
   #if USE_CACHING
    return findPatternInImage(pattern,img);
   #else
    return findPatternInImage(pattern,img);
   #endif
}


int request_imageHasNFaces(char * filename , struct Image * img)
{
   #if USE_CACHING
    return imageHasNFaces(img);
   #else
    return imageHasNFaces(img);
   #endif
}


struct Histogram *  request_generateHistogram(char * filename , struct Image * img)
{
   #if USE_CACHING
    return generateHistogram(img->pixels , img->width , img->height , img->depth );
   #else
    return generateHistogram(img->pixels , img->width , img->height , img->depth );
   #endif
}
