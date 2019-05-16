//WRAPPERS.H


inline int rnd(int i) {return rand()%i;}
inline void *d(int i) {return data[i].dat;}

void draw_trans_sprite_center(BITMAP *bmp,BITMAP *pic,int x, int y);
void draw_trans_rle_sprite_center(BITMAP *bmp,RLE_SPRITE *pic,int x, int y);

void rotate_sprite_center(BITMAP *bmp,BITMAP *pic,int x,int y,fixed a);

inline int themax(int t1,int t2) {return t1>=t2 ? t1 : t2;}
inline int themin(int t1,int t2) {return t1<=t2 ? t1 : t2;}

class musicplayer {
 public:
  musicplayer() {song=NULL; playing=false;}
  void setsong(int i) {song=(MIDI *)d(SONG000+i); cursong=i;}
  void playsong(int i) {
   if (!playing) {
    startsong(i);
   } else {
    if (i!=cursong) {
     startsong(i);
    }
   }
  }
  void startsong(int i) {
   stop();
   setsong(i);
   play();
  }
  void play() {if (!nosound) play_midi(song,99999); playing=true;}
  void stop() {if (playing && !nosound) stop_midi(); playing=false;}
 protected:
  MIDI *song;
  int cursong;
  int playing;
};

class soundplayer {
 public:
  soundplayer(){};
  void play(SAMPLE *snd, int vol=255, int pan=128, int speed=1000, int loop=0) {
   if (!nosound) play_sample(snd,vol,pan,speed,loop);
  }
  void adjust(SAMPLE *snd, int vol=255, int pan=128, int speed=1000, int loop=0) {
   if (!nosound) adjust_sample(snd,vol,pan,speed,loop);
  }
  void stop(SAMPLE *snd) {
   if (!nosound) stop_sample(snd);
  }
};

extern musicplayer Music;
extern soundplayer Sound;

class screenman {
 public:
  screenman() {buffer=0;}
  ~screenman() {/*if (buffer) destroy_bitmap(buffer);*/}
  void fadeout(int speed=1);
  void fadein(int speed=1);
  void goblack() {set_palette(black_palette);}
  int set_graphics_mode(int mode,int coldepth,int _xres,int _yres,int vxres=0,int vyres=0) {
   if (buffer) destroy_bitmap(buffer);
   set_color_depth(coldepth);
   colordepth=coldepth;
   xres=_xres;
   yres=_yres;
   int ret=set_gfx_mode(mode,xres,yres,vxres,vyres);
   int i=1;
   if (ret!=0) {
    xres=320; yres=200; coldepth=8;
    set_color_depth(8);
#ifdef _WIN32
    set_gfx_mode(GFX_AUTODETECT_WINDOWED,xres,yres,0,0);
#else
	set_gfx_mode(GFX_AUTODETECT, xres, yres, 0, 0);
#endif
    set_palette(desktop_palette);
    i=0;
   }
   buffer=create_bitmap(xres,yres);
   return i;
  }
  void setstdpal() {
   set_palette(stdpal);
  }
  void setthestdpal(PALETTE &pal) {
   memcpy(stdpal, pal, sizeof(PALETTE));
  }
  void setpal(PALETTE pal) {set_palette(pal);}
  void creatergbtable() {
   create_rgb_table(&rgb_table,stdpal,NULL);
   rgb_map=&rgb_table;
  }
  void createcolortable(void (*blend)(const PALETTE pal,int x, int y, RGB *rgb)) {
   create_color_table(&color_table,stdpal,blend,NULL);
   color_map=&color_table;
  }
  operator BITMAP* () {    //s?att man kan t ex g”ra s?h„r: clear(display);
   return buffer;
  }
  void flip() {
   blit(buffer,screen,0,0,0,0,buffer->w,buffer->h);
  }
  BITMAP *subbitmap(int x,int y, int w, int h) {
   return create_sub_bitmap(buffer,x,y,w,h);
  }
  int w() {if (buffer) return buffer->w; else return 0;}
  int h() {if (buffer) return buffer->h; else return 0;}
  int depth() {return colordepth;}
 protected:
  PALETTE stdpal;
  RGB_MAP rgb_table;
  COLOR_MAP color_table;
  BITMAP *buffer;
  int colordepth;
  int xres,yres;
};

extern screenman display;

