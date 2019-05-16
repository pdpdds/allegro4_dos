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


#ifndef Blitting_H
#define Blitting_H


#include <allegro.h>




enum STRETCH_TYPE {
   USE_ALLEGRO_STRETCH = 0,// only uses colors from the source bitmap
   USE_SMOOTH_STRETCH  = 1 // blends colors from the source bitmap
};



int Clip(BITMAP* src , BITMAP* dest , int& srcx , int& srcy , int& destx , int& desty , int& w , int& h);// returns 1 if fully clipped, 0 if not



void tint_bitmap(BITMAP* bmp , int tint_color , int n);// n [0,255] , n=0 is no tint , n=255 is full_tint
void set_hue(BITMAP* bmp , float hue);
void invert_rgb(BITMAP* bmp);
void invert_value(BITMAP* bmp);
///void invert_lightness(BITMAP* bmp);/// TODO : Implement
void convert_to_alpha(BITMAP* bmp);
void blur(BITMAP* bmp , int radius);

void masked_tint_bitmap(BITMAP* bmp , int tint_color , int n);// n [0,255] , n=0 is no tint , n=255 is full_tint
void masked_set_hue(BITMAP* bmp , float hue);
void masked_invert_rgb(BITMAP* bmp);
void masked_invert_value(BITMAP* bmp);
///void masked_invert_lightness(BITMAP* bmp);/// TODO : Implement

void tint_blit(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h , int tint_color , int n);
void set_hue_blit(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h , float hue);
void invert_rgb_blit(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h);
void invert_value_blit(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h);
///void invert_lightness_blit(BITMAP* src , BITMAP* dest , int src , int srcy , int destx , int desty , int w , int h);/// TODO : Implement
void blur_blit(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h , int radius);

// stretch_blit_smooth blends colors from the source bitmap - not suitable for stretching a masked bitmap (hot pink outline)
// perfect for shrinking a detailed bitmap though, as the details will be preserved where with stretch_blit they would be lost
void stretch_blit_smooth(BITMAP* src , BITMAP* dest , int srcx , int srcy , int srcw , int srch , int destx , int desty , int destw , int desth);

void masked_tint_blit(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h , int tint_color , int n);
void masked_set_hue_blit(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h , float hue);
void masked_invert_rgb_blit(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h);
void masked_invert_value_blit(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h);
///void masked_invert_lightness_blit(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h);/// TODO : Implement

// masked_stretch_blit_smooth blends non mask colors from the source bitmap and tends to make sprites grow - not sure how to fix that
void masked_stretch_blit_smooth(BITMAP* src , BITMAP* dest , int srcx , int srcy , int srcw , int srch , int destx , int desty , int destw , int desth);



void blit_mask(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h);

void convert_to_alpha_blit(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h);

void trans_blit(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h);

void trans_layer_blit(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h);
void trans_layer_blit_safe(BITMAP* src , BITMAP* dest , int srcx , int srcy , int destx , int desty , int w , int h);



#endif // Blitting_H




