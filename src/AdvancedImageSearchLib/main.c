/* AdvancedImageSearch Lib..
A fancy , OpenGL slideshow application !
URLs: http://ammar.gr
Written by Ammar Qammaz a.k.a. AmmarkoV 2013

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/


#include "AdvancedImageSearchLib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

#include "codecs/codecs.h"
#include "codecs/jpg.h"

#define INITIAL_ALLOCATED_MEMORY_FOR_RESULTS 1000


struct AISLib_SearchResults * createSearchResults(unsigned int initialNumberOfResults)
{
  struct AISLib_SearchResults * sr = (struct AISLib_SearchResults *)  malloc(initialNumberOfResults* sizeof(struct AISLib_SearchResults));
  if (sr==0) { fprintf(stderr,"Could not allocate new search results"); return 0;}

  sr->resultsMAX = initialNumberOfResults;
  return sr;
}

struct AISLib_SearchResults * addMoreSearchResults(struct AISLib_SearchResults * initial_results,unsigned int addedNumberOfResults)
{
  struct AISLib_SearchResults * sr = (struct AISLib_SearchResults *)  realloc(initial_results , (initial_results->resultsMAX+addedNumberOfResults)* sizeof(struct AISLib_SearchResults));
  if (sr==0)
   {
       fprintf(stderr,"Could not allocate more results ");
       return initial_results;
   }

  sr->resultsMAX += addedNumberOfResults;
  return sr;
}


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



//This is a more complex check , could use it in the future
int scanFileForImage(char * filename)
{
    char command[1024]={0};
    strcpy(command,"file \"");
    strncat(command,filename,1000);
    strcat(command,"\"");

  FILE *fp;
    /* Open the command for reading. */
     fp = popen(command, "r");
     if (fp == 0 )
       {
         fprintf(stderr,"Failed to run command\n");
         return 0;
       }

 /* Read the output a line at a time - output it. */
  char output[2048]={0};
  unsigned int size_of_output = 2048;

  unsigned int i=0;
  while (fgets(output, size_of_output , fp) != 0)
    {
        ++i;
         fprintf(stderr,"\n\nline %u = %s \n",i,output);
        break;
    }


  /* close */
  pclose(fp);

    return 0;

}



int fileIsImage(char * filename)
{
    if (!scanStringForImageExtensions(filename))
     {
         //We use the filename extensions to speed up discarding non image files
         return 0;
     }

}





struct AISLib_SearchResults * AISLib_Search(char * directory,struct AISLib_SearchCriteria * criteria)
{
  DIR *dpdf=0;
  struct dirent *epdf=0;

  if (directory == 0) { dpdf = opendir("./"); } else
                      { dpdf = opendir(directory); }

  if (dpdf != 0)
   {
     struct AISLib_SearchResults * sr = createSearchResults(INITIAL_ALLOCATED_MEMORY_FOR_RESULTS);


     while (epdf = readdir(dpdf))
      {
         if (fileIsImage(epdf->d_name))
          {
              struct Image pic;
              if ( readImage(epdf->d_name , JPG_CODEC , & pic)  )
              {
                  printf("%s\n",epdf->d_name);
                  fprintf(stderr,"Survived read , I have a %ux%u image ",pic.width,pic.height);
              }
          }
      }
    }

  return 0;
}
