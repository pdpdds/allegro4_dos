#pragma once
#include"Armour.h"

class LightArmour : public Armour
{

public:
    LightArmour(BITMAP*, int, int, int, int, int, int);
    ~LightArmour();
    void Draw(BITMAP*);
    int GetHealthAddition();
};

