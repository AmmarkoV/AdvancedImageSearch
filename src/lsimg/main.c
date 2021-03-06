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

    //Parameters supported by the SharedObject Library
    AISLib_printHelp();
}

int main(int argc, char *argv[])
{
   char * dirtosearch=0;
   struct AISLib_SearchCriteria * criteria=AISLib_createCriteria();

   if ( argc <1 )        { fprintf(stderr,"Argument zero should be executable path :S \n"); return 1; } else
   if ( argc ==1 )       {  fprintf(stderr,"No arguments supplied\n"); return 1; } else
   /*We have arguments*/ { dirtosearch = AISLib_loadDirAndCriteriaFromArgs(argc,argv,criteria); }

   if (dirtosearch==0)
    {
      //fprintf(stderr,"Directory is not specified assuming current directory \n");
      dirtosearch = (char * ) malloc (sizeof(char)*5);
      snprintf(dirtosearch,4,".");
    }

   if (criteria->needHelp)
    {
        displayHelp();
        return 0;
    }

   struct AISLib_SearchResults *  result =  AISLib_Search(dirtosearch,criteria);
     //Maybe process the results here , although we don't need to right now
   AISLib_destroySearchResults(result);
   AISLib_destroyCriteria(criteria);
   if (dirtosearch!=0) { free(dirtosearch); }
   return 0;
}
