#include "map.h"
#include <cstdio>
#include <ctime>
#include <iostream>

#ifdef _WIN32
#define random()     rand()
#define srandom(n)   srand(n)
#endif

MAP::MAP(uint w, uint h):
    myland("grass"),
    mywater("water"),
    mytilemap(w*h), 
    myx(0), myy(0),
    myw(w), myh(h) 
{
    load_terrain();
}



void MAP::load(const std::string& filename) {
}



void MAP::generate() {
    int i, j;
    int p = 4;

    srandom(time(0));

    for (j = 0; j != p; ++j) {
        for (i = 0; i != p; ++i) {
            if ((random() % 2) == 0)
                set_tile(myw*i/p, myh*j/p, mywater);
            else
                set_tile(myw*i/p, myh*j/p, myland);
        }
    }

    // recursive function to generate map
    makefractal(myw/(2*p), myh/(2*p));

    // smoooooth
    int c;
    for (j = 1; j != (myh - 1); ++j) {
        for (i = 1; i != (myw - 1); ++i) {
            c = 0;
            if (get_tile(i - 1, j)->terrain_id() == myland.id()) ++c;
            if (get_tile(i + 1, j)->terrain_id() == myland.id()) ++c;
            if (get_tile(i, j - 1)->terrain_id() == myland.id()) ++c;
            if (get_tile(i, j + 1)->terrain_id() == myland.id()) ++c;

            if ((get_tile(i, j)->terrain_id() == myland.id()) && (c < 2))
                set_tile(i, j, mywater);
        }
    }

    // use terrain transitions
    TILE * t;
    int id;
    uchar oid;
    uchar s;
    for (j = 0; j < myh; ++j) {
        for (i = 0; i < myw; ++i) {
            t = get_tile(i, j);
            if (t == NULL) continue;

            id = t->terrain_id();
            if (id == myland.id()) continue;

            oid = 0;
            s = 0;
            TILE *at;
            at = get_tile(i - 1, j    ); oid |= ((at && (at->terrain_id() == id)) << s++);
            at = get_tile(i    , j - 1); oid |= ((at && (at->terrain_id() == id)) << s++);
            at = get_tile(i + 1, j    ); oid |= ((at && (at->terrain_id() == id)) << s++);
            at = get_tile(i    , j + 1); oid |= ((at && (at->terrain_id() == id)) << s++);

            switch (oid) {
                case (0x0):
                    t->set_transition_id(17);
                    break;  // 0000
                case (0x1):
                    t->set_transition_id(14);
                    break;  // 0001
                case (0x2):
                    t->set_transition_id(16);
                    break;  // 0010
                case (0x4):
                    t->set_transition_id(13);
                    break;  // 0100
                case (0x8):
                    t->set_transition_id(15);
                    break;  // 1000

                case (0xE):
                    t->set_transition_id(1);
                    break;  // 1110  // left
                case (0xB):
                    t->set_transition_id(2);
                    break;  // 1011  // right
                case (0xD):
                    t->set_transition_id(3);
                    break;  // 1101  // up
                case (0x7):
                    t->set_transition_id(4);
                    break;  // 0111  // down

                case (0x3):
                    t->set_transition_id(12);
                    break;  // 0011
                case (0x6):
                    t->set_transition_id(11);
                    break;  // 0110
                case (0x9):
                    t->set_transition_id(10);
                    break;  // 1001
                case (0xC):
                    t->set_transition_id(9);
                    break;  // 1100
                default:
                    t->set_transition_id(0);
            };
        }
    }
}



void MAP::load_terrain() {
    myterrains.insert(std::map<uint, TERRAIN>::value_type(myland.id(), myland));
    myterrains.insert(std::map<uint, TERRAIN>::value_type(mywater.id(), mywater));
}



void MAP::makefractal(int xstep, int ystep) {
    //draw(screen, 0, 0);
    //readkey();

    for (int y = 0; y < myh; y += ystep) {
        for (int x = 0; x < myw; x += xstep) {
            // The inner loop calculates (cx,cy)
            // this is the point from which to copy map colour

            // add random offsets
            int cx = x + (((random() % 2) == 0) ? xstep : 0);
            int cy = y + (((random() % 2) == 0) ? ystep : 0);

            // truncate to nearest multiple of step*2
            // since step*2 is the previous detail level calculated
            cx = (cx/(xstep*2))*xstep*2;
            cy = (cy/(ystep*2))*ystep*2;

            // wrap around to reference world as torus
            // also guarantees getCell() and setCell() are within bounds
            cx = cx%myw;
            cy = cy%myh;

            // copy from (cx,cy) to (x,y)
            set_tile(x, y, get_tile(cx, cy));
        }
    }

    bool done = true;
    int xs = 1, ys = 1;

    if (xstep > 1) {
        xs = xstep/2;
        done = false;
    }
    if (ystep > 1) {
        ys = ystep/2;
        done = false;
    }

    if (!done)
        makefractal(xs, ys);
}


void MAP::set_tile(uint x, uint y, TERRAIN& terrain) {
    TILE * t = get_tile(x, y);
    if (t != NULL)
        t->set_terrain_id(terrain.id());
}


void MAP::set_tile(uint x, uint y, TILE * tile) {
    TILE * t = get_tile(x, y);
    if (t != NULL) {
        t->set_terrain_id(tile->terrain_id());
        t->set_transition_id(tile->transition_id());
    }
}


TILE * MAP::get_tile(uint x, uint y) {
    if ((x < myw) && (y < myh))
        return &mytilemap[myw*y + x];
    return NULL;
}



void MAP::draw(BITMAP * dest, int x, int y) {
    int i, j;
    /*
    for (i = 0; i != 18; ++i)
        blit(myterrains[0].image(i), dest, 0, 0, x + (i%9)*25, y + (i/9)*25, 24, 24);

    for (i = 0; i != 18; ++i)
        blit(myterrains[1].image(i), dest, 0, 0, x + (i%9)*25, y + (i/9)*25 + 60, 24, 24);
    */

    TILE * t;
    int sx = x;
    int sy = y;
    int ex = sx + dest->w/24;
    int ey = sy + dest->h/24;

    // map
    for (j = sy; j != ey; ++j) {
        for (i = sx; i != ex; ++i) {
            t = get_tile(i, j);
            if (t != NULL && t->terrain_id() < myterrains.size()) {
                TERRAIN& terrain = myterrains[t->terrain_id()];
                BITMAP * b = terrain.image(t->transition_id());
                blit(b, dest, 0, 0, (i - x)*24, (j - y)*24, 24, 24);
            }
        }
    }
    //clear(dest);

    // minimap
    set_trans_blender(0, 0, 0, 127);
    drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
    rectfill(dest, 1, 1, 1 + myw, 1 + myh, makecol(0, 0, 0));
    for (j = 0; j != myh; ++j) {
        for (i = 0; i != myw; ++i) {
            if (get_tile(i, j)->terrain_id() == 0)
                putpixel(dest, 1 + i, 1 + j, makecol(0, 255, 0));
            else if (get_tile(i, j)->terrain_id() == 1)
                putpixel(dest, 1 + i, 1 + j, makecol(0, 0, 255));
            else
                putpixel(dest, 1 + i, 1 + j, 0);
        }
    }
    rect(dest, 1 + x, 1 + y, 1 + x + dest->w/24, 1 + y + dest->h/24, makecol(255, 255, 255));
    drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
}

