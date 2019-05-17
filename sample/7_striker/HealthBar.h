#pragma once
#include"Point.h"
#include "Word.h"
#include <allegro.h>

class HealthBar
{
private:
    Point position;
    BITMAP* image;
    int healthfactor;
public:
    HealthBar(BITMAP*, int, int);
    ~HealthBar();
    void Move(int);
    void Draw(BITMAP*);
};
