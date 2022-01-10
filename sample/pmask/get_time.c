//#define NO_WINMM_TGT
#define NO_WINMM_QPC
#define NO_RDTSC
#define NO_LIBC
//#define NO_ALLEGRO_TIME
//#define NO_SDL_TIME
//#define NO_GETTIMEOFDAY

//#define PLATFORM_IS_ALLEGRO
//#define PLATFORM_IS_SDL

/*
These #defines are used to control what underlying system-dependant 
time functions are used to calculate the current time.  

WINMM_TGT:
	requires: Windows, linking with winmm.lib
	resolution: varies, between 1 and 10 milliseconds
	efficiency: fast
	other notes: recommended method of timing on Windows at this time

WINMM_QPC:
	requires: Windows, linking with winmm.lib
	resolution: varies, typically between 1 nanosecond and 1 microseconds
	efficiency: slow, typically 0.1-1.0 microseconds per call
	other notes: buggy on many computers

RDTSC:
	requires: x86 (including x86-64) CPU, Pentium or more recent
	resolution: varies, typically between 0.1 and 30 nanoseconds
	efficiency: fast, typically between 5 and 100 nanoseconds
	other notes: buggy on some multi-CPU systems (different times on diff. CPUs)

gettimeofday:
	requires: UNIX (including linux, and many other unix-like systems)
	resolution: varies, at best 1 microsecond, at worst ?100 microseconds? (guess)
	efficiency: no idea (untested)
	other notes: may run backwards due if the system time is adjusted

LIBC:
	requires: an implementation of the C standard library
	resolution: shitty: 1 second
	efficiency: varies
	other notes: the method of last resort

Allegro:
	requires: Allegro
	resolution: typically between 1 and 10 milliseconds
	efficiency: extremely fast on a per-call basis, but high overhead to install at all
	other notes: none

SDL:
	requires: SDL
	resolution: typically between 1 and 10 milliseconds
	efficiency: ?fast? (untested)
	other notes: none

other methods to consider for future use:
	GetTickCount:
		requires: Windows ???
		resolution: typically between 55 and ?1? milliseconds
		efficiency: ???
		other notes: ???
	libC clock:
		requires: standard C library
		resolution: frequently 55 milliseconds
		efficiency: ???
		other notes: documentation varies widely, inconsistent
	clock_gettime:

*/


#ifndef NO_LIBC
#	include <time.h>
#	include <math.h>
#endif
#include "types.h"
#include "get_time.h"
//#include "errors.h"

//static void get_time_error_function_impl ( const char *desc ) {}
//void (*get_time_error_function) (char *) = get_time_error_function_impl;
void (*get_time_error_function) (char *) = 0;
static void get_time_error(char * msg) {
	get_time_error_function(msg);
}


#ifdef NO_ALLEGRO_TIME
#	undef PLATFORM_IS_ALLEGRO
#endif
#ifdef NO_SDL_TIME
#	undef PLATFORM_IS_SDL
#endif

//converting all x86-declaring preprocessor symbols to __x86__
#ifndef __x86__
#	if defined __i386__ || defined __i386 || defined i386 || defined __I386__ || defined _M_IX86 || defined _X86 || defined __386__ || defined __x86__
#		define __x86__
#	endif
#	if defined __x86_64__ || defined _M_X64
#		define __x86__
#	endif
#endif
#ifndef __x86_64__
#	if defined __x86_64__ || defined _M_X64
#		define __x86__
#		define __x86_64__
#	endif
#endif

//same for unix
#ifndef __unix__
#	if defined __unix || defined _unix || defined _UNIX || defined __UNIX__ || defined __UNIX || defined __unix_
#		define __unix__
#	endif
#endif
//same for win32
#ifndef WIN32
#	if defined _WIN32 || defined __WIN32
#		define WIN32
#	endif
#endif

/*
------------------------------
		Timers & Stuff
------------------------------

Three sections:
1.  idle() functions for Allegro / SDL / unrecognized platform
2.  Platform-specific time functions:
		LibC time returns integer seconds (fast?)
		Allegro interrupt returns integer milliseconds (fast)
		SDL ticks returns integer milliseconds (fast?)
		Windows Multi-Media timeGetTime returns integer milliseconds (fast)
		Windows Multi-Media Performance Counter returns integer high resolution ticks (slow)
		Intel RDTSC returns integer high resolution ticks (fast)
		unix gettimeofday() returns integer microseconds (???)
3.  Wrappers to pick which time functions to use
		get_time_ms() order of preference:
			Windows Multi-Media timeGettime
			SDL ticks
			Allegro interrupt
			unix gettimeofday()
			Windows Multi-Media Performance Counter
			Libc time
		get_time_s() order of preference:
			Windows Multi-Media Performance Counter
			unix gettimeofday()
			get_time_ms()
		get_time_ticks() order of preference:
			Intel RDTSC
			Windows Multi-Media Performance Counter
			unix gettimeofday()
			get_time_ms()
*/







//1.  idle() functions for Allegro / SDL / unknown platform




int _no_idle = 0;

#if defined(PLATFORM_IS_ALLEGRO) && !defined(NO_ALLEGRO_TIME)
#	include <allegro.h>
#	if defined WIN32
#		include <winalleg.h>
#	endif
	int idle ( int time ) {
		if (_no_idle) return 0;
		rest(time);
		return time;
	}
#elif defined(PLATFORM_IS_SDL) && !defined(NO_SDL_TIME)
#	include <SDL.h>
	int idle ( int time ) {
		if (_no_idle) return 0;
		SDL_Delay ( time );
		return time;
	}
#else
//#	error get_time.c - unknown platform
	int idle ( int time ) {
		if (_no_idle) return 0;
		return 0;
		/*int i = get_time() + time;
		while (i > get_time()) {
			//do nothing real fast
		}
		return time;*/
	}
#endif




//2.  Platform specific time functions:



#if !defined NO_GETTIMEOFDAY && defined __unix__
#	define GETTIMEOFDAY_TIME
#	include <sys/time.h>
	static volatile Sint64 gettimeofday_get_time_microseconds() {
		timeval tv;
		gettimeofday( &tv, NULL );
		return (Sint64(tv.tv_sec) * 1000000) + tv.tv_usec;
	}
	static volatile double gettimeofday_get_time_seconds() {
		timeval tv;
		gettimeofday( &tv, NULL );
		return tv.tv_sec + 0.000001d * tv.tv_usec;
	}
#endif

//RDTSC:
//RDTSC is only valid on pentiums & later, right?
//so how do we detect this at run-time? (without crashing)
#if !defined NO_RDTSC
#	if defined _MSC_VER && defined __x86__
#		define RDTSC_TIMER
		static volatile Sint64 rdtsc_get_time() {
			unsigned int a, b;
			__asm RDTSC
			__asm mov a, eax
			__asm mov b, edx
			return (a + ((Sint64)b << 32));
		}
#	elif defined __GNUC__ && defined __x86__
#		define RDTSC_TIMER
		static __volatile__ Uint64 rdtsc_get_time() {
//			unsigned int a, b;
//			asm ("RDTSC" : "=a" (a), "=d" (b) : );
//			return (a + ((Sint64)b << 32));
			unsigned long long int r;
			asm ("RDTSC" : "=A" (r) : );
			return r;
		}
#	endif
#endif

#if defined WIN32 && !(defined NO_WINMM_TGT && defined NO_WINMM_QPC)
#	include <windows.h>
#	include <Mmsystem.h>
#	include <winbase.h>

#	if !defined NO_WINMM_TGT
#		define WINMM_TGT
#		define winmm_tgt_period_i 1
		static int winmm_tgt_get_time() {
			return timeGetTime();
		}
#	endif

#	if !defined NO_WINMM_QPC
#		define WINMM_QPC
		static double winmm_qpc_period_f = 1;
		static int winmm_qpc_period_i = 0;
		static Uint64 winmm_qpc_base = 0;
		static volatile Uint64 winmm_qpc_get_time() {
			LARGE_INTEGER bob;
			if (QueryPerformanceCounter(&bob)) {
				return bob.QuadPart;
			}
			get_time_error ( "Windows Performance Counter failed\n");
			return -1;
		}
		static double winmm_qpc_get_freq() {
			LARGE_INTEGER fred;
			if (QueryPerformanceFrequency(&fred)) {
				return (double)fred.QuadPart;
			}
			get_time_error ( "Windows Performance Counter failed\n");
			return 0;
		}
#	endif
#endif

#if defined PLATFORM_IS_ALLEGRO
#	include <allegro.h>
#	define allegro_period 5
	static volatile int allegro_time = 0;
	static volatile int allegro_get_time() {
		return allegro_time;
	}
	static void allegro_timer_function(void) {
		allegro_time += allegro_period;
	}
	END_OF_STATIC_FUNCTION(allegro_timer_function);
	static int allegro_time_inited = 0;
	void init_allegro_time() {
		if (allegro_time_inited++) return;
		LOCK_FUNCTION(allegro_timer_function);
		LOCK_VARIABLE(allegro_time);
		if (install_timer() < 0) get_time_error("Allegro timer initialization failed");
		install_int(&allegro_timer_function, allegro_period);
	}
	void deinit_allegro_time() {
		if (--allegro_time_inited > 0) return;
		if (allegro_time_inited < 0) get_time_error("get_time.c - deinit_allegro_time() called incorrectly");
		remove_int(&allegro_timer_function);
	}
#elif defined PLATFORM_IS_SDL
#	include <SDL.h>
	static int sdl_time_inited = 0;
	static int sdl_get_time() {
		return SDL_GetTicks();
	}
	static void init_sdl_time() {
		if (sdl_time_inited++) return;
		SDL_InitSubSystem(SDL_INIT_TIMER);
	}
	static void deinit_sdl_time() {
		if (--sdl_time_inited > 0) return;
		if (sdl_time_inited < 0) get_time_error("get_time.c - deinit_sdl_time() called incorrectly");
		SDL_QuitSubSystem(SDL_INIT_TIMER);
	}
#else
#endif

#if !defined NO_LIBC
#	define LIBC_TIME
	int libc_get_time() {
		return time(NULL);
	}
#endif





//3.  The exported wrappers for time functions




//volatile int    get_time();
/*	get_time() order of preference:
		Windows Multi-Media timeGettime
		SDL ticks
		Allegro interrupt
		unix gettimeofday()
		Windows Multi-Media Performance Counter
		Libc time
*/
static int get_time_ms_base;
#if 0
#elif defined WINMM_TGT//
	volatile int get_time() {return winmm_tgt_get_time() - get_time_ms_base;}
	static void init_get_time_ms() {};
	static void deinit_get_time_ms() {};
#elif defined PLATFORM_IS_SDL//
	volatile int get_time() {return sdl_get_time() - get_time_ms_base;}
	static void init_get_time_ms() {init_sdl_time();}
	static void deinit_get_time_ms() {deinit_sdl_time();}
#elif defined PLATFORM_IS_ALLEGRO
	volatile int get_time() {return allegro_get_time() - get_time_ms_base;}
	static void init_get_time_ms() {init_allegro_time();};
	static void deinit_get_time_ms() {deinit_allegro_time();};
#elif defined GETTIMEOFDAY_TIME
	volatile int get_time() {return int(gettimeofday_get_time_microseconds() / 1000) - get_time_ms_base;}
	static void init_get_time_ms() {};
	static void deinit_get_time_ms() {};
/*
#	elif defined WINMM_QPC
	static double get_time_ms_period = 0;
	volatile int get_time() {
		return (int)(winmm_qpc_get_time() * get_time_ms_period) - get_time_ms_base;
	}
	static void init_get_time_s() {get_time_ms_period = 1000.0 / winmm_qpc_get_freq();};
	static void deinit_get_time_s() {get_time_ms_period = 0;};
*/
#elif defined LIBC_TIME
	volatile int get_time() {return libc_get_time() - get_time_ms_base;}
	static void init_get_time_ms() {};
	static void deinit_get_time_ms() {};
#else
#	error No millisecond time function specified!
#endif


//volatile double get_time_s();
/*	get_time_s() order of preference:
		Windows Multi-Media Performance Counter
		unix gettimeofday()
		get_time_ms()
*/
static double get_time_s_base;
#	if 0
#	elif defined WINMM_QPC
	static double get_time_s_period = 0;
	volatile double get_time_s() {
		return winmm_qpc_get_time() * get_time_s_period - get_time_s_base;
	}
	static void init_get_time_s() {get_time_s_period = 1.0/winmm_qpc_get_freq();};
	static void deinit_get_time_s() {get_time_s_period = 0;};
#elif defined GETTIMEOFDAY_TIME
	volatile double get_time_s() {return gettimeofday_get_time_s() - get_time_s_base;}
	static void init_get_time_s() {};
	static void deinit_get_time_s() {};
#	else
	volatile double get_time_s() {return get_time_ms() * 0.001;}
	static void init_get_time_s() {init_get_time_ms();};
	static void deinit_get_time_s() {deinit_get_time_ms();};
#	endif


//volatile Sint64    get_time_ticks();
/*	get_time3() order of preference:
		Intel RDTSC
		unix gettimeofday()
		Windows Multi-Media Performance Counter
		get_time_ms()
*/
static Sint64 get_time_ticks_inited;
#if 0
#elif defined RDTSC_TIMER
	//require debuging mode, since we don't have run-time check
	volatile Sint64 get_time_ticks() {return rdtsc_get_time();}
	static void init_get_time_ticks() {};
	static void deinit_get_time_ticks() {};
#	define NEED_GUESS_TICKS
#elif defined WINMM_QPC
	volatile Sint64 get_time_ticks() {return winmm_qpc_get_time();}
	static void init_get_time_ticks() {};
	static void deinit_get_time_ticks() {};
	volatile double get_time_tick_period() {return 1.0 / winmm_qpc_get_freq();}
#elif defined GETTIMEOFDAY_TIME
	volatile Sint64 get_time_ticks() {return gettimeofday_get_time_microseconds();}
	static void init_get_time_s() {};
	static void deinit_get_time_s() {};
	volatile double get_time_tick_period() {return 0.000001;}
#else
	volatile Sint64 get_time_ticks() {return get_time_ms();}
	static void init_get_time_ticks() {init_get_time_ms();};
	static void deinit_get_time_ticks() {deinit_get_time_ms();};
	volatile double get_time_tick_period() {return 0.001;}
#endif

#ifdef NEED_GUESS_TICKS
	volatile double get_time_tick_period() {
		return get_time_s() / (get_time_ticks() - get_time_ticks_inited);
	}
#endif

static unsigned char time_inited = 0;
void init_time() {
	if (time_inited++) get_time_error("get_time - init_time() called repeatedly w/o deinit_time()");
	init_get_time_ms();
	init_get_time_s();
	init_get_time_ticks();
	get_time_s_base = get_time_s();
	get_time_ms_base = get_time_ms();
	get_time_ticks_inited = get_time_ticks();
/*	int ms;
	if (timer_attributes & 1) return;
	timer_attributes |= 1;
#	if defined WINMM_TGT
	{
		int tmp = winmm_tgt_get_time();
		int i;
		for (i = 0; i < 1000000; i += 1) 
			if (winmm_tgt_get_time() != tmp) break;
		winmm_tgt_base = winmm_tgt_get_time();
	}
#	endif
#	if defined PLATFORM_IS_SDL
		init_sdl_time();
		sdl_base = sdl_get_time();
#	endif
#	if defined PLATFORM_IS_ALLEGRO
		LOCK_FUNCTION(get_time);
		init_allegro_time();
		allegro_base = allegro_get_time();
#	endif
#	if defined LIBC_TIME
		libc_base = libc_get_time();
#	endif

	ms = get_time();
//finished initializing get_time()
//now we do get_time2() / get_time3()

#	if defined RDTSC_TIMER
		//require debuging mode, since we don't have a run-time check
		rdtsc_base = rdtsc_get_time();
#	endif
#	if defined WINMM_QPC
		winmm_qpc_base = winmm_qpc_get_time();
		winmm_qpc_period_f = 1000.0 / winmm_qpc_get_freq();
#	endif


#	if defined RDTSC_TIMER
	{
		Sint64 tmpl;
		double tmpd;
		while (get_time() - ms < 100) ;
		tmpl = rdtsc_get_time();
		tmpd = tmpl * (1.0 / (get_time() - ms));
#		if !defined NO_LIBC
		{
			double tmpd2, tmpd3;
			//most clock speeds are multiples of nice numbers
			//like 25.0 or 33.333 Mega-Hertz.  
			tmpd2 = floor(0.5 + tmpd / (100000/4.0)) * (100000/4.0);
			tmpd3 = floor(0.5 + tmpd / (100000/3.0)) * (100000/3.0);
			if (fabs(tmpd2-tmpd) < fabs(tmpd3-tmpd)) tmpd = tmpd2;
			else tmpd = tmpd3;
		}
#		endif
		rdtsc_period_f = 1 / tmpd;
	}
#	endif
	*/
}

void deinit_time() {
	if (--time_inited) get_time_error("get_time - deinit_time() called without init_time()");
	deinit_get_time_ticks();
	deinit_get_time_s();
	deinit_get_time_ms();
}
