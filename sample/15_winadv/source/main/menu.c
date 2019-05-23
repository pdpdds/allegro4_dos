menu()
{
int cx,cy;
/*CHOICES rectangle*/
rectfill(buffer, 0, 500, 525, 600, makecol(5,15,76));
/*TEXT OUTPUT rectangle*/
rectfill(buffer, 0, 480, 515, 500, makecol(35,5,30));
/*ITEMS rectangle*/
rectfill(buffer, 515, 480, 640, 600, makecol(105,15,15));
sprintf(msg, "ITEMS" );
textout(buffer, datafile[25].dat/*font*/, msg, 545, 480, makecol(100,80,50));

/*This loop draws the options the player can choose by looping
through an array of strings.
It also checks the position of the mouse to find what
the player is selecting*/
for(cx=0;cx<5;cx++)
for(cy=1;cy<4;cy++)
{
sprintf(msg, "%s",choice_desc[0][cx*3+cy] );
textout(buffer, datafile[25].dat, msg, (cx*100)+5, (cy*25)+500, makecol(70+cx*40,225-cx*30,60+cy*60));

if( (mouse_x>(cx*100)-25) && (mouse_x < ( ((cx+1)*100)+25 )) && (mouse_y > ( (cy*25)+500) ) && (mouse_y < ( ( (cy+1)*25 )+500) ) && (mouse_b & 1) )
pick_choice=cx*3+cy;   /*sets the choice the player has selected
                       such as walk, push, open etc.*/
}
/*This loop draws the items the player can choose by looping
through an array of strings.
It also checks the position of the mouse to find what
item the player is selecting (the player can only select an object
once a command for the character has been selected*/
for(cy=1;cy<=add_item;cy++)
{
sprintf(msg, "%s",item[1][1][cy].text );
textout(buffer, datafile[25].dat, msg, 515, (cy*25)+475, makecol(200,200,200));

if( (mouse_x>499) && (mouse_x < 600) && (mouse_y > ((cy*25)+475) )&& (mouse_y < ( ( (cy+1)*25 )+475))  && (mouse_b & 1) )
item_choice=cy;
if(!(pick_choice==USE||pick_choice==HIT||pick_choice==GIVE))item_choice=0;
}
/*if an object has been selected
the output will different than if an object is not selected.
example
if a item has been selected. use carkey with trunk
if a item is not selected. use trunk with*/

if(screen_objects==0)sprintf(msg, "%s %s %s",choice_desc[0][pick_choice],choice_desc[1][pick_choice],item[1][1][item_choice].text);
else sprintf(msg, "%s %s %s %s %s",choice_desc[0][pick_choice],choice_desc[1][pick_choice],onscreen_desc[showscreen][screen_objects],choice_desc[2][pick_choice],item[1][1][item_choice].text );
textout(buffer, datafile[25].dat, msg, 10, 477, makecol(200,200,200));
}
