#include "boatrage.h"
#include "wrappers.h"
#include "sprite.h"
#include "player.h"
#include "control.h"
#include "objects.h"
#include "tilemap.h"

boat::boat(player &p,boatcontroller &c) : sprite(0,0) {
 type=SP_BOAT;
 size=8;
 pic=(BITMAP *)d(boattypes[p.boat].picid);

 dx=0;
 dy=0;
 pl=&p;
 controller=&c;
 lap=1;
 checkpoint=0;
 finished=0;
 engine=(SAMPLE *)d(SND_ENGINE00+pl->boat);
 Sound.play(engine,200,128,1000,1);

 int r=pl->rudder,e=pl->engine; // a=pl->armor,
 turnspeed=int(boattypes[pl->boat].turnspeed*rudders[r].turnmul*engines[e].turnmul);
 acc      =int(boattypes[pl->boat].acc *engines[e].accmul*rudders[r].accmul);
 skid     =int(boattypes[pl->boat].skid*rudders[r].skidmul*engines[e].skidmul);
// armor    =int(boattypes[pl->boat].armor*rudders[r].armmul*armors[a].armmul*engines[a].armmul);
}

boat::~boat() {
 Sound.stop(engine);
}

void boat::setcoords(boatcoord &bc) {
 x=bc.x;
 y=bc.y;
 angle=bc.a;
}

void boat::move() {
 #define ir(i) (i&0xffff0000) //avrunda ner till n„rmaste heltal
 #define iru(i) (((i>>16)+1)<<16) //avrunda upp till n„rmaste heltal
 #define ri(i) ((i&0xffff))  //plocka "decimalerna"

 if (countdown>0) return;
 static int lastx,lasty;
 lastx=x;
 lasty=y;
 x+=dx;
 y+=dy;
 int sampspeed=int(1000+20000.0*fixtof(fsqrt(fmul(dx,dx)+fmul(dy,dy))));
 if (sampspeed>10000) sampspeed=10000;
 Sound.adjust(engine,170,128,sampspeed,1);

 dx=fdiv(dx,skid);
 dy=fdiv(dy,skid);

 int thistile,act;
 //om den „r utanf”r kartan s„tt tillbaks den
 if (x<0) {x=0; dx=(int)(dx*-0.5);}
 if (y<0) {y=0; dy=(int)(dy*-0.5);}
 if (x>map->getfixwidth()-1) {x=map->getfixwidth()-1; dx=(int)(dx*-0.5);}
 if (y>map->getfixheight()-1) {y=map->getfixheight()-1; dy=(int)(dy*-0.5);}

 //kolla om den har k”rt upp p† land eller n†t s†nt
 thistile=map->get(x>>16,y>>16,0);
 act=tiletypes[thistile].action;
 switch (act) {
  case A_BLOCK:
   if (dx>0 && ir(lastx) < ir(x)) {
    dx=(int)(dy*-0.5);
    x=ir(x)-1; //ha bara kvar heltalsdelen
   }
   if (dy>0 && ir(lasty) < ir(y)) {
    dy=(int)(dy*-0.5);
    y=ir(y)-1;
   }
   if (dx<0 && ir(lastx) > ir(x)) {
    dx=(int)(dy*-0.5);
    x=iru(x);
   }
   if (dy<0 && ir(lasty) > ir(y)) {
    dy=(int)(dy*-0.5);
    y=iru(y);
   }
   break;
  case A_CP1:
  case A_CP2:
  case A_CP3:
   if (checkpoint==act-A_CP1) checkpoint++;
   break;
  case A_GOAL:
   if (checkpoint==3) {
    lap++;
    checkpoint=0;
   }
   break;
 }
 if (finished) return;

 if (lap>map->getlaps()) {
  finished=true;
  position++;
  pl->lastposition=position;
 }

 thistile=map->get(x>>16,y>>16,1);
 act=tiletypes[thistile].action;
 switch (act) {
  case A_SPLEFT:
   dx-=ftofix(0.01); break;
  case A_SPUP:
   dy-=ftofix(0.01); break;
  case A_SPRIGHT:
   dx+=ftofix(0.01); break;
  case A_SPDOWN:
   dy+=ftofix(0.01); break;
 }
 int accing=0;
 controller->check();
 if (controller->keyup()) {
  dx+=fmul(fsin(angle)>>4,acc);
  dy-=fmul(fcos(angle)>>4,acc);
  accing=1;
 }
 if (controller->keyleft()) {
  angle-=turnspeed;
 }
 if (controller->keyright()) {
  angle+=turnspeed;
 }
}

void boat::draw(BITMAP *bmp,fixed sx,fixed sy) {
 int nx,ny,i;
 nx=(x-sx)>>12;
 ny=(y-sy)>>12;
 rotate_sprite(bmp,pic,nx-(pic->w>>1),ny-(pic->w>>1),angle);
 fixed a,dist,tmpx,tmpy;
 int tmpspeed;
 tmpspeed=fixtoi(fsqrt(fmul(dx,dx)+fmul(dy,dy))*70);
 if (tmpspeed<0) tmpspeed=-tmpspeed;
 tmpspeed++;
 for (i=0; i<20; i++) {
  a=itofix(fixtoi(angle)+rnd(20)-10+128);
  dist=rnd(tmpspeed)+7;
  tmpx=fixtoi(fsin(a)*dist);
  tmpy=fixtoi(-fcos(a)*dist);
  putpixel(bmp,nx+tmpx,ny+tmpy,makecol(0,rnd(128),rnd(255)));
 }
}

void boat::drawinfo(BITMAP *bmp) {
 clear(bmp);
 pl->drawinfo(bmp);
 if (finished) {
  textprintf(bmp,(FONT *)d(FONT_RAVE),3,15,-1,S_GOAL);
 } else {
  textprintf(bmp,(FONT *)d(FONT_NORMAL),1,12,-1,S_CHECKPOINT,checkpoint);
  textprintf(bmp,(FONT *)d(FONT_NORMAL),1,23,-1,S_LAP,lap,map->getlaps());
 }
}

editcursor::editcursor(fixed x, fixed y) : sprite(x,y) {
 pic=(RLE_SPRITE *)d(CURSOR);
 curtile=1;
 type=SP_EDIT;
}

void editcursor::move() {
 int mickx,micky;
 get_mouse_mickeys(&mickx,&micky);
 x+=mickx<<12;
 y+=micky<<12;

 if (x<0) {x=0;}
 if (y<0) {y=0;}
 if (x>map->getfixwidth()-1)  x=map->getfixwidth()-1;
 if (y>map->getfixheight()-1) y=map->getfixheight()-1;
}

void editcursor::draw(BITMAP *bmp,fixed sx,fixed sy) {
 int nx,ny;
 nx=(x-sx)>>12;
 ny=(y-sy)>>12;
 draw_trans_rle_sprite_center(bmp,pic,nx,ny);
}
/* // JAG HAR SLŽNGT VAPENFUNKTIONERNA. DE PASSADE INTE IN I SPELBALANSEN...
bullet::bullet(boat *en,fixed nx, fixed ny, fixed ndx, fixed ndy,
               fixed nangle, int ntype) : sprite(nx,ny) {
 type=SP_BULLET;
 enemy=en;
 dx=ndx;
 dy=ndy;
 angle=nangle;
 btype=ntype;
 size=2;
}

void bullet::draw(BITMAP *bmp,fixed sx,fixed sy) {
 int nx,ny; BITMAP *pic=(BITMAP *)d(LIGHT008);
 nx=(x-sx)>>12;
 ny=(y-sy)>>12;
 draw_trans_sprite_center(bmp,pic,nx,ny);
}

void bullet::move() {
 x+=dx;
 y+=dy;
 if (x<0 || y<0 || x>map->getfixwidth() || y>map->getfixheight()) {
  tobedeleted=true;
  return;
 }
 if (collided(enemy)) {
  enemy->damage(4000);
  new explosion(x,y,6,1);
  tobedeleted=true;
  return;
 }
}
*/
light::light(fixed x, fixed y, int _color, int _type, int _p1, int _p2) : sprite(x,y) {
 size=0;
 color=_color;
 ltype=_type;
 type=SP_LIGHT;
 p1=_p1; p2=_p2;
}

light::light(PACKFILE *f) : sprite(0,0) {
 type=SP_LIGHT;
 size=0;
 readfromfile(f);
}

void light::draw(BITMAP *bmp,fixed sx, fixed sy) {
 int nx,ny;
 nx=(x-sx)>>12;
 ny=(y-sy)>>12;
 switch (ltype) {
  case LIGHT_SMALL:
   draw_trans_rle_sprite_center(bmp,(RLE_SPRITE *)d(LIGHT007+color),nx,ny);
   break;
  case LIGHT_BIG:
   draw_trans_rle_sprite_center(bmp,(RLE_SPRITE *)d(LIGHT000+color),nx,ny);
   break;
  case LIGHT_LTHOUSE:
   drawing_mode(DRAW_MODE_TRANS,NULL,0,0);
   triangle(bmp,nx,ny,nx+fixtoi(fsin(itofix(p1-12))*30),
                      ny-fixtoi(fcos(itofix(p1-12))*30),
                      nx+fixtoi(fsin(itofix(p1+12))*30),
                      ny-fixtoi(fcos(itofix(p1+12))*30),(color+1)*16+8);
   drawing_mode(DRAW_MODE_SOLID,NULL,0,0);
   break;
 }
}

void light::writetofile(PACKFILE *f) {
 pack_mputl(x,f);
 pack_mputl(y,f);
 pack_mputl(ltype,f);
 pack_mputl(color,f);
 pack_mputl(p1,f);
 pack_mputl(p2,f);
}

void light::readfromfile(PACKFILE *f) {
 x=pack_mgetl(f);
 y=pack_mgetl(f);
 ltype=pack_mgetl(f);
 color=pack_mgetl(f);
 p1=pack_mgetl(f);
 p2=pack_mgetl(f);
}
/*
void explosion::move() {
 if (timeleft<=0) tobedeleted=true;
 timeleft--;
}

void explosion::draw(BITMAP *bmp,fixed sx,fixed sy) {
 int nx,ny,i;
 nx=(x-sx)>>12;
 ny=(y-sy)>>12;
 draw_trans_sprite_center(bmp,(BITMAP *)d(EXPLODE00),nx,ny);
}*/


