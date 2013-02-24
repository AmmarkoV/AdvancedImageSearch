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

    exit(0);
}

int main(int argc, char *argv[])
{
   char * dirtosearch=0;
   struct AISLib_SearchCriteria criteria={0}; // Initialize all null! IMPORTAN!

    if ( argc <1 )        { fprintf(stderr,"Argument zero should be executable path :S \n"); return 1; } else
    //if ( argc == 1 )      { /*Running from current directory PRINT OPTIONS MAYBE ?*/ } else
    /*We have arguments*/ { dirtosearch = AISLib_loadDirAndCriteriaFromArgs(argc,argv,&criteria); }


   struct AISLib_SearchResults *  result =  AISLib_Search(dirtosearch,&criteria);
   destroySearchResults(result);


   if (dirtosearch!=0) { free(dirtosearch); }

   return 0;
}
