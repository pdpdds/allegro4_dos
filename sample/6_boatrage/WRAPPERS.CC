//WRAPPERS.CC
#include "boatrage.h"
#include "wrappers.h"
screenman display; //tar hand om sk„rmen
musicplayer Music;
soundplayer Sound;

void draw_trans_sprite_center(BITMAP *bmp,BITMAP *pic,int x, int y) {
 draw_trans_sprite(bmp,pic,x-(pic->w>>1),y-(pic->h>>1));
}

void draw_trans_rle_sprite_center(BITMAP *bmp,RLE_SPRITE *pic,int x, int y) {
 draw_trans_rle_sprite(bmp,pic,x-(pic->w>>1),y-(pic->h>>1));
}

void rotate_sprite_center(BITMAP *bmp,BITMAP *pic,int x,int y,fixed a) {
 rotate_sprite(bmp,pic,x-(pic->w>>1),y-(pic->w>>1),a);
}

//these are smooth PSX style fade routines
void screenman::fadeout(int speed) {
 int ci,count;
 PALETTE pal2;
 for (ci=0; ci<256; ci++) {
  pal2[ci]=stdpal[ci];
 }
 for (count=0; count<64; count++) {
  for (ci=0; ci<256; ci++) {
   if (pal2[ci].r>0) pal2[ci].r--;
   if (pal2[ci].g>0) pal2[ci].g--;
   if (pal2[ci].b>0) pal2[ci].b--;
  }
  if ((count % speed)==0) set_palette(pal2);
 }
 goblack();
}

void screenman::fadein(int speed) {
 int ci, count;
 PALETTE pal2,pal3;
 for (count=0; count<256; count++) {
  pal2[count].r=stdpal[count].r;
  pal2[count].g=stdpal[count].g;
  pal2[count].b=stdpal[count].b;
  pal3[count].r=0;
  pal3[count].g=0;
  pal3[count].b=0;
 }
 for (count=0; count<64; count++) {
  for (ci=0; ci<256; ci++) {
   if (pal2[ci].r+count-64>0) pal3[ci].r++;
   if (pal2[ci].g+count-64>0) pal3[ci].g++;
   if (pal2[ci].b+count-64>0) pal3[ci].b++;
  }
  if ((count % speed)==0) set_palette(pal3);
 }
 setstdpal();
}
