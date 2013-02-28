#ifndef _PNGINPUT_H_INCLUDED
#define _PNGINPUT_H_INCLUDED

#include "codecs.h"

int ReadPNG( char *filename,struct Image * pic,char read_only_header);
int WritePNG(char * filename,struct Image * pic);

#endif // IMAGE_STORAGE_PNG_H_INCLUDED
