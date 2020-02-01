#ifndef _MAP_H_
#define _MAP_H_

#include <vector>
#include <map>
#include <allegro.h>

#include "itile.h"
#include "otile.h"
#include "types.h"

typedef 

class MAP {
    public:
        MAP(uint x, uint y);
        virtual ~MAP();
        void draw(BITMAP * dest, int x, int y);
        void make_tiles();
        void clear();
    protected:
        bool valid_iposition(int x, int y);
        ITILE * itile(int x, int y);
        bool valid_oposition(int x, int y);
        void link_otile(int x, int y, uint id);
        void add_otile(uint id);
    protected:
        uint myiw, myih, myow, myoh;
        std::vector<ITILE> myimap;
        std::vector<OTILE *> myomap;
        std::map<uint, OTILE> myotiles;
        DATAFILE * mytilesdata;
    private:
        MAP(const MAP&other); // no copying please (datafile ptr)
};

#endif

