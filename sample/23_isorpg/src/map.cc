#include "../datafile/tiles.h"
#include "map.h"

#ifdef _WIN32
#define random()     rand()
#define srandom(n)   srand(n)
#endif

MAP::MAP(uint w, uint h):
    myiw(w),
    myih(h),
    myow(2*w),
    myoh(2*h),
    myimap(myiw*myih),
    myomap(myow*myoh, 0),
    mytilesdata(0)
{
    mytilesdata = load_datafile("datafile/tiles.dat");
    clear();
}


MAP::~MAP() {
    unload_datafile(mytilesdata);
}


void MAP::clear() {
    myotiles.clear();

    for (int j = 0; j != myih; ++j) {
        for (int i = 0; i != myiw; ++i) {
            int id = (random() % 2) + 1;
            if (id == 2)
                id = 3;
            myimap[j*myiw + i].load(id, mytilesdata);
        }
    }
}


bool MAP::valid_iposition(int x, int y) {
    return x >= 0 && y >= 0 && x < myiw && y < myih;
}


bool MAP::valid_oposition(int x, int y) {
    return x >= 0 && y >= 0 && x < myow && y < myoh;
}


void MAP::add_otile(uint id) {
    if (myotiles.find(id) == myotiles.end()) { // it isn't in there
        myotiles[id] = OTILE(id, mytilesdata);
    }
}


// Work out the tile list and link tiles from scratch.
void MAP::make_tiles() {
    clear();

    // Go through the isomap and build all needed OTILEs from it, linking them
    // to myomap appropriately.

    set_palette((RGB *)mytilesdata[_PAL].dat);

    uint id1, id2, id3, id4;
    bool found;
    int ox = myiw;
    int oy = 0;

    for (int j = 0; j != myih; ++j) {
        for (int i = 0; i != myiw; ++i) {
            ITILE * it = itile(i, j);

            if (it != NULL) {
                it->set_x(i);
                it->set_y(j);

                id1 = OTILE::makeid(it, itile(i - 1, j    ), OTILE::BOTTOM_TOP);
                id2 = OTILE::makeid(it, itile(i,     j - 1), OTILE::TOP_BOTTOM);
                id3 = OTILE::makeid(itile(i,     j + 1), it, OTILE::TOP_BOTTOM);
                id4 = OTILE::makeid(itile(i + 1, j    ), it, OTILE::BOTTOM_TOP);

                add_otile(id1);
                add_otile(id2);
                add_otile(id3);
                add_otile(id4);

                link_otile(ox + i - j - 1, oy + i + j, id1);
                link_otile(ox + i - j,     oy + i + j, id2);
                link_otile(ox + i - j - 1, oy + i + j + 1, id3);
                link_otile(ox + i - j,     oy + i + j + 1, id4);
            }
        }
    }
}


void MAP::link_otile(int x, int y, uint id) {
    if (valid_oposition(x, y)) {
        myomap[myow * y + x] = &myotiles[id];
    }
}


ITILE * MAP::itile(int x, int y) {
    if (valid_iposition(x, y))
        return &myimap[myiw * y + x];
    return NULL;
}

typedef std::pair<uint, OTILE> PAIR;

void MAP::draw(BITMAP * dest, int x, int y) {
    for (int j = 0; j != myoh; ++j) {
        for (int i = 0; i != myow; ++i) {
            int a = x + (i << 4); // i*16
            int b = y + (j << 3); // j*8
            if (a > -50 && a < (dest->w + 50) && b > -50 && b < (dest->h + 50)) {
                OTILE * ot = myomap[myow * j + i];
                if (ot) {
                    ot->draw(dest, a, b);
                }
            }
        }
    }
}
