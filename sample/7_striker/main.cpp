#include<iostream>
#include <allegro.h>
#include"Point.h"
#include"Player.h"
#include"Viper.h"
#include"Cobra.h"
#include"Bullet.h"
#include"Terrain.h"
#include"LinkedList.h"
#include "HealthBar.h"

volatile long speed_counter = 0; // A long integer which will store the value of the speed counter.
volatile long objectCounter = 0;
void increment_speed_counter() // A function to increment the speed counter
{
  speed_counter++; // This will just increment the speed counter by one. :)
  objectCounter++;
}
END_OF_FUNCTION(increment_speed_counter);

const int move_factor = 3;
Player* player;

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define DOS_SCREEN_WIDTH 320
#define DOS_SCREEN_HEIGHT 200

void CreateObjects(LinkedList*, BITMAP*);

int main(int argc, char* argv[])
{
	if (allegro_init() != 0)
	{
		allegro_message("Cannot initialize Allegro.\n");
		return 1;
	}

	//Set the window title when in a GUI environment
#ifdef _WIN32
	set_window_title("Striker");
#endif

	if (install_keyboard()) {
		allegro_message("Cannot initialize keyboard input.\n");
		return 1;
	}

	if (install_timer()) {
		allegro_message("Cannot initialize timer.\n");
		return 1;
	}

	install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, "");

	LOCK_VARIABLE(speed_counter); //Used to set the timer - which regulates the game's speed
    LOCK_FUNCTION(increment_speed_counter);
    install_int_ex(increment_speed_counter, BPS_TO_TIMER(60));//Set our BPS	

											

#ifdef _WIN32

															  //set graphics mode, trying all acceptable depths
	set_color_depth(32);

	int w, h;
	get_desktop_resolution(&w, &h);
	set_color_depth(desktop_color_depth());

	set_gfx_mode(GFX_SAFE, DOS_SCREEN_WIDTH, DOS_SCREEN_HEIGHT, 0, 0);
	BITMAP* buffer = create_bitmap(SCREEN_W, 640);
#else
	set_gfx_mode(GFX_AUTODETECT, DOS_SCREEN_WIDTH, DOS_SCREEN_HEIGHT, 0, 0);
	BITMAP* buffer = create_bitmap(screen->w, screen->h);
	if (!buffer) {
		allegro_message("Failed to create double buffer. Out of memory?");
		exit(1);
	}
#endif	
	
	/*Plane Image*/
	BITMAP* gameSprite = load_bitmap("Images/gameSprite.bmp", NULL);
	
    srand((unsigned)time(0));
    //y = 210 + (rand() % (int)(110));

    //Point p(SCREEN_W/2, 32);

    SAMPLE *fireSound = load_sample("Sounds/fireSound.wav");


    LinkedList* objects = new LinkedList(gameSprite);	
    //player = new Player(gameSprite, 64, 64, SCREEN_W/2, SCREEN_H - 96);
#ifdef _WIN32	
	player = new Player(gameSprite, 0, 0, 64, 64, SCREEN_W / 2, SCREEN_H - 96, 0);
#else
	/* set the color palette */
    set_palette(desktop_palette);

    /* clear the screen to white */
    clear_to_color(screen, makecol(255, 255, 255));    
	player = new Player(gameSprite, 0, 0, 64, 64, screen->w / 2, screen->h - 96, 0);		
#endif // _WIN32
	
    objects->Append(player);
	
    HealthBar* healthBar = new HealthBar(gameSprite,1,1);
	

    Terrain* terrainTop = new Terrain(gameSprite, 0, -640, 0, 0);
    Terrain* terrainBottom = new Terrain(gameSprite, 0, 0, 0, 0);


    int spaceDelay = 0;
	
	while(!key[KEY_ESC])
    {
        //Add your game logic here
        while (speed_counter > 0)
        {
            if(key[KEY_RIGHT]) // If the user hits the right key, change the picture's X coordinate
            {
                player->SetVelocityX(move_factor);
            }
            if(key[KEY_LEFT]) // Ditto' - only for left key
            {
                player->SetVelocityX(-move_factor);
            }
            if(key[KEY_UP]) // If the user hits the up key, change the picture's Y coordinate
            {
                player->SetVelocityY(-move_factor);
            }
            if(key[KEY_DOWN]) // Ditto' - only for down
            {
                player->SetVelocityY(move_factor);
            }
            if(key[KEY_SPACE]) // press space to fire
            {

                if(spaceDelay >= 20)
                {
                    objects->Append(player->Fire());
                    play_sample(fireSound, 255, 128, 1000, 0);
                    spaceDelay = 0;
                }

            }

            spaceDelay++;

            if(terrainTop->GetPosition()->y>640)
            {
                cout<<"\nTerrainTop Y:"<<terrainTop->GetPosition()->y;
                delete terrainTop;
                terrainTop = new Terrain(gameSprite, 0, -640, 0, 0);
            }
            if(terrainBottom->GetPosition()->y>640)
            {
                cout<<"\nTerrainBottom Y:"<<terrainBottom->GetPosition()->y;
                delete terrainBottom;
                terrainBottom = new Terrain(gameSprite, 0, -640, 0, 0);
            }

            objects->MoveAll();
            healthBar->Move(player->GetHealth());
            terrainTop->Move();
            terrainBottom->Move();
            CreateObjects(objects, gameSprite);


            speed_counter--;
        }


        terrainBottom->Draw(buffer);
        terrainTop->Draw(buffer);
        healthBar->Draw(buffer);
        objects->DrawAll(buffer, false);

		draw_sprite(screen, buffer, 0, 0);
		clear_bitmap(buffer);
        objects->CleanAll();
	}
	//Cleaning Memory
	destroy_bitmap(buffer);
	destroy_bitmap(gameSprite);
	destroy_sample(fireSound);
	delete terrainTop;
	delete terrainBottom;
	delete objects;

	return 0;
	//Allegro will automatically deinitalize itself on exit

}
END_OF_MAIN();

void CreateObjects(LinkedList* objects, BITMAP* gameSprite)
{
    if(objectCounter%200 == 0)
    {
        Viper* viper = new Viper(gameSprite, 192, 0, 64, 64, (rand() % (int)(SCREEN_W)), -128, 21);
        viper->SetTarget(player);
        objects->Append(viper);
    }
    if(objectCounter%100 == 0)
    {
        Cobra* cobra = new Cobra(gameSprite, 384, 0, 64, 64, (rand() % (int)(SCREEN_W)), -128, 22);
        cobra->SetTarget(player);
        objects->Append(cobra);
    }
}
