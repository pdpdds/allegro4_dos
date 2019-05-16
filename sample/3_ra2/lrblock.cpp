#include"game.h"

TLRBlock::TLRBlock(TLevel *alevel, int ty, int atx1, int atx2):
  TObject(alevel, OBJ_LRBLOCK)
{
    size.x = TILE_W; size.y = TILE_H;
    img = IMG_WALL;
    pos.x = atx1 * TILE_W;
    pos.y = ty * TILE_H;
    tx1 = atx1; tx2 = atx2;
    wdir = RIGHT;
    cD = 0;
    tileflags = TL_WALL;
    visitlock = 0;
}

void TLRBlock::draw(BITMAP *b, int x, int y)
{
    draw_sprite(b, Images[img], x, y);
}

void TLRBlock::run()
{
    TPoint t, e;
    if (cD++ & 1) return;
    calc_pos(&t, &e);
    level->map[t.y][t.x].obj = NULL;
    level->map[t.y][t.x+!e.x].obj = NULL;
    if (get_close(wdir, t, e) & TL_WALL) wdir = (wdir == RIGHT)? LEFT: RIGHT;
    else if (e.x) if (t.x == tx1) wdir = RIGHT;
    else if (t.x == tx2) wdir = LEFT;
    pos.x += delta_x[wdir];
    calc_pos(&t, &e);
    level->map[t.y][t.x].obj = this;
    level->map[t.y][t.x+!e.x].obj = this;
    visitlock = 0;
}

void TLRBlock::handle_visit(class TObject *s)
{
    TPlayer *p;
    TPoint t, e, pt, pe;
    int ndir;
    if (s->type != OBJ_PLAYER || visitlock) return;
    p = (TPlayer *) s;
    calc_pos(&t, &e);
    p->calc_pos(&pt, &pe);
    if (!(aside(p, LEFT, 1, 1) || aside(p, RIGHT, 1, 1) ||
          aside(p, UP, 1, 1) || aside(p, DOWN, 1, 1))) goto out;
    if (pe.y && (pt.y == t.y-1))
        p->walk(p->wdir, wdir, 0, 1, pt, pe);
    else if (pt.y == t.y) {
        if (p->pos.x < pos.x) ndir = LEFT; else ndir = RIGHT;
        if (wdir != p->wdir && p->wdir == ndir) goto out;
        p->walk(p->wdir, ndir, 0, 2, pt, pe);
        if (abs(p->pos.x-pos.x) < TILE_W/2 && e.y) p->die();
    }
out:
    visitlock++;
}

