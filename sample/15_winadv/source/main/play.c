collision()
{
/*function for checking if player is about to collide*/
float chkf_playerx,chkf_playery;
int chkplayerx,chkplayery;
int colorx,colory;

/*The current position of the player is taken*/
chkf_playerx=f_playerx;
chkf_playery=f_playery;
/*pretends that the player has moved in the
direction the player is moving in*/
chkf_playerx+=playermovex*speed*0.1;
chkf_playery+=playermovey*speed*0.1;
/*takes to floating point value and converts it to a integer*/
chkplayerx=chkf_playerx;
chkplayery=chkf_playery;

/*Collision bitmaps are located on bufferc which is update
every screen update to the corresponding screen that the player is
on. To save memory the collision bitmaps are one quarter the size
of the screen bitmap*/

/*gets value for the color with only checking if player can
move in x direction*/
colorx=getpixel(bufferc, (chkplayerx+25)/4, (playery+136)/4);
/*gets value for the color with only checking if player can
move in y direction*/
colory=getpixel(bufferc, (playerx+25)/4, (chkplayery+136)/4);


color_pos=getb(makecol(0,0,254));  /*if the player is on a object
                                     that has the blue value of 254
                                     the player can walk on the object*/
if(getb(getpixel(bufferc, (chkplayerx+25)/4, (chkplayery+136)/4))==color_pos)
return 3;
/*if the both pixels are black the player can move any direction*/
if ((colorx+colory)==0)return 3;
/*if x check pixel is black the player can move left and right*/
if(colorx==0)return 1;
/*if y check pixel is black the player can move up and down*/
if(colory==0)return 2;
/*if the function is has not returned a value yet
the player can not move*/
return 0;
}

moveplayerx()
{
/*This function moves the player left or right*/
f_playerx+=playermovex*speed*0.1;
playerx=f_playerx;

}
moveplayery()
{
/*This function moves the player up or down*/
f_playery+=playermovey*speed*0.1;
playery=f_playery;
}

stopplayer()
{
/*This function stops the player*/
playerx=f_playerx;
playery=f_playery;
movetox=playerx;
movetoy=playery;
if(talkback>0)
{
playermove=0;
showplayer=3;
}
}
screenswitch()
{
/*This function checks what screen the player is on
and if the player should be moved to another screen
it also sets the translucency for the current screen*/

/*The value of showscreen is screen the player is currently on
showscreen is a direct reference to the background screens in
the level1 datafile*/
if (showscreen==1)
{
set_trans_blender(0, 0, 0, 235);
if (playerx<2&&playery<170)
   {
showscreen=2;

playerx=588;
playery=288;
f_playerx=movetox=playerx;
f_playery=movetoy=playery;

  }
}

if (showscreen==2)
{
if (playerx>590)
   {
showscreen=1;

playerx=5;
playery=150;
f_playerx=movetox=playerx;
f_playery=movetoy=playery;

  }

if(talkback==0&&object[2][BACKDOOR][OPEN].state==1)    /*once the backdoor
                                                       of the truck is opened
                                                       a movie is played*/
movies=1;
}
if (showscreen==5)
{
/*if the door is opened and the player is selection is to walk and
the player is no longer talking to the screen
the screen can be switched*/
if (playerx>336&&playerx<403&&playery<70&&object[5][DOOR1][OPEN].state==1&&talkback==0&&(pick_choice==WALK||pick_choice==0)&&playermove>0)
   {
showscreen=6;
pick_choice=0;


playerx=77;
playery=290;
f_playerx=movetox=playerx;
f_playery=movetoy=playery;

  }
/*if scientist is already freed the player can follow the
scientist to the next room*/
if(follow>=150&&follow<173)follow++;
else
if (follow>=150&&follow<315)follow=315;
if(follow>=172)object[5][DOOR2][OPEN].state=1;

fseek(f4,sizeof(struct mover)*(follow),SEEK_SET);
                fread(&guy1,sizeof(struct mover),1,f4);
if(playerx<=108&&playery>=124&&playery<=192&&object[5][DOOR2][OPEN].state==1&&talkback==0&&(pick_choice==WALK||pick_choice==0)&&playermove>0)
{
showscreen=8;
playerx=558;
playery=110;
f_playerx=movetox=playerx;
f_playery=movetoy=playery;
}
}
if (showscreen==6)
{
/*If the rag has been lighted up the jail gates
will open the computer will light on fire and transluency
effects will be used.
If the rag has not been lighted up the screen will
have no transluency effects and the scientist will
be pacing in a jail cell
*/
if(object[6][RAG][USE].state==1)
{
if (pace<350)pace++;

if (pace>300&&pace<350)
{
talkback=1;
strcpy(talkback_desc[1].text,"QUICK HELP ME PUSH THIS MAN HOLE.");
stopplayer();
}
}
else
{
pace++;
pace%=238;
}
/*read the binary file by using fseek along with pace integer
to get the position and animation for the scientist*/
fseek(f3,sizeof(struct mover)*(pace),SEEK_SET);
fread(&guy1,sizeof(struct mover),1,f3);

set_trans_blender(0, 0, 0, 196);
        if (redlightdir==0)redlightcolor+=5;
        if (redlightdir==1)redlightcolor-=5;
        if (redlightcolor>115)
        {
        redlightcolor=115;
        redlightdir=1;
        }
        if(redlightcolor<0)
        {
        redlightcolor=0;
        redlightdir=0;
        }
if (playerx<120&&playery>300)
   {
showscreen=5;

playerx=350;
playery=75;
f_playerx=movetox=playerx;
f_playery=movetoy=playery;
change_screen();
  }
/*once the sewer is opened and there is no dialog
the player will move to the sewer and a movie will start
playing*/
if(object[6][SEWER][PUSH].state==1&&talkback==0)
{
showscreen=7;
playerx=300;
playery=100;
f_playerx=movetox=playerx;
f_playery=movetoy=playery;
movies=4;
change_screen();
}
}
if(showscreen==7)
{
/*The player can follow the scientist to the next room*/
redlightcolor=0;
if(playerx<=32&&playery>=192&&playery<=270&&follow>=150)
{
showscreen=5;
playerx=220;
playery=224;
f_playerx=movetox=playerx;
f_playery=movetoy=playery;
change_screen();
}
if(follow<=136&&follow>=1)follow++;
if(follow>=136&&follow<150) follow=150;
fseek(f4,sizeof(struct mover)*(follow),SEEK_SET);
                fread(&guy1,sizeof(struct mover),1,f4);
}
if(showscreen==8)
{
/*once the player is in the lab the gas will turn on
the scientist will talk and will then stand around until
you turn off the gas*/
if( (follow<416||(object[8][COMPARTMENT][OPEN].state==1&&follow<610))&&talkback==0)follow++;
if(follow==341)
{
strcpy(talkback_desc[1].text,"OH NO THE GAS ALARM SYSTEM.");
talkback=1;
}
if(follow==342)
{
strcpy(talkback_desc[1].text,"QUICK TURN OFF THE VENT GATE.");
talkback=1;
}
if(follow==437)
{
strcpy(talkback_desc[1].text,"I'LL GET THE DATA DISC.");
talkback=1;
}
if(follow==470)
{
strcpy(talkback_desc[1].text,"OKAY I HAVE IT!");
talkback=1;
}
if(follow==609)
{
strcpy(talkback_desc[1].text,"LET'S GO");
talkback=1;
}
fseek(f4,sizeof(struct mover)*(follow),SEEK_SET);
                fread(&guy1,sizeof(struct mover),1,f4);

if(gas_off==FALSE)sleep_gas();
/*if the door is open the player can leave*/
if(playerx>=570&&playery>=36&&playery<=160&&object[8][DOOR1][OPEN].state==1)
{
showscreen=5;
playerx=126;
playery=158;
f_playerx=movetox=playerx;
f_playery=movetoy=playery;
change_screen();
}
if(playery<=5&&playerx>=484&&playerx<=542&&object[8][DOOR2][OPEN].state==1)
{
showscreen=9;
playerx=502;
playery=284;
f_playerx=movetox=playerx;
f_playery=movetoy=playery;
change_screen();
}
}
if(showscreen==9)
{
/*if the data disc is recovered the scientist will say you can drive
and then you can take off in the tank*/
if(follow>609&&talkback==0)follow++;

if(follow==670)
{
strcpy(talkback_desc[1].text,"YOU CAN DRIVE");
talkback=1;
}

fseek(f4,sizeof(struct mover)*(follow),SEEK_SET);
                fread(&guy1,sizeof(struct mover),1,f4);

if(playery>=285&&playerx>=448&&playerx<=558)
{
showscreen=8;
playerx=510;
playery=22;
f_playerx=movetox=playerx;
f_playery=movetoy=playery;
change_screen();
}
if(playery>=290&&playerx>=68&&playerx<=162&&object[10][DOOR1][OPEN].state==1&&pace2==625)
{
showscreen=10;
playerx=480;
playery=24;
f_playerx=movetox=playerx;
f_playery=movetoy=playery;
change_screen();
}

}
if(showscreen==10)
{

if(playery<=16&&playerx>=456&&playerx<=512)
{
showscreen=9;
playerx=122;
playery=274;
f_playerx=movetox=playerx;
f_playery=movetoy=playery;
change_screen();
}
/*shows the bad guy pacing in the room*/
if(pace2<625)
{
fseek(f5,sizeof(struct mover)*(pace2),SEEK_SET);
                fread(&guy1,sizeof(struct mover),1,f5);

pace2%=439;
pace2++;
}

}
/*if the gas is on longer than 2 minutes the game is over*/
if(timeleftm<0&&times_gas_called>0&&gas_off==FALSE)
{
level=GAMEOVER;

}

}

change_screen()
{
pick_choice=0;
screen_objects=0;
talkback=0;
item_choice=0;
}

on_screen()
{
/*This function is used to check what object the player
is pointing to. It then changes the value of screen_objects.
(screen_objects is the integer that determines what object
the player is pointing to.)*/

int c,x,y;
                                     /*The integer talknotshown
                                     is used so that the player
                                     only starts talking to the
                                     screen after it is close
                                     to the object.
                                     if it equals 1 the player
                                     is still moving to the object
                                     if it equals 0 the player has
                                     stopped moving*/

/*If the mouse is pressed while the player is moving
it will interrupt whatever the player was about to do
before it gets to the object*/
if(talknotshown==1&&mousepressed==1)
{
screen_objects=0;
talknotshown=0;
}
/*If the player is not yet moving towards a object
the value of the screen_objects will change as the player
moves the mouse over them*/
if(talknotshown==0)
{
screen_objects=0;


/*bufferc is updated every time the screen is updated
and will change depending what screen the player is on
and if anything has happened such as taking a object.
(The bitmaps for objects are same ones for collisions
and are a quarter the size of the bitmap screen the player is
on.)
if the value of the pixel that the mouse is on is
a multiple of 50 the mouse is on a object.
so if the color is 50,0,0 the mouse is on object 1
if the color is 100,0,0 the mouse is on object 2
since the colors only go up to 255 is used the green value
also
if the color is 0,50,0 the mouse is on object 6
if the color is 50,50,0 the mouse is on object 7


This has nothing to do with this section but
since this bitmap is the same one used for collisions
If the color has a 254 for the blue value you can walk through
the object. Such as if the object was a oil spill or manhole on the
ground*/

for(c=1;c<6;c++)
{
color_pos=getr(makecol((c*50),0,0));
if(getr(getpixel(bufferc, mouse_x/4, mouse_y/4))==color_pos)
{
screen_objects+=c;
}
color_pos=getg(makecol(0,c*50,0));
if(getg(getpixel(bufferc, mouse_x/4, mouse_y/4))==color_pos)
{
screen_objects+=c*6;
}
}
oldscreen_objects=999;/*used to make sure oldscreen_objects is currently
                      not equal to screen_objects*/
}

/*if the player is moving toward a selected object and
has chosen a operation to do.  this section is called*/
if((mousepressed==1||talknotshown==1)&&(screen_objects>0&&pick_choice>0)&&talkback==0)
     {
/*while the player is moving towards the object
the value of oldscreen_objects is locked in to equal screen_objects*/

oldscreen_objects=screen_objects;

if(playermove==0&&showplayer==3)
  {
if(pick_choice!=WALK)
{
talkback=7;   /*If the player has stopped moving and the player
              choice is not to walk the player will end up talking
              to the screen by saying "GET CLOSER"*/
}

/*This section checks a square around the player to see if
the player is close enough to the object that was chosen.
the square it checks with is on collision bitmaps and
is a quarter the size of the actual screen*/
for(x=0;x<100;x+=4)
{
for(y=0;y<175;y+=4)
 {
if(screen_objects<6)color_pos=makecol(screen_objects*50,0,0);
else color_pos=makecol((screen_objects-6)*50,50,0);
if(getpixel(bufferc, ((playerx+x)-25)/4, ((playery+y)-30)/4)==color_pos)
{
talkback=1;  /*Whenever talkback is greater than 0 the player will
             talk to the screen*/
}
/*Since some objects have a blue value of 254 instead of 0
I ran the check twice*/
if(screen_objects<6)color_pos=makecol(screen_objects*50,0,254);
else color_pos=makecol((screen_objects-6)*50,50,254);
if(getpixel(bufferc, ((playerx+x)-50+25)/4, ((playery+y)-50+136)/4)==color_pos)
{
talkback=1;
}
 }

}


if(talkback==1)
{
if(pick_choice==WALK)
{
talkback=0;
talknotshown=0;
}
else
{
objects();     /*if talkback is equal to 1 this function is
               called and compares the screen the player is
               on the screen object the player has selected
               and the choice the player has selected.*/

if(pick_choice==GET)add_items(); /*if the choice is to get a item
                                 the function add item is called*/

/*copies the string returned from calling objects();
onto talkback_desc[1].text*/
strcpy(talkback_desc[1].text,object[showscreen][screen_objects][pick_choice].text);
/*if no text is changed 1 out of 5 rand answers
is displayed*/
if(strcmp(talkback_desc[1].text, "")==0)
{
talkback=rand()%5;
talkback+=2;
}
}
talknotshown=0;    /*if talkback is greater than 0*/
}


  }
else talknotshown=1; /*if player is still moving*/
     }
else talknotshown=0; /*if player has stopped moving*/

/*if the text is done being shown talkback is set back to 0*/
if((oldscreen_objects!=screen_objects)&&mousepressed==1)talkback=0;

}