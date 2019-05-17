#pragma once
#include <allegro.h>

class Bullet : public Plane
{
    long int frameCounter;
public:
    Bullet(BITMAP* image, int spriteX, int spriteY, int frame_width, int frame_height, Point position, int);
    Bullet(BITMAP* image, int spriteX, int spriteY, int frame_width, int frame_height, float x, float y, int);
    ~Bullet();
    void Move();
    void Move(float, float);
    void Draw(BITMAP*, bool);
};

