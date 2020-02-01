#include <vector>
#include <iostream>
#include <vector>
#include <cstdio>
#include "globals.h"
#include "nincompoop.h"
#include "graphics.h"
#include "enemy.h"
#include "items.h"

int main() {
	init_allegro();
	
	textout_centre_ex(screen, font, "LOADING",
                        SCREEN_W / 2, SCREEN_H / 2,
                        makecol(255, 0, 0), makecol(0, 0, 0));
	
	nincompoops nin;
	//*********************************************************************

	nin.loadSprites();
	int i=0;

	work = create_bitmap(screen->w,screen->h);

	level = load_bitmap("assets/levels/level1/overlay.bmp",workpal);
	colmap = load_bitmap("assets/levels/level1/cmap.bmp",workpal);
	bg = load_bitmap ("assets/levels/level1/bg.bmp",workpal);
	
	BITMAP *lose = load_bitmap("assets/levels/level1/lost.bmp",workpal);
	BITMAP *win = load_bitmap("assets/levels/level1/end.bmp",workpal);
		BITMAP *start = load_bitmap("assets/levels/level1/start.bmp",workpal);
	load_enemies("assets/levels/level1/enemy.txt");
	load_items("assets/levels/level1/items.txt");
	SAMPLE *hit,*bat,*pts,*fall;
	
	hit = load_sample ("assets/sounds/hit.wav");
	bat = load_sample ("assets/sounds/bat.wav");
	pts = load_sample ("assets/sounds/points.wav");
	fall = load_sample ("assets/sounds/fall.wav");
	
	blit (start,screen,0,0,0,0,400,300);
	readkey();
	
	while (!key[KEY_ESC]){
		//poll_keyboard();			
		nin.decideKeys();
		nin.levelCollision(colmap);
				
		if (nin.b_x>bg->w-screen->w )
			nin.b_x=bg->w-screen->w;
		if (nin.b_y>bg->h-screen->h)
			nin.b_y = bg->h-screen->h;
			
		blit(bg,work,
			(nin.b_x < 0 ? 0: nin.b_x),
			(nin.b_y < 0 ? 0: nin.b_y),
			0,0,screen->w,screen->h);
		
		masked_blit (level,work,
		(nin.s_x < 0 ? 0:nin.s_x),
		(nin.s_y < 0 ? 0:nin.s_y),
		0,0,screen->w,screen->h);
		
		
		for (ii=itemset.begin();ii!=itemset.end();ii++){
			if (ii->alive){
			ii->draw(work,nin.s_x,nin.s_y);
			
			/************************** ITEM COllision ************************/
			if ( sprite_collide(nin.x,	nin.y,		
										nin.x+nin._w,		nin.y+nin._h,
										(*ii).x - nin.s_x,	(*ii).y-nin.s_y,		
										(*ii).x+(*ii)._w - nin.s_x,	(*ii).y+(*ii)._h-nin.s_y
										) ) {
											play_sample(pts,255,0,700,0);
										for (int crap=0;crap<8;crap++){
											
											nin.decideKeys();
											nin.levelCollision(colmap);
								
							
											blit(bg,work,
											(nin.b_x < 0 ? 0: nin.b_x),
											(nin.b_y < 0 ? 0: nin.b_y),
											0,0,screen->w,screen->h);
			
											masked_blit (level,work,
											(nin.s_x < 0 ? 0:nin.s_x),
											(nin.s_y < 0 ? 0:nin.s_y),
											0,0,screen->w,screen->h);
								
											masked_blit (dis[crap/2],work,0,0,(*ii).x - nin.s_x,	(*ii).y-nin.s_y,		
															dis[0]->w,dis[0]->h	);
											nin.draw(work);
											
												// draw HP stat on work *****************************************88
		// HUD
		if (nin.HP>5){
			nin.HP=5;
		}
		for (int i=0;i<(int)nin.HP;i++){
			masked_blit (nin.hp,work,0,0,i*32+2,2,31,31);
		}
		
		textprintf_right_ex(work, font, screen->w-3, 5, makecol(0,0,0), -1,"score %d",nin.score);
		textprintf_right_ex(work, font, screen->w-4, 4, makecol(255,255,255), -1,"score %d",nin.score);
		//****************************************************************
											
											
											vsync();
											blit (work,screen,0,0,0,0,screen->w,screen->h);									
										}
										if (ii->typ==3){
											nin.HP+=1;
											nin.score+=10;
										} else {
											if (ii->typ=4) nin.score+=50;
											else nin.score+=5;
										}
										if (ii->typ==2){
											nin.HP+=.2;
										}
									ii->alive = false;
						}
					}
		}
		
/*************************************************************************************
*************************************************************************************
*************************************************************************************
*************************************************************************************/
		//doCollisionTests();
			for (ei=enemyset.begin();ei!=enemyset.end();ei++){
				if (ei->alive == true){
			if ((*ei).x>nin.s_x && (*ei).y>nin.s_y
				&& (*ei).x+(*ei)._w<nin.s_x+screen->w
				&& (*ei).y+(*ei)._h<nin.s_y+screen->h){
					
					(*ei).decideKeys();
					(*ei).levelCollision(colmap);
					(*ei).draw(work,nin.s_x,nin.s_y);
					
					// check if nincompoop collides with any
					//nin.collide(*ei);
					nin.hitEnemy=false;
					if (nin.attacking){
						if ( sprite_collide(nin.x,	nin.y,		
										nin.x+nin._w,		nin.y+nin._h,
										(*ei).x - nin.s_x,	(*ei).y-nin.s_y,		
										(*ei).x+(*ei)._w - nin.s_x,	(*ei).y+(*ei)._h-nin.s_y
										) ) {
											play_sample(fall,255,0,800,0);
										for (int crap=0;crap<8;crap++){
											
											nin.decideKeys();
											nin.levelCollision(colmap);
								
							
											blit(bg,work,
											(nin.b_x < 0 ? 0: nin.b_x),
											(nin.b_y < 0 ? 0: nin.b_y),
											0,0,screen->w,screen->h);
			
											masked_blit (level,work,
											(nin.s_x < 0 ? 0:nin.s_x),
											(nin.s_y < 0 ? 0:nin.s_y),
											0,0,screen->w,screen->h);
								
											masked_blit (dis[crap/2],work,0,0,(*ei).x - nin.s_x,	(*ei).y-nin.s_y,		
															dis[0]->w,dis[0]->h	);
											nin.draw(work);
											
												// draw HP stat on work *****************************************88
		// HUD
		if (nin.HP>5){
			nin.HP=5;
		}
		for (int i=0;i<(int)nin.HP;i++){
			masked_blit (nin.hp,work,0,0,i*32+2,2,31,31);
		}
		
		textprintf_right_ex(work, font, screen->w-3, 5, makecol(0,0,0), -1,"score %d",nin.score);
		textprintf_right_ex(work, font, screen->w-4, 4, makecol(255,255,255), -1,"score %d",nin.score);
		//****************************************************************
											
											
											vsync();
											blit (work,screen,0,0,0,0,screen->w,screen->h);									
										}
										//enemyset.erase(ei);
										//if (enemyset.empty()){
										//	break;
										//} 
										ei->alive = false;
						}		
					} else {
						if ( sprite_collide(nin.x+nin.ex1,	nin.y+nin.ey1,		
										nin.x+nin.ex2,		nin.y+nin.ey2,
										(*ei).x - nin.s_x,	(*ei).y-nin.s_y,		
										(*ei).x+(*ei)._w - nin.s_x,	(*ei).y+(*ei)._h-nin.s_y
										) ) 
						{
							if (!nin.attacking){
								nin.hitEnemy=true;
								/* ****************************************************8
											NIN GOT HIT
								**************************************************/
								play_sample(hit, 255, 0, 2000, 0);
								nin.HP-=.3;
								for (int crap=0;crap<3;crap++){
								if (nin.direction == 1){
									// right
									nin.s_x-=3;
									(*ei).direction = 1;
								} else {
									nin.s_x+=3;
									(*ei).direction = 0;
								}
								nin.decideKeys();
								nin.levelCollision(colmap);
								
							
								blit(bg,work,
								(nin.b_x < 0 ? 0: nin.b_x),
								(nin.b_y < 0 ? 0: nin.b_y),
								0,0,screen->w,screen->h);
		
								masked_blit (level,work,
								(nin.s_x < 0 ? 0:nin.s_x),
								(nin.s_y < 0 ? 0:nin.s_y),
								0,0,screen->w,screen->h);
							
								(*ei).decideKeys();
								(*ei).levelCollision(colmap);
								(*ei).draw(work,nin.s_x,nin.s_y);
								
								nin.draw(work);
								vsync();
								blit (work,screen,0,0,0,0,screen->w,screen->h);	
								}
							} else {
					
								if (enemyset.empty())
									break;
							}
						}
					}
			}
		}
		}
/*************************************************************************************
*************************************************************************************
*************************************************************************************
*************************************************************************************/

		
		nin.draw(work);
		
		// draw HP stat on work *****************************************88
		// HUD
		if (nin.HP>5){
			nin.HP=5;
		}
		for (int i=0;i<(int)nin.HP;i++){
			masked_blit (nin.hp,work,0,0,i*32+2,2,31,31);
		}
		
		textprintf_right_ex(work, font, screen->w-3, 5, makecol(0,0,0), -1,"score %d",nin.score);
		textprintf_right_ex(work, font, screen->w-4, 4, makecol(255,255,255), -1,"score %d",nin.score);
		//****************************************************************
		if (getpixel(colmap,nin.x + nin.s_x,nin.y+nin.s_y) == makecol(0,255,0)){
			goto dieNIN;
		}
		
		if (getpixel(colmap,nin.x + nin.s_x,nin.y+nin.s_y) == makecol(255,0,0)){
			goto winNIN;
		}
		
		if (nin.HP<1){
			goto dieNIN;
		}
		if (nin.attacking){
				play_sample(bat, 255, 0, 700, 0);
		}
		
		vsync();
		blit (work,screen,0,0,0,0,screen->w,screen->h);	
	}
	
	goto winNIN;
	
	dieNIN:
		// ************************ NIN DIES
	rest (300);

	for (int i=screen->w-1;i>=0;i-=2){
		vsync();
			blit (lose,screen,0,0,i,0,screen->w-i,screen->h);
	}
	rest(1400);
	deinit_allegro();
	return 0;
	winNIN:
	//***************************** winning seq
		
	//*********************************************************************
	rest (500);

	for (int i=screen->w-1;i>=0;i-=2){
		vsync();
			blit (win,screen,0,0,i,0,screen->w-i,screen->h);
	}
	rest(2000);
	destroy_sample(hit);
	deinit_allegro();
	return 0;

}
END_OF_MAIN()


