#include"Tile.h"

Tile::Tile()
{
    frame_width = 128;
    frame_height = 128;
}

Tile::~Tile()
{
}


Tile::Tile(BITMAP* image, int frame_width, int frame_height, float x, float y, int tilePosition)
{
    this->frame_width = frame_width;
    this->frame_height = frame_height;
    this->image = image;
    this->position.x = x;
    this->position.y = y;
    if(tilePosition < 6)
    {
        tileY = 432;
    }
    else
    {
        tileY = 560;
    }
    tileX = 128 * (tilePosition%6);
}

void Tile::Initialize(BITMAP* image, int frame_width, int frame_height, float x, float y, int tilePosition)
{
    this->frame_width = frame_width;
    this->frame_height = frame_height;
    this->image = image;
    this->position.x = x;
    this->position.y = y;
    if(tilePosition < 6)
    {
        tileY = 432;
    }
    else
    {
        tileY = 560;
    }
    tileX = 128 * (tilePosition%6);
}

void Tile::Move(float x, float y)
{
    position.x = position.x + x;
    position.y = position.y + y;
}

void Tile::Draw(BITMAP* buffer)
{
    masked_blit(image, buffer, tileX, tileY, position.x, position.y, frame_width,frame_height);
}
