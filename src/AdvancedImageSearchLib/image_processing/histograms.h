#ifndef HISTOGRAMS_H_INCLUDED
#define HISTOGRAMS_H_INCLUDED

struct histogramChannel
{
  unsigned int intensity[256];
};

struct Histogram
{
  struct histogramChannel channel[3];
};

struct Histogram *  generateHistogram(unsigned char * rgb , unsigned int width , unsigned int height , unsigned int channels );
int histogramIsCloseToColor(struct Histogram * hist,unsigned char R,unsigned char G,unsigned char B,unsigned char Deviation,unsigned int imageSize,float targetPercentage);

#endif // HISTOGRAMS_H_INCLUDED
