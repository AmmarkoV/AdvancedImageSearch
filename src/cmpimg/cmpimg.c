#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../AdvancedImageSearchLib/AdvancedImageSearchLib.h"
#define MAX_STRING_TO_ACCEPT 4096

int main(int argc, char *argv[])
{
  if (argc<3) {  fprintf(stderr,"cmpimg called with not enough parameters\n"); return 1; }

  //Pass argument 1 to filename1
  char * filename1=0;
  unsigned int len1 = strlen(argv[1]);
  if (len1 > MAX_STRING_TO_ACCEPT ) { fprintf(stderr,"Filename 1 is too large\n"); return 1; }
  filename1 = (char *) malloc(len1);
  strncpy(filename1,argv[1],len1);
  if (filename1==0) { fprintf(stderr,"Could not allocate enough memory for filename1\n"); return 1; }

  //Pass argument 2 to filename2
  char * filename2=0;
  unsigned int len2 = strlen(argv[2]);
  if (len2 > MAX_STRING_TO_ACCEPT ) { fprintf(stderr,"Filename 2 is too large\n"); return 1; }
  filename2 = (char *) malloc(len2);
  strncpy(filename2,argv[2],len2);
  if (filename2==0) { fprintf(stderr,"Could not allocate enough memory for filename1\n"); return 1; }

  //Compare images
  fprintf(stderr,"Comparing %s with %s \n",filename1,filename2);
  int result = AIS_CompareImages(filename1,filename2);

  //Print result
  switch (result)
  {
      case COULD_NOT_PERFORM_COMPARISON : fprintf(stderr,"Error , could not perform comparison\n");  return 1; break;
      case DIFFERENT : printf("Different\n");  break;
      case SIMILAR   : printf("Similar\n");    break;
      case IDENTICAL : printf("Identical\n");  break;
  }
 return 0;
}
