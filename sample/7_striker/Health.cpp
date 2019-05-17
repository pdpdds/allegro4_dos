#include "Health.h"

Health::Health(BITMAP* image, int spriteX, int spriteY, int frame_width, int frame_height, Point position, int type) : Plane(image, spriteX, spriteY, frame_width, frame_height, position, type)
{

    this->type = type;
}

Health::~Health()
{
    //dtor
}

void Health::Draw(BITMAP* buffer, bool debug) {


}
