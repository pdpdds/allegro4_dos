#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "types.h"
#include <string>
#include <vector>
#include <allegro.h>

class TERRAIN {
    public:
        TERRAIN() {}
        ~TERRAIN();
        TERRAIN(const std::string& cfgfile);
        inline BITMAP * image(uchar trans_id) { return myimages[trans_id]; }
        inline ushort id() { return myid; }
    protected:
        ushort myid;
        std::vector<BITMAP*> myimages;
};


#endif
