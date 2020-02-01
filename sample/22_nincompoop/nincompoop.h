#define maxFrames 12

class nincompoops {
	public:
		// *** Bitmap & allegro assets for hero
		BITMAP *sprite_walk[4][12];				// 12 frame animation for walk
		BITMAP *hp;
		// *** Motion variables
		bool walking, onGround,jumping,attacking;
		int heroFrame, direction,oldDirection;
		int jumpVel;
		int xvel, yvel;							// xc,yc of hello.c
		double jumpTime, dropping, dropTime;
		double velocity, accel, velMax;
		bool hitEnemy;
		int g;
		int s_x,s_y;							// screen fix;
		int x,y, _w, _h;						// x,y, height, width
		int cx1,cx2,cy1,cy2;					// collision params
		int ex1,ex2,ey1,ey2;
		int b_x,b_y;
		int step_tol;							// lifting tolerance
		int recoil;
		double HP;
		int score;
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////		
		nincompoops(){
			// *** initial motion settings
			score=0;
			jumping=onGround = walking = dropping = false;
			heroFrame = 0;
			direction=1;
			oldDirection = 0;
			xvel=0;	yvel=0;
			jumpTime = 0;	dropping = 0; dropTime = 0;
			jumpVel = 15;
			g = 16;
			velocity = 1;
			
			velMax = 3;
			accel = 0.3;
			x = screen->w/2 -20;
			y = screen->h/2  -30;
			b_x= s_x = 593 - screen->w/2;
			b_y = s_y = 1216 - screen->h/2;
			//b_x= s_x = 506 - screen->w/2;
		//	b_y = s_y = 446 - screen->h/2;
			step_tol = 3;
			cx1= 9;		cy1= 46;   //14;
			cx2=35;		cy2=52 ;
			
			ex1=2; ey1=10;
			ex2=37; ey2=54;
			attacking = false;
			recoil=6;
			hitEnemy=false;
			HP = 5;
		}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
		bool loadSprites(){
			// loads Righ stances of sprite
			// assets/bat_attack/nincompoop0001.bmp
			hp = load_bitmap ("assets/health.bmp",workpal);
			
			sprite_walk[1][0] = load_bitmap ("assets/walk/n0001.bmp",workpal);
			sprite_walk[1][1] = load_bitmap ("assets/walk/n0002.bmp",workpal);
			sprite_walk[1][2] = load_bitmap ("assets/walk/n0003.bmp",workpal);
			sprite_walk[1][3] = load_bitmap ("assets/walk/n0004.bmp",workpal);
			sprite_walk[1][4] = load_bitmap ("assets/walk/n0005.bmp",workpal);
			sprite_walk[1][5] = load_bitmap ("assets/walk/n0006.bmp",workpal);
			sprite_walk[1][6] = load_bitmap ("assets/walk/n0007.bmp",workpal);
			sprite_walk[1][7] = load_bitmap ("assets/walk/n0008.bmp",workpal);
			sprite_walk[1][8] = load_bitmap ("assets/walk/n0009.bmp",workpal);
			sprite_walk[1][9] = load_bitmap ("assets/walk/n0010.bmp",workpal);
			sprite_walk[1][10] = load_bitmap ("assets/walk/n0011.bmp",workpal);
			sprite_walk[1][11] = load_bitmap ("assets/walk/n0012.bmp",workpal);
																
			sprite_walk[3][0] = load_bitmap ("assets/attack/n0001.bmp",workpal);
			sprite_walk[3][1] = load_bitmap ("assets/attack/n0002.bmp",workpal);
			sprite_walk[3][2] = load_bitmap ("assets/attack/n0003.bmp",workpal);
			sprite_walk[3][3] = load_bitmap ("assets/attack/n0004.bmp",workpal);
			sprite_walk[3][4] = load_bitmap ("assets/attack/n0005.bmp",workpal);
			sprite_walk[3][5] = load_bitmap ("assets/attack/n0006.bmp",workpal);
			sprite_walk[3][6] = load_bitmap ("assets/attack/n0007.bmp",workpal);
			sprite_walk[3][7] = load_bitmap ("assets/attack/n0008.bmp",workpal);
			sprite_walk[3][8] = load_bitmap ("assets/attack/n0009.bmp",workpal);
			sprite_walk[3][9] = load_bitmap ("assets/attack/n0010.bmp",workpal);
			sprite_walk[3][10] = load_bitmap ("assets/attack/n0011.bmp",workpal);
			sprite_walk[3][11] = load_bitmap ("assets/attack/n0012.bmp",workpal);
			
			_w = sprite_walk[1][0]->w;				// height & width
			_h = sprite_walk[1][0]->h;
			
			// mirror into left stances
			for (int i=0;i<12;i++){
				sprite_walk[0][i]=create_bitmap(_w,_h);
         		clear_to_color (sprite_walk[0][i],makecol(255,0,255));
         		rotate_sprite_v_flip (sprite_walk[0][i],sprite_walk[1][i],0,0,itofix(128));
			}
			
			// mirror into left attack stances
			for (int i=0;i<12;i++){
				sprite_walk[2][i]=create_bitmap(_w,_h);
         		clear_to_color (sprite_walk[2][i],makecol(255,0,255));
         		rotate_sprite_v_flip (sprite_walk[2][i],sprite_walk[3][i],0,0,itofix(128));
			}
			
			return true;
		}
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
		void levelCollision(BITMAP *lev){
			// ** Collision test with the current platform
			
			int cf;		// Boundary indicator
			bool check;
			
			y+=s_y;
		    x+=s_x;
		    
		    int next_x = x + xvel;
		    int next_y = y + yvel;
		    
   		    if (xvel>0) next_x--;
		    if (xvel<0) next_x++;

		    
		    //[======= On ground check  ==========]
		    onGround=false;
			int boo=y+cy2+1;
        	for (int foo=x+cx1; foo<=x+cx2; foo++) {
				cf = 1;
				cf = getpixel (lev,foo,boo);
            	if (cf==0){
					//textout_ex (screen,font,"g found",0,0,makecol(0,0,0),makecol(255,255,255));
	                if (!jumping) onGround=true;
                	if (dropping==true)
                    	dropTime=jumpTime;
                	break;
				}
            }
            
            //[===== Movement binders ========]
    
		   	//-- downward fall limiter
			if (yvel>0 && !jumping) {
			    yvel=0;
			    check = false;
			    for (int boo=y+cy2+1; boo<=next_y+cy2+1; boo++){
			        for (int foo=x+cx1; foo<=x+cx2; foo++){
						cf =1;
						cf = getpixel (lev,foo,boo);
			            if (cf==0){
							//textout_ex (screen,font,"g bind",0,0,makecol(0,0,0),makecol(255,255,255));
			                check=true;
			                break;
			            }
			        }
			        if (check)
			        	break;
			        yvel++;
				}	
			}
			//-- end of downfall limiter
			
			//-- fly limiter
			/*if (yvel<0) {
				yvel=0;
				check=false;
			    for (int boo=y+cy1; boo>=next_y+cy1; boo--){
			        for (int foo=x+cx1; foo<=x+cx2; foo++){
						cf=1;
						cf = getpixel (lev,foo,boo);
			            if (cf==0){
							//textout_ex (screen,font,"fly lim",0,0,makecol(0,0,0),makecol(255,255,255));
			                check=true;
			                break;
			            }
			        }
		        	if (check){
		            	yvel++;
			            break;
			        }
			        yvel--;
		    	}
			    if (yvel==0) {
			        jumpTime=-1*((g/.56-2*jumpVel)/g);
			    }
			}*/
			//-- end of fly limiter
			
			//LR limiters
  			//right
   			if (xvel>0) {
    			xvel=0;
    			cf=1;
    			check=false;
    			for (int boo=x+cx2+1; boo<=next_x+cx2+1; boo++){
        			for (int foo=y+cy1; foo<=y+cy2; foo++){
						cf=1;
            			cf=getpixel (lev,boo,foo);
            			if (cf==0){
                			check=true;
                			break;
            			}
        			}
        			if (check){
            			break;
        			}
        			xvel++;
    			}
   			}
   			
   			
   			//left
   			if (xvel<0) {
    			xvel=0;
    			cf=1;
    			check=false;
    			for (int boo=x+cx1-1; boo>=next_x+cx1-1; boo--){
        			for (int foo=y+cy1; foo<=y+cy2; foo++){
            			cf=getpixel (lev,boo,foo);
            			if (cf==0){
                			check=true;
                			break;
            			}
            			
        			}
		        	if (check){
						xvel=0;
            			break;
        			}
        			xvel--;
    			}
   			}
   			
   			y-=s_y;
        	x-=s_x;
        	if (xvel!=0 && !jumping) heroFrame = (heroFrame+1)%12;
        	if (attacking) heroFrame = ((heroFrame+1))%12;
        }

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
		void draw (BITMAP *bmp){
		//	masked_blit (sprite_walk[direction][cframe]
			if (xvel!=0){
                   x+=xvel;
                   s_x+=xvel;
                   //b_x+=xvel;
                   b_x+=(int)((double)xvel*.5);
                   x-=xvel;
    		}

    		if (yvel!=0){
                   y+=yvel;
                   s_y+=yvel;
                   //b_y+=yvel;
                   b_y+=(int)((double)yvel*.5);				
                   y-=yvel;
    		}
    		
    		if (!attacking){	
    			masked_blit (sprite_walk[direction][heroFrame],bmp,
						0,0,x,y,
						sprite_walk[direction][heroFrame]->w,
						sprite_walk[direction][heroFrame]->h
					);
			} else {
    			masked_blit (sprite_walk[direction+2][heroFrame],bmp,
						0,0,x,y,
						sprite_walk[direction+2][heroFrame]->w,
						sprite_walk[direction+2][heroFrame]->h
					);
			}
		}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
		void decideKeys(){
			bool keyLR=false;
    		walking=false;
			accel = 0.2;							// reset the vel to 1 for non control key
			velMax = 1;
			
			if (key[KEY_LCONTROL]) {
	        	accel=.4;				// control boost
	        	velMax = 3;
    		}
    		
    		if ((jumping || dropping)&& !onGround){
				if (key[KEY_LCONTROL])
					velMax = 3;
				else
					velMax = 2;
			}

			if (key[KEY_RIGHT]){
				direction=1;
				xvel=(int) velocity;
		        walking=1;
		        velocity+=accel;
		        keyLR = true;
			}
		
			if (key[KEY_LEFT]){
				direction=0;
				xvel=-(int)velocity;
	        	walking=1;
	        	velocity+=accel;
	        	keyLR=true;
			}
			
			if (key[KEY_ALT])
		    {
				if ((yvel<0 || onGround) ){
					g--;
					if (g<10)
						g=10;
				}
					
		        if (onGround){
					g=16;
		            jumping=true;
		            jumpTime=0.1;
		        }
		    }
			
			
		 	if (jumping){
		    	jumpTime+=.04;
        		yvel=-1*(int)(jumpVel - g/2 - g*jumpTime);
        		if (yvel>=0){
            		yvel=0;
            		jumping=false;
            		dropping=true;
            		dropTime=jumpTime;
            		jumpTime=0;
		        }
			}
			
			if (!onGround && !jumping){
	            yvel=0;
            	dropTime-=.04;
            	yvel=1*(int)(jumpVel - g/2 - g*dropTime);
            	if (dropTime<=0){
                	dropping=0;
                	dropTime=0;
            	}   
    		}
			
			if (velocity>velMax){
				velocity=velMax;
			}
			
			if (!keyLR || hitEnemy){
				if (velocity>0) {
					velocity-=.6/6;
					if (velocity<0)
						velocity=0;
						
					xvel=(int) ((direction==0) ? -velocity:velocity);
				}
			}
			
			attacking  = false;
			if (key[KEY_SPACE]){
				attacking = true;
			}
		}
};


