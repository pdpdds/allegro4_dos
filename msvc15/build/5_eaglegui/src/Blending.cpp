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


#include "Eagle/Blending.hpp"
#include <allegro.h>


unsigned long translucent_layering_blender32(unsigned long x , unsigned long y , unsigned long n) {

   /// Alpha : 0 is clear, 255 is fully opaque
   unsigned long xr , xg , xb , xa;
   unsigned long yr , yg , yb , ya;
   unsigned long blended_r , blended_g , blended_b , blended_a;
   xa = geta32(x);
   if (xa == 0) {
      return y;
   }
   xr = getr32(x);
   xg = getg32(x);
   xb = getb32(x);
   yr = getr32(y);
   yg = getg32(y);
   yb = getb32(y);
   ya = geta32(y);

   //  blended_a = 255 - ((255-xa)*(255-ya)/255);
   //  unsigned long const optimizer3 = 255*255 - ((255-xa)*(255-ya));/// 255*255 = 65,025
   unsigned long const optimizer3 = 65025 - ((255-xa)*(255-ya));/// 255*255 = 65,025
   blended_a = optimizer3 / 255;
   unsigned long const optimizer1 = (255-xa)*ya;
   unsigned long const optimizer2 = 255*xa;
   //  unsigned long const optimizer3 = 255*blended_a;
   blended_r = (optimizer1*yr + optimizer2*xr) / optimizer3;
   blended_g = (optimizer1*yg + optimizer2*xg) / optimizer3;
   blended_b = (optimizer1*yb + optimizer2*xb) / optimizer3;

   return makeacol32(blended_r , blended_g , blended_b , blended_a);
}



void set_alpha_layering_blender() {
//void set_blender_mode_ex(BLENDER_FUNC b15, b16, b24, b32, b15x, b16x, b24x, int r, g, b, a);
   set_blender_mode_ex(0 , 0 , 0 , translucent_layering_blender32 , 0 , 0 , 0  , 255 , 255 , 255 , 255);
}

