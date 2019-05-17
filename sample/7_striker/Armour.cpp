#include"Armour.h"


Armour::Armour()
{
    frame_width = 64;
    frame_height = 64;
}

Armour::~Armour()
{
    cout<<"\nNo Armour Destroyed";
}

Armour::Armour(BITMAP* image, int spriteX, int spriteY, int frame_width, int frame_height, Point& position)
{
    this->frame_width = frame_width;
    this->frame_height = frame_height;
    this->image = image;
    this->position.x = position.x;
    this->position.y = position.y;
    this->spriteX = spriteX;
    this->spriteY = spriteY;
}

Armour::Armour(BITMAP* image, int spriteX, int spriteY, int frame_width, int frame_height, float x, float y)
{
    this->frame_width = frame_width;
    this->frame_height = frame_height;
    this->image = image;
    this->spriteX = spriteX;
    this->spriteY = spriteY;
    this->position.x = x;
    this->position.y = y;
}

void Armour::SetPosition(Point* position)
{
    this->position.x = position->x ;
    this->position.y = position->y ;
}

void Armour::SetPosition(float x, float y)
{
    this->position.x = x ;
    this->position.y = y ;
}

void Armour::Move(float x, float y)
{
    position.x = position.x + x;
    position.y = position.y + y;
}


