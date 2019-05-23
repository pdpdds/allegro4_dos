snowing(int distance,int wind)
{

/*draws the snow in the outdoor scenes.
(no indoor scenes have snow :>)  )  */
/*distance is 1 it is in the background (snow is smaller slower)*/
/*distance is 2 it is in the foreground (snow is bigger faster)*/
/*wind determines speed in the x direction*/

int snow_count,count,rand_number;
/*SNOW_MAX determines how much snow to draw*/
for(snow_count=SNOW_MAX*(distance*6);snow_count>1;snow_count--)
{

/*gets a rand number*/
rand_number=rand()%2;
rand_number*=32;
rand_number+=128;
/*set up the snows original positions*/
if (snow[distance-1][snow_count].x==0)
snow[distance-1][snow_count].x=rand()%640+1;
if (snow[distance-1][snow_count].y==0)
snow[distance-1][snow_count].y=rand()%460+1;

/*resets value of y once snow is off the screen if going off the side*/
if (snow[distance-1][snow_count].x>640)
{
snow[distance-1][snow_count].y=rand()%460+1;
snow[distance-1][snow_count].x=1;
}

/*resets snow if it goes too far down.
as the snow goes further down it also has more of a chance of disappearing*/
if (snow[distance-1][snow_count].y>460||(rand()%(20000/snow[distance-1][snow_count].y)==25))
{
snow[distance-1][snow_count].y=1;
snow[distance-1][snow_count].x=rand()%640+1;
}
/*increments the position of a snow flake by using
rand and sin to make it look like it wiggles down the screen*/
snow[distance-1][snow_count].x+=wind+(distance*4)*sin(RAD*(rand()%360));
snow[distance-1][snow_count].y+=(distance*2)*sin(RAD*(rand()%180));
for(count=0;count<distance*2;count++)
{
/*draws the snow with white to light blue pixels*/
putpixel(buffer,snow[distance-1][snow_count].x+sin(RAD*(rand()%360))*(1.5*distance)+1,snow[distance-1][snow_count].y+sin(RAD*(rand()%360))*(1.5*distance)-1,makecol(rand_number+12,rand_number+12,rand_number+12));
putpixel(buffer,snow[distance-1][snow_count].x+sin(RAD*(rand()%360))*(1.5*distance)-1,snow[distance-1][snow_count].y+sin(RAD*(rand()%360))*(1.5*distance)+1,makecol(rand_number+24,rand_number+24,rand_number+24));
putpixel(buffer,snow[distance-1][snow_count].x+sin(RAD*(rand()%360))*(1.5*distance)+1,snow[distance-1][snow_count].y+sin(RAD*(rand()%360))*(1.5*distance)-1,makecol(rand_number+36,rand_number+36,rand_number+36));
putpixel(buffer,snow[distance-1][snow_count].x+sin(RAD*(rand()%360))*(1.5*distance)-1,snow[distance-1][snow_count].y+sin(RAD*(rand()%360))*(1.5*distance)+1,makecol(rand_number+24,rand_number+24,rand_number+24));
putpixel(buffer,snow[distance-1][snow_count].x+sin(RAD*(rand()%360))*(1.5*distance)+1,snow[distance-1][snow_count].y+sin(RAD*(rand()%360))*(1.5*distance)-1,makecol(rand_number-24,rand_number+24,rand_number+24));

putpixel(buffer,snow[distance-1][snow_count].x+sin(RAD*(rand()%360))*(1.5*distance),snow[distance-1][snow_count].y+sin(RAD*(rand()%360))*(1.5*distance),makecol(rand_number-36,rand_number+36,rand_number+36));
putpixel(buffer,snow[distance-1][snow_count].x+sin(RAD*(rand()%360))*(1.5*distance),snow[distance-1][snow_count].y+sin(RAD*(rand()%360))*(1.5*distance),makecol(rand_number+24,rand_number+24,rand_number+24));
putpixel(buffer,snow[distance-1][snow_count].x+sin(RAD*(rand()%360))*(1.5*distance),snow[distance-1][snow_count].y+sin(RAD*(rand()%360))*(1.5*distance),makecol(rand_number,rand_number,rand_number));
putpixel(buffer,snow[distance-1][snow_count].x+sin(RAD*(rand()%360))*(1.5*distance),snow[distance-1][snow_count].y+sin(RAD*(rand()%360))*(1.5*distance),makecol(rand_number,rand_number,rand_number));

}

}

}
