#include "findSemantics.h"
#include "../configuration.h"
#include "../tools/timers.h"

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>


#define NORMAL   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */

struct detectorResult
{
 char label[128];
 float probability;
};
#define detectorReturnTopNResults 5
struct detectorResult patternResults[detectorReturnTopNResults]={0};
struct detectorResult haystackResults[detectorReturnTopNResults]={0};

int haveInit=0;

#if USE_DARKNET
#include "../../DarknetProcessor/DarknetProcessor.h"
const char darknetPath[]={DARKNET_PATH};

int argc=7;
char * argv[]={
                "dummy",
                DARKNET_PATH "/darknet19.weights" ,
                DARKNET_PATH "/cfg/darknet19.cfg" ,
                DARKNET_PATH "/cfg/imagenet1k.data" ,
                DARKNET_PATH "/data/imagenet.names" ,
                "--classifier",
                "--forbidNameListChange",
                "dummy",
                "dummy"
                };

#endif // USE_DARKNET

char cwd[PATH_MAX];



int isItAMatch(struct detectorResult * needle,struct detectorResult * haystack,float similarity)
{
  int i=0,z=0,matches=0;
  float similarityNormalized = (float) similarity/100;

  if (visualization)
   { fprintf(stderr,"_______________________\n"); }
  for (i=0; i<5; i++)
  {
   for (z=0; z<5; z++)
   {
     if (
          (needle[i].probability>similarityNormalized) &&
          (haystack[z].probability>similarityNormalized)
        )
     {
       if (strstr(needle[i].label,haystack[z].label)!=0)
       {
        if (visualization)
        {
         fprintf(
                 stderr,GREEN "Matched %s(%0.2f)/%s(%0.2f)\n" NORMAL,
                 needle[i].label,
                 needle[i].probability,
                 haystack[z].label,
                 haystack[z].probability
                );
        }
          ++matches;
       }
     }
   }
 if (visualization)
        {
          fprintf(
                  stderr,"Comparing %s(%0.2f)/%s(%0.2f)\n",
                  needle[i].label,
                  needle[i].probability,
                  haystack[i].label,
                  haystack[i].probability
                 );
        }
   }
  //fprintf(stderr,"_______________________\n");
  return (matches>0);
}



int findSemanticsOfImage(struct Image * pattern,struct Image * img,float similarity)
{
   int result = 0;
   #if USE_DARKNET

   //fprintf(stderr,"DARKNET_PATH=%s\n",darknetPath);

   if(!haveInit)
   {
    if (getcwd(cwd, sizeof(cwd)) != NULL) { }

    chdir(darknetPath);
    haveInit = initArgs_DarknetProcessor(argc, argv);

    chdir(cwd);

    if (haveInit)
    {
    //fprintf(stderr,"initialized and now searching for pattern..\n");
    if (
        addDataInput_DarknetProcessor(
                                      0,
                                      pattern->pixels,
                                      pattern->width,
                                      pattern->height,
                                      pattern->channels,
                                      pattern->bitsperpixel
                                     )
       )
     {
      //fprintf(stderr,"getting back results..\n");
      for (unsigned int i=0; i<detectorReturnTopNResults; i++)
      {
       snprintf(patternResults[i].label,128,"%s", getDetectionLabel_DarknetProcessor(i));
       patternResults[i].probability = getDetectionProbability_DarknetProcessor(i);
      }
     }
    } else
    { return 0; }
   }

      StartTimer(FIND_OBJECTS_DELAY);
       if(haveInit)
        {
         if (
             addDataInput_DarknetProcessor(
                                           0,
                                           img->pixels,
                                           img->width,
                                           img->height,
                                           img->channels,
                                           img->bitsperpixel
                                          )
            )
         {
         for (unsigned int i=0; i<detectorReturnTopNResults; i++)
          {
           snprintf(haystackResults[i].label,128,"%s", getDetectionLabel_DarknetProcessor(i));
           haystackResults[i].probability = getDetectionProbability_DarknetProcessor(i);
          }

          if ( isItAMatch(patternResults,haystackResults,similarity) )
          {
            return 1;
          }

         }
        }
      EndTimer(FIND_OBJECTS_DELAY);


    #endif
    return result;
}

