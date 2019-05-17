#include"Plane.h"
#include"Armour.h"

enum {LIGHT = 1, HEAVY};

class Player : public Plane
{
private:
    int maxHealth;
    Armour* armour;
    int currentArmour;
    BITMAP* bulletImage;
public:
    Player(BITMAP* image, int spriteX, int spriteY, int frame_width, int frame_height, Point position, int type);
    Player(BITMAP* image, int spriteX, int spriteY, int frame_width, int frame_height, float x, float y, int type);
    ~Player();
    void Draw(BITMAP* , bool );
    void CreateArmour(int);
    void SetPosition(Point* position);
    void SetPosition(float, float);
    void SetVelocityX(float);
    void SetVelocityY(float);
    void Move();
    void Move(Object*);
    void Move(float, float);
    Object* Fire();
};
