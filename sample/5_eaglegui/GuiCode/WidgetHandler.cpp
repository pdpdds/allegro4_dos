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



#include "Eagle/GuiCode/WidgetHandler.hpp"
#include "Eagle/GuiCode/TabGroup.hpp"
#include "Eagle/GuiCode/MenuWidget.hpp"
#include "Eagle/Logging.hpp"

#include <algorithm>

using std::list;
using std::vector;
using std::map;
using std::string;
using std::endl;
using std::stable_sort;

typedef vector<WidgetBase*> WIDGETLIST;
typedef WIDGETLIST::iterator WLIT;

typedef list<WidgetMsg> MESSAGEQUEUE;
typedef MESSAGEQUEUE::iterator MQIT;

typedef map<WidgetBase* , bool> MEMTRACKER;
typedef MEMTRACKER::iterator MTIT;



/** Used by WidgetHandler::TakeNextMessage to signify an empty message queue */
const WidgetMsg NoMessages((WidgetBase*)0 , TOPIC_DIALOG , DIALOG_OKAY);



const unsigned int TOPIC_GUI = NextFreeTopicId();



bool WidgetHandler::OwnsWidget(WidgetBase* widget) {
   for (UINT i = 0 ; i < wlist.size() ; ++i) {
      if (wlist[i] == widget) {return true;}
   }
   return false;
}



UINT WidgetHandler::WidgetIndex(WidgetBase* widget) {
   for (UINT index = 0 ; index < wlist.size() ; ++index) {
      WidgetBase* w = wlist[index];
      WidgetHandler* wh = w->GetGui();
      
      if (w == widget) {return index;}
      if (wh && wh->HasWidget(widget)) {return index;}
   }
   return wlist.size();
}


WLIT WidgetHandler::InputListIterator(WidgetBase* widget) {
   for (WLIT it = inputlist.begin() ; it != inputlist.end() ; ++it) {
      if (*it == widget) {return it;}
   }
   return inputlist.end();
}



void WidgetHandler::ResetWidgetDrawingTargets() {
   for (UINT i = 0 ; i < wlist.size() ; ++i) {
      WidgetBase* w = wlist[i];
      w->SetDrawTarget(buffer);
   }
}



void WidgetHandler::RedrawRoundedBackground() {
      ASSERT((BITMAP*)background);
      background.ClearTo(bitmap_mask_color(background));
      Rectangle r(0,0,background.W(),background.H());
      r.RoundedFill(background , rounded_radius , (*wcols)[BGCOL]);
      r.RoundedOutline(background , rounded_radius , (*wcols)[FGCOL]);
      r.Shrink(1);
      r.RoundedOutline(background , rounded_radius - 1 , (*wcols)[FGCOL]);
      clear_background = true;
      SetBgRedrawFlag();
}



void WidgetHandler::CheckRedraw(UINT widget_index) {
   if (widget_index >= drawlist.size()) {return;}
   WidgetBase* widget = drawlist[widget_index];
   UINT widgetflags = widget->Flags();
   Rectangle widgetarea = widget->Area();

   /// For widgets that need their background redrawn, this means all widgets behind them need to be redrawn as well
   if ((widgetflags & NEEDS_BG_REDRAW) ||
      ((widgetflags & NEEDS_REDRAW) && !(widgetflags & VISIBLE))) {
      /// Check whether each widget behind it overlaps
      for (UINT i = 0 ; i < widget_index ; ++i) {
         WidgetBase* w = drawlist[i];
         Rectangle warea = w->Area();
         if (widgetarea.Overlaps(warea)) {
            UINT wflags = w->Flags();
            if ((wflags & NEEDS_REDRAW) != NEEDS_REDRAW) {// w doesn't have the redraw flag set
               w->SetRedrawFlag();
               CheckRedraw(i);
            }
         }
      }
   }
   /// For widgets that need to be redrawn, this means all widgets in front of them need to be redrawn as well
   if ((widgetflags & NEEDS_REDRAW) && (widgetflags & VISIBLE)) {
      /// Check whether each widget in front of it overlaps
      for (UINT i = widget_index + 1 ; i < drawlist.size() ; ++i) {
         WidgetBase* w = drawlist[i];
         Rectangle warea = w->Area();
         if (widgetarea.Overlaps(warea)) {
            UINT wflags = w->Flags();
            if ((wflags & NEEDS_REDRAW) != NEEDS_REDRAW) {// w doesn't have the redraw flag set
               w->SetRedrawFlag();
               CheckRedraw(i);
            }
         }
      }
   }
}



void WidgetHandler::CheckRedraw() {
   /// Each widget that needs its background redrawn needs all overlapping widgets before it to be redrawn as well
   /// Each widget that needs to be redrawn needs all overlapping widgets after it to be redrawn as well.
   /// Each time a widget is set to be redrawn by this algorithm it needs to have the same two checks performed on it as well.
   /// Each widget that overlaps a dirty background area needs to be redrawn
   for (list<Rectangle>::iterator it = dbg_list.begin() ; it != dbg_list.end() ; ++it) {
      Rectangle r = *it;
      for (UINT i = 0 ; i < drawlist.size() ; ++i) {
         WidgetBase* w = drawlist[i];
         Rectangle warea = w->Area();
         if (r.Overlaps(warea)) {
            UINT wflags = w->Flags();
            if ((wflags & NEEDS_REDRAW) != NEEDS_REDRAW) {
               w->SetRedrawFlag();
            }
         }
      }
   }
   /// TODO : From back to front? Or front to back?
   for (UINT i = 0 ; i < drawlist.size() ; ++i) {
      CheckRedraw(i);
   }
}



void WidgetHandler::DrawFocus() {
   if (wfocus && OwnsWidget(wfocus)) {
      Rectangle r = wfocus->Area();
      int color = (*(wfocus->WCols()))[HLCOL];
      int sdcolor = (*(wfocus->WCols()))[SDCOL];
      switch (focus_type) {
         case FOCUS_INVISIBLE :
            // do nothing
            break;
         case FOCUS_HIGHLIGHT_OUTLINE :
            r.Draw(buffer , color);
            break;
         case FOCUS_HIGHLIGHT_THICK_OUTLINE :
            r.Draw(buffer , color);
            r.Shrink(1);
            r.Draw(buffer , color);
            break;
         case FOCUS_HIGHLIGHT_DOTTED :
            r.DottedOutline(buffer , color , true);
            r.DottedOutline(buffer , sdcolor , false);
            break;
         case FOCUS_HIGHLIGHT_THICK_DOTTED :
            r.DottedOutline(buffer , color , true);
            r.DottedOutline(buffer , sdcolor , false);
            r.Shrink(1);
            r.DottedOutline(buffer , color , true);
            r.DottedOutline(buffer , sdcolor , false);
            break;
      }
   }
}



void WidgetHandler::SortDrawListByPriority() {
   
   stable_sort(drawlist.begin() , drawlist.end() , DrawPriorityIsLess);
   
   
/* KEEP FOR NOW
   vector<WidgetBase*> plist;
   for (unsigned int i = 0 ; i < sortlist.size() ; ++i) {
      WidgetBase* w = sortlist[i];
      MenuBar* mbar = dynamic_cast<MenuBar*>(w);
      if (mbar) {
         plist.push_back(w);
      }
   }
   for (unsigned int i = 0 ; i < sortlist.size() ; ++i) {
      WidgetBase* w = sortlist[i];
      WidgetHandler* wh = dynamic_cast<WidgetHandler*>(w);
      if (wh) {
         plist.push_back(w);
      }
   }
   for (unsigned int i = 0 ; i < sortlist.size() ; ++i) {
      WidgetBase* w = sortlist[i];
      MenuBar* mbar = dynamic_cast<MenuBar*>(w);
      WidgetHandler* wh = dynamic_cast<WidgetHandler*>(w);
      if (!mbar && !wh) {
         plist.push_back(w);
      }
   }
   sortlist = plist;
*/
///   vector<WidgetBase*> plist;
///   plist.push_back(&titlebar);
/*
   for (unsigned int i = 0 ; i < sortlist.size() ; ++i) {
      WidgetBase* w = sortlist[i];
      if (w == titlebar) {
         plist.push_back(w);
      }
   }
*/
/**
   for (unsigned int i = 0 ; i < sortlist.size() ; ++i) {
      WidgetBase* w = sortlist[i];
      WidgetHandler* wh = dynamic_cast<WidgetHandler*>(w);
      if (wh) {
         plist.push_back(w);
      }
   }
   for (unsigned int i = 0 ; i < sortlist.size() ; ++i) {
      WidgetBase* w = sortlist[i];
      WidgetHandler* wh = dynamic_cast<WidgetHandler*>(w);
      if (!wh && (w != &titlebar)) {
         plist.push_back(w);
      }
   }
   sortlist = plist;
*/
   
}



void WidgetHandler::AddToRemoveList(WidgetBase* w) {
   bool onlist = false;
   for (unsigned int i = 0 ; i < removelist.size() ; ++i) {
      if (removelist[i] == w) {
         onlist = true;
         break;
      }
   }
   if (!onlist && OwnsWidget(w)) {
      removelist.push_back(w);
   }
}



void WidgetHandler::RemoveOldWidgets() {
   for (unsigned int i = 0 ; i < removelist.size() ; ++i) {
      RemoveWidget(removelist[i]);
   }
   removelist.clear();
}



bool WidgetHandler::HasWidget(WidgetBase* widget) {
   for (UINT i = 0 ; i < wlist.size() ; ++i) {
      WidgetBase* w = wlist[i];
      WidgetHandler* wh = w->GetGui();
      
      if (w == widget) {return true;}
      if (wh && wh->HasWidget(widget)) {return true;}
   }
   return false;
}



void WidgetHandler::FocusStart() {
   focus_start = true;
}



WidgetBase* WidgetHandler::NextFocus() {
   /// focus index holds the index of the current widget with focus in the wlist
   /// return the next widget that accepts focus if there is one in the list
   /// can't skip to the next one if it is a WidgetHandler - need to use NextFocus() instead
   /// skip the WidgetHandler if it returns 0 from NextFocus()
   /// NextFocus() should return 0 when it reaches the end of it's wlist
   int start_index = focus_index;
   if (focus_start) {
      start_index = -1;
      focus_start = false;
   } else if (wlist.size()) {
      WidgetBase* w = wlist[focus_index];
      WidgetHandler* wh = w->GetGui();
      if (wh) {
         WidgetBase* next = wh->NextFocus();
         if (next) {return next;}
      }
   }
   for (int index = start_index + 1 ; index < (int)wlist.size() ; ++index) {
      WidgetBase* widget = wlist[index];
      UINT wflags = widget->Flags();
      if ((wflags & ENABLED) && (wflags & VISIBLE) && InView(widget)) {
         WidgetHandler* whandler = widget->GetGui();
         if (whandler) {
            whandler->FocusStart();
            WidgetBase* next = whandler->NextFocus();
            if (next) {
               return next;
            }
         } else {
            if (widget->AcceptsFocus()) {
               return widget;
            }
         }
      }
   }
   // if next focus reaches the end of the wlist it returns 0 and starts over next time
   focus_start = true;
   return (WidgetBase*)0;
}



WidgetBase* WidgetHandler::PreviousFocus() {
   int start_index = focus_index;
   if (focus_start) {
      start_index = (int)wlist.size();
      focus_start = false;
   } else if (wlist.size()) {
      WidgetBase* w = wlist[focus_index];
      WidgetHandler* wh = w->GetGui();
      if (wh) {
         WidgetBase* previous = wh->PreviousFocus();
         if (previous) {return previous;}
      }
   }
   for (int index = start_index - 1 ; index >= 0 ; --index) {
      WidgetBase* widget = wlist[index];
      UINT wflags = widget->Flags();
      if ((wflags & ENABLED) && (wflags & VISIBLE) && InView(widget)) {
         WidgetHandler* whandler = widget->GetGui();
         if (whandler) {
            whandler->FocusStart();
            WidgetBase* previous = whandler->PreviousFocus();
            if (previous) {
               return previous;
            }
         } else {
            if (widget->AcceptsFocus()) {
               return widget;
            }
         }
      }
   }
   focus_start = true;
   return (WidgetBase*)0;
}



void WidgetHandler::CycleFocusForward() {
   if (!parent) {
      WidgetBase* next = NextFocus();
      if (!next) {
         next = NextFocus();
      }
      GiveWidgetFocus(next);
   }
}



void WidgetHandler::CycleFocusBackward() {
   if (!parent) {
      WidgetBase* previous = PreviousFocus();
      if (!previous) {
         previous = PreviousFocus();
      }
      GiveWidgetFocus(previous);
   }
}



WidgetHandler::WidgetHandler(string name) :
         WidgetBase(name , NULL , Rectangle(0,0,0,0) , DEFAULT_FLAGS) ,
         background_type(BG_SOLID),
         background_src(0),
         background(),
         buffer(),
         wlist(),
         inputlist(),
         drawlist(),
         focus_index(0),
         focus_start(false),
         wfocus(0),
         whover(0),
         mque(),
         clear_background(true),
         focus_cycle_forward(input_key_press(KEY_TAB) && input_key_held(KEY_NO_MOD)),
         focus_cycle_backward(input_key_press(KEY_TAB) && input_key_held(KEY_ONLY_SHIFT)),
         rounded_radius(0),
         rounding_percent(0.5),
         titlebar(name + string("-titlebar") , font , "" , false , false),
         mintitlebar(name + string("-mintitlebar") , font , "" , false , false),
         minimized(false),
         focus_type(FOCUS_HIGHLIGHT_OUTLINE),
         dbg_list(),
         cam(name + string("-cam")),
         real_w(0),
         real_h(0),
         memtrack(),
         removelist()
{
   AddWidget(&titlebar);
   titlebar.SetParentGui(this);
   mintitlebar.SetParentGui(this);
   mintitlebar.SetParent(this);
   mintitlebar.UseWidgetColorset(wcols);
   SyncTitleBarPos();
   cam.SetParent(this);
   cam.TakesFocus(false);
   SetDisplayPriority(HIGH_DISPLAY_PRIORITY);
   SetRedrawFlag();
}



WidgetHandler::WidgetHandler(string name , BITMAP* surface , const Rectangle& position , UINT wflags) :
         WidgetBase(name , surface , position , wflags),
//         WidgetBase(surface , position , wflags | ALLOW_OVERLAP),
         background_type(BG_SOLID),
         background_src(0),
         background(),
         buffer(),
         wlist(),
         inputlist(),
         drawlist(),
         focus_index(0),
         focus_start(false),
         wfocus(0),
         whover(0),
         mque(),
         clear_background(true),
         focus_cycle_forward(input_key_press(KEY_TAB) && input_key_held(KEY_NO_MOD)),
         focus_cycle_backward(input_key_press(KEY_TAB) && input_key_held(KEY_ONLY_SHIFT)),
         rounded_radius(0),
         rounding_percent(0.5),
         titlebar(name + string("-titlebar") , font , "" , false , false),
         mintitlebar(name + string("-mintitlebar") , font , "" , false , false),
         minimized(false),
         focus_type(FOCUS_HIGHLIGHT_OUTLINE),
         dbg_list(),
         cam(name + string("-cam")),
         real_w(0),
         real_h(0),
         memtrack(),
         removelist()
{
   AddWidget(&titlebar);
   titlebar.SetParentGui(this);
   mintitlebar.SetParentGui(this);
   mintitlebar.SetParent(this);
   mintitlebar.UseWidgetColorset(wcols);
   SyncTitleBarPos();
   SetTitleBarVisibility(false);
   if (real_w < area.W()) {real_w = area.W();}
   if (real_h < area.H()) {real_h = area.H();}
   cam.SetParent(this);
   cam.SetArea(0 , 0 , area.W() , area.H());
   cam.TakesFocus(false);
   SetDisplayPriority(HIGH_DISPLAY_PRIORITY);
   SetRedrawFlag();
}



WidgetHandler::WidgetHandler(string name , FONT* title_font , string title_text , bool is_minimized , bool allow_movement ,
               BITMAP* surface , const Rectangle& position , UINT wflags) :
         WidgetBase(name , surface , position , wflags),
         background_type(BG_SOLID),
         background_src(0),
         background(),
         buffer(),
         wlist(),
         inputlist(),
         drawlist(),
         focus_index(0),
         focus_start(false),
         wfocus(0),
         whover(0),
         mque(),
         clear_background(true),
         focus_cycle_forward(input_key_press(KEY_TAB) && input_key_held(KEY_NO_MOD)),
         focus_cycle_backward(input_key_press(KEY_TAB) && input_key_held(KEY_ONLY_SHIFT)),
         rounded_radius(0),
         rounding_percent(0.5),
         titlebar(name + string("-titlebar") , title_font , title_text , is_minimized , allow_movement),
         mintitlebar(name + string("-mintitlebar") , title_font , title_text , is_minimized , allow_movement),
         minimized(is_minimized),
         focus_type(FOCUS_HIGHLIGHT_OUTLINE),
         dbg_list(),
         cam(name + string("-cam")),
         real_w(0),
         real_h(0),
         memtrack(),
         removelist()
{
   AddWidget(&titlebar);
   titlebar.SetParentGui(this);
   mintitlebar.SetParentGui(this);
   mintitlebar.SetParent(this);
   mintitlebar.UseWidgetColorset(wcols);
   SyncTitleBarPos();
   if (real_w < area.W()) {real_w = area.W();}
   if (real_h < area.H()) {real_h = area.H();}
   cam.SetParent(this);
   cam.SetArea(0 , 0 , area.W() , area.H());
   cam.TakesFocus(false);
   SetDisplayPriority(HIGH_DISPLAY_PRIORITY);
   SetRedrawFlag();
}



// The background and buffer will be freed when the WidgetHandler destructor is run.
// Global WidgetHandlers will need to have FreeImageBuffers() called before main exits.
WidgetHandler::~WidgetHandler() {
   for (MTIT it = memtrack.begin() ; it != memtrack.end() ; ++it) {
      if (it->second) {
         delete it->first;
      }
   }
   memtrack.clear();
}



bool WidgetHandler::SetupBitmapHandlers(BACKGROUND_TYPE bg_type , BMP_MEM_TYPE buffer_memory_type , BMP_DRAW_TYPE buffer_draw_type) {
   bool success = true;
   if (!buffer ||
       (real_w != buffer.W()) ||
       (real_h != buffer.H()) ||
       (background_type != bg_type) ||
       (buffer.MemType() != buffer_memory_type) ||
       (buffer.DrawType() != buffer_draw_type)) 
   {
      if (buffer.ClearResize(bitmap_color_depth(screen) , real_w , real_h , buffer_memory_type , buffer_draw_type)) {
         Rectangle cam_view_area = cam.ViewArea();
         int camx = cam_view_area.X();
         int camy = cam_view_area.Y();
         cam.SetView(buffer , Rectangle(camx , camy , area.W() , area.H()));
         cam.SetDrawType(buffer_draw_type);
      } else {
         success = false;
      }
      if (!SetupBackgroundBuffer(bg_type)) {success = false;}
      ResetWidgetDrawingTargets();
      SetBgRedrawFlag();
      if (parent) {
         parent->SetRedrawAllFlag();
      }
   }
   return success;
}



bool WidgetHandler::SetupBackgroundBuffer(BACKGROUND_TYPE bg_type) {
   background_type = bg_type;
   ASSERT(screen);
   Color clear;
   BITMAP* bg_src = background_src;
   background_src = 0;
   switch (bg_type) {
      case BG_SOLID :
         clear = (*wcols)[BGCOL];
         break;
      case BG_ROUNDED :
         clear.SetColor(bitmap_mask_color(screen));
         break;
      case BG_TRANSPARENT :
         clear.SetColor(bitmap_mask_color(screen));
         break;
      case BG_TRANSLUCENT :
         clear.SetRGBAex(0,0,0,0,bitmap_color_depth(screen));
         break;
      case BG_IMAGE :
         background_src = bg_src;
         clear.SetRGBAex(0,0,0,0,bitmap_color_depth(screen));
         break;
         
   };
   background.SetClearColor(clear);
   bool allocated = background.ClearResize(bitmap_color_depth(screen) , real_w , real_h , buffer.MemType() , SOLID);
   if (allocated && (bg_type == BG_ROUNDED)) {
      RedrawRoundedBackground();
   }
   if (allocated && background_src) {
      SetBackgroundImage(background_src);
   }
   clear_background = true;
   return allocated;
}



void WidgetHandler::SetBackgroundImage(BITMAP* bg) {
   BITMAP* bg_dest = (BITMAP*)background;
   ASSERT(bg);
   ASSERT(is_memory_bitmap(bg));
   ASSERT(bg_dest);
   background_src = bg;
   stretch_blit(bg , bg_dest , 0 , 0 , bg->w , bg->h , 0 , 0 , bg_dest->w , bg_dest->h);
   background_type = BG_IMAGE;
   clear_background = true;
}



void WidgetHandler::FreeImageBuffers() {
   background.Free();
   buffer.Free();
}



bool WidgetHandler::HasMessages() {
   return (!mque.empty());
}



WidgetMsg WidgetHandler::TakeNextMessage() {
   WidgetMsg message;
   if (mque.empty()) {
      message = NoMessages;
   } else {
      message = mque.front();
      mque.pop_front();
   }
   return message;
}
   


void WidgetHandler::ClearMessages() {
   mque.clear();
}



void WidgetHandler::ClearDialog() {
   for (MTIT it = memtrack.begin() ; it != memtrack.end() ; ++it) {
      if (it->second) {// bool
         delete (it->first);// WidgetBase*
      }
   }
   memtrack.clear();
   wlist.clear();
   inputlist.clear();
   drawlist.clear();
   wfocus = 0;
   focus_start = true;
   mque.clear();
   AddWidget(&titlebar);
}



void WidgetHandler::RemoveWidget(WidgetBase* w) {
   if (w && OwnsWidget(w)) {
      MakeAreaDirty(w->Area());
      
      MTIT mtit = memtrack.find(w);
      if (mtit != memtrack.end()) {
         if (mtit->second) {
            delete w;
         }
         memtrack.erase(mtit);
      }
      for (WLIT it = wlist.begin() ; it != wlist.end() ; ++it) {
         if (*it == w) {
            wlist.erase(it);
            break;
         }
      }
      for (WLIT it = inputlist.begin() ; it != inputlist.end() ; ++it) {
         if (*it == w) {
            inputlist.erase(it);
            break;
         }
      }
      for (WLIT it = drawlist.begin() ; it != drawlist.end() ; ++it) {
         if (*it == w) {
            drawlist.erase(it);
            break;
         }
      }
      
      if (wfocus == w) {
         GiveWidgetFocus(0);
      } else {
         GiveWidgetFocus(wfocus);
      }
   }
}



void WidgetHandler::AddWidget(WidgetBase* widget , bool delete_when_removed) {
///   Only add unique widgets and never itself.
   ASSERT(widget);
   ASSERT(widget != this);
   bool does_not_have_widget = !HasWidget(widget);
   ASSERT(does_not_have_widget);

   if (widget && (widget != this) && does_not_have_widget) {
      memtrack[widget] = delete_when_removed;
      wlist.push_back(widget);
      inputlist.push_back(widget);
      drawlist.insert(drawlist.begin() , widget);
      /** First widget always has the focus */
      /// The titlebar widget get added first so the second widget should get the focus
      if (wlist.size() == 2) {
         GiveWidgetFocus(widget);
      } else {
         WidgetHandler* wh = widget->GetGui();
         if (wh) {
            wh->GiveWidgetFocus(0,false);
         }
      }
      SortDrawListByPriority();
      widget->SetParent(this);
      widget->SetDrawTarget(buffer);
      widget->UseWidgetColorset(wcols);
      widget->SetBgRedrawFlag();
///      clear_background = true;
   }
}



WidgetHandler& WidgetHandler::operator<<(WidgetBase* widget) {
   AddWidget(widget);
   return *this;
}



WidgetMsg WidgetHandler::Update (double tsec) {
   WidgetMsg retval;// For widget returns
   UINT msg;
   UINT retmsg = DIALOG_OKAY;
   /** Warning - All messages not related to a dialog will be ignored.
         Use QueueUserMessage for user notifications instead.
   */
   if (minimized) {
      mintitlebar.Update(tsec);
   } else {
      for (unsigned int i = 0 ; i < inputlist.size() ; ++i) {
         WidgetBase* widget = inputlist[i];
         retval = widget->Update(tsec);
         if (retval.Topic() == TOPIC_DIALOG) {
            msg = retval.Messages();
            if (msg == DIALOG_OKAY)      {continue;}
            if (msg & DIALOG_CLOSE)      {return retval;}// Pass the CLOSE message up the chain and back to the user
            //if (msg & DIALOG_DISABLED) {}// Ignore disabled dialogs
            if (msg & DIALOG_REDRAW_ME)  {
               SetRedrawFlag();
               retmsg |= DIALOG_REDRAW_ME;
            }
            if (msg & DIALOG_REDRAW_ALL) {
               clear_background = true;
               SetRedrawFlag();
               retmsg |= DIALOG_REDRAW_ME;
            }
            /** DIALOG_INPUT_USED and DIALOG_TAKE_FOCUS are ignored in the Update function
            /// TODO : Should DIALOG_TAKE_FOCUS be ignored in Update?
            ///      : Would there ever be a useful purpose for this?
            
            if (msg & DIALOG_TAKE_FOCUS) {
               GiveWidgetFocus(msg.From());
            }
            if (msg & DIALOG_INPUT_USED) {
               retmsg |= DIALOG_INPUT_USED;
            }
            
            */
            
         }
      }
      cam.Update(tsec);
   }
   return WidgetMsg(this , TOPIC_DIALOG , retmsg);
}



WidgetMsg WidgetHandler::CheckInputs(int msx , int msy) {
   if (!(WidgetBase::flags & ENABLED)) {
      return WidgetMsg(this , TOPIC_DIALOG , DIALOG_DISABLED);
   }
   if (!(wlist.size())) {
      return WidgetMsg(this , TOPIC_DIALOG , DIALOG_OKAY);
   }
   /**
      Since WidgetHandlers have their own buffer, they need to offset the position they pass to their
      widgets since each widget stores its position on their own drawing target (the WidgetHandler buffer).
      This way, every widget that is called will have the relative mouse position on their target bitmap, and
      they won't have to adjust it.
   */
   Rectangle vrect = cam.ViewArea();
   const int relx = msx + vrect.X() - area.X();
   const int rely = msy + vrect.Y() - area.Y();
   WidgetMsg retval;
   bool gui_takes_focus = false;
   UINT msg;
   UINT retmsg = DIALOG_OKAY;
   bool focus_taken = false;

   /// GUIs can take the focus too
   if (parent && area.Contains(msx,msy) && input_mouse_press(LMB)) {
      gui_takes_focus = true;
   }

   if (minimized) {
      retval = mintitlebar.CheckInputs(msx,msy);
      if (retval.Topic() == TOPIC_DIALOG) {
         msg = retval.Messages();
         if (msg & DIALOG_CLOSE) {
            return WidgetMsg(&titlebar , TOPIC_DIALOG , msg);
         }
      }
   } else {

      /** WidgetHandlers will check whether the mouse is hovering over one of the widgets, and set the hover flag for
            that widget, as well as removing it from any widget that had it last.
      */
      if (!parent) {
         WidgetBase* new_whover = 0;
         for (int i = (int)drawlist.size() - 1 ; i >= 0 ; --i) {
            WidgetBase* w = drawlist[i];
   //         WidgetHandler* wh = w->GetGui();
   ///         bool hover = w->IsMouseOver(msx - area.X() , msy - area.Y());
            bool hover = w->IsMouseOver(msx + cam.ViewArea().X() - area.X() , msy + cam.ViewArea().Y() - area.Y());
            if (hover) {
               new_whover = w;
               break;
            }
         }
         if (new_whover != whover) {
            if (whover) {
               whover->SetHoverState(false);
            }
            if (new_whover) {
               new_whover->SetHoverState(true);
            }
            whover = new_whover;
         }
      }
      /**
         Check the widget with focus first - sortlist is sorted so that the widget with focus is first in the list
      */

      for (UINT index = 0 ; index < inputlist.size() ; ++index) {
         WidgetBase* widget = inputlist[index];
         if (!(widget->Flags() & ENABLED)) {continue;}
         retval = widget->CheckInputs(relx,rely);

         /// Warning - All messages not related to a dialog will be ignored.
         /// Use QueueUserMessage for user notifications instead.
         
         if (retval.Topic() == TOPIC_DIALOG) {
            msg = retval.Messages();
            if (msg == DIALOG_OKAY)      {continue;}
            if (msg & DIALOG_CLOSE)      {return retval;}// Pass the CLOSE message up the chain and back to the user
            //if (msg & DIALOG_DISABLED) {}// Ignore disabled dialogs
            if (msg & DIALOG_REDRAW_ME)  {
               SetRedrawFlag();
               retmsg |= DIALOG_REDRAW_ME;
            }
            if (msg & DIALOG_REDRAW_ALL) {
               clear_background = true;
               SetRedrawFlag();
               retmsg |= DIALOG_REDRAW_ME;
            }
            if (msg & DIALOG_REMOVE_ME) {
               AddToRemoveList(widget);
            }
            if (msg & DIALOG_TAKE_FOCUS) {
               /// The GiveWidgetFocus function will rearrange the widgets in the list, so stop processing then
               if (widget == &titlebar) {
                  GiveWidgetFocus(this);
               } else {
                  GiveWidgetFocus(widget);
               }
               focus_taken = true;
            }
            if (msg & DIALOG_INPUT_USED) {
               retmsg |= DIALOG_INPUT_USED;
               // Don't check any of the other widget's inputs during this update
               return WidgetMsg(this , TOPIC_DIALOG , retmsg);
            }
            if (focus_taken) {
               return WidgetMsg(this , TOPIC_DIALOG , retmsg);
            }
         }
      }
      
      msg = cam.CheckInputs(msx - area.X() , msy - area.Y()).Messages();
      if (msg & DIALOG_INPUT_USED) {
         retmsg |= DIALOG_INPUT_USED;
         return WidgetMsg(this , TOPIC_DIALOG , retmsg);
      }

      /// Now that all the widgets have been given priority, check for gui focus changes by key input
      if (focus_cycle_forward) {
         CycleFocusForward();
      }
      if (focus_cycle_backward) {
         CycleFocusBackward();
      }
   }
   if (gui_takes_focus) {
      return WidgetMsg(this , TOPIC_DIALOG , DIALOG_TAKE_FOCUS);
   }
   return WidgetMsg(this , TOPIC_DIALOG , retmsg);
}



void WidgetHandler::DisplayOn(BITMAP* bmp , int x , int y) {
   ASSERT(bmp);
   ASSERT(background);
   ASSERT(buffer);

   RemoveOldWidgets();

/* Uncomment to display diagnostic output below
   SetRedrawFlag();
//*/

   if (WidgetBase::flags & NEEDS_REDRAW) {

      if (!minimized) {
/* Uncomment to display diagnostic output below
         clear_background = true;
//*/
         if (clear_background) {
            // WidgetHandler needs to be fully redrawn, so redraw every contained widget as well, regardless
            // of the state of their NEEDS_REDRAW flag.
            blit(background , buffer , 0 , 0 , 0 , 0 , background.W() , background.H());
            dbg_list.clear();
            for (UINT i = 0 ; i < drawlist.size() ; ++i) {
               WidgetBase* w = drawlist[i];
               if (w->Flags() & VISIBLE) {
                  w->Display();
                  if (wfocus == w) {DrawFocus();}
               }
            }
/* Uncomment to output wfocus and focus_index on the buffer. Uncomment clear_background = true; above.
            Rectangle textbox(0 , area.H() - 50 , 150 , 25);
            textbox.Fill(buffer , (*wcols)[BGCOL]);
            textprintf_ex(buffer , font , textbox.X() , textbox.Y() , (*wcols)[TXTCOL] , -1 ,
                          "wfocus = %s" , (wfocus?wfocus->Name().c_str():"NULL"));
            textprintf_ex(buffer , font , textbox.X() , textbox.Y() + 12, (*wcols)[TXTCOL] , -1 , "focus_index = %i" , focus_index);
//*/
/* Uncomment to output whover on the buffer. Uncomment clear_background = true; above.
            Rectangle textbox(0 , area.H() - 50 , 150 , 25);
            textbox.Fill(buffer , (*wcols)[BGCOL]);
            textprintf_ex(buffer , font , textbox.X() , textbox.Y() , (*wcols)[TXTCOL] , -1 ,
                          "whover = %s" , (whover?whover->Name().c_str():"NULL"));
//*/
            
            clear_background = false;
         } else {
            
            CheckRedraw();
            
            /** Draw dirty backgrounds first */
            dbg_list = ConsolidateRectangles(dbg_list);
            for (list<Rectangle>::iterator it = dbg_list.begin() ; it != dbg_list.end() ; ++it) {
               Rectangle& r = *it;
               blit(background , buffer , r.X() , r.Y() , r.X() , r.Y() , r.W() , r.H());
            }
            dbg_list.clear();
            
            /** Only widgets with the NEEDS_REDRAW flag should actually redraw */
            bool some_drawn = false;
            for (UINT i = 0 ; i < drawlist.size() ; ++i) {
               WidgetBase* w = drawlist[i];
               UINT wflags = w->Flags();
               if ((wflags & NEEDS_REDRAW) || (some_drawn && (wflags & ALLOW_OVERLAP))) {
                  if (wflags & VISIBLE) {
                     some_drawn = true;
                     w->Display();
                     if (wfocus == w) {DrawFocus();}
                  }
               }
            }
         }
      }
   }
   if (flags & VISIBLE) {
      if (minimized) {
         mintitlebar.DisplayOn(bmp , x , y);
      } else {
         cam.DisplayOn(bmp , x + area.X() , y + area.Y());
      }
   }
   ClearRedrawFlag();
}



bool WidgetHandler::Init() {
   ClearMessages();// empty the message queue of any startup messages
   bool init = true;
   for (WLIT it = wlist.begin() ; it != wlist.end() ; ++it) {
      WidgetBase* w = *it;
      if (!w->Init()) {init = false;}
      w->SetRedrawFlag();
   }
   /// TODO : Is this predrawing really necessary?
   if (init) {
      SetVisibilityState(false);
      SetRedrawFlag();
      Display();// Get everything drawn to the buffer
      SetVisibilityState(true);
   }
   return init;
}



bool WidgetHandler::Ready() {
   if (!background) {return false;}
   if (!buffer) {return false;}
   bool ready = true;
   for (WLIT it = wlist.begin() ; it != wlist.end() ; ++it) {
      if (!(*it)->Ready()) {ready = false;}
   }
   return ready;
}



bool WidgetHandler::Shutdown() {
   bool shutdown = true;
   for (WLIT it = wlist.begin() ; it != wlist.end() ; ++it) {
      if (!(*it)->Shutdown()) {shutdown = false;}
   }
   return shutdown;
}



void WidgetHandler::QueueUserMessage(const WidgetMsg& wmsg) {
   
   const WidgetMsg titleminmsg(&titlebar , TOPIC_TITLE_BAR , TITLE_MINIMIZED);
   const WidgetMsg titlemaxmsg(&titlebar , TOPIC_TITLE_BAR , TITLE_MAXIMIZED);// ignore
   const WidgetMsg mintitleminmsg(&mintitlebar , TOPIC_TITLE_BAR , TITLE_MINIMIZED);// ignore
   const WidgetMsg mintitlemaxmsg(&mintitlebar , TOPIC_TITLE_BAR , TITLE_MAXIMIZED);
   
   const WidgetMsg titleclosemsg(&titlebar , TOPIC_TITLE_BAR , TITLE_SIGNAL_CLOSE);
   const WidgetMsg mintitleclosemsg(&mintitlebar , TOPIC_TITLE_BAR , TITLE_SIGNAL_CLOSE);
   
   const WidgetMsg cammovemsg(&cam , TOPIC_CAMERA , CAMERA_VIEW_MOVED);
   
   if ((wmsg == titlemaxmsg) || (wmsg == mintitleminmsg)) {return;}
   
   if (wmsg == titleminmsg) {
      minimized = true;
      mintitlebar.Minimize();
      if (parent) {
         parent->SetRedrawAllFlag();
      }
      WidgetBase::QueueUserMessage(wmsg);
      return;
   }
   
   if (wmsg == mintitlemaxmsg) {
      minimized = false;
      titlebar.Maximize();
      if (parent) {
         parent->SetRedrawAllFlag();
      }
      WidgetBase::QueueUserMessage(wmsg);
      return;
   }
   
   if (wmsg == mintitleclosemsg) {
      QueueUserMessage(titleclosemsg);
      return;
   }
   
   if (wmsg == cammovemsg) {
      Rectangle r = cam.ViewArea();
      titlebar.SetDrawPos(r.X() , r.Y());
      return;
   }
   
   if (messagehandler || parent) {
      WidgetBase::QueueUserMessage(wmsg);
   } else {
      mque.push_back(wmsg);
   }
}



void WidgetHandler::SetWidgetColors(const WidgetColorset& colors) {
   WidgetBase::SetWidgetColors(colors);
   if (!mintitlebar.UsingOwnColors()) {
      mintitlebar.UseWidgetColorset(wcols);
   }
   for (WLIT it = wlist.begin() ; it != wlist.end() ; ++it) {
      WidgetBase* const w = *it;
      if (!w->UsingOwnColors()) {
         w->UseWidgetColorset(wcols);
      }
   }
   if (background_type == BG_SOLID) {
      if (background) {
         background.ClearTo((*wcols)[BGCOL]);
      }
   }
   if (background_type == BG_ROUNDED) {
      RedrawRoundedBackground();
   }
   SetRedrawAllFlag();
}



void WidgetHandler::SetWidgetColor(const WIDGETCOLOR& wcolor , int color) {
   
   /// TODO : Check out this function a little more, test it
   
   WidgetBase::Colorset().SetColor(wcolor , color);
   if (wcols == &wc) {SetRedrawAllFlag();}
   if (wcolor == BGCOL) {
      if (background_type == BG_SOLID) {
         if (background) {
            background.ClearTo((*wcols)[BGCOL]);
         }
      }
      if (background_type == BG_ROUNDED) {
         RedrawRoundedBackground();
      }
   }
}



void WidgetHandler::UseWidgetColorset(WidgetColorset* colorset) {
   if (colorset) {
      WidgetBase::wcols = colorset;
      mintitlebar.UseWidgetColorset(colorset);
      for (WLIT it = wlist.begin() ; it != wlist.end() ; ++it) {
         (*it)->UseWidgetColorset(colorset);
      }
      if (background_type == BG_SOLID) {
         if (background) {
            background.ClearTo((*wcols)[BGCOL]);
         }
      }
      if (background_type == BG_ROUNDED) {
         RedrawRoundedBackground();
      }
      SetRedrawAllFlag();
   }
}



void WidgetHandler::SetDrawTarget(BITMAP* surface) {
   WidgetBase::SetDrawTarget(surface);
   SetRedrawFlag();
}



void WidgetHandler::SetDrawPos(int xpos , int ypos) {
   if (flags & MOVEABLE) {
      if (minimized) {
         mintitlebar.SetDrawPos(xpos,ypos);
         if (parent) {
            parent->SetRedrawAllFlag();
         }
      } else {
         WidgetBase::SetDrawPos(xpos,ypos);
      }
      SetBgRedrawFlag();
   }
}



void WidgetHandler::SetDrawDimensions(int width , int height) {
   if (minimized) {
      if (flags & RESIZEABLE) {
         mintitlebar.SetDrawDimensions(width,height);
         SetBgRedrawFlag();
         if (parent) {
            parent->SetRedrawAllFlag();
         }
      }
   } else {
      if (flags & RESIZEABLE) {
         if (real_w < width) {real_w = width;}
         if (real_h < height) {real_h = height;}
         WidgetBase::SetDrawDimensions(width,height);
         cam.SetArea(0 , 0 , area.W() , area.H());
         if (buffer) {
            SetupBitmapHandlers(background_type , buffer.MemType() , buffer.DrawType());
         }
         QueueUserMessage(WidgetMsg(this , TOPIC_GUI , GUI_RESIZED));
      }
   }
}



void WidgetHandler::SetArea(int xpos , int ypos , int width , int height) {
   if (minimized) {
      if ((flags & MOVEABLE) && !(flags & RESIZEABLE)) {
         SetDrawPos(xpos,ypos);
         return;
      }
      if (!(flags & MOVEABLE) && (flags & RESIZEABLE)) {
         SetDrawDimensions(width , height);
         return;
      }
      if ((flags & MOVEABLE) && (flags & RESIZEABLE)) {
         mintitlebar.SetArea(xpos,ypos,width,height);
         SetBgRedrawFlag();
         if (parent) {
            parent->SetRedrawAllFlag();
         }
      }
   } else {
      if ((flags & MOVEABLE) && !(flags & RESIZEABLE)) {
         SetDrawPos(xpos,ypos);
         return;
      }
      if (!(flags & MOVEABLE) && (flags & RESIZEABLE)) {
         SetDrawDimensions(width , height);
         return;
      }
      if ((flags & MOVEABLE) && (flags & RESIZEABLE)) {
         if (real_w < width) {real_w = width;}
         if (real_h < height) {real_h = height;}
         WidgetBase::SetArea(xpos,ypos,width,height);
         cam.SetArea(0 , 0 , area.W() , area.H());
         if (buffer) {
            SetupBitmapHandlers(background_type , buffer.MemType() , buffer.DrawType());
         }
         QueueUserMessage(WidgetMsg(this , TOPIC_GUI , GUI_RESIZED));
      }
   }
}



void WidgetHandler::SetArea(const Rectangle& r) {
   if (minimized) {
      mintitlebar.SetArea(r);
      SetBgRedrawFlag();
      if (parent) {
         parent->SetRedrawAllFlag();
      }
   } else {
      if ((flags & MOVEABLE) && !(flags & RESIZEABLE)) {
         SetDrawPos(r.X() , r.Y());
         return;
      }
      if (!(flags & MOVEABLE) && (flags & RESIZEABLE)) {
         SetDrawDimensions(r.W() , r.H());
         return;
      }
      if ((flags & MOVEABLE) && (flags & RESIZEABLE)) {
         if (real_w < r.W()) {real_w = r.W();}
         if (real_h < r.H()) {real_h = r.H();}
         WidgetBase::SetArea(r);
         cam.SetArea(0 , 0 , area.W() , area.H());
         if (buffer) {
            SetupBitmapHandlers(background_type , buffer.MemType() , buffer.DrawType());
         }
         QueueUserMessage(WidgetMsg(this , TOPIC_GUI , GUI_RESIZED));
      }
   }
}



void WidgetHandler::SetEnabledState(bool state) {
   WidgetBase::SetEnabledState(state);
   mintitlebar.SetEnabledState(state);
}



void WidgetHandler::SetVisibilityState(bool state) {
   WidgetBase::SetVisibilityState(state);
   mintitlebar.SetVisibilityState(state);
}



void WidgetHandler::SetNeedsRedrawState(bool state) {
   if (state && SeeThrough()) {
      WidgetBase::SetNeedsBgRedrawState(state);
   } else {
      WidgetBase::SetNeedsRedrawState(state);
   }
}



void WidgetHandler::SetRedrawFlag() {
   if (SeeThrough()) {
      WidgetBase::SetNeedsBgRedrawState(true);
   } else {
      WidgetBase::SetNeedsRedrawState(true);
   }
}


/** Revised but buggy SetFocusState and GiveWidgetFocus

/// One widget handler has the focus at a time, and it may have one of its widgets have the focus
/// 
void WidgetHandler::SetFocusState(bool state) {
   WidgetBase::SetFocusState(state);
   titlebar.SetFocusState(state);
   mintitlebar.SetFocusState(state);
   if (!state) {
      if (wfocus) {
         wfocus->SetFocusState(false);
         /// The widget handler that owns the wfocus needs to be notified it has lost focus as well
         WidgetBase* pfocus = wfocus->Parent();
         WidgetHandler* pwh = dynamic_cast<WidgetHandler*>(pfocus);
         if (pwh && (pwh != this)) {
            pwh->SetFocusState(false);
         }
         wfocus = 0;
      }
   }
}




///   How to keep the widgets sorted?
///   - Have the focused widget always be first in the sorted list by shifting it there from
///      its last position.
///   - Keep track of the index of the widget with focus and check input forward from there
///      and draw backwards from the end to the focused widget.

bool WidgetHandler::GiveWidgetFocus(WidgetBase* widget) {
//   OutputLog() << "Giving widget focus to " << widget << endl;
   if (parent) {
      return parent->GiveWidgetFocus(widget);
   }
   if (!widget) {
      SetFocusState(false);
      wfocus = 0;
      focus_index = wlist.size();
      return true;
   }
   if (HasWidget(widget)) {
      bool accepts_focus = widget->AcceptsFocus();
      if (!accepts_focus) {return false;}
      if (wfocus != widget) {
         if (wfocus) {
            WidgetBase* pfocus = wfocus->Parent();
            WidgetHandler* pwh = dynamic_cast<WidgetHandler*>(pfocus);
            if (pwh) {
               pwh->SetFocusState(false);
            } else {
               wfocus->SetFocusState(false);
            }
         }
         WidgetBase* pwidget = widget->Parent();
         WidgetHandler* pwh = dynamic_cast<WidgetHandler*>(pwidget);
         if (pwh) {
            pwh->SetFocusState(true);
         }
         wfocus = widget;
         focus_index = WidgetIndex(widget);
         wfocus->SetFocusState(true);

         // Move the widget with focus to the beginning of the sorted list
         WLIT focus_iter = SortListIterator(wfocus);
         if (focus_iter == sortlist.end()) {
            // See if any of the top level widgets is a WidgetHandler and whether it has the widget wfocus
            for (UINT i = 0 ; i < sortlist.size() ; ++i) {
               WidgetBase* w = sortlist[i];
               WidgetHandler* wh = dynamic_cast<WidgetHandler*>(w);
               TabGroup* tg = dynamic_cast<TabGroup*>(w);
               if (wh) {
                  if (wh->HasWidget(wfocus)) {
                     focus_iter = SortListIterator(w);
                     i = sortlist.size();
                  }
               }
               if (tg) {
                  if (tg->HasWidget(wfocus)) {
                     focus_iter = SortListIterator(w);
                     i = sortlist.size();
                  }
               }
            }
         }
         ASSERT(focus_iter != sortlist.end());
         WidgetBase* w = *focus_iter;
         sortlist.erase(focus_iter);
         sortlist.insert(sortlist.begin() , w);
         return true;
      }
   }
   return false;
}
//*/


//** OLD SetFocusState and GiveWidgetFocus

void WidgetHandler::SetFocusState(bool state) {
///   if (state != (flags & HASFOCUS)) {/// HACK - sometimes the state isn't different when it maybe should be
                                        /// so this is necessary to run the enclosed code
      WidgetBase::SetFocusState(state);
      titlebar.SetFocusState(state);
      mintitlebar.SetFocusState(state);
      if (!state) {
         if (wfocus) {
            wfocus->SetFocusState(false);
            /// The widget handler that owns the wfocus needs to be notified it has lost focus as well
            WidgetBase* pfocus = wfocus->Parent();
            WidgetHandler* pwh = dynamic_cast<WidgetHandler*>(pfocus);
            if (pwh && (pwh != this)) {
               pwh->GiveWidgetFocus(0 , false);
            }
            wfocus = 0;
         }
      }
///   }
}




///   How to keep the widgets sorted?
///   - Have the focused widget always be first in the sorted list by shifting it there from
///      its last position.
///   - Keep track of the index of the widget with focus and check input forward from there
///      and draw backwards from the end to the focused widget.

bool WidgetHandler::GiveWidgetFocus(WidgetBase* widget , bool notify_parent) {
//   OutputLog() << "Giving widget focus to " << widget << endl;
   if (parent && notify_parent) {
      return parent->GiveWidgetFocus(widget , true);
   }
   
   /// TODO : Working here - 
   
   if (!widget) {
      SetFocusState(false);
      return true;
   }
   if (HasWidget(widget) && widget->AcceptsFocus()) {
/** WARNING : Do not enable this line, if you do it will break RemoveWidget because it depends
              on being able to set the same focus again (this resets the focus_index properly).
   if ((widget != wfocus) && HasWidget(widget) && widget->AcceptsFocus()) {
*/      
      if (wfocus) {
         /// The widget handler that owns the wfocus needs to be notified it has lost focus as well
         wfocus->SetFocusState(false);
         WidgetBase* pfocus = wfocus->Parent();
         WidgetHandler* pwh = dynamic_cast<WidgetHandler*>(pfocus);
         if (pwh) {
            pwh->GiveWidgetFocus(0 , false);
         }
      }
      if (OwnsWidget(widget)) {
         SetFocusState(true);
      }
      wfocus = widget;
      focus_index = WidgetIndex(widget);
      wfocus->SetFocusState(true);

      // Move the widget with focus to the beginning of the sorted list
      WLIT focus_iter = InputListIterator(wfocus);
      if (focus_iter == inputlist.end()) {
         // See if any of the top level widgets is a WidgetHandler and whether it has the widget wfocus
         for (UINT i = 0 ; i < inputlist.size() ; ++i) {
            WidgetBase* w = inputlist[i];
            WidgetHandler* wh = w->GetGui();
            if (wh) {
               if (wh->HasWidget(wfocus)) {
                  wh->GiveWidgetFocus(wfocus , false);
                  focus_iter = InputListIterator(w);
                  i = inputlist.size();
               }
            }
         }
      }
      ASSERT(focus_iter != inputlist.end());
      // move to front of input list
      WidgetBase* w = *focus_iter;
      inputlist.erase(focus_iter);
      inputlist.insert(inputlist.begin() , w);
      WLIT draw_iter = drawlist.end();
      for (draw_iter = drawlist.begin() ; draw_iter != drawlist.end() ; ++draw_iter) {
         if (*draw_iter == w) {break;}
      }
      ASSERT(draw_iter != drawlist.end());
      // move to end of draw list
      drawlist.erase(draw_iter);
      drawlist.push_back(w);
      SortDrawListByPriority();
      return true;
   }
   return false;
}

//*/


const Rectangle WidgetHandler::Area() const {
   if (minimized) {
      return mintitlebar.Area();
   } else {
      return area;
   }
}



void WidgetHandler::SetRedrawAllFlag() {
   clear_background = true;
   SetRedrawFlag();
}



bool WidgetHandler::ChangeAttributes(int color_depth , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type) {
   /// If using nested WidgetHandlers, use ChangeAttributes in order from the most nested WidgetHandler to the topmost one
   /// otherwise the call to SetColorDepth will fail on nested widget handlers using SYSTEM or VIDEO memory.
   Rectangle vrect = cam.ViewArea();

   bool success = (
      buffer.ChangeAttributes(color_depth , bmp_mem_type , bmp_draw_type) &&
      background.ChangeAttributes(color_depth , bmp_mem_type , bmp_draw_type)
   );
   cam.SetView(buffer , vrect);
   
   ResetWidgetDrawingTargets();
   
   WidgetBase::SetColorDepth(color_depth);
   for (UINT i = 0 ; i < wlist.size() ; ++i) {
      WidgetBase* w = wlist[i];
      if (!(w->SetColorDepth(color_depth))) {
         success = false;
      }
   }
   return success;
}



bool WidgetHandler::SetColorDepth(int color_depth) {
   /** Only one color depth can be in use at a time with SYSTEM or VIDEO memory
      - You need to manually change the memory type to MEMORY before changing color depth,
         and then when the graphics mode is changed, change back to SYSTEM or VIDEO memory.
         The ChangeAttributes function can do both at the same time and save an allocation and blit.
   */
   ASSERT(buffer.MemType() == MEMORY);
   ASSERT(ColorDepthIndex(color_depth) != NUMCOLORDEPTHS);

   WidgetBase::SetColorDepth(color_depth);
   Rectangle vrect = cam.ViewArea();
   bool success = (
      background.ChangeColorDepth(color_depth) &&
      buffer.ChangeColorDepth(color_depth)
   );
   cam.SetView(buffer , vrect);
   
   ResetWidgetDrawingTargets();
   
   if (!mintitlebar.SetColorDepth(color_depth)) {success = false;}
   for (WLIT it = wlist.begin() ; it != wlist.end() ; ++it) {
      if (!((*it)->SetColorDepth(color_depth))) {
         success = false;
      }
   }
   return success;
}



bool WidgetHandler::ChangeToMemoryType(BMP_MEM_TYPE mem_type) {
   Rectangle vrect = cam.ViewArea();
   bool success = (
      buffer.ChangeToMemoryType(mem_type) &&
      background.ChangeToMemoryType(mem_type)
   );
   cam.SetView(buffer , vrect);
   
   ResetWidgetDrawingTargets();
   return success;
}



bool WidgetHandler::HasGui() {
   return true;
}



WidgetHandler* WidgetHandler::GetGui() {
   return this;
}



void WidgetHandler::SetRoundingRadiusPercent(double percent) {
   if (percent < 0.0) {percent = 0.0;}
   if (percent > 1.0) {percent = 1.0;}
   rounding_percent = percent;
   int min = (area.W() < area.H())?area.W():area.H();
   min = min/2;
   rounded_radius = (int)(rounding_percent*(float)min);
   if (background_type == BG_ROUNDED) {
      RedrawRoundedBackground();
      titlebar.ResetPositions(false);
   }
}



void WidgetHandler::SetTitleBarVisibility(bool visible) {
   if (visible) {
      titlebar.ShowWidget();
   } else {
      titlebar.HideWidget();
   }
//   SetRedrawAllFlag();
}



void WidgetHandler::SetTitleText(string text) {
   titlebar.SetTitle(text);
   mintitlebar.SetTitle(text);
}



void WidgetHandler::SetTitleFont(FONT* textfont) {
   ASSERT(textfont);
   const FONT* title_font = titlebar.GetFont();
   if (textfont && (textfont != title_font)) {
      titlebar.SetFont(textfont);
      titlebar.SetArea(GetTitleBarArea());
      
      int minheight = 2*text_height(textfont);
      Rectangle mtrect = mintitlebar.Area();
      if (mtrect.H() > minheight) {minheight = mtrect.H();}
      mintitlebar.SetFont(textfont);
      mintitlebar.SetArea(mtrect.X() , mtrect.Y() , mtrect.W() , minheight);
      if (minimized && parent) {parent->SetRedrawAllFlag();}
   }
}



void WidgetHandler::SyncTitleBarPos() {
   Rectangle r = GetTitleBarArea();
   r.MoveBy(area.X() , area.Y());
   bool oldminstate = mintitlebar.Minimized();
   mintitlebar.Minimize();
   mintitlebar.SetArea(r);
   mintitlebar.SetMinimizeState(oldminstate);
}



void WidgetHandler::Minimize() {
   SetBgRedrawFlag();

   minimized = true;
   titlebar.Minimize();
   mintitlebar.Minimize();

   SetRedrawFlag();
//   if (parent) {
//      parent->SetRedrawAllFlag();
//   }
}



void WidgetHandler::Maximize() {
   SetBgRedrawFlag();
   
   minimized = false;
   titlebar.Maximize();
   mintitlebar.Maximize();

   SetRedrawFlag();
//   if (parent) {
//      parent->SetRedrawAllFlag();
//   }
}



void WidgetHandler::ToggleMinimizeState() {
   SetMinimizeState(!minimized);
}



void WidgetHandler::SetMinimizeState(bool is_minimized) {
   if (is_minimized) {
      Minimize();
   } else {
      Maximize();
   }
}



void WidgetHandler::SetFocusDrawType(FOCUS_DRAW_TYPE type , bool for_all_guis) {
   focus_type = type;
   if (wfocus) {wfocus->SetBgRedrawFlag();}
   if (for_all_guis) {
      for (unsigned int i = 0 ; i < wlist.size() ; ++i) {
         WidgetBase* w = wlist[i];
         TabGroup* tg = dynamic_cast<TabGroup*>(w);
         if (tg) {
            tg->SetFocusDrawType(type);
         } else {
            WidgetHandler* wh = w->GetGui();
            if (wh) {
               wh->SetFocusDrawType(type , true);
            }
         }
      }
   }
}



void WidgetHandler::SetBlenderType(ALPHA_BLENDER_TYPE type) {
   cam.SetBlenderType(type);
   SetBgRedrawFlag();
}



void WidgetHandler::MakeAreaDirty(Rectangle dirty_area) {
   dbg_list.push_back(dirty_area);
   SetRedrawFlag();
}



bool WidgetHandler::SetVirtualDimensions(int vwidth , int vheight) {
   if (vwidth < area.W()) {vwidth = area.W();}
   if (vheight < area.H()) {vheight = area.H();}
   real_w = vwidth;
   real_h = vheight;
   if (buffer) {return SetupBitmapHandlers(background_type , buffer.MemType() , buffer.DrawType());}
   return true;
}



void WidgetHandler::AccMoveViewTlxTo(int xpos , int ypos , double time) {
   cam.AccMoveViewTlxTo(xpos , ypos , time);
}



void WidgetHandler::AccMoveViewCenterTo(int xpos , int ypos , double time) {
   cam.AccMoveViewCenterTo(xpos , ypos , time);
}



void WidgetHandler::AccMoveViewBy(int dx , int dy , double time) {
   cam.AccMoveViewBy(dx , dy , time);
}



void WidgetHandler::MoveViewTlxTo(int xpos , int ypos , double time) {
   cam.MoveViewTlxTo(xpos , ypos , time);
}



void WidgetHandler::MoveViewCenterTo(int xpos , int ypos , double time) {
   cam.MoveViewCenterTo(xpos , ypos , time);
}



void WidgetHandler::MoveViewBy(int dx , int dy , double time) {
   cam.MoveViewBy(dx , dy , time);
}



void WidgetHandler::AllowMiddleMouseButtonDrag(bool allow) {
   cam.AllowMiddleMouseButtonDrag(allow);
}



WidgetBase* WidgetHandler::CurrentFocus() {
   return wfocus;
}



WidgetBase* WidgetHandler::CurrentHover() {
   return whover;
}



BMP_DRAW_TYPE WidgetHandler::GetDrawType() {
   return buffer.DrawType();
}



bool WidgetHandler::SeeThrough() {
   return (GetDrawType() != SOLID);
}



Rectangle WidgetHandler::GetTitleBarArea() const {
   const FONT* title_font = titlebar.GetFont();
   int bar_height = 2*text_height(title_font);
   
   Rectangle r = cam.ViewArea();
   
   if (background_type == BG_ROUNDED) {
      return Rectangle(r.X() + rounded_radius + 2, r.Y() + 2 , area.W() - (2*rounded_radius + 4) , bar_height);
   }
   return Rectangle(r.X() + 1 , r.Y() + 1 , area.W()-2 , bar_height);
}



Rectangle WidgetHandler::GetRealTitleBarArea() const {
   return titlebar.Area();
}



bool WidgetHandler::Minimized() {
   return minimized;
}



bool WidgetHandler::AreaFree(Rectangle r , WidgetBase* widget) {
   if (!area.Contains(r)) {return false;}
   for (UINT i = 0 ; i < wlist.size() ; ++i) {
      WidgetBase* w = wlist[i];
      if ((w != widget) && r.Overlaps(w->Area())) {
         return false;
      }
   }
   return true;
}



bool WidgetHandler::InView(WidgetBase* w) {
   if (!w) {return false;}
   Rectangle view = cam.ViewArea();
   Rectangle warea = w->Area();
   return (view.Overlaps(warea));
   /// TODO : Perhaps add in checking to see if the widget w is fully obscured by other widgets?
}



WidgetBase* WidgetHandler::GetWidgetAt(int x , int y) {
   for (int i = (int)drawlist.size() - 1 ; i >= 0 ; --i) {
      WidgetBase* w = drawlist[i];
      if (w->Area().Contains(x,y)) {
         WidgetHandler* wh = dynamic_cast<WidgetHandler*>(w);
         if (wh) {
            return wh->GetWidgetAt(x - wh->Area().X() , y - wh->Area().Y());
         } else {
            return w;
         }
      }
   }
   if (area.Contains(x,y)) {return this;}
   return 0;
}



void WidgetHandler::PrintTo(std::ostream& os , Indenter& indent) const {
   os << indent << "WidgetHandler class object - address " << this << " {" << endl;
   ++indent;
   os << indent << "Buffer Data :" << endl;
   ++indent;
   buffer.PrintTo(os , indent);
   --indent;
   os << indent << "Background Data :" << endl;
   ++indent;
   background.PrintTo(os , indent);
   --indent;
   os << indent << "clear_background = " << (clear_background?"true":"false") << endl;
   os << indent << "wfocus = " << wfocus << " , whover = " << whover << endl;
   PrintBaseTo(os , indent);
   os << indent << "Widgets used by this dialog : {" << endl;
   ++indent;
   unsigned int nwidgets = wlist.size();
   for (unsigned int i = 0 ; i < nwidgets ; ++i) {
      os << indent << "Widget # " << i << endl;
      (*(wlist[i])).PrintTo(os , indent);
   }
   --indent;
   os << indent << "}" << endl;
   /** TODO : Implement global PrintTo for the message queue that translates the WidgetMsg values into words */
   --indent;
   os << indent << "}" << endl;
}



std::ostream& operator<<(std::ostream& os , const WidgetHandler& wh) {
///   (*(dynamic_cast<const WidgetBase*>(&wh))).PrintTo(os);/// Really? This shouldn't be necessary
///   wh.PrintTo(os);/// LAME, WidgetBase::PrintTo(ostream& os) is not recognized
   Indenter i(0,3);
   wh.PrintTo(os , i);
   return os;
}








