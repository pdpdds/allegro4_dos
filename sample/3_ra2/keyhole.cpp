#include"game.h"

TKeyhole::TKeyhole(TLevel *alevel, int row, int col):
  TObject(alevel, OBJ_KEYHOLE)
{
    type = OBJ_KEYHOLE;
    id = Obj_ID[type]++;
    img = IMG_KEYHOLE;
    tpos.x = col; tpos.y = row;
    pos.y = row * TILE_H;
    pos.x = col * TILE_W;
    size.x = TILE_W; size.y = TILE_H;
    level->map[row][col].obj = this;
    level->map[row][col+1].flags |= TL_WALL;
    pkey = NULL;
    tileflags = TL_KEYHOLE;
}

void TKeyhole::draw(BITMAP *b, int x, int y)
{
    draw_sprite(b, Images[img], x, y);
}

void TKeyhole::handle_visit(class TObject *s)
{
    if (s->type != OBJ_PLAYER) return;
    TPlayer *p = (TPlayer *) s;
    if (p->remove_object(pkey)) {
        pkey->free++;
        level->remove_object(this);
        free++;
        level->map[tpos.y][tpos.x].obj = NULL;
        level->map[tpos.y][tpos.x+1].flags -= TL_WALL;
        p->score += SCORE_KEYHOLE;
    }
}

