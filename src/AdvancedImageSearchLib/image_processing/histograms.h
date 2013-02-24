#ifndef HISTOGRAMS_H_INCLUDED
#define HISTOGRAMS_H_INCLUDED



struct histogramChannel
{
  unsigned int intensity[255];
};



struct Histogram
{
    struct histogramChannel channel[3];
};

int generateHistogram(char * rgb , unsigned int width , unsigned int height , unsigned int channels , struct Histogram * hist );

#endif // HISTOGRAMS_H_INCLUDED
