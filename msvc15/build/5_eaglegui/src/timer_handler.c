/*
 *
 *     _______       ___       ____      __       _______ 
 *    /\  ____\    /|   \     /  __\    /\ \     /\  ____\ 
 *    \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_ 
 *     \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\ 
 *      \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_ 
 *       \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *        \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    EAGLE
 *    Edgar's Allegro Gui Library and Extensions
 *
 *    Copyright 2009-2011 by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */


#include "Eagle/timer_handler.h"
#include <allegro.h>



double ticks_per_second = 60.0;// Timer ticks per second
double seconds_per_tick = 1.0/60.0;// Seconds per timer tick
double ms_per_tick      = 1000.0/60.0;// Milliseconds per tick
int    ticker_rate      = BPS_TO_TIMER(60);  // Hardware ticks per second
int    timer_running    = 0;  // boolean int
int    timer_paused     = 0;
int    timer_setup      = 0;

volatile unsigned int ticks = 0;
void EagleTicker() {
   if (!timer_paused) {++ticks;}
}
END_OF_FUNCTION(EagleTicker);



/**
From allegro's timer.h :

#define TIMERS_PER_SECOND     1193181L
#define SECS_TO_TIMER(x)      ((long)(x) * TIMERS_PER_SECOND)
#define MSEC_TO_TIMER(x)      ((long)(x) * (TIMERS_PER_SECOND / 1000))
#define BPS_TO_TIMER(x)       (TIMERS_PER_SECOND / (long)(x))
#define BPM_TO_TIMER(x)       ((60 * TIMERS_PER_SECOND) / (long)(x))

*/

/* TODO : Only DJGPP, Mac, and Watcom platforms define LOCK_VARIABLE and LOCK_FUNCTION 
          as actually doing something. */

void SetupTimerHandler(void) {
   if (!timer_setup) {
      LOCK_VARIABLE(ticks);
      LOCK_FUNCTION(EagleTicker);
   }
   timer_setup = 1;
}



int SetupAndStartTimer(int rate) {
   ASSERT(rate > 0);
   SetupTimerHandler();
   SetTimerRate(rate);
   return StartTimer();
}



int StartTimer(void) {
   int ret = 0;
   ASSERT(ticker_rate > 0);
   if (!timer_setup) {
      SetupTimerHandler();
   }
   ret = install_int_ex(EagleTicker , ticker_rate);
   if (ret == 0) {
      timer_running = 1;
   } else {
      timer_running = 0;
   }
   return ret;
}



void StopTimer(void) {
   remove_int(EagleTicker);
   timer_running = 0;
}



void ToggleTimerPause(void) {
   if (timer_paused) {
      timer_paused = 0;
   } else {
      timer_paused = 1;
   }
}



int IsTimerPaused(void) {
   return timer_paused;
}



int IsTimerRunning(void) {
   return timer_running;
}



void SetTimerRate(int rate) {
   ASSERT(rate > 0);
   
   ticker_rate = rate;
   
/* Hardware ticks per second divided by Hardware ticks per timer tick equals timer ticks per second */
   ticks_per_second = (double)TIMERS_PER_SECOND/(double)ticker_rate;
   seconds_per_tick = 1.0/ticks_per_second;
   ms_per_tick = (double)ticker_rate/(double)(TIMERS_PER_SECOND/1000);
   
   if (timer_running) {install_int_ex(EagleTicker , ticker_rate);}
}



void SetTimerBPS(int bps) {
   ASSERT(bps > 0);
   
   ticker_rate = BPS_TO_TIMER(bps);
   ticks_per_second = (double)bps;
   seconds_per_tick = 1.0/ticks_per_second;
   ms_per_tick = (double)ticker_rate/(double)(TIMERS_PER_SECOND/1000);
   
   if (timer_running) {install_int_ex(EagleTicker , ticker_rate);}
}



void SetTimerMSPT(int ms) {
   ASSERT(ms > 0);
   
   ms_per_tick = (double)ms;
   ticker_rate = MSEC_TO_TIMER(ms);
   ticks_per_second = (double)TIMERS_PER_SECOND/(double)ticker_rate;
   seconds_per_tick = 1.0/ticks_per_second;
   
   if (timer_running) {install_int_ex(EagleTicker , ticker_rate);}
}



int GetTimerRate() {
   return ticker_rate;
}


/** Returns how much time has passed in seconds or ticks since the last call to GetDelta*() */

double GetDeltaTime(void) {
   double dt = (double)ticks*seconds_per_tick;
   ticks = 0;
   return dt;
}



int GetDeltaTicks(void) {
   int dt = ticks;
   ticks = 0;
   return dt;
}



double GetSecPerTick (void)           {return seconds_per_tick;}
double GetTicksPerSec(void)           {return ticks_per_second;}
int    GetTickCount  (void)           {return ticks;}
void   SetTickCount  (int tick_count) {ticks = tick_count;}
void   ClearTickCount(void)           {ticks = 0;}



