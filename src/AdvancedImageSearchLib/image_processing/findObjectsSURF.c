#include "findObjectsSURF.h"
#include "../configuration.h"
#include "../tools/timers.h"

#if USE_OPENCV_SURF_DETECTOR
int openCV_SURFDetector(struct Image * pattern,struct Image * img)
{
  return 0;
}
#endif


int findPatternInImage(struct Image * pattern,struct Image * img)
{
    #if USE_OPENCV_SURF_DETECTOR
      return openCV_SURFDetector(pattern,img);
    #endif
    return 0;
}



