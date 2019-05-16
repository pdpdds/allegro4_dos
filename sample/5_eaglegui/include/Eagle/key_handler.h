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

#ifndef key_handler_H
#define key_handler_H

/** Copyright 2009, by Edgar Reynaldo                   */
/** Free for personal, non-commercial use only, for now */
/** Experimental, subject to change                     */
/** Copyright notice must be retained with source files */



#include <allegro.h>

#ifdef __cplusplus
   extern "C" {
#endif




/** Additional key codes for the states of the shift, ctrl, and alt modifier keys,
   as well as their possible combinations. */
enum KEYCODES {
   KEY_ANY_SHIFT          = KEY_MAX + 0,
   KEY_ANY_CTRL           = KEY_MAX + 1,
   KEY_ANY_ALT            = KEY_MAX + 2,

   KEY_NO_SHIFT           = KEY_MAX + 3,
   KEY_NO_CTRL            = KEY_MAX + 4,
   KEY_NO_ALT             = KEY_MAX + 5,

   KEY_NO_MOD             = KEY_MAX + 6,
   KEY_ONLY_SHIFT         = KEY_MAX + 7,
   KEY_ONLY_CTRL          = KEY_MAX + 8,
   KEY_ONLY_ALT           = KEY_MAX + 9,

   KEY_ONLY_SHIFT_CTRL    = KEY_MAX + 10,
   KEY_ONLY_SHIFT_ALT     = KEY_MAX + 11,
   KEY_ONLY_CTRL_ALT      = KEY_MAX + 12,

   KEY_STATE_EXTENDED_MAX = KEY_MAX + 13
};

#define NUM_KEY_EXTENSIONS (KEY_STATE_EXTENDED_MAX - KEY_MAX)

/** Key state, holds the transitional state of the keys in question as an 8 bit bitfield,
   and will not change until UpdateKeyHandler(double) is called */
extern volatile char keystate[KEY_STATE_EXTENDED_MAX];

/** Key held duration, holds the amount of time that a single key has been 
   held down consecutively */
extern volatile double key_held_duration[KEY_MAX];

/** Duration used to determine whether a double click occurred */
extern double double_click_duration;


const char* keycode_to_name(int keycode);/* keycode max is KEY_STATE_EXTENDED_MAX */

/** The possible flags that keystate[KEY_*] can have set : (don't use keystate[] like the key[] array) */
/** Pass KEYSTATE's to key_state */

enum KEYSTATES {
   KEYPRESS    = 1,
   KEYRELEASE  = 2,
   KEYHELD     = 4,
   KEYOPEN     = 8,
   KEYDBLCLICK = 16
};


/** Key macros for tidy use of allegro's key[] array with control , shift , and alternate modifier keys */
#define SHFT ((key[KEY_LSHIFT])   || (key[KEY_RSHIFT]  ))
#define CTRL ((key[KEY_LCONTROL]) || (key[KEY_RCONTROL]))
#define ALT  ((key[KEY_ALT])      || (key[KEY_ALTGR]   ))

#define NoModKey         (!((SHFT || CTRL) || (ALT)))
#define ShiftModOnly     (  SHFT && ( !(CTRL || ALT )))
#define CtrlModOnly      (  CTRL && ( !(SHFT || ALT )))
#define AltModOnly       (  ALT  && ( !(SHFT || CTRL)))
#define ShiftCtrlModOnly ( !ALT  && (SHFT && CTRL))
#define ShiftAltModOnly  ( !CTRL && (SHFT && ALT ))
#define AltCtrlModOnly   ( !SHFT && (ALT  && CTRL))


/** KEY_NONE is returned by PeekKey* and ReadKey* to indicate no key is in their buffer */
#define KEY_NONE 0

/** For keypress_buffer and keyrelease_buffer, private to source module */
#define MAX_KEYBUFFER_SIZE 60




static inline int key_press       (int al_key) {return (keystate[al_key] & KEYPRESS   );}
static inline int key_release     (int al_key) {return (keystate[al_key] & KEYRELEASE );}
static inline int key_held        (int al_key) {return (keystate[al_key] & KEYHELD    );}
static inline int key_open        (int al_key) {return (keystate[al_key] & KEYOPEN    );}
static inline int key_double_click(int al_key) {return (keystate[al_key] & KEYDBLCLICK);}

static inline int key_held_for(int al_key , double duration) {return (key_held_duration[al_key] >= duration);}




/** Sets the array of keystates to KEYOPEN and zeros out the array of previously pressed keys.
   Call once at program start to initialize the key handler. */
void SetupKeyHandler(void);



/* Useful for simulating key presses and releases */
void AddKeyPress(int al_key);
void AddKeyRelease(int al_key);


/* Check for key presses / releases stored in the buffers */
int KeyPressInBuffer(void);
int KeyReleaseInBuffer(void);


/* PeekKey[Press | Release] returns the oldest allegro KEY_* if a key is
   in the key press/release buffer, otherwise it returns KEY_NONE */
int PeekKeyPress(void);
int PeekKeyRelease(void);


/* ReadKey[Press | Release] returns PeekKey[Press | Release] and removes the key if
   there was one from the key press / release buffer */
int ReadKeyPress(void);
int ReadKeyRelease(void);


/* RemoveKey[Press | Release] removes the first key if there was one from the key press / release buffer */
void RemoveKeyPress(void);
void RemoveKeyRelease(void);


/* Clears the key buffers of stored keys */
void ClearKeyBuffers(void);// Clears both
void ClearKeyPressBuffer(void);
void ClearKeyReleaseBuffer(void);


/* Are there any keys pressed? released? held? */
/* Check upon request, or store data? */
/*  - Check upon request for now, maybe integrate into AssignKeyStates later */
int AnyKeysPressed();
int AnyKeysReleased();
int AnyKeysHeld();


void UpdateKeyHandler(double dt_sec);




#ifdef __cplusplus
   }
#endif


#endif // #ifndef key_handler_H


