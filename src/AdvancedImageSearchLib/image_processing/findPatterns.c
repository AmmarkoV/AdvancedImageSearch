#include "findPatterns.h"
#include "../configuration.h"
#include "../tools/timers.h"

#include <stdio.h>

#include "../../PatternRecognition/PatternRecognition.h"


int findPatternInImage(struct Image * pattern,struct Image * img,float similarity)
{
   //fprintf(stderr,"findPatternInImage called..");
   int result = 0;
    #if USE_PATTERN_RECOGNITION
      StartTimer(FIND_OBJECTS_DELAY);
       result = detectPattern((struct ptrnImage *) pattern,(struct ptrnImage *) img,similarity);
      EndTimer(FIND_OBJECTS_DELAY);
    #endif
    return result;
}



