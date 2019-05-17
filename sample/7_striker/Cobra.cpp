#include<iostream>
#include"Cobra.h"
#include "Bullet.h"


Cobra::Cobra(BITMAP* image, int spriteX, int spriteY, int frame_width, int frame_height, Point position, int type) : Plane(image, spriteX, spriteY, frame_width, frame_height, position, type)
{
    this->type = type;
    dodge = false;
    SetHealth(45);
    CURRENT_STATE = MIDDLE;
    velx = 1.25f;
    vely = 1.25f;
    this->BulletImage = image;
    this->image = create_sub_bitmap(image, 384, 0, frame_width*3, frame_height);
    box = new BoundingBox(position.x-frame_width*3/8, position.x+frame_width*3/8,position.y ,position.y+frame_height/2);
}

Cobra::Cobra(BITMAP* image, int spriteX, int spriteY, int frame_width, int frame_height, float x, float y, int type) : Plane(image, spriteX, spriteY, frame_width, frame_height, x, y, type)
{
    dodge = false;
    SetHealth(45);
    CURRENT_STATE = MIDDLE;
    velx = 1.25f;
    vely = 1.25f;
    this->image = create_sub_bitmap(image, 384, 0, frame_width*3, frame_height);
    box = new BoundingBox(position.x-frame_width*3/8, position.x+frame_width*3/8,position.y ,position.y+frame_height/2);
}

Cobra::~Cobra()
{
    delete box;
}


void Cobra::Move()
{
    if(target->GetBoundingBox()->left > this->box->right || target->GetBoundingBox()->right < this->box->left)
    {
        CURRENT_STATE = MIDDLE;
        velx = velx * 0.975f;
        Move(velx,vely);
        dodge = false;
    }
    else
    {
        if(dodge == false)
        {
            int random = (rand() % 2);
            if (random == 0)
            {
                CURRENT_STATE = RIGHT;
                velx = -2;
            }
            else
            {
                CURRENT_STATE = LEFT;
                velx = 2;
            }
            dodge = true;
        }


        Move(velx, vely);
    }
}

void Cobra::Move(float x, float y)
{
    position.x = position.x + x;
    position.y = position.y + y;
    if(position.y > SCREEN_H + GetFrameHeight())
        SetAlive(false);
    box->Update(position.x-frame_width*3/8, position.x+frame_width*3/8,position.y ,position.y+frame_height/2);
}

void Cobra::SetTarget(Object* object)
{
    target = object;
}

void Cobra::Draw(BITMAP* buffer, bool debug)
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

Object* Cobra::Fire() {
    //Enemy Bullet is of type 12
    Bullet* bullet = new Bullet(this->BulletImage, 0, 144, 16, 16, this->position.x, this->position.y - GetFrameHeight()/2,11);
    return bullet;
}


