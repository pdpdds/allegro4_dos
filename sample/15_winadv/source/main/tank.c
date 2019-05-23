tankgame()
{
/*This function is the first tankgame where the screen self scrolls
and you need to hit the bad turret guns*/

/*binary file to place the badguys*/
FILE* badtank;
/*positions the badguys (I didn't end up using type)*/
struct BADGUYS
{
int x,y,type;

};
struct BADGUYS enemy[100];
int badenergy[100];      /*energy for each badguy*/
float getseconds[100];   /*checks how many seconds ago badguy was hit
                         to show effect*/
float getseconds2;
float f_turretangle=0;  /*angle of tank gun*/
float f_turretangle1=0;

float rise=0;          /*floats for calculating angle badguys should shoot at*/
float run=0;

int turretangle[100];
int atenemy=0;
int mickeyx=0;
int mickeyy=0;
int adjustmagx=100;     /*adjustments for showing bullets coming out of tank*/
int adjustmagy=100;
int adjustposx=50;
int adjustposy=42;
                         /*up to 30 bullets can come out of tank at once*/
int showbullet[30];
int bulletxpos[30];
int bulletypos[30];
int bulletxforce[30];
int bulletyforce[30];
                        /*bullets for each badguy*/
int showbadbullet[100];
int badbulletxpos[100];
int badbulletypos[100];
int badbulletxforce[100];
int badbulletyforce[100];
int badr=0;


                /*variables for showing and moving tank*/
int bodyangle=0;
float f_bodyangle=0;
int gunangle=0;
float  fbodymovex=0;
float  fbodymovey=300;
int bodymovex=0;
int bodymovey=0;
int tankypos=0;
int goodenergy=500;
int speed=4;
int scrolly=480;
int c=0;
int i=0;
                /*opens the binary file for positioning badguys*/
badtank=fopen("data/level2/badtank","ab+");


                /*sets the arrays and structures to 0*/
memset(&enemy,0,sizeof(struct BADGUYS));
memset(turretangle,0,sizeof(turretangle));
memset(getseconds,0,sizeof(getseconds));

memset(bulletxpos,0,sizeof(bulletxpos));
memset(bulletxforce,0,sizeof(bulletxforce));
memset(bulletypos,0,sizeof(bulletxpos));
memset(bulletyforce,0,sizeof(bulletxforce));
memset(showbullet,FALSE,sizeof(showbullet));

memset(badbulletxpos,0,sizeof(badbulletxpos));
memset(badbulletxforce,0,sizeof(badbulletxforce));
memset(badbulletypos,0,sizeof(badbulletxpos));
memset(badbulletyforce,0,sizeof(badbulletxforce));
memset(showbadbullet,FALSE,sizeof(showbadbullet));

fseek(badtank,sizeof(struct BADGUYS)*(atenemy),SEEK_SET);
fread(&enemy,sizeof(struct BADGUYS),100,badtank);
                 /*each badguy takes ten hits*/
for(c=0;c<100;c++)
badenergy[c]=10;
                  /*transluency for showing hits*/
set_trans_blender(0, 0, 0, 196);

set_mouse_speed(80, 80);

clear(buffer);
for(;;)
{
/*title screen for when tankgame starts*/
if(mouse_b&2)break;

sprintf(msg, "MISSION START!" );
textout_centre(buffer, datafile[22].dat, msg, SCREEN_W/2, 30, 0);
sprintf(msg, "YOU HAVE TAKEN THE DISC FROM THE BASE" );
textout_centre(buffer, datafile[22].dat, msg, SCREEN_W/2, 60, 0);
sprintf(msg, "AND STARTED TO MOVE TO THE CRASH SITE" );
textout_centre(buffer, datafile[22].dat, msg, SCREEN_W/2, 90, 0);
sprintf(msg, "CONTROLS:" );
textout_centre(buffer, datafile[22].dat, msg, SCREEN_W/2, 150, 0);
sprintf(msg, "ARROW KEYS TO MOVE THE TANK" );
textout_centre(buffer, datafile[22].dat, msg, SCREEN_W/2, 180, 0);
sprintf(msg, "MOUSE TO MOVE THE GUN" );
textout_centre(buffer, datafile[22].dat, msg, SCREEN_W/2, 210, 0);

sprintf(msg, "PASSWORD : TANKGAME" );
textout_centre(buffer, datafile[22].dat, msg, SCREEN_W/2, 240, 0);

sprintf(msg, "RIGHT CLICK TO START" );
textout_centre(buffer, datafile[22].dat, msg, SCREEN_W/2, 270, 0);

 stretch_blit(buffer, screen,
                  0, 0, 640, 480,
                  0, 0, SCREEN_W, SCREEN_H);
show_mouse(NULL);
clear_to_color(buffer,makecol(196,196,196));

}
speed_counter=0;
for(;;)
{
/*if esc is pressed the game is over*/
if(key[KEY_ESC])
{
level=QUIT;
break;
}
/*if the integer counts up to 4800 the level is done*/
if(scrolly>=4800)
{
level=4;
break;
}
/*if the player runs out of energy the player loses*/
if(goodenergy==0)
{
level=GAMEOVER;
break;
}

while(speed_counter > 0)

{
if((mouse_b&1)&&mousepressed>0)mousepressed++;
if((mouse_b&1)&&mousepressed==0)mousepressed=1;
if(!(mouse_b&1))mousepressed=0;
/*moves tank in circles by pressing left and right*/
if(key[KEY_RIGHT])
{
bodyangle+=2;
if (bodyangle>256)bodyangle=0;
}
if(key[KEY_LEFT])
{
bodyangle-=2;
if (bodyangle<-256)bodyangle=0;
}
 /*the tank will move backwards*/
if(key[KEY_DOWN])
{
fbodymovey+=cos(RAD*(bodyangle*1.40625))*speed;
fbodymovex-=sin(RAD*(bodyangle*1.40625))*speed;

}
/*the tank will move forwards*/
if(key[KEY_UP])
{
fbodymovey-=cos(RAD*(bodyangle*1.40625))*speed;
fbodymovex+=sin(RAD*(bodyangle*1.40625))*speed;

}
/*uncomment this to move the gun with the keyboard*/
//if(key[KEY_A])
//{
//gunangle+=5;
/*the tank gun is adjusted by using the mouse in a circular motion*/
gunangle+=sin(RAD*(gunangle*1.40625))*mickeyy;
gunangle+=cos(RAD*(gunangle*1.40625))*mickeyx;

if (gunangle>256)gunangle-=256;
//}
//if(key[KEY_S])
//{
//gunangle-=5;
if (gunangle<-256)gunangle+=256;
//}
/* adjustments for setting up bullets
if(key[KEY_Q])
{
adjustposx++;
}
if(key[KEY_W])
{
adjustposx--;
}
if(key[KEY_E])
{
adjustmagx++;
}
if(key[KEY_R])
{
adjustmagx--;
}
if(key[KEY_T])
{
adjustposy++;
}
if(key[KEY_Y])
{
adjustposy--;
}
if(key[KEY_U])
{
adjustmagy++;
}
if(key[KEY_I])
{
adjustmagy--;
}
*/
mousepressed%=3;//used for autofire
for(c=0;c<30;c++)
{
/*for loop for showing up to 30 bullets at a time*/
if(mousepressed==1&&showbullet[c]==FALSE)
{
mousepressed=2;
/*sets the bullet to be shown and uses the angle of the gun to
calculate the force and starting position of the bullet*/
showbullet[c]=TRUE;
bulletxforce[c]=sin(RAD*(gunangle*1.40625))*10;
bulletyforce[c]=-cos(RAD*(gunangle*1.40625))*10;
bulletxpos[c]=adjustposx+bodymovex+sin(RAD*(gunangle*1.40625))*adjustmagx;
bulletypos[c]=adjustposy+tankypos-cos(RAD*(gunangle*1.40625))*adjustmagy;
}
/*if the bullet is off the screen the bullet parameters
are set to zero and can be used again*/
if(bulletxpos[c]>SCREEN_W||bulletxpos[c]<0||bulletypos[c]>SCREEN_H+50||bulletypos[c]<0)
{
showbullet[c]=FALSE;
bulletxforce[c]=0;
bulletyforce[c]=0;
bulletxpos[c]=0;
bulletypos[c]=0;
}
/*once bullet is true it will move until it hits a target or
goes off the screen*/
if(showbullet[c]==TRUE)
{
bulletxpos[c]+=bulletxforce[c];
bulletypos[c]+=bulletyforce[c];
for(i=0;i<100;i++)
{
if((bulletxpos[c]-50>enemy[i].x-20)&&(bulletxpos[c]-50<enemy[i].x+20)&&(bulletypos[c]-56>(enemy[i].y-20+scrolly))&&(bulletypos[c]-56<(enemy[i].y+20+scrolly))&&badenergy[i]>0)
{
showbullet[c]=FALSE;
bulletxforce[c]=0;
bulletyforce[c]=0;
bulletxpos[c]=0;
bulletypos[c]=0;
getseconds[i]=seconds;  /*shows flash hiting badguy*/
badenergy[i]--;         /*lowers badguy energy*/
}
}
}
}
                     /*
if(mousepressed==999)used to place badguys with binary file
{
mousepressed=1;
enemy[atenemy].x=mouse_x;
enemy[atenemy].y=mouse_y-scrolly;
enemy[atenemy].type=1;
fseek(badtank,sizeof(struct BADGUYS)*(atenemy),SEEK_SET);
fwrite(&enemy[atenemy],sizeof(struct BADGUYS),1,badtank);
atenemy++;
}
                       */
/*moves the level forward*/
scrolly++;
/*sets from left to right how far the tank can move*/
if(fbodymovex>350)fbodymovex=350;
if(fbodymovex<250)fbodymovex=250;

bodymovex=fbodymovex;
bodymovey=fbodymovey;

/*sets tank position on the screen*/
tankypos=bodymovey+scrolly;
/*checks that the tank is within bounds up and down*/
while(tankypos>400)
{
fbodymovey--;
bodymovey=fbodymovey;
tankypos=bodymovey+scrolly;
}
while(tankypos<150)
{
fbodymovey++;
bodymovey=fbodymovey;
tankypos=bodymovey+scrolly;
}

/*     used to show flash when tank is hit with enemy bullets
poly2=x,poly3=y            poly4=x,poly5=y
                  tank
poly0=x,poly1=y            poly6=x,poly7=y
*/


f_bodyangle=bodyangle;   /*gets angle tank is moving on*/
f_bodyangle=f_bodyangle*1.40625;   /*sets f_bodyangle from 0-256 to 0-360*/

poly[0]=bodymovex+sin(RAD*(f_bodyangle+180+45))*65;
poly[1]=tankypos+sin(RAD*(f_bodyangle+90+45))*75;

poly[2]=bodymovex+sin(RAD*(f_bodyangle+270+45))*65;
poly[3]=tankypos+sin(RAD*(f_bodyangle+180+45))*75;

poly[4]=bodymovex+sin(RAD*(f_bodyangle+45))*65;
poly[5]=tankypos+sin(RAD*(f_bodyangle+270+45))*75;

poly[6]=bodymovex+sin(RAD*(f_bodyangle+90+45))*65;
poly[7]=tankypos+sin(RAD*(f_bodyangle+45))*75;

for(c=0;c<100;c++)
{
/*used to show bullets that the badguys are shooting*/
if((enemy[c].type==1&&enemy[c].y+scrolly>-32&&enemy[c].y+scrolly<SCREEN_H&&badenergy[c]>0)||(showbadbullet[c]==TRUE))
{
if((bodymovex-enemy[c].x)!=0)
{
rise=tankypos-(enemy[c].y+scrolly);
run=bodymovex-enemy[c].x;
f_turretangle=atan(sqrt( (rise/run)*(rise/run) ) );
f_turretangle1=DEG(f_turretangle);

f_turretangle=f_turretangle1*256/360;
turretangle[c]=f_turretangle;
if(bodymovex < enemy[c].x)
{
if( enemy[c].y+scrolly > tankypos)turretangle[c]+=128;
else
turretangle[c]=128-turretangle[c];
}
else
{
if( enemy[c].y+scrolly > tankypos)turretangle[c]=256-turretangle[c];
}

}
/*shooting for badguy bullets*/
if(showbadbullet[c]==FALSE)
{
showbadbullet[c]=TRUE;
badr=-24+rand()%48;
badbulletxforce[c]=cos(RAD*( (turretangle[c]+badr)*1.40625) )*7;
badbulletyforce[c]=sin(RAD*( (turretangle[c]+badr)*1.40625) )*7;
badbulletxpos[c]=enemy[c].x;
badbulletypos[c]=enemy[c].y+scrolly;
}

if(badbulletxpos[c]>SCREEN_W||badbulletxpos[c]<0||badbulletypos[c]>SCREEN_H||badbulletypos[c]<0)
{
showbadbullet[c]=FALSE;
badbulletxforce[c]=0;
badbulletyforce[c]=0;
badbulletxpos[c]=0;
badbulletypos[c]=0;

}
/*if the tank is hit the badguy bullet disappears
and the player loses energy*/
if(bodymovex+50>badbulletxpos[c]&&tankypos+56>badbulletypos[c]&&bodymovex-50<badbulletxpos[c]&&tankypos-56<badbulletypos[c])
{
showbadbullet[c]=FALSE;
badbulletxforce[c]=0;
badbulletyforce[c]=0;
badbulletxpos[c]=0;
badbulletypos[c]=0;
getseconds2=seconds;
goodenergy--;
}
 /*moves bullet along*/
if(showbadbullet[c]==TRUE)
{
badbulletxpos[c]+=badbulletxforce[c];
badbulletypos[c]+=badbulletyforce[c];
}



}

}




 /*updates the mouse position*/
get_mouse_mickeys(&mickeyx, &mickeyy);





speed_counter--;


 }        /*updates fps*/
frame_count++;


  /*shows scrolling background*/
for(c=1;c<25;c++)
{
draw_sprite(buffer,level2[4],0,scrolly-(c*480));
}
/*shows body of tank*/
rotate_sprite(buffer,datafile[31].dat,bodymovex-50,tankypos-56,itofix(bodyangle));
/*the function below is almost the same as pivot_sprite
rotate_sprite(buffer,datafile[32].dat,bodymovex-30+sin(RAD*(gunangle*1.40625))*40,tankypos-60-cos(RAD*(gunangle*1.40625))*40,itofix(gunangle));
*/
/*shows gun of tank*/
pivot_sprite(buffer,datafile[32].dat,bodymovex,tankypos,26,109,itofix(gunangle));

/*shows flash on tank if hit*/
if(getseconds2+0.1>seconds&&getseconds2-2.0<seconds)polygon(redlight, 4, poly, makecol(84,107,96));
//(bodymovex+50>badbulletxpos[c]&&tankypos+56>badbulletypos[c]&&bodymovex-50<badbulletxpos[c]&&tankypos-56<badbulletypos[c]))
/*shows the tank bullets*/
for(c=0;c<30;c++)
{
if(showbullet[c]==TRUE)
{
circlefill(buffer,bulletxpos[c]-50,bulletypos[c]-60,5,makecol(30,50,60));
}
}
/*shows badguy bullets*/
for(c=0;c<100;c++)
{
if(showbadbullet[c]==TRUE)
{
circlefill(buffer,badbulletxpos[c],badbulletypos[c],2,makecol(215,10,70));
}
/*shows the badguy if it is on the screen*/
if(enemy[c].type==1&&enemy[c].y+scrolly>-32&&enemy[c].y+scrolly<SCREEN_H&&badenergy[c]>0)
{
draw_sprite(buffer,datafile[33].dat,enemy[c].x-16,enemy[c].y+scrolly-16);
rotate_sprite(buffer,datafile[34].dat,enemy[c].x-16,enemy[c].y+scrolly-16,itofix(turretangle[c]));
/*shows flash if badguy hit*/
if(getseconds[c]+0.1>seconds&&getseconds[c]-2.0<seconds)rectfill(redlight,enemy[c].x-16,enemy[c].y+scrolly-16,enemy[c].x+16,enemy[c].y+scrolly+16,makecol(255-(20*badenergy[c]),255-(20*badenergy[c]),255-(20*badenergy[c])));
/*uncomment this to show where badguy is aiming
line(buffer,enemy[c].x,enemy[c].y+scrolly,bodymovex,tankypos,makecol(205,110,100));
*/
}

}
                   /*uncomment this to show diagnostic text
sprintf(msg, "fps: %d ",fps );
textout(buffer, datafile[22].dat, msg, 0, 90, 0);

sprintf(msg, "fbodyangle: %.2f ",f_bodyangle );
textout(buffer, datafile[22].dat, msg, 0, 110, 0);

sprintf(msg, "bodymovex: %d ",bodymovex );
textout(buffer, datafile[22].dat, msg, 0, 130, 0);

sprintf(msg, "bodymovey: %d ",bodymovey );
textout(buffer, datafile[22].dat, msg, 0, 150, 0);

sprintf(msg, "gunangle: %d ",gunangle );
textout(buffer, datafile[22].dat, msg, 0, 170, 0);

sprintf(msg, "mousex: %d ",mouse_x );
textout(buffer, datafile[22].dat, msg, 0, 190, 0);

sprintf(msg, "mousey: %d ",mouse_y );
textout(buffer, datafile[22].dat, msg, 0, 210, 0);

sprintf(msg, "scrolly: %d ",scrolly );
textout(buffer, datafile[22].dat, msg, 0, 230, 0);

sprintf(msg, "poly0: %d ",poly[0] );
textout(buffer, datafile[22].dat, msg, 0, 250, 0);

sprintf(msg, "2: %d ",poly[2] );
textout(buffer, datafile[22].dat, msg, 0, 270, 0);

sprintf(msg, "4: %d ",poly[4] );
textout(buffer, datafile[22].dat, msg, 0, 290, 0);

sprintf(msg, "6: %d ",poly[6] );
textout(buffer, datafile[22].dat, msg, 0, 310, 0);

sprintf(msg, "mickeyx: %d ",mickeyx );
textout(buffer, datafile[22].dat, msg, 0, 330, 0);

sprintf(msg, "mickeyy: %d ",mickeyy );
textout(buffer, datafile[22].dat, msg, 0, 350, 0);


sprintf(msg, "turretangle: %d ",turretangle );
textout(buffer, datafile[22].dat, msg, 0, 370, 0);

sprintf(msg, "rise: %.2f ",rise );
textout(buffer, datafile[22].dat, msg, 0, 390, 0);

sprintf(msg, "run: %.2f ",run );
textout(buffer, datafile[22].dat, msg, 0, 410, 0);

sprintf(msg, "f_turretangle1: %.2f ",f_turretangle1 );
textout(buffer, datafile[22].dat, msg, 0, 430, 0);

sprintf(msg, "goodenergy: %d ",goodenergy);
textout(buffer, datafile[22].dat, msg, 0, 450, 0);
*/

/*good guy energy bar*/
rect(buffer,10,460,400,479,makecol(10,10,10));
rectfill(buffer,10,460,goodenergy*0.8,479,makecol(60,90,110));

  /* show_mouse(buffer);   shows the mouse after all drawing
                              to the buffer is done*/

   draw_trans_sprite(buffer,redlight,0,0);

   stretch_blit(buffer, screen,
                  0, 0, 640, 480,
                  0, 0, SCREEN_W, SCREEN_H);

   show_mouse(NULL);
   clear(buffer);
   clear_to_color(redlight,makecol(255,0,255));
}
/*set mouse speed back to normal*/
set_mouse_speed(2, 2);

return 1;

}






tankgame2()
{
/*second level with a tank this time you fight another tank*/
/*also the function above is almost the same so I didn't comment all of this*/
int badenergy=500;
int oldbadenergy=500;
float getseconds=0;
float getseconds2=0;
float getseconds3=0;
float f_badtankangle=0;
float f_badtankangle1=0;
float f_forcex=0;
float f_forcey=0;
float f_pathx=0;
float f_pathy=0;
float pathx=0;
float pathy=0;
float badspeed=2;
float rise=0;
float run=0;
float attack=0;
int quadgun=0;
int quadbody=0;
int pseconds=0;
int getpseconds=0;

int badtankangle=0;
int badbodyangle=0;
float f_badbodyangle=0;
int atenemy=0;
int mickeyx=0;
int mickeyy=0;
int adjustmagx=100;
int adjustmagy=100;
int adjustposx=50;
int adjustposy=56;

int showbullet[30];
int bulletxpos[30];
int bulletypos[30];
int bulletxforce[30];
int bulletyforce[30];

int showbadbullet=0;
int badbulletxpos=0;
int badbulletypos=0;
int badbulletxpos1=0;
int badbulletypos1=0;
int badbulletxforce=0;
int badbulletyforce=0;
int badr=0;

int aiming=0;
float aimingseconds=0;
float showbadx=0;
float showbady=0;
int bodyangle=0;
float f_bodyangle=0;
int gunangle=0;
float  fbodymovex=2500;
float  fbodymovey=3500;
float bodymovex=0;
float bodymovey=0;
int tankypos=0;
int goodenergy=500;
int speed=4;
int scrolly=480;
int c=0;
int i=0;
int getposy=0;
int getposx=0;
int getposy2=0;
int getposx2=0;
int badhit[8];

struct BADGUYS
{
int x,y,type;

};
struct BADGUYS enemy;

memset(bulletxpos,0,sizeof(bulletxpos));
memset(bulletxforce,0,sizeof(bulletxforce));
memset(bulletypos,0,sizeof(bulletxpos));
memset(bulletyforce,0,sizeof(bulletxforce));
memset(showbullet,FALSE,sizeof(showbullet));



enemy.x=2000;
enemy.y=2000;
set_trans_blender(0, 0, 0, 196);
/*sets mouse speed to very low for moving gun*/
set_mouse_speed(80, 80);

clear(buffer);
for(;;)
{
/*shows title screen*/
if(mouse_b&2)break;
sprintf(msg, "MISSION START!" );
textout_centre(buffer, datafile[22].dat, msg, SCREEN_W/2, 30, 0);
sprintf(msg, "YOU ARE PAST THE FIRST LINE OF DEFENSE" );
textout_centre(buffer, datafile[22].dat, msg, SCREEN_W/2, 60, 0);
sprintf(msg, "ONE MORE BAD GUY LEFT" );
textout_centre(buffer, datafile[22].dat, msg, SCREEN_W/2, 90, 0);
sprintf(msg, "CONTROLS:" );
textout_centre(buffer, datafile[22].dat, msg, SCREEN_W/2, 150, 0);
sprintf(msg, "ARROW KEYS TO MOVE THE TANK" );
textout_centre(buffer, datafile[22].dat, msg, SCREEN_W/2, 180, 0);
sprintf(msg, "MOUSE TO MOVE THE GUN" );
textout_centre(buffer, datafile[22].dat, msg, SCREEN_W/2, 210, 0);

sprintf(msg, "PASSWORD : TANKGAME2" );
textout_centre(buffer, datafile[22].dat, msg, SCREEN_W/2, 240, 0);

sprintf(msg, "RIGHT CLICK TO START" );
textout_centre(buffer, datafile[22].dat, msg, SCREEN_W/2, 270, 0);

blit(buffer, screen,0, 0,0,0, SCREEN_W, SCREEN_H);
show_mouse(NULL);
clear_to_color(buffer,makecol(196,196,196));

}
speed_counter=0;
for(;;)
{
if(key[KEY_ESC])
{
level=QUIT;
break;
}
if(goodenergy==0)
{
level=GAMEOVER;
break;
}
if(badenergy==0)
{
level=5;
break;
}
while(speed_counter > 0)

{
if((mouse_b&1)&&mousepressed>0)mousepressed++;
if((mouse_b&1)&&mousepressed==0)mousepressed=1;
if(!(mouse_b&1))mousepressed=0;
if(key[KEY_RIGHT])
{
bodyangle+=2;
if (bodyangle>256)bodyangle=0;
}
if(key[KEY_LEFT])
{
bodyangle-=2;
if (bodyangle<-256)bodyangle=0;
}

if(key[KEY_DOWN])
{

fbodymovey=cos(RAD*(bodyangle*1.40625))*speed;
fbodymovex=-sin(RAD*(bodyangle*1.40625))*speed;

}
if(key[KEY_UP])
{
fbodymovey=-cos(RAD*(bodyangle*1.40625))*speed;
fbodymovex=sin(RAD*(bodyangle*1.40625))*speed;

}

gunangle+=sin(RAD*(gunangle*1.40625))*mickeyy;
gunangle+=cos(RAD*(gunangle*1.40625))*mickeyx;
if (gunangle>256)gunangle-=256;

if (gunangle<-256)gunangle+=256;

if(key[KEY_Q])
{
badspeed+=0.05;
}
if(key[KEY_W])
{
badspeed-=0.05;
}
/*
if(key[KEY_E])
{
adjustmagx++;
}
if(key[KEY_R])
{
adjustmagx--;
}
if(key[KEY_T])
{
adjustposy++;
}
if(key[KEY_Y])
{
adjustposy--;
}
if(key[KEY_U])
{
adjustmagy++;
}
if(key[KEY_I])
{
adjustmagy--;
}
*/
mousepressed%=3;//used for autofire
for(c=0;c<30;c++)
{
if(mousepressed==1&&showbullet[c]==FALSE)
{
mousepressed=2;
showbullet[c]=TRUE;
bulletxforce[c]=sin(RAD*(gunangle*1.40625))*10;
bulletyforce[c]=-cos(RAD*(gunangle*1.40625))*10;
bulletxpos[c]=adjustposx+bodymovex+sin(RAD*(gunangle*1.40625))*adjustmagx;
bulletypos[c]=adjustposy+tankypos-cos(RAD*(gunangle*1.40625))*adjustmagy;
}

if(bulletxpos[c]+200-bodymovex>SCREEN_W||bulletxpos[c]+200-bodymovex<0||bulletypos[c]+200-tankypos>SCREEN_H||bulletypos[c]+200-tankypos<0)
{
showbullet[c]=FALSE;
bulletxforce[c]=0;
bulletyforce[c]=0;
bulletxpos[c]=0;
bulletypos[c]=0;
}


if(showbullet[c]==TRUE)
{
bulletxpos[c]+=bulletxforce[c];
bulletypos[c]+=bulletyforce[c];

if((bulletxpos[c]+200-bodymovex>showbadx)&&(bulletxpos[c]+200-bodymovex<showbadx+100)&&(bulletypos[c]+200-tankypos>showbady)&&(bulletypos[c]+200-tankypos<showbady+112)&&badenergy>0)
{
showbullet[c]=FALSE;
bulletxforce[c]=0;
bulletyforce[c]=0;
bulletxpos[c]=0;
bulletypos[c]=0;
getseconds=seconds;
badenergy--;
}
}
}

if(bodymovex>6400)bodymovex=6400;
if(bodymovex<0)bodymovex=0;

bodymovex+=fbodymovex;
bodymovey+=fbodymovey;
fbodymovex=0;
fbodymovey=0;


tankypos=bodymovey+scrolly;

while(tankypos>4800)
{
bodymovey--;
tankypos=bodymovey+scrolly;
}
while(tankypos<0)
{
bodymovey++;
tankypos=bodymovey+scrolly;
}

/*     used to show flash when a tank is hit with bullets
poly2=x,poly3=y            poly4=x,poly5=y
                  tank
poly0=x,poly1=y            poly6=x,poly7=y
*/
f_bodyangle=bodyangle;
f_bodyangle=f_bodyangle*1.40625;
poly[0]=250+sin(RAD*(f_bodyangle+180+45))*65;
poly[1]=256+sin(RAD*(f_bodyangle+90+45))*75;

poly[2]=250+sin(RAD*(f_bodyangle+270+45))*65;
poly[3]=256+sin(RAD*(f_bodyangle+180+45))*75;

poly[4]=250+sin(RAD*(f_bodyangle+45))*65;
poly[5]=256+sin(RAD*(f_bodyangle+270+45))*75;

poly[6]=250+sin(RAD*(f_bodyangle+90+45))*65;
poly[7]=256+sin(RAD*(f_bodyangle+45))*75;

badhit[0]=showbadx+50+sin(RAD*((f_badbodyangle*1.40625)+180+45))*65;
badhit[1]=showbady+56+sin(RAD*((f_badbodyangle*1.40625)+90+45))*75;

badhit[2]=showbadx+50+sin(RAD*((f_badbodyangle*1.40625)+270+45))*65;
badhit[3]=showbady+56+sin(RAD*((f_badbodyangle*1.40625)+180+45))*75;

badhit[4]=showbadx+50+sin(RAD*((f_badbodyangle*1.40625)+45))*65;
badhit[5]=showbady+56+sin(RAD*((f_badbodyangle*1.40625)+270+45))*75;

badhit[6]=showbadx+50+sin(RAD*((f_badbodyangle*1.40625)+90+45))*65;
badhit[7]=showbady+56+sin(RAD*((f_badbodyangle*1.40625)+45))*75;
/*if statement used to decide if badtank is to run or fight
if it gets hit too many times in ten seconds it runs*/
if(getseconds3<seconds)
{
getseconds3=seconds+10;
if(badenergy+40<oldbadenergy)
{
badspeed=-6;
if(badenergy+80<oldbadenergy)
badspeed=-9;
}
else
{
badspeed=4;
}
oldbadenergy=badenergy;
}
/*used to have bad tank aim for a while before shooting
to give the good tank more time to escape*/
if(aiming==0)
{

f_forcey=-cos(RAD*(f_badbodyangle*1.40625))*badspeed;
f_forcex=sin(RAD*(f_badbodyangle*1.40625))*badspeed;

if(f_pathx+f_forcex>0&&f_pathx+f_forcex<6400) f_pathx+=f_forcex;
if(f_pathy+f_forcey>0&&f_pathy+f_forcey<4800) f_pathy+=f_forcey;

if(enemy.x<0)f_pathx++;
if(enemy.x>6400)f_pathx--;

if(enemy.y<0)f_pathy++;
if(enemy.y>4800)f_pathy--;

enemy.x=f_pathx;
enemy.y=f_pathy;

showbadx=enemy.x-bodymovex+200;
showbady=enemy.y-tankypos+200;

rise=tankypos-enemy.y;
run=bodymovex-enemy.x;
f_badtankangle=atan(sqrt( (rise/run)*(rise/run) ) );
f_badtankangle1=DEG(f_badtankangle);

f_badtankangle=f_badtankangle1*256/360;
badtankangle=f_badtankangle;

if(bodymovex < enemy.x)
{
if( enemy.y > tankypos)badtankangle+=128;
else
badtankangle=128-badtankangle;
}
else
{
if( enemy.y > tankypos)badtankangle=256-badtankangle;
}
badtankangle+=64;
if(badtankangle>256)badtankangle-=256;
/*direction towards the good tank is the same
as the badtankangle I added + 64 because I drew the tank
90degrees fliped*/

 /*the follwing section uses the distance the badtank is from
 the good tank and decides if the bad tank show rotate clockwise or
 counterclockwise*/
/*check if two angles are in same quadrant
2    1
3    4
*/
if(badtankangle>=0&&badtankangle<64)
quadgun=1;
if(badtankangle>=64&&badtankangle<128)
quadgun=2;
if(badtankangle>=128&&badtankangle<196)
quadgun=3;
if(badtankangle>=196&&badtankangle<256)
quadgun=4;

if(f_badbodyangle>=0&&f_badbodyangle<64)
quadbody=1;
if(f_badbodyangle>=64&&f_badbodyangle<128)
quadbody=2;
if(f_badbodyangle>=128&&f_badbodyangle<196)
quadbody=3;
if(f_badbodyangle>=196&&f_badbodyangle<256)
quadbody=4;
if(quadgun==quadbody)
{
if(badtankangle>f_badbodyangle) f_badbodyangle+=1.5;
if(badtankangle<f_badbodyangle) f_badbodyangle-=1.5;
}

else
{
if(quadgun!=1&&quadgun!=4&&quadbody!=1&&quadbody!=4)
{
if(quadbody<quadgun)f_badbodyangle+=1.5;
else f_badbodyangle-=1.5;
}

else
{
if(quadgun==1||quadgun==4)
{
if(quadgun==1)
{
if(f_badbodyangle-badtankangle>128)f_badbodyangle+=1.5;
else f_badbodyangle-=1.5;
}

if(quadgun==4)
{
if(badtankangle-f_badbodyangle>128)f_badbodyangle-=1.5;
else f_badbodyangle+=1.5;
}
}
else
{
if(quadbody==4)
{
if(f_badbodyangle-badtankangle>128)f_badbodyangle+=1.5;
else f_badbodyangle-=1.5;
}

if(quadbody==1)
{
if(badtankangle-f_badbodyangle>128)f_badbodyangle-=1.5;
else f_badbodyangle+=1.5;
}

}
}
}
if(f_badbodyangle>=256)f_badbodyangle=0;
if(f_badbodyangle<0)f_badbodyangle+=256;
if(showbadx>-60&&showbadx<640&&showbady>-50&&showbady<480&&aimingseconds+0.25<seconds&&showbadbullet==FALSE)
{
getposx=bodymovex;
getposy=tankypos;
aiming=1;
aimingseconds=seconds;
}
}
/*has badguy shooting if it is on the screen*/
while(showbadx>100&&showbadx<300&&showbady>100&&showbady<300)
{
showbadx=enemy.x-bodymovex+200;
showbady=enemy.y-tankypos+200;
enemy.x=f_pathx;
enemy.y=f_pathy;
if(showbadx<200&&showbadx>100)f_pathx--;
if(showbadx>=200&&showbadx<300) f_pathx++;
if(showbady<200&&showbady>100)f_pathy--;
if(showbady>=200&&showbady<300) f_pathy++;
}

if(aiming>0)
{
showbadx=enemy.x-bodymovex+200;
showbady=enemy.y-tankypos+200;


getposx2=getposx-bodymovex;


getposy2=getposy-tankypos;

if(aimingseconds+0.45<seconds&&showbadbullet==FALSE)
{
/*badguy bullets*/
getposx=bodymovex;
getposy=tankypos;


showbadbullet=TRUE;

badbulletxforce=sin(RAD*( (badtankangle)*1.40625) )*14;
badbulletyforce=-cos(RAD*( (badtankangle)*1.40625) )*14;
badbulletxpos1=adjustposx+showbadx+sin(RAD*(badtankangle*1.40625))*adjustmagx;
badbulletypos1=adjustposy+showbady-cos(RAD*(badtankangle*1.40625))*adjustmagy;
aimingseconds=seconds;
aiming=0;
}
}

if(badbulletxpos>SCREEN_W||badbulletxpos<0||badbulletypos>SCREEN_H||badbulletypos<0)
{
showbadbullet=FALSE;
badbulletxforce=0;
badbulletyforce=0;
badbulletxpos=0;
badbulletypos=0;
badbulletxpos1=0;
badbulletypos1=0;

}

if(badbulletxpos>200&&badbulletypos>200&&badbulletxpos<300&&badbulletypos<312)
{
showbadbullet=FALSE;
badbulletxforce=0;
badbulletyforce=0;
badbulletxpos=0;
badbulletypos=0;

badbulletxpos1=0;
badbulletypos1=0;

getseconds2=seconds;
goodenergy-=10;
}

if(showbadbullet==TRUE)
{
badbulletxpos1+=badbulletxforce;
badbulletypos1+=badbulletyforce;
getposx2=getposx-bodymovex;
getposy2=getposy-tankypos;
badbulletxpos=badbulletxpos1+getposx2;
badbulletypos=badbulletypos1+getposy2;
}






get_mouse_mickeys(&mickeyx, &mickeyy);





speed_counter--;


 }
frame_count++;



for(c=0;c<11;c++)
{
for(i=0;i<11;i++)
draw_sprite(buffer,level2[6],(i*640)-bodymovex,(c*480)-tankypos);
}
badbodyangle=f_badbodyangle;
rotate_sprite(buffer,datafile[31].dat,200,200,itofix(bodyangle));
pivot_sprite(buffer,datafile[32].dat,250,256,26,109,itofix(gunangle));
if(getseconds2+0.1>seconds&&getseconds2-2.0<seconds)polygon(redlight, 4, poly, makecol(84,107,96));

for(c=0;c<30;c++)
if(showbullet[c]==TRUE)
circlefill(buffer,bulletxpos[c]+200-bodymovex,bulletypos[c]+200-tankypos,5,makecol(30,50,60));


rotate_sprite(buffer,datafile[31].dat,showbadx,showbady,itofix(badbodyangle));
pivot_sprite(buffer,datafile[32].dat,enemy.x+250-bodymovex,enemy.y+256-tankypos,26,109,itofix(badtankangle));
if(getseconds+0.1>seconds&&getseconds-2.0<seconds)polygon(redlight,4,badhit,makecol(255,255,255));


if(aiming>0)
line(buffer,adjustposx+showbadx+sin(RAD*(badtankangle*1.40625))*adjustmagx
,adjustposy+showbady-cos(RAD*(badtankangle*1.40625))*adjustmagy,250+getposx2,256+getposy2,makecol((seconds-aimingseconds)*200,50,50));

if(showbadbullet==TRUE)
circlefill(buffer,badbulletxpos,badbulletypos,5,makecol(215,10,70));

/*
sprintf(msg, "fps: %d ",fps );
textout(buffer, datafile[22].dat, msg, 0, 90, 0);

sprintf(msg, "fbodyangle: %.2f ",f_bodyangle );
textout(buffer, datafile[22].dat, msg, 0, 110, 0);

sprintf(msg, "bodymovex: %.2f ",bodymovex );
textout(buffer, datafile[22].dat, msg, 0, 130, 0);

sprintf(msg, "bodymovey: %.2f ",bodymovey );
textout(buffer, datafile[22].dat, msg, 0, 150, 0);

sprintf(msg, "oldbadenergy: %d ",oldbadenergy );
textout(buffer, datafile[22].dat, msg, 0, 170, 0);

sprintf(msg, "badenergy: %d ",badenergy );
textout(buffer, datafile[22].dat, msg, 0, 190, 0);

sprintf(msg, "seconds: %.2f ",seconds );
textout(buffer, datafile[22].dat, msg, 0, 210, 0);

sprintf(msg, "seconds3: %.2f ",getseconds3 );
textout(buffer, datafile[22].dat, msg, 0, 230, 0);

sprintf(msg, "badspeed: %.2f ",badspeed );
textout(buffer, datafile[22].dat, msg, 0, 250, 0);

sprintf(msg, "f_badbodyangle: %.2f ",f_badbodyangle );
textout(buffer, datafile[22].dat, msg, 0, 270, 0);

sprintf(msg, "badbulletxpos: %d ",badbulletxpos );
textout(buffer, datafile[22].dat, msg, 0, 290, 0);

sprintf(msg, "badbulletypos: %d ",badbulletypos );
textout(buffer, datafile[22].dat, msg, 0, 310, 0);

sprintf(msg, "mickeyx: %d ",mickeyx );
textout(buffer, datafile[22].dat, msg, 0, 330, 0);

sprintf(msg, "mickeyy: %d ",mickeyy );
textout(buffer, datafile[22].dat, msg, 0, 350, 0);

sprintf(msg, "turretangle: %d ",turretangle );
textout(buffer, datafile[22].dat, msg, 0, 370, 0);

sprintf(msg, "enemy.x : %.2f ",showbadx );
textout(buffer, datafile[22].dat, msg, 0, 390, 0);

sprintf(msg, "enemy.y : %.2f ",showbady );
textout(buffer, datafile[22].dat, msg, 0, 410, 0);

sprintf(msg, "f_turretangle1: %.2f ",f_badtankangle1 );
textout(buffer, datafile[22].dat, msg, 0, 430, 0);

sprintf(msg, "goodenergy: %d ",goodenergy);
textout(buffer, datafile[22].dat, msg, 0, 450, 0);
*/
/*energy bars*/
rect(buffer,10,440,400,459,makecol(10,10,10));
rectfill(buffer,10,440,badenergy*0.8+10,459,makecol(90,40,25));

rect(buffer,10,460,400,479,makecol(10,10,10));
rectfill(buffer,10,460,goodenergy*0.8+10,479,makecol(60,90,110));

/*show radar*/
rect(buffer,500,300,600,400,makecol(10,10,10));
/*show dot for good guy*/
   putpixel(buffer,(500+(bodymovex*100/6400))-1,(300+(tankypos*100/4800))-1,makecol(25,75,25));
   putpixel(buffer,(500+(bodymovex*100/6400)),(300+(tankypos*100/4800))-1,makecol(25,75,25));
   putpixel(buffer,(500+(bodymovex*100/6400))+1,(300+(tankypos*100/4800))-1,makecol(25,75,25));
   putpixel(buffer,(500+(bodymovex*100/6400))-1,(300+(tankypos*100/4800)),makecol(25,75,25));
   putpixel(buffer,(500+(bodymovex*100/6400))+1,(300+(tankypos*100/4800)),makecol(25,75,25));
   putpixel(buffer,(500+(bodymovex*100/6400))-1,(300+(tankypos*100/4800))+1,makecol(25,75,25));
   putpixel(buffer,(500+(bodymovex*100/6400)),(300+(tankypos*100/4800))+1,makecol(25,75,25));
   putpixel(buffer,(500+(bodymovex*100/6400))+1,(300+(tankypos*100/4800))+1,makecol(25,75,25));

/*show dot for bad guy*/
   putpixel(buffer,(500+(enemy.x*100/6400))-1,(300+(enemy.y*100/4800))-1,makecol(75,25,25));
   putpixel(buffer,(500+(enemy.x*100/6400))+1,(300+(enemy.y*100/4800))-1,makecol(75,25,25));
   putpixel(buffer,(500+(enemy.x*100/6400)),(300+(enemy.y*100/4800)),makecol(75,25,25));
   putpixel(buffer,(500+(enemy.x*100/6400))-1,(300+(enemy.y*100/4800))+1,makecol(75,25,25));
   putpixel(buffer,(500+(enemy.x*100/6400))+1,(300+(enemy.y*100/4800))+1,makecol(75,25,25));

  /* show_mouse(buffer);   shows the mouse after all drawing
                              to the buffer is done*/

   draw_trans_sprite(buffer,redlight,0,0);
   stretch_blit(buffer, screen,
                  0, 0, 640, 480,
                  0, 0, SCREEN_W, SCREEN_H);
   show_mouse(NULL);
   clear(buffer);
   clear_to_color(redlight,makecol(255,0,255));
}

set_mouse_speed(2, 2);
return 1;

}



