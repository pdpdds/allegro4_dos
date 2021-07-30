/*****************************************************************************
 PPCOL Library example program, by Ivan Baldo.
 Based on ex1.c.
 This program demonstrates the use of the amount of colision detection
 routines.
*****************************************************************************/

#include <stdio.h>
#include <allegro.h>
#include <ppcol.h>

#include "..\\sprites.h" //Datafile generated header containing some sprite index and a pallete. 

DATAFILE *data;
BITMAP *sbuffer; //This will be the screen buffer on memory...

int X1, Y1; //Positions of the sprites.
int X2, Y2;
int SP1, SP2; //This stores what sprite we are using from the datafile...
int MASK; //This tells if we have to show the mask or not...

PPCOL_MASK *sprite_masks[6]; //Creates an array of sprite masks pointers.

int create_sprite_masks(); //This function will initialize all the masks for all the sprites. You have to look at this function! If there is an error, it returns 1.
void destroy_sprite_masks(); //This function will deinitialize all the masks of all the sprites. See it.
void move_sprites(); //This function will check for the correct keys and move the sprite.
void draw_sprites(); //This function will clear the buffer to black and draw the sprites in their respective positions...
void show_info(); //This function will show the amount of colision. This is the function where you have to look!
void blit_screen(); //This function will blit the buffer to the screen.


int main(int argc, char *argv[])
{
 int card=GFX_AUTODETECT;
 int width=320;
 int height=240;
 int depth=8;

 allegro_init();
 install_timer(); //For vsync() and the video mode selection screen...
 install_keyboard();
 install_mouse(); //For the video mode selection menu...


 set_gfx_mode(GFX_VGA,320,200,0,0);
 set_palette(desktop_palette);
 if (!gfx_mode_select_ex(&card, &width, &height, &depth)) return 1; //The user has pressed ESC to quit the example before selecting a video mode...

 set_color_depth(depth); //First set the requested color depth...
 if(set_gfx_mode(card, width, height, 0, 0)) {printf(allegro_error); return 2;} //There was a problem setting the requested video mode... show Allegro error information...

 clear(screen);

 //Initialize the variables...
 X1=width/3; Y1=height/3;
 X2=X1*2; Y2=Y1*2;
 SP1=1; SP2=2;
 MASK=0; //Don't show the masks...

 data=load_datafile("sprites.dat");
 if(!data) {
   set_gfx_mode(GFX_TEXT,0,0,0,0);
   printf("Couldn't load sprites.dat!");
   return 1;
   }

 if(depth!=8) { //Transparent color conversion method by Peter Wang.
   int SPRNUM, X,Y;
   select_palette(data[PAL].dat);
   unload_datafile(data); //This and the next is because we have loaded the datafile with a wrong palette thus the conversion between 8 colors and Xbpp format fails.
   data=load_datafile("sprites.dat");
   for(SPRNUM=SPRITE_0; SPRNUM<SPRITE_0+6; SPRNUM++) {
      for(Y=0; Y< ((BITMAP *)data[SPRNUM].dat)->h; Y++) {
         for(X=0; X< ((BITMAP *)data[SPRNUM].dat)->w; X++) {
            if( getpixel(data[SPRNUM].dat, X,Y)==makecol(0,0,0) ) putpixel(data[SPRNUM].dat, X,Y, makecol(255,0,255) );
            }
         }
      }
   } else {
   set_palette(data[PAL].dat);
   }

 sbuffer=create_bitmap(width,height);

 if(create_sprite_masks()) return 1; //We call this function to initialize the masks for all the sprites. This is a function used in this example, it is not a PPCol library function! It returns 1 if there is an error, 0 if all is correct.

 while(!key[KEY_ESC]) { //While the user is not bored...
   move_sprites();
   draw_sprites();
   show_info();
   blit_screen();
   vsync(); //This prevents the movement going too fast.
   }

 destroy_sprite_masks();

 return 0;
}


//This function will create the masks for every sprite... Look at it...
int create_sprite_masks()
{
 int SPNUM;

 for(SPNUM=SPRITE_0; SPNUM<SPRITE_0+6; SPNUM++) { //Loop through all the sprites...
    sprite_masks[SPNUM-1] = create_ppcol_mask(data[SPNUM].dat); //Call the ppcol library mask creation routine.
    if(sprite_masks[SPNUM-1]==NULL) { //If the mask has not been created...
      set_gfx_mode(GFX_TEXT,0,0,0,0);
      printf("Error creating mask for sprite %d!",SPNUM-SPRITE_0+1);
      return 1;
      }
    }
 return 0;
 }

//This function will deinitialize all the masks of all the sprites. See it.
void destroy_sprite_masks()
{
 int MASKNUM;

 for(MASKNUM=0; MASKNUM<6; MASKNUM++) { //Loop through all the masks...
    destroy_ppcol_mask(sprite_masks[MASKNUM]); //Release memory...
    }

 }



//This function will check for some keys and move the sprites and select the sprites...
void move_sprites()
{
 //Move sprite 1 with the arrow keys...
 if(key[KEY_UP]) Y1--; else if(key[KEY_DOWN]) Y1++;
 if(key[KEY_LEFT]) X1--; else if(key[KEY_RIGHT]) X1++;
 //Check if sprite 1 is in range...
 if(Y1<0) Y1=0;
 if(Y1>SCREEN_H) Y1=SCREEN_H;
 if(X1<0) X1=0;
 if(X1>SCREEN_W) X1=SCREEN_W;

  //Move sprite 2 with the keyboard keys...
 if(key[KEY_W]) Y2--; else if(key[KEY_X]) Y2++;
 if(key[KEY_A]) X2--; else if(key[KEY_D]) X2++;
 //Check if sprite 2 is in range...
 if(Y2<0) Y2=0;
 if(Y2>SCREEN_H) Y2=SCREEN_H;
 if(X2<0) X2=0;
 if(X2>SCREEN_W) X2=SCREEN_W;

 //Select sprites with the 1 and 2 keys respectivelly...
 if(key[KEY_1]) { SP1++; clear_keybuf(); } //clear_keybuf() is used to slow down the keypress...
 if(key[KEY_2]) { SP2++; clear_keybuf(); }
 if(SP1>6) SP1=1;
 if(SP2>6) SP2=1;

 //If you press M you can toggle the mask view...
 if(key[KEY_M]) {
   if(MASK) MASK=0; else MASK=1;
   clear_keybuf();
   }

 }


// This function will clear the screen buffer to color black and draw the sprites...
void draw_sprites()
{
 int CHUNK;  //Stores the current chunk for the mask view...
 int ROW;    //Stores the current row for the mask view...
 int CHUNKP; //Stores the current position in the chunk for the mask view...

 clear(sbuffer);

 draw_sprite(sbuffer, data[SP1].dat, X1, Y1);
 draw_sprite(sbuffer, data[SP2].dat, X2, Y2);

 //Draw sprite masks if the user wants...
 if(MASK) {
   for(CHUNK=0; CHUNK<=sprite_masks[SP1-1]->max_chunk; CHUNK++) {
      for(ROW=0; ROW<sprite_masks[SP1-1]->h; ROW++) {
         for(CHUNKP=CHUNK*32; CHUNKP<(CHUNK*32+32); CHUNKP++) {
             if( sprite_masks[SP1-1]->sp_mask[ROW][CHUNK]<<CHUNKP & 0x80000000) putpixel(sbuffer, X1+CHUNKP, Y1+ROW, makecol(64,255,64) );
             }
         }
      }
   for(CHUNK=0; CHUNK<=sprite_masks[SP2-1]->max_chunk; CHUNK++) {
      for(ROW=0; ROW<sprite_masks[SP2-1]->h; ROW++) {
         for(CHUNKP=CHUNK*32; CHUNKP<(CHUNK*32+32); CHUNKP++) {
             if( sprite_masks[SP2-1]->sp_mask[ROW][CHUNK]<<CHUNKP & 0x80000000) putpixel(sbuffer, X2+CHUNKP, Y2+ROW, makecol(64,255,64) );
             }
         }
      }

   }

 }


//This function will show the amount of collision. This is the function where you have to look!
void show_info()
{
 int LINE=0, AMOUNT;
 textout(sbuffer,font,"Keys: 1, 2, M, Arrows, A, D, W, X, ESC.",7,LINE*8,makecol(255,255,32) );

 LINE++;

 AMOUNT=check_ppmask_collision_amount(sprite_masks[SP1-1], sprite_masks[SP2-1], X1,Y1, X2,Y2, PPCAMOUNT_UP);
 textprintf(sbuffer,font,7,LINE*8,makecol(255,255,255),"        P.P.M. collision amount up: %d",AMOUNT );
 LINE++;
 AMOUNT=check_pp_collision_amount(data[SP1].dat, data[SP2].dat, X1,Y1, X2,Y2, PPCAMOUNT_UP);
 textprintf(sbuffer,font,7,LINE*8,makecol(255,255,255),"          P.P. collision amount up: %d",AMOUNT );
 LINE++;

 AMOUNT=check_ppmask_collision_amount(sprite_masks[SP1-1], sprite_masks[SP2-1], X1,Y1, X2,Y2, PPCAMOUNT_UP_RIGHT);
 textprintf(sbuffer,font,7,LINE*8,makecol(255,255,255),"  P.P.M. collision amount up right: %d",AMOUNT );
 LINE++;
 AMOUNT=check_pp_collision_amount(data[SP1].dat, data[SP2].dat, X1,Y1, X2,Y2, PPCAMOUNT_UP_RIGHT);
 textprintf(sbuffer,font,7,LINE*8,makecol(255,255,255),"    P.P. collision amount up right: %d",AMOUNT );
 LINE++;

 AMOUNT=check_ppmask_collision_amount(sprite_masks[SP1-1], sprite_masks[SP2-1], X1,Y1, X2,Y2, PPCAMOUNT_RIGHT);
 textprintf(sbuffer,font,7,LINE*8,makecol(255,255,255),"     P.P.M. collision amount right: %d",AMOUNT );
 LINE++;
 AMOUNT=check_pp_collision_amount(data[SP1].dat, data[SP2].dat, X1,Y1, X2,Y2, PPCAMOUNT_RIGHT);
 textprintf(sbuffer,font,7,LINE*8,makecol(255,255,255),"       P.P. collision amount right: %d",AMOUNT );
 LINE++;

 AMOUNT=check_ppmask_collision_amount(sprite_masks[SP1-1], sprite_masks[SP2-1], X1,Y1, X2,Y2, PPCAMOUNT_DOWN_RIGHT);
 textprintf(sbuffer,font,7,LINE*8,makecol(255,255,255),"P.P.M. collision amount down right: %d",AMOUNT );
 LINE++;
 AMOUNT=check_pp_collision_amount(data[SP1].dat, data[SP2].dat, X1,Y1, X2,Y2, PPCAMOUNT_DOWN_RIGHT);
 textprintf(sbuffer,font,7,LINE*8,makecol(255,255,255),"  P.P. collision amount down right: %d",AMOUNT );
 LINE++;

 AMOUNT=check_ppmask_collision_amount(sprite_masks[SP1-1], sprite_masks[SP2-1], X1,Y1, X2,Y2, PPCAMOUNT_DOWN);
 textprintf(sbuffer,font,7,LINE*8,makecol(255,255,255),"      P.P.M. collision amount down: %d",AMOUNT );
 LINE++;
 AMOUNT=check_pp_collision_amount(data[SP1].dat, data[SP2].dat, X1,Y1, X2,Y2, PPCAMOUNT_DOWN);
 textprintf(sbuffer,font,7,LINE*8,makecol(255,255,255),"        P.P. collision amount down: %d",AMOUNT );
 LINE++;

 AMOUNT=check_ppmask_collision_amount(sprite_masks[SP1-1], sprite_masks[SP2-1], X1,Y1, X2,Y2, PPCAMOUNT_DOWN_LEFT);
 textprintf(sbuffer,font,7,LINE*8,makecol(255,255,255)," P.P.M. collision amount down left: %d",AMOUNT );
 LINE++;
 AMOUNT=check_pp_collision_amount(data[SP1].dat, data[SP2].dat, X1,Y1, X2,Y2, PPCAMOUNT_DOWN_LEFT);
 textprintf(sbuffer,font,7,LINE*8,makecol(255,255,255),"   P.P. collision amount down left: %d",AMOUNT );
 LINE++;

 AMOUNT=check_ppmask_collision_amount(sprite_masks[SP1-1], sprite_masks[SP2-1], X1,Y1, X2,Y2, PPCAMOUNT_LEFT);
 textprintf(sbuffer,font,7,LINE*8,makecol(255,255,255),"      P.P.M. collision amount left: %d",AMOUNT );
 LINE++;
 AMOUNT=check_pp_collision_amount(data[SP1].dat, data[SP2].dat, X1,Y1, X2,Y2, PPCAMOUNT_LEFT);
 textprintf(sbuffer,font,7,LINE*8,makecol(255,255,255),"        P.P. collision amount left: %d",AMOUNT );
 LINE++;

 AMOUNT=check_ppmask_collision_amount(sprite_masks[SP1-1], sprite_masks[SP2-1], X1,Y1, X2,Y2, PPCAMOUNT_UP_LEFT);
 textprintf(sbuffer,font,7,LINE*8,makecol(255,255,255),"   P.P.M. collision amount up left: %d",AMOUNT );
 LINE++;
 AMOUNT=check_pp_collision_amount(data[SP1].dat, data[SP2].dat, X1,Y1, X2,Y2, PPCAMOUNT_UP_LEFT);
 textprintf(sbuffer,font,7,LINE*8,makecol(255,255,255),"     P.P. collision amount up left: %d",AMOUNT );
 LINE++;

 }



//This will draw everything...
void blit_screen()
{
 blit(sbuffer,screen,0,0, 0,0, SCREEN_W,SCREEN_H);
 }

