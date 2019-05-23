passwordscreen()
{
/*screen for entering passwords for the level*/
char password[10];
int levelpassword;
int c,i,letter=0;

memset(password,0,sizeof(password));

clear(buffer);
sprintf(msg, "ENTER PASSWORD");
textout_centre(buffer, datafile[25].dat, msg, SCREEN_W/2, SCREEN_H/2,makecol(150,225,200));
sprintf(msg, "PRESS ENTER TO FINISH");
textout_centre(buffer, datafile[25].dat, msg, SCREEN_W/2, SCREEN_H/2+100,makecol(150,225,200));
sprintf(msg,"%s",password);
textout_centre(buffer, datafile[25].dat, msg, SCREEN_W/2, SCREEN_H/2-150,makecol(150,225,200));
blit(buffer, screen,0, 0,0,0, SCREEN_W, SCREEN_H);

while (!(key[KEY_ENTER]))
       {
clear(buffer);
sprintf(msg, "ENTER PASSWORD");
textout_centre(buffer, datafile[25].dat, msg, SCREEN_W/2, SCREEN_H/2,makecol(150,225,200));
sprintf(msg, "PRESS ENTER TO FINISH");
textout_centre(buffer, datafile[25].dat, msg, SCREEN_W/2, SCREEN_H/2+100,makecol(150,225,200));

password[letter]=readkey();
if(key[KEY_ENTER])password[letter]=0;
/*backs up the string of letters*/
if(key[KEY_BACKSPACE])
{
password[letter]=0;
if(letter>0)
{
password[letter-1]=0;
letter--;
}
}
if(!(key[KEY_BACKSPACE])&&letter<9) letter++;

sprintf(msg,"%s %d",password,letter);
textout_centre(buffer, datafile[25].dat, msg, SCREEN_W/2, SCREEN_H/2-150,makecol(150,225,200));
 stretch_blit(buffer, screen,
                  0, 0, 640, 480,
                  0, 0, SCREEN_W, SCREEN_H);
       }
/*takes the spaces out of the password*/
for(c=0;c<letter;c++)
{
if(password[c]==32)
{
for(i=c+1;i<letter;i++)
{
password[i-1]=password[i];
}
}
}
/*capitalizes the password*/
strupr(password);
/*if the password matches any good passwords the level will come up*/
if(strcmp(password, "LEVEL2")==0)
{
level=2;
object[5][RAG][GET].state=2;
object[5][MATCHES][GET].state=2;
object[5][DOOR1][OPEN].state=3;
playerx=300;
playery=100;
f_playerx=movetox=playerx;
f_playery=movetoy=playery;
showscreen=7;
movies=0;
}
if(strcmp(password, "TANKGAME")==0)
{
level=3;
movies=0;
}
if(strcmp(password, "TANKGAME2")==0)
{
level=4;
movies=0;
}
return level;
}


titlescreen()
{
/*shows the starting screen currently the player can play a new game
or continue (options isn't being used)*/
int startgame=0;
int menutoshow=0;
while(startgame<1)
{
if((mouse_b&1)&&mousepressed==1)mousepressed=2;
if((mouse_b&1)&&mousepressed==0)mousepressed=1;
if(!(mouse_b&1))mousepressed=0;

if(menutoshow==0)
{
/*clears the buffer*/
   clear(buffer);
sprintf(msg, "NEW GAME");
if(mouse_x>SCREEN_W/2-150&&mouse_x<SCREEN_W/2+150&&mouse_y>SCREEN_H/2-50&&mouse_y<SCREEN_H/2+50)
{
textout_centre(buffer, datafile[25].dat, msg, SCREEN_W/2, SCREEN_H/2,makecol(150,225,200));
if(mousepressed==1)
{
level=1;
startgame=1;
}

}
else
textout_centre(buffer, datafile[25].dat, msg, SCREEN_W/2, SCREEN_H/2,makecol(175,175,175));

sprintf(msg, "CONTINUE");
if(mouse_x>SCREEN_W/2-150&&mouse_x<SCREEN_W/2+150&&mouse_y>SCREEN_H/2+50&&mouse_y<SCREEN_H/2+150)
{
textout_centre(buffer, datafile[25].dat, msg, SCREEN_W/2, (SCREEN_H/2)+100,makecol(150,225,200));

if(mousepressed==1)
{
level=passwordscreen();
if(level>=1)
{
startgame=1;
break;
}
}

}
else
textout_centre(buffer, datafile[25].dat, msg, SCREEN_W/2, (SCREEN_H/2)+100,makecol(175,175,175));

sprintf(msg, "OPTIONS");
if(mouse_x>SCREEN_W/2-150&&mouse_x<SCREEN_W/2+150&&mouse_y>SCREEN_H/2+150&&mouse_y<SCREEN_H/2+250)
{
textout_centre(buffer, datafile[25].dat, msg, SCREEN_W/2, (SCREEN_H/2)+200,makecol(150,225,200));
}
else
textout_centre(buffer, datafile[25].dat, msg, SCREEN_W/2, (SCREEN_H/2)+200,makecol(175,175,175));
}
   show_mouse(buffer);   /*shows the mouse after all drawing
                              to the buffer is done*/
    stretch_blit(buffer, screen,
                  0, 0, 640, 480,
                  0, 0, SCREEN_W, SCREEN_H);
   show_mouse(NULL);


}
}
