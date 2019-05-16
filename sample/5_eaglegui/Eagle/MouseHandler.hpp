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


#ifndef MouseHandler_H
#define MouseHandler_H


#include "Eagle/BitmapHandler.hpp"
#include "Eagle/AllegroUtilities.hpp"

#include <map>
#include <list>
#include <string>
#include <allegro.h>

///  5 extra buttons included (but not necessarily tracked by allegro, so don't rely on them)
enum MOUSECODE {
   LMB  = 0,
   RMB  = 1,
   MMB  = 2,
   EMB1 = 3,
   EMB2 = 4,
   EMB3 = 5,
   EMB4 = 6,
   EMB5 = 7,
   NUM_MBTNS = 8
};

/* Maps the mousecodes to the flags used in the bitfields */
extern const int mbtn_flags[NUM_MBTNS];


/** Call once at the program start after mouse initialization to get the mouse handler ready */
void SetupMouseHandler();

/** Call once for every input update, passing the elapsed time, or 0.0 if you don't wish to
   track mouse button held durations */
void UpdateMouseHandler(double dt_sec);

/** Exposed variables to hold the current mouse position, z and w wheel position , and button state */
extern volatile int mpos;
extern volatile int mx;
extern volatile int my;
extern volatile int mz;
extern volatile int mw;
extern volatile int mb;

/** Exposed variables containing the differences in x,y and z and w wheel position */
extern volatile int ms_xdiff;/* old_mx + ms_xdiff = new_mx */
extern volatile int ms_ydiff;/* old_my + ms_ydiff = new_my */
extern volatile int ms_zdiff;/* old_mz + ms_zdiff = new_mz */
extern volatile int ms_wdiff;/* old_mw + ms_wdiff = new_mw */

/** These are only public to allow inlining of the nice shortcuts below.*/
/* Variables for button state change tracking */
extern volatile int held_mbtns;
extern volatile int pressed_mbtns;
extern volatile int released_mbtns;
extern volatile int open_mbtns;
extern volatile int dbl_click_mbtns;

extern volatile double held_mbtns_duration[NUM_MBTNS];
extern volatile double since_last_click_mbtns[NUM_MBTNS];
extern double double_click_duration_mbtn;

/* Returns ascii name for mbtn index */
const char* mousecode_to_name(int mbtn);


static inline int mb_press       (int mbtn) {return (pressed_mbtns   & mbtn_flags[mbtn]);}
static inline int mb_release     (int mbtn) {return (released_mbtns  & mbtn_flags[mbtn]);}
static inline int mb_held        (int mbtn) {return (held_mbtns      & mbtn_flags[mbtn]);}
static inline int mb_open        (int mbtn) {return (open_mbtns      & mbtn_flags[mbtn]);}
static inline int mb_double_click(int mbtn) {return (dbl_click_mbtns & mbtn_flags[mbtn]);}

static inline int mb_held_for(int mbtn , double duration) {
   return (held_mbtns_duration[mbtn] >= duration);
}


static inline int AnyMbtnsPressed()  {return pressed_mbtns;}
static inline int AnyMbtnsReleased() {return released_mbtns;}
static inline int AnyMbtnsHeld()     {return held_mbtns;}



/// Mouse pointer code



enum MOUSE_POINTER {
   POINTER_NORMAL   = 0,
   POINTER_BUSY     = 1,
   POINTER_QUESTION = 2,
   POINTER_EDIT     = 3,
   POINTER_GRAB     = 4,
   POINTER_GRABBED  = 5,
   POINTER_NS       = 6,
   POINTER_NESW     = 7,
   POINTER_EW       = 8,
   POINTER_SENW     = 9,
   POINTER_CUSTOM   = 10,
   NUM_POINTERS     = 11
};


extern const char* pointer_names[NUM_POINTERS];


typedef struct POINTER_INFO {
   BITMAP* bmp;
   int x;
   int y;
} POINTER_INFO;

void SetupMouseHandler();

int AcquireMouseOwnerId();
int GrabMousePointer(int owner_id);
int UseMousePointer(MOUSE_POINTER pointer_type , int owner_id);
int ReleaseMousePointer(int owner_id);

void SetPointerAndPosition(MOUSE_POINTER pointer_type , BITMAP* pointer_sprite , int pointer_x = 0 , int pointer_y = 0);
void SetupPointers(POINTER_INFO* info_array);
void SetAllegroPointerUse(bool use_allegro_pointer);

void DrawPointer(BITMAP* bmp , int x , int y);



class MousePointerSetup {
private :
   POINTER_INFO info[NUM_POINTERS];
public :
   MousePointerSetup();
   ~MousePointerSetup();

   void Free();

   bool LoadPointersFromScript(const char* script_file);
};




#endif // #ifndef MouseHandler_H

