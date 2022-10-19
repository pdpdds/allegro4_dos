 /*****************************************************/
 /*         A L L E G R O   P O N G   G A M E         */
 /*      VERSION FOR MS-DOS, USING ALLEGRO 4.2.2      */
 /*****************************************************/
 /*                                                   */
 /*     CREATED BY AARON C.D.C, IN C (USING DJGPP)    */
 /*             AND COMPILED USING GNU C              */
 /*                                                   */
 /*****************************************************/

 /* REDISTRIBUTION OF THIS SOFTWARE IS ALLOWED, "AS IT
  * IS". SELLING OR MONETIZING THIS SOFTWARE, OR ANY OF
  * IT'S PARTS, IS FORBIDDEN.
  *
  * YOU CAN MODIFY AND REDISTRIBUTE THIS GAME, ALSO "AS
  * IT IS".
  *
  * ----------------------------------------------------
  *
  * PONG is a classic arcade game, originally developed
  * by Atari Inc, in 1972. Becoming one of the earliest
  * and most famous games ever made. Because of it's fame,
  * many companies developed clones of this game, into
  * 'stand-alone' consoles.
  *
  * In PONG, each player control a "paddle" that can move
  * left or right. The goal of the game, is to bounce back
  * a ball, that is being disputed, and make it tresspass
  * the other player's screen bound (score).
  *
  * Usually, there is a score limit. The first player that
  * reach this score limit, wins the round.
  *
  * ---------------------------------------------------
  *
  *                 GAME DESCRIPTION
  *
  * The goal of this game is to be the first to score 15
  * points. The player (player 1), controls the paddle in the
  * bottom with the ARROW KEYS. The AI (player 2), controls
  * the one in the upper part of the screen.
  *
  * To start the game, press the SPACE key when the ball is in
  * the neutral position. This will make the ball move.
  *
  * To score points, make the ball go beyond the Player 2's screen
  * bound. But make sure he doesn't score by using your paddle to
  * bounce back the ball when it's moving towards you.
  *
  * You can also hold the LEFT SHIFT key to slow down your paddle.
  * this will make your movement smoother, but also slower.
  * If you press the LEFT CONTROL key, you can also see the trajectory
  * of the ball. This can be helpfull sometimes, but it is considered
  * "cheating".
  */

 #include <stdio.h>
 #include <stdlib.h>
 #include <time.h>
 #include <string.h>
 #include <allegro.h>

 /* ATARI 2600 STYLE STRUCTURES (2 PLAYERS, 1 BALL :) */

 //'Pragma pack' is a GCC exclusive directive. Use
 //__attribute__((packed)) if you are compiling this
 //using any other compiler.
 #pragma pack(push,1)
 typedef struct player {
   int posx;
   int posy;
   int vel;
   int score;
 }PLAYER;
 #pragma pack(pop)

 #pragma pack(push,1)
 typedef struct ball {
  int posx;
  int posy;
  int velx;
  int vely;
  int velxmul;
  int velymul;
  unsigned short init;
 }BALL;
 #pragma pack(pop)

 /* STRUCTURE FOR RAYCAST POINTS */
 #pragma pack(push,1)
 typedef struct bpoint {
  int posx;
  int posy;
 }BOUNCE;
 #pragma pack(pop)

 /* MAIN TIMER */
 volatile int ticks = 0;

 /* GLOBAL VARIABLES */
 int screenw = 640; //Width of the screen
 int screenh = 480; //Height of the screen
 unsigned short game_over = 0; //Is the game over?
 unsigned short cheat_a = 0; //Show lines where the ball bounce


 /* SOUNDS */
 SAMPLE *BOUNCE1S;
 SAMPLE *BOUNCE2S;
 SAMPLE *SCORES;
 SAMPLE *GAMEOVERS;

 /* MAIN TIMER FUNCTION */
 void ticker(){
   ticks++;
 }
 END_OF_FUNCTION(ticker);

 /* RESET BALL OBJECT */
 void resetball(BALL *ballobj) {

   ballobj->posx=screenw/2; ballobj->posy=screenh/2; ballobj->velx = 0; ballobj->vely = 0;
   ballobj->init = 0; ballobj->velxmul = 1; ballobj->velymul = 4;
   return;
 }

 /* CALCULATE BOUNCE POINTS */
 BOUNCE* calculate_bounces(BALL *myball, int *tbounces)
 {
   BOUNCE temp; //Where we store the point of contact.
   static BOUNCE* bouncelist;
   if(!bouncelist)
        bouncelist = (BOUNCE*)calloc(1,sizeof(BOUNCE)*10); //List of points
   memset(bouncelist, 0x0, sizeof(BOUNCE)*10);
   BALL fakeball; //Ball for raycasting.
   int bounces = 0; //Number of bounces.

   //SET UP THE RAYCAST BALL
   fakeball.posx = myball->posx;
   fakeball.posy = myball->posy;
   fakeball.velx = myball->velx;
   fakeball.vely = myball->vely;
   fakeball.velxmul = myball->velxmul;
   fakeball.velymul = myball->velymul;

   //Set the current position as a first contact
   temp.posx = fakeball.posx;
   temp.posy = fakeball.posy;
   bouncelist[bounces] = temp;
   bounces++;

   //Do the raycast
   while(bounces < 10 && (fakeball.posy > 0 && fakeball.posy < screenh)) {
         fakeball.posx += fakeball.velx * fakeball.velxmul;
         fakeball.posy += fakeball.vely * fakeball.velymul;
     
         //Is the ball hitting a wall? If so, bounce!
         if(fakeball.posx < 0 && fakeball.velx < 0) {
                //Set the contact information and store it
                temp.posx = fakeball.posx;
                temp.posy = fakeball.posy;
                bouncelist[bounces] = temp;
                bounces++; //Keep going
                fakeball.velx = 1; //Bounce
          }
         if(fakeball.posx > screenw && fakeball.velx > 0){
                temp.posx = fakeball.posx;
                temp.posy = fakeball.posy;
                bouncelist[bounces] = temp;
                bounces++;
                fakeball.velx = -1;
         }
   }

   //Save the last position of the ball as the end point
   temp.posx = fakeball.posx;
   temp.posy = fakeball.posy;
   bouncelist[bounces] = temp;
   bounces++;
   
   //Return the raycast simulation data
   *tbounces = bounces;
   return bouncelist;
 }
 
 /* DRAW BOUNCES */
 void drawbounces(BITMAP *scr, BOUNCE *blst, int bounces) {
   int c = 0;
   for(c = 0; c < bounces; c++)
   {
     fastline(scr, blst[c].posx, blst[c].posy, blst[c+1].posx, blst[c+1].posy,
     makecol(0,255,0));
   }
   textprintf_ex(scr,font,16,screenh-16,makecol(255,255,0),-1,"%d %d",c,bounces);
   return;
 }

 
 /* MAIN FUNCTION */
 int main(int args, char **argv)
 {
   BITMAP *screen2; //Double buffer
   int buttons = 0; //Mouse buttons
   int bounces = 0; //Balls bounced
   int xbounces = 0; //Raycast bounces
   BOUNCE *bbounces = 0; //Calculated bounces;
   allegro_404_char = '*'; //Show unknown glyphs with an asterisk.

   /* SETUP THE RANDOMIZER */
   time_t t;
   srand((unsigned) time(&t));

   /* SET UP THE PLAYER OBJECTS */
   PLAYER player1; player1.posx = screenw/2; player1.posy = screenh - 16; player1.vel = 0;
   player1.score = 0;
   PLAYER player2; player2.posx = screenw/2; player2.posy = 16; player2.vel = 0;
   player2.score = 0;

   /* SET UP THE BALL OBJECT */
   BALL ball1;
   resetball(&ball1);
   
   /* INITIALIZE THE ALLEGRO LIBRARY */
   allegro_init(); //Starts allegro
   set_config_file("pong.cfg"); //Create a config file
   install_keyboard(); //Setup the keyboard
   buttons = install_mouse(); //Setup a mouse

   /* SETUP THE MAIN TIMER TO 60 FPS */
   install_timer(); //Setup the timer
   LOCK_VARIABLE(ticks); //Set timer variable
   LOCK_FUNCTION(ticker); //Set timer function
   install_int_ex(ticker, BPS_TO_TIMER(60));

   /* INITIALIZE SOUND */
   if(install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL)) {
     allegro_message("Sound Error: %s\n", allegro_error);
     return 1;
   }

   set_volume(255, 255); //Max volume

   /* INITIALIZE GRAPHICS */
   set_color_depth(8); //256 color mode, 8 bits
#ifdef _WIN32
   if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, screenw, screenh, 0, 0)) {
#else
   if (set_gfx_mode(GFX_AUTODETECT, screenw, screenh, 0, 0)) {
#endif
     printf("Video error: %s", allegro_error);
     return 1;
   }

   /* INITIALIZE BACK BUFFER (MUST BE AFTER THE GRAPHICS!) */
   screen2 = create_bitmap(screen->w, screen->h); //Set up double buffer
   if(!screen2) {
     allegro_message("Failed to create double buffer. Out of memory?");
     exit(1);
   }

   /* LOAD EXTERNAL RESOURCES */
   BOUNCE1S = load_wav("sound\\bounce1.wav");
   BOUNCE2S = load_wav("sound\\bounce2.wav");
   SCORES   = load_wav("sound\\score.wav");
   GAMEOVERS = load_wav("sound\\gameover.wav");

   if(!BOUNCE1S || !BOUNCE2S || !SCORES || !GAMEOVERS) {
     allegro_message("Error loading WAV sounds!");
     return(1);
   }
   
   /* MAIN GAME LOOP - DO NOT LOAD ANYTHING INTO MEMORY PAST THIS POINT - */

   while(!key[KEY_ESC]){
      while(ticks == 0) {
        rest(1); //Wait for a full tick
      }

      while(ticks > 0) {
         int old_ticks = ticks;
         
         /* ADD YOUR GAME LOGIC HERE! */

         //Is the space key pressed?
         if(key[KEY_SPACE]) {
           // If the game is over, start a new game
           if(game_over == 1)
           {
             player1.score = 0;
             player2.score = 0;
             game_over = 0;
           }
           //If so, check if the ball is still in neutral position
           if(ball1.init == 0) {
              //And if that's the case, start the game!
              ball1.vely = rand() % 2 + 1;
              if(ball1.vely == 2)
                ball1.vely = -1;
                
              ball1.init = 1; //The ball is being disputed!
           }
         }
         
         //Is the ball out of bounds?
         if(ball1.posy < 0 || ball1.posy > screenh) {
            //Score! Check who gets the point
            if(ball1.posy < 0)
                player1.score += 1;
            if(ball1.posy > screenh)
                player2.score += 1;
            //Reset the ball
            resetball(&ball1);
            
            if(player1.score == 15 || player2.score == 15) {
                        game_over = 1; //First to score 15 wins.
                        play_sample(GAMEOVERS, 255, 0, 1000, 0);
                }else{
                        play_sample(SCORES, 255, 0, 1000, 0);
                }
         }

         //Is the ball hitting a wall? If so, bounce!
         if(ball1.posx < 0 && ball1.velx < 0) {
                ball1.velx = 1;
                play_sample(BOUNCE1S, 255, 0, 1000, 0);
         }
         if(ball1.posx > screenw && ball1.velx > 0) {
                ball1.velx = -1;
                play_sample(BOUNCE1S, 255, 0, 1000, 0);
         }

         //Is the ball hitting a paddle? If so, bounce!
         //For player 1
         if(ball1.vely > 0 && (ball1.posy >= player1.posy-4
         && ball1.posy <= player1.posy+3) && (ball1.posx >= player1.posx-20)
         && (ball1.posx <= player1.posx+20))
         {
           ball1.vely = -1;
           ball1.velx = rand() % 2 + 1;
           if(ball1.velx > 1) ball1.velx = -1;
           ball1.velxmul = rand() % 12 + 1;
           bounces++;
           play_sample(BOUNCE2S, 255, 0, 1000, 0);
         }

         //For player 2
         if(ball1.vely < 0 && (ball1.posy >= player2.posy-3
         && ball1.posy <= player2.posy+4) && (ball1.posx >= player2.posx-20)
         && (ball1.posx <= player2.posx+20))
         {
           ball1.vely = 1;
           ball1.velx = rand() % 2 + 1;
           if(ball1.velx > 1) ball1.velx = -1;
           ball1.velxmul = rand() % 8 + 1;
           bounces++;
           play_sample(BOUNCE2S, 255, 0, 1000, 0);
         }         

         //If 4 balls have been bounced back (using the paddles), increase the
         //speed.

         if(bounces >= 4) {
           bounces = 0;
           ball1.velymul++;
         }

         //Compute the ball speed
         ball1.posx += ball1.velx * ball1.velxmul;
         ball1.posy += ball1.vely * ball1.velymul;

         if(key[KEY_LCONTROL]) {
            if(cheat_a == 0)
                cheat_a = 1;
            else { cheat_a = 0;}
         }

         //Calculate bounces
         if(ball1.init > 0 && cheat_a == 1)
                bbounces = calculate_bounces(&ball1, &xbounces);

         //Is one of the direction keys pressed?
         //If so, set the velocity.
         if(key[KEY_RIGHT] ^ key[KEY_LEFT]) {
                if(key[KEY_RIGHT])
                        player1.vel = 1;
                else if(key[KEY_LEFT]) {
                        player1.vel = -1;
                        }
         }else{ player1.vel = 0; }

         //Apply the velocity to the paddle, assuming it's in the limits of the screen.
         if((player1.posx-20)<=0 && player1.vel < 0)
           player1.vel = 0;
         if((player1.posx+20)>=screenw && player1.vel > 0)
           player1.vel = 0;

         //The SHIFT key makes the paddle move slower.
         player1.posx += (player1.vel * 12)+(player1.vel * (key[KEY_LSHIFT]
         | key[KEY_RSHIFT]) * 6);

         //Compute the movement of the AI player
         if(ball1.vely < 0 && player2.posx > ball1.posx)
                player2.vel = -1;
         if(ball1.vely < 0 && player2.posx < ball1.posx)
                player2.vel = 1;
         if(ball1.vely > 1 || player2.posx == ball1.posx)
                player2.vel = 0;

         //Apply the velocity, assuming it's in the limits (same as player 1)
         if((player2.posx-20)<=0 && player2.vel < 0)
           player2.vel = 0;
         if((player2.posx+20)>=screenw && player2.vel > 0)
           player2.vel = 0;

         //I'll make this guy's speed a bit lower to stand a chance against him.
         //It's that, or delay the inputs.
         player2.posx += player2.vel * 10;
         /****************************/
         
         ticks--;
         if(old_ticks <= ticks) break; //Time up! Beggin drawing
      }
      /* ADD YOUR DRAWING ROUTINES HERE! */
      
      /* CLEAR BACK BUFFER */
      clear_bitmap(screen2);

      /* DRAW BOUNCES (CHEAT) *
       * Press LEFT CONTROL key to show the raycast */
      if(cheat_a==1)
        drawbounces(screen2, bbounces, xbounces);

      /* DRAW PLAYERS */
      rectfill(screen2, player1.posx-16, player1.posy-4, player1.posx+16,player1.posy+4,
      makecol(255,255,255));
      rectfill(screen2, player2.posx-16, player2.posy-4, player2.posx+16, player2.posy+4,
      makecol(255,255,255));

      /* DRAW THE BALL */
      rectfill(screen2,ball1.posx-2,ball1.posy-2,ball1.posx+2,ball1.posy+2,
      makecol(255,255,255));

      /* DRAW TEXTS */
      textprintf_ex(screen2, font, 16, 16, makecol(255,255,255), -1,
      "PLAYER 1: %d", player1.score);
            textprintf_ex(screen2, font, (screenw-16-(8*13)), 16, makecol(255,255,255), -1,
      "PLAYER 2: %d", player2.score);

      if(game_over == 1) {
            if(player1.score == 15)
                textprintf_ex(screen2, font, (screenw/2-(8*7)), (screenh/2)+4, makecol(255,255,255), -1,
      "PLAYER 1 WINS!");
            if(player2.score == 15)
                textprintf_ex(screen2, font, (screenw/2-(8*7)), (screenh/2)+4, makecol(255,255,255), -1,
      "PLAYER 2 WINS!");
      }
            

      /* SWAP BUFFERS */
      blit(screen2, screen, 0, 0, 0, 0, screen->w, screen->h);

   }
      /* - END OF LOOP - */

   /* TERMINATE PROGRAM */
   allegro_exit();
   return 0;
 }

#ifdef _WIN32
 END_OF_MAIN();
#endif
