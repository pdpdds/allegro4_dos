#pragma once
#include "Plane.h"

class Health : public Plane
{
    public:
        Health(BITMAP* image, int spriteX, int spriteY, int frame_width, int frame_height, Point position, int type);
       ~Health();
        void Draw(BITMAP*, bool);

    protected:

    private:
        BITMAP* image;
        int type;
        int health;
};

