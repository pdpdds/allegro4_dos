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


#ifndef GfxModeHandler_H
#define GfxModeHandler_H



#include <iostream>
#include <string>

#include <allegro.h>

enum CHANGE_RES_MODE {
   CRM_DIRECT                   = 1,  /// Change directly to the new mode
   CRM_GFX_TEXT                 = 2,  /// Set GFX_TEXT mode before changing to new mode
   CRM_GFX_TEXT_ALLEGRO_MESSAGE = 3   /// Set GFX_TEXT and use allegro_message before changing to new mode
};

const char* ChangeResModeText(CHANGE_RES_MODE crm);



/** Graphics mode utility functions */

bool ChangeRes(int card , int width , int height , int color_depth ,
               CHANGE_RES_MODE change_res_mode = CRM_GFX_TEXT_ALLEGRO_MESSAGE);

void LogGfxAbilitiesTo(std::ostream& text_log);

const char* SwitchModeText(int switch_mode);



/** Simple structure to hold graphics mode data */

class GfxModeData {

private :
   int mode,w,h,cd;

public :
	//20190509
    //GfxModeData() : mode(GFX_TYPE_UNKNOWN) , w(0) , h(0) , cd(0) 
	GfxModeData() : mode(GFX_TEXT), w(0), h(0), cd(0)
      {}
   
   GfxModeData(int gfx_mode , int width , int height , int color_depth) :
               mode(gfx_mode) , w(width) , h(height) , cd(color_depth)
      {}
   
   bool operator==(const GfxModeData& g) {
      return ((mode == g.mode) && (w == g.w) && (h == g.h) && (cd == g.cd));
   }
   
   void SetMode(int newmode) {mode = newmode;}
   void SetWidth(int width) {w = width;}
   void SetHeight(int height) {h = height;}
   void SetResolution(int width , int height) {w = width;h = height;}
   void SetColorDepth(int color_depth) {cd = color_depth;}
   
   bool Valid();// Allegro needs to be initialized first

   inline int Mode() const {return mode;}
   inline int W()    const {return w;}
   inline int H()    const {return h;}
   inline int CD()   const {return cd;}
   
   friend std::ostream& operator<<(std::ostream& os , const GfxModeData& gmd);
};


inline bool ChangeRes(const GfxModeData& gfxmode , CHANGE_RES_MODE change_res_mode = CRM_GFX_TEXT_ALLEGRO_MESSAGE) {
   return ChangeRes(gfxmode.Mode() , gfxmode.W() , gfxmode.H() , gfxmode.CD() , change_res_mode);
}



typedef void (*SWITCH_CB_FUNC) ();

// Global callbacks set by the GfxModeHandler function.
// These call GfxModeHandler::GainFocusBase() and LoseFocusBase() respectively.
// The user callbacks set in the GfxModeHandler will be called from there.

void GainFocus();
void LoseFocus();

/** ChangeToMode will attempt to set the non amnesia mode first and fallback to the amnesia mode if necessary. */

enum SWITCH_PREFERENCE {
   SW_NONE       = 0 ,
   SW_PAUSE      = 1 ,
   SW_BACKGROUND = 2
};

/** Return value flags for GfxModeHandler::ChangeToMode */
enum CHANGE_MODE_FLAGS {
   FULL_SUCCESS              = 0x00,
   NEW_MODE_FAILURE          = 0x01,
   REVERT_MODE_FAILURE       = 0x02,
   SWITCH_MODE_FAILURE       = 0x04,
   SWITCH_CALLBACK_FAILURE   = 0x08
};

std::string ChangeModeFlagText(int change_mode_flags);

/** Graphics mode handler class */

/** There is a single external GfxModeHandler, do not create other instances of this class, they won't work right */

class GfxModeHandler {
   
private :
   GfxModeData gfxmode;
   SWITCH_PREFERENCE sw_pref;
   int sw_mode;
   
   bool amnesia;
   bool pause;
   
   // User callbacks
   SWITCH_CB_FUNC cb_gain_focus;
   SWITCH_CB_FUNC cb_lose_focus;
   

public :

   void GainFocusBase();// do not call directly
   void LoseFocusBase();// do not call directly

   GfxModeHandler();
   
   void Setup(SWITCH_PREFERENCE pref , SWITCH_CB_FUNC gain_focus , SWITCH_CB_FUNC lose_focus);
   
   int ChangeToMode(const GfxModeData& newmode , CHANGE_RES_MODE change_res_mode = CRM_GFX_TEXT_ALLEGRO_MESSAGE);
   
   /// Getters
   const GfxModeData CurrentGraphicsModeData() {return gfxmode;}
   const SWITCH_PREFERENCE SwitchPreference() {return sw_pref;}
   const int SwitchMode() {return sw_mode;}
   const bool AmnesiaMode() {return amnesia;}
   const bool PauseMode() {return pause;}
};

extern GfxModeHandler gfx_mode_handler;





#endif // GfxModeHandler_H





