#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_extension_scanner.h"
#include "../codecs/codecs.h"


//I have to make an external tool to generate code segments like the following!
int scanStringForImageExtensionsFastJPGOnly(char * inpt)
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



int is_valid_suffix(char *s)
{
	ssize_t i = strlen(s) - 1;
	while (--i, i >= 0) s[i] = tolower(s[i]);



    if (strcmp("jpg", s) == 0) { return JPG_CODEC; }
    if (strcmp("jpeg", s) == 0) { return JPG_CODEC; }


    if (strcmp("png", s) == 0) { return PNG_CODEC; }

    if (strcmp("ppm", s) == 0) { return PPM_CODEC; }
    if (strcmp("pnm", s) == 0) { return PPM_CODEC; }

    return 0;
}


int scanStringForImageExtensionsSimple(char * inpt)
 {
	char *s = 0;

	s = strrchr(inpt, '.');
	if (s != 0)
	 {
	     return is_valid_suffix(++s);
	 }
	return 0;
}
























