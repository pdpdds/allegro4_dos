#include"Plane.h"




Plane::Plane()
{
    frame_width = 64;
    frame_height = 64;
    health = 100;
}

Plane::~Plane()
{
}

Plane::Plane(BITMAP* image, int spriteX, int spriteY, int frame_width, int frame_height, Point position, int type)
{
    this->frame_width = frame_width;
    this->frame_height = frame_height;
    this->image = create_sub_bitmap(image, spriteX, spriteY, frame_width, frame_height);
    this->position.x = position.x;
    this->position.y = position.y;
    this->type = type;
    health = 100;
    alive = true;
}

Plane::Plane(BITMAP* image, int spriteX, int spriteY, int frame_width, int frame_height, float x, float y, int type)
{
    this->frame_width = frame_width;
    this->frame_height = frame_height;
    this->image = create_sub_bitmap(image, spriteX, spriteY, frame_width, frame_height);
    this->position.x = x;
    this->position.y = y;
    this->type = type;
    health = 100;
    alive = true;
}


Point Plane::GetPosition()
{
    return position;
}

void Plane::SetPosition(Point* position)
{
    this->position.x = position->x;
    this->position.y = position->y;
}

void Plane::SetPosition(float x, float y)
{
    this->position.x = x;
    this->position.y = y;
}


void Plane::Move(float x, float y)
{
    position.x = position.x + x;
    position.y = position.y + y;
}

int Plane::GetFrameWidth()
{
    return frame_width;
}

int Plane::GetFrameHeight()
{
    return frame_height;
}

void Plane::SetAlive(bool alive)
{
    this->alive = alive;
}

bool Plane::Alive()
{
    return alive;
}

void Plane::SetHealth(int health)
{
    this->health = health;
}

int Plane::GetHealth()
{
    return health;
}

void Plane::TakeDamage(int damage)
{
    this->health = health - damage;
    if(health<=0)
        alive = false;
}

int Plane::GetType()
{
    return type;
}


void Plane::SetVelocityX(float)
{

}
void Plane::SetVelocityY(float)
{

}

BoundingBox* Plane::GetBoundingBox()
{
    return box;
}

