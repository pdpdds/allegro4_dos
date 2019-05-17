#pragma once
#include "Point.h"
#include "Plane.h"
#include <allegro.h>

class Power : public Plane
{
private:
    BITMAP* image;
    Point position;
    int PowerUp;
public:
    Power(BITMAP*,Point,int);
    ~Power();
    void Draw(BITMAP*,bool);
    void Move();
};
