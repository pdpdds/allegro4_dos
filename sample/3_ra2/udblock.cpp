#include"game.h"

TUDBlock::TUDBlock(TLevel *alevel, int tx, int aty1, int aty2):
  TObject(alevel, OBJ_UDBLOCK)
{
    size.x = TILE_W; size.y = TILE_H;
    img = IMG_WALL;
    pos.x = tx * TILE_H;
    pos.y = aty1 * TILE_W;
    ty1 = aty1; ty2 = aty2;
    wdir = UP;
    cD = 0;
    tileflags = TL_WALL;
    visitlock = 0;
}

void TUDBlock::draw(BITMAP *b, int x, int y)
{
    draw_sprite(b, Images[img], x, y);
}

void TUDBlock::run()
{
    TPoint t, e;
    if (cD++ & 2) return;
    calc_pos(&t, &e);
    level->map[t.y][t.x].obj = NULL;
    level->map[t.y+!e.y][t.x].obj = NULL;
    if (get_close(wdir, t, e) & TL_WALL) wdir = (wdir == UP)? DOWN: UP;
    else if (e.y) if (t.y == ty1) wdir = DOWN;
    else if (t.y == ty2) wdir = UP;
    pos.y += delta_y[wdir]*2;
    calc_pos(&t, &e);
    level->map[t.y][t.x].obj = this;
    level->map[t.y+!e.y][t.x].obj = this;
    visitlock = 0;
}

void TUDBlock::handle_visit(class TObject *s)
{
    TPlayer *p;
    TPoint t, e, pt, pe;
    if (s->type != OBJ_PLAYER || visitlock) return;
    p = (TPlayer *) s;
    calc_pos(&t, &e);
    p->calc_pos(&pt, &pe);
    if (!(aside(p, LEFT, 1, 1) || aside(p, RIGHT, 1, 1) ||
          aside(p, UP, 1, 1) || aside(p, DOWN, 1, 1))) goto out;
    p->elev = this;
    if (abs(p->pos.y-pos.y) < TILE_W/2) p->die();
out:
    visitlock++;
}
