#include<iostream>
#include <algorithm>
#include"Word.h"

Word::Word()
{

}

Word::Word(std::string text, BITMAP* font_image, Point* position)
{
    this->position.x = position->x;
    this->position.y = position->y;
    counter = 0;
    std::transform(text.begin(), text.end(), text.begin(), ::tolower);
    length = text.size();
    toDisplay = length;
    character = new Character[length];
    frame_width = character[0].GetFrameWidth();
    startx = position->x - (length*frame_width/2);

    for (int i=0; i<length; i++)
    {
        int val = text[i];
        val = val - 97;
        character[i].Initialize(font_image, startx, position->y, val);
        startx = startx + frame_width;
    }

}



Word::~Word()
{
    delete[] character;
}


void Word::Draw(BITMAP* buffer, int animation, bool debug)
{
    if(animation == 0)
    {
        for (int i=0; i<length; i++)
        {
            character[i].Draw(buffer, debug);
        }
    }
    else if(animation == 1)
    {
        for (int i=0; i<length - toDisplay; i++)
        {
            character[i].Draw(buffer, debug);
        }
        counter++;
        if(toDisplay>0)
        {
            if(counter == 300)
            {
                toDisplay--;
                counter = 0;
            }
        }
    }
    else if(animation == 2)
    {
        if(counter <300)
        {
            for (int i=0; i<length; i++)
            {
                character[i].Draw(buffer, debug);
            }
        }
        counter++;
        if(counter == 600)
        {
            counter = 0;
        }
    }
}

void Word::SetPosition(Point* position)
{
    this->position.x = position->x;
    this->position.y = position->y;

    startx = position->x - (length*frame_width/2);

    for (int i=0; i<length; i++)
    {
        character[i].SetPosition(startx, position->y);
        startx = startx + frame_width;
    }
}

int Word::GetStringLength()
{
    return length;
}
