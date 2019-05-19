#include "../include/tank.h"

unsigned char tank::num_bullet_bounces = 0;

/*****************
  General Tank
*****************/
tank::tank( world *newWorld, int newX, int newY, int newHurtTime, int newHealth, int newFireSpeed, int newFireDelay, float newSpeed, BITMAP* newBaseImage, BITMAP* newTurretImage, BITMAP* newHurtImage, BITMAP* newTreadsImage)
{
  x = newX;
  y = newY;

  hurt_timer = newHurtTime;
  health = newHealth;
  initialHealth = newHealth;
  fire_speed = newFireSpeed;

  fire_delay_rate = newFireDelay;
  max_speed = newSpeed;
  speed = 0;

  image_base = newBaseImage;
  image_hurt = newHurtImage;
  image_top = newTurretImage;
  image_treads = newTreadsImage;

  width = image_base -> w;
  height = image_base -> h;

  worldPointer = newWorld;

  // Map size
  map_width = SCREEN_W;
  map_height = SCREEN_H;

  if (image_base -> w < 1)
    abort_on_error( "Cannot find tank base\nPlease check your files and try again");
  if (image_hurt -> w < 1)
    abort_on_error( "Cannot find tank hurt\nPlease check your files and try again");
  if (image_top -> w < 1)
    abort_on_error( "Cannot find tank turret\nPlease check your files and try again");
  if (image_treads -> w < 1)
    abort_on_error( "Cannot find tank treads\nPlease check your files and try again");

  dead = false;
  
  sample_shot = load_sample_ex( "sfx/fire.wav");

  rotation_radians_body = 0;
  rotation_allegro_body = 0;
  rotation_radians_turret = 0;
  rotation_allegro_turret = 0;
}

// Delete
tank::~tank(){
  destroy_sample(sample_shot);
}

// Check dead
bool tank::isDead(){
  // Just died
  if( !dead && (health < 1)){
    explode( x + 25, y + 25, 10, 200, 20);
    play_sample( sample_shot, 255, 127, 500, 0);
  }

  // Set dead
  dead = (health < 1);

  return dead;
}

// Explode
void tank::explode( int newX, int newY, int newVelocity, int newAmount, int newLife){
  for( int i = 0; i < newAmount; i ++){
    particle *newParticle = new particle(newX, newY, makecol(255,random(0,255),0), -newVelocity, newVelocity, -newVelocity, newVelocity, 1, CIRCLE, newLife, EXPLODE);
    worldPointer -> addParticle(newParticle);
  }
}

// Get bullets
std::vector<bullet> *tank::getBullets(){
  return &bullets;
}

// Check collision
void tank::checkCollision( std::vector<bullet>* newBullets){
  for( unsigned int i = 0; i < newBullets -> size(); i++){
    if( collisionAny( x, x + 50, newBullets -> at(i).getX(), newBullets -> at(i).getX() + newBullets -> at(i).getXVelocity(), y, y + 50, newBullets -> at(i).getY(), newBullets -> at(i).getY() + newBullets -> at(i).getYVelocity())){
      health -= 10;
      newBullets -> at(i).destroy();
    }
  }
}
void tank::checkCollision( std::vector<barrier>* newBarriers){
  float guess_vector_x = -speed * cos( rotation_radians_body);
  float guess_vector_y = -speed * sin( rotation_radians_body);

  canMoveX = true;
  canMoveY = true;

  for( unsigned int i = 0; i < newBarriers -> size(); i++){
    if( collisionAny( x + 2 + guess_vector_x, x + width - 2 + guess_vector_x,
                     newBarriers -> at(i).getX(), newBarriers -> at(i).getX() + newBarriers -> at(i).getWidth(),
                     y + 2, y + height - 2,
                     newBarriers -> at(i).getY(), newBarriers -> at(i).getY() + newBarriers -> at(i).getHeight())){
      canMoveX = false;
    }
    if( collisionAny( x + 2, x + width - 2,
                     newBarriers -> at(i).getX(), newBarriers -> at(i).getX() + newBarriers -> at(i).getWidth(),
                     y + 2 + guess_vector_y, y + height - 2 + guess_vector_y,
                     newBarriers -> at(i).getY(), newBarriers -> at(i).getY() + newBarriers -> at(i).getHeight())){
      canMoveY = false;
    }
  }
}
void tank::checkCollision( std::vector<powerup>* newPowerups){
  for( unsigned int i = 0; i < newPowerups -> size(); i++){
    if( collisionAny( x, x + 50, newPowerups -> at(i).getX(), newPowerups -> at(i).getX() + newPowerups -> at(i).getWidth(), y, y + 50, newPowerups -> at(i).getY(), newPowerups -> at(i).getY() + newPowerups -> at(i).getHeight())){
      get_powerup( newPowerups -> at(i).getType());
      newPowerups -> at(i).pickup();
    }
  }
}

// Move around
void tank::drive( float newRotation){
  if( canMoveX){
    vector_x = -speed * cos( newRotation);
    x += vector_x;
  }
  if( canMoveY){
    vector_y = -speed * sin( newRotation);
    y += vector_y;
  }
}

// Update timers
void tank::update_timers(){
  // Change timers
  hurt_timer--;
  bullet_delay++;
}

// Update bullets
void tank::update_bullets(){
  // Update bullets
  for( unsigned int i = 0; i < bullets.size(); i++){
    bullets.at(i).update();
    if(bullets.at(i).getErase())
      bullets.erase(bullets.begin() + i);
  }

  // Erase bullets
  if( key[KEY_C] || joy[0].button[4].b){
    bullets.clear();
  }
}

// Shoot
void tank::shoot( float newRotation, float newX, float newY){
  if( bullet_delay > fire_delay_rate ){
    bool magicMODE = key[KEY_LSHIFT];

    bullet newBullet( worldPointer, newX, newY, newRotation, fire_speed, true, 1 + num_bullet_bounces + (magicMODE * 10), sample_shot);
    bullets.push_back( newBullet);
    bullet_delay = 0;
  }
}

// Update
void tank::update(){

}

// Draw bullets
void tank::drawBullets( BITMAP* tempImage){
  // Draw bullets
  for( unsigned int i = 0; i < bullets.size(); i++)
    bullets.at(i).draw( tempImage);
}

// Draw tank
void tank::drawTankBase( BITMAP* tempImage){
  // Hurt image for player
  if( health <= 0){
    rotate_sprite( tempImage, image_hurt, x, y, itofix(rotation_allegro_body));
  }
  else{
    rotate_sprite( tempImage, image_base, x, y, itofix(rotation_allegro_body));
  }
}

// Draw turret
void tank::drawTankTurret( BITMAP* tempImage){
  // Turret
  rotate_sprite( tempImage, image_top, x, y, itofix(rotation_allegro_turret));
}

// Draw health
void tank::drawHealthBar( BITMAP* tempImage, int newX, int newY, int newWidth, int newHeight, int newBorderWidth){
  // Health Bar
  rectfill( tempImage, newX, newY, newX + newWidth, newY + newHeight, makecol(0,0,0));
  rectfill( tempImage, newX + newBorderWidth, newY + newBorderWidth,
           newX + newWidth - newBorderWidth, newY + newHeight - newBorderWidth, makecol(255,0,0));
  rectfill( tempImage, newX + newBorderWidth ,newY + newBorderWidth,
           newX + (((float)health/(float)initialHealth) * newWidth) - newBorderWidth, newY + newHeight - newBorderWidth,makecol(0,255,0));
}

// Draw
void tank::draw( BITMAP* tempImage){
  // Tank
  drawTankBase( tempImage);

  // Bullets
  drawBullets( tempImage);

  // Turret
  if( !isDead()){
    drawTankTurret( tempImage);

    // Health bar
    if( health < initialHealth)
      drawHealthBar( tempImage, x - 5, y - 10, 50, 6, 1);
  }
}

// Put decals
void tank::putDecal( BITMAP* tempImage){
  if( !dead && speed > 0)
    rotate_sprite( tempImage, image_treads, x + width/2, y, itofix(rotation_allegro_body));
  else if( dead)
    drawTankBase( tempImage);
}

// Health
void tank::giveHealth( int healthAmount){
  health += healthAmount;
  if( health > initialHealth)
    health = initialHealth;
}

// Powerups
void tank::get_powerup( int powerup_id){
  if( powerup_id == 0){
    health += 10;
    if( health > 100)
      health = 100;
  }
  else if( powerup_id == 1){
    max_speed += 0.5;
  }
  else if( powerup_id == 2){
    fire_speed += 1;
  }
  else if( powerup_id == 3){
    fire_delay_rate -= 1;
    if( fire_delay_rate < 0)
      fire_delay_rate = 0;
  }
}


/*****************
   Player Tank
*****************/
// Init
player_tank::player_tank( world *newWorld, int newX, int newY, int newHurtTime, int newHealth, int newFireSpeed, int newFireDelay, float newSpeed, BITMAP* newBaseImage, BITMAP* newTurretImage, BITMAP* newHurtImage, BITMAP* newTreadsImage) :
      tank( newWorld, newX, newY, newHurtTime, newHealth, newFireSpeed, newFireDelay, newSpeed, newBaseImage, newTurretImage, newHurtImage, newTreadsImage){

}

// Update
void player_tank::update(){
  if( !isDead()){
    // Shoot
    rotation_radians_turret = find_angle( SCREEN_W/2, SCREEN_H/2, mouse_x, mouse_y); // find_angle( x + 25, y + 25, mouse_x, mouse_y);
    if( joy[0].stick[0].axis[0].pos != 0 || joy[0].stick[0].axis[1].pos != 0)
      rotation_radians_turret = find_angle( x + width/2 - 2, y + height/2 - 2, (joy[0].stick[0].axis[0].pos) + (x + 25), (joy[0].stick[0].axis[1].pos) + (y + 25));
    rotation_allegro_turret = rotation_radians_turret * 40.5845104792;


    if( key[KEY_SPACE] || mouse_b & 1 || joy[0].button[1].b){
      shoot( rotation_radians_turret, x + width/2 - 2, y + height/2 - 2);
    }

    // Rotate with keys
    if( key[KEY_A] || key[KEY_LEFT]){
      rotation_radians_body -= 0.03;
      rotation_allegro_body = rotation_radians_body * 40.5845104792;
    }
    if( key[KEY_D] || key[KEY_RIGHT]){
      rotation_radians_body += 0.03;
      rotation_allegro_body = rotation_radians_body * 40.5845104792;
    }

    // Drive
    drive( rotation_radians_body);

    if( mouse_b & 2 || joy[0].button[0].b || key[KEY_W] || key[KEY_UP]){
      if( mouse_b & 2){
        rotation_radians_body = find_angle( SCREEN_W/2, SCREEN_H/2, mouse_x, mouse_y); // find_angle( x + width/2, y + height/2, mouse_x, mouse_y);
      }
      else if( joy[0].button[0].b){
        rotation_radians_body = find_angle( x + width/2, y + height/2, (joy[0].stick[0].axis[0].pos) + (x + width/2), (joy[0].stick[0].axis[1].pos) + (y + height/2));
      }

      rotation_allegro_body = rotation_radians_body * 40.5845104792;

      // Accelerate
      if( speed == 0)
        speed = 0.2;
      else if( speed < max_speed)
        speed *= (max_speed * 1.03);
      else
        speed = max_speed;
    }
    else{
      // Decelerate
      if( speed > 0.1)
        speed *= 0.95;
      else
        speed = 0;

    }
  }

  // Update bullets
  update_timers();
  update_bullets();
}

// Feed AI player positions
void tank::process_enemies( std::vector<tank*>* tempOtherTanks){
  otherTanks = tempOtherTanks;
}

/*****************
    AI Tank
*****************/
// Init
ai_tank::ai_tank( world *newWorld, int newX, int newY, int newHurtTime, int newHealth, int newFireSpeed, int newFireDelay, float newSpeed, BITMAP* newBaseImage, BITMAP* newTurretImage, BITMAP* newHurtImage, BITMAP* newTreadsImage) :
      tank( newWorld, newX, newY, newHurtTime, newHealth, newFireSpeed, newFireDelay, newSpeed, newBaseImage, newTurretImage, newHurtImage, newTreadsImage){
  destination_x = x;
  destination_y = y;
}

// Update
void ai_tank::update(){
  if( !isDead()){
    // Rotate turret (at closest enemy)
    int best_enemy_x = destination_x;
    int best_enemy_y = destination_y;
    int target_enemy_index = 0;

    // Find nearest enemy and assign to target_enemy_index
    if( otherTanks -> size() > 0){
      int temp_enemy_x, temp_enemy_y;

      for( unsigned int i = 0; i < otherTanks -> size(); i++){
        temp_enemy_x = otherTanks -> at(i) -> getX() + otherTanks -> at(i) -> getWidth()/2;
        temp_enemy_y = otherTanks -> at(i) -> getY() + otherTanks -> at(i) -> getHeight()/2;

        best_enemy_x = otherTanks -> at(target_enemy_index) -> getX() + otherTanks -> at(target_enemy_index) -> getWidth()/2;
        best_enemy_y = otherTanks -> at(target_enemy_index) -> getY() + otherTanks -> at(target_enemy_index) -> getHeight()/2;

        if( find_distance( x, y, temp_enemy_x, temp_enemy_y) <
            find_distance( x, y, best_enemy_x, best_enemy_y)){
          target_enemy_index = i;
          best_enemy_x = otherTanks -> at(target_enemy_index) -> getX() + otherTanks -> at(target_enemy_index) -> getWidth()/2;
          best_enemy_y = otherTanks -> at(target_enemy_index) -> getY() + otherTanks -> at(target_enemy_index) -> getHeight()/2;
        }
      }

      rotation_radians_turret = find_angle( x + 25, y + 25, best_enemy_x, best_enemy_y);//randomf(-0.1,0.1);
      rotation_allegro_turret = rotation_radians_turret * 40.5845104792;

      // Shoot
      if( random(0,10) == 0 && find_distance( x, y, best_enemy_x, best_enemy_y) < 500){
        shoot( rotation_radians_turret, x + 23, y + 23);
      }
    }
    else{
      rotation_radians_turret = rotation_radians_body;
      rotation_allegro_turret = rotation_allegro_body;
    }

    // Path
    update_target();

    // Drive
    if( random(0,100)){
      rotation_radians_body = find_angle( x + 25, y + 25, destination_x, destination_y);
      rotation_allegro_body = rotation_radians_body * 40.5845104792;

      // Accelerate
      if( speed == 0)
        speed = 0.2;
      else if( speed < 1)
        speed *= 1.03;
      else
        speed = 1;

      drive( rotation_radians_body);
    }
    else{
      speed = 0;
    }
  }

  // Update bullets
  update_timers();
  update_bullets();
}

// Ai point choosing
void ai_tank::update_target(){
  if( find_distance(x + 25, y + 25, destination_x, destination_y) < 10 || (canMoveX == false && canMoveY == false)){
    destination_x = random( 0, map_width);
    destination_y = random( 0, map_height);
  }
}
