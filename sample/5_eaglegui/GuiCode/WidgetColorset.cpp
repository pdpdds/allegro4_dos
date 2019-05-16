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


#include "Eagle/GuiCode/WidgetColorset.hpp"

#include "Eagle/AllegroUtilities.hpp"

/*
enum WIDGETCOLOR {
   SDCOL   = 0,
   BGCOL   = 1,
   MGCOL   = 2,
   FGCOL   = 3,
   HLCOL   = 4,
   TXTCOL  = 5
};
*/
unsigned char default_rgba_colors[NUMCOLS][4] = {
   {0   , 0   , 0   , 255},// Shadow        - black
   {64  , 64  , 64  , 255},// Background    - dark grey
   {127 , 127 , 127 , 255},// Middleground  - grey
   {192 , 192 , 192 , 255},// Foreground    - bright grey
   {255 , 255 , 255 , 254},// Highlight     - white , alpha 254 to prevent a value of -1 when packed
   {255 , 255 , 255 , 254} // Text Color    - white again
};

float default_hsv_vcolors[NUMCOLS] = {
   0.0, // Shadow
   0.2, // Background
   0.4, // Middleground
   0.6, // Foreground
   1.0, // Highlight
   1.0  // TextColor
};



void set_default_rgba_color(WIDGETCOLOR wc , UCHAR r , UCHAR g , UCHAR b , UCHAR a) {
   default_rgba_colors[wc][0] = r;
   default_rgba_colors[wc][1] = g;
   default_rgba_colors[wc][2] = b;
   default_rgba_colors[wc][3] = a;
}



void set_default_hsv_vcolor(WIDGETCOLOR wc , float hsv_value) {
   if (hsv_value < 0.0f) {hsv_value = 0.0f;}
   if (hsv_value > 1.0f) {hsv_value = 1.0f;}
   default_hsv_vcolors[wc] = hsv_value;
}



WidgetColorset DefaultWidgetColors() {
   int scd = (screen?bitmap_color_depth(screen):32);
   
   WidgetColorset wc;
   for (int i = 0 ; i < NUMCOLS ; ++i) {
      unsigned char* col = &default_rgba_colors[i][0];
      wc.SetRGBAex(WIDGETCOLOR(i) , col[0] , col[1] , col[2] , col[3] , scd);
   }
   return wc;
}



WidgetColorset DefaultWidgetHSVColors(float hue , float saturation) {
   int scd = (screen?bitmap_color_depth(screen):32);
   ASSERT(hue >= 0.0f);
   ASSERT(hue <= 360.0f);
   ASSERT(saturation >= 0.0f);
   ASSERT(saturation <= 1.0f);
   int r,g,b,a=254;
   WidgetColorset wc;
   for (unsigned int i = 0 ; i < NUMCOLS ; ++i) {
      hsv_to_rgb(hue,saturation,default_hsv_vcolors[i],&r,&g,&b);
      wc[i].SetRGBAex(r,g,b,a,scd);
   }
   return wc;
}


/** ------------     WidgetColorset methods     --------------- */

void WidgetColorset::SetColorDepth(int color_depth) {
   COLORDEPTH cdi = ColorDepthIndex(color_depth);
   SetColorDepthIndex(cdi);
}


