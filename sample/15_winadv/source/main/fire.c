fires(int xpos,int ypos,int xsize,int ysize, int xspeed, int yspeed,int xlength)
{
/*This function is used to draw fires*/
#define COUNT 6400

int count;

if (timesfire<10000)timesfire++;
for(count=0;count<COUNT;count++)
{

/*if the value of the fire is off the screen
or should disappear the pixel is given a new starting point*/
if(fire[count].y<=0||fire[count].x>640||rand()%(xsize/fire[count].x+fire[count].y/ysize)==1||(timesfire<10000&&rand()%timesfire/3==1))
{
fire[count].y=ypos+rand()%6;
fire[count].x=rand()%xlength+xpos;
}
/*determines position of pixels for fire*/
fire[count].y-=yspeed/fire[count].y+rand()%(1+2250/fire[count].x);
fire[count].x+=xspeed/fire[count].x+(4-rand()%4);

/*determines the color*/
fire[count].r=230-fire[count].x/15+fire[count].y/15;
fire[count].g=fire[count].x/3-fire[count].y*0.25;
fire[count].b=fire[count].x/5-fire[count].y*0.1;
/*draws the fire to the transcluency buffer to make it look transparent*/
putpixel(redlight,fire[count].x+rand()%3+1,fire[count].y+rand()%3-1,makecol(fire[count].r+rand()%-5,fire[count].g+rand()%35,fire[count].b+rand()%55));
putpixel(redlight,fire[count].x+rand()%3,fire[count].y+rand()%3,makecol(fire[count].r+rand()%15,fire[count].g+rand()%85,fire[count].b+rand()%25));
putpixel(redlight,fire[count].x+rand()%3-1,fire[count].y+rand()%3+1,makecol(fire[count].r+rand()%20,fire[count].g+rand()%55,fire[count].b+rand()%35));

}
}
clearfire()
{
/*used to reset the fire*/
#define COUNT 6400

int count;
for(count=0;count<COUNT;count++)
{
fire[count].y=0;
}
timesfire=0;
}
