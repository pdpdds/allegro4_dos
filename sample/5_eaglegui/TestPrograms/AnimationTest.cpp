

#include "Eagle.hpp"

using std::endl;



int AnimationTestMain(int argc , char** argv) {
   
   OutputLog() << "Animation test main started." << endl;
   if (!SendOutputToFile("AnimationTestMain_Log.txt" , "Animation Test\n\n" , false)) {return 0;}
   
   if (!GetAllegroReady(true , true , true)) {return 0;}
   if (!DesktopInfoKnown()) {return 0;}
   SetupInputHandler();

   const int dw = 800;
   const int dh = 600;
   GfxModeData gfxmode(GFX_AUTODETECT_WINDOWED , dw , dh , DCD());

   gfx_mode_handler.Setup(SW_BACKGROUND , 0 , 0);
   int ret = gfx_mode_handler.ChangeToMode(gfxmode , CRM_DIRECT);
   if (ret != FULL_SUCCESS) {
      OutputLog() << "Couldn't change to requested graphics mode." << endl;
      return 0;
   }
   
   set_window_title("Animation Test");

   bool show_instructions = true;
   int text_color = makecol(255,255,255);
// 28*8 = 160 + 64 = 224
   while (!input_key_press(KEY_ESC) && !close_program) {
      if (input_key_press(KEY_1)) {
         RunEagleCredits1();
         show_instructions = true;
      }
      if (input_key_press(KEY_2)) {
         RunEagleCredits2();
         show_instructions = true;
      }
      if (input_key_press(KEY_3)) {
         RunEagleCredits3();
         show_instructions = true;
      }
      if (show_instructions) {
         clear(screen);
         textout_ex(screen , font , "Press 1 for Eagle Credits #1" , SCREEN_W/32 , SCREEN_H/2 - 20 , text_color , -1);
         textout_ex(screen , font , "Press 2 for Eagle Credits #2" , SCREEN_W/32 , SCREEN_H/2 - 4 , text_color , -1);
         textout_ex(screen , font , "Press 3 for Eagle Credits #3" , SCREEN_W/32 , SCREEN_H/2 + 12 , text_color , -1);
         show_instructions = false;
      }
      rest(16);
      UpdateInputHandler(0.016);
   }
   OutputLog() << "Animation test complete. Exiting." << std::endl;
   
   allegro_exit();
   
   return 0;
}


