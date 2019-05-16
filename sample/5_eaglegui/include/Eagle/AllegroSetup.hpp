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

#ifndef BasicAllegroSetup_H
#define BasicAllegroSetup_H

#include <allegro.h>


/** Setting up Allegro, and a shortcut for using a fullscreen graphics mode 
   GetAllegroReady will setup the basic allegro modules for you, and calls CheckDesktopInfo which
      can fail, but GetAllegroReady won't bail out if it does.
   GetAllegroReady also sets up a close_button_callback that sets the 'close_program' variable below to true
      when called. You can check the close_program state from your main loop as well as set it to true yourself
      if you want to close your program.

   SetGfxToDesktop is a quick way to set the native fullscreen mode and allocate a memory buffer.
      It uses RetrieveDesktopInfo from below, and defaults to 800 X 600 X 32 if the desktop info
      cannot be found.
*/

extern volatile bool close_program;// NOTE : Only changes state if GetAllegroReady's call to allegro_init
                                   //        and set_close_button_callback are successful.

bool GetAllegroReady(bool use_keyboard , bool use_timer , bool use_mouse);

bool SetGfxToDesktop(BITMAP** allocationbuffer);



/** Desktop Information
   allegro_init() needs to be successfully called before using these functions.
   CheckDesktopInfo should only be called once after allegro_init() and before set_gfx_mode is called,
   as the reported desktop resolution can change after changing graphics modes.

*/

bool CheckDesktopInfo();
bool RetrieveDesktopInfo(int* w , int* h , int* cd);
int DW();
int DH();
int DCD();

bool DesktopInfoKnown();

bool DesktopResolutionKnown();
bool DesktopColorDepthKnown();


#endif
