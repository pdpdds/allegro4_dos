
//PSXFADE.CC
//Public Domain (Freeware) av Henrik Rydgård
#include <allegro.h>
#include "psxfade.h"

//these are smooth PSX style fade routines
void screenman::fadeout(PALETTE pal, int speed=1) {
 int ci,count;
 PALETTE pal2;
 for (ci=0; ci<256; ci++) {
  pal2[ci]=pal[ci];
 }
 for (count=0; count<64; count++) {
  for (ci=0; ci<256; ci++) {
   if (pal2[ci].r>0) pal2[ci].r--;
   if (pal2[ci].g>0) pal2[ci].g--;
   if (pal2[ci].b>0) pal2[ci].b--;
  }
  if ((count % speed)==0) set_palette(pal2);
 }
 set_palette(black_palette);
}

void screenman::fadein(PALETTE pal, int speed=1) {
 int ci, count;
 PALETTE pal2,pal3;
 for (count=0; count<256; count++) {
  pal2[count].r=pal[count].r;
  pal2[count].g=pal[count].g;
  pal2[count].b=pal[count].b;
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
 set_palette(pal);
}
