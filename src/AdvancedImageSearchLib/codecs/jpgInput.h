#ifndef _JPGINPUT_H_INCLUDED
#define _JPGINPUT_H_INCLUDED


#include "codecs.h"

int ReadJPEG( char *filename,struct Image * pic,char read_only_header);
int WriteJPEG( char *filename,struct Image * pic,char *mem,unsigned long * mem_size);

#endif // _JPG_H_INCLUDED
