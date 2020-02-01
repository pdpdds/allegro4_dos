#ifndef _TILE_H_
#define _TILE_H_

#include "terrain.h"

class TILE {
    public:
        TILE();
        ushort terrain_id() const { return myterrain_id; }
        uchar  transition_id() const {return mytrans_id; }
        void set_terrain_id(ushort newid) { myterrain_id = newid; }
        void set_transition_id(uchar newid) { mytrans_id = newid; }
    protected:
        ushort myterrain_id;
        uchar mytrans_id;
};

#endif
