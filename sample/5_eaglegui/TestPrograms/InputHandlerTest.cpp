

#include "InputHandlerTest.hpp"


#include "Eagle.hpp"

#include <fstream>
using std::ofstream;
#include <iostream>
using std::endl;


volatile bool quit = false;

void CloseButtonHandler() {
   quit = true;
}
END_OF_FUNCTION(CloseButtonHandler);



int InputHandlerTestMain(int argc , char** argv) {
   
   
   ofstream logfile("InputHandlerTest_Log.txt");
   if (!logfile) {return 0;}
   
   
   const int dw = 1024;
   const int dh = 768;
   const int gmode = GFX_AUTODETECT_FULLSCREEN;

   if (allegro_init()     != 0) {return 0;}
   if (install_timer()    != 0) {return 0;}
   if (install_keyboard() != 0) {return 0;}
   if (install_mouse()     < 0) {return 0;}

   SetupKeyHandler();
   SetupMouseHandler();


   int dcd = desktop_color_depth();
   if (dcd == 0) {dcd = 32;}

   set_color_depth(dcd);
   if (set_gfx_mode(gmode , dw , dh , 0 , 0) != 0) {return 0;}

   clear_to_color(screen , makecol(0,0,0));
   BitmapHandler bg(create_bitmap(dw , dh) , SOLID);
   BitmapHandler buffer(create_bitmap(dw , dh) , SOLID);
   if (!bg || !buffer) {
      return 0;
   }
   bg.Clear();
   buffer.Clear();
   
   set_close_button_callback(CloseButtonHandler);
   
   
///   const int mbtn_index[NUM_MBTNS] = {LMB , RMB , MMB , EMB1 , EMB2 , EMB3 , EMB4 , EMB5};

   const int NUM_INPUTS = KEY_STATE_EXTENDED_MAX + NUM_MBTNS;
   
   Input input_keys[NUM_INPUTS][NUMINPUTTYPES];//~140 keys
   
   for (int i = 0 ; i < KEY_STATE_EXTENDED_MAX ; ++i) {
      for (int j = 0 ; j < NUMINPUTTYPES ; ++j) {
         input_keys[i][j].AssignTo(KB , j , i);
      }
   }
   for (int i = KEY_STATE_EXTENDED_MAX ; i < NUM_INPUTS ; ++i) {
      for (int j = 0 ; j < NUMINPUTTYPES ; ++j) {
         input_keys[i][j].AssignTo(MS , j , i - KEY_STATE_EXTENDED_MAX);
      }
   }
   
   
   const char* keynames[NUM_INPUTS];
   
   for (int i = 0 ; i < KEY_STATE_EXTENDED_MAX ; ++i) {
      keynames[i] = keycode_to_name(i);
   }
   for (int i = 0 ; i < NUM_MBTNS ; ++i) {
      keynames[KEY_STATE_EXTENDED_MAX + i] = mousecode_to_name(i);
   }
   
   int col1width = 0, col6width = 0 , col2x = 0, col7x = 0;
   int width = 0 , maxwidth = 0;
   
   for (int i = 0 ; i < 75 ; ++i) {
      width = text_length(font , keynames[i]);
      if (width > maxwidth) {maxwidth = width;}
   }
   col1width = maxwidth + 8;
   col2x = col1width;
   maxwidth = 0;
   for (int i = 75 ; i < NUM_INPUTS ; ++i) {
      width = text_length(font , keynames[i]);
      if (width > maxwidth) {maxwidth = width;}
   }
   col6width = maxwidth + 8;
   col7x = 512 + col6width;
   
   const char* header = "   Press Release    Hold    Open";// 32 chars wide
   // 1024 X 768
   // 768/10 = 76 lines, 1024/8 = 128 chars wide
   // 70 rows, 10 columns [strmax1,8,8,8,8,strmax2,8,8,8,8 chars], (strmax1 + strmax2) = (128 - 2*32) = 64
   // 32 char max for key names
   
   int x = 0 , y = 0;
   
   const int textcol = makecol(255,255,255);
   
   InputGroup QuitKey = input_key_press(KEY_ESC);// && input_key_held(KEY_ONLY_CTRL_ALT);
   
   double dt = 0.0;
   if (SetupAndStartTimer(BPS_TO_TIMER(60)) != 0) {return 0;}
   
   while (!quit && !QuitKey) {
      while(GetTickCount() < 1) {rest(1);}
      dt = GetDeltaTime();
      
      /// Logic
      UpdateInputHandler(dt);
      
      
      /// Draw
      buffer.Clear();
      
      // Headers
      textout_ex(buffer , font , header , col2x , 1 , textcol , -1);
      textout_ex(buffer , font , header , col7x , 1 , textcol , -1);
      // First set of columns
      x = 0;
      for (int i = 0 ; i < 75 ; ++i) {
         y = 10*(i+1) - 1;
         textout_ex(buffer , font , keynames[i] , 0 , y , textcol , -1);
         textprintf_ex(buffer , font , col2x , y , textcol , -1 , "%8i%8i%8i%8i" ,
                        (int)input_keys[i][0] , (int)input_keys[i][1] , (int)input_keys[i][2] , (int)input_keys[i][3]);
         // %-8i
      }
      // Second set of columns
      x = 512;
      for (int j = 0 , i = 75 ; i < NUM_INPUTS ; ++i , ++j) {
         y = 10*(j+1) - 1;
         textout_ex(buffer , font , keynames[i] , 512 , y , textcol , -1);
         textprintf_ex(buffer , font , col7x , y , textcol , -1 , "%8i%8i%8i%8i" ,
                        (int)input_keys[i][0] , (int)input_keys[i][1] , (int)input_keys[i][2] , (int)input_keys[i][3]);
      }
      
      buffer.DrawTo(screen , 0 , 0);
   }

   return 0;
}

