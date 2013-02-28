
#include "timers.h"
#include "../configuration.h"
#include <sys/time.h>
#include <unistd.h>

char * timerIDDescription[]    =   {
   "SEARCH_OPERATION_DELAY    " ,
   "LOAD_IMAGE_DELAY          " ,
   "FACE_DETECTION_DELAY      " ,
   "FIND_OBJECTS_DELAY        " ,
   "HISTOGRAM_GENERATION_DELAY" ,
   "HISTOGRAM_COMPARISON_DELAY" ,
   "IMAGE_COMPARISON_DELAY    " ,
   "IMAGE_RESIZE_DELAY        "
                                  };


struct TimerArrItem
{
   struct timeval starttime;
   struct timeval endtime;
   struct timeval timediff;

   unsigned int last_time;
   unsigned int total_time;
   unsigned int times_counted;
};


struct TimerArrItem timers_array[TOTAL_TIMERS];


long timeval_diff ( struct timeval *difference, struct timeval *end_time, struct timeval *start_time )
{

   struct timeval temp_diff;

   if(difference==0) { difference=&temp_diff; }

  difference->tv_sec =end_time->tv_sec -start_time->tv_sec ;
  difference->tv_usec=end_time->tv_usec-start_time->tv_usec;

  /* Using while instead of if below makes the code slightly more robust. */

  while(difference->tv_usec<0)
  {
    difference->tv_usec+=1000000;
    difference->tv_sec -=1;
  }

  return 1000000LL*difference->tv_sec+ difference->tv_usec;

}

void StartTimer(timerID timer_num )
{
  #if USE_TIMERS
   gettimeofday(&timers_array[timer_num].starttime,0x0);
  #endif
   return ;
}

unsigned int EndTimer(timerID timer_num )
{
  #if USE_TIMERS
  gettimeofday(&timers_array[timer_num].endtime,0x0);


  timers_array[timer_num].last_time = timeval_diff(&timers_array[timer_num].timediff,&timers_array[timer_num].endtime,&timers_array[timer_num].starttime);

  timers_array[timer_num].total_time+=timers_array[timer_num].last_time;
  ++timers_array[timer_num].times_counted;


  if ( timers_array[timer_num].total_time > 900000 )
    {
          timers_array[timer_num].total_time = timers_array[timer_num].total_time / 2;
          timers_array[timer_num].times_counted = timers_array[timer_num].times_counted / 2;
    }


  return timers_array[timer_num].last_time;
  #else
   return 0;
  #endif // USE_TIMERS
  return 0;
}

unsigned int GetLastTimer(timerID timer_num )
{
  return timers_array[timer_num].last_time;
}

unsigned int GetAverageTimer(timerID timer_num )
{
  if (timers_array[timer_num].times_counted == 0 ) { return 0; }
  return (unsigned int) timers_array[timer_num].total_time/timers_array[timer_num].times_counted;
}

unsigned int GetTimesTimerTimed(timerID timer_num )
{
  return  timers_array[timer_num].times_counted;
}


void sleepMilliseconds(unsigned int milliseconds)
{
    usleep(milliseconds*1000);
}

void sleepMicroseconds(unsigned int microseconds)
{
    usleep(microseconds);
}
