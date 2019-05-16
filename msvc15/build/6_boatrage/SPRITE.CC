//SPRITE.CC
#include "boatrage.h"
#include "sprite.h"

sprite *sprite::pfirst=NULL; //pekare till den f”rsta spriten
int sprite::spritecount=0;
//skapa sprite

sprite::sprite(fixed newx, fixed newy) {
 x=newx; y=newy;
 type=0;
 size=1;
 tobedeleted=0;
 spritecount++;
 //pnext=pfirst;
 //pfirst=this;
 sprite *s;
 if (pfirst) {
  for (s=pfirst; s->pnext; s=s->pnext) {
  }
  s->pnext=this;
  pnext=NULL;
 } else {
  pfirst=this;
  pnext=NULL;
 }
}

//d”da sprite
sprite::~sprite() {
 spritecount--;
 if (tobedeleted) return;
 sprite *s;
 if (pfirst==this) {
  pfirst=pnext;
 } else {
  for (s=pfirst; s; s=s->pnext) {
   if (s->pnext==this) {
    s->pnext=pnext;
    break;
   }
  }
 }
}

/*
sprite *delsprite(sprite *s, int i) {
 sprite *sp;
 if (s->valid!=1234) {cerr << "Sprite #" << i << endl; return s->pnext;}
 sp=s->pnext;
 delete s;
 return sp;
} */

//flytta allt krafs
void sprite::moveall() {
 int i=0;
 sprite *s,*sn;
 for (s=pfirst; s; s=s->pnext) {
  s->move();
 }
 sn=pfirst;
 for (s=pfirst; s; s=s->pnext) {
  i++;
  if (s->tobedeleted==true) {
   s->tobedeleted=false;
   sn->pnext=s->pnext;
   delete s;
   s=sn->pnext;
  }
  sn=s;
  if (sn==0) break;
 }
}

//kolla om den har krockat
int sprite::collided(sprite *sp) {
 fixed dist;
 dist=fsqrt(fmul(sp->x-x,sp->x-x)+fmul(sp->y-y,sp->y-y));
 if (dist<(sp->size+size)<<12)
  {return 1;}
 else
  {return 0;}
}

void sprite::killall() {
 //s† l„nge n†gon finns kvar...
 while (pfirst>NULL) {
  //ta bort den
  delete pfirst;
 }
}

//jag har problem med dessa: anv„nder dom inte
/*void sprite::sendtoback() {
 if (pfirst==this) return;
 sprite *sp;
 for (sp=pfirst; (sp->pnext!=this) && (sp); sp=sp->pnext);
 sp->pnext=pnext;
 pnext=pfirst;
 pfirst=this;
}

void sprite::bringtofront() {
 sprite *sp;
 for (sp=pfirst; (sp->pnext!=this) && (sp); sp=sp->pnext);
 sp->pnext=pnext;
 for (; sp; sp=sp->pnext);
 sp->pnext=this;
 pnext=NULL;
} */

sprite *sprite::findat(fixed x,fixed y,int thetype) {
 sprite *s;
 int flag=0;
 for (s=sprite::pfirst; s; s=s->pnext) {
  if (s->hit(x,y) && s->type==thetype) {flag=1;break;}
 }
 if (flag==1) return s; else return 0;
}
