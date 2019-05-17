#include<iostream>
#include"Character.h"


Character::Character()
{
    frame_width = 32;
    frame_height = 32;
}

Character::Character(int frame_width, int frame_height)
{
    this->frame_width = frame_width;
    this->frame_height = frame_height;
}

Character::~Character()
{
    char n = character_value + 97;
    std::cout<<"\nCharacter "<<n<<" Destroyed";
}

void Character::Initialize(BITMAP* image, Point position, int character_value)
{
    this->image = image;
    this->position.x = position.x - frame_width/2;
    this->position.y = position.y - frame_height/2;
    this->character_value = character_value;
}

void Character::Initialize(BITMAP* image, int x, int y, int character_value)
{
    this->image = image;
    this->position.x = x - frame_width/2;
    this->position.y = y - frame_height/2;
    this->character_value = character_value;
}

void Character::Draw(BITMAP* buffer, bool debug)
{

    if(character_value < 13)

        masked_blit(image, buffer, character_value * frame_width, 336, position.x + 120, position.y + 20, frame_width,frame_height);
    else
        masked_blit(image, buffer, (character_value%13) * frame_width,368, position.x + 120, position.y + 20, frame_width,frame_height);
    if(debug == true)
    {
        line(buffer, position.x,position.y, position.x+frame_width,position.y, makecol(255, 0, 0));
        line(buffer, position.x+frame_width,position.y, position.x+frame_width,position.y+frame_height, makecol(255, 0, 0));
        line(buffer, position.x,position.y+frame_height, position.x+frame_width,position.y+frame_height, makecol(255, 0, 0));
        line(buffer, position.x,position.y+frame_height, position.x,position.y, makecol(255, 0, 0));
    }
}

void Character::SetPosition(Point* position)
{
    this->position.x = position->x - frame_width/2;
    this->position.y = position->y - frame_height/2;
}

void Character::SetPosition(int x, int y)
{
    this->position.x = x - frame_width/2;
    this->position.y = y - frame_height/2;
}

int Character::GetFrameWidth()
{
    return frame_width;
}
int Character::GetFrameHeight()
{
    return frame_height;
}
