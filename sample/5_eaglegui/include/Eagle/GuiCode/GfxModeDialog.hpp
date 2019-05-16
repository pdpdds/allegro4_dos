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


#ifndef GfxModeDialog_H
#define GfxModeDialog_H


#include "Eagle/GfxModeHandler.hpp"
#include "Eagle/AllegroUtilities.hpp"
#include "Eagle/GuiCode/WidgetBase.hpp"
#include "Eagle/GuiCode/TextWidget.hpp"
#include "Eagle/GuiCode/ListBox.hpp"

#include <string>
#include <list>
#include <vector>



extern const unsigned int TOPIC_GFX_MODE_DIALOG;

enum GFX_DIALOG_MSGS {
   GFX_MODE_OKAYED = 0,
   GFX_MODE_CANCELLED = 1
};



class GfxMode {
private :
   int mode;
public :
   GfxMode() : mode(0) {}
   GfxMode(int gfx_mode) : mode(gfx_mode) {}
   
   bool operator==(const GfxMode& gm);
   
   void Set(int gfx_mode) {mode = gfx_mode;}
   
   std::string ToString();
   
   int Mode() {return mode;}
};



class GfxResolution {
private :
   int w,h;
public :
   GfxResolution() : w(0) , h(0) {}
   GfxResolution(int width , int height) : w(width) , h(height) {}
   
   bool operator==(const GfxResolution& gr);
   
   void Set(int width , int height) {w = width;h = height;}
   
   std::string ToString();

   int Width() const {return w;}
   int Height() const {return h;}
};



class GfxResolutionEx {
private :
   int w,h,cd;
public :
   GfxResolutionEx() : w(0) , h(0) , cd(0) {}
   GfxResolutionEx(int width , int height , int color_depth) : w(width) , h(height) , cd(color_depth) {}
   
   bool operator==(const GfxResolutionEx& gre);
   
   void Set(int width , int height , int color_depth) {w = width;h = height;cd = color_depth;}
   
   int Width() const {return w;}
   int Height() const {return h;}
   int ColorDepth() const {return cd;}
};



class GfxColorDepth {
private :
   int cd;
public :
   GfxColorDepth() : cd(0) {}
   GfxColorDepth(int color_depth) : cd(color_depth) {}
   
   bool operator==(const GfxColorDepth& gcd);
   
   void Set(int color_depth) {cd = color_depth;}
   
   std::string ToString();
   
   int ColorDepth() const {return cd;}
};



extern std::list<GfxResolutionEx> extra_res_list;

void SortResExList(std::list<GfxResolutionEx>& gre_list);

void RemoveDuplicates(std::list<GfxResolutionEx>& gre_list);

std::list<GfxModeData> GetAllGfxModeDataList();

void FilterModeList(std::list<GfxModeData>& gmd_list , int (*filter)(int,int,int,int));

std::list<GfxMode> GetGfxModes(const std::list<GfxModeData>& gmd_list);

std::list<GfxResolution> GetGfxResolutions(const std::list<GfxModeData>& gmd_list , int driver);

std::list<GfxColorDepth> GetGfxColorDepths(const std::list<GfxModeData>& gmd_list , int driver , int width , int height);



class GfxModeDialog : public WidgetBase {
private :
   FONT* textfont;

   TextWidget titletext;
   TextWidget drivertext;
   TextWidget restext;
   TextWidget infotext;
   TextWidget cdtext;
   
   TypedListBox<GfxMode> modebox;
   TypedListBox<GfxResolution> resbox;
   TypedListBox<GfxColorDepth> cdbox;
   
   Button okaybtn;
   Button cancelbtn;
   
   std::list<GfxModeData> modelist;
   
   GfxModeData origmode;
   GfxModeData selmode;
   
   int (*filter_function) (int,int,int,int);
   
   std::vector<WidgetBase*> wlist;
   
   
   void ResetPositions();
   
   void ResetInfoText();
   
   void ResetModeList();
   void ResetResolutionList();
   void ResetColorDepthList();

public :
   /// Minimum width of 2*(11*text_length(text_font," ")) + 3*10 or 146 whichever is greater and a
   /// minimum height of 8*text_height(text_font) + 2*58 + 10
   /// Minimum size of 216 X 190 with default allegro font
   GfxModeDialog(std::string name , GfxModeData orig_mode , FONT* text_font , int (*filter) (int,int,int,int) ,
                  const Rectangle& position , UINT wflags = DEFAULT_FLAGS);
   
   /// Overloaded functions from WidgetBase
   virtual WidgetMsg Update (double tsec);
   virtual WidgetMsg CheckInputs(int msx , int msy);// may send DIALOG_CLOSE
   virtual void DisplayOn(BITMAP* bmp , int x , int y);

   virtual bool Init();
   virtual bool Ready();
   virtual bool Shutdown();
   
   virtual void QueueUserMessage(const WidgetMsg& wmsg);

   virtual void SetWidgetColors(const WidgetColorset& colors);
   virtual void UseWidgetColorset(WidgetColorset* colorset);
   virtual void UseIndividualColorset();
   
   virtual int AbsMinWidth();
   virtual int AbsMinHeight();
   
   int AbsMinWidth(const FONT* text_font);
   int AbsMinHeight(const FONT* text_font);

   void SetFilter(int (*filter) (int,int,int,int));
   
   GfxModeData SelectedMode();
};


#endif // GfxModeDialog_H

