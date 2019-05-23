void cdisplay()
{
/*This function changes the collision (object bitmaps)
as objects are taken, or doors are opened by checking
what has happened and what screen the player is on*/
if(showscreen<8)blit(level1cdat[showscreen].dat,bufferc,0,0,0,0,160,120);
else
blit(level2cdat[showscreen-7].dat,bufferc,0,0,0,0,160,120);

if (object[1][TRUNK][OPEN].state==1&&object[1][CROWBAR][GET].state<2&&showscreen==1)
   {
rectfill(bufferc,300/4,325/4,350/4,330/4,makecol(150,0,0));
rectfill(bufferc,340/4,325/4,350/4,320/4,makecol(150,0,0));
   }
if(showscreen==5)
{
/*stop drawing rag and matches once they are taken*/
if(object[5][RAG][GET].state<2)
draw_sprite(bufferc, datafile[27].dat, 270/4, 155/4);
if(object[5][MATCHES][GET].state<2)
draw_sprite(bufferc, datafile[29].dat, 230/4, 155/4);
}
if(showscreen==6)
{
if (object[6][JAIL_COMP][USE].state==1)
draw_sprite(bufferc, datafile[27].dat, 130/4, 90/4);
/*I decided to remove this gate*/
poly[0]=23/4;
poly[1]=257/4;
poly[2]=23/4;
poly[3]=159/4;
poly[4]=130/4;
poly[5]=158/4;
poly[6]=130/4;
poly[7]=261/4;
polygon(bufferc, 4, poly, makecol(0,0,0));
if(object[6][RAG][USE].state==1)
{
/*removes the collision for jail cell gates*/

poly[0]=465/4;
poly[1]=192/4;
poly[2]=465/4;
poly[3]=56/4;
poly[4]=485/4;
poly[5]=56/4;
poly[6]=489/4;
poly[7]=192/4;
polygon(bufferc, 4, poly, makecol(0,0,0));
poly[0]=475/4;
poly[1]=452/4;
poly[2]=467/4;
poly[3]=323/4;
poly[4]=500/4;
poly[5]=323/4;
poly[6]=500/4;
poly[7]=452/4;
polygon(bufferc, 4, poly, makecol(0,0,0));
}

}
if(showscreen==10)
{
if(object[10][DOOR1][OPEN].state==1)
{
poly[0]=185/4;
poly[1]=423/4;
poly[2]=185/4;
poly[3]=295/4;
poly[4]=270/4;
poly[5]=295/4;
poly[6]=270/4;
poly[7]=423/4;
polygon(bufferc, 4, poly, makecol(0,0,0));
}
}

}
display()
{
/*draws the screen. Seperate calls are made for the menu box
the background, foreground and effects like snow and fire*/


menu(); /*  shows the menu at the bottom of the screen*/

background();   /*calls the background image to show*/

/*depending on what screen the player is on fire or snow may be shown*/
if(showscreen<5||showscreen==9)snowing(1,6);

if(showscreen==1)fires(290,230,700,40,3000,425,100);

/*shows transluency effects */
if(showscreen!=6&&showscreen!=7&&showscreen!=8&&showscreen!=10)draw_trans_sprite(buffer, redlight, 0, 0);


back_guy();     /*if another character is behind the player
                this function will show the character*/

/*if statement decides what clothes the player is wearing*/
if(uniform==0)draw_sprite(buffer, datafile[showplayer].dat, playerx, playery);
else
draw_sprite(buffer, character[showplayer+9], playerx, playery);

fore_guy();     /*if another character is in front of the player
                this function will show the character*/
                
/*if the screen is 6 or 7 the whole screen will change colors*/
if(showscreen==6)
{
if(object[6][RAG][USE].state==1)draw_trans_sprite(buffer, redlight, 0, 0);
}

if(showscreen==7||showscreen==8||showscreen==10) draw_trans_sprite(buffer, redlight, 0, 0);

/*shows larger snow in front of player*/
if(showscreen<5||showscreen==9)snowing(2,6);



fore_ground();   /*shows the dialog*/
}

background()
{
/*This function is called by the display() to show the
background and whatever items may be in the background or
if doors open a black hole will be shown to say it has opened*/
if(showscreen<8)
blit(level1[showscreen], buffer, 0,0,0, 0, 640, 480);
else
blit(level2[showscreen-7], buffer, 0,0,0, 0, 640, 480);
if (showscreen==1)
{
draw_sprite(buffer, datafile[23].dat, 260, 190);
if (object[1][TRUNK][OPEN].state==1)  draw_sprite(buffer,datafile[24].dat,260,290);
if (object[1][TRUNK][OPEN].state==1&&object[1][CROWBAR][GET].state<2)
   {
rectfill(buffer,300,325,350,330,makecol(128,128,128));
rectfill(buffer,340,325,350,320,makecol(128,128,128));
   }
}
if (showscreen==2)
{
draw_sprite(buffer, datafile[21].dat, 90,244);
if(object[2][BACKDOOR][OPEN].state==1)
{
/*shows black hole when back door of the truck is opened*/
poly[0]=409;
poly[1]=399;
poly[2]=409;
poly[3]=263;
poly[4]=456;
poly[5]=242;
poly[6]=456;
poly[7]=375;
polygon(buffer, 4, poly, makecol(10,10,10));
}
}
if(showscreen==5)
{
/*shows rags and matches if they have not been taken*/
if(object[5][RAG][GET].state<2)draw_sprite(buffer, datafile[26].dat, 270, 155);
if(object[5][MATCHES][GET].state<2)draw_sprite(buffer, datafile[28].dat, 230, 155);
if(object[5][DOOR1][OPEN].state==1)
{
/*shows hole when door1 is opened*/
poly[0]=356;
poly[1]=199;
poly[2]=357;
poly[3]=87;
poly[4]=440;
poly[5]=86;
poly[6]=429;
poly[7]=204;
polygon(buffer, 4, poly, makecol(10,10,10));
}
if(object[5][DOOR2][OPEN].state==1)
{
/*shows hole when door2 is opened*/
poly[0]=68;
poly[1]=321;
poly[2]=30;
poly[3]=181;
poly[4]=120;
poly[5]=136;
poly[6]=134;
poly[7]=249;
polygon(buffer, 4, poly, makecol(10,10,10));
}

}
if(showscreen==6)
{
/*shows rag when it has been used on the computer*/
if (object[6][JAIL_COMP][USE].state==1)
draw_sprite(buffer, datafile[26].dat, 130, 90);
/*I decided to remove this gate*/
poly[0]=23;
poly[1]=257;
poly[2]=23;
poly[3]=159;
poly[4]=130;
poly[5]=158;
poly[6]=130;
poly[7]=261;
polygon(buffer, 4, poly, makecol(250,156,147));
if(object[6][RAG][USE].state==1)
{
/*once the rag is put on the computer and the
rag is lighted a fire will start the gates will open
and background will change from grey to red*/
fires(80,125,28000,120,5,0,175);

poly[0]=465;
poly[1]=192;
poly[2]=465;
poly[3]=56;
poly[4]=485;
poly[5]=56;
poly[6]=489;
poly[7]=192;
polygon(buffer, 4, poly, makecol(250,156,147));
poly[0]=475;
poly[1]=452;
poly[2]=467;
poly[3]=323;
poly[4]=500;
poly[5]=323;
poly[6]=500;
poly[7]=452;
polygon(buffer, 4, poly, makecol(250,156,147));
}
/*shows the scientist pacing*/
fseek(f3,sizeof(struct mover)*(pace),SEEK_SET);
                fread(&guy1,sizeof(struct mover),1,f3);

if(object[6][SEWER][PUSH].state==1)circlefill(buffer, 440, 404, 30, makecol(10,50,10));
}
if(showscreen==7)
{
/*shows scientist with you in the sewer*/
if(follow==0)follow=1;

}
if(showscreen==8)
{
if(object[8][DOOR1][OPEN].state==1)
{
poly[0]=505;
poly[1]=133;
poly[2]=505;
poly[3]=17;
poly[4]=575;
poly[5]=17;
poly[6]=575;
poly[7]=133;
polygon(buffer, 4, poly, makecol(0,0,0));
poly[0]=612;
poly[1]=295;
poly[2]=612;
poly[3]=170;
poly[4]=639;
poly[5]=170;
poly[6]=639;
poly[7]=295;
polygon(buffer, 4, poly, makecol(0,0,0));
}

/*shows grate in top right corner of screen*/
draw_sprite(buffer, datafile[30].dat, 575, 5);

/*changes the background to simulate lights*/
if(lablights==1)clear_to_color(redlight,makecol(32,32,48));
else if(gas_off==TRUE)clear_to_color(redlight,makecol(255,0,255));
/*shows the beam panel broken open*/
if(object[8][PANEL][USE].state==1)
{
draw_sprite(buffer, datafile[35].dat, 367, 98);
draw_sprite(buffer, datafile[36].dat, 500, 150);
}
/*shows the compartment holding the disc open*/
if(object[8][COMPARTMENT][OPEN].state==1)
{
poly[0]=177;
poly[1]=304;
poly[2]=177;
poly[3]=250;
poly[4]=318;
poly[5]=250;
poly[6]=318;
poly[7]=304;
polygon(buffer, 4, poly, makecol(0,0,0));
fires(385,165,700,40,2000,225,30);
}

}
if(showscreen==10)
{
/*simulates lights*/
if(stocklights==1)clear_to_color(redlight,makecol(32,32,48));
else clear_to_color(redlight,makecol(255,0,255));
if(object[10][DOOR1][OPEN].state==1)
{
poly[0]=475;
poly[1]=145;
poly[2]=475;
poly[3]=42;
poly[4]=540;
poly[5]=42;
poly[6]=540;
poly[7]=145;
polygon(buffer, 4, poly, makecol(0,0,0));
poly[0]=185;
poly[1]=423;
poly[2]=185;
poly[3]=295;
poly[4]=270;
poly[5]=295;
poly[6]=270;
poly[7]=423;
polygon(buffer, 4, poly, makecol(178,86,68));
}

}
}

back_guy()
{
/*show the other character on the screen behind the main character*/
if(showscreen==5)
{
if(follow>=150&&follow<315&&guy1.y<=playery)draw_sprite(buffer, character[guy1.show+19], guy1.x, guy1.y);
}

if(showscreen==6)
{
if(guy1.y<=playery)draw_sprite(buffer, character[guy1.show+19], guy1.x, guy1.y);
}

if(showscreen==7)
{
if(follow<=136&&guy1.y<=playery)draw_sprite(buffer, character[guy1.show+19], guy1.x, guy1.y);
}

if(showscreen==8)
{
if(guy1.y<=playery&&follow<610)draw_sprite(buffer, character[guy1.show+19], guy1.x, guy1.y);
}
if(showscreen==9)
{
if(guy1.y<=playery&&follow>609)draw_sprite(buffer, character[guy1.show+19], guy1.x, guy1.y);
}
if(showscreen==10)
{
if(120<=playery)draw_sprite(buffer, character[12], 200, 120);
}

}


fore_guy()
{
/*shows other characters on the screen in front of the player*/

/*shows bad guys standing by the truck*/
if(showscreen==2)
{
draw_sprite(buffer, character[4], 20, 300);
draw_sprite(buffer, character[17], 60, 300);
}

if(showscreen==5)
{
if(follow>=150&&follow<315&&guy1.y>playery)draw_sprite(buffer, character[guy1.show+19], guy1.x, guy1.y);
}

if(showscreen==6)
{
if(guy1.y>playery)draw_sprite(buffer, character[guy1.show+19], guy1.x, guy1.y);
}

if(showscreen==7)
{
if(follow<=136&&guy1.y>playery)draw_sprite(buffer, character[guy1.show+19], guy1.x, guy1.y);
}

if(showscreen==8)
{
if(guy1.y>playery&&follow<610)draw_sprite(buffer, character[guy1.show+19], guy1.x, guy1.y);
}
if(showscreen==9)
{
if(guy1.y>playery&&follow>609)draw_sprite(buffer, character[guy1.show+19], guy1.x, guy1.y);
}
if(showscreen==10)
{
if(120>playery)draw_sprite(buffer, character[12], 200, 120);
}

}
show_player()
{
/*This function is used to animate the main character
depending on direction the character is moving in*/
switch (playermove%15) {
	    case 0: showplayer=3;break;
            case 1:
                 {
            if (showplayer==8||showplayer==9)showplayer++;
            else showplayer=8;
            break;
            }
            case 2:
                 {
            if (showplayer==5||showplayer==6)showplayer++;
            else showplayer=5;
            break;
            }
            case 4:
                 {
            if (showplayer==3)showplayer=4;
            else showplayer=3;
            break;
            }
            case 8:
                 {
            if (showplayer==1)showplayer=2;
            else showplayer=1;
            break;
            }
            default:
                    {
            if (moveright==2)
               {
            if (showplayer==5||showplayer==6)showplayer++;
            else showplayer=5;
            }
            else
                {
            if (showplayer==8||showplayer==9)showplayer++;
            else showplayer=8;
            }
            break;
                  }
         }



}
fore_ground()
{
/*shows dialog to the screen and other text for development.
this function is also used to form the text into a paragraph*/
char talktemp[80];
char talktemp1[80];
char talktemp2[80];
char talktemp3[80];
int count,count1,count2;
int the_text_height,the_text_length;

memset(talktemp,0,sizeof(talktemp));
memset(talktemp1,0,sizeof(talktemp1));
memset(talktemp2,0,sizeof(talktemp2));
memset(talktemp3,0,sizeof(talktemp3));
     /*
sprintf(msg, "f_Playerx: %.2f ",f_playerx );
textout(buffer, font, msg, 0, 20, 0);
sprintf(msg, "f_Playerfy: %.2f ",f_playery );
textout(buffer, font, msg, 0, 40, 0);
sprintf(msg, "Playerx: %d ",playerx );
textout(buffer, font, msg, 0, 60, 0);
sprintf(msg, "Playery: %d ",playery );
textout(buffer, font, msg, 0, 80, 0);
sprintf(msg, "moveleft: %d ",moveleft );
textout(buffer, font, msg, 0, 100, 0);
sprintf(msg, "moveright: %d ",moveright );
textout(buffer, font, msg, 0, 120, 0);

sprintf(msg, "level: %d ",level );
textout(buffer, datafile[22].dat, msg, 0, 140, 0);

sprintf(msg, "mouse_x: %d ",mouse_x );
textout(buffer, datafile[22].dat, msg, 0, 170, makecol(200,80,50));
sprintf(msg, "mouse_y: %d ",mouse_y );
textout(buffer, datafile[22].dat, msg, 0, 200, 0);
sprintf(msg, "playermovex: %.2f ",playermovex );
textout(buffer, font, msg, 0, 240, 0);
sprintf(msg, "playermovey: %.2f ",playermovey );
textout(buffer, font, msg, 0, 260, 0);
sprintf(msg, "follow: %d ",follow );
textout(buffer, font, msg, 0, 280, 0);
                             */
if(showscreen==8&&gas_off==FALSE)
{
showtimeleftmm=timeleftmm/10;

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





/*if the player is talking to the screen this section is used*/
if(text_length(datafile[22].dat,talkback_desc[talkback].text)>0&&talkback!=0)
{
if(text_length(datafile[22].dat,talkback_desc[talkback].text)>50)
{


strcpy(talktemp1,talkback_desc[talkback].text);    /*copy the string to temp1*/
count1=79;                                    /*set count1 to highest number*/
for(count=1;count<79;count++)
{
strncpy(talktemp, talktemp1, count);         /*add one letter at a time to
                                             find best spot to break line*/

if (talktemp[count-1]==' '&&text_length(datafile[22].dat,talktemp)<=300
&&text_length(datafile[22].dat,talktemp)>=150)
        {
count1=count;                               /*if line is not too long and not
                                            to short and is at a space
                                            change count1 to spot
                                            looping is at*/
        }

}
for(count=0;count<79;count++)
{
talktemp1[count]=0;                        /*clear the string after doing
                                           looping*/
}

strncpy(talktemp1,talktemp,count1);       /*copy the words in talktemp
                                          to talktemp1 with max being of where
                                          best spot for cutting up string was
                                          */
for(count=0;count<79;count++)
{
talktemp[count]=0;                        /*clear the string after doing
                                           copying*/
}


for(count=0;count<79;count++)
{                                        /*copy words that are left to
                                         in string to talktemp2*/
if(count1+count<79)talktemp2[count]=talkback_desc[talkback].text[count+count1];
}

count2=79;                               /*set count 2 to highest number*/
for(count=1;count<79;count++)
{
strncpy(talktemp, talktemp2, count);    /*add a letter at a time to
                                             find best spot to break line*/

if (talktemp[count-1]==' '&&text_length(datafile[22].dat,talktemp)<=300
&&text_length(datafile[22].dat,talktemp)>=150)
        {
count2=count;                            /*if line is not too long and not
                                            to short and is at a space
                                            change count1 to spot
                                            looping is at*/
        }

}
for(count=0;count<79;count++)
{
talktemp2[count]=0;                     /*clear the string after doing
                                           looping*/
}
strncpy(talktemp2,talktemp,count2);       /*copy the words in talktemp
                                          to talktemp2 with max being of where
                                          best spot for cutting up string was
                                          */
                                          
for(count=0;count<79;count++)
{                                        /*copy words that are left to
                                         in string to talktemp3*/
if(count+count1+count2<79)talktemp3[count]=talkback_desc[talkback].text[count+count1+count2];
}

}

/*this section finds what the longest string was and how
many lines the paragragh will be to form the white box around the text*/
the_text_height=text_height(datafile[22].dat);
if (talktemp3[0]!=0)the_text_height=the_text_height*3;
else if (talktemp2[0]!=0)the_text_height=the_text_height*2;
if (text_length(datafile[22].dat,talktemp1)>
text_length(datafile[22].dat,talktemp2))
{
if (text_length(datafile[22].dat,talktemp1)>
text_length(datafile[22].dat,talktemp3))
the_text_length=text_length(datafile[22].dat,talktemp1);
else
the_text_length=text_length(datafile[22].dat,talktemp3);
}
else
{
if (text_length(datafile[22].dat,talktemp2)>
text_length(datafile[22].dat,talktemp3))
the_text_length=text_length(datafile[22].dat,talktemp2);
else
the_text_length=text_length(datafile[22].dat,talktemp3);
}
/*draws the text box*/
masked_blit(dialog, buffer, 0, 0,
                  195, 195, the_text_length+10,
                  the_text_height+10);

/*draws the words*/
sprintf(msg, "%s",talktemp1);
textout(buffer, datafile[22].dat, msg, 200, 200, makecol(32,32,32*talkcolor));
sprintf(msg, "%s",talktemp2);
textout(buffer, datafile[22].dat, msg, 200, 200+5+text_height(datafile[22].dat), makecol(32,32,32*talkcolor));
sprintf(msg, "%s",talktemp3);
textout(buffer, datafile[22].dat, msg, 200, 200+5+text_height(datafile[22].dat)*2, makecol(32,32,32*talkcolor));
}
}
