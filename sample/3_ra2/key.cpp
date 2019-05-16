#include"game.h"

TKey::TKey(TLevel *alevel, class TKeyhole *akeyhole,
  int row, int col): TObject(alevel, OBJ_KEY)
{
    type = OBJ_KEY;
    id = Obj_ID[type]++;
    img = IMG_KEY;
    tpos.x = col; tpos.y = row;
    pos.y = row * TILE_H;
    pos.x = col * TILE_W;
    size.x = TILE_W; size.y = TILE_H;
    level->map[row][col].obj = this;
    keyhole = akeyhole;
    keyhole->pkey = this;
    tileflags = TL_KEY;
}

void TKey::draw(BITMAP *b, int x, int y)
{
    draw_sprite(b, Images[img], x, y);
}

void TKey::handle_visit(class TObject *s)
{
    if (s->type != OBJ_PLAYER) return;
    TPlayer *p = (TPlayer *) s;
    level->remove_object(this);
    level->map[tpos.y][tpos.x].obj = NULL;
    p->add_object(this);
    p->score += SCORE_KEY;
}

