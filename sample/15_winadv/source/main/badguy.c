 badguy()
{
/*This function was used to make my binary files
first you move the character where you want with the
mouse or arrow keys. Once the character is in position
you use a different set of keys to make the character walk
and to record the value of the position of the player
and the animation frame*/
if ( key[KEY_J] )
{
moveleft=1;
}
if ( key[KEY_L] )
{
moveright=2;
}
if ( key[KEY_K] )
{
movedown=4;
}
if ( key[KEY_I] )
{
moveup=8;
}

guy1.x=playerx;
guy1.y=playery;
guy1.show=showplayer;

fseek(f4,sizeof(struct mover)*(follow),SEEK_SET);
fwrite(&guy1,sizeof(struct mover),1,f4);
follow++;     /*as the function is called this integer moves
            up so the data is written not on top of other data*/
}