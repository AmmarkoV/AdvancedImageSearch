#ifndef IMAGECOMPARISON_H_INCLUDED
#define IMAGECOMPARISON_H_INCLUDED


#include "../codecs/codecs.h"

int imagesAreSimilar(struct Image * img1,struct Image * img2,float differencePercent);

#endif // IMAGECOMPARISON_H_INCLUDED
