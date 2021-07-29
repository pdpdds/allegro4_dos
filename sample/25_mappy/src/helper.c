#include "helper.h"
#include <allegro.h>
#include "mappyal.h"

BITMAP* grabframe(BITMAP* source,
    int width, int height,
    int startx, int starty,
    int columns, int frame)
{
    BITMAP* temp = create_bitmap(width, height);
    int x = startx + (frame % columns) * width;
    int y = starty + (frame / columns) * height;
    blit(source, temp, x, y, 0, 0, width, height);
    return temp;
}

int collided(int x, int y)
{
    BLKSTR* blockdata;
    blockdata = MapGetBlock(x / mapblockwidth, y / mapblockheight);
    return blockdata->tl;
}