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

using std::ostream;

void Color::RefreshRGBA() {
   r = getr_functions[cd_index](color);
   g = getg_functions[cd_index](color);
   b = getb_functions[cd_index](color);
   if (cd == 32) {
      a = geta32(color);
   }
}



void Color::RefreshColor() {
   if (cd == 32) {
      color = makeacol(r,g,b,a);
   } else {
      color = makecol_functions[cd_index](r,g,b);
   }
}


/** Constructors */

Color::Color() : 
      r(0) , g(0) , b(0) , a(255) , color(0) , cd(32) , cd_index(BPP32) 
{
///   RefreshColor();// commented out so this constructor is safe for global Color objects
}



Color::Color(int al_color) :
      r(0) , g(0) , b(0) , a(255) , color(al_color) , cd(32) , cd_index(BPP32) {
   /* Uses current color_depth of screen if available or else get_color_depth. */
   cd = AllegroColorDepth();
   cd_index = ColorDepthIndex(cd);
   r = getr_functions[cd_index](al_color);
   g = getg_functions[cd_index](al_color);
   b = getb_functions[cd_index](al_color);
   if (cd_index == BPP32) {
      a = geta32(al_color);
   }
}



Color::Color(UCHAR red , UCHAR green , UCHAR blue , int color_depth) :
      r(red) , g(green) , b(blue) , a(255) , color(0) , cd(32) , cd_index(BPP32) {
   SetColorDepth(color_depth);
}



Color::Color(UCHAR red , UCHAR green , UCHAR blue , UCHAR alpha , int color_depth) :
            r(red) , g(green) , b(blue) , a(alpha) , color(0) , cd(32) , cd_index(BPP32) {
   SetColorDepth(color_depth);
}



void Color::SetColor(int col) {
   color = col;
   RefreshRGBA();
}



void Color::SetRGB(UCHAR red , UCHAR green , UCHAR blue) {
   r = red;
   g = green;
   b = blue;
   RefreshColor();
}



void Color::SetRGBex(UCHAR red , UCHAR green , UCHAR blue , int color_depth) {
   r = red;
   g = green;
   b = blue;
   COLORDEPTH cdi = ColorDepthIndex(color_depth);
   ASSERT(cdi != NUMCOLORDEPTHS);
   cd = color_depth;
   cd_index = cdi;
   RefreshColor();
}


void Color::SetRGBA(UCHAR red , UCHAR green , UCHAR blue , UCHAR alpha) {
   r = red;
   g = green;
   b = blue;
   a = alpha;
   RefreshColor();
}



void Color::SetRGBAex(UCHAR red , UCHAR green , UCHAR blue , UCHAR alpha , int color_depth) {
   r = red;
   g = green;
   b = blue;
   a = alpha;
   COLORDEPTH cdi = ColorDepthIndex(color_depth);
   ASSERT(cdi != NUMCOLORDEPTHS);
   cd = color_depth;
   cd_index = cdi;
   RefreshColor();
}



void Color::SetHSV(float hue , float sat , float val) {
   int red,green,blue;
   hsv_to_rgb(hue,sat,val,&red,&green,&blue);
   r = red;
   g = green;
   b = blue;
   RefreshColor();
}



void Color::SetColorDepth(int color_depth) {
   COLORDEPTH cdi = ColorDepthIndex(color_depth);
   ASSERT(cdi != NUMCOLORDEPTHS);
   SetColorDepthIndex(cdi);
}



void Color::SetColorDepthIndex(COLORDEPTH cdi) {
   ASSERT(cdi != NUMCOLORDEPTHS);
   cd_index = cdi;
   RefreshColor();
}



void Color::ShiftHue(float hue_shift) {
   float h,s,v;
   int red,green,blue;
   rgb_to_hsv(r,g,b,&h,&s,&v);
   h += hue_shift;
   while (h < 0.0f) {h += 360.0f;}
   while (h >= 360.0f) {h -= 360.0f;}
   hsv_to_rgb(h,s,v,&red,&green,&blue);
   r = red;
   g = green;
   b = blue;
   RefreshColor();
}



void Color::Tint(UCHAR red , UCHAR green , UCHAR blue , UCHAR amt) {
   r += ((red-r)*amt)/255;
   g += ((green - g)*amt)/255;
   b += ((blue - b)*amt)/255;
   RefreshColor();
}



void Color::Invert() {
   r = 255 - r;
   g = 255 - g;
   b = 255 - b;
   RefreshColor();
}



void Color::SetAlpha(UCHAR alpha) {
   a = alpha;
   RefreshColor();
}



ostream& Color::PrintTo(ostream& os) const {
   Indenter indent;
   PrintTo(os,indent);
   return os;
}



ostream& Color::PrintTo(ostream& os , Indenter& indent) const {
//   using std::endl;
   os << indent << "Color (" << this << ") : (r,g,b = " << (int)r << "," << (int)g << "," << (int)b << "), (a = " << (int)a << "), ";
   os << "(color = " << color << " , cd = " << cd << ")";
   return os;
}



ostream& operator<<(ostream& os , const Color& col) {
   col.PrintTo(os);
   return os;
}



void ColorToHSV(const Color& c , float* h , float* s , float* v) {
   rgb_to_hsv(c.R() , c.G() , c.B() , h , s , v);
}



int AverageColor(BITMAP* bmp , float xpos , float ypos , float width , float height , COLORDEPTH dest_cdi) {
   ASSERT(bmp);
   const GPX_FUNC gpx = bmp->vtable->getpixel;
   COLORDEPTH cdi = ColorDepthIndex(bitmap_color_depth(bmp));
   const GETR_FUNC grfunc = getr_functions[cdi];
   const GETG_FUNC ggfunc = getg_functions[cdi];
   const GETB_FUNC gbfunc = getb_functions[cdi];
   const MKCOL_FUNC mkcol = makecol_functions[dest_cdi];
   return AverageColor(bmp , xpos , ypos , width , height , dest_cdi ,
                       gpx , cdi , grfunc , ggfunc , gbfunc , mkcol);
}



int AverageColor(BITMAP* bmp , float xpos , float ypos , float width , float height , COLORDEPTH dest_cdi,
                 const GPX_FUNC gpx , COLORDEPTH cdi , const GETR_FUNC grfunc , const GETG_FUNC ggfunc , const GETB_FUNC gbfunc,
                 const MKCOL_FUNC mkcol) {
   ASSERT(bmp);
   if (xpos < 0.0f) {
      width += xpos;
      xpos = 0.0f;
   }
   if (ypos < 0.0f) {
      height += ypos;
      ypos = 0.0f;
   }
   if (xpos + width > (float)bmp->w) {width = (float)bmp->w - xpos;}
   if (ypos + height > (float)bmp->h) {height = (float)bmp->h - ypos;}
   ASSERT(width > 0.0f);
   ASSERT(height > 0.0f);
   
   float xpercent,ypercent,percent;
   float numpixels = 0.0;
   float nextx,nexty;
   int col;
   // totals for color components
   float tr = 0.0;
   float tg = 0.0;
   float tb = 0.0;
   float ta = 0.0;
   int r = 0;
   int g = 0;
   int b = 0;
   int a = 0;
   const float ystop = ypos + height;
   const float xstop = xpos + width;
/*
   const GPX_FUNC gpx = bmp->vtable->getpixel;
   COLORDEPTH cdi = ColorDepthIndex(bitmap_color_depth(bmp));
   const GETR_FUNC grfunc = getr_functions[cdi];
   const GETG_FUNC ggfunc = getg_functions[cdi];
   const GETB_FUNC gbfunc = getb_functions[cdi];
   const MKCOL_FUNC mkcol = makecol_functions[dest_cdi];
*/
   for (float dy = ypos ; dy < ystop ; ) {
      nexty = (float)((int)dy + 1);
      if (nexty > ystop) {nexty = ystop;}
      ypercent = nexty - dy;
      for (float dx = xpos ; dx < xstop ;) {
         nextx = (float)((int)dx + 1);
         if (nextx > xstop) {nextx = xstop;}
         xpercent = nextx - dx;
         
         percent = xpercent*ypercent;
         col = gpx(bmp , (int)dx , (int)dy);
         r = grfunc(col);
         g = ggfunc(col);
         b = gbfunc(col);
         a = geta32(col);
         tr += percent*(float)r;
         tg += percent*(float)g;
         tb += percent*(float)b;
         ta += percent*(float)a;
         numpixels += percent;
         
         dx = nextx;
      }
      dy = nexty;
   }
   
   r = (int)(tr/numpixels + 0.5);
   g = (int)(tg/numpixels + 0.5);
   b = (int)(tb/numpixels + 0.5);
   a = (int)(ta/numpixels + 0.5);
   
   if ((dest_cdi == BPP32) && (cdi == BPP32)) {
      col = makeacol(r,g,b,a);
   } else {
      col = mkcol(r,g,b);
   }
   
   return col;
}



int MaskedAverageColor(BITMAP* bmp , float xpos , float ypos , float width , float height , COLORDEPTH dest_cdi) {
   ASSERT(bmp);
   if (xpos < 0.0f) {
      width += xpos;
      xpos = 0.0f;
   }
   if (ypos < 0.0f) {
      height += ypos;
      ypos = 0.0f;
   }
   if (xpos + width > (float)bmp->w) {width = (float)bmp->w - xpos;}
   if (ypos + height > (float)bmp->h) {height = (float)bmp->h - ypos;}
   ASSERT(width > 0.0f);
   ASSERT(height > 0.0f);
   
   float xpercent,ypercent,percent;
   float numpixels = 0.0;
   float nextx,nexty;
   int col;
   const int maskcol = bitmap_mask_color(bmp);
   // totals for color components
   float tr = 0.0;
   float tg = 0.0;
   float tb = 0.0;
   float ta = 0.0;
   int r = 0;
   int g = 0;
   int b = 0;
   int a = 0;
   const float ystop = ypos + height;
   const float xstop = xpos + width;

   const GPX_FUNC gpx = bmp->vtable->getpixel;
   COLORDEPTH cdi = ColorDepthIndex(bitmap_color_depth(bmp));
   const GETR_FUNC grfunc = getr_functions[cdi];
   const GETG_FUNC ggfunc = getg_functions[cdi];
   const GETB_FUNC gbfunc = getb_functions[cdi];
   const MKCOL_FUNC mkcol = makecol_functions[dest_cdi];

   bool all_masked = true;

   for (float dy = ypos ; dy < ystop ; ) {
      nexty = (float)((int)dy + 1);
      if (nexty > ystop) {nexty = ystop;}
      ypercent = nexty - dy;
      for (float dx = xpos ; dx < xstop ;) {
         nextx = (float)((int)dx + 1);
         if (nextx > xstop) {nextx = xstop;}
         xpercent = nextx - dx;
         
         percent = xpercent*ypercent;
         col = gpx(bmp , (int)dx , (int)dy);
         if (col != maskcol) {
            all_masked = false;
            r = grfunc(col);
            g = ggfunc(col);
            b = gbfunc(col);
            a = geta32(col);
            tr += percent*(float)r;
            tg += percent*(float)g;
            tb += percent*(float)b;
            ta += percent*(float)a;
            numpixels += percent;
         }
//         numpixels += percent;
         
         dx = nextx;
      }
      dy = nexty;
   }
   
   r = (int)(tr/numpixels + 0.5f);
   g = (int)(tg/numpixels + 0.5f);
   b = (int)(tb/numpixels + 0.5f);
   a = (int)(ta/numpixels + 0.5f);
   
   if (dest_cdi == BPP32) {
      col = makeacol(r,g,b,a);
   } else {
      col = mkcol(r,g,b);
   }
   
   if (all_masked) {
      return MaskColor(ColorDepth(dest_cdi));
   }
   
   return col;
}




