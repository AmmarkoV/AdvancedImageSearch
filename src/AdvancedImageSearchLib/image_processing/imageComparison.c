#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imageComparison.h"

int compareImages(struct Image * img1,struct Image * img2,float similarityPercent)
{
 if ( (img1==0) || (img2==0) ) { fprintf(stderr,"Cannot compare images that have not been properly allocated \n"); return 0; }
 if ( (img1->pixels==0) || (img2->pixels==0) ) { fprintf(stderr,"Cannot compare images that do not have a body allocated \n"); return 0; }
 if ( (img1->width!=img2->width) || (img1->height!=img2->height) ) { fprintf(stderr,"Cannot compare images that have different dimensions\n"); return 0; }






 return 0;
}

