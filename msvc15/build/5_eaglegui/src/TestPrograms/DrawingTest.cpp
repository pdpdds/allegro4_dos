

#include "DrawingTest.hpp"

#include "Eagle.hpp"

#include <cstdlib>

using std::endl;
using std::list;

void RectangleDrawingTest();
void CircleDrawingTest();
void CombineRectangleTest();
void ConsolidateRectanglesTest();
void StretchTest();


BitmapHandler buffer;

int DrawingTestMain(int argc , char** argv) {
   
   if (!SendOutputToFile("DrawingTestMain_Log.txt" , "Drawing Test\n\n" , false)) {return 0;}
   
   if (!GetAllegroReady(true , true , true)) {return 0;}
   if (!DesktopInfoKnown()) {return 0;}
   SetupInputHandler();
   
   const int dw = 800;
   const int dh = 600;
   GfxModeData gfxmode(GFX_AUTODETECT_WINDOWED , dw , dh , DCD());

   gfx_mode_handler.Setup(SW_BACKGROUND , 0 , 0);
   int ret = gfx_mode_handler.ChangeToMode(gfxmode , CRM_DIRECT);
   if (ret != FULL_SUCCESS) {return 0;}
   
   set_window_title("Drawing Test");

//   bool Realloc(int color_depth , int width , int height , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type);
	if (!buffer.Realloc(bitmap_color_depth(screen) , dw , dh , MEMORY , SOLID)) {
	   OutputLog() << "Couldn't reallocate buffer." << endl;
	   return 0;
	}

   if (SetupAndStartTimer(BPS_TO_TIMER(60)) != 0) {return 0;}
   
   show_mouse(screen);
   
   double dt = 0.0;
   
   Input input_quit(input_key_press(KEY_ESC));

   bool show_instructions = true;
   
   const int white = makecol(255,255,255);

   while (!input_quit && !close_program) {
      while(GetTickCount() < 1) {rest(1);}
      
      dt = GetDeltaTime();
      UpdateInputHandler(dt);
      
      if (input_key_press(KEY_1)) {
         RectangleDrawingTest();
         show_instructions = true;
      }
      
      if (input_key_press(KEY_2)) {
         CircleDrawingTest();
         show_instructions = true;
      }
      
      if (input_key_press(KEY_3)) {
         CombineRectangleTest();
         show_instructions = true;
      }
      
      if (input_key_press(KEY_4)) {
         ConsolidateRectanglesTest();
         show_instructions = true;
      }
      if (input_key_press(KEY_5)) {
         StretchTest();
         show_instructions = true;
      }
      if (show_instructions) {
         acquire_screen();
         clear(screen);
         textout_ex(screen , font , "Key 1 for Rectangle Drawing Test (Keys = ESC , RMB , Hold RMB)" , 10 , 20 , white , -1);
         textout_ex(screen , font , "Key 2 for Circle Drawing Test (Keys = ESC , MSwheel , LMB , RMB , S)" , 10 , 36 , white , -1);
         textout_ex(screen , font , "Key 3 for Rectangle Combine Test" , 10 , 52 , white , -1);
         textout_ex(screen , font , "   (Keys = ESC , 1-4 , Left , Right , Up , Down , LMB , Hold LMB)" , 10 , 68 , white , -1);
         textout_ex(screen , font , "Key 4 for Rectangle Consolidation Test" , 10 , 84 , white , -1);
         textout_ex(screen , font , "   (Keys = ESC , Up , Down , R , P , F)" , 10 , 100 , white , -1);
         textout_ex(screen , font , "Key 5 for Stretch Blit Smooth Test" , 10 , 116 , white , -1);
         textout_ex(screen , font , "   (Keys = ESC , 1-4 , Left , Right , Up , Down , M , LMB , Hold LMB)" , 10 , 132 , white , -1);
         release_screen();
         show_instructions = false;
      }
      
   }
   
   buffer.Free();
   return 0;
}

void RectangleDrawingTest() {

   OutputLog() << "Beginning RectangleDrawingTest()..." << endl;

   const int black = makecol(0,0,0);
   const int white = makecol(255,255,255);
   const int cyan = makecol(0,255,255);
//   const int purple = makecol(127,0,255);
   const int magenta = makecol(255,0,255);
   const int mintgreen = makecol(127,255,127);
   const int darkblue = makecol(0,0,64);
   clear_to_color(screen , black);
   
   Rectangle r1(10,10,780,20);
   Rectangle r2(10,40,780,20);
   Rectangle r3(10,70,20,520);
   Rectangle r4(40,70,20,520);
   Rectangle r5(70,70,200,100);
   Rectangle r6(280,70,200,100);
   
   Rectangle r7(350,300,200,150);
   int num_frames = 0;

   Input input_quit(input_key_press(KEY_ESC));

   double dt = 0.0;

   int go = 1;

   while (!input_quit && !close_program) {
      while(GetTickCount() < 1) {rest(1);}
      
      dt = GetDeltaTime();
      UpdateInputHandler(dt);

      if (go) {
         clear_to_color(buffer , black);
//         r1.DrawGradientLtoR(buffer , cyan , purple);
//         r2.DrawGradientRtoL(buffer , cyan , purple);
         r1.DrawGradientLtoR(buffer , white , black);
         r2.DrawGradientRtoL(buffer , white , black);
         r3.DrawGradientTtoB(buffer , white , black);
         r4.DrawGradientBtoT(buffer , white , black);
         r5.DrawGradientOtoM(buffer , mintgreen , darkblue);
         r6.DrawGradientMtoO(buffer , mintgreen , darkblue);
         
         
         r7.DrawGradientFrame(buffer , num_frames , cyan , magenta);
         
         acquire_screen();
         buffer.DrawTo(screen , 0 , 0);
         release_screen();
         go = 0;
      }
      
      if (input_mouse_press(RMB) || mb_held_for(RMB , 0.5)) {
         int dx = mx - r7.X();
         int dy = my - r7.Y();
         num_frames = (abs(dx)>abs(dy))?dx:dy;
         go = 1;
      }
      if (ms_zdiff) {
         num_frames += ms_zdiff;
         go = 1;
      }
      
   }
   UpdateInputHandler(0.0);
}



void CircleDrawingTest() {

   OutputLog() << "Beginning CircleDrawingTest()..." << endl;

   double dt = 0.0;
   int go = 1;
   int rad1 = 25;
   int rad2 = 75;
   int cx = 400;
   int cy = 300;
//   const int red = makecol(255,0,0);
//   const int green = makecol(0,255,0);
//   const int cyan = makecol(0,255,255);
   const int black = makecol(0,0,0);
   const int white = makecol(255,255,255);
   int col1 = black;
   int col2 = white;
   
   Input input_quit(input_key_press(KEY_ESC));

   while(!input_quit && !close_program) {
      while(GetTickCount() < 1) {rest(1);}
      
      dt = GetDeltaTime();
      UpdateInputHandler(dt);
      
      if (ms_zdiff) {
         rad1 += ms_zdiff;
         if (rad1 < 0) {rad1 = 0;}
         if (rad1 > rad2) {rad1 = rad2;}
         go = 1;
      }
      
      if (input_mouse_press(LMB) || mb_held_for(LMB , 0.5)) {
         double dx = mx - cx;
         double dy = my - cy;
         double radius = sqrt(dx*dx + dy*dy);
         rad2 = (int)radius;
         if (rad2 <= rad1) {rad2 = rad1 + 1;}
         go = 1;
      }
      if (input_mouse_press(RMB) || mb_held_for(RMB , 0.5)) {
         double dx = mx - cx;
         double dy = my - cy;
         double radius = sqrt(dx*dx + dy*dy);
         rad1 = (int)radius;
         if (rad1 >= rad2) {rad1 = rad2 - 1;}
         go = 1;
      }
      
      if (input_key_press(KEY_S)) {
         int temp = col1;
         col1 = col2;
         col2 = temp;
         go = 1;
      }
      
      if (go) {
         clear(buffer);
         
//         circle(buffer , cx , cy , rad , red);
//         do_circle_nogap(buffer , cx , cy , rad , green , ((BITMAP*)buffer)->vtable->putpixel);
         
//         ring(buffer , cx , cy , rad1 , rad2 , green);
         
         ring_gradient(buffer , cx , cy , rad1 , rad2 , col1 , col2);
         
         acquire_screen();
         buffer.DrawTo(screen , 0 , 0);
         release_screen();
         go = 0;
      }
      
      
   }
   UpdateInputHandler(0.0);
}



void CombineRectangleTest() {
   
   OutputLog() << "Beginning CombineRectangleTest()..." << endl;
   
   // 800 X 600 split into 2 400X600
   int r1tlx = 50 , r1tly = 50 , r1brx = 249 , r1bry = 249;
   int r2tlx = 100 ,r2tly = 100 , r2brx = 299 , r2bry = 299;
   Rectangle r1(r1tlx , r1tly , r1brx - r1tly + 1 , r1bry - r1tly + 1);
   Rectangle r2(r2tlx , r2tly , r2brx - r2tly + 1 , r2bry - r2tly + 1);
   list<Rectangle> rlist;
   
   int *xp = &r1tlx;
   int *yp = &r1tly;
   
   BITMAP* left = create_sub_bitmap(buffer , 0 , 0 , 400 , 600);
   BITMAP* right = create_sub_bitmap(buffer , 400 , 0 , 400 , 600);
   if (!left || !right) {
      OutputLog() << "Could not create sub bitmaps in combine rectangle test." << endl;
      if (left) {destroy_bitmap(left);}
      if (right) {destroy_bitmap(right);}
      return;
   }
   
   int show = 1;
   
   Input input_quit(input_key_press(KEY_ESC));
   
   double dt = 0.0;
   
   int green = makecol(0,255,0);
   int blue = makecol(0,0,255);
   int black = makecol(0,0,0);
   int rcol[3] = {
      makecol(255,0,0),
      makecol(255,127,0),
      makecol(255,255,0)
   };
   
   while (!input_quit && !close_program) {
      while(GetTickCount() < 1) {rest(1);}
      
      dt = GetDeltaTime();
      UpdateInputHandler(dt);
      
      if (input_key_press(KEY_1)) {
         xp = &r1tlx;
         yp = &r1tly;
      }
      if (input_key_press(KEY_2)) {
         xp = &r1brx;
         yp = &r1bry;
      }
      if (input_key_press(KEY_3)) {
         xp = &r2tlx;
         yp = &r2tly;
      }
      if (input_key_press(KEY_4)) {
         xp = &r2brx;
         yp = &r2bry;
      }
      if (input_mouse_press(LMB) || mb_held_for(LMB , 0.5)) {
         *xp = mx;
         *yp = my;
         show = 1;
      }
      if (input_key_press(KEY_LEFT) || key_held_for(KEY_LEFT , 0.5)) {
         --(*xp);
         show = 1;
      }
      if (input_key_press(KEY_RIGHT) || key_held_for(KEY_RIGHT , 0.5)) {
         ++(*xp);
         show = 1;
      }
      if (input_key_press(KEY_UP) || key_held_for(KEY_UP , 0.5)) {
         --(*yp);
         show = 1;
      }
      if (input_key_press(KEY_DOWN) || key_held_for(KEY_DOWN , 0.5)) {
         ++(*yp);
         show = 1;
      }
      if (show) {
         r1.SetCorners(r1tlx,r1tly,r1brx,r1bry);
         r2.SetCorners(r2tlx,r2tly,r2brx,r2bry);
         rlist = CombineRectangles(r1 , r2);
         if (rlist.size() == 0) {
            rlist.push_back(r1);
            rlist.push_back(r2);
         }
         clear_to_color(buffer , black);
         r1.Draw(left , green);
         r2.Draw(left , blue);
         
         int i = 0;
         if (rlist.size() >= 4) {OutputLog() << "Error, rlist size is " << rlist.size() << endl;}
         for (list<Rectangle>::iterator it = rlist.begin() ; it != rlist.end() ; ++it) {
            if (i == 3) {break;}
            Rectangle& r = *it;
            r.Draw(right , rcol[i]);
            ++i;
         }
         acquire_screen();
         buffer.DrawTo(screen , 0 , 0);
         release_screen();
         show = 0;
      }
      
   }
   
   UpdateInputHandler(0.0);
   destroy_bitmap(left);
   destroy_bitmap(right);
   
}



void ConsolidateRectanglesTest() {
   
   OutputLog() << "Beginning ConsolidateRectanglesTest()..." << endl;
   
   list<Rectangle> rlist,clist;
   
   BITMAP* left = create_sub_bitmap(buffer , 0 , 0 , 400 , 600);
   BITMAP* right = create_sub_bitmap(buffer , 400 , 0 , 400 , 600);
   if (!left || !right) {
      OutputLog() << "Could not create sub bitmaps in combine rectangle test." << endl;
      if (left) {destroy_bitmap(left);}
      if (right) {destroy_bitmap(right);}
      return;
   }
   
   int randomize = 1;
   int show = 1;
   int fill = 0;
   
   Input input_quit(input_key_press(KEY_ESC));
   
   double dt = 0.0;

   int numrectangles = 10;
   int r,g,b,col;
   srand(time(NULL));

   while (!input_quit && !close_program) {
      while(GetTickCount() < 1) {rest(1);}
      
      dt = GetDeltaTime();
      UpdateInputHandler(dt);
      
      if (input_key_press(KEY_R)) {
         randomize = 1;
      }
      if (input_key_press(KEY_DOWN) || key_held_for(KEY_DOWN , 0.5)) {
         --numrectangles;
         if (numrectangles < 2) {numrectangles = 2;}
         randomize = 1;
      }
      if (input_key_press(KEY_UP) || key_held_for(KEY_UP , 0.5)) {
         ++numrectangles;
         if (numrectangles > 30) {numrectangles = 30;}
         randomize = 1;
      }
      if (input_key_press(KEY_P)) {
         OutputLog() << "Printing rlist contents - " << rlist.size() << " entries :" << endl;
         for (list<Rectangle>::iterator it = rlist.begin() ; it != rlist.end() ; ++it) {
            OutputLog() << "   " << *it << endl;
         }
         OutputLog() << endl;
      }
      if (input_key_press(KEY_F)) {
         fill = !fill;
         show = 1;
      }
      if (randomize) {
         rlist.clear();
         for (int i = 0 ; i < numrectangles ; ++i) {
//            int x = rand()%400;
//            int y = rand()%600;
//            int w = rand()%(400-x);
//            int h = rand()%(600-y);
            int x = rand()%350;
            int y = rand()%550;
            int w = rand()%(350-x) + 50;
            int h = rand()%(550-y) + 50;
            rlist.push_back(Rectangle(x,y,w,h));
         }
         clist = ConsolidateRectangles(rlist);
         randomize = 0;
         show = 1;
      }
      if (show) {
         clear(buffer);
         double h = 180.0;
         for (list<Rectangle>::iterator it = rlist.begin() ; it != rlist.end() ; ++it) {
            hsv_to_rgb(h,1.0,1.0,&r,&g,&b);
            col = makecol(r,g,b);
            if (fill) {
               it->Fill(left,col);
            } else {
               it->Draw(left,col);
            }
            h += (90.0/(double)(numrectangles - 1));
         }
         h = 0.0;
         for (list<Rectangle>::iterator it = clist.begin() ; it != clist.end() ; ++it) {
            hsv_to_rgb(h,1.0,1.0,&r,&g,&b);
            col = makecol(r,g,b);
            if (fill) {
               it->Fill(right,col);
            } else {
               it->Draw(right,col);
            }
            double numsteps = (double)(clist.size() - 1);
            if (numsteps == 0.0) {numsteps = 1.0;}
            h += 90.0/numsteps;
         }
         acquire_screen();
         buffer.DrawTo(screen,0,0);
         release_screen();
         show = 0;
      }
   }
   UpdateInputHandler(0.0);
   destroy_bitmap(left);
   destroy_bitmap(right);
}



void StretchTest() {
   
   OutputLog() << "Beginning StretchTest()..." << endl;
   
   int r1tlx = 50 , r1tly = 50 , r1brx = 349 , r1bry = 249;
   int r2tlx = 125 ,r2tly = 125 , r2brx = 274 , r2bry = 224;
   Rectangle r1(r1tlx , r1tly , r1brx - r1tly + 1 , r1bry - r1tly + 1);
   Rectangle r2(r2tlx , r2tly , r2brx - r2tly + 1 , r2bry - r2tly + 1);

   int *xp = &r1tlx;
   int *yp = &r1tly;

   BITMAP* topleft = create_sub_bitmap(buffer , 0 , 0 , 400 , 300);
   BITMAP* topright = create_sub_bitmap(buffer , 400 , 0 , 400 , 300);
   BITMAP* botleft = create_sub_bitmap(buffer , 0 , 300 , 400 , 300);
   BITMAP* botright = create_sub_bitmap(buffer , 400 , 300 , 400 , 300);
   if (!topleft || !topright || !botleft || !botright) {
      OutputLog() << "Could not create sub bitmaps in stretch test." << endl;
      if (topleft) {destroy_bitmap(topleft);}
      if (topright) {destroy_bitmap(topright);}
      if (botleft) {destroy_bitmap(botleft);}
      if (botright) {destroy_bitmap(botright);}
      return;
   }
   
//   BitmapHandler(const char* filepath , RGB* pal , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type);
/*
   BitmapHandler image("Images\\k9_05.bmp" , NULL , MEMORY , SOLID);
   if (!image) {
      OutputLog() << "Could not load k9_05.bmp." << endl;
      return;
   }
*/
   BitmapHandler image("Images\\Maze1.bmp" , NULL , MEMORY , SOLID , true);
   if (!image) {
      OutputLog() << "Could not load Maze1.bmp." << endl;
      return;
   }
   BitmapHandler maskimage("Images\\MaskMaze1.bmp" , NULL , MEMORY , TRANSPARENT , true);
   if (!maskimage) {
      OutputLog() << "Could not load MaskMaze1.bmp." << endl;
      return;
   }
   
   bool masked = false;
   
   int show = 1;
   
   Input input_quit(input_key_press(KEY_ESC));
   
   double dt = 0.0;
   
   int green = makecol(0,127,0);
   
   while (!input_quit && !close_program) {
      while (GetTickCount() < 1) {rest(1);}
      
      dt = GetDeltaTime();
      UpdateInputHandler(dt);
      
      if (input_key_press(KEY_1)) {
         xp = &r1tlx;
         yp = &r1tly;
         show = 1;
      }
      if (input_key_press(KEY_2)) {
         xp = &r1brx;
         yp = &r1bry;
         show = 1;
      }
      if (input_key_press(KEY_3)) {
         xp = &r2tlx;
         yp = &r2tly;
         show = 1;
      }
      if (input_key_press(KEY_4)) {
         xp = &r2brx;
         yp = &r2bry;
         show = 1;
      }
      if (input_key_press(KEY_M)) {
         masked = !masked;
         show = 1;
      }
      if (input_mouse_press(LMB) || mb_held_for(LMB , 0.5)) {
         *xp = mx;
         *yp = my;
//         if (*xp < 0) {*xp = 0;}
//         if (*yp < 0) {*yp = 0;}
         if (*xp > 399) {*xp -= 400;}
         if (*yp > 299) {*yp -= 300;}
         show = 1;
      }
      if (input_key_press(KEY_LEFT) || key_held_for(KEY_LEFT , 0.5)) {
         --(*xp);
         if (*xp < 0) {*xp = 0;}
         show = 1;
      }
      if (input_key_press(KEY_RIGHT) || key_held_for(KEY_RIGHT , 0.5)) {
         ++(*xp);
         if (*xp > 399) {*xp = 399;}
         show = 1;
      }
      if (input_key_press(KEY_UP) || key_held_for(KEY_UP , 0.5)) {
         --(*yp);
         if (*yp < 0) {*yp = 0;}
         show = 1;
      }
      if (input_key_press(KEY_DOWN) || key_held_for(KEY_DOWN , 0.5)) {
         ++(*yp);
         if (*yp > 299) {*yp = 299;}
         show = 1;
      }
      if (show) {
         r1.SetCorners(r1tlx,r1tly,r1brx,r1bry);
         r2.SetCorners(r2tlx,r2tly,r2brx,r2bry);
         
         clear_to_color(buffer , makecol(0,0,0));
         if (masked) {
            clear_to_color(topleft , bitmap_mask_color(topleft));
            clear_to_color(botleft , bitmap_mask_color(botleft));
            blit(maskimage , topleft , 200 , 0 , 0 , 0 , 400 , 300);
            blit(maskimage , botleft , 200 , 0 , 0 , 0 , 400 , 300);
         } else {
            blit(image , topleft , 200 , 0 , 0 , 0 , 400 , 300);
            blit(image , botleft , 200 , 0 , 0 , 0 , 400 , 300);
         }
         
         if (masked) {
            masked_stretch_blit_smooth(topleft , topright , r1.X() , r1.Y() , r1.W() , r1.H() , r2.X() , r2.Y() , r2.W() , r2.H());
            masked_stretch_blit(botleft , botright , r1.X() , r1.Y() , r1.W() , r1.H() , r2.X() , r2.Y() , r2.W() , r2.H());
         } else {
            stretch_blit_smooth(topleft , topright , r1.X() , r1.Y() , r1.W() , r1.H() , r2.X() , r2.Y() , r2.W() , r2.H());
            stretch_blit(botleft , botright , r1.X() , r1.Y() , r1.W() , r1.H() , r2.X() , r2.Y() , r2.W() , r2.H());
         }
         
         r1.Draw(topleft , green);
         r1.Draw(botleft , green);
         r2.Draw(topright , green);
         r2.Draw(botright , green);
//         putpixel(topleft , *xp , *yp , white);
//         putpixel(botleft , *xp , *yp , white);
         
         
         acquire_screen();
         buffer.DrawTo(screen , 0 , 0);
         release_screen();
         
         show = 0;
      }
   }
   UpdateInputHandler(0.0);
}








