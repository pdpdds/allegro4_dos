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


#ifndef timer_handler_H
#define timer_handler_H


#ifdef __cplusplus
   extern "C" {
#endif


/** install_int* returns 0 on success or a negative number if there aren't enough timers available
      The StartTimer, and SetupAndStartTimer functions return the same values as install_int* so that
      you can check if installing the timer was successful. If you don't call SetupAndStartTimer, then you
      will need to call SetupTimerHandler before calling SetTimer* and then StartTimer. */

void   SetupTimerHandler (void);

int    SetupAndStartTimer(int rate);

int    StartTimer        (void);
void   StopTimer         (void);

void   ToggleTimerPause  (void);
int    IsTimerPaused     (void);
int    IsTimerRunning    (void);

void   SetTimerRate  (int rate);
void   SetTimerBPS   (int bps);
void   SetTimerMSPT  (int ms);

int    GetTimerRate();

// Both GetDelta* functions also clear the timer ticks to 0.
double GetDeltaTime  (void);
int    GetDeltaTicks (void);

double GetSecPerTick (void);
double GetTicksPerSec(void);

int    GetTickCount  (void);
void   SetTickCount  (int);
void   ClearTickCount(void);




#ifdef __cplusplus
   }
#endif


#endif // timer_handler_H




