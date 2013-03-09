#ifndef PATTERNRECOGNITION_H_INCLUDED
#define PATTERNRECOGNITION_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif


struct ptrnImage
{
  unsigned char * pixels;
  unsigned int width;
  unsigned int height;
  unsigned int depth;
  unsigned int image_size;
};


int detectPattern(struct ptrnImage * pattern,struct ptrnImage * img);



#ifdef __cplusplus
}
#endif

#endif // PATTERNRECOGNITION_H_INCLUDED
