#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../AdvancedImageSearchLib/AdvancedImageSearchLib.h"


void displayHelp()
{
    printf("lsimg written by Ammar Qammaz a.k.a. AmmarkoV , http://ammar.gr\n");
    printf("linked to AdvancedImageSearch lib v.%s\n",AISLib_Version());
    printf("This is a GPLv3 licensed program\n");
    printf("Source code : https://github.com/AmmarkoV/AdvancedImageSearch\n");
    printf("todo explain here \n\n\n");
    printf("Parameters : \n");
    printf("-minDims MIN_WIDTH MIN_HEIGHT i.e. -minDims 1920 1080\n");
    printf("Returned images will have a minimum dimension specified by MIN_WIDTH and MIN_HEIGHT\n");
    printf("-maxDims MAX_WIDTH MAX_HEIGHT i.e. -maxDims 1920 1080\n");
    printf("Returned images will have a maximum dimension specified by MAX_WIDTH and MAX_HEIGHT\n");
    exit(0);
}

int main(int argc, char *argv[])
{
   char * dirtosearch=0;
   struct AISLib_SearchCriteria criteria={0}; // Initialize all null! IMPORTAN!

    if ( argc <1 )
     {
       fprintf(stderr,"Something weird is happening , argument zero should be executable path :S \n");
       return 1;
     }
      else
    if ( argc == 1 ) { /*Running from current directory PRINT OPTIONS MAYBE ?*/ } else

    {
       dirtosearch = loadDirAndCriteriaFromArgs(argc,argv,&criteria);
    }


   struct AISLib_SearchResults *  result =  AISLib_Search(dirtosearch,&criteria);

   if (dirtosearch!=0) { free(dirtosearch); }

   return 0;
}
