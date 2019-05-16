//view.cc
#include "boatrage.h"
#include "sprite.h"
#include "player.h"
#include "control.h"
#include "objects.h"
#include "tilemap.h"
#include "view.h"

void view::draw() {

 fixed x,y,
       xoff=-(sx&0xffff),
       yoff=-(sy&0xffff),
       scrx=xoff>>12,
       scry=yoff>>12,
       sx2=sx>>16,
       sy2=sy>>16,
       bmpw=bmp->w<<12,
       bmph=bmp->h<<12;

 int   w=(bmp->w>>4)+2,
       h=(bmp->h>>4)+2;
 int tmp;
 tiletype *tileset=tiletypes;

 for (y=sy2; y<sy2+h; y++) {
  for (x=sx2; x<sx2+w; x++) {
   if ((x<map->getwidth()) && (y<map->getheight()) && (x>=0) && (y>=0)) {
    draw_rle_sprite(bmp,tileset[map->get(x,y,0)].bmp,scrx,scry);
    tmp=map->get(x,y,1);
    if (tmp>=0 && tmp<MAXTILES) draw_rle_sprite(bmp,tileset[tmp].bmp,scrx,scry);
   } else {
    rectfill(bmp,scrx,scry,scrx+15,scry+15,makecol(0,0,0));
   }
   scrx+=16;
  }
  scrx=xoff>>12;
  scry+=16;
 }


 sprite *s;
 for (s=sprite::pfirst; s; s=s->pnext) {
  if ((s->x>sx-(2<<16)) &&
      (s->y>sy-(2<<16)) &&
      (s->x<sx+bmpw+(2<<16)) &&
      (s->y<sy+bmph+(2<<16))) s->draw(bmp,sx,sy);
 }

}

void view::scrollto(fixed x,fixed y) {
 sx=x-(bmp->w<<11);
 sy=y-(bmp->h<<11);
 int tmp;
 tmp=itofix(map->getwidth()-(bmp->w>>4));
 if (sx>tmp) sx=tmp;
 tmp=itofix(map->getheight()-(bmp->h>>4));
 if (sy>tmp) sy=tmp;
 if (sx<0) sx=0;
 if (sy<0) sy=0;
}

void view::scrollx(fixed x) {
 scrollto(x,sy);
}

void view::scrolly(fixed y) {
 scrollto(sx,y);
}

void view::follow(sprite *sp) {
 scrollto(sp->getX(),sp->getY());
}

void view::followboat(boat *b) {
// fixed cx,cy;//,cx2,cy2;
 fixed destx,desty;
 destx=b->getX();//+b->dx*16;//fsin(b->angle)*2;
 desty=b->getY();//+b->dy*16;//fcos(b->angle)*2;
 //sx+=(bmp->w<<11);
 //sy+=(bmp->h<<11);
 //sx-=(sx-destx)/8;
 //sy-=(sy-desty)/8;
 scrollto(destx,desty);
}
