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

#include "Eagle/Area.hpp"
#include "Eagle/AllegroUtilities.hpp"
#include "Eagle/Drawing.hpp"



//void do_bres_mpa_nogap_circle (BITMAP* bmp , int x , int y , int radius , int d , void (*proc) (BITMAP* , int , int , int)) {// old name
void do_circle_nogap (BITMAP* bmp , int x , int y , int radius , int color , void (*proc) (BITMAP* , int , int , int)) {
   /// A Bresenham Circle mid point algorithm that fills in the gaps between a circle of the current radius and 
   ///   between a circle of radius - 1 while drawing the current radius circle
   /// Based on allegro's do_circle and a PROC8 macro written by SiegeLord
   /// Written by Edgar Reynaldo after an explanation of Bresenham circles by SiegeLord
   ///   and inspired by his version of a gapless circle
   ASSERT(bmp);
   ASSERT(proc);
   
   if (radius < 1) {return;}
   
   int cx = 0;
   int cy = radius;
   int df = 1 - radius;// value of (x^2 + y^2 - radius^2) starting at (1,radius - 1/2)
   int d_e = 3;// change in df when moving right one pixel
   int d_se = -2*radius + 5;// change in df when moving down and right one pixel each
   
   /// Need to keep track of the inner radius plotting as well so gaps can be detected and filled
   const int iradius = radius - 1;
   int icx = 0;
   int icy = iradius;
   int idf = 1 - iradius;// value of (ix^2 + iy_2 - iradius^2) starting at (1 , iradius - 1/2)
   int id_e = 3;// change in idf when moving right one pixel
   int id_se = -2*iradius + 5;// change in idf when moving down and right one pixel each
   
   /// vertical distance between next c(x,y) and next ic(x,y)
   int vdst = 1;// equal to cy - icy = radius - (radius-1) = 1
   if (df >= 0) {vdst--;}// since the next c(x,y) is one lower than the current one
   if (idf >= 0) {vdst++;}// since the next ic(x,y) is one lower than the current one
   
#define PROC8                                \
proc(bmp , x + cx , y + cy , color);         \
if ((cx) && (cy)) {                          \
   proc(bmp , x - cx , y - cy , color);      \
   proc(bmp , x - cx , y + cy , color);      \
   proc(bmp , x + cx , y - cy , color);      \
} else {                                     \
   if (cx) {                                 \
      proc(bmp , x - cx , y + cy , color);   \
   }                                         \
   if (cy) {                                 \
      proc(bmp , x + cx , y - cy , color);   \
   }                                         \
}                                            \
if (cx != cy) {                              \
   proc(bmp , x + cy , y + cx , color);      \
   if ((cx) && (cy)) {                       \
      proc(bmp , x - cy , y - cx , color);   \
      proc(bmp , x + cy , y - cx , color);   \
      proc(bmp , x - cy , y + cx , color);   \
   } else {                                  \
      if (cx) {                              \
         proc(bmp , x + cy , y - cx , color);\
      }                                      \
      if (cy) {                              \
         proc(bmp , x - cy , y + cx , color);\
      }                                      \
   }                                         \
}
   do {
      PROC8
      
      if (df < 0) {
         df += d_e;
         d_e += 2;
         d_se += 2;
      } else {
         df += d_se;
         d_e += 2;
         d_se += 4;
         --cy;
         
         --vdst;
      }
      
      if (idf < 0) {
         idf += id_e;
         id_e += 2;
         id_se += 2;
      } else {
         idf += id_se;
         id_e += 2;
         id_se += 4;
         --icy;
         
         ++vdst;
      }
      

      ++cx;
      ++icx;

      if (vdst > 1){// gap between next cx,cy pixel and next icx,icy pixel
         if ((radius > 1) && (cx <= (cy - 1))) {
///            log << "Filling gap , vdst = " << vdst << " , c(x,y) = (" << cx << "," << cy << ") , ic(x,y) = (" << icx << "," << icy << ")" << endl;
            --cy;
            PROC8
            ++cy;
         }
      }

   } while (cx <= cy);
#undef PROC8
   
}



//void _soft_ring_hfill_bottom_up (BITMAP* bmp , int x , int y , int inner_radius , int outer_radius , int d) {// old name
void ring(BITMAP* bmp , int x , int y , int inner_radius , int outer_radius , int color) {
   
   ASSERT(bmp);
   ASSERT(inner_radius >= 0);
   ASSERT(outer_radius >= inner_radius);
   
   /// Perhaps gap filler routine should be run after the hfills are completed
   ///   it looks quite complex to track the gaps along 0 to PI/4 in strictly
   ///   horizontal distances (due to hfill)
   
   int icx = inner_radius;
   int icy = 0;
   int idf = 1 - inner_radius;
   int id_n = 3;
   int id_nw = -2*inner_radius + 5;
   int id_w;// = -2*inner_radius + 2;
   
   int cx = outer_radius;
   int cy = 0;
   int df = 1 - outer_radius;
   int d_n = 3;
   int d_nw = -2*outer_radius + 5;
   int d_w;// = -2*outer_radius + 2;
   
   void (*hfill_func_ptr) (BITMAP* , int , int , int , int) = bmp->vtable->hfill;
   
   if (icx == 0) {
      hfill_func_ptr(bmp , x - cx , y , x  + cx , color);
   } else {
      hfill_func_ptr(bmp , x + icx , y , x + cx , color);
      hfill_func_ptr(bmp , x - cx , y , x - icx , color);
   }
   
   ++icy;
   if (idf < 0) {
      idf += id_n;
      id_n += 2;
      id_nw += 2;
   } else {
      idf += id_nw;
      id_n += 2;
      id_nw += 4;
      --icx;
   }
   
   ++cy;
   if (df < 0) {
      df += d_n;
      d_n += 2;
      d_nw += 2;
   } else {
      df += d_nw;
      d_n += 2;
      d_nw += 4;
      --cx;
   }
   
   // draw , move , check
   
   while (icy < icx) {
      hfill_func_ptr(bmp , x + icx , y + icy , x + cx , color);
      hfill_func_ptr(bmp , x - cx , y + icy , x - icx , color);
      hfill_func_ptr(bmp , x + icx , y - icy , x + cx , color);
      hfill_func_ptr(bmp , x - cx , y - icy , x - icx , color);
      
      ++icy;
      if (idf < 0) {
         idf += id_n;
         id_n += 2;
         id_nw += 2;
      } else {
         idf += id_nw;
         id_n += 2;
         id_nw += 4;
         --icx;
      }
      
      ++cy;
      if (df < 0) {
         df += d_n;
         d_n += 2;
         d_nw += 2;
      } else {
         df += d_nw;
         d_n += 2;
         d_nw += 4;
         --cx;
      }
   }
   // need to recalculate deviation for points on inner circle as it now 
   //   moves (up-left or left) instead of (up or up-left)
   idf = (icx - 1)*(icx - 1) + icy*(icy + 1) - inner_radius*inner_radius;// 1/4 gets truncated
   id_nw = 2*(icy - icx) + 5;
   id_w = -2*icx + 3;
   
   while((icy < inner_radius) && (cy < cx)) {
      // move point on inner circle as far left as it will go before it would move up-left
      while(idf >= 0) {
         idf += id_w;
         id_w += 2;
         id_nw += 2;
         --icx;
      }
      hfill_func_ptr(bmp , x + icx , y + icy , x + cx , color);
      hfill_func_ptr(bmp , x - cx , y + icy , x - icx , color);
      hfill_func_ptr(bmp , x + icx , y - icy , x + cx , color);
      hfill_func_ptr(bmp , x - cx , y - icy , x - icx , color);
      
      ++icy;
      --icx;
      idf += id_nw;
      id_w += 2;
      id_nw += 4;
      
      ++cy;
      if (df < 0) {
         df += d_n;
         d_n += 2;
         d_nw += 2;
      } else {
         df += d_nw;
         d_n += 2;
         d_nw += 4;
         --cx;
      }
   }
   
   if (icy < inner_radius) {// stopped last while loop because cy >= cx , so direction test needs to be changed
      df = (cx - 1)*(cx - 1) + cy*(cy + 1) - (outer_radius*outer_radius);
      d_nw = 2*(cy - cx) + 5;
      d_w = -2*cx + 3;
      
      while (icy < inner_radius) {
         // move point on inner circle as far left as it will go before it would move up-left
         while (idf >= 0) {
            idf += id_w;
            id_w += 2;
            id_nw += 2;
            --icx;
         }
         
         hfill_func_ptr(bmp , x + icx , y + icy , x + cx , color);
         hfill_func_ptr(bmp , x - cx , y + icy , x - icx , color);
         hfill_func_ptr(bmp , x + icx , y - icy , x + cx , color);
         hfill_func_ptr(bmp , x - cx , y - icy , x - icx , color);
         
         // move point on outer circle as far left as it will go before it would move up-left
         while (df >= 0) {
            df += d_w;
            d_w += 2;
            d_nw += 2;
            --cx;
         }
         
         ++icy;
         --icx;
         idf += id_nw;
         id_w += 2;
         id_nw += 4;
         
         ++cy;
         --cx;
         df += d_nw;
         d_w += 2;
         d_nw += 4;
      }
   }
   
   if (cy < cx) {// stopped second to last while loop because icy >= inner_radius so now hfills are drawn all the way across
      while (cy < cx) {
         hfill_func_ptr(bmp , x - cx , y + cy , x + cx , color);
         hfill_func_ptr(bmp , x - cx , y - cy , x + cx , color);
         
         ++cy;
         if (df < 0) {
            df += d_n;
            d_n += 2;
            d_nw += 2;
         } else {
            df += d_nw;
            d_n += 2;
            d_nw += 4;
            --cx;
         }
      }
   }
   /// Now cy >= cx so direction test for outer circle needs to be changed
   df = (cx - 1)*(cx - 1) + cy*(cy + 1) - (outer_radius*outer_radius);
   d_nw = 2*(cy - cx) + 5;
   d_w = -2*cx + 3;
   
   /// Okay , now cy >= cx and cy >= inner_radius so hfill all the way across while cy < outer_radius
   while (cy < outer_radius) {
      hfill_func_ptr(bmp , x - cx , y + cy , x + cx , color);
      hfill_func_ptr(bmp , x - cx , y - cy , x + cx , color);
      
      // move point on outer circle as far left as it will go before it would move up-left
      while (df >= 0) {
         df += d_w;
         d_w += 2;
         d_nw += 2;
         --cx;
      }
      ++cy;
      --cx;
      df += d_nw;
      d_w += 2;
      d_nw += 4;
   }
   
   // cy is now equal to outer_radius
   hfill_func_ptr(bmp , x - cx , y + cy , x + cx , color);
   hfill_func_ptr(bmp , x - cx , y - cy , x + cx , color);
   
   /// Don't forget to fill the gaps
   if (inner_radius > 1) {
      void (*putpixelfunc) (BITMAP* , int , int , int) = bmp->vtable->putpixel;
      
      cx = inner_radius;
      cy = 0;
      df = 1 - inner_radius;
      d_n = 3;
      d_nw = -2*inner_radius + 5;
      
      icx = inner_radius - 1;
      icy = 0;
      idf = 2 - inner_radius;// 1 - (inner_radius - 1)
      id_n = 3;
      id_nw = -2*inner_radius + 7;// -2(inner_radius - 1) + 5
      
      int hdst = 1;// cx - icx;
      
      while (cy < cx) {
         if (hdst > 1) {
            --cx;
            putpixelfunc(bmp , x + cx , y + cy , color);
            putpixelfunc(bmp , x + cx , y - cy , color);
            putpixelfunc(bmp , x - cx , y + cy , color);
            putpixelfunc(bmp , x - cx , y - cy , color);
            if (cy != cx) {
               putpixelfunc(bmp , x + cy , y + cx , color);
               putpixelfunc(bmp , x + cy , y - cx , color);
               putpixelfunc(bmp , x - cy , y + cx , color);
               putpixelfunc(bmp , x - cy , y - cx , color);
            }
            ++cx;
         }
         ++cy;
         if (df < 0) {
            df += d_n;
            d_n += 2;
            d_nw += 2;
         } else {
            df += d_nw;
            d_n += 2;
            d_nw += 4;
            --cx;
            --hdst;
         }
         ++icy;
         if (idf < 0) {
            idf += id_n;
            id_n += 2;
            id_nw += 2;
         } else {
            idf += id_nw;
            id_n += 2;
            id_nw += 4;
            --icx;
            ++hdst;
         }
      }
//      if (cy == cx) {
//         putpixelfunc(
//      }
   }
   
}




//void _soft_gradient_ring(BITMAP* bmp , int cx , int cy , int inner_radius , int outer_radius , int inner_color , int outer_color) {// old name
void ring_gradient(BITMAP* bmp , int cx , int cy , int inner_radius , int outer_radius , int inner_color , int outer_color) {
   
   ASSERT(bmp);
   ASSERT(outer_radius >= inner_radius);
   ASSERT(inner_radius >= 0);
   
   if (inner_radius == outer_radius) {// prevent division by zero later
      do_circle_nogap(bmp , cx , cy , inner_radius , inner_color , bmp->vtable->putpixel);
      return;
   }
   
   const double ic_red = getr(inner_color);
   const double ic_green = getg(inner_color);
   const double ic_blue = getb(inner_color);
   
   
   const double oc_red = getr(outer_color);
   const double oc_green = getg(outer_color);
   const double oc_blue = getb(outer_color);
   
   const double num_steps = (double)(outer_radius - inner_radius);
   
   const double ic_to_oc_red_step = (oc_red - ic_red)/num_steps;
   const double ic_to_oc_green_step = (oc_green - ic_green)/num_steps;
   const double ic_to_oc_blue_step = (oc_blue - ic_blue)/num_steps;

   double r = ic_red;
   double g = ic_green;
   double b = ic_blue;
   int color;
   
   COLORDEPTH cdi = ColorDepthIndex(bitmap_color_depth(bmp));
   MKCOL_FUNC makecol_func_ptr = makecol_functions[cdi];
   PPX_FUNC putpixelfunc = bmp->vtable->putpixel;
   
   for (int radius = inner_radius ; radius <= outer_radius ; radius++) {
      color = makecol_func_ptr((int)r , (int)g , (int)b);
      do_circle_nogap(bmp , cx , cy , radius , color , putpixelfunc);
      r += ic_to_oc_red_step;
      g += ic_to_oc_green_step;
      b += ic_to_oc_blue_step;
   }
}



void rect_gradient_LtoR(BITMAP* bmp , int x , int y , int w , int h , int lcolor , int rcolor) {
   Rectangle r(x,y,w,h);
   r.DrawGradientLtoR(bmp , lcolor , rcolor);
}



void rect_gradient_RtoL(BITMAP* bmp , int x , int y , int w , int h , int rcolor , int lcolor) {
   Rectangle r(x,y,w,h);
   r.DrawGradientRtoL(bmp , rcolor , lcolor);
}



void rect_gradient_TtoB(BITMAP* bmp , int x , int y , int w , int h , int tcolor , int bcolor) {
   Rectangle r(x,y,w,h);
   r.DrawGradientTtoB(bmp , tcolor , bcolor);
}



void rect_gradient_BtoT(BITMAP* bmp , int x , int y , int w , int h , int bcolor , int tcolor) {
   Rectangle r(x,y,w,h);
   r.DrawGradientBtoT(bmp , bcolor , tcolor);
}



void rect_gradient_OtoM(BITMAP* bmp , int x , int y , int w , int h , int ocolor , int mcolor) {
   Rectangle r(x,y,w,h);
   r.DrawGradientOtoM(bmp , ocolor , mcolor);
}



void rect_gradient_MtoO(BITMAP* bmp , int x , int y , int w , int h , int mcolor , int ocolor) {
   Rectangle r(x,y,w,h);
   r.DrawGradientMtoO(bmp , mcolor , ocolor);
}



void rect_gradient_frame(BITMAP* bmp , int x , int y , int w , int h , int num_frames , int start_color , int finish_color) {
   Rectangle r(x,y,w,h);
   r.DrawGradientFrame(bmp , num_frames , start_color , finish_color);
}



void triangle_outline(BITMAP* bmp , int x1 , int y1 , int x2 , int y2 , int x3 , int y3 , int color) {
   line(bmp , x1 , y1 , x2 , y2 , color);
   line(bmp , x2 , y2 , x3 , y3 , color);
   line(bmp , x3 , y3 , x1 , y1 , color);
}






