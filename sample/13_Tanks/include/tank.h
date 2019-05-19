#ifndef TANK_H
#define TANK_H

#include "particle.h"
#include "bullet.h"
#include "barrier.h"
#include "powerup.h"
#include "world.h"
#include <allegro.h>

class tank{
  public:
    explicit tank( world *newWorld, int newX, int newY, int newHurtTime, int newHealth, int newFireSpeed, int newFireDelay, float newSpeed, BITMAP* newBaseImage, BITMAP* newTurretImage, BITMAP* newHurtImage, BITMAP* newTreadsImage);
    virtual ~tank();

    virtual bool isDead();

    virtual std::vector<bullet>* getBullets();

    virtual void update();
    virtual void draw( BITMAP* tempImage);
    virtual void putDecal( BITMAP* tempImage);

    virtual void giveHealth( int healthAmount);

    virtual int getX(){ return x; }
    virtual int getY(){ return y; }

    virtual int getCenterX(){ return x + width/2; }
    virtual int getCenterY(){ return y + height/2; }

    virtual int getHeight(){ return width; }
    virtual int getWidth(){ return height; }

    virtual void set_map_dimensions( int newMapWidth, int newMapHeight){ map_width = newMapWidth; map_height = newMapHeight;}

    virtual void checkCollision( std::vector<bullet>* newBullets);
    virtual void checkCollision( std::vector<barrier>* newBarriers);
    virtual void checkCollision( std::vector<powerup>* newPowerups);

    virtual void process_enemies( std::vector<tank*> *tempOtherTanks);

    virtual void get_powerup( int powerup_id);

    static unsigned char num_bullet_bounces;
  protected:
    float x;
    float y;

    int width, height;

    int hurt_timer;
    int health;
    int initialHealth;
    int fire_speed;
    int fire_delay_rate;
    int bullet_delay;

    int map_width, map_height;

    float rotation_radians_body;
    float rotation_allegro_body;
    float rotation_radians_turret;
    float rotation_allegro_turret;
    float vector_x;
    float vector_y;
    float speed, max_speed;

    bool dead;

    bool canMoveX;
    bool canMoveY;

    std::vector<bullet> bullets;
    std::vector<tank*> *otherTanks;

    BITMAP *image_base;
    BITMAP *image_hurt;
    BITMAP *image_top;
    BITMAP *image_treads;

    SAMPLE *sample_shot;

    world *worldPointer;

    // Update
    virtual void drive( float newRotation);
    virtual void shoot( float newRotation, float newX, float newY);
    virtual void update_bullets();
    virtual void update_timers();
    virtual void explode( int newX, int newY, int newVelocity, int newAmount, int newLife);

    // Draw
    virtual void drawBullets( BITMAP* tempImage);
    virtual void drawTankBase( BITMAP* tempImage);
    virtual void drawTankTurret( BITMAP* tempImage);
    virtual void drawHealthBar( BITMAP* tempImage, int newX, int newY, int newWidth, int newHeight, int newBorderWidth);
};

class player_tank: public tank{
  public:
    player_tank( world *newWorld, int newX, int newY, int newHurtTime, int newHealth, int newFireSpeed, int newFireDelay, float newSpeed, BITMAP* newBaseImage, BITMAP* newTurretImage, BITMAP* newHurtImage, BITMAP* newTreadsImage);
    virtual void update();
  protected:
};

class ai_tank: public tank{
  public:
    ai_tank( world *newWorld, int newX, int newY, int newHurtTime, int newHealth, int newFireSpeed, int newFireDelay, float newSpeed, BITMAP* newBaseImage, BITMAP* newTurretImage, BITMAP* newHurtImage, BITMAP* newTreadsImage);
    virtual void update();
  private:
    float destination_x;
    float destination_y;

    void update_target();
};

#endif // TANK_H
