#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_extension_scanner.h"


//I have to make an external tool to generate code segments like the following!
int scanStringForImageExtensions(char * inpt)
{
  if (inpt==0) { return 0; }
  unsigned int length = strlen(inpt);
  if (length<3) { return 0; }


  char * inptPTR = inpt;
  char * inptLimit = inpt+length;

  //We recognize the words JPG only , case insensitive :P

  while (inptPTR<inptLimit)
  {
    switch (*inptPTR)
     {
       case 'j' :
       case 'J' :
        if (inptPTR+2>=inptLimit)  { /*fprintf(stderr,"Out of bounds skip");*/ return 0; }
        switch (*(inptPTR+1))
        {
          case 'p' :
          case 'P' :
          switch (*(inptPTR+2))
          {
           case 'g' :
           case 'G' :
             //fprintf(stderr,"JPG found\n");
             return 1;
           break;
          }
          break;
        }
       break;
     }

    ++inptPTR;
  }
 return 0;
}
