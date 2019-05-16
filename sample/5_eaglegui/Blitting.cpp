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

#include "Eagle/Color.hpp"
#include "Eagle/AllegroUtilities.hpp"
#include "Eagle/Blitting.hpp"

int CLAMP(int minValue, int value, int maxValue)
{
	if (value < minValue)
		return minValue;

	if (value > maxValue)
		return maxValue;

	return value;
}


int Clip(BITMAP* src , BITMAP* dest , int& srcx , int& srcy , int& destx , int& desty , int& w , int& h) {

   // clip to source bitmap

   if (srcx >= src->w) {return 1;}
   if (srcy >= src->h) {return 1;}

   // clip left
   if (srcx < 0) {
      w += srcx;
      destx -= srcx;// preserve alignment
      srcx = 0;
   }
   // clip top
   if (srcy < 0) {
      h += srcy;
      desty -= srcy;// preserve alignment
      srcy = 0;
   }
   int brx = srcx + w - 1;
   int bry = srcy + h - 1;
   // clip right
   if (brx >= src->w) {
      w = src->w - srcx;
   }
   // clip bottom
   if (bry >= src->h) {
      h = src->h - srcy;
   }
   
   // clip to destination bitmap
   // cl and ct are inclusive and cr and cb are exclusive

   if (destx >= dest->cr) {return 1;}
   if (desty >= dest->cb) {return 1;}

   int extra = 0;
   // clip left
   if (destx < dest->cl) {
      extra = dest->cl - destx;
      w -= extra;
      srcx += extra;// preserve alignment
      if (srcx >= src->w) {return 1;}
      destx = dest->cl;
   }
   // clip top
   if (desty < dest->ct) {
      extra = dest->ct - desty;
      h -= extra;
      srcy += extra;// preserve alignment
      if (srcy >= src->h) {return 1;}
      desty = dest->ct;
   }
   brx = destx + w - 1;
   bry = desty + h - 1;
   // clip right
   if (brx >= dest->cr) {
      w = dest->cr - destx;
   }
   // clip bottom
   if (bry >= dest->cb) {
      h = dest->cb - desty;
   }

   // Is there anything to draw?
   if (w < 1) {return 1;}
   if (h < 1) {return 1;}
   
   return 0;
}



void tint_bitmap(BITMAP* bmp , int tint_color , int n) {
   ASSERT(bmp);
   
   n = CLAMP(0,n,255);
   const GPX_FUNC gpx_func(bmp->vtable->getpixel);
   const PPX_FUNC ppx_func(bmp->vtable->putpixel);
   COLORDEPTH cdi = ColorDepthIndex(bitmap_color_depth(bmp));
   const MKCOL_FUNC mkcol_func(makecol_functions[cdi]);
   const GETR_FUNC getr_func(getr_functions[cdi]);
   const GETG_FUNC getg_func(getg_functions[cdi]);
   const GETB_FUNC getb_func(getb_functions[cdi]);
   int col,r,g,b;
   const int tr = getr_func(tint_color);
   const int tg = getg_func(tint_color);
   const int tb = getb_func(tint_color);
   
   if (n) {
      acquire_bitmap(bmp);
      for (int y = 0 ; y < bmp->h ; ++y) {
         for (int x = 0 ; x < bmp->w ; ++x) {
            col = gpx_func(bmp , x , y);
            r = getr_func(col);
            g = getg_func(col);
            b = getb_func(col);
            r += ((tr - r)*n)/255;
            g += ((tg - g)*n)/255;
            b += ((tb - b)*n)/255;
            col = mkcol_func(r,g,b);
            ppx_func(bmp , x , y , col);
         }
      }
      release_bitmap(bmp);
   }
}



void set_hue(BITMAP* bmp , float hue) {
   ASSERT(bmp);
   
   const GPX_FUNC gpx_func(bmp->vtable->getpixel);
   const PPX_FUNC ppx_func(bmp->vtable->putpixel);
   COLORDEPTH cdi = ColorDepthIndex(bitmap_color_depth(bmp));
   const MKCOL_FUNC mkcol_func(makecol_functions[cdi]);
   const GETR_FUNC getr_func(getr_functions[cdi]);
   const GETG_FUNC getg_func(getg_functions[cdi]);
   const GETB_FUNC getb_func(getb_functions[cdi]);
   int col,r,g,b;
   float h,s,v;
   
   acquire_bitmap(bmp);
   for (int y = 0 ; y < bmp->h ; ++y) {
      for (int x = 0 ; x < bmp->w ; ++x) {
         col = gpx_func(bmp , x , y);
         r = getr_func(col);
         g = getg_func(col);
         b = getb_func(col);
         rgb_to_hsv(r,g,b,&h,&s,&v);
         hsv_to_rgb(hue,s,v,&r,&g,&b);
         col = mkcol_func(r,g,b);
         ppx_func(bmp , x , y , col);
      }
   }
   release_bitmap(bmp);
}



void invert_rgb(BITMAP* bmp) {
   ASSERT(bmp);
   
   const GPX_FUNC gpx = bmp->vtable->getpixel;
   const PPX_FUNC ppx = bmp->vtable->putpixel;
   COLORDEPTH cdi = ColorDepthIndex(bitmap_color_depth(bmp));
   const MKCOL_FUNC mkcol = makecol_functions[cdi];
   const GETR_FUNC getr = getr_functions[cdi];
   const GETG_FUNC getg = getg_functions[cdi];
   const GETB_FUNC getb = getb_functions[cdi];
   
   int r,g,b,col;
   
   if (is_video_bitmap(bmp)) {acquire_bitmap(bmp);}
   for (int y = 0 ; y < bmp->h ; ++y) {
      for (int x = 0 ; x < bmp->w ; ++x) {
         col = gpx(bmp , x , y);
         r = getr(col);
         g = getg(col);
         b = getb(col);
         r = 255 - r;
         g = 255 - g;
         b = 255 - b;
         col = mkcol(r,g,b);
         ppx(bmp , x , y , col);
      }
   }
   if (is_video_bitmap(bmp)) {release_bitmap(bmp);}
}



void invert_value(BITMAP* bmp) {
   ASSERT(bmp);
   
   const GPX_FUNC gpx = bmp->vtable->getpixel;
   const PPX_FUNC ppx = bmp->vtable->putpixel;
   COLORDEPTH cdi = ColorDepthIndex(bitmap_color_depth(bmp));
   const GETR_FUNC getr = getr_functions[cdi];
   const GETG_FUNC getg = getg_functions[cdi];
   const GETB_FUNC getb = getb_functions[cdi];
   const MKCOL_FUNC mkcol = makecol_functions[cdi];
   
   int r,g,b,col;
   float h,s,v;
   
   if (is_video_bitmap(bmp)) {acquire_bitmap(bmp);}
   for (int y = 0 ; y < bmp->h ; ++y) {
      for (int x = 0 ; x < bmp->w ; ++x) {
         col = gpx(bmp , x , y);
         r = getr(col);
         g = getg(col);
         b = getb(col);
         rgb_to_hsv(r,g,b,&h,&s,&v);
         v = 1.0-v;
         hsv_to_rgb(h,s,v,&r,&g,&b);
         col = mkcol(r,g,b);
         ppx(bmp , x , y , col);
      }
   }
   if (is_video_bitmap(bmp)) {release_bitmap(bmp);}
}



void convert_to_alpha(BITMAP* bmp) {
   ASSERT(bmp);
   ASSERT(bitmap_color_depth(bmp) == 32);
   
   const GPX_FUNC gpx = bmp->vtable->getpixel;
   const PPX_FUNC ppx = bmp->vtable->putpixel;
   
   const int maskcolor = bitmap_mask_color(bmp);
   int r,g,b,a,col;
   
   for (int y = 0 ; y < bmp->h ; ++y) {
      for (int x = 0 ; x < bmp->w ; ++x) {
         col = gpx(bmp , x , y);
         if (col == maskcolor) {
            r = g = b = a = 0;
         } else {
            r = getr32(col);
            g = getg32(col);
            b = getb32(col);
            a = 255;
         }
         col = makeacol(r,g,b,a);
         ppx(bmp , x , y , col);
      }
   }
}



void blur(BITMAP* bmp , int radius) {
   ASSERT(bmp);
   ASSERT(bmp->h > 0);
   ASSERT(bmp->w > 0);
   ASSERT(radius > 0);
   
   if (radius < 1) {return;}
   BITMAP* blur = create_bitmap_ex(bitmap_color_depth(bmp) , bmp->w , bmp->h);
   if (!blur) {return;}
   
   const PPX_FUNC ppx = blur->vtable->putpixel;
   const COLORDEPTH dest_cdi = ColorDepthIndex(bitmap_color_depth(blur));
   
   const GPX_FUNC gpx = bmp->vtable->getpixel;
   COLORDEPTH cdi = ColorDepthIndex(bitmap_color_depth(bmp));
   const GETR_FUNC grfunc = getr_functions[cdi];
   const GETG_FUNC ggfunc = getg_functions[cdi];
   const GETB_FUNC gbfunc = getb_functions[cdi];
   const MKCOL_FUNC mkcol = makecol_functions[dest_cdi];

//   const int bry = bmp->h - 1;
//   const int brx = bmp->w - 1;
   
//   float r,g,b,a,percent;
   const float width = (float)(2*radius + 1);
   const float height = (float)(2*radius + 1);
   int col;
   
   for (int sy = 0 ; sy < bmp->h ; ++sy) {
      for (int sx = 0 ; sx < bmp->w ; ++sx) {
/* TODO : Implement algorithm that gives less total value to pixels farther away?
         numpixels = 0;
         int starty = ((sy - radius) >= 0)?(sy - radius):(0);
         int endy = ((sy + radius) <= bry)?(sy + radius):(bry);
         int startx = ((sx - radius) >= 0)?(sx - radius):(0);
         int endx = ((sx + radius) <= brx)?(sx + radius):(brx);
         for (int y = starty ; y <= endy ; ++y) {
            for (int x = startx ; x <= endx ; ++x) {
               
            }
         }
*/
         col = AverageColor(bmp , (float)(sx - radius) , (float)(sy - radius) , width , height , dest_cdi,
                            gpx , cdi , grfunc , ggfunc , gbfunc , mkcol);
         ppx(blur , sx , sy , col);
      }
   }
   blit(blur , bmp , 0 , 0 , 0 , 0 , blur->w , blur->h);
   destroy_bitmap(blur);
}



void masked_tint_bitmap(BITMAP* bmp , int tint_color , int n) {
   ASSERT(bmp);
   
   n = CLAMP(0,n,255);
   const GPX_FUNC gpx_func(bmp->vtable->getpixel);
   const PPX_FUNC ppx_func(bmp->vtable->putpixel);
   COLORDEPTH cdi = ColorDepthIndex(bitmap_color_depth(bmp));
   const MKCOL_FUNC mkcol_func(makecol_functions[cdi]);
   const GETR_FUNC getr_func(getr_functions[cdi]);
   const GETG_FUNC getg_func(getg_functions[cdi]);
   const GETB_FUNC getb_func(getb_functions[cdi]);
   const int maskcolor = bitmap_mask_color(bmp);
   
   int col,r,g,b;
   const int tr = getr_func(tint_color);
   const int tg = getg_func(tint_color);
   const int tb = getb_func(tint_color);
   
   if (n) {
      acquire_bitmap(bmp);
      for (int y = 0 ; y < bmp->h ; ++y) {
         for (int x = 0 ; x < bmp->w ; ++x) {
            col = gpx_func(bmp , x , y);
            if (col != maskcolor) {
               r = getr_func(col);
               g = getg_func(col);
               b = getb_func(col);
               r += ((tr - r)*n)/255;
               g += ((tg - g)*n)/255;
               b += ((tb - b)*n)/255;
               col = mkcol_func(r,g,b);
               ppx_func(bmp , x , y , col);
            }
         }
      }
      release_bitmap(bmp);
   }
}



void masked_set_hue(BITMAP* bmp , float hue) {
   ASSERT(bmp);
   
   const GPX_FUNC gpx_func(bmp->vtable->getpixel);
   const PPX_FUNC ppx_func(bmp->vtable->putpixel);
   COLORDEPTH cdi = ColorDepthIndex(bitmap_color_depth(bmp));
   const MKCOL_FUNC mkcol_func(makecol_functions[cdi]);
   const GETR_FUNC getr_func(getr_functions[cdi]);
   const GETG_FUNC getg_func(getg_functions[cdi]);
   const GETB_FUNC getb_func(getb_functions[cdi]);
   const int maskcolor = bitmap_mask_color(bmp);
   
   int col,r,g,b;
   float h,s,v;
   
   acquire_bitmap(bmp);
   for (int y = 0 ; y < bmp->h ; ++y) {
      for (int x = 0 ; x < bmp->w ; ++x) {
         col = gpx_func(bmp , x , y);
         if (col != maskcolor) {
            r = getr_func(col);
            g = getg_func(col);
            b = getb_func(col);
            rgb_to_hsv(r,g,b,&h,&s,&v);
            hsv_to_rgb(hue,s,v,&r,&g,&b);
            col = mkcol_func(r,g,b);
            ppx_func(bmp , x , y , col);
         }
      }
   }
   release_bitmap(bmp);
}



void masked_invert_rgb(BITMAP* bmp) {
   ASSERT(bmp);
   
   const GPX_FUNC gpx = bmp->vtable->getpixel;
   const PPX_FUNC ppx = bmp->vtable->putpixel;
   COLORDEPTH cdi = ColorDepthIndex(bitmap_color_depth(bmp));
   const MKCOL_FUNC mkcol = makecol_functions[cdi];
   const GETR_FUNC getr = getr_functions[cdi];
   const GETG_FUNC getg = getg_functions[cdi];
   const GETB_FUNC getb = getb_functions[cdi];
   const int maskcolor = bitmap_mask_color(bmp);
   
   int r,g,b,col;
   
   if (is_video_bitmap(bmp)) {acquire_bitmap(bmp);}
   for (int y = 0 ; y < bmp->h ; ++y) {
      for (int x = 0 ; x < bmp->w ; ++x) {
         col = gpx(bmp , x , y);
         if (col != maskcolor) {
            r = getr(col);
            g = getg(col);
            b = getb(col);
            r = 255 - r;
            g = 255 - g;
            b = 255 - b;
            col = mkcol(r,g,b);
            ppx(bmp , x , y , col);
         }
      }
   }
   if (is_video_bitmap(bmp)) {release_bitmap(bmp);}
}



void masked_invert_value(BITMAP* bmp) {
   ASSERT(bmp);
   
   const GPX_FUNC gpx = bmp->vtable->getpixel;
   const PPX_FUNC ppx = bmp->vtable->putpixel;
   COLORDEPTH cdi = ColorDepthIndex(bitmap_color_depth(bmp));
   const GETR_FUNC getr = getr_functions[cdi];
   const GETG_FUNC getg = getg_functions[cdi];
   const GETB_FUNC getb = getb_functions[cdi];
   const MKCOL_FUNC mkcol = makecol_functions[cdi];
   const int maskcolor = bitmap_mask_color(bmp);
   
   int r,g,b,col;
   float h,s,v;
   
   if (is_video_bitmap(bmp)) {acquire_bitmap(bmp);}
   for (int y = 0 ; y < bmp->h ; ++y) {
      for (int x = 0 ; x < bmp->w ; ++x) {
         col = gpx(bmp , x , y);
         if (col != maskcolor) {
            r = getr(col);
            g = getg(col);
            b = getb(col);
            rgb_to_hsv(r,g,b,&h,&s,&v);
            v = 1.0-v;
            hsv_to_rgb(h,s,v,&r,&g,&b);
            col = mkcol(r,g,b);
            ppx(bmp , x , y , col);
         }
      }
   }
   if (is_video_bitmap(bmp)) {release_bitmap(bmp);}
}



void tint_blit(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h , int tint_color , int n) {
   ASSERT(src);
   ASSERT(dest);
   
   n = CLAMP(0,n,255);
   const GPX_FUNC gpx_func(src->vtable->getpixel);
   const PPX_FUNC ppx_func(dest->vtable->putpixel);
   COLORDEPTH srccdi = ColorDepthIndex(bitmap_color_depth(src));
   const GETR_FUNC getr_func(getr_functions[srccdi]);
   const GETG_FUNC getg_func(getg_functions[srccdi]);
   const GETB_FUNC getb_func(getb_functions[srccdi]);
   COLORDEPTH destcdi = ColorDepthIndex(bitmap_color_depth(dest));
   const MKCOL_FUNC mkcol_func(makecol_functions[destcdi]);
   int col,r,g,b;
   const int tr = getr(tint_color);
   const int tg = getg(tint_color);
   const int tb = getb(tint_color);
   
   int sx,sy,dx,dy;
   
   if (Clip(src , dest , srcx , srcy , destx , desty , w , h)) {return;}

   if (is_video_bitmap(dest)) {acquire_bitmap(dest);}
   sy = srcy;
   dy = desty;
   for (int y = 0 ; y < h ; ++y) {
      sx = srcx;
      dx = destx;
      for (int x = 0 ; x < w ; ++x) {
         col = gpx_func(src , sx , sy);
         r = getr_func(col);
         g = getg_func(col);
         b = getb_func(col);
         r += ((tr - r)*n)/255;
         g += ((tg - g)*n)/255;
         b += ((tb - b)*n)/255;
         col = mkcol_func(r,g,b);
         ppx_func(dest , dx , dy , col);
         ++sx;
         ++dx;
      }
      ++sy;
      ++dy;
   }
   if (is_video_bitmap(dest)) {release_bitmap(dest);}
}



void set_hue_blit(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h , float hue) {
   ASSERT(src);
   ASSERT(dest);
   
   hue = CLAMP(0.0f , hue , 360.0f);
   const GPX_FUNC gpx_func(src->vtable->getpixel);
   const PPX_FUNC ppx_func(dest->vtable->putpixel);
   COLORDEPTH srccdi = ColorDepthIndex(bitmap_color_depth(src));
   const GETR_FUNC getr_func(getr_functions[srccdi]);
   const GETG_FUNC getg_func(getg_functions[srccdi]);
   const GETB_FUNC getb_func(getb_functions[srccdi]);
   COLORDEPTH destcdi = ColorDepthIndex(bitmap_color_depth(dest));
   const MKCOL_FUNC mkcol_func(makecol_functions[destcdi]);
   int col,r,g,b;
   float oldhue,s,v;
   
   int sx,sy,dx,dy;
   
   if (Clip(src , dest , srcx , srcy , destx , desty , w , h)) {return;}

   if (is_video_bitmap(dest)) {acquire_bitmap(dest);}
   sy = srcy;
   dy = desty;
   for (int y = 0 ; y < h ; ++y) {
      sx = srcx;
      dx = destx;
      for (int x = 0 ; x < w ; ++x) {
         col = gpx_func(src , sx , sy);
         r = getr_func(col);
         g = getg_func(col);
         b = getb_func(col);
         rgb_to_hsv(r,g,b,&oldhue,&s,&v);
         hsv_to_rgb(hue,s,v,&r,&g,&b);
         col = mkcol_func(r,g,b);
         ppx_func(dest,dx,dy,col);
         ++sx;
         ++dx;
      }
      ++sy;
      ++dy;
   }
   if (is_video_bitmap(dest)) {release_bitmap(dest);}
}



void invert_rgb_blit(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h) {
   ASSERT(src);
   ASSERT(dest);
   
   const GPX_FUNC gpx_func(src->vtable->getpixel);
   const PPX_FUNC ppx_func(dest->vtable->putpixel);
   COLORDEPTH srccdi = ColorDepthIndex(bitmap_color_depth(src));
   const GETR_FUNC getr_func(getr_functions[srccdi]);
   const GETG_FUNC getg_func(getg_functions[srccdi]);
   const GETB_FUNC getb_func(getb_functions[srccdi]);
   COLORDEPTH destcdi = ColorDepthIndex(bitmap_color_depth(dest));
   const MKCOL_FUNC mkcol_func(makecol_functions[destcdi]);
   int col,r,g,b;
   
   int sx,sy,dx,dy;
   
   if (Clip(src , dest , srcx , srcy , destx , desty , w , h)) {return;}

   if (is_video_bitmap(dest)) {acquire_bitmap(dest);}
   sy = srcy;
   dy = desty;
   for (int y = 0 ; y < h ; ++y) {
      sx = srcx;
      dx = destx;
      for (int x = 0 ; x < w ; ++x) {
         col = gpx_func(src , sx , sy);
         r = getr_func(col);
         g = getg_func(col);
         b = getb_func(col);
         r = 255 - r;
         g = 255 - g;
         b = 255 - b;
         col = mkcol_func(r,g,b);
         ppx_func(dest,dx,dy,col);
         ++sx;
         ++dx;
      }
      ++sy;
      ++dy;
   }
   if (is_video_bitmap(dest)) {release_bitmap(dest);}
}



void invert_value_blit(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h) {
   ASSERT(src);
   ASSERT(dest);
   
   const GPX_FUNC gpx_func(src->vtable->getpixel);
   const PPX_FUNC ppx_func(dest->vtable->putpixel);
   COLORDEPTH srccdi = ColorDepthIndex(bitmap_color_depth(src));
   const GETR_FUNC getr_func(getr_functions[srccdi]);
   const GETG_FUNC getg_func(getg_functions[srccdi]);
   const GETB_FUNC getb_func(getb_functions[srccdi]);
   COLORDEPTH destcdi = ColorDepthIndex(bitmap_color_depth(dest));
   const MKCOL_FUNC mkcol_func(makecol_functions[destcdi]);
   int col,r,g,b;
   float hue,s,v;
   
   int sx,sy,dx,dy;
   
   if (Clip(src , dest , srcx , srcy , destx , desty , w , h)) {return;}

   if (is_video_bitmap(dest)) {acquire_bitmap(dest);}
   sy = srcy;
   dy = desty;
   for (int y = 0 ; y < h ; ++y) {
      sx = srcx;
      dx = destx;
      for (int x = 0 ; x < w ; ++x) {
         col = gpx_func(src , sx , sy);
         r = getr_func(col);
         g = getg_func(col);
         b = getb_func(col);
         rgb_to_hsv(r,g,b,&hue,&s,&v);
         v = 1.0 - v;
         hsv_to_rgb(hue,s,v,&r,&g,&b);
         col = mkcol_func(r,g,b);
         ppx_func(dest,dx,dy,col);
         ++sx;
         ++dx;
      }
      ++sy;
      ++dy;
   }
   if (is_video_bitmap(dest)) {release_bitmap(dest);}
   
}



void blur_blit(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h , int radius) {
   ASSERT(src);
   ASSERT(dest);
   ASSERT(radius > 0);
   
   if (radius < 1) {return;}
   if (Clip(src,dest,srcx,srcy,destx,desty,w,h)) {return;}
   
   const COLORDEPTH dest_cdi = ColorDepthIndex(bitmap_color_depth(dest));
   const PPX_FUNC ppx = dest->vtable->putpixel;

   const GPX_FUNC gpx = src->vtable->getpixel;
   COLORDEPTH cdi = ColorDepthIndex(bitmap_color_depth(src));
   const GETR_FUNC grfunc = getr_functions[cdi];
   const GETG_FUNC ggfunc = getg_functions[cdi];
   const GETB_FUNC gbfunc = getb_functions[cdi];
   const MKCOL_FUNC mkcol = makecol_functions[dest_cdi];

   const float width = 2*radius + 1;
   const float height = 2*radius + 1;

   int sy = srcy;
   int dy = desty;
   int sx,dx;
   int col;
   
   for (int y = 0 ; y < h ; ++y) {
      sx = srcx;
      dx = destx;
      for (int x = 0 ; x < w ; ++x) {
         col = AverageColor(src , (float)(sx - radius) , (float)(sy - radius) , width , height , dest_cdi,
                            gpx , cdi , grfunc , ggfunc , gbfunc , mkcol);
         ppx(dest , dx , dy , col);
         ++sx;
         ++dx;
      }
      ++sy;
      ++dy;
   }
}



void stretch_blit_smooth(BITMAP* src , BITMAP* dest , int srcx , int srcy , int srcw , int srch , int destx , int desty , int destw , int desth) {
   ASSERT(src);
   ASSERT(dest);
   // The source bitmap is not clipped. Debug mode will catch errors, but non debug mode will crash if you
   // attempt to draw from outside the source bitmap.
   ASSERT(srcx >= 0);
   ASSERT(srcy >= 0);
   ASSERT(srcx + srcw <= src->w);
   ASSERT(srcy + srch <= src->h);
   
   if ((srcw == destw) && (srch == desth)) {
      blit(src,dest,srcx,srcy,destx,desty,srcw,srch);
      return;
   }

   const PPX_FUNC ppx = dest->vtable->putpixel;
   COLORDEPTH dest_cdi = ColorDepthIndex(bitmap_color_depth(dest));
   
   const GPX_FUNC gpx = src->vtable->getpixel;
   COLORDEPTH cdi = ColorDepthIndex(bitmap_color_depth(src));
   const GETR_FUNC grfunc = getr_functions[cdi];
   const GETG_FUNC ggfunc = getg_functions[cdi];
   const GETB_FUNC gbfunc = getb_functions[cdi];
   const MKCOL_FUNC mkcol = makecol_functions[dest_cdi];

   const float hsptodp = (float)srcw/(float)destw;// horizontal source pixels to destination pixels ratio
   const float vsptodp = (float)srch/(float)desth;// vertical source pixels to destination pixels ratio
   
   float sy,sx;
   
   int ystop = desty + desth;
   int xstop = destx + destw;
   if (ystop > dest->h) {ystop = dest->h;}
   if (xstop > dest->w) {xstop = dest->w;}
   
   sy = (float)srcy;
   for (int y = desty ; y < ystop ; ++y) {
      sx = (float)srcx;
      for (int x = destx ; x < xstop ; ++x) {
         int col = AverageColor(src , sx , sy , hsptodp , vsptodp , dest_cdi,
                                gpx , cdi , grfunc , ggfunc , gbfunc , mkcol);
         ppx(dest , x , y , col);
         sx += hsptodp;
      }
      sy += vsptodp;
   }
}



void masked_tint_blit(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h , int tint_color , int n) {
   ASSERT(src);
   ASSERT(dest);
   
   n = CLAMP(0,n,255);
   const int maskcolor = bitmap_mask_color(src);
   const GPX_FUNC gpx_func(src->vtable->getpixel);
   const PPX_FUNC ppx_func(dest->vtable->putpixel);
   COLORDEPTH srccdi = ColorDepthIndex(bitmap_color_depth(src));
   const GETR_FUNC getr_func(getr_functions[srccdi]);
   const GETG_FUNC getg_func(getg_functions[srccdi]);
   const GETB_FUNC getb_func(getb_functions[srccdi]);
   COLORDEPTH destcdi = ColorDepthIndex(bitmap_color_depth(dest));
   const MKCOL_FUNC mkcol_func(makecol_functions[destcdi]);
   int col,r,g,b;
   const int tr = getr(tint_color);
   const int tg = getg(tint_color);
   const int tb = getb(tint_color);
   
   int sx,sy,dx,dy;
   
   if (Clip(src , dest , srcx , srcy , destx , desty , w , h)) {return;}

   if (is_video_bitmap(dest)) {acquire_bitmap(dest);}
   sy = srcy;
   dy = desty;
   for (int y = 0 ; y < h ; ++y) {
      sx = srcx;
      dx = destx;
      for (int x = 0 ; x < w ; ++x) {
         col = gpx_func(src , sx , sy);
         if (col != maskcolor) {
            r = getr_func(col);
            g = getg_func(col);
            b = getb_func(col);
            r += ((tr - r)*n)/255;
            g += ((tg - g)*n)/255;
            b += ((tb - b)*n)/255;
            col = mkcol_func(r,g,b);
            ppx_func(dest , dx , dy , col);
         }
         ++sx;
         ++dx;
      }
      ++sy;
      ++dy;
   }
   if (is_video_bitmap(dest)) {release_bitmap(dest);}
}



void masked_set_hue_blit(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h , float hue) {
   ASSERT(src);
   ASSERT(dest);
   
   hue = CLAMP(0.0f , hue , 360.0f);
   const int maskcolor = bitmap_mask_color(src);
   const GPX_FUNC gpx_func(src->vtable->getpixel);
   const PPX_FUNC ppx_func(dest->vtable->putpixel);
   COLORDEPTH srccdi = ColorDepthIndex(bitmap_color_depth(src));
   const GETR_FUNC getr_func(getr_functions[srccdi]);
   const GETG_FUNC getg_func(getg_functions[srccdi]);
   const GETB_FUNC getb_func(getb_functions[srccdi]);
   COLORDEPTH destcdi = ColorDepthIndex(bitmap_color_depth(dest));
   const MKCOL_FUNC mkcol_func(makecol_functions[destcdi]);
   int col,r,g,b;
   float oldhue,s,v;
   
   int sx,sy,dx,dy;
   
   if (Clip(src , dest , srcx , srcy , destx , desty , w , h)) {return;}

   if (is_video_bitmap(dest)) {acquire_bitmap(dest);}
   sy = srcy;
   dy = desty;
   for (int y = 0 ; y < h ; ++y) {
      sx = srcx;
      dx = destx;
      for (int x = 0 ; x < w ; ++x) {
         col = gpx_func(src , sx , sy);
         if (col != maskcolor) {
            r = getr_func(col);
            g = getg_func(col);
            b = getb_func(col);
            rgb_to_hsv(r,g,b,&oldhue,&s,&v);
            hsv_to_rgb(hue,s,v,&r,&g,&b);
            col = mkcol_func(r,g,b);
            ppx_func(dest,dx,dy,col);
         }
         ++sx;
         ++dx;
      }
      ++sy;
      ++dy;
   }
   if (is_video_bitmap(dest)) {release_bitmap(dest);}
}



void masked_invert_rgb_blit(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h) {
   ASSERT(src);
   ASSERT(dest);
   
   const GPX_FUNC gpx_func(src->vtable->getpixel);
   const PPX_FUNC ppx_func(dest->vtable->putpixel);
   COLORDEPTH srccdi = ColorDepthIndex(bitmap_color_depth(src));
   const GETR_FUNC getr_func(getr_functions[srccdi]);
   const GETG_FUNC getg_func(getg_functions[srccdi]);
   const GETB_FUNC getb_func(getb_functions[srccdi]);
   COLORDEPTH destcdi = ColorDepthIndex(bitmap_color_depth(dest));
   const MKCOL_FUNC mkcol_func(makecol_functions[destcdi]);
   const int maskcolor = bitmap_mask_color(src);
   
   int col,r,g,b;
   
   int sx,sy,dx,dy;
   
   if (Clip(src , dest , srcx , srcy , destx , desty , w , h)) {return;}

   if (is_video_bitmap(dest)) {acquire_bitmap(dest);}
   sy = srcy;
   dy = desty;
   for (int y = 0 ; y < h ; ++y) {
      sx = srcx;
      dx = destx;
      for (int x = 0 ; x < w ; ++x) {
         col = gpx_func(src , sx , sy);
         if (col != maskcolor) {
            r = getr_func(col);
            g = getg_func(col);
            b = getb_func(col);
            r = 255 - r;
            g = 255 - g;
            b = 255 - b;
            col = mkcol_func(r,g,b);
            ppx_func(dest,dx,dy,col);
         }
         ++sx;
         ++dx;
      }
      ++sy;
      ++dy;
   }
   if (is_video_bitmap(dest)) {release_bitmap(dest);}
   
}



void masked_invert_value_blit(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h) {
   ASSERT(src);
   ASSERT(dest);
   
   const GPX_FUNC gpx_func(src->vtable->getpixel);
   const PPX_FUNC ppx_func(dest->vtable->putpixel);
   COLORDEPTH srccdi = ColorDepthIndex(bitmap_color_depth(src));
   const GETR_FUNC getr_func(getr_functions[srccdi]);
   const GETG_FUNC getg_func(getg_functions[srccdi]);
   const GETB_FUNC getb_func(getb_functions[srccdi]);
   COLORDEPTH destcdi = ColorDepthIndex(bitmap_color_depth(dest));
   const MKCOL_FUNC mkcol_func(makecol_functions[destcdi]);
   const int maskcolor = bitmap_mask_color(src);
   
   int col,r,g,b;
   float hue,s,v;
   
   int sx,sy,dx,dy;
   
   if (Clip(src , dest , srcx , srcy , destx , desty , w , h)) {return;}

   if (is_video_bitmap(dest)) {acquire_bitmap(dest);}
   sy = srcy;
   dy = desty;
   for (int y = 0 ; y < h ; ++y) {
      sx = srcx;
      dx = destx;
      for (int x = 0 ; x < w ; ++x) {
         col = gpx_func(src , sx , sy);
         if (col != maskcolor) {
            r = getr_func(col);
            g = getg_func(col);
            b = getb_func(col);
            rgb_to_hsv(r,g,b,&hue,&s,&v);
            v = 1.0 - v;
            hsv_to_rgb(hue,s,v,&r,&g,&b);
            col = mkcol_func(r,g,b);
            ppx_func(dest,dx,dy,col);
         }
         ++sx;
         ++dx;
      }
      ++sy;
      ++dy;
   }
   if (is_video_bitmap(dest)) {release_bitmap(dest);}
  }



void masked_stretch_blit_smooth(BITMAP* src , BITMAP* dest , int srcx , int srcy , int srcw , int srch , int destx , int desty , int destw , int desth) {
   ASSERT(src);
   ASSERT(dest);
   // The source bitmap is not clipped. Debug mode will catch errors, but non debug mode will crash if you
   // attempt to draw from outside the source bitmap.
   ASSERT(srcx >= 0);
   ASSERT(srcy >= 0);
   ASSERT(srcx + srcw <= src->w);
   ASSERT(srcy + srch <= src->h);
   
   if ((srcw == destw) && (srch == desth)) {
      masked_blit(src,dest,srcx,srcy,destx,desty,srcw,srch);
      return;
   }

   const PPX_FUNC ppx = dest->vtable->putpixel;
   COLORDEPTH dest_cdi = ColorDepthIndex(bitmap_color_depth(dest));
   const int dest_mask_col = bitmap_mask_color(dest);
   
   const float hsptodp = (float)srcw/(float)destw;// horizontal source pixels to destination pixels ratio
   const float vsptodp = (float)srch/(float)desth;// vertical source pixels to destination pixels ratio
   
   float sy,sx;
   
   int ystop = desty + desth;
   int xstop = destx + destw;
   if (ystop > dest->h) {ystop = dest->h;}
   if (xstop > dest->w) {xstop = dest->w;}
   
   sy = (float)srcy;
   for (int y = desty , i = 0 ; y < ystop ; ++y , ++i) {
//      sy = (float)srcy + (float)i*vsptodp;
      sx = (float)srcx;
      for (int x = destx , j = 0 ; x < xstop ; ++x , ++j) {
//         sx = (float)srcx + (float)j*hsptodp;
         int col = MaskedAverageColor(src , sx , sy , hsptodp , vsptodp , dest_cdi);
         if (col != dest_mask_col) {
            ppx(dest , x , y , col);
         }
         sx += hsptodp;
      }
      sy += vsptodp;
   }
}



void blit_mask(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h) {
   ASSERT(src);
   ASSERT(dest);
   
   const int srcmaskcolor = bitmap_mask_color(src);
   const int destmaskcolor = bitmap_mask_color(dest);
   
   const GPX_FUNC gpx_func = src->vtable->getpixel;
   const PPX_FUNC ppx_func = dest->vtable->putpixel;
   
   if (Clip(src , dest , srcx , srcy , destx , desty , w , h)) {return;}

   int sx,dx,sy,dy;
   int col;
   
   acquire_bitmap(dest);
   sy = srcy;
   dy = desty;
   for (int y = 0 ; y < h ; ++y) {
      sx = srcx;
      dx = destx;
      for (int x = 0 ; x < w ; ++x) {
         col = gpx_func(src , sx , sy);
         if (col == srcmaskcolor) {
            ppx_func(dest , dx , dy , destmaskcolor);
         }
         ++sx;
         ++dx;
      }
      ++sy;
      ++dy;
   }
   release_bitmap(dest);
}



void convert_to_alpha_blit(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h) {
   ASSERT(src);
   ASSERT(dest);
   ASSERT(bitmap_color_depth(dest) == 32);
   
   if (Clip(src , dest , srcx , srcy , destx , desty , w , h)) {return;}
   
   const int maskcolor = bitmap_mask_color(src);
   const GPX_FUNC gpx = src->vtable->getpixel;
   const PPX_FUNC ppx = dest->vtable->putpixel;
   const COLORDEPTH cdi = ColorDepthIndex(bitmap_color_depth(src));
   const GETR_FUNC gr = getr_functions[cdi];
   const GETG_FUNC gg = getg_functions[cdi];
   const GETB_FUNC gb = getb_functions[cdi];
   
   int sx = srcx;
   int sy = srcy;
   int dx = destx;
   int dy = desty;
   int r,g,b,a,col;
   
   if (is_video_bitmap(dest)) {acquire_bitmap(dest);}
   for (int y = 0 ; y < h ; ++y) {
      sx = srcx;
      dx = destx;
      for (int x = 0 ; x < w ; ++x) {
         col = gpx(src , sx , sy);
         if (col == maskcolor) {
            r = g = b = a = 0;
         } else {
            r = gr(col);
            g = gg(col);
            b = gb(col);
            a = 255;
         }
         col = makeacol(r,g,b,a);
         ppx(dest , dx , dy , col);
         ++sx;
         ++dx;
      }
      ++sy;
      ++dy;
   }
   if (is_video_bitmap(dest)) {release_bitmap(dest);}
}



void trans_blit(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h) {
   ASSERT(src);
   ASSERT(dest);
   ASSERT(bitmap_color_depth(src) == 32);
   
//int Clip(BITMAP* src , BITMAP* dest , int& srcx , int& srcy , int& destx , int& desty , int& w , int& h);// returns 1 if fully clipped, 0 if not
   if (Clip(src , dest , srcx , srcy , destx , desty , w , h)) {return;}
   
   const COLORDEPTH destcdi = ColorDepthIndex(bitmap_color_depth(dest));
   const GPX_FUNC sgpx = src->vtable->getpixel;
   const GPX_FUNC dgpx = dest->vtable->getpixel;
   const PPX_FUNC ppx = dest->vtable->putpixel;
///   const GETR_FUNC sgr = getr_functions[cdi];
///   const GETG_FUNC sgg = getg_functions[cdi];
///   const GETB_FUNC sgb = getb_functions[cdi];
//   const GETA_FUNC sga = geta_functions[cdi];
   const GETR_FUNC dgr = getr_functions[destcdi];
   const GETG_FUNC dgg = getg_functions[destcdi];
   const GETB_FUNC dgb = getb_functions[destcdi];
//   const GETA_FUNC dga = geta_functions[destcdi];
   const MKCOL_FUNC mkcol = makecol_functions[destcdi];
   
   int sy = srcy;
   int dy = desty;
   int sx,dx;
   int sr,sg,sb,sa;
   int dr,dg,db;
   int br,bg,bb;
   int opt1;// optimizer 1
//   int a;
   int col;
   
   if (destcdi == BPP32) {
      for (int y = 0 ; y < h ; ++y) {
         sx = srcx;
         dx = destx;
         for (int x = 0 ; x < w ; ++x) {
            col = sgpx(src , sx , sy);
            sa = geta32(col);
            if (sa == 0) {
               ++sx;
               ++dx;
               continue;
            }
            if (sa == 255) {
               ppx(dest , dx , dy , col);
               ++sx;
               ++dx;
            }
            sr = getr32(col);
            sg = getg32(col);
            sb = getb32(col);
            col = dgpx(dest , dx , dy);
            dr = dgr(col);
            dg = dgg(col);
            db = dgb(col);

            opt1 = 255 - sa;
            br = (sr*sa + dr*opt1)/255;
            bg = (sg*sa + dg*opt1)/255;
            bb = (sb*sa + db*opt1)/255;
            col = makeacol(br,bg,bb,255);
            ppx(dest , dx , dy , col);
            ++sx;
            ++dx;
         }
         ++sy;
         ++dy;
      }
   } else {
      for (int y = 0 ; y < h ; ++y) {
         sx = srcx;
         dx = destx;
         for (int x = 0 ; x < w ; ++x) {
            col = sgpx(src , sx , sy);
            sa = geta32(col);
            if (sa == 0) {
               ++sx;
               ++dx;
               continue;
            }
            if (sa == 255) {
               ppx(dest , dx , dy , col);
               ++sx;
               ++dx;
            }
            sr = getr32(col);
            sg = getg32(col);
            sb = getb32(col);
            col = dgpx(dest , dx , dy);
            dr = dgr(col);
            dg = dgg(col);
            db = dgb(col);
            
            opt1 = 255 - sa;
            br = (sr*sa + dr*opt1)/255;
            bg = (sg*sa + dg*opt1)/255;
            bb = (sb*sa + db*opt1)/255;
            // (src*sa + dest*(255-sa))/255;
            // (src*sa + 255*dest - sa*dest)/255;
            // (sa*(src - dest) + 255*dest)/255;
            // (sa*(src - dest))/255 + dest
            col = mkcol(br,bg,bb);
            ppx(dest , dx , dy , col);
            ++sx;
            ++dx;
         }
         
         ++sy;
         ++dy;
      }
   }
   // one subtraction, 6 multiplications, 3 additions, 3 divisions, 4 assignments
}
//** For reference
unsigned long _blender_alpha32(unsigned long x, unsigned long y, unsigned long n)
{
   unsigned long res, g;

   n = geta32(x);

   if (n)
      n++;

   res = ((x & 0xFF00FF) - (y & 0xFF00FF)) * n / 256 + y;
   // ((src - dest)*sa)/255 + dest
   // ((src - dest)*sa)/255 + (255*src)/255
   // ((src*sa - dest*sa) + 255*src)/255
   // (src(sa + 255) - dest*sa)/255
   y &= 0xFF00;
   x &= 0xFF00;
   g = (x - y) * n / 256 + y;

   res &= 0xFF00FF;
   g &= 0xFF00;

   return res | g;
}
// 6 bitwise ands, 3 additions, 2 subtractions, 2 multiplications, 2 divisions, 6 assignments, 1 bitwise or
//*/

void trans_layer_blit_mem_to_mem(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h);
void trans_layer_blit_mem_to_sysvid(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h);
void trans_layer_blit_sysvid_to_mem(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h);
void trans_layer_blit_sysvid_to_sysvid(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h);

void trans_layer_blit(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h) {
   ASSERT(src);
   ASSERT(dest);
   ASSERT(src != dest);
   ASSERT(bitmap_color_depth(src) == 32);
   ASSERT(bitmap_color_depth(dest) == 32);
   
   if (Clip(src,dest,srcx,srcy,destx,desty,w,h)) {return;}
   
///   trans_layer_blit_safe(src,dest,srcx,srcy,destx,desty,w,h);
///   return;
   
   if (is_memory_bitmap(src)) {
      /// From memory...
      if (is_memory_bitmap(dest)) {
         /// To memory...
         trans_layer_blit_mem_to_mem(src,dest,srcx,srcy,destx,desty,w,h);
         return;
      } else {
         /// To system/video...
         trans_layer_blit_mem_to_sysvid(src,dest,srcx,srcy,destx,desty,w,h);
         return;
      }
   } else {
      /// From system/video...
      if (is_memory_bitmap(dest)) {
         /// To memory...
         trans_layer_blit_sysvid_to_mem(src,dest,srcx,srcy,destx,desty,w,h);
         return;
      } else {
         /// To system/video
         trans_layer_blit_sysvid_to_sysvid(src,dest,srcx,srcy,destx,desty,w,h);
         return;
      }
   }

}


/// ORIGINAL, KEEP
void trans_layer_blit_safe(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h) {
/**
   ASSERT(src);
   ASSERT(dest);
   ASSERT(bitmap_color_depth(src) == 32);
   ASSERT(bitmap_color_depth(dest) == 32);
*/   
//int Clip(BITMAP* src , BITMAP* dest , int& srcx , int& srcy , int& destx , int& desty , int& w , int& h);// returns 1 if fully clipped, 0 if not
///   if (Clip(src , dest , srcx , srcy , destx , desty , w , h)) {return;}
   
   const GPX_FUNC sgpx = src->vtable->getpixel;
   const GPX_FUNC dgpx = dest->vtable->getpixel;
   const PPX_FUNC ppx = dest->vtable->putpixel;
   
   int sr,sg,sb,sa;
   int dr,dg,db,da;
   int br,bg,bb,ba;
   int opt0,opt1,opt2,opt3;// optimizers
   int col;
   
   int sy = srcy;
   int dy = desty;
   int sx,dx;
   
   for (int y = 0 ; y < h ; ++y) {
      sx = srcx;
      dx = destx;
      for (int x = 0 ; x < w ; ++x) {
         col = sgpx(src , sx , sy);
         sa = geta32(col);
         if (sa == 0) {
            ++sx;
            ++dx;
            continue;
         }
         if (sa == 255) {
            ppx(dest , dx , dy , col);
            ++sx;
            ++dx;
            continue;
         }
         sr = getr32(col);
         sg = getg32(col);
         sb = getb32(col);
         col = dgpx(dest , dx , dy);
         dr = getr32(col);
         dg = getg32(col);
         db = getb32(col);
         da = geta32(col);
         
         opt0 = 255-sa;// 1 subtract instead of 3
         opt1 = da*opt0;// 1 multiply instead of 3
         opt2 = 255*sa;// 1 multiply instead of 3
         opt3 = 65025 - opt0*(255-da);// 2 subtract and 1 multiply instead of 8 subtract and 4 multiply
         
         br = (dr*opt1 + sr*opt2)/opt3;
         bg = (dg*opt1 + sg*opt2)/opt3;
         bb = (db*opt1 + sb*opt2)/opt3;
         ba = opt3/255;
         
         ppx(dest , dx , dy , makeacol(br,bg,bb,ba));
         ++sx;
         ++dx;
      }
      ++sy;
      ++dy;
   }
}



void trans_layer_blit_mem_to_mem(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h) {
   
   int* srcline = 0;
   int* destline = 0;
   
   int sr,sg,sb,sa;
   int dr,dg,db,da;
   int br,bg,bb,ba;
   int opt0,opt1,opt2,opt3;
   int col;
   
   int sy = srcy;
   int dy = desty;
   
   for (int y = 0 ; y < h ; ++y) {
      srcline = (int*)src->line[sy] + srcx;
      destline = (int*)dest->line[dy] + destx;
      for (int x = 0 ; x < w ; ++x) {
         col = *srcline;
         sa = geta32(col);
         if (sa == 0) {
            ++srcline;
            ++destline;
            continue;
         }
         if (sa == 255) {
            *destline = col;
            ++srcline;
            ++destline;
         }
         sr = getr32(col);
         sg = getg32(col);
         sb = getb32(col);
         col = *destline;
         dr = getr32(col);
         dg = getg32(col);
         db = getb32(col);
         da = geta32(col);
         
         opt0 = 255-sa;// 1 subtract instead of 4
         opt1 = da*opt0;// 1 multiply instead of 3
         opt2 = 255*sa;// 1 multiply instead of 3
         opt3 = 65025 - opt0*(255-da);// 2 subtract and 1 multiply instead of 8 subtract and 4 multiply
         
         ba = opt3/255;
         br = (dr*opt1 + sr*opt2)/opt3;
         bg = (dg*opt1 + sg*opt2)/opt3;
         bb = (db*opt1 + sb*opt2)/opt3;
         
         *destline = makeacol(br,bg,bb,ba);
         ++srcline;
         ++destline;
      }
      ++sy;
      ++dy;
   }
}



void trans_layer_blit_mem_to_sysvid(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h) {
   
   int* srcline = 0;
   unsigned long destline = 0;
   
   int sr,sg,sb,sa;
   int dr,dg,db,da;
   int br,bg,bb,ba;
   int opt0,opt1,opt2,opt3;
   int col;
   
   int sy = srcy;
   int dy = desty;

   int* readdest = (int*)malloc(sizeof(int)*w);
   if (!readdest) {return;}
   int* read = readdest;
   
   bmp_select(dest);
//   acquire_bitmap(dest);
   for (int y = 0 ; y < h ; ++y) {
      srcline = (int*)src->line[sy] + srcx;

      destline = bmp_read_line(dest , dy) + 4*destx;
      read = readdest;
      for (int x = 0 ; x < w ; ++x) {
         *read = bmp_read32(destline);
         ++read;
         destline += 4;
      }
      read = readdest;
      destline = bmp_write_line(dest , dy) + 4*destx;
      for (int x = 0 ; x < w ; ++x) {
         col = *srcline;
         sa = geta32(col);
         if (sa == 0) {
            ++read;
            ++srcline;
            destline += 4;
            continue;
         }
         if (sa == 255) {
            bmp_write32(destline , col);
            ++read;
            ++srcline;
            destline += 4;
         }
         sr = getr32(col);
         sg = getg32(col);
         sb = getb32(col);
         col = *read;
         dr = getr32(col);
         dg = getg32(col);
         db = getb32(col);
         da = geta32(col);
         
         opt0 = 255-sa;// 1 subtract instead of 4
         opt1 = da*opt0;// 1 multiply instead of 3
         opt2 = 255*sa;// 1 multiply instead of 3
         opt3 = 65025 - opt0*(255-da);// 2 subtract and 1 multiply instead of 8 subtract and 4 multiply
         
         ba = opt3/255;
         br = (dr*opt1 + sr*opt2)/opt3;
         bg = (dg*opt1 + sg*opt2)/opt3;
         bb = (db*opt1 + sb*opt2)/opt3;
         
         bmp_write32(destline , makeacol(br,bg,bb,ba));
         ++read;
         ++srcline;
         destline += 4;
      }
      ++sy;
      ++dy;
   }
   bmp_unwrite_line(dest);
//   release_bitmap(dest);
   free(readdest);
}



void trans_layer_blit_sysvid_to_mem(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h) {
   
   unsigned long srcline = 0;
   int* destline = 0;
   
   int sr,sg,sb,sa;
   int dr,dg,db,da;
   int br,bg,bb,ba;
   int opt0,opt1,opt2,opt3;
   int col;
   
   int sy = srcy;
   int dy = desty;
   
   bmp_select(src);
   for (int y = 0 ; y < h ; ++y) {
      srcline = bmp_read_line(src , sy) + 4*srcx;
      destline = (int*)dest->line[dy] + destx;
      for (int x = 0 ; x < w ; ++x) {
         col = bmp_read32(srcline);
         sa = geta32(col);
         if (sa == 0) {
            srcline += 4;
            ++destline;
            continue;
         }
         if (sa == 255) {
            *destline = col;
            srcline += 4;
            ++destline;
            continue;
         }
         sr = getr32(col);
         sg = getg32(col);
         sb = getb32(col);
         col = *destline;
         dr = getr32(col);
         dg = getg32(col);
         db = getb32(col);
         da = geta32(col);
         
         opt0 = 255-sa;// 1 subtract instead of 4
         opt1 = da*opt0;// 1 multiply instead of 3
         opt2 = 255*sa;// 1 multiply instead of 3
         opt3 = 65025 - opt0*(255-da);// 2 subtract and 1 multiply instead of 8 subtract and 4 multiply
         
         ba = opt3/255;
         br = (dr*opt1 + sr*opt2)/opt3;
         bg = (dg*opt1 + sg*opt2)/opt3;
         bb = (db*opt1 + sb*opt2)/opt3;
         
         *destline = makeacol(br,bg,bb,ba);
         srcline += 4;
         ++destline;
      }
      ++sy;
      ++dy;
   }
   bmp_unwrite_line(src);
}



void trans_layer_blit_sysvid_to_sysvid(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h) {
   
   unsigned long srcline = 0;
   unsigned long destline = 0;
   
   int sr,sg,sb,sa;
   int dr,dg,db,da;
   int br,bg,bb,ba;
   int opt0,opt1,opt2,opt3;
   int col;
   
   int* readsrc = (int*)malloc(sizeof(int)*w);
   int* readdest = (int*)malloc(sizeof(int)*w);
   
   if (!readsrc || !readdest) {
      if (readsrc) {free(readsrc);}
      if (readdest) {free(readdest);}
      return;
   }
   
   int* rs;
   int* rd;
   
   int sy = srcy;
   int dy = desty;
   
//   acquire_bitmap(dest);
   for (int y = 0 ; y < h ; ++y) {
      rs = readsrc;
      bmp_select(src);
      srcline = bmp_read_line(src , sy) + 4*srcx;
      for (int x = 0 ; x < w ; ++x) {
         *rs = bmp_read32(srcline);
         ++rs;
         srcline += 4;
      }
      rd = readdest;
      bmp_select(dest);
      destline = bmp_read_line(dest , dy) + 4*destx;
      for (int x = 0 ; x < w ; ++x) {
         *rd = bmp_read32(destline);
         ++rd;
         destline += 4;
      }
      rs = readsrc;
      rd = readdest;
      destline = bmp_write_line(dest , dy) + 4*destx;
      for (int x = 0 ; x < w ; ++x) {
         col = *rs;
         sa = geta32(col);
         if (sa == 0) {
            ++rs;
            ++rd;
            destline += 4;
            continue;
         }
         if (sa == 255) {
            bmp_write32(destline , col);
            ++rs;
            ++rd;
            destline += 4;
            continue;
         }
         sr = getr32(col);
         sg = getg32(col);
         sb = getb32(col);
         col = *rd;
         dr = getr32(col);
         dg = getg32(col);
         db = getb32(col);
         da = geta32(col);
         
         opt0 = 255-sa;// 1 subtract instead of 4
         opt1 = da*opt0;// 1 multiply instead of 3
         opt2 = 255*sa;// 1 multiply instead of 3
         opt3 = 65025 - opt0*(255-da);// 2 subtract and 1 multiply instead of 8 subtract and 4 multiply
         
         ba = opt3/255;
         br = (dr*opt1 + sr*opt2)/opt3;
         bg = (dg*opt1 + sg*opt2)/opt3;
         bb = (db*opt1 + sb*opt2)/opt3;
         
         bmp_write32(destline , makeacol(br,bg,bb,ba));
         ++rs;
         ++rd;
         destline += 4;
      }
      ++sy;
      ++dy;
   }
   bmp_unwrite_line(src);
   bmp_unwrite_line(dest);
//   release_bitmap(dest);
   free(readsrc);
   free(readdest);
}



