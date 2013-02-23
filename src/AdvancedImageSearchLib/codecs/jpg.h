#ifndef _JPG_H_INCLUDED
#define _JPG_H_INCLUDED


#include "codecs.h"

int ReadJPEG( char *filename,struct Image * pic);
int WriteJPEG( char *filename,struct Image * pic,char *mem,unsigned long * mem_size);

#endif // _JPG_H_INCLUDED
