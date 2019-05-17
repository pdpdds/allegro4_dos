#include "Library.h"

class Prostokat
{
    int x;
    int y;
    int w;
    int h;
public:
    Prostokat(int x,int y,int w,int h);
    void draw(BITMAP *buffor);
    void move();
};
