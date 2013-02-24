/***************************************************************************
* Copyright (C) 2010 by Ammar Qammaz *
* ammarkov@gmail.com *
* *
* This program is free software; you can redistribute it and/or modify *
* it under the terms of the GNU General Public License as published by *
* the Free Software Foundation; either version 2 of the License, or *
* (at your option) any later version. *
* *
* This program is distributed in the hope that it will be useful, *
* but WITHOUT ANY WARRANTY; without even the implied warranty of *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the *
* GNU General Public License for more details. *
* *
* You should have received a copy of the GNU General Public License *
* along with this program; if not, write to the *
* Free Software Foundation, Inc., *
* 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. *
***************************************************************************/

#include "codecs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jpg.h"

struct Image * readImage( char *filename,unsigned int type,char read_only_header)
{
   struct Image * pic = (struct Image *) malloc(sizeof(struct Image));
   memset(pic,0,sizeof(struct Image));

   switch (type)
   {
      case JPG_CODEC :
       if (!ReadJPEG(filename,pic,read_only_header)) { free(pic); pic=0; }
      break;
      default :
      free(pic);
      pic=0;
      break;
   };

   return pic;
}


int writeImage( char *filename,unsigned int type,struct Image * pic,char *mem,unsigned long * mem_size)
{
   switch (type)
   {
      case JPG_CODEC :
       return WriteJPEG(filename,pic,mem,mem_size);
   };

   return 0;
}
