//PSXFADE.H
//Public Domain (Freeware) av Henrik Rydgård

class screenman {
 public:
  void fadeout(PALETTE pal, int speed=1);
  void fadein(PALETTE pal, int speed=1);
  void goblack() {set_palette(black_palette);}
  void set_graphics_mode(int mode,int coldepth,int xres,int yres,int vxres=0,int vyres=0) {
   set_color_depth(coldepth);
   int ret=set_gfx_mode(mode,xres,yres,vxres,vyres);
   if (ret!=0) {
    set_color_depth(8);
    set_gfx_mode(GFX_VGA,320,200,0,0);
    set_palette(desktop_palette);
    alert("Misslyckades att fixa grafikmode","Avslutar",NULL,"OOOPS",NULL,'O',NULL);
    abort();
   }
  }
  BITMAP *create_buffer() {return create_bitmap(SCREEN_W,SCREEN_H);}
};
