#include <iostream>
#include"HealthBar.h"
#include "Word.h"
#include <string>

using namespace std;
HealthBar::HealthBar(BITMAP* image, int x, int y)
{
    this->image=image;
    position.x = x;
    position.y = y;
    healthfactor = 100;
}


HealthBar::~HealthBar()
{

}
void HealthBar::Move(int damage)
{

    healthfactor = damage;
    if(healthfactor < 0){
        healthfactor = 0;
    }
}

void HealthBar::Draw(BITMAP* buffer)
{

    //std::string s = "health";
    Point* p = new Point;
    p->x = 0; p->y=0;
    Word* word = new Word("health",image,p);
    word->Draw(buffer, 0, false);
    masked_blit(image,buffer,192,128,position.x + 14,position.y + 40,healthfactor,16);
    masked_blit(image,buffer,288,128,position.x + 14,position.y + 40,96,16);

}



