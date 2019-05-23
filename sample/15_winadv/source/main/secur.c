camerascreen()
{
/*function shows the monitor screen that has images of
the lab and the stock room*/
BITMAP *cameras;
cameras=create_bitmap(SCREEN_W,SCREEN_H);
clear(cameras);
clear_to_color(redlight,makecol(255,0,255));
/*changes color of background if lights are off*/
if(lablights==1)rectfill(redlight,320,0,640,240,makecol(32,32,48));
if(stocklights==1)rectfill(redlight,320,240,640,480,makecol(32,32,48));
/*makes the stock room and the lab room appear on same screen on the side*/
stretch_blit(level2[1], cameras,
                  0, 0, 640, 480,
                  320, 0, 320, 240);

stretch_blit(level2[3], cameras,
                  0, 0, 640, 480,
                  320, 240, 320, 240);
speed_counter=0;

for(;;)
{
if(pace2<625&&movies==0)
{
fseek(f5,sizeof(struct mover)*(pace2),SEEK_SET);
                fread(&guy1,sizeof(struct mover),1,f5);
pace2%=439;
pace2++;
}
if(movies==0||pace2==625)
{

if(mouse_x>SCREEN_W/2-150&&mouse_x<SCREEN_W/2+150&&mouse_y>SCREEN_H/2+150&&mouse_y<SCREEN_H/2+250)
{
if(mousepressed==2)  break;
}

}

/*shows a movie when the lights in the stock room turn off*/
if(pace2<625&&stocklights==1)movies=5;

/*the following if statements shows if doors are open or closed
and if the lights are on or off*/
while(speed_counter > 0 && movies==0)
{
if((mouse_b&1)&&mousepressed==1)mousepressed=2;
if((mouse_b&1)&&mousepressed==0)mousepressed=1;
if(!(mouse_b&1))mousepressed=0;

blit(cameras,buffer,0, 0,0,0, SCREEN_W, SCREEN_H);
sprintf(msg, "EXIT");
if(mouse_x>SCREEN_W/2-150&&mouse_x<SCREEN_W/2+150&&mouse_y>SCREEN_H/2+150&&mouse_y<SCREEN_H/2+250)
{
textout_centre(buffer, datafile[25].dat, msg, SCREEN_W/2-50, (SCREEN_H/2)+200,makecol(150,225,200));
if(mousepressed==2)  break;
}
else
textout_centre(buffer, datafile[25].dat, msg, SCREEN_W/2-50, (SCREEN_H/2)+200,makecol(175,175,175));

if(object[8][DOOR1][OPEN].state==0)sprintf(msg, "OPEN LAB DOORS");
else sprintf(msg, "CLOSE LAB DOORS");

if(mouse_x<320&&mouse_y<50)
{
textout(buffer, datafile[25].dat, msg, 0, 10,makecol(150,225,200));
if(mousepressed==1)
  {
if(object[8][DOOR1][OPEN].state==0)
       {
object[8][DOOR1][OPEN].state=1;
object[8][DOOR2][OPEN].state=1;
       }
else
   {
object[8][DOOR1][OPEN].state=0;
object[8][DOOR2][OPEN].state=0;
   }
}

}
else
textout(buffer, datafile[25].dat, msg, 0, 10,makecol(175,175,175));

if(lablights==0)sprintf(msg, "TURN OFF LAB LIGHTS");
else sprintf(msg, "TURN ON LAB LIGHTS");

if(mouse_x<320&&mouse_y>50&&mouse_y<100)
{
textout(buffer, datafile[25].dat, msg, 0, 70,makecol(150,225,200));
if(mousepressed==1)
 {
if(lablights==0)
{
lablights=1;
rectfill(redlight,320,0,640,240,makecol(32,32,48));
}
else
    {
lablights=0;
rectfill(redlight,320,0,640,240,makecol(255,0,255));
    }

 }

}
else
textout(buffer, datafile[25].dat, msg, 0, 70,makecol(175,175,175));

if(object[10][DOOR1][OPEN].state==0)sprintf(msg, "OPEN STOCK DOORS");
else sprintf(msg, "CLOSE STOCK DOORS");

if(mouse_x<320&&mouse_y>240&&mouse_y<300)
{
textout(buffer, datafile[25].dat, msg, 0, 250,makecol(150,225,200));

if(mousepressed==1)
  {
if(object[10][DOOR1][OPEN].state==0)
       {
object[10][DOOR1][OPEN].state=1;
object[10][DOOR2][OPEN].state=1;
       }
else
   {
object[10][DOOR1][OPEN].state=0;
object[10][DOOR2][OPEN].state=0;
   }
  }
}
else
textout(buffer, datafile[25].dat, msg, 0, 250,makecol(175,175,175));

if(stocklights==0)sprintf(msg, "TURN OFF STOCK LIGHTS");
else sprintf(msg, "TURN ON STOCK LIGHTS");

if(mouse_x<320&&mouse_y>300&&mouse_y<350)
{
if(mousepressed==1)
 {
if(stocklights==0)
{
stocklights=1;
rectfill(redlight,320,240,640,480,makecol(32,32,48));
}
else
    {
stocklights=0;
rectfill(redlight,320,240,640,480,makecol(255,0,255));
    }
 }
textout(buffer, datafile[25].dat, msg, 0, 310,makecol(150,225,200));
}

else
textout(buffer, datafile[25].dat, msg, 0, 310,makecol(175,175,175));

speed_counter--;
}

while(speed_counter > 0 && movies==5)
{
/*shows movie with bad guys in the stock room*/
moviesinput(movies);
speed_counter--;
}

if(movies==0)
{
/*shows if doors are open*/
if(object[8][DOOR1][OPEN].state==1)
{
poly[0]=573;
poly[1]=64;
poly[2]=573;
poly[3]=8;
poly[4]=608;
poly[5]=6;
poly[6]=606;
poly[7]=64;
polygon(buffer, 4, poly, makecol(0,0,0));
poly[0]=628;
poly[1]=147;
poly[2]=628;
poly[3]=85;
poly[4]=639;
poly[5]=85;
poly[6]=639;
poly[7]=147;
polygon(buffer, 4, poly, makecol(0,0,0));
}
if(object[10][DOOR1][OPEN].state==1)
{
poly[0]=558;
poly[1]=308;
poly[2]=558;
poly[3]=260;
poly[4]=590;
poly[5]=260;
poly[6]=590;
poly[7]=308;
polygon(buffer, 4, poly, makecol(0,0,0));
poly[0]=415;
poly[1]=450;
poly[2]=412;
poly[3]=389;
poly[4]=455;
poly[5]=389;
poly[6]=458;
poly[7]=450;
polygon(buffer, 4, poly, makecol(178,86,68));

}
stretch_sprite(buffer, character[12], 420, 70+240, 25,68);

if(pace2<625)
{
stretch_sprite(buffer, character[guy1.show+9], (guy1.x+8)/2+320, (guy1.y+10)/2+240, 25,68);
}
draw_trans_sprite(buffer, redlight, 0, 0);
}

if(movies==5)
{
clear_to_color(redlight,makecol(32,32,48));
moviedisplay1(movies);
}




show_mouse(buffer);   /*shows the mouse after all drawing
                              to the buffer is done*/
    stretch_blit(buffer, screen,
                  0, 0, 640, 480,
                  0, 0, SCREEN_W, SCREEN_H);
   show_mouse(NULL);
   clear(buffer);

}

destroy_bitmap(cameras);
mousepressed=2;
return 1;
}

ventscreen()
{
/*shows screen for the vent game that is used to shut off the gas*/

/* variables for changing the size of the game*/
int offsetx=100;
int width=20;
int height=12;
int offsety=50;
/*variables for gates 1 for closed and 0 for original position*/
int redgate=0;
int greengate=0;
int bluegate=0;
int yellowgate=0;

while (!(mousepressed==1&&mouse_x>=(10*width)+offsetx-50&&mouse_x<=(10*width)+offsetx+50&&mouse_y>=(height*32)+offsety-20&&mouse_y<=(height*32)+offsety+20))
       {
if((mouse_b&1)&&mousepressed==1)mousepressed=2;
if((mouse_b&1)&&mousepressed==0)mousepressed=1;
if(!(mouse_b&1))mousepressed=0;
/*as player clicks on the squares the gates open and close*/
if((mousepressed==1)&&(mouse_x>= ((1*width)+offsetx))&& (mouse_y>=((20*height)+offsety))&& (mouse_x<=((4*width)+offsetx))&& (mouse_y<=((22*height)+offsety)))
{
if (redgate==0)redgate=1;
else redgate=0;
}
if((mousepressed==1)&&(mouse_x>= ((6*width)+offsetx))&& (mouse_y>=((20*height)+offsety))&& (mouse_x<=((9*width)+offsetx))&& (mouse_y<=((22*height)+offsety)))
{
if (greengate==0)greengate=1;
else greengate=0;
}
if((mousepressed==1)&&(mouse_x>= ((11*width)+offsetx))&& (mouse_y>=((20*height)+offsety))&& (mouse_x<=((14*width)+offsetx))&& (mouse_y<=((22*height)+offsety)))
{
if (bluegate==0)bluegate=1;
else bluegate=0;
}
if((mousepressed==1)&&(mouse_x>= ((16*width)+offsetx))&& (mouse_y>=((20*height)+offsety))&& (mouse_x<=((19*width)+offsetx))&& (mouse_y<=((22*height)+offsety)))
{
if (yellowgate==0)yellowgate=1;
else yellowgate=0;
}


 /*shows the text on the screen*/
sprintf(msg, "FAN");
textout(buffer, datafile[25].dat, msg, offsetx, offsety-50,makecol(150,225,200));

sprintf(msg, "OUTSIDE");
textout(buffer, datafile[25].dat, msg, (9*width)+offsetx, (height*16)+offsety,makecol(150,225,200));

sprintf(msg, "BEAM ROOM");
textout(buffer, datafile[25].dat, msg, (20*width)+offsetx, (height*6)+offsety,makecol(150,225,200));


sprintf(msg, "RED");
textout_centre(buffer, datafile[25].dat, msg, (2.5*width)+offsetx, (height*18)+offsety,makecol(150,225,200));

sprintf(msg, "GREEN");
textout_centre(buffer, datafile[25].dat, msg, (7.5*width)+offsetx, (height*18)+offsety,makecol(150,225,200));

sprintf(msg, "BLUE");
textout_centre(buffer, datafile[25].dat, msg, (12.5*width)+offsetx, (height*18)+offsety,makecol(150,225,200));

sprintf(msg, "YELLOW");
textout_centre(buffer, datafile[25].dat, msg, (17.5*width)+offsetx, (height*18)+offsety,makecol(150,225,200));
/*RED SELECTION*/
rectfill(buffer, (1*width)+offsetx, (20*height)+offsety, (4*width)+offsetx, (22*height)+offsety, makecol(225,64,64));
/*GREEN SELECTION*/
rectfill(buffer, (6*width)+offsetx, (20*height)+offsety, (9*width)+offsetx, (22*height)+offsety, makecol(64,225,64));
/*BLUE SELECTION*/
rectfill(buffer, (11*width)+offsetx, (20*height)+offsety, (14*width)+offsetx, (22*height)+offsety, makecol(64,64,225));
/*YELLOW SELECTION*/
rectfill(buffer, (16*width)+offsetx, (20*height)+offsety, (19*width)+offsetx, (22*height)+offsety, makecol(200,200,64));


/*fan entrance*/
rectfill(buffer, (0*width)+offsetx, (0*height)+offsety, (2*width)+offsetx, (1*height)+offsety, makecol(235,235,235));
/*beam entrance*/
rectfill(buffer, (19*width)+offsetx, (5*height)+offsety, (20*width)+offsetx, (7*height)+offsety, makecol(235,235,235));
/*outside entrance*/
rectfill(buffer, (9*width)+offsetx, (15*height)+offsety, (11*width)+offsetx, (16*height)+offsety, makecol(235,235,235));


sprintf(msg, "CLICK COLORED SQUARES TO");
textout_centre(buffer, datafile[25].dat, msg, (10*width)+offsetx, (height*27)+offsety,makecol(175,175,175));
sprintf(msg, "CHANGE DIRECTION OF GAS");
textout_centre(buffer, datafile[25].dat, msg, (10*width)+offsetx, (height*29)+offsety,makecol(175,175,175));
sprintf(msg, "EXIT");
if(mouse_x>=(10*width)+offsetx-50&&mouse_x<=(10*width)+offsetx+50&&mouse_y>=(height*32)+offsety-20&&mouse_y<=(height*32)+offsety+20)
textout_centre(buffer, datafile[25].dat, msg, (10*width)+offsetx, (height*32)+offsety,makecol(150,225,200));
else
textout_centre(buffer, datafile[25].dat, msg, (10*width)+offsetx, (height*32)+offsety,makecol(175,175,175));

/*draws the gates*/
if (redgate==0)
{
rectfill(buffer, (2*width)+offsetx, (1*height)+offsety, (3*width)+offsetx, (3*height)+offsety, makecol(225,64,64));
rectfill(buffer, (13*width)+offsetx, (11*height)+offsety, (15*width)+offsetx, (12*height)+offsety, makecol(225,64,64));
}
else
{
rectfill(buffer, (0*width)+offsetx, (3*height)+offsety, (2*width)+offsetx, (4*height)+offsety, makecol(225,64,64));
rectfill(buffer, (15*width)+offsetx, (9*height)+offsety, (16*width)+offsetx, (11*height)+offsety, makecol(225,64,64));
}

if (greengate==0)
{
rectfill(buffer, (7*width)+offsetx, (5*height)+offsety, (8*width)+offsetx, (7*height)+offsety, makecol(64,225,64));
rectfill(buffer, (7*width)+offsetx, (11*height)+offsety, (9*width)+offsetx, (12*height)+offsety, makecol(64,225,64));
}
else
{
rectfill(buffer, (5*width)+offsetx, (7*height)+offsety, (7*width)+offsetx, (8*height)+offsety, makecol(64,225,64));
rectfill(buffer, (9*width)+offsetx, (9*height)+offsety, (10*width)+offsetx, (11*height)+offsety, makecol(64,225,64));

}

if (bluegate==0)
{
rectfill(buffer, (7*width)+offsetx, (1*height)+offsety, (8*width)+offsetx, (3*height)+offsety, makecol(64,64,225));
rectfill(buffer, (9*width)+offsetx, (14*height)+offsety, (11*width)+offsetx, (15*height)+offsety, makecol(64,64,225));
}
else
{
rectfill(buffer, (5*width)+offsetx, (3*height)+offsety, (7*width)+offsetx, (4*height)+offsety, makecol(64,64,225));
rectfill(buffer, (11*width)+offsetx, (13*height)+offsety, (12*width)+offsetx, (15*height)+offsety, makecol(64,64,225));
}

if (yellowgate==0)
{
rectfill(buffer, (12*width)+offsetx, (1*height)+offsety, (13*width)+offsetx, (3*height)+offsety, makecol(200,200,64));
rectfill(buffer, (0*width)+offsetx, (7*height)+offsety, (2*width)+offsetx, (8*height)+offsety, makecol(200,200,64));
}
else
{
rectfill(buffer, (10*width)+offsetx, (3*height)+offsety, (12*width)+offsetx, (4*height)+offsety, makecol(200,200,64));
rectfill(buffer, (2*width)+offsetx, (5*height)+offsety, (3*width)+offsetx, (7*height)+offsety, makecol(200,200,64));
}
/*draws the outline for the game*/

/*square 1*/
rectfill(buffer, (2*width)+offsetx, (3*height)+offsety, (5*width)+offsetx, (5*height)+offsety, makecol(128,128,196));

/*square 2*/
rectfill(buffer, (7*width)+offsetx, (3*height)+offsety, (10*width)+offsetx, (5*height)+offsety, makecol(128,128,196));

/*square 3*/
rectfill(buffer, (12*width)+offsetx, (3*height)+offsety, (15*width)+offsetx, (5*height)+offsety, makecol(128,128,196));

/*square 4*/
rectfill(buffer, (2*width)+offsetx, (7*height)+offsety, (5*width)+offsetx, (9*height)+offsety, makecol(128,128,196));

/*square 5*/
rectfill(buffer, (7*width)+offsetx, (7*height)+offsety, (10*width)+offsetx, (9*height)+offsety, makecol(128,128,196));

/*square 6*/
rectfill(buffer, (12*width)+offsetx, (7*height)+offsety, (15*width)+offsetx, (9*height)+offsety, makecol(128,128,196));

/*square 7*/
rectfill(buffer, (9*width)+offsetx, (11*height)+offsety, (13*width)+offsetx, (13*height)+offsety, makecol(128,128,196));

/*square 8*/
rectfill(buffer, (0*width)+offsetx, (11*height)+offsety, (7*width)+offsetx, (15*height)+offsety, makecol(128,128,196));

/*square 9*/
rectfill(buffer, (15*width)+offsetx, (11*height)+offsety, (19*width)+offsetx, (15*height)+offsety, makecol(128,128,196));

/*square 10*/
rectfill(buffer, (17*width)+offsetx, (7*height)+offsety, (19*width)+offsetx, (11*height)+offsety, makecol(128,128,196));

/*square 11*/
rectfill(buffer, (17*width)+offsetx, (1*height)+offsety, (19*width)+offsetx, (5*height)+offsety, makecol(128,128,196));
/*bounds for game*/
rect(buffer, (0*width)+offsetx, (1*height)+offsety, (19*width)+offsetx, (15*height)+offsety, makecol(128,128,196));

floodfill(buffer,(1*width)+offsetx,(2*height)+offsety,makecol(128,32,128));

/*if the right gates are open the floodfill will cause the game to be over*/
if(makecol(128,32,128)==getpixel(buffer,(9*width)+offsetx, (14*height)+offsety))
{
gas_off=TRUE;
set_trans_blender(0, 0, 0, 196);
clear_to_color(redlight,makecol(255,0,255));
}





/*shows the time left to finish the puzzle*/
 showtimeleftmm=timeleftmm/10;
 if(gas_off==FALSE)
 {
 if (timelefts>9)
    {
    sprintf(msg,"  %d  :  %d  :  %2d  ", timeleftm,timelefts,showtimeleftmm);
    textout(buffer, datafile[22].dat, msg, (SCREEN_W/2)-50, 30, makecol(225,126,64));
    }
    else
    {
    sprintf(msg,"  %d  :  0%d  :  %2d  ", timeleftm,timelefts,showtimeleftmm);
    textout(buffer, datafile[22].dat, msg, (SCREEN_W/2)-50, 30, makecol(225,126,64));
    }
 }
   show_mouse(buffer);   /*shows the mouse after all drawing
                              to the buffer is done*/
    stretch_blit(buffer, screen,
                  0, 0, 640, 480,
                  0, 0, SCREEN_W, SCREEN_H);
   show_mouse(NULL);
   clear(buffer);
/*if time runs out the game is over*/
if(timeleftm<0&&gas_off==FALSE)
{
level=GAMEOVER;
break;
}
       }

mousepressed=2;
}


security()
{
/*shows the main screen for the secuirity monitor*/

int startgame=0;
int menutoshow=0;

for(;;)
{
if((mouse_b&1)&&mousepressed==1)mousepressed=2;
if((mouse_b&1)&&mousepressed==0)mousepressed=1;
if(!(mouse_b&1))mousepressed=0;


/*clears the buffer*/
   clear(buffer);
/*this section is used for player to decide what screen to go to
or to exit*/
sprintf(msg, "SECURITY CAMERAS");

if(mouse_x>SCREEN_W/2-150&&mouse_x<SCREEN_W/2+150&&mouse_y>SCREEN_H/2-50&&mouse_y<SCREEN_H/2+50)
{
textout_centre(buffer, datafile[25].dat, msg, SCREEN_W/2, SCREEN_H/2,makecol(150,225,200));
if(gas_off==FALSE)
{
sprintf(msg, "NOT ACCESSIBLE WHEN GAS ALARM IS ON.");
textout_centre(buffer, datafile[25].dat, msg, SCREEN_W/2, SCREEN_H/2-20,makecol(150,225,200));
}
if(mousepressed==1&&gas_off==TRUE) camerascreen();

}

else
textout_centre(buffer, datafile[25].dat, msg, SCREEN_W/2, SCREEN_H/2,makecol(175,175,175));



sprintf(msg, "VENTILATION SYSTEM");

if(mouse_x>SCREEN_W/2-150&&mouse_x<SCREEN_W/2+150&&mouse_y>SCREEN_H/2+50&&mouse_y<SCREEN_H/2+150)
{
textout_centre(buffer, datafile[25].dat, msg, SCREEN_W/2, (SCREEN_H/2)+100,makecol(150,225,200));

if(mousepressed==1&&gas_off==FALSE) ventscreen();

if(gas_off==TRUE)
{
sprintf(msg, "GAS SYSTEM DEACTIVATED");
textout_centre(buffer, datafile[25].dat, msg, SCREEN_W/2, (SCREEN_H/2)+80,makecol(150,225,200));
}

}
else
{
textout_centre(buffer, datafile[25].dat, msg, SCREEN_W/2, (SCREEN_H/2)+100,makecol(175,175,175));


}
sprintf(msg, "TURN OFF MONITOR");
if(mouse_x>SCREEN_W/2-150&&mouse_x<SCREEN_W/2+150&&mouse_y>SCREEN_H/2+150&&mouse_y<SCREEN_H/2+250)
{
textout_centre(buffer, datafile[25].dat, msg, SCREEN_W/2, (SCREEN_H/2)+200,makecol(150,225,200));
if(mousepressed==1)  break;

}
else
textout_centre(buffer, datafile[25].dat, msg, SCREEN_W/2, (SCREEN_H/2)+200,makecol(175,175,175));

showtimeleftmm=timeleftmm/10;
    if(gas_off==FALSE)
   {
    if (timelefts>9)
    {
    sprintf(msg,"  %d  :  %d  :  %2d  ", timeleftm,timelefts,showtimeleftmm);
    textout(buffer, datafile[22].dat, msg, (SCREEN_W/2)-50, 30, makecol(225,126,64));
    }
    else
    {
    sprintf(msg,"  %d  :  0%d  :  %2d  ", timeleftm,timelefts,showtimeleftmm);
    textout(buffer, datafile[22].dat, msg, (SCREEN_W/2)-50, 30, makecol(225,126,64));
    }
   }

   show_mouse(buffer);   /*shows the mouse after all drawing
                              to the buffer is done*/
    stretch_blit(buffer, screen,
                  0, 0, 640, 480,
                  0, 0, SCREEN_W, SCREEN_H);
   show_mouse(NULL);
/*if time runs out the game is over*/
if(timeleftm<0&&gas_off==FALSE)
{
level=GAMEOVER;
break;
}

}
}
