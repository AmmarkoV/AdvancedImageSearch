#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parameter_parser.h"


void printListOfParametersRecognized()
{
    printf("Parameters : \n");

    printf("-minDims MIN_WIDTH MIN_HEIGHT i.e. -minDims 1920 1080\n");
    printf("Returned images will have a minimum dimension specified by MIN_WIDTH and MIN_HEIGHT\n");

    printf("-maxDims MAX_WIDTH MAX_HEIGHT i.e. -maxDims 1920 1080\n");
    printf("Returned images will have a maximum dimension specified by MAX_WIDTH and MAX_HEIGHT\n");

    printf("-histogram R G B DEVIATION i.e. -histogram 240 240 0 30\n");
    printf("Returned images will have a median rgb color close to this (+-DEVIATION for every channel value)\n");
}



char * parseCommandLineParameters(int argc, char *argv[], struct AISLib_SearchCriteria * criteria )
{
 char * outdir=0;
 int i=0;
 for (i=0; i<argc; i++)
 {
   if (strcmp(argv[i],"-help")==0)
    {
      displayHelp();
    } else
   if (strcmp(argv[i],"-minDims")==0)
    {
     if (i+2<argc) {
                     criteria->minDimensionsUsed = 1;
                     criteria->minWidth = atoi(argv[i+1]);
                     criteria->minHeight = atoi(argv[i+2]);
                   }
    } else
   if (strcmp(argv[i],"-maxDims")==0)
    {
     if (i+2<argc) {
                     criteria->maxDimensionsUsed = 1;
                     criteria->maxWidth = atoi(argv[i+1]);
                     criteria->maxHeight = atoi(argv[i+2]);
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
                   }
    } else


   //last argument should be dir!
   if (i==argc-1)
    {
      fprintf(stderr,"Found DIR! %s \n",argv[i]);
      outdir = ( char* ) malloc((strlen(argv[i])+1) *(sizeof(char) )); //+1 for null termination!
      if (outdir!=0) {   strcpy(outdir,argv[i]);  }

    }

 }
 return outdir;
}



char searchCriteriaRequireOnlyImageHeaderLoaded(struct AISLib_SearchCriteria * criteria)
{
   if (criteria->colorRangeUsed) { return 0; }
   return 1;
}



