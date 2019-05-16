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


#ifndef Drawing_H
#define Drawing_H

#include <allegro.h>

/// A no gap circle, a ring , and a gradient ring function
void do_circle_nogap (BITMAP* bmp , int x , int y , int radius , int color , void (*proc) (BITMAP* , int , int , int));

void ring(BITMAP* bmp , int x , int y , int inner_radius , int outer_radius , int color);

void ring_gradient(BITMAP* bmp , int cx , int cy , int inner_radius , int outer_radius , int inner_color , int outer_color);

/// Filled gradient rectangles
void rect_gradient_LtoR(BITMAP* bmp , int x , int y , int w , int h , int lcolor , int rcolor);
void rect_gradient_RtoL(BITMAP* bmp , int x , int y , int w , int h , int rcolor , int lcolor);

void rect_gradient_TtoB(BITMAP* bmp , int x , int y , int w , int h , int tcolor , int bcolor);
void rect_gradient_BtoT(BITMAP* bmp , int x , int y , int w , int h , int bcolor , int tcolor);

void rect_gradient_OtoM(BITMAP* bmp , int x , int y , int w , int h , int ocolor , int mcolor);
void rect_gradient_MtoO(BITMAP* bmp , int x , int y , int w , int h , int mcolor , int ocolor);

/// Rectangular gradient frame
void rect_gradient_frame(BITMAP* bmp , int x , int y , int w , int h , int num_frames , int start_color , int finish_color);

void triangle_outline(BITMAP* bmp , int x1 , int y1 , int x2 , int y2 , int x3 , int y3 , int color);





#endif // Drawing_H



