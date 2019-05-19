/**
 * GAME
 * Allan Legemaate
 * 20/08/2017
**/
#ifndef GAME_H
#define GAME_H
#include <time.h>
#include <vector>

#include "barrier.h"
#include "tools.h"
#include "tank.h"
#include "powerup.h"
#include "globals.h"
#include "world.h"

#include "state.h"

#include <math.h>
#include <allegro.h>

#define M_PI 3.14159265358979323846264338327

#define number_of_rays 200

// Coordinate system
struct coordinate{
  int x = 0;
  int y = 0;
};

class game : public state
{
  public:
    game();
    ~game();

    void update();
    void draw();

    // Map stuff
    static const unsigned char max_map_width = 255;
    static const unsigned char max_map_height = 255;

    static unsigned char map_width;
    static unsigned char map_height;

    static unsigned char num_enemies;
    static unsigned char num_friends;

  private:
    // Images
    BITMAP *buffer;
    BITMAP *map_buffer;
    BITMAP *decal_buffer;
    BITMAP *vision_buffer;
    BITMAP *background;
    BITMAP *cursor;
    BITMAP *blocks[3];
    BITMAP *powerup_images[4];
    BITMAP *tank_images[10];

    // World
    world game_world;

    // Objects
    std::vector<barrier> barriers;
    std::vector<tank*> enemy_tanks;
    std::vector<tank*> player_tanks;
    std::vector<powerup> powerups;
    std::vector<coordinate> startLocations;

    int map_temp[max_map_width][max_map_height];
    int map_x, map_y;

    int currentRound = 0;
};

#endif // GAME_H
