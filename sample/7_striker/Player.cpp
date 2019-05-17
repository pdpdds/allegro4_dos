#include"Player.h"
#include"Bullet.h"

Player::Player(BITMAP* image, int spriteX, int spriteY, int frame_width, int frame_height, Point position, int type) : Plane(image, spriteX, spriteY, frame_width, frame_height, position, type)
{
    bulletImage = image;
    maxHealth = 100;
    SetHealth(100);
    currentArmour = 0;
    box = new BoundingBox(position.x-frame_width*3/8, position.x+frame_width*3/8,position.y ,position.y+frame_height/2);
}

Player::Player(BITMAP* image, int spriteX, int spriteY, int frame_width, int frame_height, float x, float y, int type) : Plane(image, spriteX, spriteY, frame_width, frame_height, x, y, type)
{
    bulletImage = image;
    maxHealth = 100;
    SetHealth(100);
    currentArmour = 0;
    box = new BoundingBox(position.x-frame_width*3/8, position.x+frame_width*3/8,position.y ,position.y+frame_height/2);
}

Player::~Player()
{
    delete armour;
    delete box;
}

void Player::CreateArmour(int currentArmour)
{
    if(this->currentArmour > 0)
    {
        delete armour;
        maxHealth = 100;
    }
    this->currentArmour = currentArmour;
    if(currentArmour == LIGHT)
    {
        armour = new LightArmour(image, 64, 0, frame_width, frame_height, position.x, position.y);
    }
    else if(currentArmour == HEAVY)
    {
        armour = new HeavyArmour(image, 128, 0, frame_width, frame_height, position.x, position.y);
    }
    maxHealth = maxHealth + armour->GetHealthAddition();
    health = health + armour->GetHealthAddition();
}

void Player::Draw(BITMAP* buffer, bool debug)
{
    masked_blit(image, buffer, 0, 0, position.x-frame_width/2,position.y-frame_height/2, frame_width,frame_height);

    if (currentArmour > 0)
    {
        armour->Draw(buffer);
    }

    box->Draw(buffer, debug);
}

void Player::SetVelocityX(float vx)
{
    velx = vx;
}

void Player::SetVelocityY(float vy)
{
    vely = vy;
}

void Player::Move()
{
    velx = velx * 0.97f;
    vely = vely * 0.97f;

    Move(velx, vely);

}

void Player::Move(float x, float y)
{
    position.x = position.x + x;
    position.y = position.y + y;
    if(position.x<0)
    {
        position.x = 0;
    }

    if(position.x>SCREEN_W - GetFrameWidth())
    {
        position.x = SCREEN_W - GetFrameWidth();
    }

    if(position.y<0)
    {
        position.y = 0;
    }

    if(position.y>SCREEN_H - GetFrameHeight())
    {
        position.y = SCREEN_H - GetFrameHeight();
    }

    if(currentArmour>0)
    {
        armour->SetPosition(position.x, position.y);
    }
    box->Update(position.x-frame_width*3/8, position.x+frame_width*3/8,position.y ,position.y+frame_height/2);
}

void Player::SetPosition(Point* position)
{
    this->position.x = position->x - frame_width/2;
    this->position.y = position->y - frame_height/2;
    if(currentArmour>0)
    {
        armour->SetPosition(this->position.x, this->position.y);
    }
}

void Player::SetPosition(float x, float y)
{
    this->position.x = x - frame_width/2;
    this->position.y = y - frame_height/2;
    if(currentArmour>0)
    {
        armour->SetPosition(this->position.x, this->position.y);
    }
}

Object* Player::Fire()
{
    Bullet* bullet = new Bullet(this->bulletImage, 0, 144, 16, 16, this->position.x, this->position.y - GetFrameHeight()/2,12);
    return bullet;
}
