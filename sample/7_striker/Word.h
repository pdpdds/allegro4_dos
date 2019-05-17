#pragma once
#include<string>
#include"Point.h"
#include"Character.h"
#include"allegro.h"

class Word
{
private:
    Point position;
    Character* character;
    int length;
    int startx;
    int frame_width;
    int counter;
    int toDisplay;
public:
    Word();
    Word(std::string text, BITMAP* font_image, Point* position);
    ~Word();
    //void Initialize(std::string text, BITMAP* font_image, Point* position);
    void Draw(BITMAP* buffer, int animation, bool debug);
    void SetPosition(Point* position);
    int GetStringLength();
};

