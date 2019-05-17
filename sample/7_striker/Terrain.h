#include"Point.h"
#include"Tile.h"
#include <allegro.h>

class Terrain
{
private:
    Point position;
    Tile* tile;
    int totaltiles;
    float moveFactor;
    BITMAP* terrainImage;
public:
    Terrain(BITMAP*, float x, float y, int startx, int starty);
    ~Terrain();
    void Move();
    void Draw(BITMAP*);
    Point* GetPosition();
};
