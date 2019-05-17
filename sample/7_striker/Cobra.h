#include"Plane.h"

class Cobra : public Plane
{
    STATE CURRENT_STATE;
    Object* target;
    bool dodge;
    BITMAP* BulletImage;
public:
    Cobra(BITMAP* image, int spriteX, int spriteY, int frame_width, int frame_height, Point position, int type);
    Cobra(BITMAP* image, int spriteX, int spriteY, int frame_width, int frame_height, float x, float y, int type);
    ~Cobra();
    void Move();
    void SetTarget(Object*);
    void Move(float, float);
    void Draw(BITMAP*, bool);
    Object* Fire();
};

