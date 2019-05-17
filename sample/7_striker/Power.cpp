#include <iostream>
#include "Power.h"

using namespace std;

Power::Power(BITMAP* img,Point p,int LowOrHigh)
{
    position.y = p.y;
    position.x = p.x;
    this->image = img;
    position = p;
    alive = true;
    type = 23 + LowOrHigh; //low high can be 1 or 2

}

Power::~Power()
{

}

void Power::Move()
{
    position.y = position.y +1;
    if(position.y>SCREEN_H)
        SetAlive(false);
}

void Power::Draw(BITMAP* buffer,bool debug)
{
    if(type == 22){
        masked_blit(image,buffer,32,733,position.x,position.y,32,32);
    }
    else if(type == 23){
        masked_blit(image,buffer,64,733,position.x,position.y,32,32);
    }
    else{
        masked_blit(image,buffer,0,733,position.x,position.y,32,32);
    }
}
