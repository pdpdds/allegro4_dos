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


#ifndef BitmapUtility_H
#define BitmapUtility_H

#include <allegro.h>

#include "Eagle/AllegroUtilities.hpp"


enum BMP_MEM_TYPE {
   MEMORY = 0,
   SYSTEM = 1,
   VIDEO  = 2
};



enum BMP_DRAW_TYPE {
   SOLID       = 0,
   TRANSPARENT = 1,
   TRANSLUCENT = 2
};



BMP_MEM_TYPE GetMemType(BITMAP* bmp);

BITMAP* CreateBitmap(int color_depth , int width , int height , BMP_MEM_TYPE bmp_mem_type);

/// If font_to_use is not NULL, it will be used to print the characters on the font template as a guide.
/// Using a translucent font with this function is not recommended. If the font sizes are different, the 
/// characters from the font_to_use will be stretched to fit the new size.
BITMAP* CreateFontBitmap(int color_depth , int charwidth , int charheight , FONT* font_to_use , bool use_color_font);

void ReplaceColor(BITMAP* bmp , int oldcol , int newcol);
void ReplaceMask(BITMAP* bmp , int newcol);

void SetAlphaTotal(BITMAP* bmp , int alpha);
void SetAlphaMasked(BITMAP* bmp , int alpha);// only sets the alpha for pixels that have a non zero alpha value

bool BitmapsEquivalent(BITMAP* bmp1 , BITMAP* bmp2);





#endif // BitmapUtility_H

