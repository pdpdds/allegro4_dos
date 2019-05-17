#include<iostream>
#include"Plane.h"
#include"Bullet.h"




Bullet::Bullet(BITMAP* image, int spriteX, int spriteY, int frame_width, int frame_height, Point position, int type) : Plane(image, spriteX, spriteY, frame_width, frame_height, position, type)
{
    this->type = type;
    SetHealth(15);
    velx = 0;
    vely = -8.0f;
}

Bullet::Bullet(BITMAP* image, int spriteX, int spriteY, int frame_width, int frame_height, float x, float y, int type) : Plane(image,  spriteX, spriteY, frame_width, frame_height, x, y, type)
{
    this->type = type;
    SetHealth(15);
    velx = 0;
    vely = -8.0f;
    this->image = create_sub_bitmap(image, 0, 128, frame_width*3, frame_height);
    frameCounter = 0;
    box = new BoundingBox(position.x-frame_width/4, position.x+frame_width/4, position.y+frame_height/2 ,position.y-frame_height/2);
}

Bullet::~Bullet()
{
    delete box;
}


void Bullet::Move()
{
    if(this->type == 11)
    {
        //if bullet is of player
        Move(velx, -vely);
    }
    else if(this->type == 12)
    {
        //If Bullet is of enemy then Bullet moves down
        Move(velx, vely);
    }
}

void Bullet::Move(float x, float y)
{
    position.x = position.x + x;
    position.y = position.y + y;
    if(type == 12) {
        if(position.y < 0 - GetFrameHeight())
        SetAlive(false);
    }
    else if (type == 11) {
        if(position.y > SCREEN_H + GetFrameHeight())
        SetAlive(false);
    }

    box->Update(position.x-frame_width/4, position.x+frame_width/4, position.y+frame_height/2 ,position.y-frame_height/2);
}


void Bullet::Draw(BITMAP* buffer, bool debug)
{
    if(frameCounter%30 < 10)
        masked_blit(image, buffer, 0, 0, position.x-frame_width/2,position.y-frame_height/2, frame_width,frame_height);
    else if(frameCounter%30 >= 10 && frameCounter%30<20)
        masked_blit(image, buffer, 16, 0, position.x-frame_width/2,position.y-frame_height/2, frame_width,frame_height);
    else if(frameCounter%30 >= 20 && frameCounter%30<30)
        masked_blit(image, buffer, 32, 0, position.x-frame_width/2,position.y-frame_height/2, frame_width,frame_height);

    frameCounter++;

    box->Draw(buffer, debug);
}


