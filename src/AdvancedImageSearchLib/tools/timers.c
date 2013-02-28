
#include "timers.h"
#include "../configuration.h"
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define GUARD_VALUE 123123

char * timerIDDescription[]    =   {
   "SEARCH_OPERATION_DELAY    " ,
   "LOAD_IMAGE_DELAY          " ,
   "FACE_DETECTION_DELAY      " ,
   "FIND_OBJECTS_DELAY        " ,
   "HISTOGRAM_GENERATION_DELAY" ,
   "HISTOGRAM_COMPARISON_DELAY" ,
   "IMAGE_COMPARISON_DELAY    " ,
   "IMAGE_RESIZE_DELAY        " ,
   "TOTAL TIMERS              "
                                  };


struct TimerArrItem
{
   struct timeval starttime;
   struct timeval endtime;
   struct timeval timediff;

   unsigned int minimum_time;
   unsigned int maximum_time;
   unsigned int last_time;
   unsigned int total_time;
   unsigned int times_counted;
};


struct TimerArrItem * timers_array=0;

void initTimers()
{
  if(timers_array==0) {  timers_array = (struct TimerArrItem * ) malloc(sizeof(struct TimerArrItem)*(TOTAL_TIMERS)); }
  if(timers_array==0)
  {  //If we still couldn't initialize them..!
     fprintf(stderr,"Could not initialize timers , timing operations will fail..\n");
     return ;
  }

  memset(timers_array,0,sizeof(struct TimerArrItem)*(TOTAL_TIMERS) );
}

void destroyTimers()
{
   if(timers_array!=0) { free(timers_array); }
}


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
  if ( (timer_num>=TOTAL_TIMERS)||(timers_array==0) ) { return; }
  #if USE_TIMERS
   gettimeofday(&timers_array[timer_num].starttime,0x0);
  #endif
   return ;
}

unsigned int EndTimer(timerID timer_num )
{
  if ( (timer_num>=TOTAL_TIMERS)||(timers_array==0) ) { return 0; }
  #if USE_TIMERS

  gettimeofday(&timers_array[timer_num].endtime,0x0);

  timers_array[timer_num].last_time = timeval_diff(&timers_array[timer_num].timediff,&timers_array[timer_num].endtime,&timers_array[timer_num].starttime);

  timers_array[timer_num].total_time+=timers_array[timer_num].last_time;

  if (timers_array[timer_num].times_counted==0)
      { //First sample , initialize min max values
        timers_array[timer_num].minimum_time=timers_array[timer_num].last_time;
        timers_array[timer_num].maximum_time=timers_array[timer_num].last_time;
      } else
      {
        if ( timers_array[timer_num].minimum_time>timers_array[timer_num].last_time )
                { timers_array[timer_num].minimum_time=timers_array[timer_num].last_time; }
        if ( timers_array[timer_num].maximum_time<timers_array[timer_num].last_time )
                { timers_array[timer_num].maximum_time=timers_array[timer_num].last_time; }
      }

  ++timers_array[timer_num].times_counted;
  if ( timers_array[timer_num].total_time > 3294967295 )
    { /*THIS MEANS WE ARE GOING OVER THE ROOF , SO WE WILL HALF OUR TIME AND SAMPLE FOR THE SAKE OF NOT OVERFLOWING!*/
          timers_array[timer_num].total_time = timers_array[timer_num].total_time / 2;
          timers_array[timer_num].times_counted = timers_array[timer_num].times_counted / 2;
    }


  return timers_array[timer_num].last_time;
  #else
   return 0;
  #endif // USE_TIMERS
  return 0;
}

unsigned int GetMinimumTimer(timerID timer_num )
{
  if ( (timer_num>=TOTAL_TIMERS)||(timers_array==0) ) { return 0; }
return timers_array[timer_num].minimum_time;
}

unsigned int GetMaximumTimer(timerID timer_num )
{
  if ( (timer_num>=TOTAL_TIMERS)||(timers_array==0) ) { return 0; }
  return timers_array[timer_num].maximum_time;
}

unsigned int GetLastTimer(timerID timer_num )
{
  if ( (timer_num>=TOTAL_TIMERS)||(timers_array==0) ) { return 0; }
  return timers_array[timer_num].last_time;
}

unsigned int GetAverageTimer(timerID timer_num )
{
  if ( (timer_num>=TOTAL_TIMERS)||(timers_array==0) ) { return 0; }
  if (timers_array[timer_num].times_counted == 0 ) { return 0; }
  return (unsigned int) timers_array[timer_num].total_time/timers_array[timer_num].times_counted;
}


unsigned int GetTimesTimerTimed(timerID timer_num )
{
  if ( (timer_num>=TOTAL_TIMERS)||(timers_array==0) ) { return 0; }
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
