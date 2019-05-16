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


#include "Eagle/AllegroUtilities.hpp"



const char* al_id_to_ascii(int al_id) {
   static char ascii_name[5] = {'\0','\0','\0','\0','\0'};
   ascii_name[0] = (char)((al_id >> 24) & 0xFF);
   ascii_name[1] = (char)((al_id >> 16) & 0xFF);
   ascii_name[2] = (char)((al_id >> 8) & 0xFF);
   ascii_name[3] = (char)(al_id & 0xFF);
   return (const char*)ascii_name;
}



const char* GraphicsCardName(int card) {
   switch (card) {
      case GFX_TEXT                  : return "GFX_TEXT";break;
      case GFX_AUTODETECT            : return "GFX_AUTODETECT";break;
      case GFX_AUTODETECT_FULLSCREEN : return "GFX_AUTODETECT_FULLSCREEN";break;
      case GFX_AUTODETECT_WINDOWED   : return "GFX_AUTODETECT_WINDOWED";break;
      case GFX_SAFE                  : return "GFX_SAFE";break;
      //case GFX_NONE                  : return "GFX_NONE";break;// GFX_NONE is from Allegro 4.3.10+
   }
   ASSERT(system_driver);
   _DRIVER_INFO* info = _gfx_driver_list;
   if (system_driver->gfx_drivers) {
      info = system_driver->gfx_drivers();
   }
   int i = 0;
   while (info[i].driver) {
      GFX_DRIVER* gd = (GFX_DRIVER*)info[i].driver;
      if (gd->id == card) {
         return gd->ascii_name;
      }
      ++i;
   }
   return al_id_to_ascii(card);
}



int AllegroColorDepth() {
   if (screen) {return bitmap_color_depth(screen);}
   else        {return get_color_depth();}
   return -1;
}



COLORDEPTH ColorDepthIndex(int colordepth) {
   switch(colordepth) {
      case 32: return BPP32;
      case 24: return BPP24;
      case 16: return BPP16;
      case 15: return BPP15;
      case 8 : return BPP8;
      default : ASSERT(0);
   }
   return NUMCOLORDEPTHS;
}



int ColorDepth(COLORDEPTH cdi) {
   switch (cdi) {
      case BPP32 : return 32;
      case BPP24 : return 24;
      case BPP16 : return 16;
      case BPP15 : return 15;
      case BPP8 : return 8;
      case NUMCOLORDEPTHS : ASSERT(0);
   }
   return 0;
}



const GPX_FUNC   gpx_functions[NUMCOLORDEPTHS]     = {_getpixel32 , _getpixel24 , _getpixel16 , _getpixel15 , _getpixel};
const PPX_FUNC   ppx_functions[NUMCOLORDEPTHS]     = {_putpixel32 , _putpixel24 , _putpixel16 , _putpixel15 , _putpixel};
const GETR_FUNC  getr_functions[NUMCOLORDEPTHS]    = {getr32      , getr24      , getr16      , getr15      , getr     };
const GETG_FUNC  getg_functions[NUMCOLORDEPTHS]    = {getg32      , getg24      , getg16      , getg15      , getg     };
const GETB_FUNC  getb_functions[NUMCOLORDEPTHS]    = {getb32      , getb24      , getb16      , getb15      , getb     };
const GETA_FUNC  geta_functions[NUMCOLORDEPTHS]    = {geta32      , 0           , 0           , 0           , 0        };
const MKCOL_FUNC makecol_functions[NUMCOLORDEPTHS] = {makecol32   , makecol24   , makecol16   , makecol15   , makecol8 };



GPX_FUNC GetPixelFuncPtr(int colordepth) {
   COLORDEPTH cdi = ColorDepthIndex(colordepth);
   ASSERT(cdi != NUMCOLORDEPTHS);
   return gpx_functions[cdi];
}

PPX_FUNC PutPixelFuncPtr(int colordepth) {
   COLORDEPTH cdi = ColorDepthIndex(colordepth);
   ASSERT(cdi != NUMCOLORDEPTHS);
   return ppx_functions[cdi];
}

GETR_FUNC GetRedFuncPtr(int colordepth) {
   COLORDEPTH cdi = ColorDepthIndex(colordepth);
   ASSERT(cdi != NUMCOLORDEPTHS);
   return getr_functions[cdi];
}

GETG_FUNC GetGreenFuncPtr(int colordepth) {
   COLORDEPTH cdi = ColorDepthIndex(colordepth);
   ASSERT(cdi != NUMCOLORDEPTHS);
   return getg_functions[cdi];
}

GETB_FUNC GetBlueFuncPtr(int colordepth) {
   COLORDEPTH cdi = ColorDepthIndex(colordepth);
   ASSERT(cdi != NUMCOLORDEPTHS);
   return getb_functions[cdi];
}

// NOTE : sort of superfluous, since there is only one - makeacol
GETA_FUNC GetAlphaFuncPtr(int colordepth) {
   COLORDEPTH cdi = ColorDepthIndex(colordepth);
   ASSERT(cdi != NUMCOLORDEPTHS);
   return geta_functions[cdi];
}

// NOTE : doesn't support alpha with makeacol
MKCOL_FUNC MakeColFuncPtr(int colordepth) {
   COLORDEPTH cdi = ColorDepthIndex(colordepth);
   ASSERT(cdi != NUMCOLORDEPTHS);
   return makecol_functions[cdi];
}



int MaskColor(int colordepth) {
   switch (colordepth) {
      case 32 : return MASK_COLOR_32;
      case 24 : return MASK_COLOR_24;
      case 16 : return MASK_COLOR_16;
      case 15 : return MASK_COLOR_15;
      case 8 : return MASK_COLOR_8;
      default : ASSERT(0);
   }
   return 0;
}



