#ifndef _GET_TIME_H
#define _GET_TIME_H

#ifndef _TYPES_H
#include "types.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

void init_time();      //to initialize time stuff
void deinit_time();    //to de-initialize time stuff

/*
three forms of get_time
NAME / ALTERNATE NAME				RETURNS	UNITS			PRECISION	SPEED
get_time() / get_time_ms()			int		milliseconds	moderate	fast
get_time_s() / get_time_seconds()	double	seconds			high		slow on WIN32
get_time_ticks()					Sint64	unspecified		high		fast on intel

the current time is measured relative to the call 
	to init_time() for get_time_ms() and get_time_s(), but NOT for get_time_ticks()
*/

volatile int    get_time();   //to get the current time in milliseconds
#define get_time_ms get_time

volatile double get_time_s(); //to get the current time in seconds at high precision
#define get_time_seconds get_time_s

volatile Sint64 get_time_ticks(); //to get the current time in unspecified units
volatile double get_time_tick_period();
/*note: The period of the ticks returned by get_time_ticks() may change over time
		and the value reported is only an aproximation.  
*/

//set this to determine how you want to react to errors
extern void (*get_time_error_function) (char *);

int idle ( int time ); //to yeild a number of milliseconds to the OS
extern int _no_idle;   //to disable the above function

#ifdef __cplusplus
}
#endif


#endif
