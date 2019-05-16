

#include "RecordInputTest.hpp"

#include "Eagle.hpp"

#include <iostream>
using std::endl;

#include <sstream>
using std::ostringstream;

#include <string>
using std::string;

#include <vector>
using std::vector;


int RecordInputTestMain(int argc , char** argv) {
   
   if (allegro_init()     != 0) {return 0;}
   if (install_timer()    != 0) {return 0;}
   if (install_keyboard() != 0) {return 0;}
   if (install_mouse()     < 0) {return 0;}
   
   const int dw = 800;
   const int dh = 600;
   const int gmode = GFX_AUTODETECT_WINDOWED;


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
   


   ostringstream ostr;
   string str;
   int textcol = makecol(255,255,255);
   
   const InputGroup quit = input_key_press(KEY_ESC);//(KB , PRESS , KEY_ESC);

   InputGroup input_group;

   vector<string> output;
   
   int width = 0 , maxwidth = 0;
   const int line_spacing = (3*text_height(font))/2;
   int height = 0;
   int tx = 0 , ty = 0;
   
   double dt = 0.0;
   SetupAndStartTimer(BPS_TO_TIMER(60));
   

   while(!quit) {
      while(GetTickCount() < 1) {rest(1);}
      dt = GetDeltaTime();
      
      UpdateInputHandler(dt);
      
      RecordInputGroup(&input_group);
      output.clear();
      input_group.WriteLines(&output);

      maxwidth = 0;
      for (unsigned int i = 0 ; i < output.size() ; ++i) {
         width = text_length(font , output[i].c_str());
         if (width > maxwidth) {maxwidth = width;}
      }
      height = line_spacing*output.size();
      tx = (dw - maxwidth)/2;
      ty = (dh - height)/2;
      
      buffer.Clear();
      
      for (unsigned int i = 0 ; i < output.size() ; ++i) {
         textout_ex(buffer , font , output[i].c_str() , tx , ty + i*line_spacing , textcol , -1);
      }
      
      buffer.DrawTo(screen , 0 , 0);
      
   }
   
   return 0;
}




