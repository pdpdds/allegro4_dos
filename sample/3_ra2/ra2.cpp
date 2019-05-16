/*

  (c) 2000 by George M. Tzoumas

This program is distributed in the hope that it will be useful, 
but WITHOUT ANY WARRANTY; without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
Use this software AT YOUR OWN RISK.

*/


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<allegro.h>
#include<time.h>

#include"game.h"
#include"ra2dat.h"

#ifdef WIN32
#include <math.h>
#endif

BITMAP *Images[IMG_TOT];        /* pointers to all images */
int Obj_ID[OBJ_MAX] = {0};
int delta_x[DIR_MAX] = {1, -1, 0, 0};
int delta_y[DIR_MAX] = {0, 0, -1, 1};
int keyb;

PALETTE pal;
PALETTE pal2;

char ch_key[25] = {0};
char gmsg[80] = {0};
char msg_c = 0;
char klen = 0;

int intersect(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
    if (x3 > x1) x1 = x3;
    if (y3 > y1) y1 = y3;
    if (x4 < x2) x2 = x4;
    if (y4 < y2) y2 = y4;
    return x1 <= x2 && y1 <= y2;
}

/* handle possible removals */
void purge(TColly *d, TColly *s, int force)
{
    TObject *p;
    for (int i = 0; i < d->count; i++) {
        p = (TObject *) d->items[i];
        if (p->free || force) delete p;
        if (s != NULL) s->remove(d->items[i]);
    }
    d->count = 0;
}

void init_images()
{
    int i;
    Images[0] = load_pcx("backgr.pcx", pal);
    Images[1] = load_pcx("wall.pcx", pal);
    Images[2] = load_pcx("art1.pcx", pal);
    Images[3] = load_pcx("diamond.pcx", pal);
    Images[4] = load_pcx("key.pcx", pal);
    Images[5] = load_pcx("keyhole.pcx", pal);
    Images[6] = load_pcx("boots.pcx", pal);
    Images[7] = load_pcx("pins.pcx", pal);
    Images[8] = create_bitmap(TILE_W, TILE_H);
    clear(Images[8]);
    Images[9] = load_pcx("gameover.pcx", pal);
    Images[10] = load_pcx("paused.pcx", pal);
    Images[PL_FBASE] = load_pcx("wf.pcx", pal);
    Images[PL_FBASE+1] = load_pcx("wr1.pcx", pal);
    Images[PL_FBASE+2] = load_pcx("wr2.pcx", pal);
    Images[PL_FBASE+3] = load_pcx("wr3.pcx", pal);
    Images[PL_FBASE+4] = load_pcx("wr4.pcx", pal);
    for (i=PL_FBASE+5; i<=PL_FBASE+2*(PL_WFRAMES-1); i++) {
      Images[i] = create_bitmap(TILE_W, TILE_H);
      clear(Images[i]);
    }
    for (i=0; i<4; i++)
        draw_sprite_h_flip(Images[PL_FBASE+5+i], Images[PL_FBASE+1+i], 0, 0);
    Images[PL_FBASE+2*PL_WFRAMES-1] = load_pcx("wjr.pcx", pal);
    Images[PL_FBASE+2*PL_WFRAMES] = create_bitmap(TILE_W, TILE_H);
    clear(Images[PL_FBASE+2*PL_WFRAMES]);
    draw_sprite_h_flip(Images[PL_FBASE+2*PL_WFRAMES],
                       Images[PL_FBASE+2*PL_WFRAMES-1], 0, 0);
    Images[PL_FBASE+2*PL_WFRAMES+1] = load_pcx("dead1.pcx", pal);
    Images[PL_FBASE+2*PL_WFRAMES+2] = load_pcx("dead2.pcx", pal);
    Images[PL_FBASE+2*PL_WFRAMES+3] = load_pcx("dead3.pcx", pal);
    Images[PL_FBASE+2*PL_WFRAMES+4] = load_pcx("dead4.pcx", pal);
}

void cheat_enQ(char c)
{
    ch_key[klen++] = c;
    if (klen >= 25) klen = 0;
}

int cheat_test(char *s)
{
    int i, j = klen-1, l = strlen(s);
    if (j<0) j = 24;
    for (i=1; i<=l; i++) {
        if (ch_key[j] != s[l-i]) return 0;
        if (--j < 0) j = 24;
    }
    memset(ch_key, 0, sizeof(ch_key));
    klen = 0;
    return 1;
}

void mesg(char *s)
{
    strcpy(gmsg, s);
    msg_c = 200;
}

void cheat(TPlayer *p)
{
    if (keypressed()) {
        keyb = readkey();
        cheat_enQ(keyb & 0xff);
    } else keyb = 0;
    if (p == NULL) return;
    if (cheat_test("jumpmore")) if (p->jlen < JLEN*4) {
        p->jlen += JLEN;
        mesg("jump length increased");
    } else; else
    if (cheat_test("jumpless")) if (p->jlen > JLEN) {
        p->jlen -= JLEN;
        mesg("jump length decreased");
    } else; else
    if (cheat_test("jumpnorm")) {
        p->jlen = JLEN;
        mesg("jump length set to default value");
    } else
    if (cheat_test("scoreup"))
      if (p->score < 400000000) {
          p->score = p->score*4 / 3 / 10 * 10;
          mesg("score increased");
      } else; else
    if (cheat_test("rip")) {
        p->die();
        mesg("rest in peace");
    } else
    if (cheat_test("qzwx")) {
        p->lives++;
        mesg("extra life!");
    } else
    if (cheat_test("idkfa") || cheat_test("iddqd")) mesg(":-)");
}

void focus(TLevel *l)
{
    int x,y;
    if (l == NULL || l->player == NULL) return;
    x = l->player->pos.x-VIS_X/2;
    y = l->player->pos.y-VIS_Y/2;
    l->move_camera(x,y);
}

void __pause()
{
//    textout_centre(screen, font, "-- PAUSED --", 160, 180, 152);
//    draw_sprite(screen, Images[IMG_PAUSED], 110, 128);
    blit(Images[IMG_PAUSED], screen, 0, 0, 110, 80, 100, 40);
    set_palette(pal2);
//    fade_from(pal, pal2, 6);
    do; while ((keyb=readkey())>>8!=KEY_F12);
//    fade_from(pal2, pal, 8);
    set_palette(pal);
}

int main(void)
{
    DATAFILE *dat;
    int seed = time(NULL);
    srand(seed);
//    srand(3);
    TLevel *l = new TLevel(1,(rand()%3+1)*20, (rand()%3+1)*11, TILE_W,TILE_H,RIGHT);
    TPlayer *pl = new TPlayer(l);
    TKeyhole *akeyhole = new TKeyhole(l, rand() % l->size.y + 1,
                               rand() % l->size.x + 1);
    TKey *akey = new TKey(l, akeyhole, rand() % l->size.y + 1,
                             rand() % l->size.x + 1);

#ifdef _WIN32
	TBoots *boots = new TBoots(l, rand() % l->size.y + 1,
		rand() % l->size.x + 1);
#else
	TBoots *boots = new TBoots(l, random() % l->size.y + 1,
		rand() % l->size.x + 1);
#endif    
    allegro_init();
    dat = load_datafile("ra2.dat");
    font = (FONT *) dat[TINY_FONT].dat;
    install_keyboard();
    install_mouse();
    l->move_camera(0,0);
    BITMAP *gs, *ps;
    int i,j,k;
    for (i=0; i<2; i++) {
        j = rand() % l->size.x;
        new TLRBlock(l, rand() % l->size.y, j, j+l->size.x);
    }
    new TUDBlock(l, 2, 1, l->size.x);
    for (i = 1; i<=l->size.y; i++)
      for (j = 1; j<=l->size.x; j++) {
          k = rand() % 5;
          if (k < 4) k = 0; else k = 1;
          l->map[i][j].img = char (k);
          l->map[i][j].flags |= (k==0)?TL_EMPTY:TL_WALL;
      }
    for (k = 0; k<0.26*l->size.y*l->size.x; k++) {
          i = rand() % l->size.y + 1;
          j = rand() % l->size.x + 1;
          if (l->map[i][j].flags & TL_WALL) {
              l->map[i][j].img = IMG_ART1;
          } else {
                l->map[i][j].img = IMG_DIAMOND;
                l->map[i][j].flags |= TL_ITEM;
          }
    }
    for (k = 0; k<0.26*l->size.y*l->size.x; k++) {
          i = rand() % l->size.y + 1;
          j = rand() % l->size.x + 1;
          if ((l->map[i+1][j].flags & TL_WALL) &&
              (l->map[i][j].flags & TL_WALL) &&
              !(l->map[i][j].flags & TL_TRAP) &&
              (l->map[i-1][j].img == IMG_EMPTY)) {
              l->map[i][j].img = IMG_PINS;
              l->map[i][j].flags = TL_TRAP;
          }
    }
    for (i=1; i<=l->size.y; i++) l->map[i][2].img = 0, l->map[i][2].flags = 0;
#ifdef _WIN32
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 320, 200, 0, 0);
#else
	set_gfx_mode(GFX_VGA, 320, 200, 0, 0);
#endif    
    gs = create_bitmap(VIS_X, VIS_Y);
    ps = create_bitmap(320, 24);
    init_images();
    set_palette(pal);
    set_mouse_range(0,0,l->size.x*TILE_W-VIS_X,l->size.y*TILE_H-VIS_Y);
    position_mouse(0,0);
    fade_interpolate(pal, black_palette, pal2, 20, 0, 255);
    mesg("GOOD LUCK!");
    do {
        clear(gs); l->draw(gs);
        clear(ps);
        if (l->player) {
            l->player->show_status(ps);
        }
        if (msg_c) {
            msg_c--;
            textout(ps, font, gmsg, VIS_X-text_length(font, gmsg), 8, 152);
        }
        if (l->player == NULL)
//          draw_sprite(gs, Images[IMG_GAMEOVER], 90, 80);
          blit(Images[IMG_GAMEOVER], gs, 0, 0, 75, 80, 170, 40);
        vsync();
        blit(gs, screen, 0, 0, 0, 0, VIS_X, VIS_Y);
        blit(ps, screen, 0, 0, 0, VIS_Y, 320, 24);
/*        stretch_blit(vs, screen, 0, 0, VIS_X, VIS_Y, 0, 0, 639, 479); */
        l->run();
        if (!(key[KEY_C] && key[KEY_A])) focus(l);
        else l->move_camera(mouse_x, mouse_y);

        cheat(l->player);
        if ((keyb>>8) == KEY_F12) __pause();
    } while (!key[KEY_ESC]);
    fade_out(8);
    clear_keybuf();
    delete l;
    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
    printf("Seed was %d\n", seed);
    return 0;
}

#ifdef _WIN32
END_OF_MAIN()
#endif