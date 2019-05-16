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



#include "Eagle/GuiCode/GfxModeDialog.hpp"
#include "Eagle/StringWork.hpp"

using std::string;
using std::list;
using std::vector;



bool GfxMode::operator==(const GfxMode& gm) {
   return (mode == gm.mode);
}



string GfxMode::ToString() {
   return string(GraphicsCardName(mode));
}



bool GfxResolution::operator==(const GfxResolution& gr) {
   return ((w == gr.w) && (h == gr.h));
}



string GfxResolution::ToString() {
   return StringPrintF("%i X %i" , w , h);
}



bool GfxResolutionEx::operator==(const GfxResolutionEx& gre) {
   return ((w == gre.w) && (h == gre.h) && (cd == gre.cd));
}



bool GfxColorDepth::operator==(const GfxColorDepth& gcd) {
   return (cd == gcd.cd);
}



string GfxColorDepth::ToString() {
   return StringPrintF("%i" , cd);
}


#define GRE_ALL_BPP(w,h) \
   GfxResolutionEx(w,h,32),\
   GfxResolutionEx(w,h,24),\
   GfxResolutionEx(w,h,16),\
   GfxResolutionEx(w,h,15),\
   GfxResolutionEx(w,h,8)

const GfxResolutionEx default_res_list[12*5] = {
   GRE_ALL_BPP(1600,1200),
   GRE_ALL_BPP(1280,1024),
   GRE_ALL_BPP(1280,960),
   GRE_ALL_BPP(1280,800),
   GRE_ALL_BPP(1024,768),
   GRE_ALL_BPP(800,600),
   GRE_ALL_BPP(640,480),
   GRE_ALL_BPP(640,400),
   GRE_ALL_BPP(480,360),
   GRE_ALL_BPP(480,300),
   GRE_ALL_BPP(320,240),
   GRE_ALL_BPP(320,200)
};

const int RESLISTSIZE = 12*5;



list<GfxResolutionEx> extra_res_list;



void SortResExList(list<GfxResolutionEx>& gre_list) {
   
   if (gre_list.size() < 2) {return;}
   
   list<GfxResolutionEx> sort_list;
   
   GfxResolutionEx max;
   list<GfxResolutionEx>::iterator maxit;
   
   for (list<GfxResolutionEx>::iterator it = gre_list.begin() ; it != gre_list.end() ; ++it) {
      if (it == gre_list.begin()) {
         max = *it;
         maxit = it;
         continue;
      }
      GfxResolutionEx item = *it;
      if (item.Width() > max.Width()) {
         max = *it;
         maxit = it;
      } else if ((item.Width() == max.Width()) && (item.Height() > max.Height())) {
         max = *it;
         maxit = it;
      } else if ((item.Width() == max.Width()) && (item.Height() == max.Height()) && (item.ColorDepth() > max.ColorDepth())) {
         max = *it;
         maxit = it;
      }
      list<GfxResolutionEx>::iterator last = gre_list.end();
      --last;
      if (it == last) {
         sort_list.push_back(max);
         gre_list.erase(maxit);
         it == gre_list.begin();
         --it;
      }
   }
   gre_list = sort_list;
}



void RemoveDuplicates(list<GfxResolutionEx>& gre_list) {
   if (gre_list.size() < 2) {return;}
   
   for (list<GfxResolutionEx>::iterator it1 = gre_list.begin() ; it1 != gre_list.end() ; ++it1) {
      GfxResolutionEx current = *it1;
      for (list<GfxResolutionEx>::iterator it2 = it1 ; it2 != gre_list.end() ; ) {
         if (it2 == it1) {++it2;}
         GfxResolutionEx test = *it2;
         if (current == test) {
            it2 = gre_list.erase(it2);
         } else {
            ++it2;
         }
      }
   }
}


list<GfxModeData> GetAllGfxModeDataList() {
   
   list<GfxResolutionEx> total_res_list;
   
   for (int i = 0 ; i < RESLISTSIZE ; ++i) {
      total_res_list.push_back(default_res_list[i]);
   }
   for (list<GfxResolutionEx>::iterator it = extra_res_list.begin() ; it != extra_res_list.end() ; ++it) {
      total_res_list.push_back(*it);
   }
   
   RemoveDuplicates(total_res_list);
   SortResExList(total_res_list);
   
   list<GfxModeData> modelist;
   
   for (list<GfxResolutionEx>::iterator it = total_res_list.begin() ; it != total_res_list.end() ; ++it) {
      const GfxResolutionEx& item = *it;
      modelist.push_back(GfxModeData(GFX_AUTODETECT , item.Width() , item.Height() , item.ColorDepth()));
   }
   for (list<GfxResolutionEx>::iterator it = total_res_list.begin() ; it != total_res_list.end() ; ++it) {
      const GfxResolutionEx& item = *it;
      modelist.push_back(GfxModeData(GFX_AUTODETECT_FULLSCREEN , item.Width() , item.Height() , item.ColorDepth()));
   }
   for (list<GfxResolutionEx>::iterator it = total_res_list.begin() ; it != total_res_list.end() ; ++it) {
      const GfxResolutionEx& item = *it;
      modelist.push_back(GfxModeData(GFX_AUTODETECT_WINDOWED , item.Width() , item.Height() , item.ColorDepth()));
   }
   for (list<GfxResolutionEx>::iterator it = total_res_list.begin() ; it != total_res_list.end() ; ++it) {
      const GfxResolutionEx& item = *it;
      modelist.push_back(GfxModeData(GFX_SAFE , item.Width() , item.Height() , item.ColorDepth()));
   }
   
   ASSERT(system_driver);
   
   _DRIVER_INFO* info = _gfx_driver_list;
   if (system_driver->gfx_drivers) {
      info = system_driver->gfx_drivers();
   }
   
   int i = 0;
   while (info[i].driver) {
      GFX_DRIVER* gd = (GFX_DRIVER*)info[i].driver;
      int card = gd->id;
      GFX_MODE_LIST* new_mode_list = get_gfx_mode_list(card);
      if (new_mode_list) {
         GFX_MODE* m = new_mode_list->mode;
         for (int j = 0 ; j < new_mode_list->num_modes ; ++j) {
            modelist.push_back(GfxModeData(card , m[j].width , m[j].height , m[j].bpp));
         }
         destroy_gfx_mode_list(new_mode_list);
      } else {
         // use default mode list
         for (list<GfxResolutionEx>::iterator it = total_res_list.begin() ; it != total_res_list.end() ; ++it) {
            const GfxResolutionEx& item = *it;
            modelist.push_back(GfxModeData(card , item.Width() , item.Height() , item.ColorDepth()));
         }
         
      }
      ++i;
   }
   return modelist;
}



void FilterModeList(list<GfxModeData>& gmd_list , int (*filter)(int,int,int,int)) {
   if (!filter) {return;}
   if (gmd_list.empty()) {return;}
   
   list<GfxModeData> filter_list;
   
   for (list<GfxModeData>::iterator it = gmd_list.begin() ; it != gmd_list.end() ; ++it) {
      GfxModeData& item = *it;
      int ret = filter(item.Mode() , item.W() , item.H() , item.CD());
      if (ret == 0) {
         filter_list.push_back(item);
      }
   }
   
   gmd_list = filter_list;
}



list<GfxMode> GetGfxModes(const list<GfxModeData>& gmd_list) {
   list<GfxMode> mode_list;
   if (gmd_list.empty()) {return mode_list;}
   
   for (list<GfxModeData>::const_iterator cit = gmd_list.begin() ; cit != gmd_list.end() ; ++cit) {
      const GfxModeData& item = *cit;
      bool on_list = false;
      const int mode = item.Mode();
      for (list<GfxMode>::iterator it = mode_list.begin() ; it != mode_list.end() ; ++it) {
         if ((*it).Mode() == mode) {
            on_list = true;
            break;
         }
      }
      if (!on_list) {
         mode_list.push_back(GfxMode(mode));
      }
   }
   
   return mode_list;
}



list<GfxResolution> GetGfxResolutions(const list<GfxModeData>& gmd_list , int driver) {
   list<GfxResolution> res_list;
   if (gmd_list.empty()) {return res_list;}
   
   for (list<GfxModeData>::const_iterator cit = gmd_list.begin() ; cit != gmd_list.end() ; ++cit) {
      const GfxModeData& item = *cit;
      const int drv = item.Mode();
      if (drv == driver) {
         res_list.push_back(GfxResolution(item.W() , item.H()));
      }
   }
   
   return res_list;
}



list<GfxColorDepth> GetGfxColorDepths(const list<GfxModeData>& gmd_list , int driver , int width , int height) {
   list<GfxColorDepth> cd_list;
   if (gmd_list.empty()) {return cd_list;}
   
   for (list<GfxModeData>::const_iterator cit = gmd_list.begin() ; cit != gmd_list.end() ; ++cit) {
      const GfxModeData& item = *cit;
      if ((driver == item.Mode()) && (width == item.W()) && (height == item.H())) {
         cd_list.push_back(GfxColorDepth(item.CD()));
      }
   }
   
   return cd_list;
}



/// --------------------------------- GfxModeDialog class ------------------------------------



const unsigned int TOPIC_GFX_MODE_DIALOG = NextFreeTopicId();



void GfxModeDialog::ResetPositions() {
   ASSERT(area.W() >= AbsMinWidth());
   ASSERT(area.H() >= AbsMinHeight());
   
   const int textheight = text_height(textfont);
   const int x = area.X();
   const int y = area.Y();
   const int w = area.W();
   const int h = area.H();
   const int boxw = (w - 30)/2;
   ASSERT(boxw >= 58);// ListBox minimum width
   const int boxh = (h - (10 + 8*textheight))/2;
   ASSERT(boxh >= 58);// ListBox minimum height
   
   titletext.SetTextPos(x + w/2 , y + textheight/2);
   drivertext.SetTextPos(x + 10 , y + (5*textheight)/2);
   restext.SetTextPos(x + 10 + boxw + 10 , y + (5*textheight)/2);
   infotext.SetTextPos(x + 10 + boxw , y + 4*textheight + boxh + textheight/2);
   cdtext.SetTextPos(x + 10 , y + 4*textheight + boxh + textheight + textheight/2);
   modebox.SetArea(x + 10 , y + 4*textheight , boxw , boxh);
   resbox.SetArea(x + 10 + boxw + 10 , y + 4*textheight , boxw , boxh);
   cdbox.SetArea(x + 10 , y + 4*textheight + boxh + 4*textheight , boxw , boxh);
   
   const int btnx = 10 + boxw + 10;
   const int btny = 4*textheight + boxh;
   const int btnw = boxw;
   const int btnh = (h - (4*textheight + boxh))/5;
   
   okaybtn.SetArea(btnx , btny + btnh , btnw , btnh);
   cancelbtn.SetArea(btnx , btny + 3*btnh , btnw , btnh);
}



void GfxModeDialog::ResetInfoText() {
   if (modelist.size() == 0) {
      infotext.SetText("");
   } else {
//20190509
      /*int type = get_gfx_mode_type(selmode.Mode());
      if (!(type & GFX_TYPE_DEFINITE)) {
         infotext.SetText("Indefinite");
      } else if (type & GFX_TYPE_WINDOWED) {
         infotext.SetText("Windowed");
      } else if (type & GFX_TYPE_FULLSCREEN) {
         infotext.SetText("Fullscreen");
      }*/
   }
}



void GfxModeDialog::ResetModeList() {
   list<GfxMode> gmlist = GetGfxModes(modelist);
   modebox.SetList(gmlist);
   GfxMode selgm(selmode.Mode());
   if (modebox.OnList(selgm)) {
      modebox.TypedSelectItem(selgm);
   } else {
      modebox.SelectItem(0);
      selmode.SetMode((modebox.GetTypedSelection()).Mode());
   }
   
   ResetResolutionList();
}



void GfxModeDialog::ResetResolutionList() {
   list<GfxResolution> grlist = GetGfxResolutions(modelist , selmode.Mode());
   resbox.SetList(grlist);
   GfxResolution selgres(selmode.W() , selmode.H());
   if (resbox.OnList(selgres)) {
      resbox.TypedSelectItem(selgres);
   } else {
      resbox.SelectItem(0);
      GfxResolution res = resbox.GetTypedSelection();
      selmode.SetResolution(res.Width() , res.Height());
   }
   
   ResetColorDepthList();
}



void GfxModeDialog::ResetColorDepthList() {
   list<GfxColorDepth> gcdlist = GetGfxColorDepths(modelist , selmode.Mode() , selmode.W() , selmode.H());
   cdbox.SetList(gcdlist);
   GfxColorDepth selgcd(selmode.CD());
   if (cdbox.OnList(selgcd)) {
      cdbox.TypedSelectItem(selgcd);
   } else {
      cdbox.SelectItem(0);
      GfxColorDepth gcd = cdbox.GetTypedSelection();
      selmode.SetColorDepth(gcd.ColorDepth());
   }
}



GfxModeDialog::GfxModeDialog(string name , GfxModeData orig_mode , FONT* text_font , int (*filter) (int,int,int,int) ,
                              const Rectangle& position , UINT wflags) :
      WidgetBase(name , NULL , position , wflags),
      textfont(text_font),
      titletext(name + string("-titletext") , "Select a graphics mode" , text_font , TEXT_CENTER , TEXT_TOP , 0 , 0),
      drivertext(name + string ("-drivertext") , "Driver" , text_font , TEXT_LEFT , TEXT_TOP , 0 , 0),
      restext(name + string("-restext") , "Resolution" , text_font , TEXT_LEFT , TEXT_TOP , 0 , 0),
      infotext(name + string("-infotext") , "" , text_font , TEXT_RIGHT , TEXT_TOP , 0 , 0),// "Fullscreen" , "Windowed" , "Indefinite"
      cdtext(name + string("-cdtext") , "Color Depth" , text_font , TEXT_LEFT , TEXT_TOP , 0 , 0),
      modebox(name + string("-modebox") , text_font , false , Rectangle(0,0,58,58)),
      resbox(name + string("-resbox") , text_font , false , Rectangle(0,0,58,58)),
      cdbox(name + string("-cdbox") , text_font , false , Rectangle(0,0,58,58)),
      okaybtn(name + string("-okaybtn") , RECTANGLE_BTN , SPRING_BTN , text_font , "Okay" ,
               input_key_press(KEY_NONE) , Rectangle(0,0,0,0) , DEFAULT_FLAGS | ALLOW_CLOSE),
      cancelbtn(name + string("-cancelbtn") , RECTANGLE_BTN , SPRING_BTN , text_font , "Cancel" ,
               input_key_press(KEY_NONE) , Rectangle(0,0,0,0) , DEFAULT_FLAGS | ALLOW_CLOSE),
      modelist(),
      origmode(orig_mode),
      selmode(orig_mode),
      filter_function(0),
      wlist()
{
   wlist.push_back(&titletext);
   wlist.push_back(&drivertext);
   wlist.push_back(&restext);
   wlist.push_back(&infotext);
   wlist.push_back(&cdtext);
   wlist.push_back(&modebox);
   wlist.push_back(&resbox);
   wlist.push_back(&cdbox);
   wlist.push_back(&okaybtn);
   wlist.push_back(&cancelbtn);
   for (unsigned int i = 0 ; i < wlist.size() ; ++i) {
      WidgetBase* wb = wlist[i];
      wb->SetParent(this);
      wb->UseWidgetColorset(&wc);
   }
   SetMinWidth(AbsMinWidth());
   SetMinHeight(AbsMinHeight());
   if ((area.W() < minw) || (area.H() < minh)) {
      SetDrawDimensions((area.W() > minw)?area.W():minw , (area.H() > minh)?area.H():minh);
   }
   ResetPositions();
   SetFilter(filter);
}



WidgetMsg GfxModeDialog::Update (double tsec) {
   UINT retmsg = DIALOG_OKAY;
   WidgetMsg ret;
   
   for (UINT i = 0 ; i < wlist.size() ; ++i) {
      WidgetBase* wb = wlist[i];
      ret = wb->Update(tsec);
      retmsg |= ret.Messages();
      if (wb->Flags() & NEEDS_BG_REDRAW) {
         SetBgRedrawFlag();
      }
   }
   return WidgetMsg(this , TOPIC_DIALOG , retmsg);
}



WidgetMsg GfxModeDialog::CheckInputs(int msx , int msy) {
   UINT retmsg = DIALOG_OKAY;
   WidgetMsg ret;
   for (UINT i = 0 ; i < wlist.size() ; ++i) {
      WidgetBase* wb = wlist[i];
      ret = wb->CheckInputs(msx,msy);
      retmsg |= ret.Messages();
      if (wb->Flags() & NEEDS_BG_REDRAW) {
         SetBgRedrawFlag();
      }
   }
   return WidgetMsg(this , TOPIC_DIALOG , retmsg);
}



void GfxModeDialog::DisplayOn(BITMAP* bmp , int x , int y) {

   Rectangle r = area;
   r.MoveBy(x,y);
   Rectangle old_clip = GetClippingRectangle(bmp);

   r.SetClipRect(bmp);
///   r.Fill(bmp , (*wcols)[BGCOL]);// let the users background show through
   for (UINT i = 0 ; i < wlist.size() ; ++i) {
      WidgetBase* wb = wlist[i];
      wb->DisplayOn(bmp,x,y);
   }
   old_clip.SetClipRect(bmp);
}




bool GfxModeDialog::Init() {
   bool init = true;
   for (UINT i = 0 ; i < wlist.size() ; ++i) {
      WidgetBase* wb = wlist[i];
      if (!wb->Init()) {init = false;}
   }
   return init;
}



bool GfxModeDialog::Ready() {
   bool ready = true;
   for (UINT i = 0 ; i < wlist.size() ; ++i) {
      WidgetBase* wb = wlist[i];
      if (!wb->Ready()) {ready = false;}
   }
   return ready;
}



bool GfxModeDialog::Shutdown() {
   bool shutdown = true;
   for (UINT i = 0 ; i < wlist.size() ; ++i) {
      WidgetBase* wb = wlist[i];
      if (!wb->Shutdown()) {shutdown = false;}
   }
   return shutdown;
}



void GfxModeDialog::QueueUserMessage(const WidgetMsg& wmsg) {
   const WidgetMsg modemsg(&modebox , TOPIC_LISTBOX , LISTBOX_SELECTION_CHANGED);
   const WidgetMsg resmsg(&resbox , TOPIC_LISTBOX , LISTBOX_SELECTION_CHANGED);
   const WidgetMsg cdmsg(&cdbox , TOPIC_LISTBOX , LISTBOX_SELECTION_CHANGED);

   const WidgetMsg okaymsg(&okaybtn , TOPIC_BUTTON_WIDGET , BUTTON_CLICKED);
   const WidgetMsg cancelmsg(&cancelbtn , TOPIC_BUTTON_WIDGET , BUTTON_CLICKED);

   if (wmsg == modemsg) {
      int mode = (modebox.GetTypedSelection()).Mode();
      selmode.SetMode(mode);
      ResetInfoText();
      ResetResolutionList();
   } else if (wmsg == resmsg) {
      GfxResolution gres = resbox.GetTypedSelection();
      selmode.SetResolution(gres.Width() , gres.Height());
      ResetColorDepthList();
   } else if (wmsg == cdmsg) {
      int cd = (cdbox.GetTypedSelection()).ColorDepth();
      selmode.SetColorDepth(cd);
   } else if (wmsg == okaymsg) {
      QueueUserMessage(WidgetMsg(this , TOPIC_GFX_MODE_DIALOG , GFX_MODE_OKAYED));
   } else if (wmsg == cancelmsg) {
      QueueUserMessage(WidgetMsg(this , TOPIC_GFX_MODE_DIALOG , GFX_MODE_CANCELLED));
   } else {
      WidgetBase::QueueUserMessage(wmsg);
   }
}



void GfxModeDialog::SetWidgetColors(const WidgetColorset& colors) {
   WidgetBase::SetWidgetColors(colors);
   for (UINT i = 0 ; i < wlist.size() ; ++i) {
      WidgetBase* wb = wlist[i];
      wb->SetWidgetColors(colors);
   }
   SetRedrawFlag();
}



void GfxModeDialog::UseWidgetColorset(WidgetColorset* colorset) {
   WidgetBase::UseWidgetColorset(colorset);
   for (UINT i = 0 ; i < wlist.size() ; ++i) {
      WidgetBase* wb = wlist[i];
      wb->UseWidgetColorset(colorset);
   }
   SetRedrawFlag();
}



void GfxModeDialog::UseIndividualColorset() {
   WidgetBase::UseIndividualColorset();
   for (UINT i = 0 ; i < wlist.size() ; ++i) {
      WidgetBase* wb = wlist[i];
      wb->UseWidgetColorset(&wc);
   }
   SetRedrawFlag();
}



int GfxModeDialog::AbsMinWidth() {return AbsMinWidth(textfont);}



int GfxModeDialog::AbsMinHeight() {return AbsMinHeight(textfont);}



int GfxModeDialog::AbsMinWidth(const FONT* text_font) {
   int minwidth = 2*11*(text_length(text_font , " ")) + 3*10;// 206 with default 8x8 font
   const int absmin = 2*58 + 3*10;// 146 
   if (minwidth < absmin) {minwidth = absmin;}
   return minwidth;
}



int GfxModeDialog::AbsMinHeight(const FONT* text_font) {
   int minheight = (8*text_height(text_font)) + (2*58) + 10;// 190 with default 8x8 font
   return minheight;
}



void GfxModeDialog::SetFilter(int (*filter) (int,int,int,int)) {
   filter_function = filter;
   modelist = GetAllGfxModeDataList();
   FilterModeList(modelist , filter_function);
   modebox.ClearList();
   resbox.ClearList();
   cdbox.ClearList();
   selmode = origmode;

   if (modelist.size()) {
      ResetModeList();
   } else {
      ResetInfoText();
   }
}



GfxModeData GfxModeDialog::SelectedMode() {
   return selmode;
}








