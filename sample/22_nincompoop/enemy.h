

using namespace std;

class enemies {
	public:
		// *** Bitmap & allegro assets for hero
		BITMAP *sprite_walk[2][12];				// 12 frame animation for walk
		
		// *** Motion variables
		bool walking, onGround,jumping,gotBorder;
		bool outaGround;
		int direction,oldDirection,heroFrame;
		int jumpVel;
		int xvel, yvel;							// xc,yc of hello.c
		double jumpTime, dropping, dropTime;
		double velocity, accel, velMax,heroFrames;
		int g;
		int s_x,s_y;							// screen fix;
		int x,y, _w, _h;						// x,y, height, width
		int cx1,cx2,cy1,cy2;					// collision params
		int b_x,b_y;
		int step_tol;							// lifting tolerance
		int maxFrame;							// all enemies frames vary
		bool alive;
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
		enemies(){
			// *** initial motion settings
			alive=true;
			gotBorder=jumping=onGround = walking = dropping = false;
			heroFrame = 0;
			heroFrames = 0;
			direction=1;
			oldDirection = 0;
			xvel=0;	yvel=0;
			jumpTime = 0;	dropping = 0; dropTime = 0;
			jumpVel = 15;
			g = 16;
			velocity = .1;
			b_x= s_x = 0;
			b_y = s_y = 0;
			
			velMax = .2;
			accel = 0.01;
			step_tol = 3;
			/*  SET at sprite load
			cx1= 0;		cy1= 46;   //14;
			cx2= ;		cy2=52 ;
			*/
		}


/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////	
		bool loadSprites(int type,int _x,int _y){
			if (type == 1){
				/******* MMX BOT  *******/
				maxFrame = 3;
				// loads Righ stances of sprite
				sprite_walk[1][0] = load_bitmap ("assets/enemies/1/frame1.bmp",workpal);
				sprite_walk[1][1] = load_bitmap ("assets/enemies/1/frame2.bmp",workpal);
				sprite_walk[1][2] = load_bitmap ("assets/enemies/1/frame3.bmp",workpal);
				x = _x;
				y = _y;
			}
			
			if (type == 2){
				/******* FIRE OWL  *******/
				maxFrame = 6;
				// loads Righ stances of sprite
				sprite_walk[1][0] = load_bitmap ("assets/enemies/2/frame1.bmp",workpal);
				sprite_walk[1][1] = load_bitmap ("assets/enemies/2/frame2.bmp",workpal);
				sprite_walk[1][2] = load_bitmap ("assets/enemies/2/frame3.bmp",workpal);
				sprite_walk[1][3] = load_bitmap ("assets/enemies/2/frame4.bmp",workpal);
				sprite_walk[1][4] = load_bitmap ("assets/enemies/2/frame5.bmp",workpal);
				sprite_walk[1][5] = load_bitmap ("assets/enemies/2/frame6.bmp",workpal);
				x = _x;
				y = _y;
			}
			
			if (type == 3){
				/******* FIRE OWL  *******/
				maxFrame = 5;
				// loads Righ stances of sprite
				sprite_walk[1][0] = load_bitmap ("assets/enemies/3/frame1.bmp",workpal);
				sprite_walk[1][1] = load_bitmap ("assets/enemies/3/frame2.bmp",workpal);
				sprite_walk[1][2] = load_bitmap ("assets/enemies/3/frame3.bmp",workpal);
				sprite_walk[1][3] = load_bitmap ("assets/enemies/3/frame4.bmp",workpal);
				sprite_walk[1][4] = load_bitmap ("assets/enemies/3/frame5.bmp",workpal);
				//rite_walk[1][5] = load_bitmap ("assets/enemies//2//frame6.bmp",workpal);
				x = _x;
				y = _y;
			}
			
			if (type == 4){
				/******* FIRE OWL  *******/
				maxFrame = 4;
				// loads Righ stances of sprite
				sprite_walk[1][0] = load_bitmap ("assets/enemies/4/frame1.bmp",workpal);
				sprite_walk[1][1] = load_bitmap ("assets/enemies/4/frame2.bmp",workpal);
				sprite_walk[1][2] = load_bitmap ("assets/enemies/4/frame3.bmp",workpal);
				sprite_walk[1][3] = load_bitmap ("assets/enemies/4/frame4.bmp",workpal);
				//sprite_walk[1][4] = load_bitmap ("assets/enemies//3//frame5.bmp",workpal);
				//rite_walk[1][5] = load_bitmap ("assets/enemies//2//frame6.bmp",workpal);
				x = _x;
				y = _y;
			}
			
			if (type == 5){
				/******* FIRE OWL  *******/
				maxFrame = 4;
				// loads Righ stances of sprite
				sprite_walk[1][0] = load_bitmap ("assets/enemies/5/frame1.bmp",workpal);
				sprite_walk[1][1] = load_bitmap ("assets/enemies/5/frame2.bmp",workpal);
				sprite_walk[1][2] = load_bitmap ("assets/enemies/5/frame3.bmp",workpal);
				sprite_walk[1][3] = load_bitmap ("assets/enemies/5/frame4.bmp",workpal);
				//sprite_walk[1][4] = load_bitmap ("assets/enemies//3//frame5.bmp",workpal);
				//rite_walk[1][5] = load_bitmap ("assets/enemies//2//frame6.bmp",workpal);
				x = _x;
				y = _y;
			}
			
			_w = sprite_walk[1][0]->w;				// height & width
			_h = sprite_walk[1][0]->h;
			
			cx1=0; cx2=_w-1; cy1=0; cy2=_h-1;
			
			// mirror into left stances
			for (int i=0;i<maxFrame;i++){
				sprite_walk[0][i]=create_bitmap(_w,_h);
         		clear_to_color (sprite_walk[0][i],makecol(255,0,255));
         		rotate_sprite_v_flip (sprite_walk[0][i],sprite_walk[1][i],0,0,itofix(128));
			}

			return true;
		}
		
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
		void levelCollision(BITMAP *lev){
			// ** Collision test with the current platform
			gotBorder = false;
			int cf;		// Boundary indicator
			bool check;
			
			//y+=s_y;
		    //x+=s_x;
		    
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
					
	                if (!jumping){
						onGround=true;
					}
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
            			if (cf==0 || cf == makecol(0,0,255)){
							direction = 0;
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
            			if (cf==0 || cf == makecol(0,255,0)){
							direction = 1;
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
   			
   			//y-=s_y;
        	//x-=s_x;

        }
		
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
		void draw (BITMAP *bmp,int nx,int ny){
		//	masked_blit (sprite_walk[direction][cframe]
			if (xvel!=0){
                   x+=xvel;
                   //s_x+=xvel;
                   //b_x+=xvel;
                   //b_x+=(int)((double)xvel*.5);
                  //x-=xvel;
    		}

    		if (yvel!=0){
                   y+=yvel;
                   //s_y+=yvel;
                   //b_y+=yvel;
                  // b_y+=(int)((double)yvel*.5);				
                   //y-=yvel;
    		}
    		heroFrames+=.2;
    		if (heroFrames>20) heroFrames = 0;
    		heroFrame = ((int)heroFrames)%maxFrame;
    		if (heroFrame>6){
				heroFrame = 0;
			}
    			masked_blit (sprite_walk[direction][heroFrame],bmp,
						0,0,x - nx,y-ny,
						sprite_walk[direction][heroFrame]->w,
						sprite_walk[direction][heroFrame]->h
					);
		}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
		void decideKeys(){
			bool keyLR=false;
    		walking=false;
			accel = 0.01;							// reset the vel to 1 for non control key
			velMax = .2;
			    		
			if (direction == 1){
				//direction=1;
				xvel=(int) velocity;
		        walking=1;
		        velocity+=accel;
		        keyLR = true;
			}

			if (direction == 0){
				//direction=1;
				xvel=-(int)velocity;
	        	walking=1;
	        	velocity+=accel;
	        	keyLR=true;
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
						

/*				if (velocity>0) {
					velocity-=.6/6;
					if (velocity<0)
						velocity=0;
						
					xvel=(int) ((direction==0) ? -velocity:velocity);
				}
*/
			
	
			
		oldDirection = direction;
		if (xvel>1) xvel=1;
		if (xvel<-1)	xvel = -1;
		
		}
};

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

vector<enemies> enemyset;
vector<enemies>::iterator ei;

void load_enemies(char efile[]){
	FILE *fp;
	fp = fopen (efile,"r");
	
	if (!fp){
		allegro_exit();
		cout << "Error opening level files/n";
		system ("pause");
	}
	
	enemies en;
	int type,x,y;
	
	while ( (fscanf(fp,"%d%d%d",&type,&x,&y))!=EOF ){
		en.loadSprites(type,x,y);
		enemyset.push_back (en);
	}
	fclose(fp);
}

