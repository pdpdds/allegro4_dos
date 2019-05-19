#include "menu.h"

// Initilizer
menu::menu(){
  // Background image
  background = load_bitmap_ex( "images/menu.bmp");

  // Buffer
  buffer = create_bitmap( SCREEN_W, SCREEN_H);

  // Buttons
  friends_up = button( 90, 275, "/\\", font);
  friends_down = button( 90, 335, "\\/", font);
  enemies_up = button( 210, 275, "/\\", font);
  enemies_down = button( 210, 335, "\\/", font);
  width_up = button( 330, 275, "/\\", font);
  width_down = button( 330, 335, "\\/", font);
  height_up = button( 450, 275, "/\\", font);
  height_down = button( 450, 335, "\\/", font);
  bounce_up = button( 570, 275, "/\\", font);
  bounce_down = button( 570, 335, "\\/", font);
  start = button( 340, 485, "START", font);
}

// Update routine
void menu::update(){
  // Update buttons
  enemies_up.update();
  enemies_down.update();
  friends_up.update();
  friends_down.update();
  width_up.update();
  width_down.update();
  height_up.update();
  height_down.update();
  bounce_up.update();
  bounce_down.update();
  start.update();

  // Make teams
  if( enemies_up.clicked())
    game::num_enemies ++;
  if( enemies_down.clicked())
    game::num_enemies --;
  if( friends_up.clicked())
    game::num_friends ++;
  if( friends_down.clicked())
    game::num_friends --;
  if( width_up.clicked())
    game::map_width ++;
  if( width_down.clicked())
    game::map_width --;
  if( height_up.clicked())
    game::map_height ++;
  if( height_down.clicked())
    game::map_height --;
  if( bounce_up.clicked())
    tank::num_bullet_bounces ++;
  if( bounce_down.clicked())
    tank::num_bullet_bounces --;

  // Start game
  if( start.clicked())
    set_next_state( STATE_GAME);
}

// Drawing routine
void menu::draw(){
  // Background
  draw_sprite( buffer, background, 0, 0);

  // Buttons
  enemies_up.draw( buffer);
  enemies_down.draw( buffer);
  friends_up.draw( buffer);
  friends_down.draw( buffer);
  width_up.draw( buffer);
  width_down.draw( buffer);
  height_up.draw( buffer);
  height_down.draw( buffer);
  bounce_up.draw( buffer);
  bounce_down.draw( buffer);
  start.draw( buffer);

  // Player nums
  textprintf_centre_ex( buffer, font, 109, 315, makecol( 0, 0, 0), -1, "%i", game::num_friends);
  textprintf_centre_ex( buffer, font, 229, 315, makecol( 0, 0, 0), -1, "%i", game::num_enemies);
  textprintf_centre_ex( buffer, font, 349, 315, makecol( 0, 0, 0), -1, "%i", game::map_width);
  textprintf_centre_ex( buffer, font, 469, 315, makecol( 0, 0, 0), -1, "%i", game::map_height);
  textprintf_centre_ex( buffer, font, 589, 315, makecol( 0, 0, 0), -1, "%i", tank::num_bullet_bounces);

  // Mouse
  rectfill( buffer, mouse_x, mouse_y, mouse_x + 4, mouse_y + 4, makecol( 255, 255, 255));

  // Draw to screen
  draw_sprite( screen, buffer, 0, 0);
}
