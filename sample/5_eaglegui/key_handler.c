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


#include "Eagle/key_handler.h"


/* Public variables */
volatile char   keystate[KEY_STATE_EXTENDED_MAX];
volatile double key_held_duration[KEY_MAX];
volatile double since_last_click[KEY_MAX];
double double_click_duration = 0.5;



/* Private variables */
volatile char pkey[KEY_MAX];// Tracks previous state of allegro key array

volatile int keypress_buffer[MAX_KEYBUFFER_SIZE];
volatile int keyrelease_buffer[MAX_KEYBUFFER_SIZE];
volatile unsigned int keypress_buffer_size = 0;
volatile unsigned int keyrelease_buffer_size = 0;

const int state_mask = KEYPRESS | KEYRELEASE | KEYHELD | KEYOPEN;

void AssignKeyStates(double dt_sec);
void StoreCurrentKeys(void);



const char* keycode_to_name(int keycode) {/* keycode max is KEY_STATE_MAX - 1. (KEY_ANY_ALT) */

/** Had to grab this key_names array from exkeys.c, as scancode_to_name
      returns a bunch of faulty values as well as having the 'normal' values
      defined as KEY105, KEY112, etc...(As of Allegro 4.2.2) */

   const char* key_names[] =
   {
      "KEY_NONE",       "KEY_A",          "KEY_B",          "KEY_C",
      "KEY_D",          "KEY_E",          "KEY_F",          "KEY_G",
      "KEY_H",          "KEY_I",          "KEY_J",          "KEY_K",
      "KEY_L",          "KEY_M",          "KEY_N",          "KEY_O",
      "KEY_P",          "KEY_Q",          "KEY_R",          "KEY_S",
      "KEY_T",          "KEY_U",          "KEY_V",          "KEY_W",
      "KEY_X",          "KEY_Y",          "KEY_Z",          "KEY_0",
      "KEY_1",          "KEY_2",          "KEY_3",          "KEY_4",
      "KEY_5",          "KEY_6",          "KEY_7",          "KEY_8",
      "KEY_9",          "KEY_0_PAD",      "KEY_1_PAD",      "KEY_2_PAD",
      "KEY_3_PAD",      "KEY_4_PAD",      "KEY_5_PAD",      "KEY_6_PAD",
      "KEY_7_PAD",      "KEY_8_PAD",      "KEY_9_PAD",      "KEY_F1",
      "KEY_F2",         "KEY_F3",         "KEY_F4",         "KEY_F5",
      "KEY_F6",         "KEY_F7",         "KEY_F8",         "KEY_F9",
      "KEY_F10",        "KEY_F11",        "KEY_F12",        "KEY_ESC",
      "KEY_TILDE",      "KEY_MINUS",      "KEY_EQUALS",     "KEY_BACKSPACE",
      "KEY_TAB",        "KEY_OPENBRACE",  "KEY_CLOSEBRACE", "KEY_ENTER",
      "KEY_COLON",      "KEY_QUOTE",      "KEY_BACKSLASH",  "KEY_BACKSLASH2",
      "KEY_COMMA",      "KEY_STOP",       "KEY_SLASH",      "KEY_SPACE",
      "KEY_INSERT",     "KEY_DEL",        "KEY_HOME",       "KEY_END",
      "KEY_PGUP",       "KEY_PGDN",       "KEY_LEFT",       "KEY_RIGHT",
      "KEY_UP",         "KEY_DOWN",       "KEY_SLASH_PAD",  "KEY_ASTERISK",
      "KEY_MINUS_PAD",  "KEY_PLUS_PAD",   "KEY_DEL_PAD",    "KEY_ENTER_PAD",
      "KEY_PRTSCR",     "KEY_PAUSE",      "KEY_ABNT_C1",    "KEY_YEN",
      "KEY_KANA",       "KEY_CONVERT",    "KEY_NOCONVERT",  "KEY_AT",
      "KEY_CIRCUMFLEX", "KEY_COLON2",     "KEY_KANJI",      "KEY_EQUALS_PAD",
      "KEY_BACKQUOTE",  "KEY_SEMICOLON",  "KEY_COMMAND",    "KEY_UNKNOWN1",
      "KEY_UNKNOWN2",   "KEY_UNKNOWN3",   "KEY_UNKNOWN4",   "KEY_UNKNOWN5",
      "KEY_UNKNOWN6",   "KEY_UNKNOWN7",   "KEY_UNKNOWN8",   "KEY_LSHIFT",
      "KEY_RSHIFT",     "KEY_LCONTROL",   "KEY_RCONTROL",   "KEY_ALT",
      "KEY_ALTGR",      "KEY_LWIN",       "KEY_RWIN",       "KEY_MENU",
      "KEY_SCRLOCK",    "KEY_NUMLOCK",    "KEY_CAPSLOCK",   "KEY_MAX"
   };
   static const char* modkey_names[NUM_KEY_EXTENSIONS] = {
      "KEY_ANY_SHIFT",
      "KEY_ANY_CTRL",
      "KEY_ANY_ALT",
      "KEY_NO_SHIFT",
      "KEY_NO_CTRL",
      "KEY_NO_ALT",
      "KEY_NO_MOD",
      "KEY_ONLY_SHIFT",
      "KEY_ONLY_CTRL",
      "KEY_ONLY_ALT",
      "KEY_ONLY_SHIFT_CTRL",
      "KEY_ONLY_SHIFT_ALT",
      "KEY_ONLY_CTRL_ALT"
   };
/* Keep for now
   static const char* modkey_names[NUM_KEY_EXTENSIONS] = {
      "Any Shift Key",
      "Any Ctrl Key",
      "Any Alt Key",
      "No Shift Key",
      "No Ctrl Key",
      "No Alt Key",
      "No Mod Keys",
      "Only Shift Key",
      "Only Ctrl Key",
      "Only Alt Key",
      "Only Shift Ctrl",
      "Only Shift Alt",
      "Only Ctrl Alt"
   };
*/
   int index = keycode - KEY_MAX;

   if (keycode < KEY_MAX) {
///      return scancode_to_name(keycode);/* Not working in Allegro 4.2.2 */
      return key_names[keycode];
   }
   if (keycode < KEY_STATE_EXTENDED_MAX) {
      return modkey_names[index];
   }
   return "Invalid Keycode";
}



void AddKeyPress(int al_key) {
   int i = 0;
   if (keypress_buffer_size < MAX_KEYBUFFER_SIZE) {
      keypress_buffer[keypress_buffer_size] = al_key;
      ++keypress_buffer_size;
   } else {
      for (i = 0 ; i < MAX_KEYBUFFER_SIZE - 1 ; ++i) {
         keypress_buffer[i] = keypress_buffer[i+1];
      }
      keypress_buffer[keypress_buffer_size - 1] = al_key;
   }
}



void AddKeyRelease(int al_key) {
   int i = 0;
   if (keyrelease_buffer_size < MAX_KEYBUFFER_SIZE) {
      keyrelease_buffer[keyrelease_buffer_size] = al_key;
      ++keyrelease_buffer_size;
   } else {// keyrelease_buffer_size should be equal to MAX_KEYBUFFER_SIZE
      for (i = 0 ; i < MAX_KEYBUFFER_SIZE - 1 ; ++i) {
         keyrelease_buffer[i] = keyrelease_buffer[i+1];
      }
      keyrelease_buffer[keyrelease_buffer_size - 1] = al_key;
   }
}



int KeyPressInBuffer() {
   return keypress_buffer_size;
}



int KeyReleaseInBuffer() {
   return keyrelease_buffer_size;
}



int PeekKeyPress() {
   if (keypress_buffer_size) {
      return keypress_buffer[0];
   }
   return KEY_NONE;
}



int PeekKeyRelease() {
   if (keyrelease_buffer_size) {
      return keyrelease_buffer[0];
   }
   return KEY_NONE;
}



/* ReadKeyPress() returns the oldest allegro KEY_* if a key is in the key press buffer , otherwise it returns 0 */
int ReadKeyPress() {
   int pressed_key = PeekKeyPress();
   RemoveKeyPress();
   return pressed_key;
}



/* ReadKeyRelease() returns the oldest allegro KEY_* if a key is in the key release buffer , otherwise it returns 0 */
int ReadKeyRelease() {
   int released_key = PeekKeyRelease();
   RemoveKeyRelease();
   return released_key;
}



void RemoveKeyPress() {
   int i = 0;
   if (keypress_buffer_size) {
      for (i = 0 ; i < (int)keypress_buffer_size - 1 ; ++i) {
         keypress_buffer[i] = keypress_buffer[i+1];
      }
      --keypress_buffer_size;
   }
}



void RemoveKeyRelease() {
   int i = 0;
   if (keyrelease_buffer_size) {
      for (i = 0 ; i < (int)keyrelease_buffer_size - 1 ; ++i) {
         keyrelease_buffer[i] = keyrelease_buffer[i+1];
      }
      --keyrelease_buffer_size;
   }
}



void ClearKeyBuffers() {
   keypress_buffer_size = 0;
   keyrelease_buffer_size = 0;
}



void ClearKeyPressBuffer() {
   keypress_buffer_size = 0;
}



void ClearKeyReleaseBuffer() {
   keyrelease_buffer_size = 0;
}



void SetupKeyHandler() {
   int i = 0;
   for (i = 0 ; i < KEY_MAX ; ++i) {
      pkey[i] = 0;
      key_held_duration[i] = 0.0;
   }
   for (i = 0 ; i < KEY_STATE_EXTENDED_MAX ; ++i) {
      keystate[i] = KEYOPEN;
   }
}



void AssignKeyStates(double dt_sec) {
   int i = 0;
   for (i = 0 ; i < KEY_MAX ; ++i) {
      since_last_click[i] += dt_sec;
      if (key[i]) {
         if (pkey[i]) {
            keystate[i] = KEYHELD;
            key_held_duration[i] += dt_sec;
         } else {
            keystate[i] = KEYPRESS;
            if (since_last_click[i] <= double_click_duration) {
               keystate[i] = KEYPRESS | KEYDBLCLICK;
            }
            since_last_click[i] = 0.0;
            AddKeyPress(i);
         }
      } else {
         if (pkey[i]) {
            keystate[i] = KEYRELEASE;
            key_held_duration[i] = 0.0;
            AddKeyRelease(i);
         } else {
            keystate[i] = KEYOPEN;
         }
      }
   }

   /* These are mostly only useful with the key_held function, but they are used quite often for me. */
   keystate[KEY_ANY_SHIFT]       = keystate[KEY_LSHIFT  ] | keystate[KEY_RSHIFT  ];
   keystate[KEY_ANY_CTRL]        = keystate[KEY_LCONTROL] | keystate[KEY_RCONTROL];
   keystate[KEY_ANY_ALT]         = keystate[KEY_ALT     ] | keystate[KEY_ALTGR   ];

   keystate[KEY_NO_SHIFT]        = ~keystate[KEY_ANY_SHIFT] & state_mask;
   keystate[KEY_NO_CTRL]         = ~keystate[KEY_ANY_CTRL ] & state_mask;
   keystate[KEY_NO_ALT]          = ~keystate[KEY_ANY_ALT  ] & state_mask;

   keystate[KEY_NO_MOD]          = keystate[KEY_NO_SHIFT ] & keystate[KEY_NO_CTRL] & keystate[KEY_NO_ALT];

   keystate[KEY_ONLY_SHIFT]      = keystate[KEY_ANY_SHIFT] & keystate[KEY_NO_CTRL] & keystate[KEY_NO_ALT];
   keystate[KEY_ONLY_CTRL]       = keystate[KEY_NO_SHIFT ] & keystate[KEY_ANY_CTRL] & keystate[KEY_NO_ALT];
   keystate[KEY_ONLY_ALT]        = keystate[KEY_NO_SHIFT ] & keystate[KEY_NO_CTRL] & keystate[KEY_ANY_ALT];
   keystate[KEY_ONLY_SHIFT_CTRL] = keystate[KEY_ANY_SHIFT] & keystate[KEY_ANY_CTRL] & keystate[KEY_NO_ALT];
   keystate[KEY_ONLY_SHIFT_ALT]  = keystate[KEY_ANY_SHIFT] & keystate[KEY_NO_CTRL] & keystate[KEY_ANY_ALT];
   keystate[KEY_ONLY_CTRL_ALT]   = keystate[KEY_NO_SHIFT ] & keystate[KEY_ANY_CTRL] & keystate[KEY_ANY_ALT];

}



void StoreCurrentKeys() {
   int i;
   for (i = 0 ; i < KEY_MAX ; ++i) {pkey[i] = key[i];}
}


/** TODO : Should these search up through KEY_STATE_EXTENDED_MAX instead? */
/**        No, as they are pseudo key presses/releases */
int AnyKeysPressed() {
   int i;
   for (i = 0 ; i < KEY_MAX ; ++i) {if (keystate[i] & KEYPRESS) {return 1;}}
   return 0;
}
int AnyKeysReleased() {
   int i;
   for (i = 0 ; i < KEY_MAX ; ++i) {if (keystate[i] & KEYRELEASE) {return 1;}}
   return 0;
}
int AnyKeysHeld() {
   int i;
   for (i = 0 ; i < KEY_MAX ; ++i) {if (keystate[i] & KEYHELD) {return 1;}}
   return 0;
}



void UpdateKeyHandler(double dt_sec) {
  AssignKeyStates(dt_sec);
  StoreCurrentKeys();
}





