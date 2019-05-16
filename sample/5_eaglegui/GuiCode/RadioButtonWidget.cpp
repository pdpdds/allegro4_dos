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


#include "Eagle/GuiCode/RadioButtonWidget.hpp"
#include "Eagle/Error.hpp"
#include "Eagle/StringWork.hpp"

using std::string;
using std::map;
using std::vector;
using std::endl;



extern const unsigned int TOPIC_RADIO_BUTTON = NextFreeTopicId();



const char* ToString(RADIO_TYPE type) {
   switch (type) {
      case RADIO_HORIZONTAL : return "RADIO_HORIZONTAL";
      case RADIO_VERTICAL : return "RADIO_VERTICAL";
   }
   return "Unknown RADIO_TYPE";
}



/// --------------------------------     Radio Button Item Base class     ------------------------------



RadioButtonItemBase::RadioButtonItemBase(std::string text_str) :
      text(text_str)
{}
/*
RadioButtonItemBase::RadioButtonItemBase(std::string text_str , std::string name) :
      text(text_str)
{}



RadioButtonItemBase::RadioButtonItemBase(std::string text_str , std::string name ,
                                         Rectangle position , UINT wflags) :
      text(text_str)
{}
*/


/// --------------------------------     Icon Radio Button Item class     ------------------------------



IconRadioButtonItem::IconRadioButtonItem(std::string text_label , std::string name) :
      RadioButtonItemBase(text_label),
      IconButton(name)
{}



IconRadioButtonItem::IconRadioButtonItem(std::string text_label , std::string name , ICON_DISPLAY_TYPE dtype ,
                                         Rectangle position , UINT wflags) :
      RadioButtonItemBase(text_label),
      IconButton(name , dtype , position , wflags)
{}



IconRadioButtonItem::IconRadioButtonItem(std::string text_label , std::string name , ICON_DISPLAY_TYPE dtype , BUTTON_SHAPE shape ,
                                         InputGroup input , Rectangle position , UINT wflags) :
      RadioButtonItemBase(text_label),
      IconButton(name , dtype , shape , TOGGLE_BTN , input , position , wflags)
{}



string IconRadioButtonItem::Text() {
   return RadioButtonItemBase::text;
}



void IconRadioButtonItem::SetButtonUpState(bool button_up) {
   IconButton::SetButtonUpState(button_up);
}



bool IconRadioButtonItem::ButtonUpState() {
   return IconButton::ButtonUpState();
}



/// --------------------------------     Radio Button Item class     ------------------------------



RadioButtonItem::RadioButtonItem(std::string name , BUTTON_CLASS btnclass , BUTTON_SHAPE shape , FONT* textfont , std::string text_label ,
                                 int btnw , int btnh , int padding , Rectangle position , UINT wflags) :
      RadioButtonItemBase(text_label),
      WidgetBase(name , position , wflags),
      button(0),
//      button(name + string("-button") , btype , TOGGLE_BTN , textfont , "" , input_key_press(KEY_NONE) , Rectangle(0,0,0,0)),
      text_font(textfont),
      hpad(0),
      tx(0),
      ty(0),
      tw(0),
      th(0)
{
   ASSERT(textfont);
   if (btnw < 1) {btnw = 1;}
   if (btnh < 1) {btnh = 1;}
   if (padding < 0) {padding = 0;}
   
//      button(name + string("-button") , btype , TOGGLE_BTN , textfont , "" , input_key_press(KEY_NONE) , Rectangle(0,0,0,0)),
   switch (btnclass) {
      case PLAIN_BUTTON :
         button = new Button(name + string("-button") , shape , TOGGLE_BTN , textfont , "" , input_key_press(KEY_NONE) , Rectangle());
         break;
      case HOVER_BUTTON :
         button = new HoverButton(name + string("-button") , shape , TOGGLE_BTN , textfont , "" , input_key_press(KEY_NONE) , Rectangle());
         break;
   }
   /// TODO : Height/Width formulas OK???
   hpad = padding;
   
   tw = text_length(text_font , text.c_str());
   th = text_height(text_font);
   
   int totalheight = (btnh > th)?btnh:th;
   int totalwidth = btnw + hpad + tw;
   
   SetMinDimensions(totalwidth,totalheight);

   tx = area.X() + btnw + hpad;
   ty = area.Y() + (area.H() - th)/2;
   
   int btnx = area.X();
   int btny = area.Y() + (area.H() - btnh)/2;
   
   button->SetArea(btnx,btny,btnw,btnh);
   button->SetParent(this);
   button->UseWidgetColorset(wcols);
}



RadioButtonItem::~RadioButtonItem() {
   delete button;
   button = 0;
}



WidgetMsg RadioButtonItem::Update (double tsec) {
   return button->Update(tsec);
}


WidgetMsg RadioButtonItem::CheckInputs(int msx , int msy) {
   return button->CheckInputs(msx,msy);
}



void RadioButtonItem::DisplayOn(BITMAP* bmp , int x , int y) {
   ASSERT(bmp);
   button->DisplayOn(bmp,x,y);
   Rectangle r = area;
   r.MoveBy(x,y);
   Rectangle old_clip = GetClippingRectangle(bmp);
   r.SetClipRect(bmp);
   textout_ex(bmp , text_font , text.c_str() , tx + x , ty + y , (*wcols)[TXTCOL] , -1);
   old_clip.SetClipRect(bmp);
}



bool RadioButtonItem::Init() {
   return button->Init();
}



bool RadioButtonItem::Ready() {
   return button->Ready();
}



bool RadioButtonItem::Shutdown() {
   return button->Shutdown();
}



void RadioButtonItem::QueueUserMessage(const WidgetMsg& wmsg) {
   if (wmsg == WidgetMsg(button , TOPIC_BUTTON_WIDGET , BUTTON_TOGGLED)) {
      WidgetBase::QueueUserMessage(WidgetMsg(this , TOPIC_BUTTON_WIDGET , BUTTON_TOGGLED));
   } else {
      WidgetBase::QueueUserMessage(wmsg);
   }
}






void RadioButtonItem::SetWidgetColors(const WidgetColorset& colors) {
   WidgetBase::SetWidgetColors(colors);
   button->SetWidgetColors(colors);
}



void RadioButtonItem::UseWidgetColorset(WidgetColorset* colorset) {
   WidgetBase::UseWidgetColorset(colorset);
   button->UseWidgetColorset(colorset);
}



void RadioButtonItem::UseIndividualColorset() {
   WidgetBase::UseIndividualColorset();
   button->UseWidgetColorset(wcols);
}



void RadioButtonItem::SetDrawPos(int xpos , int ypos) {
   int dx = xpos - area.X();
   int dy = ypos - area.Y();
   tx += dx;
   ty += dy;
   button->SetDrawPos(button->Area().X() + dx , button->Area().Y() + dy);
   WidgetBase::SetDrawPos(xpos,ypos);
}



void RadioButtonItem::SetDrawDimensions(int width , int height) {
   WidgetBase::SetDrawDimensions(width,height);
   tx = area.X() + button->Area().W() + hpad;
   ty = area.Y() + (area.H() - th)/2;
   button->SetDrawPos(area.X() , area.Y() + (area.H() - button->Area().H())/2);
}



void RadioButtonItem::SetArea(int xpos , int ypos , int width , int height) {
   WidgetBase::SetArea(xpos,ypos,width,height);
   tx = area.X() + button->Area().W() + hpad;
   ty = area.Y() + (area.H() - th)/2;
   button->SetDrawPos(area.X() , area.Y() + (area.H() - button->Area().H())/2);
}



void RadioButtonItem::SetArea(const Rectangle& r) {
   WidgetBase::SetArea(r);
   tx = area.X() + button->Area().W() + hpad;
   ty = area.Y() + (area.H() - th)/2;
   button->SetDrawPos(area.X() , area.Y() + (area.H() - button->Area().H())/2);
}



bool RadioButtonItem::SetColorDepth(int color_depth) {
   bool success = true;
   if (!WidgetBase::SetColorDepth(color_depth)) {success = false;}
   if (!button->SetColorDepth(color_depth)) {success = false;}
   return success;
}



void RadioButtonItem::SetFont(FONT* textfont) {
   ASSERT(textfont);
   text_font = textfont;
   tw = text_length(text_font , text.c_str());
   th = text_height(text_font);
   SetMinDimensions(AbsMinWidth() , AbsMinHeight());
   SetBgRedrawFlag();
}



int RadioButtonItem::AbsMinWidth() {
   return (button->Area().W() + hpad + tw);
}



int RadioButtonItem::AbsMinHeight() {
   return (button->Area().H() > th)?button->Area().H():th;
}



std::string RadioButtonItem::Text() {
   return text;
}



void RadioButtonItem::SetButtonUpState(bool button_up) {
   button->SetButtonUpState(button_up);
}



bool RadioButtonItem::ButtonUpState() {
   return button->ButtonUpState();
}



/// ---------------------------     Generic Radio Button class     ---------------------------------------



void GenericRadioButton::Reposition() {
   int sx = area.X();
   int sy = area.Y();
   for (unsigned int i = 0 ; i < buttons.size() ; ++i) {
      WidgetBase* w = buttons[i];
      ASSERT(w);
      w->SetDrawPos(sx,sy);
      switch (radio_type) {
         case RADIO_HORIZONTAL :
            sx += w->Area().W() + padding;
            break;
         case RADIO_VERTICAL :
            sy += w->Area().H() + padding;
            break;
      }
   }
}



void GenericRadioButton::ResetMinDimensions() {
   SetMinDimensions(AbsMinWidth() , AbsMinHeight());
}



void GenericRadioButton::AdjustItemSizes() {
   int tw = area.W();
   int th = area.H();
   int num_items = (int)buttons.size();
   if (num_items) {
      int tpad = (num_items - 1)*padding;
      int w = 0;
      int h = 0;
      switch (radio_type) {
         case RADIO_HORIZONTAL :
            w = (tw - tpad)/num_items;
            h = th;
            break;
         case RADIO_VERTICAL :
            w = tw;
            h = (th - tpad)/num_items;
            break;
      }
      for (UINT i = 0 ; i < buttons.size() ; ++i) {
         WidgetBase* widget = buttons[i];
         widget->SetDrawDimensions(w,h);
      }
      Reposition();
   }
}



UINT GenericRadioButton::ItemIndex(Button* btn) {
   for (UINT i = 0 ; i < buttons.size() ; ++i) {
      if (buttons[i] == btn) {return i;}
   }
   return buttons.size();
}



bool GenericRadioButton::HasButton(Button* btn) {
   return (delete_map.find(btn) != delete_map.end());
}



GenericRadioButton::GenericRadioButton(std::string name , RADIO_TYPE rtype , Rectangle position , int pad , UINT wflags) :
      WidgetBase(name , position , wflags),
      buttons(),
      delete_map(),
      radio_type(rtype),
      padding(0),
      active_index(0),
      active_item(0),
      hover_item(0),
      delete_added_widgets(false),
      text_font(font)
{
   if (pad < 0) {pad = 0;}
   padding = pad;
}



WidgetMsg GenericRadioButton::Update(double tsec) {
   for (UINT i = 0 ; i < buttons.size() ; ++i) {
      buttons[i]->Update(tsec);
   }
   return WidgetMsg(this , TOPIC_DIALOG , DIALOG_OKAY);
}



WidgetMsg GenericRadioButton::CheckInputs(int msx , int msy) {
   UINT retmsg = DIALOG_OKAY;
   if (WidgetBase::flags & ENABLED) {
      Button* new_hover = 0;
      if (flags & HOVER) {
         for (UINT i = 0 ; i < buttons.size() ; ++i) {
            WidgetBase* w = buttons[i];
            if (w->Area().Contains(msx,msy)) {
               new_hover = buttons[i];
            }
         }
      }
      if (new_hover != hover_item) {
         if (hover_item) {
            hover_item->SetHoverState(false);
         }
         if (new_hover) {
            new_hover->SetHoverState(true);
         }
         hover_item = new_hover;
      }
      
      for (UINT i = 0 ; i < buttons.size() ; ++i) {
         WidgetBase* w = buttons[i];
         WidgetMsg msg = w->CheckInputs(msx,msy);
         retmsg |= msg.Messages();
      }
   }
   return WidgetMsg(this , TOPIC_DIALOG , retmsg);
}



void GenericRadioButton::DisplayOn(BITMAP* bmp , int x , int y) {
   if (WidgetBase::flags & VISIBLE) {
      for (UINT i = 0 ; i < buttons.size() ; ++i) {
         WidgetBase* w = buttons[i];
         w->DisplayOn(bmp,x,y);
      }
   }
   ClearRedrawFlag();
}



bool GenericRadioButton::Init() {
   bool init = true;
   for (UINT i = 0 ; i < buttons.size() ; ++i) {
      WidgetBase* w = buttons[i];
      if (!w->Init()) {init = false;}
   }
   return init;
}



bool GenericRadioButton::Ready() {
   bool ready = true;
   for (UINT i = 0 ; i < buttons.size() ; ++i) {
      WidgetBase* w = buttons[i];
      if (!w->Ready()) {ready = false;}
   }
   return ready;
}



bool GenericRadioButton::Shutdown() {
   bool shutdown = true;
   for (UINT i = 0 ; i < buttons.size() ; ++i) {
      WidgetBase* w = buttons[i];
      if (!w->Shutdown()) {shutdown = false;}
   }
   return shutdown;
}



void GenericRadioButton::QueueUserMessage(const WidgetMsg& wmsg) {
   OutputLog() << wname << "::QueueUserMessage received : " << wmsg << endl;
   if (wmsg.IsMessageTopic(TOPIC_BUTTON_WIDGET , BUTTON_TOGGLED)) {
      Button* btn = dynamic_cast<Button*>(wmsg.From());
      if (btn && HasButton(btn) && (btn != active_item)) {
         WidgetBase::QueueUserMessage(WidgetMsg(this , TOPIC_RADIO_BUTTON , RADIO_BTN_CLICKED));
      }
      SetActiveItem(btn);
   }
}



void GenericRadioButton::SetWidgetColors(const WidgetColorset& colors) {
   WidgetBase::SetWidgetColors(colors);
   for (UINT i = 0 ; i < buttons.size() ; ++i) {
      WidgetBase* w = buttons[i];
      w->SetWidgetColors(colors);
   }
}



void GenericRadioButton::UseWidgetColorset(WidgetColorset* colorset) {
   if (colorset) {
      wcols = colorset;
      for (UINT i = 0 ; i < buttons.size() ; ++i) {
         WidgetBase* w = buttons[i];
         w->UseWidgetColorset(colorset);
      }
   }
}



void GenericRadioButton::UseIndividualColorset() {
   WidgetBase::UseIndividualColorset();
   for (UINT i = 0 ; i < buttons.size() ; ++i) {
      WidgetBase* w = buttons[i];
      w->UseWidgetColorset(wcols);
   }
}



void GenericRadioButton::SetDrawPos(int xpos , int ypos) {
   int dx = xpos - area.X();
   int dy = ypos - area.Y();
   for (UINT i = 0 ; i < buttons.size() ; ++i) {
      WidgetBase* w = buttons[i];
      Rectangle r = w->Area();
      w->SetDrawPos(r.X() + dx , r.Y() + dy);
   }
   WidgetBase::SetDrawPos(xpos,ypos);
}



void GenericRadioButton::SetDrawDimensions(int width , int height) {
   WidgetBase::SetDrawDimensions(width,height);
   AdjustItemSizes();
}



void GenericRadioButton::SetArea(int xpos , int ypos , int width , int height) {
   SetDrawPos(xpos,ypos);
   SetDrawDimensions(width,height);
}



void GenericRadioButton::SetArea(const Rectangle& r) {
   SetDrawPos(r.X() , r.Y());
   SetDrawDimensions(r.W() , r.H());
}



int GenericRadioButton::AbsMinWidth() {
   int minwidth = 0;
   int tpad = ((int)buttons.size() - 1)*padding;
   if (tpad < 0) {tpad = 0;}
   int maxw = 0;
   for (UINT i = 0 ; i < buttons.size() ; ++i) {
      WidgetBase* widget = buttons[i];
      int w = widget->AbsMinWidth();
      if (w > maxw) {maxw = w;}
   }
   switch (radio_type) {
      case RADIO_HORIZONTAL :
         minwidth = maxw*((int)buttons.size()) + tpad;
         break;
      case RADIO_VERTICAL :
         minwidth = maxw;
         break;
   }
   if (minwidth == 0) {minwidth = 1;}
   return minwidth;
}



int GenericRadioButton::AbsMinHeight() {
   int minheight = 0;
   int tpad = ((int)buttons.size() - 1)*padding;
   if (tpad < 0) {tpad = 0;}
   int maxh = 0;
   for (UINT i = 0 ; i < buttons.size() ; ++i) {
      WidgetBase* w = buttons[i];
      int h = w->AbsMinHeight();
      if (h > maxh) {maxh = h;}
   }
   switch (radio_type) {
      case RADIO_HORIZONTAL :
         minheight = maxh;
         break;
      case RADIO_VERTICAL :
         minheight = maxh*((int)buttons.size()) + tpad;
         break;
   }
   if (minheight == 0) {minheight = 1;}
   return minheight;
}



void GenericRadioButton::AddItem(Button* btn , bool delete_when_done) {
   ASSERT(btn);
   if (!btn) {return;}
   
   map<Button* , bool>::iterator it = delete_map.find(btn);
   if (it == delete_map.end()) {
      buttons.push_back(btn);
      delete_map[btn] = delete_when_done;
      if (buttons.size() == 1) {
         SetActiveItem(btn);
      } else {
         btn->SetButtonUpState(true);
      }
      
      btn->SetParent(this);
      btn->UseWidgetColorset(wcols);
      
      RenewItemSizes();
   }
}



GenericRadioButton& GenericRadioButton::operator<<(Button* btn) {
   AddItem(btn , delete_added_widgets);
   return *this;
}



GenericRadioButton& GenericRadioButton::operator<<(MEMORY_MANAGEMENT_TYPE type) {
   switch (type) {
      case DO_NOT_DELETE : delete_added_widgets = false;break;
      case DELETE_WHEN_DONE : delete_added_widgets = true;break;
   }
   return *this;
}



void GenericRadioButton::RemoveItem(const std::string item_text) {
   for (UINT i = 0 ; i < buttons.size() ; ++i) {
      if (item_text == buttons[i]->Text()) {
         RemoveItem(buttons[i]);
      }
   }
}



void GenericRadioButton::RemoveItem(Button* btn) {
   map<Button* , bool>::iterator it = delete_map.find(btn);
   if (it != delete_map.end()) {
      if (it->second) {
         delete it->first;
      }
      delete_map.erase(it);
      for (vector<Button*>::iterator vit = buttons.begin() ; vit != buttons.end() ; ++vit) {
         if (*vit == btn) {
            buttons.erase(vit);
            break;
         }
      }
      if (btn == active_item) {
         if (buttons.size()) {
            SetActiveItem(buttons[0]);
         }
         else {
            active_item = 0;
         }
      }
      
   }
   
}



void GenericRadioButton::ClearItems() {
   for (map<Button* , bool>::iterator it = delete_map.begin() ; it != delete_map.end() ; ++it) {
      if (it->second) {
         delete it->first;
      }
   }
   buttons.clear();
   delete_map.clear();
   active_item = 0;
   ResetMinDimensions();
///   SetDrawDimensions(AbsMinWidth() , AbsMinHeight());/// TODO : Huh? Why do this? Dimensions should stay the same
}



void GenericRadioButton::SetActiveItem(Button* btn) {
   if (btn && HasButton(btn)) {
      for (UINT i = 0 ; i < buttons.size() ; ++i) {
         Button* btn2 = buttons[i];
         if (btn2 == btn) {
            btn2->SetButtonUpState(false);
            active_item = btn2;
         } else {
            btn2->SetButtonUpState(true);
         }
      }
      SetBgRedrawFlag();
   }
}



void GenericRadioButton::SetActiveItem(string item_text) {
   for (UINT i = 0 ; i < buttons.size() ; ++i) {
      Button* btn = buttons[i];
      if (btn->Text() == item_text) {
         SetActiveItem(btn);
         break;
      }
   }
}



void GenericRadioButton::SetActiveItem(UINT index) {
   if (index >= buttons.size()) {
      throw EagleError(StringPrintF("GenericRadioButton::SetActiveItem index (%u) out of bounds (%u)\n" , index , buttons.size()));
   }
   for (UINT i = 0 ; i < buttons.size() ; ++i) {
      Button* btn = buttons[i];
      if (i == index) {
         btn->SetButtonUpState(false);
         active_item = btn;
      }
      else {
         btn->SetButtonUpState(true);
      }
   }
   SetBgRedrawFlag();
}



void GenericRadioButton::SetFont(FONT* textfont) {
   ASSERT(textfont);
   if (textfont) {
      text_font = textfont;
      for (UINT i = 0 ; i < buttons.size() ; ++i) {
         Button* btn = buttons[i];
         btn->SetFont(text_font);
      }
      RenewItemSizes();
   }
}



void GenericRadioButton::RenewItemSizes() {
   ResetMinDimensions();
   AdjustItemSizes();
}



Button* GenericRadioButton::ActiveItemPointer() {
   return active_item;
}



string GenericRadioButton::ActiveItem() {
   if (active_item) {
      return active_item->Text();
   }
   return string("");
}



UINT GenericRadioButton::ActiveItemIndex() {
   return ItemIndex(active_item);
}



UINT GenericRadioButton::Size() {
   return buttons.size();
}



Button* GenericRadioButton::ItemIndexToButton(UINT index) {
   if (index < buttons.size()) {
      return buttons[index];
   }
   return 0;
}



void GenericRadioButton::PrintTo(std::ostream& os , Indenter& indent) const {
   os << indent << "GenericRadioButton {" << endl;
   ++indent;
   PrintBaseTo(os,indent);
   os << indent << "radio_type = " << ToString(radio_type) << " , padding = " << padding << " , active_item = " << active_item << endl;
   os << indent << "Buttons (Count = " << buttons.size() << ") {" << endl;
   ++indent;
   for (UINT i = 0 ; i < buttons.size() ; ++i) {
      os << indent << "Button #" << i << " {" << endl;
      ++indent;
      buttons[i]->PrintTo(os,indent);
      --indent;
      os << indent << "}" << endl;
   }
   --indent;
   os << indent << "}" << endl;
   --indent;
   os << indent << "}" << endl;
}



/// ---------------------------     Radio Button class     ---------------------------------------



void RadioButton::Reposition() {
   int sx = area.X();
   int sy = area.Y();
   for (unsigned int i = 0 ; i < items.size() ; ++i) {
      WidgetBase* w = dynamic_cast<WidgetBase*>(items[i]);
      ASSERT(w);
      w->SetDrawPos(sx,sy);
      switch (radio_type) {
         case RADIO_HORIZONTAL :
            sx += w->Area().W() + pad;
            break;
         case RADIO_VERTICAL :
            sy += w->Area().H() + pad;
            break;
      }
   }
}



void RadioButton::ResetMinDimensions() {
   SetMinDimensions(AbsMinWidth() , AbsMinHeight());
}



void RadioButton::AdjustItemSizes() {
   int tw = area.W();
   int th = area.H();
   int num_items = (int)items.size();
   if (num_items) {
      int tpad = (num_items - 1)*pad;
      int w = 0;
      int h = 0;
      switch (radio_type) {
         case RADIO_HORIZONTAL :
            w = (tw - tpad)/num_items;
            h = th;
            break;
         case RADIO_VERTICAL :
            w = tw;
            h = (th - tpad)/num_items;
            break;
      }
      for (UINT i = 0 ; i < items.size() ; ++i) {
         WidgetBase* widget = dynamic_cast<WidgetBase*>(items[i]);
         widget->SetDrawDimensions(w,h);
      }
      Reposition();
   }
}



UINT RadioButton::ItemIndex(RadioButtonItemBase* item) {
   for (UINT i = 0 ; i < items.size() ; ++i) {
      if (items[i] == item) {return i;}
   }
   return items.size();
}



bool RadioButton::HasButton(RadioButtonItemBase* item) {
   return (delete_map.find(item) != delete_map.end());
}



RadioButton::RadioButton(std::string name , RADIO_TYPE rtype , Rectangle position , int padding , UINT wflags) :
      WidgetBase(name , position , wflags),
      pad(0),
      radio_type(rtype),
      text_font(0),
      items(),
      delete_map(),
      active_item(0),
      hover_item(0)
{
   if (padding < 0) {padding = 0;}
   pad = padding;
}



WidgetMsg RadioButton::Update(double tsec) {
   return WidgetMsg(this , TOPIC_DIALOG , DIALOG_OKAY);
}



WidgetMsg RadioButton::CheckInputs(int msx , int msy) {
   UINT retmsg = DIALOG_OKAY;
   if (WidgetBase::flags & ENABLED) {
      RadioButtonItemBase* new_hover = 0;
      if (flags & HOVER) {
         for (UINT i = 0 ; i < items.size() ; ++i) {
            WidgetBase* w = dynamic_cast<WidgetBase*>(items[i]);
            ASSERT(w);
            if (w->Area().Contains(msx,msy)) {
               new_hover = items[i];
            }
         }
      }
      if (new_hover != hover_item) {
         if (hover_item) {
            WidgetBase* w = dynamic_cast<WidgetBase*>(hover_item);
            ASSERT(w);
            w->SetHoverState(false);
         }
         if (new_hover) {
            WidgetBase* w = dynamic_cast<WidgetBase*>(new_hover);
            ASSERT(w);
            w->SetHoverState(true);
         }
         hover_item = new_hover;
      }
      
      for (UINT i = 0 ; i < items.size() ; ++i) {
         WidgetBase* w = dynamic_cast<WidgetBase*>(items[i]);
         ASSERT(w);
         WidgetMsg msg = w->CheckInputs(msx,msy);
         retmsg |= msg.Messages();
//         if (retmsg & DIALOG_REDRAW_ME) {
//            SetRedrawFlag();
//         }
      }
   }
   return WidgetMsg(this , TOPIC_DIALOG , retmsg);
}



void RadioButton::DisplayOn(BITMAP* bmp , int x , int y) {
   if (WidgetBase::flags & VISIBLE) {
      for (UINT i = 0 ; i < items.size() ; ++i) {
         WidgetBase* w = dynamic_cast<WidgetBase*>(items[i]);
         ASSERT(w);
         w->DisplayOn(bmp,x,y);
      }
   }
   ClearRedrawFlag();
}



bool RadioButton::Init() {
   bool init = true;
   for (UINT i = 0 ; i < items.size() ; ++i) {
      WidgetBase* w = dynamic_cast<WidgetBase*>(items[i]);
      ASSERT(w);
      if (!w->Init()) {init = false;}
   }
   return init;
}



bool RadioButton::Ready() {
   bool ready = true;
   for (UINT i = 0 ; i < items.size() ; ++i) {
      WidgetBase* w = dynamic_cast<WidgetBase*>(items[i]);
      ASSERT(w);
      if (!w->Ready()) {ready = false;}
   }
   return ready;
}



bool RadioButton::Shutdown() {
   bool shutdown = true;
   for (UINT i = 0 ; i < items.size() ; ++i) {
      WidgetBase* w = dynamic_cast<WidgetBase*>(items[i]);
      ASSERT(w);
      if (!w->Shutdown()) {shutdown = false;}
   }
   return shutdown;
}



void RadioButton::QueueUserMessage(const WidgetMsg& wmsg) {
   OutputLog() << wname << "::QueueUserMessage received : " << wmsg << endl;
   if (wmsg.IsMessageTopic(TOPIC_BUTTON_WIDGET , BUTTON_TOGGLED)) {
      RadioButtonItemBase* rbi = dynamic_cast<RadioButtonItemBase*>(wmsg.From());
      OutputLog() << "   rbi is " << rbi << endl;
      if (rbi && HasButton(rbi) && (rbi != active_item)) {
         WidgetBase::QueueUserMessage(WidgetMsg(this , TOPIC_RADIO_BUTTON , RADIO_BTN_CLICKED));
      }
      SetActiveItem(rbi);
      /// Then next line does the same thing as the previous lines, but it doesn't work for some reason
      //WidgetBase::QueueUserMessage(this , TOPIC_RADIO_BUTTON , RADIO_BTN_CLICKED);
//      OutputLog() << "Queueing user message of [ " << WidgetMsg(this , TOPIC_RADIO_BUTTON , RADIO_BTN_CLICKED) << " ]" << endl;
   }
}



void RadioButton::SetWidgetColors(const WidgetColorset& colors) {
   WidgetBase::SetWidgetColors(colors);
   for (UINT i = 0 ; i < items.size() ; ++i) {
      WidgetBase* w = dynamic_cast<WidgetBase*>(items[i]);
      ASSERT(w);
      w->SetWidgetColors(colors);
   }
}



void RadioButton::UseWidgetColorset(WidgetColorset* colorset) {
   if (colorset) {
      wcols = colorset;
      for (UINT i = 0 ; i < items.size() ; ++i) {
         WidgetBase* w = dynamic_cast<WidgetBase*>(items[i]);
         ASSERT(w);
         w->UseWidgetColorset(colorset);
      }
   }
}



void RadioButton::UseIndividualColorset() {
   WidgetBase::UseIndividualColorset();
   for (UINT i = 0 ; i < items.size() ; ++i) {
      WidgetBase* w = dynamic_cast<WidgetBase*>(items[i]);
      ASSERT(w);
      w->UseWidgetColorset(wcols);
   }
}



void RadioButton::SetDrawPos(int xpos , int ypos) {
   int dx = xpos - area.X();
   int dy = ypos - area.Y();
   for (UINT i = 0 ; i < items.size() ; ++i) {
      WidgetBase* w = dynamic_cast<WidgetBase*>(items[i]);
      ASSERT(w);
      Rectangle r = w->Area();
      w->SetDrawPos(r.X() + dx , r.Y() + dy);
   }
   WidgetBase::SetDrawPos(xpos,ypos);
}



void RadioButton::SetDrawDimensions(int width , int height) {
   WidgetBase::SetDrawDimensions(width,height);
   AdjustItemSizes();
}



void RadioButton::SetArea(int xpos , int ypos , int width , int height) {
   SetDrawPos(xpos,ypos);
   SetDrawDimensions(width,height);
}



void RadioButton::SetArea(const Rectangle& r) {
   SetDrawPos(r.X() , r.Y());
   SetDrawDimensions(r.W() , r.H());
}



int RadioButton::AbsMinWidth() {
   int minwidth = 0;
   int tpad = ((int)items.size() - 1)*pad;
   if (tpad < 0) {tpad = 0;}
   int maxw = 0;
   for (UINT i = 0 ; i < items.size() ; ++i) {
      WidgetBase* widget = dynamic_cast<WidgetBase*>(items[i]);
      ASSERT(widget);
      int w = widget->AbsMinWidth();
      if (w > maxw) {maxw = w;}
   }
   switch (radio_type) {
      case RADIO_HORIZONTAL :
         minwidth = maxw*((int)items.size()) + tpad;
         break;
      case RADIO_VERTICAL :
         minwidth = maxw;
         break;
   }
   if (minwidth == 0) {minwidth = 1;}
   return minwidth;
}



int RadioButton::AbsMinHeight() {
   int minheight = 0;
   int tpad = ((int)items.size() - 1)*pad;
   if (tpad < 0) {tpad = 0;}
   int maxh = 0;
   for (UINT i = 0 ; i < items.size() ; ++i) {
      WidgetBase* w = dynamic_cast<WidgetBase*>(items[i]);
      ASSERT(w);
      int h = w->AbsMinHeight();
      if (h > maxh) {maxh = h;}
   }
   switch (radio_type) {
      case RADIO_HORIZONTAL :
         minheight = maxh;
         break;
      case RADIO_VERTICAL :
         minheight = maxh*((int)items.size()) + tpad;
         break;
   }
   if (minheight == 0) {minheight = 1;}
   return minheight;
}



void RadioButton::AddItem(RadioButtonItemBase* rbtn , bool delete_when_done) {
   if (!rbtn) {return;}
   WidgetBase* w = dynamic_cast<WidgetBase*>(rbtn);
   if (!w) {
      OutputLog() << "Radio Button Items must also be derived from WidgetBase!!! ABORT! ABORT! Ayuuugah!" << endl;
      ASSERT(0);
      throw EagleError("Radio Button Items must also be derived from WidgetBase!!! ABORT! ABORT! Ayuuugah!");
   }
   
   map<RadioButtonItemBase* , bool>::iterator it = delete_map.find(rbtn);
   if (it == delete_map.end()) {
      items.push_back(rbtn);
      delete_map[rbtn] = delete_when_done;
      if (items.size() == 1) {
         SetActiveItem(rbtn);
      } else {
         rbtn->SetButtonUpState(true);
      }
      
      w->SetParent(this);
      w->UseWidgetColorset(wcols);
      
      RenewItemSizes();
   }
}



RadioButton& RadioButton::operator<<(RadioButtonItemBase* rbtn) {
   AddItem(rbtn , false);
   return *this;
}



void RadioButton::RemoveItem(const std::string item_text) {
   for (UINT i = 0 ; i < items.size() ; ++i) {
      if (item_text == items[i]->Text()) {
         RemoveItem(items[i]);
      }
   }
}



void RadioButton::RemoveItem(RadioButtonItemBase* rbtn) {
   map<RadioButtonItemBase* , bool>::iterator it = delete_map.find(rbtn);
   if (it != delete_map.end()) {
      if (it->second) {
         delete it->first;
      }
      delete_map.erase(it);
      for (vector<RadioButtonItemBase*>::iterator vit = items.begin() ; vit != items.end() ; ++vit) {
         if (*vit == rbtn) {
            items.erase(vit);
            break;
         }
      }
      if (rbtn == active_item) {
         if (items.size()) {SetActiveItem(items[0]);} else {active_item = 0;}
      }
      
   }
   
}



void RadioButton::ClearItems() {
   for (map<RadioButtonItemBase* , bool>::iterator it = delete_map.begin() ; it != delete_map.end() ; ++it) {
      if (it->second) {
         delete it->first;
      }
   }
   items.clear();
   delete_map.clear();
   active_item = 0;
   ResetMinDimensions();
   SetDrawDimensions(AbsMinWidth() , AbsMinHeight());
}



void RadioButton::SetActiveItem(RadioButtonItemBase* item) {
   if (item && HasButton(item)) {
      for (UINT i = 0 ; i < items.size() ; ++i) {
         RadioButtonItemBase* rbtn = items[i];
         if (rbtn == item) {
            rbtn->SetButtonUpState(false);
            active_item = rbtn;
         } else {
            rbtn->SetButtonUpState(true);
         }
      }
      SetBgRedrawFlag();
   }
}



void RadioButton::SetActiveItem(string item_text) {
   for (UINT i = 0 ; i < items.size() ; ++i) {
      RadioButtonItemBase* rbtn = items[i];
      if (rbtn->Text() == item_text) {
         SetActiveItem(rbtn);
         break;
      }
   }
}



void RadioButton::SetActiveItem(UINT index) {
   if (index >= items.size()) {
      throw EagleError(StringPrintF("RadioButton::SetActiveItem index (%u) out of bounds (%u)\n" , index , items.size()));
   }
   for (UINT i = 0 ; i < items.size() ; ++i) {
      RadioButtonItemBase* rbtn = items[i];
      if (i == index) {
         rbtn->SetButtonUpState(false);
         active_item = rbtn;
      }
      else {
         rbtn->SetButtonUpState(true);
      }
   }
   SetBgRedrawFlag();
}



void RadioButton::SetFont(FONT* textfont) {
   ASSERT(textfont);
   if (textfont) {
      text_font = textfont;
      for (UINT i = 0 ; i < items.size() ; ++i) {
         WidgetBase* w = dynamic_cast<WidgetBase*>(items[i]);
         ASSERT(w);
         w->SetFont(text_font);
      }
      RenewItemSizes();
   }
}



void RadioButton::RenewItemSizes() {
   ResetMinDimensions();
   AdjustItemSizes();
}



RadioButtonItemBase* RadioButton::ActiveItemPointer() {
   return active_item;
}



string RadioButton::ActiveItem() {
   if (active_item) {
      return active_item->Text();
   }
   return string("");
}



UINT RadioButton::ActiveItemIndex() {
   return ItemIndex(active_item);
}



UINT RadioButton::Size() {
   return items.size();
}








