#pragma once
#include"Point.h"
#include"Plane.h"
#include <allegro.h>

class Explosion : public Plane
{
    long int frameCounter;
public:
    Explosion(BITMAP* image, int spriteX, int spriteY, int frame_width, int frame_height, float x, float y, int type);
    ~Explosion();
    void Draw(BITMAP*, bool);
    void Move();
};

