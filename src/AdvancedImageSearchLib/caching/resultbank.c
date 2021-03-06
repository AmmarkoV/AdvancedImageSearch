#include "resultbank.h"

#include "../neural_networks/findSemantics.h"
#include "../image_processing/findPatterns.h"
#include "../image_processing/faceDetection.h"
#include "../image_processing/histograms.h"

#include <stdio.h>

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

// ---  ---  ---  ---  ---  ---  ---  ---  ---  ---

int request_findPatternInImage(char * filename , char * patternname,  struct Image * pattern,struct Image * img , float similarity)
{
   //fprintf(stderr,"request_findPatternInImage");
   #if USE_CACHING
    //TODO IMPLEMENT A QUESTION TO A DATABASE IF WE HAVE THE PATTERN IN FILENAME
    return findPatternInImage(pattern,img,similarity);
   #else
    return findPatternInImage(pattern,img,similarity);
   #endif
}

int request_findSemanticsOfImage(char * filename , char * patternname,  struct Image * pattern,struct Image * img , float similarity)
{
   //fprintf(stderr,"request_findPatternInImage");
   #if USE_CACHING
    //TODO IMPLEMENT A QUESTION TO A DATABASE IF WE HAVE THE PATTERN IN FILENAME
    return findSemanticsOfImage(pattern,img,similarity);
   #else
    return findSemanticsOfImage(pattern,img,similarity);
   #endif
}


int request_imageHasNFaces(char * filename , struct Image * img)
{
   #if USE_CACHING
    //TODO IMPLEMENT A QUESTION TO A DATABASE IF WE HAVE A LIST OF FACES IN IMAGE
    return imageHasNFaces(img);
   #else
    return imageHasNFaces(img);
   #endif
}


struct Histogram *  request_generateHistogram(char * filename , struct Image * img)
{
   #if USE_CACHING
    //TODO IMPLEMENT A QUESTION TO A DATABASE IF WE HAVE A PRECALCULATED HISTOGRAM
    return generateHistogram(img->pixels , img->width , img->height , img->depth );
   #else
    return generateHistogram(img->pixels , img->width , img->height , img->channels );
   #endif
}
