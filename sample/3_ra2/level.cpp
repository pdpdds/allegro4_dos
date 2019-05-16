#include"game.h"

TLevel::TLevel(int ano, int asx, int asy, int ox, int oy, int idir)
{
    int i,j;
    no = ano;
    size.x = asx;
    size.y = asy;
    if (size.x > MAX_LEVEL_W) size.x = MAX_LEVEL_W;
    if (size.y > MAX_LEVEL_H) size.y = MAX_LEVEL_H;
    for (i=0; i<MAX_LEVEL_H+2; i++) for (j=0; j<MAX_LEVEL_W+2; j++) {
        map[i][j].img = IMG_EMPTY;
        map[i][j].flags = 0;
        map[i][j].obj = NULL;
    }
    for (i=0; i<size.y+2; i++) {
        map[i][0].flags = TL_WALL;
        map[i][size.x+1].flags = TL_WALL;
    }
    for (i=0; i<size.x+2; i++) {
        map[0][i].flags = TL_WALL;
        map[size.y+1][i].flags = TL_WALL;
    }
    home.pos.x = ox;
    home.pos.y = oy;
    home.wdir = idir;
    objects = new TColly(20,5);
    delQ = new TColly(20,5);
    player = NULL;
}

TLevel::~TLevel()
{
/*    int o = objects->count;
    fprintf(stderr, "level:-----\n");*/
    purge(delQ, objects, 1);
    purge(objects, NULL, 1);
    delete objects;
    delete delQ;
//    fprintf(stderr, "level with %d objects destroyed\n", o);
}

void TLevel::loadmap(FILE *f)
{
    fread(map, sizeof(map), 1, f);
}

int TLevel::visible(class TObject *p)
{
    return intersect(eye.x, eye.y, eye.x+VIS_X, eye.y+VIS_Y,
             p->pos.x, p->pos.y, p->pos.x+p->size.x, p->pos.y+p->size.y);
}

void TLevel::move_camera(int x, int y)
{
    eye.x = x; eye.y = y;
    if (eye.x < 0) eye.x = 0;
    if (eye.y < 0) eye.y = 0;
    if (eye.x > size.x*TILE_W-VIS_X) eye.x = size.x*TILE_W-VIS_X;
    if (eye.y > size.y*TILE_H-VIS_Y) eye.y = size.y*TILE_H-VIS_Y;
    eye.x += TILE_W; eye.y += TILE_H;
}

void TLevel::drawtile(BITMAP *b, int row, int col)
{
    int x, y;
    x = col*TILE_W - eye.x;
    y = row*TILE_H - eye.y;
/*    if (x < -TILEW || y < -TILEH || x > VIS_X || y > VIS_Y) return; */
    draw_sprite(b, Images[IMG_EMPTY], x, y);
    if (map[row][col].img) draw_sprite(b, Images[map[row][col].img], x, y);
}

void TLevel::add_object(TObject *p)
{
    objects->insert((void *) p);
    if (p->type == OBJ_PLAYER) player = (TPlayer *) p;
}

void TLevel::remove_object(TObject *p)
{
    delQ->insert((void *) p);
    if (p->id == OBJ_PLAYER) player = NULL;
}

void TLevel::draw(BITMAP *b)
{
    int i, j, row, col, row2, col2;
    TObject *p;

    col = eye.x / TILE_W;
    row = eye.y / TILE_H;
    col2 = col + VIS_X / TILE_W;
    row2 = row + VIS_Y / TILE_H;
    for (i = row; i <= row2; i++)
      for (j = col; j <= col2; j++)
        drawtile(b, i, j);

    for (i = 0; i < objects->count; i++) {
      p = (TObject *) objects->items[i];
      if (visible(p)) p->draw(b, p->pos.x - eye.x, p->pos.y - eye.y);
    }
}

void TLevel::run()
{
    int i;
    for (i = 0; i < objects->count; i++)
      ((TObject *) objects->items[i])->run();
    purge(delQ, objects, 0);
}
