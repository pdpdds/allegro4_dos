#pragma once
#include"Point.h"
#include <allegro.h>

class Character
{
private:
    Point position;
    BITMAP* image;
    int character_value;
    int frame_width;
    int frame_height;
public:
    Character();
    Character(int, int);
    ~Character();
    void Initialize(BITMAP* image, Point position, int character_value);
    void Initialize(BITMAP* image, int, int, int character_value);
    void Draw(BITMAP*, bool);
    void SetPosition(Point*);
    void SetPosition(int, int);
    int GetFrameWidth();
    int GetFrameHeight();
};
