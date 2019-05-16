#include "boatrage.h"

bullet::bullet(fixed nx, fixed ny, fixed ndx, fixed ndy,
               fixed nangle, int ntype) : sprite(nx,ny) {
 dx=ndx;
 dy=ndy;
 angle=nangle;
}

void bullet::draw(BITMAP *bmp,fixed sx,fixed sy) {
 int nx,ny; BITMAP *pic=(BITMAP *)d(BULLET000);
 nx=(x-sx)>>12;
 ny=(y-sy)>>12;
 draw_trans_sprite(bmp,pic,nx-(pic->w>>1),ny-(pic->h>>1));
}

void bullet::move() {
 x+=dx;
 y+=dy;
 if (x<0 || y<0 || x>map->getfixwidth() || y>map->getfixheight()) {
  delete this;
  return;
 }
}

