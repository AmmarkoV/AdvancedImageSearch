
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

   unsigned int last_time;
   unsigned int guard1;
   unsigned int total_time;
   unsigned int guard2;
   unsigned int times_counted;
   unsigned int guard3;
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
  int i=0;
  for (i=0; i<TOTAL_TIMERS; i++)
  {
      timers_array[i].guard1=GUARD_VALUE;
      timers_array[i].guard2=GUARD_VALUE;
      timers_array[i].guard3=GUARD_VALUE;
  }
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
   if (timer_num>=TOTAL_TIMERS) { fprintf(stderr,"StartTimer: Incorrect timer requested\n"); return; }
   gettimeofday(&timers_array[timer_num].starttime,0x0);
  #endif
   return ;
}

unsigned int EndTimer(timerID timer_num )
{
  if ( (timer_num>=TOTAL_TIMERS)||(timers_array==0) ) { return 0; }
  #if USE_TIMERS
  if (timer_num>=TOTAL_TIMERS) { fprintf(stderr,"EndTimer: Incorrect timer requested\n"); return 0; }
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

int checkGuard(timerID i)
{
  if ( (i>=TOTAL_TIMERS)||(timers_array==0) ) { return 0; }
  if ((timers_array[i].guard1!=GUARD_VALUE) ||
      (timers_array[i].guard2!=GUARD_VALUE) ||
      (timers_array[i].guard3!=GUARD_VALUE) )
   {
       fprintf(stderr,"Corrupted Timer %u , %s \n",i,timerIDDescription[i]);
       return 0;
   }
   return 1;
}


unsigned int GetLastTimer(timerID timer_num )
{
  if ( (timer_num>=TOTAL_TIMERS)||(timers_array==0) ) { return 0; }
  checkGuard(timer_num);
  return timers_array[timer_num].last_time;
}

unsigned int GetAverageTimer(timerID timer_num )
{
  if ( (timer_num>=TOTAL_TIMERS)||(timers_array==0) ) { return 0; }
  if (timers_array[timer_num].times_counted == 0 ) { return 0; }
  checkGuard(timer_num);
  return (unsigned int) timers_array[timer_num].total_time/timers_array[timer_num].times_counted;
}

unsigned int GetTimesTimerTimed(timerID timer_num )
{
  if ( (timer_num>=TOTAL_TIMERS)||(timers_array==0) ) { return 0; }
  checkGuard(timer_num);
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
