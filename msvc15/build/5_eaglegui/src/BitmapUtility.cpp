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


#include "Eagle/BitmapUtility.hpp"


BMP_MEM_TYPE GetMemType(BITMAP* bmp) {
   if (is_memory_bitmap(bmp)) {return MEMORY;}
   if (is_system_bitmap(bmp)) {return SYSTEM;}
   if (is_video_bitmap(bmp))  {return VIDEO;}
   return MEMORY;
}



BITMAP* CreateBitmap(int color_depth , int width , int height , BMP_MEM_TYPE bmp_mem_type) {
   /** 
      If a SYSTEM or VIDEO bitmap is requested, the color_depth is set to the color depth of the screen.
   */
   BITMAP* bitmap = NULL;
   if (bmp_mem_type != MEMORY) {
      ASSERT(screen);
      color_depth = bitmap_color_depth(screen);
   }
   switch (bmp_mem_type) {
      case MEMORY :
         bitmap = create_bitmap_ex(color_depth , width , height);
         break;
      case SYSTEM :
         bitmap = create_system_bitmap(width , height);
         break;
      case VIDEO :
         bitmap = create_video_bitmap(width , height);
         break;
   }
   return bitmap;
}



BITMAP* CreateFontBitmap(int color_depth , int charwidth , int charheight , FONT* font_to_use , bool use_color_font) {
   ASSERT((charwidth > 0) && (charheight > 0));
   // 16 X 6 grid with 17 X 7 spaces
   int hspace = charwidth/2;
   if (hspace < 2) {hspace = 2;}
   int vspace = charheight/2;
   if (vspace < 2) {vspace = 2;}
   int bw = 16*charwidth + 17*hspace;
   int bh = 6*charheight + 7*vspace;
   BITMAP* bmp = create_bitmap_ex(color_depth , bw , bh);
   if (!bmp) {return NULL;}
   BITMAP* letterbmp = 0;
   int lw = 0;
   int lh = 0;
   if (font_to_use) {
      lw = text_length(font_to_use , " ");
      lh = text_height(font_to_use);
      letterbmp = create_bitmap_ex(color_depth , lw , lh);
   }
   char letter[2] = {'\0' , '\0'};
   char c = 32;// start with space
   int transcol;
   int bgcol;
   int textcol = makecol_depth(color_depth , 0 , 0 , 0);
   if (use_color_font) {textcol = -1;}
   if (color_depth == 8) {
      transcol = 0;
      bgcol = 255;
   } else {
      transcol = makecol_depth(color_depth , 255 , 0 , 255);
      bgcol = makecol_depth(color_depth , 255 , 255 , 0);
   }
   clear_to_color(bmp , bgcol);
   int cy = vspace;
   for (int y = 0 ; y < 6 ; ++y) {
      int cx = hspace;
      for (int x = 0 ; x < 16 ; ++x) {
         letter[0] = c;
         rectfill(bmp , cx , cy , cx + charwidth - 1 , cy + charheight - 1 , transcol);
         if (letterbmp) {
            clear_to_color(letterbmp , transcol);
            textout_ex(letterbmp , font_to_use , letter , 0 , 0 , textcol , -1);
            stretch_blit(letterbmp , bmp , 0 , 0 , lw , lh , cx , cy , charwidth , charheight);
         }
         ++c;
         cx += charwidth + hspace;
      }
      cy += charheight + vspace;
   }
   return bmp;
}



void ReplaceColor(BITMAP* bmp , int oldcol , int newcol) {
   ASSERT(bmp);
   
   const GPX_FUNC gpx = bmp->vtable->getpixel;
   const PPX_FUNC ppx = bmp->vtable->putpixel;
   
   for (int y = 0 ; y < bmp->h ; ++y) {
      for (int x = 0 ; x < bmp->w ; ++x) {
         if (gpx(bmp , x , y) == oldcol) {
            ppx(bmp , x , y , newcol);
         }
      }
   }
}



void ReplaceMask(BITMAP* bmp , int newcol) {
   ASSERT(bmp);
   ReplaceColor(bmp , bitmap_mask_color(bmp) , newcol);
}



void SetAlphaTotal(BITMAP* bmp , int alpha) {
   ASSERT(bmp);
   ASSERT(bitmap_color_depth(bmp) == 32);
   for (int y = 0 ; y < bmp->h ; ++y) {
      for (int x = 0 ; x < bmp->w ; ++x) {
         int c = getpixel(bmp , x , y);
         int r = getr32(c);
         int g = getg32(c);
         int b = getb32(c);
         int col = makeacol32(r,g,b,alpha);
         putpixel(bmp , x , y , col);
      }
   }
}



void SetAlphaMasked(BITMAP* bmp , int alpha) {
   ASSERT(bmp);
   ASSERT(bitmap_color_depth(bmp) == 32);
   for (int y = 0 ; y < bmp->h ; ++y) {
      for (int x = 0 ; x < bmp->w ; ++x) {
         int c = getpixel(bmp , x , y);
         int r = getr32(c);
         int g = getg32(c);
         int b = getb32(c);
         int a = geta32(c);
         if (a) {
            int col = makeacol32(r,g,b,alpha);
            putpixel(bmp , x , y , col);
         }
      }
   }
}



bool BitmapsEquivalent(BITMAP* bmp1 , BITMAP* bmp2) {
   ASSERT(bmp1);
   ASSERT(bmp2);
   if ((bmp1->w != bmp2->w) || (bmp1->h != bmp2->h)) {return false;}
   if (bitmap_color_depth(bmp1) != bitmap_color_depth(bmp2)) {return false;}
   
   const GPX_FUNC gpx1 = bmp1->vtable->getpixel;
   const GPX_FUNC gpx2 = bmp2->vtable->getpixel;
   
   for (int y = 0 ; y < bmp1->h ; ++y) {
      for (int x = 0 ; x < bmp1->w ; ++x) {
         if (gpx1(bmp1 , x , y) != gpx2(bmp2 , x , y)) {return false;}
      }
   }
   
   return true;
}

