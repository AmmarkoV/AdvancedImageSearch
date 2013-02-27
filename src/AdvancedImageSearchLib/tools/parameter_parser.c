#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parameter_parser.h"

#include "../codecs/codecs.h"

//This can be removed after finished testing with resize function
#include "../codecs/ppm.h"


#include "../image_processing/imageResizer.h"



static int strcasecmp_internal(char * input1, char * input2)
{
  #if CASE_SENSITIVE_OBJECT_NAMES
    return strcmp(input1,input2);
  #endif

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



void printListOfParametersRecognized()
{
    printf("Parameters : \n");

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

    printf("-like FILENAME THRESHOLD i.e. -like myphoto.jpg 10.5\n");
    printf("Returned images will look like myphoto.jpg with a 10.5%% threshold ( THRESHOLD is a float )\n");
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
           if (strcasecmp_internal(argv[i+1],"red")==0)    { criteria->colorRangeSpecificR = 255; criteria->colorRangeSpecificG = 0; criteria->colorRangeSpecificB = 0; } else
           if (strcasecmp_internal(argv[i+1],"orange")==0) { criteria->colorRangeSpecificR = 255; criteria->colorRangeSpecificG = 165; criteria->colorRangeSpecificB = 0; } else
           if (strcasecmp_internal(argv[i+1],"yellow")==0) { criteria->colorRangeSpecificR = 255; criteria->colorRangeSpecificG = 255; criteria->colorRangeSpecificB = 0; } else
           if (strcasecmp_internal(argv[i+1],"green")==0)  { criteria->colorRangeSpecificR = 0; criteria->colorRangeSpecificG = 128; criteria->colorRangeSpecificB = 0; } else
           if (strcasecmp_internal(argv[i+1],"teal")==0)   { criteria->colorRangeSpecificR = 0; criteria->colorRangeSpecificG = 128; criteria->colorRangeSpecificB = 128; } else
           if (strcasecmp_internal(argv[i+1],"blue")==0)   { criteria->colorRangeSpecificR = 0; criteria->colorRangeSpecificG = 0; criteria->colorRangeSpecificB = 255; } else
           if (strcasecmp_internal(argv[i+1],"purple")==0) { criteria->colorRangeSpecificR = 128; criteria->colorRangeSpecificG = 0; criteria->colorRangeSpecificB = 128; } else
           if (strcasecmp_internal(argv[i+1],"pink")==0)   { criteria->colorRangeSpecificR = 255; criteria->colorRangeSpecificG = 192; criteria->colorRangeSpecificB = 253; } else
           if (strcasecmp_internal(argv[i+1],"white")==0)  { criteria->colorRangeSpecificR = 255; criteria->colorRangeSpecificG = 255; criteria->colorRangeSpecificB = 255; } else
           if (strcasecmp_internal(argv[i+1],"gray")==0)   { criteria->colorRangeSpecificR = 123; criteria->colorRangeSpecificG = 123; criteria->colorRangeSpecificB = 123; } else
           if (strcasecmp_internal(argv[i+1],"black")==0)  { criteria->colorRangeSpecificR = 0; criteria->colorRangeSpecificG = 0; criteria->colorRangeSpecificB = 0; } else
           if (strcasecmp_internal(argv[i+1],"brown")==0)  { criteria->colorRangeSpecificR = 165; criteria->colorRangeSpecificG = 42; criteria->colorRangeSpecificB = 42; }
            else
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
     if (i+2<argc) {
                     criteria->similarityPercent=atof(argv[i+2]);
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
    } else
   //last argument should be dir!
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
   return 1;
}



