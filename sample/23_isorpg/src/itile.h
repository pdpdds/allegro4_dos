#ifndef _ITILE_H_
#define _ITILE_H_

#include "types.h"
#include <allegro.h>

// Really just a wrapper for a terrain ID and a map position.
class ITILE {
    public:
        ITILE();
        void load(unsigned int id, DATAFILE * d);
        uint id() const { return myid; }
        int x() const { return myx; }
        int y() const { return myy; }
        void set_x(int x) { myx = x; }
        void set_y(int y) { myy = y; }
        uint w() const { return myw; }
        uint h() const { return myh; }
    protected:
        uint myid;
        int myx, myy;
        uint myw, myh;
};

#endif

