#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <unistd.h>

#include "parameter_parser.h"

#include "timers.h"

#include "../configuration.h"
#include "../codecs/codecs.h"

//This can be removed after finished testing with resize function
#include "../codecs/ppmInput.h"


#include "../image_processing/imageResizer.h"

void runTurbo()
{
 //elevate io ops
 pid_t pid;
 if ((pid = getpid()) < 0) { /*Could not get PID */ } else
                           {
                                char command[512]={0};
                                sprintf(command,"sudo ionice -c 1 -n 0 -p %u",pid);
                                int i=system((char* )command);
                                if (i==0) { /*SUCCESS*/ }

                                sprintf(command,"sudo /bin/bash -c 'sudo renice -n -20 -p %u > /dev/null'",pid);
                                i=system((char* )command);
                                if (i==0) { /*SUCCESS*/ }


                                strcpy(command,"sudo /bin/bash -c 'for CPUFREQ in /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor\n");
                                strcat(command,"do\n");
                                strcat(command,"[ -f $CPUFREQ ] || continue\n");
                                strcat(command,"echo -n \"performance\" > $CPUFREQ\n");
                                strcat(command,"done\n");
                                strcat(command,"exit 0\n' ");
                                i=system((char* )command);
                                if (i==0) { /*SUCCESS*/ }
                           }

}

static int strcasecmp_internal(char * input1, char * input2)
{
    if ( (input1==0) || (input2==0) )
     {
         fprintf(stderr,"Error , calling strcasecmp_internal with null parameters \n");
         return 1;
     }
    unsigned int len1 = strlen(input1);
    unsigned int len2 = strlen(input2);
    if (len1!=len2)
     {
         //mismatched lengths of strings , they can't be equal..!
         return 1;
     }

   char A; //<- character buffer for input1
   char B; //<- character buffer for input2

   int i=0;
   while (i<len1) //len1 and len2 are equal
    {
       A = toupper(input1[i]);
       B = toupper(input2[i]);
       if (A!=B) { return 1; }
       ++i;
    }
  //if we reached this point , there where no reasons
  //why input1 and input2 could not be equal..
  return 0;
}

int colorNameToRGB(char * name,unsigned char * R ,unsigned char * G ,unsigned char * B)
{
           if (strcasecmp_internal(name,"red")==0)    { *R=255; *G=0;   *B=0; }   else
           if (strcasecmp_internal(name,"orange")==0) { *R=255; *G=165; *B=0; }   else
           if (strcasecmp_internal(name,"yellow")==0) { *R=255; *G=255; *B=0; }   else
           if (strcasecmp_internal(name,"green")==0)  { *R=0;   *G=128; *B=0; }   else
           if (strcasecmp_internal(name,"teal")==0)   { *R=0;   *G=128; *B=128; } else
           if (strcasecmp_internal(name,"blue")==0)   { *R=0;   *G=0;   *B=255; } else
           if (strcasecmp_internal(name,"purple")==0) { *R=128; *G=0;   *B=128; } else
           if (strcasecmp_internal(name,"pink")==0)   { *R=255; *G=192; *B=253; } else
           if (strcasecmp_internal(name,"white")==0)  { *R=255; *G=255; *B=255; } else
           if (strcasecmp_internal(name,"gray")==0)   { *R=123; *G=123; *B=123; } else
           if (strcasecmp_internal(name,"black")==0)  { *R=0;   *G=0;   *B=0; }   else
           if (strcasecmp_internal(name,"brown")==0)  { *R=165; *G=42;  *B=42; }  else
                                                      { return 0;  }
  return 1;
}


void printTimersToStderr()
{
  fprintf(stderr,"Performance report -=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
  fprintf(stderr," FUNCTION                     TIME  IN  MICROSECONDS  SAMPLES!!\n");
  fprintf(stderr,"                              min/avg/max   last      count    \n");
  unsigned int i=0;
  for (i=0; i<TOTAL_TIMERS; i++)
   {
     fprintf(stderr," %s , %u/%u/%u , LAST %u , SAMPLES %u \n",timerIDDescription[i],GetMinimumTimer(i),GetAverageTimer(i),GetMaximumTimer(i),GetLastTimer(i),GetTimesTimerTimed(i));
   }
  fprintf(stderr,"     -=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-\n");

}



void printNotCompiledInSupport(char* whatisnot)
{
   fprintf(stderr,"Parameter %s is not compiled in this binary of libAdvancedImageSearch\n",whatisnot);
   fprintf(stderr,"consider altering configuration.h , installing dependencies needed and then recompiling \n");
}


void printListOfParametersRecognized()
{
    printf("Parameters : \n");

    printf("-report\n");
    printf("On completion of operations a detailed analysis of how much time elapsed during which\n");
    printf("operations will be printed on stderr \n");

    printf("-turbo\n");
    printf("Requires root , run at top schedule!\n");

    printf("-minDims MIN_WIDTH MIN_HEIGHT i.e. -minDims 1920 1080\n");
    printf("Returned images will have a minimum dimension specified by MIN_WIDTH and MIN_HEIGHT\n");

    printf("-maxDims MAX_WIDTH MAX_HEIGHT i.e. -maxDims 1920 1080\n");
    printf("Returned images will have a maximum dimension specified by MAX_WIDTH and MAX_HEIGHT\n");

    printf("-histogram R G B DEVIATION i.e. -histogram 240 240 0 30\n");
    printf("Returned images will have a median rgb color close to this (+-DEVIATION for every channel value)\n");

    printf("-color COLORNAME i.e. -histogram red\n");
    printf("Returned images will have a median rgb color close to this color\n");
    printf("Possible colors : red orange yellow green teal blue purple pink green white gray black brown\n");

    printf("-limit NUMBER_OF_RESULTS i.e. -limit 10\n");
    printf("Returned images will be no more than NUMBER_OF_RESULTS\n");

    printf("-like FILENAME  i.e. -like myphoto.jpg  \n");
    printf("Returned images will look like myphoto.jpg with an automatic threshold\n");

    printf("-likeExt FILENAME PIXEL_THRESHOLD MAX_DIFFERENCE i.e. -like myphoto.jpg 30 10.5\n");
    printf("Returned images will look like myphoto.jpg with a 30 pixel threshold per pixel and\n");
    printf("a maximum of 10.5%% different pixels ( MAX_DIFFERENCE is a float )\n");

    #if USE_OPENCV_SURF_DETECTOR
    printf("-contains FILENAME  i.e. -contains object.jpg  \n");
    printf("Returned images will contain a pattern like object.jpg \n");
    #endif


     #if USE_OPENCV_FACEDETECTION
    printf("-minFaces NUMBER i.e. -minFaces 1\n");
    printf("Returned images will have a minimum count of NUMBER faces\n");

    printf("-maxFaces NUMBER i.e. -maxFaces 2\n");
    printf("Returned images will have no more than NUMBER faces\n");
    #endif

}



char * parseCommandLineParameters(int argc, char *argv[], struct AISLib_SearchCriteria * criteria )
{
 if (criteria==0)
 {
    fprintf(stderr,"Please initialize criteria using AISLib_createCriteria before calling parseCommandLineParameters\n");
    return 0;
 }

 criteria->needHelp = 0;
 criteria->limitResults = 0;


 char * outdir=0;
 int i=0;
 for (i=0; i<argc; i++)
 {
   if (strcmp(argv[i],"-help")==0)
    {
      criteria->needHelp=1;
    } else
   if (strcmp(argv[i],"-report")==0)
    {
      criteria->printTimers=1;
    } else
   if (strcmp(argv[i],"-turbo")==0)
    {
      runTurbo();
    } else
   if (strcmp(argv[i],"-limit")==0)
    {
     if (i+1<argc) {
                     criteria->limitResults = atoi(argv[i+1]);
                     criteria->criteriaSpecified=1;
                   }
    } else
   if (strcmp(argv[i],"-minDims")==0)
    {
     if (i+2<argc) {
                     criteria->minDimensionsUsed = 1;
                     criteria->minWidth = atoi(argv[i+1]);
                     criteria->minHeight = atoi(argv[i+2]);
                     criteria->criteriaSpecified=1;
                   }
    } else
   if (strcmp(argv[i],"-maxDims")==0)
    {
     if (i+2<argc) {
                     criteria->maxDimensionsUsed = 1;
                     criteria->maxWidth = atoi(argv[i+1]);
                     criteria->maxHeight = atoi(argv[i+2]);
                     criteria->criteriaSpecified=1;
                   }
    } else
   if (strcmp(argv[i],"-histogram")==0)
    {
     if (i+4<argc) {
                     criteria->colorRangeUsed = 1;
                     criteria->colorRangeSpecificR = atoi(argv[i+1]);
                     criteria->colorRangeSpecificG = atoi(argv[i+2]);
                     criteria->colorRangeSpecificB = atoi(argv[i+3]);
                     criteria->colorRange = atoi(argv[i+4]);
                     criteria->criteriaSpecified=1;
                   }
    } else
   if (strcmp(argv[i],"-color")==0)
    {
     if (i+1<argc)
        {
           criteria->colorRangeUsed = 1;
           if (!colorNameToRGB(argv[i+1],&criteria->colorRangeSpecificR,&criteria->colorRangeSpecificG,&criteria->colorRangeSpecificB))
              {
                fprintf(stderr,"Could not identify color (%s) , consider running -help for a possible color list\n",argv[i+1]);
                criteria->colorRangeUsed = 0;
              }

           if (criteria->colorRangeUsed) {  criteria->criteriaSpecified=1; }
           criteria->colorRange = 50;
        }
    } else
   if (strcmp(argv[i],"-like")==0)
    {
     if (i+1<argc) {
                     criteria->perPixelThreshold=30;
                     criteria->similarityPercent=60;
                     strncpy( criteria->similarImageFilename , argv[i+1] , MAX_CRITERIA_STRING_SIZE );

                     struct Image * img = readImage(argv[i+1],JPG_CODEC,0);
                     if (img!=0)
                     {
                       struct Image * rszdImage = resizeImage(img,criteria->comparisonWidth,criteria->comparisonHeight);
                       if (rszdImage!=0)
                       {
                         //WritePPM("test.ppm",rszdImage);
                         criteria->similarityUsed=1;
                         criteria->similarImage = (void*) rszdImage;
                         criteria->criteriaSpecified=1;
                       }
                       //Deallocate initial BIG image , we won't be needing it
                       destroyImage(img);
                     }
                   }
    } else
   if (strcmp(argv[i],"-likeExt")==0)
    {
     if (i+3<argc) {
                     criteria->perPixelThreshold=atoi(argv[i+2]);
                     criteria->similarityPercent=atof(argv[i+3]);
                     strncpy( criteria->similarImageFilename , argv[i+1] , MAX_CRITERIA_STRING_SIZE );

                     struct Image * img = readImage(argv[i+1],JPG_CODEC,0);
                     if (img!=0)
                     {
                       struct Image * rszdImage = resizeImage(img,criteria->comparisonWidth,criteria->comparisonHeight);
                       if (rszdImage!=0)
                       {
                         //WritePPM("test.ppm",rszdImage);
                         criteria->similarityUsed=1;
                         criteria->similarImage = (void*) rszdImage;
                         criteria->criteriaSpecified=1;
                       }
                       //Deallocate initial BIG image , we won't be needing it
                       destroyImage(img);
                     }
                   }
    } else
   if (strcmp(argv[i],"-contains")==0)
    {
     #if USE_PATTERN_RECOGNITION
     if (i+2<argc) {
                     criteria->containsSimilarityPercent=atof(argv[i+2]);
                     strncpy( criteria->containsImageFilename , argv[i+1] , MAX_CRITERIA_STRING_SIZE );

                     struct Image * img = readImage(argv[i+1],JPG_CODEC,0);
                     if (img!=0)
                     {
                         criteria->containsUsed=1;
                         criteria->containsImage = (void*) img;
                         criteria->criteriaSpecified=1;
                     }
                   }
      #else
       printNotCompiledInSupport(argv[i]);
     #endif // USE_OPENCV_SURF_DETECTOR
    } else
   if (strcmp(argv[i],"-minFaces")==0)
    {
     #if USE_OPENCV_FACEDETECTION
     if (i+1<argc) {
                     criteria->minFacesUsed=1;
                     criteria->minimumFaceCount = atoi(argv[i+1]);
                     criteria->criteriaSpecified=1;
                   }
     #else
       printNotCompiledInSupport(argv[i]);
     #endif // USE_OPENCV_FACEDETECTION

    } else
   if (strcmp(argv[i],"-maxFaces")==0)
    {
     #if USE_OPENCV_FACEDETECTION
     if (i+1<argc) {
                     criteria->maxFacesUsed=1;
                     criteria->maximumFaceCount = atoi(argv[i+1]);
                     criteria->criteriaSpecified=1;
                   }
     #else
       printNotCompiledInSupport(argv[i]);
     #endif // USE_OPENCV_FACEDETECTION
    }


   //last argument should be dir!
     else


   if (i==argc-1)
    {
      //fprintf(stderr,"Found DIR! %s \n",argv[i]);
      outdir = ( char* ) malloc((strlen(argv[i])+1) *(sizeof(char) )); //+1 for null termination!
      if (outdir!=0) {   strcpy(outdir,argv[i]);  }

    }

 }
 return outdir;
}



//This function returns true if criteria needs only header
char searchCriteriaRequireOnlyImageHeaderLoaded(struct AISLib_SearchCriteria * criteria)
{
   if (criteria->colorRangeUsed) { return 0; }
   if (criteria->similarityUsed) { return 0; }
   if (criteria->containsUsed) { return 0; }
   if ( (criteria->minFacesUsed)||(criteria->maxFacesUsed) ) { return 0; }
   return 1;
}



