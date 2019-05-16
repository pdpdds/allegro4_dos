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


#ifndef AllegroUtilities_H
#define AllegroUtilities_H


#include <allegro.h>


typedef int  (*GPX_FUNC) (BITMAP* , int , int);
typedef void (*PPX_FUNC) (BITMAP* , int , int , int);
typedef int  (*GETR_FUNC) (int);
typedef int  (*GETG_FUNC) (int);
typedef int  (*GETB_FUNC) (int);
typedef int  (*GETA_FUNC) (int);
typedef int  (*MKCOL_FUNC) (int , int , int);


enum COLORDEPTH {
   BPP32 = 0,
   BPP24 = 1,
   BPP16 = 2,
   BPP15 = 3,
   BPP8  = 4,
   NUMCOLORDEPTHS = 5
};



/** For getting text representations of allegro id's and graphics card names */
const char* al_id_to_ascii(int al_id);

const char* GraphicsCardName(int card);



/** For working with color depths and bitmap pixels */

/* Returns bitmap_color_depth(screen) if available or else get_color_depth */
int AllegroColorDepth();

COLORDEPTH ColorDepthIndex(int colordepth);
int ColorDepth(COLORDEPTH cdi);


extern const GPX_FUNC   gpx_functions[NUMCOLORDEPTHS];
extern const PPX_FUNC   ppx_functions[NUMCOLORDEPTHS];
extern const GETR_FUNC  getr_functions[NUMCOLORDEPTHS];
extern const GETG_FUNC  getg_functions[NUMCOLORDEPTHS];
extern const GETB_FUNC  getb_functions[NUMCOLORDEPTHS];
extern const GETA_FUNC  geta_functions[NUMCOLORDEPTHS];
extern const MKCOL_FUNC makecol_functions[NUMCOLORDEPTHS];


GPX_FUNC   GetPixelFuncPtr(int colordepth);
PPX_FUNC   PutPixelFuncPtr(int colordepth);

GETR_FUNC  GetRedFuncPtr  (int colordepth);
GETG_FUNC  GetGreenFuncPtr(int colordepth);
GETB_FUNC  GetBlueFuncPtr (int colordepth);
GETA_FUNC  GetAlphaFuncPtr(int colordepth);

MKCOL_FUNC MakeColFuncPtr (int colordepth);

int MaskColor(int colordepth);










#endif // AllegroUtilities_H





