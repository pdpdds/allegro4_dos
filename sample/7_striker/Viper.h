#include"Plane.h"

class Viper : public Plane
{
    STATE CURRENT_STATE;
    Object* target;
public:
    Viper(BITMAP* image, int spriteX, int spriteY, int frame_width, int frame_height, Point position, int type);
    Viper(BITMAP* image, int spriteX, int spriteY, int frame_width, int frame_height, float x, float y, int type);
    ~Viper();
    void Move();
    void SetTarget(Object*);
    void Move(float, float);
    void Draw(BITMAP*, bool);
};
