#include "resultbank.h"

#include "../image_processing/findPatterns.h"
#include "../image_processing/faceDetection.h"


int request_findPatternInImage(struct Image * pattern,struct Image * img)
{
   return findPatternInImage(pattern,img);
}


int request_imageHasNFaces(struct Image * img)
{
   return imageHasNFaces(img);
}
