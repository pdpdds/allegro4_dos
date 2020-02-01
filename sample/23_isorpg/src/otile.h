#ifndef _OTILE_H_
#define _OTILE_H_

#include "types.h"
#include "bitmap_ptr.h"
#include "itile.h"

// Implementation used to draw isometric tiles. An OTILE object is a bit like
// 1/4 of two different ITILEs.
class OTILE {
    public:
        // Orientation: always think left -> right
        // so  TOP_BOTTOM = \
        // and BOTTOM_TOP = /
        enum ORIENTATION { TOP_BOTTOM = 0, BOTTOM_TOP = 1 };
    public:
        static uint makeid(ITILE * isotile1, ITILE * isotile2, ORIENTATION o);
        static uint makeid(uint id1, uint id2, ORIENTATION o);
        static void splitid(uint id, uint * id1, uint * id2, ORIENTATION * o);
    public:
        OTILE();
        OTILE(uint id, DATAFILE * itilesdata);
        uint id() const { return myid; }
        void construct(uint id, DATAFILE * itilesdata);
        void draw(BITMAP * dest, int x, int y);
    protected:
        uint myid;
        BITMAP* myimage;
        BITMAP* myextra;
        // extra parts of tile to be drawn as sprites
        int myextra_xoff, myextra_yoff;
};

#endif

