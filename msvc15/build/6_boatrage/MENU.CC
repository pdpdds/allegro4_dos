#include "boatrage.h"
#include "wrappers.h"
#include "menu.h"

int coolmenu(RLE_SPRITE *background, coolmenuitem *items, int count, int sel) {
 int i,first=1;
 clear(display);
 display.goblack();
 text_mode(-1);
 Music.playsong(menumusic);
 do {
  clear(display);
  if (background) draw_rle_sprite(display,background,SCR_X,SCR_Y);
  for (i=0; i<count; i++)
   textprintf(display,(FONT *)d(FONT_SLICKER),items[i].x+SCR_X,items[i].y+SCR_Y-5,-1,items[i].text);

  draw_trans_rle_sprite(display,(RLE_SPRITE *)d(SELECTOR),items[sel].x+SCR_X,items[sel].y+SCR_Y);
  display.flip();
  if (first) {first=0; display.fadein(4);}
  if (key[KEY_UP])    {
   clear_keybuf();
   Sound.play((SAMPLE *)d(SND_CHOOSE));
   sel--;
   clear(display);
  }
  if (key[KEY_DOWN])  {
   clear_keybuf();
   Sound.play((SAMPLE *)d(SND_CHOOSE));
   sel++;
  }
  if (key[KEY_ENTER]) {
   clear_keybuf();
   Sound.play((SAMPLE *)d(SND_SELECT));
   display.fadeout(4); return sel;
  }
  if (sel<0) sel=count-1;
  if (sel>count-1) sel=0;
 } while (!key[KEY_ESC]);
 clear_keybuf();
 display.fadeout(4);
 clear(display);
 return -1;
}

