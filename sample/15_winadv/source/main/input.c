input()
{
/*This section is used to move the player around
either with the mouse or the arrow keys*/
/* uncomment this section if you want to switch screens by
using the numbers!

if (key[KEY_1])showscreen=1;
if (key[KEY_2])showscreen=2;
if (key[KEY_3])showscreen=3;
if (key[KEY_4])showscreen=4;
if (key[KEY_5])showscreen=5;
if (key[KEY_6])
{
showscreen=6;
clearfire();
}
if (key[KEY_7])showscreen=7;
if (key[KEY_8])showscreen=8;
if (key[KEY_9])showscreen=9;
if (key[KEY_0])showscreen=10;
if (key[KEY_1]&&key[KEY_0])showscreen=11;
if (key[KEY_2]&&key[KEY_0])showscreen=12;
if (key[KEY_3]&&key[KEY_0])showscreen=13;
                    */

/*if the mouse is not in the menu selection pressing the mouse
button will cause the player to move towards where the mouse
was when the button was pressed*/
if ( (mousepressed==1)&&(mouse_y<479)   )
{
oldmovetox=movetox=mouse_x-25;
oldmovetoy=movetoy=mouse_y-136;
}
/* These four lines used so I could move the player
with different keys when I was writing the binary files*/
if (!( key[KEY_J]) )moveleft=0;
if (!( key[KEY_L]) )moveright=0;
if (!( key[KEY_I]) )moveup=0;
if (!( key[KEY_K]) )movedown=0;
/*checks where player is and decides to move either left or right and/or
up or down If arrow keys are used the mouse is ignored.
values are 1,2,4 or 8 like binary*/
if ( ((playerx-2)>movetox) || ((key[KEY_LEFT])&&(!(key[KEY_RIGHT]))) )
{
moveleft=1;
}
if ( ((playerx+2)<movetox) || ((key[KEY_RIGHT])&&(!(key[KEY_LEFT]))) )
{
moveright=2;
}
if ( ((playery+2)<movetoy) || ((key[KEY_DOWN])&&(!(key[KEY_UP]))) )
{
movedown=4;
}
if ( ((playery-2)>movetoy) || ((key[KEY_UP])&&(!(key[KEY_DOWN]))) )
{
moveup=8;
}
/*values are added up to form direction player will move in*/
playermove=moveleft+moveright+movedown+moveup;
playermovex=0;playermovey=0;
 switch (playermove%15) {
	    case 0: playermovex=0;playermovey=0; break; /*stops player*/
            case 1: playermovex=-1;break;               /*move left*/
            case 2: playermovex=1; break;               /*move right*/
            case 4: playermovey=1; break;               /*move down*/
            case 8: playermovey=-1;break;               /*move up*/
            default:
            {
            /*if combination than player moves on an angle*/
            if (playermove>8)playermovey=-0.5;
            else playermovey=0.5;
            if (moveright==2) playermovex=0.5;
            else              playermovex=-0.5;
            break;
            }
         }



}