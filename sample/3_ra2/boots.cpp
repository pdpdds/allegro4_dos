#include"game.h"

TBoots::TBoots(TLevel *alevel, int row, int col): TObject(alevel, OBJ_BOOTS)
{
    type = OBJ_BOOTS;
    id = Obj_ID[type]++;
    img = IMG_BOOTS;
    tpos.x = col; tpos.y = row;
    pos.y = row * TILE_H;
    pos.x = col * TILE_W;
    size.x = TILE_W; size.y = TILE_H;
    level->map[row][col].obj = this;
    jval = BOOT_JLEN;
    cc = BOOT_TIMER;
    tileflags = TL_BOOTS;
}

void TBoots::draw(BITMAP *b, int x, int y)
{
    draw_sprite(b, Images[img], x, y);
    if (owner != NULL)
      line(b, x, y, x+(int) ((1.0*cc/BOOT_TIMER)*TILE_W) , y, 155);
}

void TBoots::handle_visit(class TObject *s)
{
    if (s->type != OBJ_PLAYER) return;
    TPlayer *p = (TPlayer *) s;
    level->remove_object(this);
    level->map[tpos.y][tpos.x].obj = NULL;
    p->add_object(this);
    p->score += SCORE_BOOTS;
    p->jlen = jval;
}

int TBoots::useit(class TObject *s)
{
    if (s->type != OBJ_PLAYER) return 0;
    if (((TPlayer *)s)->state == DEAD) cc = 0;
    return cc-- > 0;
}

void TBoots::drop(class TObject *s)
{
    if (s->type != OBJ_PLAYER) return;
    ((TPlayer *)s)->jlen = JLEN;
    free++;
}

