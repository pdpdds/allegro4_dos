#include"Explosion.h"

Explosion::Explosion(BITMAP* image, int spriteX, int spriteY, int frame_width, int frame_height, float x, float y, int type) : Plane(image,  spriteX, spriteY, frame_width, frame_height, x, y, type)
{
    this->image = create_sub_bitmap(image, 0, 688, frame_width*6, frame_height);
    this->type = -100;
    position.x = x;
    position.y = y;
    frameCounter=0;
    alive = true;
    box = new BoundingBox(position.x-frame_width/4, position.x+frame_width/4, position.y+frame_height/2 ,position.y-frame_height/2);
}

Explosion::~Explosion()
{
}

void Explosion::Draw(BITMAP* buffer, bool debug)
{
    if(frameCounter%600 < 100)
    {
        masked_blit(image, buffer, 0, 0, position.x,position.y, frame_width,frame_height);
    }

    else if(frameCounter%600 >= 100 && frameCounter%600<200)
    {
        masked_blit(image, buffer, 46, 0, position.x,position.y, frame_width,frame_height);
    }

    else if(frameCounter%600 >= 200 && frameCounter%600<300)
    {
        masked_blit(image, buffer, 92, 0, position.x,position.y, frame_width,frame_height);
    }

    else if(frameCounter%600 >= 300 && frameCounter%600<400)
    {
        masked_blit(image, buffer, 138, 0, position.x,position.y, frame_width,frame_height);
    }

    else if(frameCounter%600 >= 400 && frameCounter%600<500)
    {
        masked_blit(image, buffer, 184, 0, position.x,position.y, frame_width,frame_height);
    }

    else if(frameCounter%600 >= 500 && frameCounter%600<590)
    {
        masked_blit(image, buffer, 230, 0, position.x,position.y, frame_width,frame_height);
    }

    else if(frameCounter%600 > 590 && frameCounter%600 <= 600)
    {
        alive = false;
    }

    frameCounter++;

    box->Draw(buffer, debug);
}

void Explosion::Move()
{

}
