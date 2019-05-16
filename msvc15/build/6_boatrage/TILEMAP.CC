#include "boatrage.h"
#include "sprite.h"
#include "player.h"
#include "control.h"
#include "objects.h"
#include "tilemap.h"

//if a tile type with a certain action can be used in a certain layer
int permit[11][2]={
{1,0},
{1,0},
{0,1},
{1,0},
{1,0},
{1,0},
{1,0},
{0,1},
{0,1},
{0,1},
{0,1}};

tilemap *map; //the current map

//create a new map (used by the editor)
void tilemap::newmap(int w, int h) {
 width=w;
 height=h;
 closemap();
 _map=new int[width*height*2];
 strcpy(name,   "Bana X\0        ");
 strcpy(creator, "Mr. X\0         ");
 clear();
}

//load a map from a file
void tilemap::loadmap(char *filename,boatcoord &bc1,boatcoord &bc2) {
 PACKFILE *f; int n,i,t;
 //open the file
 f=pack_fopen(filename,"r");
 //read the width and height
 width=pack_mgetl(f);
 height=pack_mgetl(f);
 //to be safe, destroy the current map if there is one...
 closemap(0);
 //and allocate space for a new one.
 _map=new int[width*height*2];
 //clear the map (also to be safe, if the file's corrupt)
 clear();

 //read the map data into the newly created buffer
 pack_fread(_map,width*height*sizeof(int)*2,f);

 //read the position of the boats
 bc1.x=pack_mgetl(f);
 bc1.y=pack_mgetl(f);
 bc1.a=pack_mgetl(f);
 bc2.x=pack_mgetl(f);
 bc2.y=pack_mgetl(f);
 bc2.a=pack_mgetl(f);

 //read the number of sprites
 n=pack_mgetl(f);

 //then loop through 'em
 sprite *s;
 for (i=0; i<(n); i++) {
  t=pack_mgetl(f);
  switch (t) {
   case SP_LIGHT:
    //create a light
    s=new light(0,0,0,0,0,0);
    s->readfromfile(f);
    break;
   default:
    //no other kinds can be stored yet in the file
    break;
  }
 }
 //read the number of laps and check it to be safe
 laps=pack_mgetl(f);
 if (laps<=0 || laps>99) laps=5;
 //read the name and creator strings
 pack_fread(name,16,f);
 pack_fread(creator,16,f);
 //read the song #...
 song=pack_mgetl(f);
 //and check it to be safe
 if (song<0 || song>15) song=0;
 //Finally, close the file.
 pack_fclose(f);
}



//save the map to a file (used by the editor)
void tilemap::savemap(char *filename,boatcoord &bc1,boatcoord &bc2) {
 PACKFILE *f; sprite *s;
 //open the file
 f=pack_fopen(filename,"w");
 //write level width and height
 pack_mputl(width,f);
 pack_mputl(height,f);

 //write the map data
 pack_fwrite(_map,width*height*sizeof(int)*2,f);

 //where the boats are
 pack_mputl(bc1.x,f);
 pack_mputl(bc1.y,f);
 pack_mputl(bc1.a,f);
 pack_mputl(bc2.x,f);
 pack_mputl(bc2.y,f);
 pack_mputl(bc2.a,f);

 //the number of sprites
 pack_mputl(sprite::numsprites(),f);
 for (s=sprite::pfirst;s;s=s->pnext) {
  pack_mputl(s->type,f);
  s->writetofile(f);
 }
 //number of laps
 pack_mputl(laps,f);
 //name and creator
 pack_fwrite(name,16,f);
 pack_fwrite(creator,16,f);
 //which song to play
 pack_mputl(song,f);
 //close the file
 pack_fclose(f);
}

//destructor
tilemap::~tilemap() {
 closemap();
}

//free the map
void tilemap::closemap(int killsprites) {
 if (_map) {delete[] _map; _map=0;}
 if (killsprites) sprite::killall();
}


//clear the map
void tilemap::clear() {
 int i;
 for (i=0; i<width*height; i++) {
  _map[i]=0;
 }
 for (i=width*height; i<width*height*2; i++) {
  _map[i]=-1;
 }
}

//draw a map of the map :-)
void tilemap::drawminimap(BITMAP *bmp) {
 int x,y,tmp;
 for (y=0; y<height; y++) {
  for (x=0; x<width; x++) {
   tmp=get(x,y,0);
   if (tmp<0 || tmp>255) tmp=0; //f”r att vara s„ker
   putpixel(bmp,x,y,tiletypes[tmp].color);
  }
 }
}

//fix the edges of grass and sand, used by the editor
void tilemap::fixedges() {
 int x,y;
 int L,U,R,D,UL,UR,DL,DR,THIS;
 for (y=0; y<height; y++) {
  for (x=0; x<width;  x++) {
   if (x>0) L=get(x-1,y,0); else L=0;
   if (!(L==2 || L==31)) L=0;

   if (y>0) U=get(x,y-1,0); else U=0;
   if (!(U==2 || U==31)) U=0;

   if (x<width-1) R=get(x+1,y,0); else R=0;
   if (!(R==2 || R==31)) R=0;

   if (y<height-1) D=get(x,y+1,0); else D=0;
   if (!(D==2 || D==31)) D=0;

   if (x>0 && y>0) UL=get(x-1,y-1,0); else UL=0;
   if (!(UL==2 || UL==31)) UL=0;

   if (x<(width-1) && y>0) UR=get(x+1,y-1,0); else UR=0;
   if (!(UR==2 || UR==31)) UR=0;

   if (x>0 && y<(height-1))DL=get(x-1,y+1,0); else DL=0;
   if (!(DL==2 || DL==31)) DL=0;

   if (x<(width-1) && y<(height-1)) DR=get(x+1,y+1,0); else DR=0;
   if (!(DR==2 || DR==31)) DR=0;

   THIS=get(x,y,0);
   if (THIS>1) continue;
   //Rakt upp/ner/v„nster/h”ger
   if (L && !U && !R && !D)
    set(x,y,1,L+1);
   if (!L && U && !R && !D)
    set(x,y,1,U+2);
   if (!L && !U && R && !D)
    set(x,y,1,R+3);
   if (!L && !U && !R && D)
    set(x,y,1,D+4);
   //Tv†riktnings
   if (L && U && !R && !D)
    set(x,y,1,L+9);
   if (!L && U && R && !D)
    set(x,y,1,U+10);
   if (!L && !U && R && D)
    set(x,y,1,R+11);
   if (L && !U && !R && D)
    set(x,y,1,D+12);

   if (L && !U && R && !D)
    set(x,y,1,L+13);
   if (!L && U && !R && D)
    set(x,y,1,U+14);

   if (L && U && !R && D)
    set(x,y,1,L+15);
   if (L && U && R && !D)
    set(x,y,1,U+16);
   if (!L && U && R && D)
    set(x,y,1,R+17);
   if (L && !U && R && D)
    set(x,y,1,D+18);

   if (L && U && R && D)
    set(x,y,1,L+19);

    //h”rn
   if (UL && !UR && !DR && !DL && !L && !U && !R && !D)
    set(x,y,1,UL+5);
   if (!UL && UR && !DR && !DL && !L && !U && !R && !D)
    set(x,y,1,UR+6);
   if (!UL && !UR && DR && !DL && !L && !U && !R && !D)
    set(x,y,1,DR+7);
   if (!UL && !UR && !DR && DL && !L && !U && !R && !D)
    set(x,y,1,DL+8);

   if (UL && !UR && !DR && DL && !L && !U && !R && !D)
    set(x,y,1,DL+20);
   if (UL && UR && !DR && !DL && !L && !U && !R && !D)
    set(x,y,1,UL+21);
   if (!UL && UR && DR && !DL && !L && !U && !R && !D)
    set(x,y,1,UR+22);
   if (!UL && !UR && DR && DL && !L && !U && !R && !D)
    set(x,y,1,DR+23);

   if (UL && UR && !DR && DL && !L && !U && !R && !D)
    set(x,y,1,UL+24);
   if (UL && UR && DR && !DL && !L && !U && !R && !D)
    set(x,y,1,UR+25);
   if (!UL && UR && DR && DL && !L && !U && !R && !D)
    set(x,y,1,DR+26);
   if (UL && !UR && DR && DL && !L && !U && !R && !D)
    set(x,y,1,DL+27);

   if (UL && UR && DR && DL && !L && !U && !R && !D)
    set(x,y,1,UL+28);

  }
 }
}

//draw using different brush sizes
void tilemap::draw_using_brush(int x, int y, int layer, int tile, int brush) {
 //NOTE: No break, as the brushes get smaller and smaller
 switch (brush) {
  case 4:
   set(x-2,y-1,layer,tile);
   set(x-2,y,  layer,tile);
   set(x-2,y+1,layer,tile);

   set(x+2,y-1,layer,tile);
   set(x+2,y,  layer,tile);
   set(x+2,y+1,layer,tile);

   set(x-1,y-2,layer,tile);
   set(x,  y-2,layer,tile);
   set(x+1,y-2,layer,tile);

   set(x-1,y+2,layer,tile);
   set(x,  y+2,layer,tile);
   set(x+1,y+2,layer,tile);
  case 3:
   set(x-1,y-1,layer,tile);
   set(x+1,y-1,layer,tile);
   set(x-1,y+1,layer,tile);
   set(x+1,y+1,layer,tile);
  case 2:
   set(x-1,y,layer,tile);
   set(x+1,y,layer,tile);
   set(x,y-1,layer,tile);
   set(x,y+1,layer,tile);
  case 1:
   set(x,y,layer,tile);
 }
}
