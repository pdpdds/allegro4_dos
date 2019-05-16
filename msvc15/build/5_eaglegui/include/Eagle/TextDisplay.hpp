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


#ifndef TextDisplay_H
#define TextDisplay_H


#include <allegro.h>
#include <cstdarg>



/// TODO : Make textout_ex_gui support variable width fonts
// replacement for gui_textout_ex so it can use custom fonts, fixed width fonts only though
int textout_ex_gui(BITMAP* bmp , FONT* text_font , const char* s , int x , int y , int color , int bg , int center);

// vertical text drawing routines
void vtextout_ex(BITMAP* bmp , AL_CONST FONT* f , AL_CONST char* str , int x , int y , int color , int bg);
void vtextout_centre_ex(BITMAP* bmp , AL_CONST FONT* f , AL_CONST char* str , int x , int y , int color , int bg);
void vtextout_bottom_ex(BITMAP* bmp , AL_CONST FONT* f , AL_CONST char* str , int x , int y , int color , int bg);

void vtextprintf_ex(BITMAP* bmp , AL_CONST FONT* f , int x , int y , int color , int bg , AL_CONST char* format , ...);
void vtextprintf_centre_ex(BITMAP* bmp , AL_CONST FONT* f , int x , int y , int color , int bg , AL_CONST char* format , ...);
void vtextprintf_bottom_ex(BITMAP* bmp , AL_CONST FONT* f , int x , int y , int color , int bg , AL_CONST char* format , ...);



#endif // TextDisplay_H

