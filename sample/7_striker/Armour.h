#pragma once
#include<iostream>
#include"Point.h"
#include <allegro.h>


using namespace std;

class Armour
{
protected:
    BITMAP* image;
    Point position;
    int spriteX;
    int spriteY;
    int frame_width;
    int frame_height;
    int healthAddition;
public:
    Armour();
    Armour(BITMAP* image, int spriteX, int spriteY, int frame_width, int frame_height, Point& position);
    Armour(BITMAP* image, int spriteX, int spriteY, int frame_width, int frame_height, float, float);
    virtual ~Armour();
    virtual void Draw(BITMAP*) = 0;
    void SetPosition(Point*);
    void SetPosition(float, float);
    void Move(float, float);
    virtual int GetHealthAddition() = 0;
};
