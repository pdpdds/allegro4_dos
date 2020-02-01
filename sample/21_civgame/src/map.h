#ifndef _MAP_H_
#define _MAP_H_

#include "types.h"
#include "../data/tiles.h"
#include "tile.h"
#include <map>
#include <vector>
#include <string>
#include <allegro.h>


class MAP {
    public:
        MAP(uint w, uint h);
        void draw(BITMAP * dest, int x, int y);
        void load(const std::string& filename); // load a saved map from a file
        void generate(); // randomly generate a map
        void set_tile(uint x, uint y, TERRAIN& terrain);
        void set_tile(uint x, uint y, TILE * tile);
        TILE * get_tile(uint x, uint y);
    protected:
        void makefractal(int xstep, int ystep);
        void load_terrain(); // load terrain from a bitmap
    protected:
        TERRAIN mywater;
        TERRAIN myland;
        std::vector<TILE> mytilemap;
        int myx, myy;
        uint myw, myh;
        std::map<uint, TERRAIN> myterrains;
};


#endif

