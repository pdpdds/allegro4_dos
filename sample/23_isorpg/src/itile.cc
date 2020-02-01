#include "itile.h"
#include "../datafile/tiles.h"

ITILE::ITILE(): myid(0), myx(0), myy(0), myw(32), myh(16) {
}

void ITILE::load(uint id, DATAFILE * d)
{
    if (d == NULL) 
        return;
    BITMAP * s = (BITMAP *)d[TILE000 + id].dat;
    if (s == NULL) {
        //printf("Can't find itile %d bitmap in datafile\n", id);
        return;
    }
    myid = id + 1;
}

