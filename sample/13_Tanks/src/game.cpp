#include "game.h"

unsigned char game::map_width = 20;
unsigned char game::map_height = 20;

unsigned char game::num_enemies = 5;
unsigned char game::num_friends = 5;

// Init state (and game)
game::game(){
  // Create buffers
  buffer = create_bitmap( SCREEN_W, SCREEN_H);

  decal_buffer = create_bitmap( map_width * 40, map_height * 40);
  clear_to_color( decal_buffer, 0xFF00FF);

  vision_buffer = create_bitmap( map_width * 40, map_height * 40);
  clear_to_color( vision_buffer, 0x000000);

  map_buffer = create_bitmap( map_width * 40, map_height * 40);
  clear_to_color( map_buffer, 0x000000);

  // Load images
  background = load_bitmap_ex( "images/back.bmp");
  cursor = load_bitmap_ex( "images/cursor.bmp");

  blocks[0] = load_bitmap_ex("images/res1.bmp");
  blocks[1] = load_bitmap_ex("images/res2.bmp");
  blocks[2] = load_bitmap_ex("images/res1.bmp");
  powerup_images[0] = load_bitmap_ex("images/res3.bmp");
  powerup_images[1] = load_bitmap_ex("images/res4.bmp");
  powerup_images[2] = load_bitmap_ex("images/res5.bmp");
  powerup_images[3] = load_bitmap_ex("images/res6.bmp");
  tank_images[0] = load_bitmap_ex("images/res7.bmp");
  tank_images[1] = load_bitmap_ex("images/res8.bmp");
  tank_images[2] = load_bitmap_ex("images/res9.bmp");
  tank_images[3] = load_bitmap_ex("images/res10.bmp");
  tank_images[4] = load_bitmap_ex("images/res11.bmp");
  tank_images[5] = load_bitmap_ex("images/res12.bmp");
  tank_images[6] = load_bitmap_ex("images/res13.bmp");
  tank_images[7] = load_bitmap_ex("images/res14.bmp"); 
  /*blocks[0] = load_bitmap_ex( "images/block_box_1.bmp");
  blocks[1] = load_bitmap_ex( "images/block_stone_1.bmp");
  blocks[2] = load_bitmap_ex( "images/block_box_1.bmp");
  powerup_images[0] = load_bitmap_ex( "images/powerup_health.bmp");
  powerup_images[1] = load_bitmap_ex( "images/powerup_tank_speed.bmp");
  powerup_images[2] = load_bitmap_ex( "images/powerup_bullet_speed.bmp");
  powerup_images[3] = load_bitmap_ex( "images/powerup_bullet_delay.bmp");
  tank_images[0] = load_bitmap_ex( "images/tank_treads.bmp");
  tank_images[1] = load_bitmap_ex( "images/tank_dead.bmp");
  tank_images[2] = load_bitmap_ex( "images/tank_turret_green.bmp");
  tank_images[3] = load_bitmap_ex( "images/tank_base_green.bmp");
  tank_images[4] = load_bitmap_ex( "images/tank_turret_red.bmp");
  tank_images[5] = load_bitmap_ex( "images/tank_base_red.bmp");
  tank_images[6] = load_bitmap_ex( "images/tank_turret_blue.bmp");
  tank_images[7] = load_bitmap_ex( "images/tank_base_blue.bmp");*/

  // Make a map
  for( unsigned char pass = 0; pass < 8; pass++){
    for( unsigned char i = 0; i < map_width; i++){
      for( unsigned char t = 0; t < map_height; t++){
        // Pass 0 (Erase)
        if( pass == 0){
          map_temp[i][t] = 0;
        }
        // Pass 1 (Edges)
        else if( pass == 1){
          if( i == 0 || t == 0 || i == map_width - 1 || t == map_height - 1){
            map_temp[i][t] = 1;
          }
        }
        // Pass 2 (Well Placed blocks)
        else if( pass == 2){
          if( map_temp[i - 1][t] == 0 && map_temp[i + 1][t] == 0 &&
                   map_temp[i - 1][t + 1] == 0 && map_temp[i + 1][t + 1] == 0 &&
                   map_temp[i - 1][t - 1] == 0 && map_temp[i + 1][t - 1] == 0 &&
                   map_temp[i][t - 1] == 0 && map_temp[i][t + 1] == 0 &&
                   random( 0, 2) == 1){
            map_temp[i][t] = 1;
          }
        }
        // Pass 3 (Filling)
        else if( pass == 3){
          if( (map_temp[i - 1][t] == 1 && map_temp[i + 1][t] == 1) ||
              (map_temp[i][t - 1] == 1 && map_temp[i][t + 1] == 1)){
            map_temp[i][t] = 1;
          }
        }
        // Pass 4 (Filling inaccessable areas)
        else if( pass == 4){
          if( map_temp[i - 1][t] == 1 && map_temp[i + 1][t] == 1 &&
              map_temp[i][t - 1] == 1 && map_temp[i][t + 1] == 1){
            map_temp[i][t] = 1;
          }
        }
        // Pass 5 (Boxes!)
        else if( pass == 5){
          if( map_temp[i][t] == 0 && random( 1, 20) == 1){
            map_temp[i][t] = 2;
          }
        }
        // Pass 6 (Find start locations)
        else if( pass == 6){
          if( map_temp[i][t] == 0){
            coordinate newStartLocation;
            newStartLocation.x = i * 40;
            newStartLocation.y = t * 40;
            startLocations.push_back( newStartLocation);
          }
        }
        // Pass 7 (create barriers)
        else if( pass == 7){
          if( map_temp[i][t] == 1 || map_temp[i][t] == 2 || map_temp[i][t] == 3){
            barrier newBarrier( &game_world, i * 40, t * 40, blocks[map_temp[i][t]], 0);

            // Destroyable
            if( map_temp[i][t] == 2)
              newBarrier.setHealth(3);
            else if( i != 0 && t != 0 && i != map_width - 1 && t != map_height - 1)
              newBarrier.setHealth(20);
            else
              newBarrier.setIndestructable(true);


            barriers.push_back( newBarrier);
          }
        }
      }
    }
  }  
  // Player
  int randomStartLocation = random( 0, startLocations.size() - 1);
  player_tank *newPlayer = new player_tank( &game_world, startLocations.at( randomStartLocation).x, startLocations.at( randomStartLocation).y, 3,
                          100, 4, 20, 1,
                          tank_images[3], tank_images[2], tank_images[1], tank_images[0]);

  newPlayer -> process_enemies( &enemy_tanks);
  newPlayer -> set_map_dimensions( map_width * 40, map_height * 40);
  player_tanks.push_back( newPlayer);  

  // Enemies
  for( unsigned char i = 0; i < num_enemies; i ++){
    int randomStartLocation = random( 0, startLocations.size() - 1);
    ai_tank *newPlayer = new ai_tank( &game_world, startLocations.at( randomStartLocation).x, startLocations.at( randomStartLocation).y, 3,
                      random(50,150), random(1,4), random(50,300), random(1,10)/10,
                      tank_images[5], tank_images[4], tank_images[1], tank_images[0]);

    newPlayer -> process_enemies( &player_tanks);
    newPlayer -> set_map_dimensions( map_width * 40, map_height * 40);
    enemy_tanks.push_back( newPlayer);
  }  

  // Friends
  for( unsigned char i = 0; i < num_friends; i ++){
    int randomStartLocation = random( 0, startLocations.size() - 1);
    ai_tank *newPlayer = new ai_tank( &game_world, startLocations.at( randomStartLocation).x, startLocations.at( randomStartLocation).y, 3,
                          100, 4, 20, 1,
                          tank_images[7], tank_images[6], tank_images[1], tank_images[0]);

    newPlayer -> process_enemies( &enemy_tanks);
    newPlayer -> set_map_dimensions( map_width * 40, map_height * 40);
    player_tanks.push_back( newPlayer);	
  }
}

// Clean up
game::~game(){
  destroy_bitmap(buffer);
  destroy_bitmap(decal_buffer);
  destroy_bitmap(vision_buffer);
  destroy_bitmap(map_buffer);

  destroy_bitmap(background);
  destroy_bitmap(cursor);

  destroy_bitmap(blocks[0]);
  destroy_bitmap(blocks[1]);
  destroy_bitmap(blocks[2]);

  for( int i = 0; i < 4; i++)
    destroy_bitmap(powerup_images[i]);

  for( int i = 0; i < 8; i++)
    destroy_bitmap(tank_images[i]);
}


void game::update(){
  // Get joystick input
  poll_joystick();

  // Update world
  game_world.update();
  
  // Move
  for( unsigned int i = 0; i < enemy_tanks.size(); i++){
    // Update barriers
    for( unsigned int t = 0; t < barriers.size(); t++)
      barriers.at(t).update( enemy_tanks.at(i) -> getBullets());

    // Update bullets
    for( unsigned int t = 0; t < player_tanks.size(); t++)
      player_tanks.at(t) -> checkCollision( enemy_tanks.at(i) -> getBullets());

    // Collision with barrier
    enemy_tanks.at(i) -> checkCollision( &barriers);

    // Collision with powerups
    enemy_tanks.at(i) -> checkCollision( &powerups);

    // Update tanks
    enemy_tanks.at(i) -> update();

    // Delete tank
    if(enemy_tanks.at(i) -> isDead()){
      //enemy_tanks.at(i) -> putDecal( decal_buffer);
      delete enemy_tanks[i];
      enemy_tanks.erase(enemy_tanks.begin() + i);
    }
  }
  for( unsigned int i = 0; i < player_tanks.size(); i++){
    // Update barriers
    for( unsigned int t = 0; t < barriers.size(); t++)
      barriers.at(t).update( player_tanks.at(i) -> getBullets());

    // Update bullets
    for( unsigned int t = 0; t < enemy_tanks.size(); t++)
      enemy_tanks.at(t) -> checkCollision( player_tanks.at(i) -> getBullets());

    // Collision with barrier
    player_tanks.at(i) -> checkCollision( &barriers);

    // Collision with powerups
    player_tanks.at(i) -> checkCollision( &powerups);

    // Update tanks
    player_tanks.at(i) -> update();

    // Delete tank
    if( player_tanks.at(i) -> isDead()){
      player_tanks.at(i) -> putDecal( decal_buffer);
      delete player_tanks[i];
      player_tanks.erase(player_tanks.begin() + i);
    }
  }

  // Remove broken barriers
  for( unsigned int i = 0; i < barriers.size(); i++){
    if( barriers.at(i).getDead()){
      // Spawn powerup
      if( random( 0, 1) == 0){
        int type = random( 0, 3);
        powerup newPowerup( barriers.at(i).getX(), barriers.at(i).getY(), type, powerup_images[type]);
        powerups.push_back( newPowerup);
      }

      barriers.erase( barriers.begin() + i);
    }
  }

  // Delete powerup
  for( unsigned int i = 0; i < powerups.size(); i++){
    if(powerups.at(i).getDead()){
      powerups.erase(powerups.begin() + i);
    }
  }

  // Game over
  if( key[KEY_SPACE] && (player_tanks.size() == 0 || enemy_tanks.size() == 0)){
    set_next_state( STATE_MENU);
  }

  // Scroll map
  if( player_tanks.size() > 0){
    map_x = player_tanks.at(0) -> getCenterX() - buffer -> w / 2;
    map_y = player_tanks.at(0) -> getCenterY() - buffer -> h / 2;
  }


  // Vision buffer
  // Check collision with all boxes!
  /*for( unsigned int i = 0; i < barriers.size(); i++){
    barriers.at(i).visible = false;
  }

  for( double q = 0; q < 2 * M_PI; q += ((2 * M_PI) / number_of_rays)){
    float point_x = (SCREEN_W * 2) * cos(q) + player_tanks.at(0) -> getCenterX();
    float point_y = (SCREEN_H * 2) * sin(q) + player_tanks.at(0) -> getCenterY();

    // Closest POI
    float poi_x = point_x;
    float poi_y = point_y;

    bool intersection_found = false;
    int closest_index = -1;

    for( unsigned int i = 0; i < barriers.size(); i++){
      for( int t = 0; t < 4; t++){
        float temp_poi_x = -1;
        float temp_poi_y = -1;

        float bar_x_1 = 0, bar_x_2 = 0, bar_y_1 = 0, bar_y_2 = 0;

        // TOP
        if( t == 0){
          bar_x_2 = barriers.at(i).getWidth();
        }
        // RIGHT
        if( t == 1){
          bar_x_1 = barriers.at(i).getWidth();
          bar_x_2 = barriers.at(i).getWidth();
          bar_y_2 = barriers.at(i).getHeight();
        }
        // BOTTOM
        if( t == 2){
          bar_x_2 = barriers.at(i).getWidth();
          bar_y_1 = barriers.at(i).getHeight();
          bar_y_2 = barriers.at(i).getHeight();
        }
        // LEFT
        if( t == 3){
          bar_y_2 = barriers.at(i).getHeight();
        }

        // Check if ray and side intersect
        if( get_line_intersection( player_tanks.at(0) -> getCenterX(), player_tanks.at(0) -> getCenterY(), point_x, point_y,
                                   barriers.at(i).getX() + bar_x_1, barriers.at(i).getY() + bar_y_1,
                                   barriers.at(i).getX() + bar_x_2, barriers.at(i).getY() + bar_y_2,
                                   &temp_poi_x, &temp_poi_y)){
          // Check if closer match found and if so update POI
          if( distanceTo2D( temp_poi_x, temp_poi_y, player_tanks.at(0) -> getCenterX(), player_tanks.at(0) -> getCenterY()) <
              distanceTo2D( poi_x, poi_y, player_tanks.at(0) -> getCenterX(), player_tanks.at(0) -> getCenterY())){
            poi_x = temp_poi_x;
            poi_y = temp_poi_y;
            intersection_found = true;
            closest_index = i;
          }
        }
      }
    }

    // Draw line to closest collision
    if( intersection_found){
      //line( vision_buffer, player_tanks.at(0) -> getCenterX(), player_tanks.at(0) -> getCenterY(), poi_x, poi_y, makecol( 255, 255, 255));
      barriers.at(closest_index).visible = true;
    }

    // Draw intersection if there is one
    if( intersection_found && key[KEY_C])
      ellipse( vision_buffer, poi_x, poi_y, 5, 5, makecol( 255, 0, 0));
  }*/
}


void game::draw(){
  // Draw background
  draw_sprite( buffer, background, 0, 0);

  // Blank map map_buffer
  rectfill( map_buffer, 0, 0, map_buffer -> w, map_buffer -> h, makecol( 0, 88, 0));

  // Decal to buffer
  draw_sprite( map_buffer, decal_buffer, 0, 0);

  // Draw tanks
  for( unsigned int i = 0; i < enemy_tanks.size(); i++){
    enemy_tanks.at(i) -> draw( map_buffer);
    enemy_tanks.at(i) -> putDecal( decal_buffer);
  }
  for( unsigned int i = 0; i < player_tanks.size(); i++){
    player_tanks.at(i) -> draw( map_buffer);
    player_tanks.at(i) -> putDecal( decal_buffer);
  }

  // Draw world
  game_world.draw( map_buffer);

  // Draw barriers
  for( unsigned int i = 0; i < barriers.size(); i++)
    barriers.at(i).draw( map_buffer);

  // Vision buffer
  //draw_sprite( map_buffer, vision_buffer, 0, 0);
  //clear_to_color( vision_buffer, 0xFF00FF);

  // Draw powerups
  for( unsigned int i = 0; i < powerups.size(); i++)
    powerups.at(i).draw( map_buffer);

  // Map to buffer
  blit( map_buffer, buffer, map_x, map_y, 0, 0, buffer -> w, buffer -> h);

  // Text
  textprintf_ex( buffer, font, 20, 20, makecol(0,0,0), makecol(255,255,255), "Round: %i", currentRound);
  textprintf_ex( buffer, font, 20, 35, makecol(0,0,0), makecol(255,255,255), "Team BLUE: %i", player_tanks.size());
  textprintf_ex( buffer, font, 20, 50, makecol(0,0,0), makecol(255,255,255), "Team RED: %i", enemy_tanks.size());

  // Cursor
  draw_sprite( buffer, cursor, mouse_x - 10, mouse_y - 10);

  // Buffer to screen
  draw_sprite( screen, buffer, 0, 0);
}
