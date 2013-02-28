#ifndef VISCORTEXTIMER_H_INCLUDED
#define VISCORTEXTIMER_H_INCLUDED



enum TimerList
{
   SEARCH_OPERATION_DELAY = 0 ,
   LOAD_IMAGE_DELAY ,
   FACE_DETECTION_DELAY ,
   FIND_OBJECTS_DELAY ,
   HISTOGRAM_GENERATION_DELAY ,
   HISTOGRAM_COMPARISON_DELAY ,
   IMAGE_COMPARISON_DELAY ,
   IMAGE_RESIZE_DELAY ,
   //------------------------
   TOTAL_TIMERS
};

extern char * timerIDDescription[];
typedef unsigned int timerID;

void initTimers();
void destroyTimers();

void StartTimer(timerID timer_num );
unsigned int EndTimer(timerID timer_num );
unsigned int GetLastTimer(timerID timer_num );
unsigned int GetAverageTimer(timerID timer_num );
unsigned int GetTimesTimerTimed(timerID timer_num );

void sleepMilliseconds(unsigned int milliseconds);
void sleepMicroseconds(unsigned int microseconds);
#endif // VISCORTEXTIMER_H_INCLUDED
