#ifndef _PPM_H_INCLUDED
#define _PPM_H_INCLUDED

#include "codecs.h"


int ReadPPM(char * filename,struct Image * pic);
int WritePPM(char * filename,struct Image * pic);


#endif
