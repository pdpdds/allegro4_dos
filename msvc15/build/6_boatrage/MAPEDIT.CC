//MAPEDIT.CC
#include "boatrage.h"
#include "wrappers.h"
#include "sprite.h"
#include "player.h"
#include "control.h"
#include "objects.h"
#include "tilemap.h"
#include "view.h"
#include "menu.h"
#include "mapedit.h"

boatcoord bc[2];

const char *strwidth="32";
const char *strheight="32";
char strlaps[3]="5";
char strsong[3]="1";
char strname[16]="X";
char strcreator[16]="Y";

char lcolnames[7][8]={
S_WHITE,
S_RED,
S_GREEN,
S_BLUE,
S_YELLOW,
S_MAGENTA,
S_CYAN,
};

char ltypenames[3][24]={
S_SMALL,
S_BIG,
S_ROTATING,
};

MENU filemenu[]={
{S_NEWLEVEL},
{S_LOADLEVEL},
{S_SAVELEVEL},
{S_PROPERTIES},
{S_FIXEDGES},
{""},
{S_CLOSEEDITOR},
{0,0,0,0,0}
};

DIALOG newlevel[] ={
//Typ          X   Y   W   H   FG     BG     Key    Flags    d1   d2   dp     dp2    dp3
{d_clear_proc, 0,  0,  0,  0,  255,   0,     0,     0,       0,   0},
{d_text_proc , 50, 80, 0,  0,  255,   0,     0,     0,       0,   0, S_HOWBIGLEVEL},
{d_text_proc , 70, 90, 0,  0,  255,   0,     0,     0,       0,   0, S_WIDTH},
{d_text_proc , 70, 110,0,  0,  255,   0,     0,     0,       0,   0, S_HEIGHT},
{d_edit_proc , 150,90, 32, 10, 255,   0,     0,     0,       sizeof(strwidth)-1,   0,  (void*)strwidth},
{d_edit_proc , 150,110,32, 10, 255,   0,     0,     0,       sizeof(strheight)-1,  0,  (void*)strheight},
{d_button_proc,255,90, 60, 12, 255,   0, KEY_ENTER, D_EXIT,  0,   0, S_OK},
{NULL,0,0,0,0,0,0,0,0,0,0,NULL,NULL,NULL}
};

DIALOG levelprops[] ={
//Typ          X   Y   W   H   FG     BG     Key    Flags    d1   d2   dp     dp2    dp3
{d_clear_proc, 0,  0,  0,  0,  255,   0,     0,     0,       0,   0},
{d_text_proc , 10, 80, 0,  0,  255,   0,     0,     0,       0,   0, S_LEVELPROPS},
{d_text_proc , 30, 90, 0,  0,  255,   0,     0,     0,       0,   0, S_NAME},
{d_text_proc , 30, 110,0,  0,  255,   0,     0,     0,       0,   0, S_BY},
{d_text_proc , 30, 130,0,  0,  255,   0,     0,     0,       0,   0, S_NUMLAPS},
{d_text_proc , 30, 150,0,  0,  255,   0,     0,     0,       0,   0, S_NUMSONG},
{d_edit_proc , 90,90, 100,10,  255,   0,     0,     0,       sizeof(strname)-1,   0,  strname},
{d_edit_proc , 90,110,100,10,  255,   0,     0,     0,       sizeof(strcreator)-1,     0,  strcreator},
{d_edit_proc , 90,130,32, 10,  255,   0,     0,     0,       sizeof(strlaps)-1,   0,  strlaps},
{d_edit_proc , 90,150,32, 10,  255,   0,     0,     0,       sizeof(strsong)-1,   0,  strsong},
{d_button_proc,200,90, 60, 12, 255,   0, KEY_ENTER, D_EXIT,  0,   0, S_OK},
{NULL,0,0,0,0,0,0,0,0,0,0,NULL,NULL,NULL}
};

void showprops() {
 int ok,laps,song;
 set_palette(desktop_palette);
 strcpy(strname,map->name);
 strcpy(strcreator,map->creator);
 laps=map->getlaps();
 song=map->getsong()+1;
 itoa(laps,(char *)levelprops[8].dp,10);
 itoa(song,(char *)levelprops[9].dp,10);
 do {
  do_dialog(levelprops,-1);
  laps=atoi((char *)levelprops[8].dp);
  if (laps<=0) {
   alert(S_ERRLAPS1,S_ERRLAPS2,NULL,S_OK,NULL,S_O,NULL);
   ok=false;
  } else {
   ok=true;
  }
  song=atoi((char *)levelprops[9].dp);
  if (song<=0 || song>16) {
   alert(S_ERRSONG1,S_ERRSONG2,NULL,S_OK,NULL,S_O,NULL);
   ok=false;
  } else {
   ok=true;
  }
 } while (!ok);
 strcpy(map->name,strname);
 strcpy(map->creator,strcreator);
 map->setlaps(laps);
 map->setsong(song-1);
 Music.playsong(song-1);
 display.setstdpal();
}

void newmap() {
 int ok,w,h;
 strwidth="32\0";
 strheight="32\0";
 set_palette(desktop_palette);
 do {
  do_dialog(newlevel, -1);
  w=atoi((char *)newlevel[4].dp);
  h=atoi((char *)newlevel[5].dp);
  if (w<=0 || h<=0) {
   alert(S_ERRWH1,S_ERRWH2,NULL,S_OK,NULL,S_O,NULL);
   ok=false;
  } else {
   ok=true;
  }
 } while (!ok);
 display.setstdpal();
 map->newmap(w,h);
 map->setsong(0);
 Music.playsong(0);
 map->setlaps(3);
 for (w=0; w<2; w++) {
  bc[w].x=bc[w].y=bc[w].a=0;
 }
}

int loadmap() {
 char filename[255];
 int ret;
 for (ret=0; ret<256; ret++) filename[ret]='\0';
 display.setstdpal();
 ret=file_select(S_LOADLEVEL,filename,"BOA");
 if (ret==0) return 0;
 if (!exists(filename)) {
  alert(S_FILEDONTEXIST,NULL,NULL,S_OK,NULL,S_O,NULL);
  return 0;
 }
 sprite::killall();
 map->loadmap(filename,bc[0],bc[1]);
 Music.playsong(map->getsong());
 return 1;
}

void savemap() {
 char filename[255];
 int ret;
 for (ret=0; ret<256; ret++) filename[ret]='\0';
 ret=file_select(S_SAVELEVEL,filename,"BOA");
 if (ret==0) return;
 map->savemap(filename,bc[0],bc[1]);
}

void fixedges() {
 map->fixedges();
}

void editmap() {
 #define RED makecol(240,0,0)
 #define BLUE makecol(0,64,255)
 #define GREEN makecol(0,192,0)

 BITMAP *viewbitmap=display.subbitmap(0,0,display.w(),display.h()-26);
 BITMAP *tools=display.subbitmap(0,display.h()-26,display.w(),26);
 FONT *nfont=(FONT *)d(FONT_SMALL);

 view *mapview=new view(viewbitmap); //de tv† vyerna
 editcursor *cur;
 int i,curtile=0,quit=0,a;
 int tool=0;
 int curx,cury;
 int lcolor=0,ltype=0;
 int tmpx,tmpy;
 int layer=0;
 int y;
 int brush=1;
 sprite *sp;

 install_int_ex(speeder,BPS_TO_TIMER(60));

 actual_cycle=0;
 target_cycle=0;
 //Huvudloopen
 display.setstdpal();
 cur=new editcursor(30,30);
 do {
  curx=cur->getX();
  cury=cur->getY();
  clear_to_color(tools,makecol(0,0,60));
  hline(tools,0,0,tools->w,makecol(0,0,192));
  hline(tools,0,1,tools->w,makecol(128,128,255));
  hline(tools,0,2,tools->w,makecol(0,0,128));

  if (key[KEY_PGUP]) {
   if (tool>0) tool--;
   key[KEY_PGUP]=0;
  }
  if (key[KEY_PGDN]) {
   if (tool<9) tool++;
   key[KEY_PGDN]=0;
  }
  textprintf(tools,nfont,1,10,BLUE,S_F1HELP);
  switch (tool) {
   case 0:
    if (curtile==-1) layer=1;
    textprintf(tools,nfont,1,1,RED,S_LAYER,tool+1,layer+1);
    textprintf(tools,nfont,40,10,RED,S_BRUSH,brush);
    if (mouse_b & 1) {
     map->draw_using_brush(curx>>16,cury>>16,layer,curtile,brush);
    }
    if (mouse_b & 2) {
     curtile=map->get(curx>>16,cury>>16,layer);
    }
    rectfill(tools,89,2,SCREEN_W,19,makecol(0,0,0));
    for (i=0; (i+curtile)<MAXTILES && (90+i*17)<SCREEN_W; i++) {
     if (i+curtile>-1) {
      if (tiletypes[i+curtile].loaded==true) {
       draw_rle_sprite(tools,tiletypes[i+curtile].bmp,90+i*17,3);
      }
     }
    }
    rect(tools,89,2,106,19,makecol(0,255,0));
    if (key[KEY_LEFT]) {
     if (curtile>-1) curtile--;
     if(curtile==-1) layer=1;
     clear_keybuf();
    }
    if (key[KEY_RIGHT]) {
     if (curtile<MAXTILES-1) curtile++;
     clear_keybuf();
    }
    if (key[KEY_DOWN]) layer++;
    if (key[KEY_UP]) {
     clear_keybuf();
     brush++;
    }
    if (key[KEY_DOWN]) {
     clear_keybuf();
     brush--;
    }
    if (brush<1) brush=1;
    if (brush>3) brush=4;

    if (curtile>=0) {
     if (!permit[tiletypes[curtile].action][layer]) {
      if (layer==0) layer=1; else layer=0;
     }
    }
    break;
   case 1:
    textprintf(tools,nfont,1,1,RED,S_PLACELIGHT);
    textprintf(tools,nfont,80,1,GREEN,S_LCOLOR, lcolnames[lcolor]);
    textprintf(tools,nfont,80,12,BLUE,S_LTYPE, ltypenames[ltype]);
    if (mouse_b & 1) {
     new light(curx,cury,lcolor,ltype,0,0); mouse_b=0;
    }
    if (mouse_b & 2) {
     sp=sprite::findat(curx,cury,SP_LIGHT);
     if (sp) delete sp;
    }
    if (key[KEY_LEFT]) {
     if (lcolor>0) lcolor--;
     clear_keybuf();
    }
    if (key[KEY_RIGHT]) {
     if (lcolor<6) lcolor++;
     clear_keybuf();
    }
    if (key[KEY_UP]) {
     if (ltype>0) ltype--;
     clear_keybuf();
    }
    if (key[KEY_DOWN]) {
     if (ltype<2) ltype++;
     clear_keybuf();
    }
    break;
   case 2:
   case 3:
    textprintf(tools,nfont,1,1,RED,S_PLACEBOAT,tool+1,tool-1);
    if (mouse_b & 1) {
     bc[tool-2].x=curx;
     bc[tool-2].y=cury;
    }
    if (key[KEY_LEFT]) {
     bc[tool-2].a-=itofix(1);
    }
    if (key[KEY_RIGHT]) {
     bc[tool-2].a+=itofix(1);
    }
    break;
   default:
    tool=3;
    break;
  }

  //F”lj efter muspekaren
  mapview->follow(cur);

  //rita krafset
  mapview->draw();

  if (key[KEY_M])
   map->drawminimap(viewbitmap);
  //rita spelarpositioner
  text_mode(-1);
  for (i=0; i<2; i++) {
   tmpx=(bc[i].x-mapview->getsx())>>12;
   tmpy=(bc[i].y-mapview->getsy())>>12;
   textprintf(viewbitmap,nfont,tmpx-4,tmpy-4,GREEN,"%i",i+1);
   line(viewbitmap,tmpx,tmpy,tmpx+(fsin(bc[i].a)>>12),tmpy-(fcos(bc[i].a)>>12),makecol(255,128,0));
  }
  if (key[KEY_F1]) {
   y=4;
   textprintf(viewbitmap,nfont,4,y+=10,RED,S_EDITHELP1);
   textprintf(viewbitmap,nfont,4,y+=10,RED,S_EDITHELP2);
   textprintf(viewbitmap,nfont,4,y+=10,RED,S_EDITHELP3);
  }
  //TADAM! Skicka till sk„rmen!
  display.flip();
  while (target_cycle>actual_cycle) {
   sprite::moveall();
   actual_cycle++;
  }
  if (key[KEY_ESC]) {
   clear_keybuf();
   show_mouse(screen);
   a=do_menu(filemenu,3,3);
   show_mouse(NULL);
   switch(a) {
    case 0:
     newmap();
     cur=new editcursor(30,30);
     break;
    case 1:
     if (loadmap()) cur=new editcursor(30,30);
     break;
    case 2:
     savemap();
     break;
    case 3:
     showprops();
     break;
    case 4:
     fixedges();
     break;
    case 6:
     if (alert(S_EXITEDITOR1,S_EXITEDITOR2,S_EXITEDITOR3,S_YES,S_NO,S_Y,S_N)==1) {
      quit=true;
     }
     clear_keybuf();
     break;
   }
  }
 } while (!quit);
 sprite::killall();
 remove_int(speeder);
 delete mapview;
 display.fadeout();
 destroy_bitmap(viewbitmap);
 destroy_bitmap(tools);
}

void mapedit() {
 newmap();
 editmap();
}
