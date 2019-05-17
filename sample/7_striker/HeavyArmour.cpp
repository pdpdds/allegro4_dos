#include"HeavyArmour.h"

HeavyArmour::~HeavyArmour()
{
    cout<<"\nHeavy Armour Destroyed";
}

HeavyArmour::HeavyArmour(BITMAP* image, int spriteX, int spriteY, int frame_width, int frame_height, int x, int y): Armour(image, spriteX, spriteY, frame_width, frame_height, x, y)
{
    healthAddition = 30;
}

void HeavyArmour::Draw(BITMAP* buffer)
{
    masked_blit(image, buffer, 128, 0, position.x, position.y, frame_width,frame_height);

}

int HeavyArmour::GetHealthAddition()
{
    return healthAddition;
}



