#pragma once
#include <allegro.h>


struct BoundingBox
{
    float left;
    float right;
    float top;
    float bottom;

    int red;
    int green;
    int blue;

    BoundingBox(float left, float right, float top, float bottom)
    {
        this->left = left;
        this->right = right;
        this->top = top;
        this->bottom = bottom;
        red = rand() % (int)(255);
        green = rand() % (int)(255);
        blue = rand() % (int)(255);
    }
    void Draw(BITMAP* buffer, bool debug)
    {
        if(debug == true)
        {
            line(buffer, left, top, right, top, makecol(red,green,blue));
            line(buffer, right, top, right, bottom, makecol(red,green,blue));
            line(buffer, right, bottom, left, bottom, makecol(red,green,blue));
            line(buffer, left, bottom, left, top, makecol(red,green,blue));
        }
    }
    void Update(float left, float right, float top, float bottom)
    {
        this->left = left;
        this->right = right;
        this->top = top;
        this->bottom = bottom;
    }
};
