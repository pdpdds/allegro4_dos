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


#ifndef Color_H
#define Color_H


#include <iostream>

#include "Eagle/AllegroUtilities.hpp"
#include "Eagle/Logging.hpp"


/** NOTES :
   Only the default Color constructor is safe for global objects! Otherwise you are most likely
   calling Allegro functions before Allegro is initialized!
*/

class Color {
   typedef unsigned char UCHAR;

private :
   UCHAR      r,g,b,a;
   int        color;
   int        cd;       // color depth
   COLORDEPTH cd_index; // index for color depth

   void RefreshRGBA();
   void RefreshColor();
   
public :
   Color();/// Only constructor that is safe for global object construction!
   Color(int al_color);
   Color(UCHAR red , UCHAR green , UCHAR blue , int color_depth);
   Color(UCHAR red , UCHAR green , UCHAR blue , UCHAR alpha , int color_depth);

   /** TODO : Should default constructor read the current color depth from Allegro?
               - Would that mean no global default constructed Color objects? */

   void SetColor          (int col);
   void SetRGB            (UCHAR red , UCHAR green , UCHAR blue);
   void SetRGBex          (UCHAR red , UCHAR green , UCHAR blue , int color_depth);
   void SetRGBA           (UCHAR red , UCHAR green , UCHAR blue , UCHAR alpha);
   void SetRGBAex         (UCHAR red , UCHAR green , UCHAR blue , UCHAR alpha , int color_depth);
   void SetHSV            (float hue , float sat   , float val);// hue [0.0,360.0] , sat&val [0.0,1.0]
   
   void SetColorDepth     (int color_depth);
   void SetColorDepthIndex(COLORDEPTH cd);

   void ShiftHue(float hue_shift);// [-180.0 , 180.0]
   
   void Tint(UCHAR red , UCHAR green , UCHAR blue , UCHAR amt);

   void Invert();

   void SetAlpha(UCHAR alpha);

   


   const int R()       const {return r;}
   const int G()       const {return g;}
   const int B()       const {return b;}
   const int A()       const {return a;}
   const int Col()     const {return color;}
   const int CD()      const {return cd;}// color depth
   const int CDIndex() const {return cd_index;}// color depth index

   inline operator int() const {return color;}

   std::ostream& PrintTo(std::ostream& os) const ;
   std::ostream& PrintTo(std::ostream& os , Indenter& indent) const ;
   friend std::ostream& operator<<(std::ostream& os , const Color& col);


};

void ColorToHSV(const Color& c , float* h , float* s , float* v);

int AverageColor(BITMAP* bmp , float xpos , float ypos , float width , float height , COLORDEPTH dest_cdi);
int AverageColor(BITMAP* bmp , float xpos , float ypos , float width , float height , COLORDEPTH dest_cdi,
                 const GPX_FUNC gpx , COLORDEPTH cdi , const GETR_FUNC grfunc , const GETG_FUNC ggfunc , const GETB_FUNC gbfunc,
                 const MKCOL_FUNC mkcol);
int MaskedAverageColor(BITMAP* bmp , float xpos , float ypos , float width , float height , COLORDEPTH dest_cdi);


#endif // Color_H

