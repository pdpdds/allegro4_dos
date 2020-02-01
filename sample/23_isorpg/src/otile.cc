#include "otile.h"

OTILE::OTILE(): myextra_xoff(0), myextra_yoff(0) 
{
    myextra = 0;
}


OTILE::OTILE(uint id, DATAFILE * itilesdata): myextra_xoff(0), myextra_yoff(0), myextra(0)
{
    construct(id, itilesdata);    
}

void OTILE::construct(uint id, DATAFILE * itilesdata) {
    if (itilesdata == NULL) 
        return;

    myid = id;

    OTILE::ORIENTATION o;
    uint id1, id2;
    splitid(id, &id1, &id2, &o);

    int tw = 16, th = 8;
    myimage = create_bitmap(tw, th);

    BITMAP * iimage1 = 0;
    BITMAP * iimage2 = 0;

    int i1w = 0, i1h = 0;
    int i2w = 0, i2h = 0;

    if (id1 != 0) {
        iimage1 = (BITMAP *)itilesdata[id1 - 1].dat;
        if (iimage1 != NULL) {
            i1w = iimage1->w;
            i1h = iimage1->h;
        }
    }
    if (id2 != 0) {
        iimage2 = (BITMAP *)itilesdata[id2 - 1].dat;
        if (iimage2 != NULL) {
            i2w = iimage2->w;
            i2h = iimage2->h;
        }
    }

    int ix1, iy1, ix2, iy2;
    if (o == BOTTOM_TOP) {
        ix1 = 0;  iy1 = i1h - 2*th;
        ix2 = tw; iy2 = i2h - th;
    } else if (o == TOP_BOTTOM) {
        ix1 = tw; iy1 = i1h - 2*th;
        ix2 = 0;  iy2 = i2h - th;
    }

    clear(myimage);
    if (iimage2 != NULL)
        blit(iimage2, myimage, ix2, iy2, 0, 0, tw, th);

    if (iimage1 != NULL)
        masked_blit(iimage1, myimage, ix1, iy1, 0, 0, tw, th);

    // add extra bit (sprite, drawn above otile), if it needs it

    int extrah = 0;
    if (iimage1 != NULL) {
        extrah = i1h - 2*th;
    }
    if (extrah > 0) {
        destroy_bitmap(myextra);
        myextra = create_bitmap(tw, extrah);
        clear(myextra);
        blit(iimage1, myextra, ix1, 0, 0, 0, tw, extrah);
        myextra_xoff = 0;
        myextra_yoff = -extrah;
    }

    /*
    clear(screen);

    if (iimage1 != NULL)
        draw_sprite(screen, iimage1, 10, 10);
    rect(screen, 9 + ix1, 9 + iy1, 10 + ix1 + tw, 10 + iy1 + th, 15);

    if (iimage2 != NULL)
        draw_sprite(screen, iimage2, 50, 10);
    rect(screen, 49 + ix2, 9 + iy2, 50 + ix2 + tw, 10 + iy2 + th, 15);

    rect(screen, 99, 9, 100 + tw, 10 + th, 15);
    draw_sprite(screen, myimage.get(), 100, 10);

    if (myextra != NULL)
        draw_sprite(screen, myextra.get(), 150, 10);
    rect(screen, 149, 9, 150 + tw, 10 + extrah, 15);

    textprintf_ex(screen, font, 10, 50, 15, -1, "id1, id2 = %d, %d", id1, id2);

    if (!key[KEY_SPACE])
        readkey();
    */
}


void OTILE::draw(BITMAP * dest, int x, int y) {
    //draw_sprite(dest, myimage.get(), x, y);
    blit(myimage, dest, 0, 0, x, y, myimage->w, myimage->h);
    if (myextra != 0)
        draw_sprite(dest, myextra, x + myextra_xoff, y + myextra_yoff);
}



uint OTILE::makeid(ITILE * itile1, ITILE * itile2, ORIENTATION o) {
    unsigned int id1 = 0, id2 = 0;

    if (itile1 != NULL)
        id1 = itile1->id();

    if (itile2 != NULL)
        id2 = itile2->id();

    return makeid(id1, id2, o);
}



uint OTILE::makeid(uint id1, uint id2, ORIENTATION o) {
    //printf("makeid(%d, %d, %d)\n", id1, id2, o);
    id1 &= 0xFFFF;
    id2 &= 0xFFFF;
    int obit = (int)o & 0x1;
    return (id2 << 17) | (id1 << 1) | obit;
}


void OTILE::splitid(uint id, uint * id1, uint * id2, ORIENTATION * o) {
    *o = (ORIENTATION)(id & 1);
    *id1 = (id >> 1) & 0xFFFF;
    *id2 = (id >> 17) & 0xFFFF;
}

