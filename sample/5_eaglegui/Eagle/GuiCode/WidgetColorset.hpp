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


#ifndef WidgetColorset_H
#define WidgetColorset_H

#include <allegro.h>

#include "Eagle/Color.hpp"



typedef unsigned char UCHAR;

/// From darkest (SDCOL shadow color) to lightest (HLCOL highlight color), with a few extras
enum WIDGETCOLOR {
   SDCOL   = 0,
   BGCOL   = 1,
   MGCOL   = 2,
   FGCOL   = 3,
   HLCOL   = 4,
   TXTCOL  = 5
};
#define NUMCOLS 6


class WidgetColorset {

private :
   Color colors[NUMCOLS];

public :
   
   WidgetColorset() : colors() {}
   
   WidgetColorset(const WidgetColorset& wc) : colors() {/* No way to initialize an array in a constructor? */
      *this = wc;
   }
   WidgetColorset& operator=(const WidgetColorset& wc) {
      for (unsigned int i = 0 ; i < NUMCOLS ; ++i) {colors[i] = wc.colors[i];}
      return *this;
   }
   
   inline void SetColor(const WIDGETCOLOR& wc , int color) {
      colors[wc].SetColor(color);
   }

   inline void SetRGB(const WIDGETCOLOR& wc , char r , char g , char b) {
      colors[wc].SetRGB(r,g,b);
   }

   inline void SetRGBex(const WIDGETCOLOR& wc , char r , char g , char b , int color_depth) {
      colors[wc].SetRGBex(r,g,b,color_depth);
   }

   inline void SetRGBA(const WIDGETCOLOR& wc , char r , char g , char b , char a) {
      colors[wc].SetRGBA(r,g,b,a);
   }

   inline void SetRGBAex(const WIDGETCOLOR& wc , char r , char g , char b , char a , int color_depth) {
      colors[wc].SetRGBAex(r,g,b,a,color_depth);
   }

   void SetColorDepth(int color_depth);

   inline void SetColorDepthIndex(COLORDEPTH cdi) {
      for (unsigned int i = 0 ; i < NUMCOLS ; ++i) {
         colors[i].SetColorDepthIndex(cdi);
      }
   }

   inline void ShiftHue(const WIDGETCOLOR& wc , float shift) {// shift = [-180.0,180.0]
      colors[wc].ShiftHue(shift);
   }

   inline void ShiftHue(float shift) {// shift = [-180.0,180.0]
      for (unsigned int i = 0 ; i < NUMCOLS ; ++i) {
         colors[i].ShiftHue(shift);
      }
   }

   inline void Tint(const WIDGETCOLOR& wc , UCHAR red , UCHAR green , UCHAR blue , UCHAR amt) {
      colors[wc].Tint(red,green,blue,amt);
   }

   inline void Tint(UCHAR red , UCHAR green , UCHAR blue , UCHAR amt) {
      for (unsigned int i = 0 ; i < NUMCOLS ; ++i) {
         colors[i].Tint(red,green,blue,amt);
      }
   }
   
   inline void Tint(const Color& c , UCHAR amt) {
      for (unsigned int i = 0 ; i < NUMCOLS ; ++i) {
         colors[i].Tint(c.R() , c.G() , c.B() , amt);
      }
   }
   
   inline void Invert() {
      for (unsigned int i = 0 ; i < NUMCOLS ; ++i) {
         colors[i].Invert();
      }
   }

   inline void SetAlpha(UCHAR alpha) {
      for (unsigned int i = 0 ; i < NUMCOLS ; ++i) {
         colors[i].SetAlpha(alpha);
      }
   }

///   inline const int operator[](const WIDGETCOLOR& wc) const {return (int)(colors[wc]);}
   const Color& operator[](const WIDGETCOLOR& wc) const {return colors[wc];}
   const Color& operator[](unsigned int wc) const {return colors[wc];}
   Color& operator[](const WIDGETCOLOR& wc) {return colors[wc];}
   Color& operator[](unsigned int wc) {return colors[wc];}
   
   int ColorDepth() {return colors[SDCOL].CD();}
};


extern UCHAR default_rgba_colors[NUMCOLS][4];
extern float default_hsv_vcolors[NUMCOLS];

/// These let you affect the colorsets that DefaultWidgetColors and DefaultWidgetHSVColors will return
void set_default_rgba_color(WIDGETCOLOR wc , UCHAR r , UCHAR g , UCHAR b , UCHAR a);
void set_default_hsv_vcolor(WIDGETCOLOR wc , float hsv_value);

WidgetColorset DefaultWidgetColors();

WidgetColorset DefaultWidgetHSVColors(float hue , float saturation);

#endif // #ifndef WidgetColorset_H





