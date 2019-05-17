#pragma once
#include"Armour.h"

class HeavyArmour : public Armour
{

public:
    HeavyArmour(BITMAP*, int, int, int, int, int, int);
    ~HeavyArmour();
    void Draw(BITMAP*);
    int GetHealthAddition();
};


