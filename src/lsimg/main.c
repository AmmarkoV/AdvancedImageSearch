#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../AdvancedImageSearchLib/AdvancedImageSearchLib.h"

int main(int argc, char *argv[])
{
    char * dirtosearch=0;

    if ( argc <1 )
     {
       fprintf(stderr,"Something weird is happening , argument zero should be executable path :S \n");
       return 1;
     }
      else
    if ( argc == 1 ) { /*Running from current directory*/ } else
                     {
                      unsigned int len = strlen(argv[1]);
                      dirtosearch = (char*) malloc(sizeof(char) * len);
                     }
   //TODO SENSE INPUT HERE
   struct AISLib_SearchCriteria criteria;
   struct AISLib_SearchResults *  result =  AISLib_Search(dirtosearch,&criteria);

   if (dirtosearch!=0) { free(dirtosearch); }

   return 0;
}
