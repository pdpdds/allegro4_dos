#include<iostream>
#include"Viper.h"


Viper::Viper(BITMAP* image, int spriteX, int spriteY, int frame_width, int frame_height, Point position, int type) : Plane(image, spriteX, spriteY, frame_width, frame_height, position, type)
{
    SetHealth(30);
    CURRENT_STATE = MIDDLE;
    velx = 1;
    vely = 1.5;
    this->image = create_sub_bitmap(image, 192, 0, frame_width*3, frame_height);
    box = new BoundingBox(position.x-frame_width/2, position.x+frame_width/2,position.y ,position.y+frame_height/2);
}

Viper::Viper(BITMAP* image, int spriteX, int spriteY, int frame_width, int frame_height, float x, float y, int type) : Plane(image, spriteX, spriteY, frame_width, frame_height, x, y, type)
{
    SetHealth(30);
    CURRENT_STATE = MIDDLE;
    velx = 1;
    vely = 1.5f;
    this->image = create_sub_bitmap(image, 192, 0, frame_width*3, frame_height);
    box = new BoundingBox(position.x-frame_width/2, position.x+frame_width/2,position.y ,position.y+frame_height/2);
}

Viper::~Viper()
{
    delete box;
}


void Viper::Move()
{
    if(this->position.x < target->GetPosition().x - target->GetFrameWidth()/2)
    {
        velx = 1;
        CURRENT_STATE = LEFT;
        Move(velx,vely);
    }
    else if(this->position.x > target->GetPosition().x + target->GetFrameWidth()/2)
    {
        velx = -1;
        CURRENT_STATE = RIGHT;
        Move(velx,vely);
    }
    else
    {
        CURRENT_STATE = MIDDLE;
        velx = velx * 0.975f;
        Move(velx, vely);
    }
}

void Viper::Move(float x, float y)
{
    position.x = position.x + x;
    position.y = position.y + y;
    if(position.y > SCREEN_H + GetFrameHeight())
        SetAlive(false);
    box->Update(position.x-frame_width/2, position.x+frame_width/2,position.y ,position.y+frame_height/2);
}

void Viper::SetTarget(Object* object)
{
    target = object;
}

void Viper::Draw(BITMAP* buffer, bool debug)
{

    if(CURRENT_STATE == LEFT)
        masked_blit(image, buffer, 128, 0, position.x-frame_width/2,position.y-frame_height/2, frame_width,frame_height);

    else if(CURRENT_STATE == MIDDLE)
        masked_blit(image, buffer, 64, 0, position.x-frame_width/2,position.y-frame_height/2, frame_width,frame_height);

    else if(CURRENT_STATE == RIGHT)
        masked_blit(image, buffer, 0, 0, position.x-frame_width/2,position.y-frame_height/2, frame_width,frame_height);


    //line(buffer, position.x,position.y, target->GetPosition().x,target->GetPosition().y, makecol(255, 0, 0));
    //rotate_sprite(buffer, image, 256, 0, position.y);
    box->Draw(image, debug);
}

