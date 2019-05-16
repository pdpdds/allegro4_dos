#include"game.h"

TObject::TObject(TLevel *alevel, int atype)
{
    type = atype;
    id = Obj_ID[type]++;
    level = alevel;
    level->add_object(this);
    free = 0;
    owner = NULL;
    tileflags = 0;
}

TObject::~TObject() {}

/*int visible(TObject *p)
{
    if (p == NULL) return 0;
    else return intersect(pos.x, pos.y, pos.x+size.x, pos.y+size.y,
      p->pos.x, p->pos.y, p->pos.x+p->size.x, p->pos.y+p->size.y);
}*/

int TObject::aside(TObject *p, int dir, int thresx, int thresy)
{
    int x1, y1;
    if (p == NULL || p == this) return 0;
    x1 = pos.x + delta_x[dir]*thresx; y1 = pos.y + delta_y[dir]*thresy;
    return intersect(x1, y1, x1+size.x-1, y1+size.y-1,
      p->pos.x, p->pos.y, p->pos.x+p->size.x-1, p->pos.y+p->size.y-1);
}


void TObject::calc_pos(TPoint *t, TPoint *e)
{
    div_t d;
    d = div(pos.x, TILE_W);
    t->x = d.quot; e->x = d.rem == 0;
    d = div(pos.y, TILE_H);
    t->y = d.quot; e->y = d.rem == 0;
}

/* tile pos (t) and exact pos (e) */
/* for tiles */
short int TObject::get_close1(int dir, TPoint t, TPoint e)
{
    switch (dir) {
        case RIGHT:
            if (e.x && t.x == level->size.x) return TL_WALL;
            return level->map[t.y][t.x+1].flags |
                   level->map[t.y+!e.y][t.x+1].flags;
        case LEFT:
            if (e.x && t.x == 1) return TL_WALL;
            return level->map[t.y][t.x-e.x].flags |
                   level->map[t.y+!e.y][t.x-e.x].flags;
        case UP:
            if (e.y && t.y == 1) return TL_WALL;
            return level->map[t.y-e.y][t.x].flags |
                   level->map[t.y-e.y][t.x+!e.x].flags;
        case DOWN:
            if (e.y && t.y == level->size.y) return TL_WALL;
            return level->map[t.y+1][t.x].flags |
                   level->map[t.y+1][t.x+!e.x].flags;
    }
    return TL_WALL;
}

/* tile pos (t) and exact pos (e) */
/* for objects */
short int TObject::get_close2(int dir, TPoint t, TPoint e)
{
    TObject *p;
    short int v = 0;
    switch (dir) {
        case RIGHT:
            if (e.x && t.x == level->size.x) return TL_WALL;
            p = level->map[t.y][t.x+1].obj;
            if (p != NULL && p != this)
              v = aside(p, RIGHT, e.x, 0)*p->tileflags;
            p = level->map[t.y+!e.y][t.x+1].obj;
            if (p == NULL || p == this) break;
            v |= aside(p, RIGHT, e.x, 0)*p->tileflags;
            break;
        case LEFT:
            if (e.x && t.x == 1) return TL_WALL;
            p = level->map[t.y][t.x-e.x].obj;
            if (p != NULL && p != this)
              v = aside(p, LEFT, e.x, 0)*p->tileflags;
            p = level->map[t.y+!e.y][t.x-e.x].obj;
            if (p == NULL || p == this) break;
            v |= aside(p, LEFT, e.x, 0)*p->tileflags;
            break;
        case UP:
            if (e.y && t.y == 1) return TL_WALL;
            p = level->map[t.y-e.y][t.x].obj;
            if (p != NULL && p != this)
              v = aside(p, UP, 0, e.y)*p->tileflags;
            p = level->map[t.y-e.y][t.x+!e.x].obj;
            if (p == NULL || p == this) break;
            v |= aside(p, UP, 0, e.y)*p->tileflags;
            break;
        case DOWN:
            if (e.y && t.y == level->size.y) return TL_WALL;
            p = level->map[t.y+1][t.x].obj;
            if (p != NULL && p != this)
              v = aside(p, DOWN, 0, e.y)*p->tileflags;
            p = level->map[t.y+1][t.x+!e.x].obj;
            if (p == NULL || p == this) break;
            v |= aside(p, DOWN, 0, e.y)*p->tileflags;
            break;
    }
//    textprintf(screen, font, v*8,0,155,"%d %d (%d)", v, tileflags, type);
    return v;
}

short int TObject::get_close(int dir, TPoint t, TPoint e)
{
    return get_close1(dir, t, e) | get_close2(dir, t, e);
}

void TObject::draw(BITMAP *b, int x, int y) {}
void TObject::run() {}
void TObject::handle_visit(class TObject *s) {}
int TObject::useit(class TObject *s) { return 1; }
void TObject::drop(class TObject *s) {}
