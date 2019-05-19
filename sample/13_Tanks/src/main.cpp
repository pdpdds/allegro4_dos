#include "mouseListener.h"
#include "keyListener.h"

#include "state.h"
#include "init.h"
#include "menu.h"
#include "game.h"
#include <allegro.h>

// Input listener classes
mouseListener m_listener;
keyListener k_listener;

// Current state object
state *currentState = 0;

// Are we closing?
bool closing = false;

// FPS Tickers
volatile int ticks = 0;
int fps;
int frames_done;
int old_time;
const int updates_per_second = 120;
int frames_array[10];
int frame_index = 0;

void ticker(){
	ticks++;
}
END_OF_FUNCTION(ticker)

volatile int game_time = 0;
void game_time_ticker(){
	game_time++;
}
END_OF_FUNCTION(game_time_ticker)


// Close button handler
void close_button_handler( void){
  closing = true;
}
END_OF_FUNCTION( close_button_handler)

// Delete game state and free state resources
void clean_up(){
  delete currentState;
}

// Change game screen
void change_state(){
  //If the state needs to be changed
  if( nextState != STATE_NULL ){
    //Delete the current state
    if( nextState != STATE_EXIT ){
      delete currentState;
    }

    //Change the state
    switch( nextState ){
      case STATE_INIT:
        currentState = new init();
        break;
      case STATE_GAME:
        currentState = new game();
        break;
      case STATE_MENU:
        currentState = new menu();
        break;
      case STATE_EXIT:
        closing = true;
        break;
      default:
        currentState = new game();
    }

    //Change the current state ID
    stateID = nextState;

    //NULL the next state ID
    nextState = STATE_NULL;
  }
}

// Calibrate joystick
void calibrateJoystick(){
  for( int i = 0; i < num_joysticks; i ++){
    while (joy[i].flags & JOYFLAG_CALIBRATE) {
      //AL_CONST char *msg = calibrate_joystick_name(i);

      if ((readkey()&0xFF) == 27)
        exit(0);

      if (calibrate_joystick(i) != 0) {
         set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
         allegro_message("Error calibrating joystick!\n");
         exit(1);
      }
    }
    if (!(joy[i].stick[0].flags & JOYFLAG_ANALOGUE))
      allegro_message("This game only supports analogue joysticks, please unplug and try again. \n");
  }

  save_joystick_data("joy_config.dat");
}

// Setup game
void setup(){
  // Init Allegro
  allegro_init();
  //alpng_init();
  install_timer();
  install_keyboard();
  install_mouse();

  // Setup joystick
  if( !load_joystick_data("joy_config.dat")){
    install_joystick(JOY_TYPE_AUTODETECT);
    calibrateJoystick();
  }

  set_color_depth(32);

  // Setup screen
#ifdef _WIN32
  set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0);
  install_sound( DIGI_AUTODETECT, MIDI_AUTODETECT, ".");
#else
  set_gfx_mode(GFX_AUTODETECT, 800, 600, 0, 0);
#endif

  // Window Title
  set_window_title( "Tanks!");

  // Create random number generator
  srand( time( NULL));

   // Setup for FPS system
  LOCK_VARIABLE( ticks);
  LOCK_FUNCTION( ticker);
  install_int_ex( ticker, BPS_TO_TIMER( updates_per_second));

  LOCK_VARIABLE( game_time);
  LOCK_FUNCTION( game_time_ticker);
  install_int_ex( game_time_ticker, BPS_TO_TIMER(10));

  // FPS STUFF
  for(int i = 0; i < 10; i++)
    frames_array[i] = 0;

  // Close button
  LOCK_FUNCTION( close_button_handler);
  set_close_button_callback( close_button_handler);

  //Set the current state ID
  stateID = STATE_INIT;
  currentState = new init();
}

void update(){
  // Change state (if needed)
  change_state();

  // Update listeners
  m_listener.update();
  k_listener.update();

  // Update state
  currentState -> update();
}

int main(){
  // Setup
  setup();

  // FPS Counter
  while( !key[KEY_ESC] && !closing && !joy[0].button[7].b){
    while( ticks == 0)
      rest( 1);
    while( ticks > 0){
      int old_ticks = ticks;
      update();
      ticks--;
      if( old_ticks <= ticks){
        break;
      }
    }
    if(game_time >= old_time + 1){// i.e. a 0.1 second has passed since we last counted the frames{
			fps -= frames_array[frame_index];// decrement the fps by the frames done a second ago
			frames_array[frame_index] = frames_done;// store the number of frames done this 0.1 second
			fps += frames_done;// increment the fps by the newly done frames
			frame_index = (frame_index + 1) % 10;// increment the frame index and snap it to 10
			frames_done = 0;
			old_time += 1;
		}
    currentState -> draw();
    frames_done++;
  }

  // Exit allegro
  allegro_exit();

	return 0;
}
END_OF_MAIN()
