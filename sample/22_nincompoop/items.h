
using namespace std;
BITMAP *dis[4];

class items {
	public:
		// *** Bitmap & allegro assets for items
		BITMAP *sprite[12];				// 12 frame animation 
		
		
		// *** Motion variables
		double animFrames;
		int s_x,s_y;							// screen fix;
		int x,y, _w, _h;						// x,y, height, width
		int cx1,cx2,cy1,cy2;					// collision params
		int b_x,b_y;
		int maxFrame;							// all enemies frames vary
		int value;
		int animFrame;
		int typ;
		bool alive;
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
		items(){
			// *** initial motion settings
			alive = true;
			animFrame = 0;
			animFrames = 0;
			b_x= s_x = 0;
			b_y = s_y = 0;
			dis[0] = load_bitmap ("assets/items/0/frame1.bmp",workpal);
			dis[1] = load_bitmap ("assets/items/0/frame2.bmp",workpal);
			dis[2] = load_bitmap ("assets/items/0/frame3.bmp",workpal);
			dis[3] = load_bitmap ("assets/items/0/frame4.bmp",workpal);
		}


/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////	
		bool loadSprites(int type,int _x,int _y){
			typ=type;
			maxFrame = 4;
			if (type == 1){
				
				// loads Righ stances of sprite
				sprite[0] = load_bitmap ("assets/items/1/frame1.bmp",workpal);
				sprite[1] = load_bitmap ("assets/items/1/frame2.bmp",workpal);
				sprite[2] = load_bitmap ("assets/items/1/frame3.bmp",workpal);
				sprite[3] = load_bitmap ("assets/items/1/frame4.bmp",workpal);
				x = _x;
				y = _y;
			}
			
			if (type == 2){

				// loads Righ stances of sprite
				sprite[0] = load_bitmap ("assets/items/2/frame1.bmp",workpal);
				sprite[1] = load_bitmap ("assets/items/2/frame2.bmp",workpal);
				sprite[2] = load_bitmap ("assets/items/2/frame3.bmp",workpal);
				sprite[3] = load_bitmap ("assets/items/2/frame4.bmp",workpal);
				x = _x;
				y = _y;
			}
			
			if (type == 3){

				// loads Righ stances of sprite
				sprite[0] = load_bitmap ("assets/items/3/frame1.bmp",workpal);
				sprite[1] = load_bitmap ("assets/items/3/frame2.bmp",workpal);
				sprite[2] = load_bitmap ("assets/items/3/frame3.bmp",workpal);
				sprite[3] = load_bitmap ("assets/items/3/frame4.bmp",workpal);
				x = _x;
				y = _y;
			}
			
			if (type == 4){

				// loads Righ stances of sprite
				sprite[0] = load_bitmap ("assets/items/4/frame1.bmp",workpal);
				sprite[1] = load_bitmap ("assets/items/4/frame2.bmp",workpal);
				sprite[2] = load_bitmap ("assets/items/4/frame3.bmp",workpal);
				sprite[3] = load_bitmap ("assets/items/4/frame4.bmp",workpal);
				x = _x;
				y = _y;
			}
			

			
			_w = sprite[0]->w;				// height & width
			_h = sprite[0]->h;
			
			cx1=0; cx2=_w-1; cy1=0; cy2=_h-1;
			return true;
		}
		
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
		void draw (BITMAP *bmp,int nx,int ny){
    		animFrames+=.05;
    		if (animFrames>20) animFrames = 0;
    		animFrame = ((int)animFrames)%maxFrame;
    		if (animFrame>6){
				animFrame = 0;
			}
    			masked_blit (sprite[animFrame],bmp,
						0,0,x - nx,y-ny,
						sprite[animFrame]->w,
						sprite[animFrame]->h
					);
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

vector<items> itemset;
vector<items>::iterator ii;

void load_items(char efile[]){
	FILE *fp;
	fp = fopen (efile,"r");
	
	if (!fp){
		allegro_exit();
		cout << "Error opening level files\n";
		system ("pause");
	}
	
	items en;
	int type,x,y;
	
	while ( (fscanf(fp,"%d%d%d",&type,&x,&y))!=EOF ){
		en.loadSprites(type,x,y);
		itemset.push_back (en);
	}
	fclose(fp);
}

