/*this function shows the gas coming out of the grate in the lab room*/
sleep_gas()
{
int i=0;
int c=0;

int gas_x,gas_y;

/*initializes the function*/
if(times_gas_called2==0)
{

clear_to_color(redlight,makecol(76,90,48));
times_gas_called++;
timeleftm=2;
timelefts=0;
}
if(times_gas_called!=times_gas_called2)
{
times_gas_called2=times_gas_called;
set_trans_blender(0, 0, 0, 160+times_gas_called2*0.5);
}
gas_x=rand()%50+590;
gas_y=rand()%15;
/*draws the gas randly moving right to left and from the top of the screen
to the bottom*/
while(c<25&&gas_x>0&&gas_y<480&&i<2000)
{
i++;
gas_x+=1-rand()%7-i/400;
gas_y+=-1+rand()%(3+i/800);
if(gas_y<=0)gas_y=rand()%2;
if(gas_x<=0)gas_x=630+rand()%10;
if(getpixel(redlight,gas_x,gas_y)!=makecol(90,120,60))
{
putpixel(redlight,gas_x,gas_y,makecol(90,120,60));
c++;
}
}
}
