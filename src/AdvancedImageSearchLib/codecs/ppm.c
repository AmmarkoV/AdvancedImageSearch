#include "codecs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ppm.h"

#define PPMREADBUFLEN 256
#define READ_CREATES_A_NEW_PIXEL_BUFFER 1


int ReadPPM(char * filename,struct Image * pic)
{
    FILE *pf=0;
    pf = fopen(filename,"rb");

    if (pf!=0 )
    {
        char buf[PPMREADBUFLEN], *t;
        unsigned int w=0, h=0, d=0;
        int r=0;

        t = fgets(buf, PPMREADBUFLEN, pf);
        if ( (t == 0) || ( strncmp(buf, "P6\n", 3) != 0 ) ) { fclose(pf); return 0; }
        do
        { /* Px formats can have # comments after first line */
           t = fgets(buf, PPMREADBUFLEN, pf);
           if ( t == 0 ) { fclose(pf); return 0; }
        } while ( strncmp(buf, "#", 1) == 0 );
        r = sscanf(buf, "%u %u", &w, &h);
        if ( r < 2 ) { fclose(pf); return 0; }
        // The program fails if the first byte of the image is equal to 32. because
        // the fscanf eats the space and the image is read with some bit less
        r = fscanf(pf, "%u\n", &d);
        if ( (r < 1) || ( d != 255 ) ) { fclose(pf); return 0; }

        if ( (w!=pic->width) || (h!=pic->height) )
           {
             fprintf(stderr,"Incorrect file size ( %s ) :P\n",filename);
             if ( w * h > pic->width * pic->height )
               {
                 fprintf(stderr,"File %s will lead to overflow stopping read..\n",filename);
                 fclose(pf);
                 return 0;
               }
           }


      #if READ_CREATES_A_NEW_PIXEL_BUFFER
	    pic->pixels = (char * ) malloc(pic->height*pic->width * 3 * sizeof(char));
	  #endif


        if ( pic->pixels != 0 )
        {
            size_t rd = fread(pic->pixels,3, w*h, pf);
            fclose(pf);
            if ( rd < w*h )
            {
               return 0;
            }
            return 1;
        }
        fclose(pf);
    }
  return 0;
}

int WritePPM(char * filename,struct Image * pic)
{

    FILE *fd=0;
    fd = fopen(filename,"wb");

    if (fd!=0)
	{
     unsigned int n=0;

     fprintf(fd, "P6\n%d %d\n255\n", pic->width, pic->height);
     n = (unsigned int ) ( pic->width * pic->height ) ;

     fwrite(pic->pixels, 3, n, fd);

     fflush(fd);
     fclose(fd);

     return 1;
	}

  return 0;
}
