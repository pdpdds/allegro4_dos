#include"Terrain.h"
#include"math.h"

Terrain::Terrain(BITMAP* image, float x, float y, int startx, int starty)
{
    moveFactor = 0.3f;
    position.x = x;
    position.y = y;
    int numx = ceil(SCREEN_W/128) + 1;
    int numy = ceil(SCREEN_H/128) + 1;
    terrainImage = create_bitmap(numx * 128, numy * 128);
    totaltiles = numx * numy;

    tile = new Tile[totaltiles];

    int tilenum = 0;

    for(int i = 0; i< totaltiles; i++)
    {
        if(tilenum == numx)
        {
            starty = starty + 128;
            startx = 0;
            tilenum = 0;
        }
        tile[i].Initialize(image, 128, 128,tilenum*128,starty,(rand() % (int)(12)));
        tile[i].Draw(terrainImage);
        tilenum++;
    }
}
Terrain::~Terrain()
{
    delete[] tile;
    destroy_bitmap(terrainImage);
}
void Terrain::Move()
{
    /*
    for(int i = 0; i< totaltiles; i++)
    {
        tile[i].Move(0.0f,moveFactor);
    }*/
    position.y = position.y + moveFactor;
}

void Terrain::Draw(BITMAP* buffer)
{
    /*
    for(int i = 0; i< totaltiles; i++)
    {
        tile[i].Draw(terrainImage);
    }*/
    masked_blit(terrainImage, buffer, 0, 0, position.x, position.y, SCREEN_W,640);
}

Point* Terrain::GetPosition()
{
    return &position;
}
