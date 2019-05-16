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
#include "Eagle/GfxModeHandler.hpp"
#include "Eagle/AllegroUtilities.hpp"
#include "Eagle/timer_handler.h"
#include "Eagle/StringWork.hpp"

#include <string>
using std::string;

using std::ostream;
using std::endl;


const char* ChangeResModeText(CHANGE_RES_MODE crm) {
   const char* text = "";
   switch (crm) {
      case CRM_DIRECT : text = "CRM_DIRECT";break;
      case CRM_GFX_TEXT : text = "CRM_GFX_TEXT";break;
      case CRM_GFX_TEXT_ALLEGRO_MESSAGE : text = "CRM_GFX_TEXT_ALLEGRO_MESSAGE";break;
   }
   return text;
}



bool ChangeRes(int card , int width , int height , int color_depth , CHANGE_RES_MODE change_res_mode) {

   ASSERT(card != GFX_TEXT);
   ASSERT(ColorDepthIndex(color_depth) != NUMCOLORDEPTHS);
   
   /// ##########################################################
   /// ##########  Function parameter options
   // Change_res_mode == 1 : Use only step 3 below.
   //                         (just set new gfx mode) (don't use GFX_TEXT or allegro_message)
   //
   // Change_res_mode == 2 : Use steps 1 and 3 only. 
   //                         (set GFX_TEXT , then set new gfx mode) (don't use allegro_message)
   //
   // Change_res_mode == 3 : Use steps 1 , 2 , and 3 below. 
   //                         (set GFX_TEXT , use allegro_message , and set new gfx mode)
   //
   // Steps used :
   // 1. set GFX_TEXT mode
   // 2. use allegro_message
   // 3. set new graphics mode
   /// ##########################################################


   /// Change graphics card value into readable characters
   const char* g_card = GraphicsCardName(card);

   OutputLog() << endl;
   OutputLog() << StringPrintF("Starting change_res(%d (%s) , %d , %d , %d , mode = %s)" ,
                               card , g_card , width , height , color_depth , ChangeResModeText(change_res_mode)) << endl;

   bool use_set_gfx_text    = (change_res_mode != 1);// Only used in mode 2 and 3
   bool use_allegro_message = (change_res_mode == 3);// Only used in mode 3

   /// Set graphics mode GFX_TEXT first
   if (use_set_gfx_text) {
      if (set_gfx_mode(GFX_TEXT , 0 , 0 , 0 , 0) != 0) {
         OutputLog() << "  Failed to set graphics mode GFX_TEXT" << endl;
         OutputLog() << "    Allegro reports : " << allegro_error << endl;
         return false;
      }
   }

   /// Use allegro_message to display a message (changing to new resolution such and such).
   if (use_allegro_message) {
      allegro_message("  Changing to %d X %d X %d in %s mode" , width , height , color_depth , g_card);
   }

   /// Change to new resolution
   set_color_depth(color_depth);

   if (set_gfx_mode(card , width , height , 0 , 0) == 0) {
      OutputLog() << "    Success changing to new resolution" << endl;
      al_trace("\n");
   } else {
      OutputLog() << "    Failed to set " << width << " X " << height << " X " << color_depth;
      OutputLog() << " using " << g_card << " graphics mode" << endl;
      OutputLog() << "      Allegro reports this error : " << allegro_error << endl;
      if (set_gfx_mode(GFX_TEXT , 0 , 0 , 0 , 0) == 0) {
         allegro_message(allegro_error);
      }
      return false;
   }

   return true;
}



void LogGfxAbilitiesTo(ostream& text_log) {
	//20190509
   /*const int gc = gfx_capabilities;
   const char* gcardname = GraphicsCardName(get_gfx_mode());
   text_log << "Graphics capabilities for the current driver (" << gcardname << ")" << endl;
   if (get_gfx_mode() != GFX_NONE) {
      text_log << "(" << ((gc & GFX_CAN_SCROLL)?("Y"):("N")) << ") GFX_CAN_SCROLL" << endl;
      text_log << "(" << ((gc & GFX_CAN_TRIPLE_BUFFER)?("Y"):("N")) << ") GFX_CAN_TRIPLE_BUFFER" << endl;
      text_log << "(" << ((gc & GFX_HW_CURSOR)?("Y"):("N")) << ") GFX_HW_CURSOR" << endl;
      text_log << "(" << ((gc & GFX_SYSTEM_CURSOR)?("Y"):("N")) << ") GFX_SYSTEM_CURSOR" << endl;
      text_log << "(" << ((gc & GFX_HW_HLINE)?("Y"):("N")) << ") GFX_HW_HLINE" << endl;
      text_log << "(" << ((gc & GFX_HW_HLINE_XOR)?("Y"):("N")) << ") GFX_HW_HLINE_XOR" << endl;
      text_log << "(" << ((gc & GFX_HW_HLINE_SOLID_PATTERN)?("Y"):("N")) << ") GFX_HW_HLINE_SOLID_PATTERN" << endl;
      text_log << "(" << ((gc & GFX_HW_HLINE_COPY_PATTERN)?("Y"):("N")) << ") GFX_HW_HLINE_COPY_PATTERN" << endl;
      text_log << "(" << ((gc & GFX_HW_FILL)?("Y"):("N")) << ") GFX_HW_FILL" << endl;
      text_log << "(" << ((gc & GFX_HW_FILL_XOR)?("Y"):("N")) << ") GFX_HW_FILL_XOR" << endl;
      text_log << "(" << ((gc & GFX_HW_FILL_SOLID_PATTERN)?("Y"):("N")) << ") GFX_HW_FILL_SOLID_PATTERN" << endl;
      text_log << "(" << ((gc & GFX_HW_FILL_COPY_PATTERN)?("Y"):("N")) << ") GFX_HW_FILL_COPY_PATTERN" << endl;
      text_log << "(" << ((gc & GFX_HW_LINE)?("Y"):("N")) << ") GFX_HW_LINE" << endl;
      text_log << "(" << ((gc & GFX_HW_LINE_XOR)?("Y"):("N")) << ") GFX_HW_LINE_XOR" << endl;
      text_log << "(" << ((gc & GFX_HW_TRIANGLE)?("Y"):("N")) << ") GFX_HW_TRIANGLE" << endl;
      text_log << "(" << ((gc & GFX_HW_TRIANGLE_XOR)?("Y"):("N")) << ") GFX_HW_TRIANGLE_XOR" << endl;
      text_log << "(" << ((gc & GFX_HW_GLYPH)?("Y"):("N")) << ") GFX_HW_GLYPH" << endl;
      text_log << "(" << ((gc & GFX_HW_VRAM_BLIT)?("Y"):("N")) << ") GFX_HW_VRAM_BLIT" << endl;
      text_log << "(" << ((gc & GFX_HW_VRAM_BLIT_MASKED)?("Y"):("N")) << ") GFX_HW_VRAM_BLIT_MASKED" << endl;
      text_log << "(" << ((gc & GFX_HW_MEM_BLIT)?("Y"):("N")) << ") GFX_HW_MEM_BLIT" << endl;
      text_log << "(" << ((gc & GFX_HW_MEM_BLIT_MASKED)?("Y"):("N")) << ") GFX_HW_MEM_BLIT_MASKED" << endl;
      text_log << "(" << ((gc & GFX_HW_SYS_TO_VRAM_BLIT)?("Y"):("N")) << ") GFX_HW_SYS_TO_VRAM_BLIT" << endl;
      text_log << "(" << ((gc & GFX_HW_SYS_TO_VRAM_BLIT_MASKED)?("Y"):("N")) << ") GFX_HW_SYS_TO_VRAM_BLIT_MASKED" << endl;
      text_log << "(" << ((gc & GFX_HW_VRAM_STRETCH_BLIT)?("Y"):("N")) << ") GFX_HW_VRAM_STRETCH_BLIT" << endl;
      text_log << "(" << ((gc & GFX_HW_SYS_STRETCH_BLIT)?("Y"):("N")) << ") GFX_HW_SYS_STRETCH_BLIT" << endl;
      text_log << "(" << ((gc & GFX_HW_VRAM_STRETCH_BLIT_MASKED)?("Y"):("N")) << ") GFX_HW_VRAM_STRETCH_BLIT_MASKED" << endl;
      text_log << "(" << ((gc & GFX_HW_SYS_STRETCH_BLIT_MASKED)?("Y"):("N")) << ") GFX_HW_SYS_STRETCH_BLIT_MASKED" << endl;
   }*/
}



const char* SwitchModeText(int switch_mode) {
   switch (switch_mode) {
      case SWITCH_NONE        : return "SWITCH_NONE";
      case SWITCH_PAUSE       : return "SWITCH_PAUSE";
      case SWITCH_AMNESIA     : return "SWITCH_AMNESIA";
      case SWITCH_BACKGROUND  : return "SWITCH_BACKGROUND";
      case SWITCH_BACKAMNESIA : return "SWITCH_BACKAMNESIA";
   }
   return "SWITCH_UNKNOWN";
}



/** GfxModeData methods */

bool GfxModeData::Valid() {
	//20190509
   //if (get_gfx_mode_type(mode) == GFX_TYPE_UNKNOWN) {return false;}
   if ((w < 1) || (h < 1)) {return false;}
   if (ColorDepthIndex(cd) == NUMCOLORDEPTHS) {return false;}
   return true;
}



ostream& operator<<(ostream& os , const GfxModeData& gmd) {
   os << "Graphics Mode : " << GraphicsCardName(gmd.mode) << " at ";
   os << gmd.w << " X " << gmd.h << " in " << gmd.cd << " bit color depth.";
   return os;
}



/** External GfxModeHandler */

GfxModeHandler gfx_mode_handler;


/** Callback functions used by gfx_mode_handler */

void GainFocus() {
   gfx_mode_handler.GainFocusBase();
}



void LoseFocus() {
   gfx_mode_handler.LoseFocusBase();
}



string ChangeModeFlagText(int change_mode_flags) {
   int cmf = change_mode_flags;
   if (cmf == FULL_SUCCESS) {return string("FULL_SUCCESS");}
   return StringPrintF("-%s-%s-%s-%s-" ,
                     ((cmf & NEW_MODE_FAILURE)?"NEW_MODE_FAILURE":""),
                     ((cmf & REVERT_MODE_FAILURE)?"REVERT_MODE_FAILURE":""),
                     ((cmf & SWITCH_MODE_FAILURE)?"SWITCH_MODE_FAILURE":""),
                     ((cmf & SWITCH_CALLBACK_FAILURE)?"SWITCH_CALLBACK_FAILURE":""));
}



/** GfxModeHandler methods */


void GfxModeHandler::GainFocusBase() {
   /** TODO : Redo logging */
   OutputLog() << "GfxModeHandler : Program regained window focus." << endl;
   if (pause) {
      ToggleTimerPause();
   }
   if (cb_gain_focus) {
      cb_gain_focus();
   }
}



void GfxModeHandler::LoseFocusBase() {
   /** TODO : Redo logging */
   OutputLog() << "GfxModeHandler : Program lost window focus." << endl;
   if (pause) {
      ToggleTimerPause();
   }
   if (cb_lose_focus) {
      cb_lose_focus();
   }
}



GfxModeHandler::GfxModeHandler() :
         gfxmode() ,
         sw_pref(SW_PAUSE) ,
         sw_mode(SWITCH_PAUSE) ,
         amnesia(false) ,
         pause(true) ,
         cb_gain_focus(0) ,
         cb_lose_focus(0)
         
{}



void GfxModeHandler::Setup(SWITCH_PREFERENCE pref , SWITCH_CB_FUNC gain_focus , SWITCH_CB_FUNC lose_focus) {
   sw_pref = pref;
   cb_gain_focus = gain_focus;
   cb_lose_focus = lose_focus;
}



int GfxModeHandler::ChangeToMode(const GfxModeData& newmode , CHANGE_RES_MODE change_res_mode) {
   if (gfxmode == newmode) {return FULL_SUCCESS;}

   OutputLog() << "Gfx_mode_handler attempting new mode..." << endl;
   
   /** Return values :
      See the CHANGE_MODE_FLAGS enum above. If any of the steps failed, a flag to indicate it will be
      set in the return value. If the new mode can't be set, an attempt to revert to the last successful
      mode will be made.
   */
   
   int retflags = FULL_SUCCESS;
   

   if (!ChangeRes(newmode , change_res_mode)) {/// Couldn't change to new mode
      retflags |= NEW_MODE_FAILURE;
      OutputLog() << "GfxModeHandler : Failed to set new mode." << endl;
      if (set_gfx_mode(GFX_TEXT , 0 , 0 , 0 , 0) == 0) {
         allegro_message("Failed to set %i X %i using %s." , newmode.W() , newmode.H() , GraphicsCardName(newmode.Mode()));
      }
      // Try reverting to previous mode if valid
      if (gfxmode.Valid()) {
         OutputLog() << "GfxModeHandler : Old mode valid, attempting to revert to old mode..." << endl;
         if (!ChangeRes(gfxmode , change_res_mode)) {/// Couldn't revert
            OutputLog() << "GfxModeHandler : Failed to revert to old mode." << endl;
            retflags |= REVERT_MODE_FAILURE;
            OutputLog() << "Change Mode Flags : " << ChangeModeFlagText(retflags) << endl;
            return retflags;
         } else {
            OutputLog() << "GfxModeHandler : Successfully reverted to previous mode." << endl;
         }
      } else {
         OutputLog() << "GfxModeHandler : Old mode is invalid, cannot revert." << endl;
         retflags |= REVERT_MODE_FAILURE;
         OutputLog() << "Change Mode Flags : " << ChangeModeFlagText(retflags) << endl;
         return retflags;
      }
   }
   
   /// Reset the display switch mode
   int ret = 0;// Zero on success and -1 if not currently possible
   switch (sw_pref) {
      case SW_NONE :
         ret = set_display_switch_mode(SWITCH_NONE);
         break;
      case SW_PAUSE :
         ret = set_display_switch_mode(SWITCH_PAUSE);
         if (ret != 0) {
            ret = set_display_switch_mode(SWITCH_AMNESIA);
         }
         break;
      case SW_BACKGROUND :
         ret = set_display_switch_mode(SWITCH_BACKGROUND);
         if (ret != 0) {
            ret = set_display_switch_mode(SWITCH_BACKAMNESIA);
         }
         break;
   }
   if (ret != 0) {
      retflags |= SWITCH_MODE_FAILURE;
      OutputLog() << "GfxModeHandler : Failed to set the ";
   } else {
      OutputLog() << "GfxModeHandler : Successfully set the ";
   }
   switch (sw_pref) {
      case SW_NONE : OutputLog() << "SW_NONE";break;
      case SW_PAUSE : OutputLog() << "SW_PAUSE";break;
      case SW_BACKGROUND : OutputLog() << "SW_BACKGROUND";break;
   }
   OutputLog() << " display switch mode." << endl;
   sw_mode = get_display_switch_mode();
   amnesia = ((sw_mode == SWITCH_AMNESIA) || (sw_mode == SWITCH_BACKAMNESIA));
   pause = ((sw_mode == SWITCH_PAUSE) || (sw_mode == SWITCH_AMNESIA));
   OutputLog() << "GfxModeHandler : Current display switch mode is " << SwitchModeText(sw_mode) << endl;
   
   /// Reset all the callbacks
   OutputLog() << "GfxModeHandler : ";
   if (set_display_switch_callback(SWITCH_IN , GainFocus) == 0) {
      OutputLog() << "Successfully set the ";
   } else {
      retflags |= SWITCH_CALLBACK_FAILURE;
      OutputLog() << "Failed to set the ";
   }
   OutputLog() << "display callback in." << endl;
   
   OutputLog() << "GfxModeHandler : ";
   if (set_display_switch_callback(SWITCH_OUT , LoseFocus) == 0) {
      OutputLog() << "Successfully set the ";
   } else {
      retflags |= SWITCH_CALLBACK_FAILURE;
      OutputLog() << "Failed to set the ";
   }
   OutputLog() << "display callback out." << endl;
   
   
   if (!(retflags & NEW_MODE_FAILURE)) {
      gfxmode = newmode;
   }
   
   OutputLog() << "Change Mode Flags : " << ChangeModeFlagText(retflags) << endl;
   return retflags;
}









