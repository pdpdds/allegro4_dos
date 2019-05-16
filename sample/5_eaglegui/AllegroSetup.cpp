/*
 *
 *     _______       ___       ____      __       _______ 
 *    /\  ____\    /|   \     /  __\    /\ \     /\  ____\ 
 *    \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_ 
 *     \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\ 
 *      \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_ 
 *       \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *        \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    EAGLE
 *    Edgar's Allegro Gui Library and Extensions
 *
 *    Copyright 2009-2011 by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */

#include "Eagle/Logging.hpp"
#include "Eagle/AllegroSetup.hpp"



using std::endl;


volatile bool close_program = false;

void CloseProgram() {
   close_program = true;
   OutputLog() << "Close Program callback called." << endl;
}
END_OF_FUNCTION(CloseProgram);



int AssertHandler(const char* msg) {
   OutputLog() << msg << endl;
   int zero = 0;
   int dividebyzero = 1/zero;
   return 0;
}



bool GetAllegroReady(bool use_keyboard , bool use_timer , bool use_mouse) {

   /// Register the assert handler so our debugger will catch the signal on a failed assert
   register_assert_handler(AssertHandler);
   
   /// Install Allegro
   if (allegro_init() != 0) {
      OutputLog() << "### Allegro initialization failed. Allegro reports : " << allegro_error << endl;
      return false;
   } else {
      OutputLog() << "$$$ Allegro initialization successful." << endl;
   }

#ifndef  _WIN32
   set_color_depth(8); //256 color mode, 8 bits
#endif // ! _WIN32


   /// Setup the close button callback function
   LOCK_FUNCTION(CloseProgram);
   if (set_close_button_callback(CloseProgram) != 0) {
      OutputLog() << "### Could not set close button callback function." << endl;
   } else {
      OutputLog() << "$$$ Close button callback successfully set." << endl;
   }


   /// Find the desktop resolution and color depth
   if (!CheckDesktopInfo()) {
      OutputLog() << "### CheckDesktopInfo failed to detect desktop resolution and/or color depth." << endl;
   } else {
      int dw = -1,dh = -1,dcd = -1;
      if (!RetrieveDesktopInfo(&dw , &dh , &dcd)) {
         OutputLog() << "### Desktop info checked sucessfully but could not be retrieved - unknown error." << dw << " X " << dh << " Y " << dcd << endl;
      } else {
         OutputLog() << "$$$ Desktop resolution and color depth determined to be " << dw << " X " << dh << " X " << dcd << endl;
      }
   }

   /// Keyboard setup
   if (use_keyboard) {
      if (install_keyboard() != 0) {
      OutputLog() << "### Allegro keyboard initialization failed. Allegro reports : " << allegro_error << endl;
      return false;
      } else {
         OutputLog() << "$$$ Allegro keyboard initialization successful." << endl;
      }
   }

   /// Timer setup
   if (use_timer || use_mouse) {
      if (!use_timer && use_mouse) {
         OutputLog() << "### Allegro mouse routines need the timer module installed, installing allegro timer." << endl;
      }
      if (install_timer() != 0) {
         OutputLog() << "### Allegro timer initialization failed. Allegro reports : " << allegro_error << endl;
         return false;
      } else {
         OutputLog() << "$$$ Allegro timer initialization successful." << endl;
      }
   }

   /// Mouse setup
   int num_mouse_buttons = 0;
   if (use_mouse) {
      num_mouse_buttons = install_mouse();
      if (num_mouse_buttons == -1) {
         OutputLog() << "### Allegro mouse initialization failed. Allegro reports : " << allegro_error << endl;
         return false;
      } else if (num_mouse_buttons == 0) {
         OutputLog() << "### Allegro mouse initialization already completed. No need to call it again." << endl;
      } else {
         OutputLog() << "$$$ Allegro mouse initialization successful. The mouse in use has " << num_mouse_buttons << " buttons." << endl;
      }
   }

   OutputLog() << endl;

   return true;
}




BITMAP* buffer_address = NULL;

// CheckDesktopInfo needs to be called before using set_gfx_desktop and after using allegro_init()
bool SetGfxToDesktop(BITMAP** allocationbuffer) {

   bool success = true;

   int dw = -1;
   int dh = -1;
   int dcd = -1;
   if (!RetrieveDesktopInfo(&dw , &dh , &dcd)) {
      dw = 800;
      dh = 600;
      dcd = 32;
      OutputLog() << "Could not retrieve desktop resolution and/or colordepth. Using default of 800 X 600 X 32." << endl;
   } else {
      OutputLog() << "Using desktop resolution and color depth of " << dw << " X " << dh << " X " << dcd << "." << endl;
   }

   set_color_depth(dcd);
   if (set_gfx_mode(GFX_AUTODETECT_FULLSCREEN , dw , dh , 0 , 0) != 0) {
      success = false;
      OutputLog() << "Could not set graphics mode (GFX_AUTODETECT_FULLSCREEN , " << dw << " , " << dh << " , 0 , 0)" << endl;
   } else {
      clear_to_color(screen , makeacol(0,0,0,0));
      OutputLog() << "Graphics mode set to (GFX_AUTODETECT_FULLSCREEN , " << dw << " , " << dh << " , 0 , 0)" << endl;
   }

   if (success) {
      if (allocationbuffer == NULL) {
         success = false;
         OutputLog() << "The address passed for buffer allocation is a NULL pointer. Buffer will not be allocated." << endl;
      } else {
         *allocationbuffer = create_bitmap(dw , dh);
         if (*allocationbuffer == NULL) {
            success = false;
            OutputLog() << "The buffer could not be allocated." << endl;
         } else {
            clear_to_color(*allocationbuffer , makeacol(0,0,0,0));
            buffer_address = *allocationbuffer;
            OutputLog() << "Buffer allocated successfully. Width = " << buffer_address->w << " , Height = " << buffer_address->h <<
                        " , Color Depth = " << bitmap_color_depth(buffer_address) << endl;
         }
      }
   }

   if (success) {
      OutputLog() << "Graphics mode set successfully and buffer ready." << endl;
   } else {
      OutputLog() << "Set Graphics to Desktop function failed." << endl;
   }

   return success;
}







/** Desktop Information */

bool dt_res_known = false;
bool dt_cd_known = false;
int dt_w = -1;
int dt_h = -1;
int dt_cd = -1;


bool DesktopResolutionKnown() {return dt_res_known;}
bool DesktopColorDepthKnown() {return dt_cd_known;}
bool DesktopInfoKnown()       {return (dt_res_known && dt_cd_known);}



bool CheckDesktopInfo() {
   if (DesktopInfoKnown()) 
   {
	   OutputLog() << "### DesktopInfoKnown" << endl;
	   return true;
   }
#ifdef _WIN32
   dt_cd = desktop_color_depth();
   dt_cd_known = (dt_cd != 0);

   dt_res_known = (get_desktop_resolution(&dt_w, &dt_h) == 0);
#else
   dt_cd = 8;
   dt_cd_known = (dt_cd != 0);
   dt_w = 320;
   dt_h = 200;
   dt_res_known = 2;
#endif // _WIN32   

   OutputLog() << "Checking " << dt_cd << " " << dt_cd_known << " " << dt_res_known << endl;

   return (dt_cd_known && dt_res_known);
}



bool RetrieveDesktopInfo(int* w , int* h , int* cd) {
   if (!DesktopInfoKnown()) {return false;}
   if (w) {*w = dt_w;}
   if (h) {*h = dt_h;}
   if (cd) {*cd = dt_cd;}
   return true;
}



int DW() {return dt_w;}
int DH() {return dt_h;}
int DCD() {return dt_cd;}




