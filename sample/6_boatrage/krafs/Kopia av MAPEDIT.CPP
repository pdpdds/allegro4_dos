//MAPEDIT.CC
#include "boatrage.h"
#include "wrappers.h"
#include "sprite.h"
#include "player.h"
#include "objects.h"
#include "tilemap.h"
#include "view.h"
#include "menu.h"
#include "mapedit.h"

boatcoord bc[2];

coolmenuitem m_editmenu[]={
{"Skapa en ny bana",M_CHOICE,40, 64,0,0,0,0},
{"Ladda en bana",   M_CHOICE,40, 84,0,0,0,0},
{"Tillbaka",        M_CHOICE,40,114,0,0,0,0}
};

MENU filemenu[]={
{"Ny bana..."},
{"Ladda bana..."},
{"Spara bana..."},
{"Egenskaper..."},
{""},
{"Stäng editorn"},
{NULL,NULL,0,NULL,NULL}
};

char strwidth[4]="32";
char strheight[4]="32";
char strlaps[3]="5";
char strsong[3]="1";
char strname[16]="X";
char strcreator[16]="Y";

char lcolnames[7][8]={
"Vitt",
"Rött",
"Grönt",
"Blått",
"Gult",
"Magenta",
"Cyan",
};

char ltypenames[3][12]={
"Litet",
"Stort",
"Fyr",
};
//Ž™†„”=ÅÄÖåäö

DIALOG newlevel[] ={
//Typ          X   Y   W   H   FG     BG     Key    Flags    d1   d2   dp     dp2    dp3
{d_clear_proc, 0,  0,  0,  0,  255,   0,     0,     0,       0,   0},
{d_text_proc , 50, 80, 0,  0,  255,   0,     0,     0,       0,   0, "Hur stor ska banan vara?"},
{d_text_proc , 70, 90, 0,  0,  255,   0,     0,     0,       0,   0, "Bredd (x):"},
{d_text_proc , 70, 110,0,  0,  255,   0,     0,     0,       0,   0, "Höjd (y):"},
{d_edit_proc , 150,90, 32, 10, 255,   0,     0,     0,       sizeof(strwidth)-1,   0,  strwidth},
{d_edit_proc , 150,110,32, 10, 255,   0,     0,     0,       sizeof(strheight)-1,  0,  strheight},
{d_button_proc,255,90, 60, 12, 255,   0, KEY_ENTER, D_EXIT,  0,   0, "OK"},
{NULL,0,0,0,0,0,0,0,0,0,0,NULL,NULL,NULL}
};

DIALOG levelprops[] ={
//Typ          X   Y   W   H   FG     BG     Key    Flags    d1   d2   dp     dp2    dp3
{d_clear_proc, 0,  0,  0,  0,  255,   0,     0,     0,       0,   0},
{d_text_proc , 10, 80, 0,  0,  255,   0,     0,     0,       0,   0, "Egenskaper för banan"},
{d_text_proc , 30, 90, 0,  0,  255,   0,     0,     0,       0,   0, "Namn:"},
{d_text_proc , 30, 110,0,  0,  255,   0,     0,     0,       0,   0, "Av:"},
{d_text_proc , 30, 130,0,  0,  255,   0,     0,     0,       0,   0, "# varv:"},
{d_text_proc , 30, 150,0,  0,  255,   0,     0,     0,       0,   0, "Låt #:"},
{d_edit_proc , 90,90, 100,10,  255,   0,     0,     0,       sizeof(strname)-1,   0,  strname},
{d_edit_proc , 90,110,100,10,  255,   0,     0,     0,       sizeof(strcreator)-1,     0,  strcreator},
{d_edit_proc , 90,130,32, 10,  255,   0,     0,     0,       sizeof(strlaps)-1,   0,  strlaps},
{d_edit_proc , 90,150,32, 10,  255,   0,     0,     0,       sizeof(strsong)-1,   0,  strsong},
{d_button_proc,200,90, 60, 12, 255,   0, KEY_ENTER, D_EXIT,  0,   0, "OK"},
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
   alert("Antalet varv måste vara","ett heltal över 0!",NULL,"Ja, ja, ja",NULL,'J',NULL);
   ok=false;
  } else {
   ok=true;
  }
  song=atoi((char *)levelprops[9].dp);
  if (song<=0 || song>16) {
   alert("Låtnumret måste vara","ett heltal mellan 1 och 16!",NULL,"Okay, okay",NULL,'J',NULL);
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
   alert("Både bredd och höjd måste","vara heltal över 0",NULL,"Okiedokie",NULL,'J',NULL);
   ok=false;
  } else {
   ok=true;
  }
 } while (!ok);
 display.setstdpal();
 map->newmap(w,h);
 for (w=0; w<2; w++) {
  bc[w].x=bc[w].y=bc[w].a=0;
 }
}

void loadmap() {
 char filename[255];
 int ret;
 for (ret=0; ret<256; ret++) filename[ret]='\0';
 display.setstdpal();
 ret=file_select("Ladda bana...",filename,"BOA");
 if (ret==0) return;
 if (!exists(filename)) {
  alert("Filen finns inte!",NULL,NULL,"Nähä...",NULL,'J',NULL);
  return;
 }
 sprite::killall();
 map->loadmap(filename,bc[0],bc[1]);
 Music.playsong(map->getsong());
}

void savemap() {
 char filename[255];
 int ret;
 for (ret=0; ret<256; ret++) filename[ret]='\0';
 ret=file_select("Spara bana...",filename,"BOA");
 if (ret==0) return;
 map->savemap(filename,bc[0],bc[1]);
}

void editmap() {
 //fixa ett par vyer
 BITMAP *viewbitmap=create_sub_bitmap(display,0,0,display.w(),display.h()-32);
 BITMAP *tools=create_sub_bitmap(display,0,display.h()-32,display.w(),32);
 FONT *nfont=(FONT *)d(FONT_NORMAL);

 view *mapview=new view(viewbitmap); //de tv† vyerna
 editcursor *cur;

 install_int_ex(speeder,BPS_TO_TIMER(60));

 actual_cycle=0;
 target_cycle=0;
 //Huvudloopen
 display.setstdpal();
 int i,curtile=0,quit=0,a;
 int tool=0;
 int curx,cury;
 int lcolor=0,ltype=0;
 int tmpx,tmpy;
 sprite *sp;
 cur=new editcursor(30,30);
 do {
  curx=cur->getX();
  cury=cur->getY();
  clear(tools);
  if (key[KEY_PGUP]) {
   if (tool>0) tool--;
   key[KEY_PGUP]=0;
  }
  if (key[KEY_PGDN]) {
   if (tool<9) tool++;
   key[KEY_PGDN]=0;
  }
  switch (tool) {
   case 0:
   case 1:
    if(tool==0) {
     if(curtile==-1) curtile++;
    }
    textprintf(tools,nfont,1,1,-1,"%i: Lager #%i",tool+1,tool);
    textprintf(tools,nfont,76,10,-1,"%i",curtile);
    if (mouse_b & 1) {
     map->set(curx>>16,cury>>16,tool,curtile);
    }
    if (mouse_b & 2) {
     curtile=map->get(curx>>16,cury>>16,tool);
    }
    for (i=0; (i+curtile)<MAXTILES && i<20; i++) {
     if (i+curtile>-1) {
      if (tiletypes[i+curtile].loaded==true) {
       draw_sprite(tools,tiletypes[i+curtile].bmp,90+i*17,1);
      }
     }
    }
    rect(tools,89,0,106,18,makecol(0,255,0));
    if (key[KEY_LEFT]) {
     if (curtile>0-tool) curtile--;
     clear_keybuf();
    }
    if (key[KEY_RIGHT]) {
     if (curtile<MAXTILES-1) curtile++;
     clear_keybuf();
    }
    break;
   case 2:
    textprintf(tools,nfont,1,1,-1,"3: Ljus");
    textprintf(tools,nfont,60,1,-1,lcolnames[lcolor]);
    textprintf(tools,nfont,60,12,-1,ltypenames[ltype]);
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
   case 3:
   case 4:
    textprintf(tools,nfont,1,1,-1,"%i: Båt #%i",tool+1,tool-3);
    if (mouse_b & 1) {
     bc[tool-3].x=curx;
     bc[tool-3].y=cury;
    }
    if (key[KEY_LEFT]) {
     bc[tool-3].a-=itofix(1);
    }
    if (key[KEY_RIGHT]) {
     bc[tool-3].a+=itofix(1);
    }
    break;
   default:
    break;
  }

  //byt denna rad mot n†t *mycket* b„ttre!!!!!!!!
  mapview->follow(cur);

  //rita krafset
  mapview->draw();
  //rita spelarpositioner
  text_mode(-1);
  for (i=0; i<2; i++) {
   tmpx=(bc[i].x-mapview->getsx())>>12;
   tmpy=(bc[i].y-mapview->getsy())>>12;
   textprintf(viewbitmap,nfont,tmpx-4,tmpy-4,-1,"%i",i);
   line(viewbitmap,tmpx,tmpy,tmpx+(fsin(bc[i].a)>>12),tmpy-(fcos(bc[i].a)>>12),makecol(255,128,0));
  }
  //TADAM! Skicka till sk„rmen!
  blit(display,screen,0,0,0,0,SCREEN_W,SCREEN_H);
  while (target_cycle>actual_cycle) {
   sprite::moveall();
   actual_cycle++;
  }
  if (key[KEY_ESC]) {
   clear_keybuf();
   show_mouse(screen);
   a=do_menu(filemenu,0,0);
   show_mouse(NULL);
   switch(a) {
    case 0:
     newmap();
     cur=new editcursor(30,30);
     //b[0]=new boat(100000,100000,3,NULL);
     //b[1]=new boat(200000,100000,3,NULL);
     break;
    case 1:
     loadmap();
     cur=new editcursor(30,30);
     break;
    case 2:
     savemap();
     break;
    case 3:
     showprops();
     break;
    case 5:
     if (alert("Säker på att du","vill stänga editorn","utan att spara?","Ja","Nej",'J','N')==1) {
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
 int quit=0,a=0;
 do {
  a=coolmenu((BITMAP *)d(BOATRAGE),m_editmenu,3,a);
  switch (a) {
   case 0:
    newmap();
    editmap();
    Music.playsong(menumusic);
    break;
   case 1:
    loadmap();
    if (map) editmap();
    Music.playsong(menumusic);
    break;
   case -1:
   case 2:
    quit=true;
    break;
  }
 } while (!quit);
}
