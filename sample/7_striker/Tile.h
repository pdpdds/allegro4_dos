#include"Point.h"
#include"Object.h"
#include <allegro.h>

class Tile
{
    Point position;
    BITMAP* image;
    int frame_width;
    int frame_height;
    int tileX;
    int tileY;
public:
    Tile();
    ~Tile();
    Tile(BITMAP*, int , int , float, float, int);
    void Initialize(BITMAP*, int , int , float, float, int);
    void Draw(BITMAP*);
    void Move(float, float);
};
