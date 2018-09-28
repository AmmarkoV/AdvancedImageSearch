#include "findSemantics.h"
#include "../configuration.h"
#include "../tools/timers.h"

#include <unistd.h>
#include <stdio.h>
#include <limits.h>

struct detectorResult
{
 char label[128];
 float probability;
};
#define detectorReturnTopNResults 5
struct detectorResult patternResults[detectorReturnTopNResults]={0};
struct detectorResult detectorResults[detectorReturnTopNResults]={0};

#if USE_DARKNET
#include "../../DarknetProcessor/DarknetProcessor.h"
#endif // USE_DARKNET

int haveInit=0;
int argc=7;
char * argv[]={
                "dummy",
                "/home/ammar/Documents/Programming/RGBDAcquisition/3dparty/darknet/darknet19.weights" ,
                "/home/ammar/Documents/Programming/RGBDAcquisition/3dparty/darknet/cfg/darknet19.cfg" ,
                "/home/ammar/Documents/Programming/RGBDAcquisition/3dparty/darknet/cfg/imagenet1k.data" ,
                "/home/ammar/Documents/Programming/RGBDAcquisition/3dparty/darknet/data/imagenet.names" ,
                "--classifier",
                "--forbidNameListChange",
                "dummy",
                "dummy"
                };
char cwd[PATH_MAX];

int findSemanticsOfImage(struct Image * pattern,struct Image * img,float similarity)
{
   int result = 0;
    #if USE_DARKNET
  if(!haveInit)
   {
    if (getcwd(cwd, sizeof(cwd)) != NULL) { }

    chdir("/home/ammar/Documents/Programming/RGBDAcquisition/3dparty/darknet");
    haveInit = initArgs_DarknetProcessor(argc, argv);

    chdir(cwd);

    if (haveInit)
    {
    fprintf(stderr,"initialized and now searching for pattern..\n");
    addDataInput_DarknetProcessor(
                                  0,
                                  pattern->pixels,
                                  pattern->width,
                                  pattern->height,
                                  pattern->channels,
                                  pattern->bitsperpixel
                                 );

     fprintf(stderr,"getting back results..\n");
     for (unsigned int i=0; i<detectorReturnTopNResults; i++)
     {
      snprintf(patternResults[i].label,128,"%s", getDetectionLabel_DarknetProcessor(i));
      patternResults[i].probability = getDetectionProbability_DarknetProcessor(i);
     }
    } else
    { return 0; }
   }

      StartTimer(FIND_OBJECTS_DELAY);
       if(haveInit)
        {
         addDataInput_DarknetProcessor(
                                       0,
                                       pattern->pixels,
                                       pattern->width,
                                       pattern->height,
                                       pattern->channels,
                                       pattern->bitsperpixel
                                      );

         for (unsigned int i=0; i<detectorReturnTopNResults; i++)
          {
           snprintf(detectorResults[i].label,128,"%s", getDetectionLabel_DarknetProcessor(i));
           detectorResults[i].probability = getDetectionProbability_DarknetProcessor(i);
          }
        }
      EndTimer(FIND_OBJECTS_DELAY);


    #endif
    return result;
}

