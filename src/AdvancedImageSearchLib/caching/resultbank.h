#ifndef RESULTBANK_H_INCLUDED
#define RESULTBANK_H_INCLUDED

#include "../codecs/codecs.h"

int request_findPatternInImage(char * filename , char * patternname , struct Image * pattern,struct Image * img,float similarity);

int request_findSemanticsOfImage(char * filename , char * patternname,  struct Image * pattern,struct Image * img , float similarity);
int request_imageHasNFaces(char * filename , struct Image * img);
struct Histogram *  request_generateHistogram(char * filename , struct Image * img);

#endif // RESULTBANK_H_INCLUDED
