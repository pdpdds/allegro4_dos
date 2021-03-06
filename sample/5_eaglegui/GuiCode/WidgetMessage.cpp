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


#include "Eagle/GuiCode/WidgetMessage.hpp"
#include "Eagle/GuiCode.hpp"
#include "Eagle/StringWork.hpp"

using std::ostream;
using std::string;


#define NONE 0



unsigned int NextFreeTopicId() {
   static unsigned int id = NONE;
   id += 1;
   return id;
}



void (*TopicMessageStringsCB) (unsigned int topic , int msgs , string* store_topic_str , string* store_msgs_str , bool* store_handled);



void SetTopicAndMessageToStringsCallback(void (*callback)(unsigned int , int , std::string* , std::string* , bool*)) {
   TopicMessageStringsCB = callback;
}




WidgetMsg::WidgetMsg() : from(0) , topic(NONE) , msgs(NONE)
   {}



WidgetMsg::WidgetMsg(WidgetBase* widget_address , UINT widget_topic , int message) :
      from(widget_address) , topic(widget_topic) , msgs(message)
   {}



WidgetMsg::WidgetMsg(const WidgetMsg& wmsg) : from(wmsg.from) , topic(wmsg.topic) , msgs(wmsg.msgs)
   {}



ostream& operator<<(ostream& os , const WidgetMsg& wmsg) {
   WidgetBase* w = wmsg.from;
   string wname = "Null widget";
   string wtopic = "";
   string wmessage = "";
   bool handled = false;
   
   UINT topic = wmsg.topic;
   int msgs = wmsg.msgs;
   
   if (w) {wname = w->Name();}
   
   if (TopicMessageStringsCB) {
      TopicMessageStringsCB(wmsg.topic , wmsg.msgs , &wtopic , &wmessage , &handled);
   }
   if (!handled) {
      wtopic = "Unknown topic";
      wmessage = "Unknown message";
/*
TODO : Make sure this list is current, use grep -r -E -I -n "extern.*TOPIC" .\*.* in the eagle/include folder

.\Eagle/GuiCode/Animation.hpp:45:extern const unsigned int 
TOPIC_ANIMATION;
.\Eagle/GuiCode/Animation.hpp:47:extern const unsigned int 
TOPIC_STATE_ANIMATION;
.\Eagle/GuiCode/BitmapViewer.hpp:31:extern const unsigned int 
TOPIC_BITMAP_VIEWER;
.\Eagle/GuiCode/ButtonBaseWidget.hpp:34:extern const unsigned int 
TOPIC_BUTTON_WIDGET;
.\Eagle/GuiCode/Camera.hpp:35:extern const unsigned int 
TOPIC_CAMERA;
.\Eagle/GuiCode/Camera.hpp:42:extern const unsigned int 
TOPIC_ZOOM_CAMERA;
.\Eagle/GuiCode/DropDownList.hpp:37:extern const unsigned int 
TOPIC_DDL;
.\Eagle/GuiCode/FileSelectorDialog.hpp:53:extern const unsigned int 
TOPIC_FOLDER_ITEM;
.\Eagle/GuiCode/FileSelectorDialog.hpp:60:extern const unsigned int 
TOPIC_FOLDER_TREE;
.\Eagle/GuiCode/FileSelectorDialog.hpp:66:extern const unsigned int 
TOPIC_FILE_SELECTOR;
.\Eagle/GuiCode/GfxModeDialog.hpp:129:extern const unsigned int 
TOPIC_GFX_MODE_DIALOG;
.\Eagle/GuiCode/ListBox.hpp:36:extern const unsigned int 
TOPIC_LISTBOX;
.\Eagle/GuiCode/MenuWidget.hpp:38:extern const unsigned int 
TOPIC_MENU_ITEM;
.\Eagle/GuiCode/RadioButtonWidget.hpp:37:extern const unsigned int 
TOPIC_RADIO_BUTTON;
.\Eagle/GuiCode/ScrollBarWidget.hpp:30:extern const unsigned int 
TOPIC_SCROLLER;
.\Eagle/GuiCode/ScrollBarWidget.hpp:37:extern const unsigned int 
TOPIC_SCROLLBAR;
.\Eagle/GuiCode/SliderWidget.hpp:31:extern const unsigned int 
TOPIC_SLIDER;
.\Eagle/GuiCode/TextInput.hpp:34:extern const unsigned int 
TOPIC_TEXT_INPUT;
.\Eagle/GuiCode/TitleBar.hpp:33:extern const unsigned int 
TOPIC_MINIMIZE_BUTTON;
.\Eagle/GuiCode/TitleBar.hpp:40:extern const unsigned int 
TOPIC_CLOSE_BUTTON;
.\Eagle/GuiCode/TitleBar.hpp:46:extern const unsigned int 
TOPIC_TITLE_BAR;
.\Eagle/GuiCode/WidgetBase.hpp:37:extern const unsigned int 
TOPIC_DIALOG;
.\Eagle/GuiCode/WidgetHandler.hpp:42:extern const unsigned int 
TOPIC_GUI;
*/
      if (topic == TOPIC_ANIMATION) {
         wtopic = "TOPIC_ANIMATION";
         wmessage = StringPrintF("Loop number %d" , msgs);
      } else if (topic == TOPIC_STATE_ANIMATION) {
         wtopic = "TOPIC_STATE_ANIMATION";
         wmessage = ((msgs == -1)?"Finished queued animations":"Unknown message");
      } else if (topic == TOPIC_BITMAP_VIEWER) {
         wtopic = "TOPIC_BITMAP_VIEWER";
         switch (msgs) {
            case BITMAP_VIEW_CHANGED : wmessage = "BITMAP_VIEW_CHANGED";break;
            default : wmessage = "UNKNOWN";break;
         }
      } else if (topic == TOPIC_BUTTON_WIDGET) {
         wtopic = "TOPIC_BUTTON_WIDGET";
         switch (msgs) {
            case BUTTON_CLICKED : wmessage = "BUTTON_CLICKED";break;
            case BUTTON_HELD : wmessage = "BUTTON_HELD";break;
            case BUTTON_RELEASED : wmessage = "BUTTON_RELEASED";break;
            case BUTTON_TOGGLED : wmessage = "BUTTON_TOGGLED";break;
            default : wmessage = "UNKNOWN";break;
         }
      } else if (topic == TOPIC_CAMERA) {
         wtopic = "TOPIC_CAMERA";
         switch (msgs) {
            case CAMERA_VIEW_MOVED : wmessage = "CAMERA_VIEW_MOVED";break;
            case CAMERA_VIEW_DEST_REACHED : wmessage = "CAMERA_VIEW_DEST_REACHED";break;
            default : wmessage = "UNKNOWN";break;
         }
      } else if (topic == TOPIC_ZOOM_CAMERA) {
         wtopic = "TOPIC_ZOOM_CAMERA";
         switch (msgs) {
            case CAMERA_ZOOM_CHANGED : wmessage = "CAMERA_ZOOM_CHANGED";break;
            case CAMERA_ZOOM_DEST_REACHED : wmessage = "CAMERA_ZOOM_DEST_REACHED";break;
            default : wmessage = "UNKNOWN";break;
         }
      } else if (topic == TOPIC_DDL) {
         wtopic = "TOPIC_DDL";
         switch (msgs) {
            case DDL_SELECTION_CHANGED : wmessage = "DDL_SELECTION_CHANGED";break;
            default : wmessage = "UNKNOWN";break;
         }
      } else if (topic == TOPIC_FOLDER_ITEM) {
         wtopic = "TOPIC_FOLDER_ITEM";
         switch (msgs) {
            case FOLDER_ITEM_SELECTED : wmessage = "FOLDER_ITEM_SELECTED";break;
            case FOLDER_VIEW_TOGGLED : wmessage = "FOLDER_VIEW_TOGGLED";break;
            default : wmessage = "UNKNOWN";
         }
      } else if (topic == TOPIC_FOLDER_TREE) {
         wtopic = "TOPIC_FOLDER_TREE";
         switch (msgs) {
            case FOLDER_SELECTED : wmessage = "FOLDER_SELECTED";break;
            default : wmessage = "UNKNOWN";break;
         }
      } else if (topic == TOPIC_FILE_SELECTOR) {
         wtopic = "TOPIC_FILE_SELECTOR";
         switch (msgs) {
            case FILE_OKAYED : wmessage = "FILE_OKAYED";break;
            case FILE_CANCELLED : wmessage = "FILE_CANCELLED";break;
            default : wmessage = "UNKNOWN";break;
         }
      } else if (topic == TOPIC_GFX_MODE_DIALOG) {
         wtopic = "TOPIC_GFX_MODE_DIALOG";
         switch (msgs) {
            case GFX_MODE_OKAYED : wmessage = "GFX_MODE_OKAYED";break;
            case GFX_MODE_CANCELLED : wmessage = "GFX_MODE_CANCELLED";break;
            default : wmessage = "UNKNOWN";break;
         }
      } else if (topic == TOPIC_LISTBOX) {
         wtopic = "TOPIC_LISTBOX";
         switch (msgs) {
            case LISTBOX_SELECTION_CHANGED : wmessage = "LISTBOX_SELECTION_CHANGED";break; 
            case LISTBOX_DOUBLE_CLICK_SELECT : wmessage = "LISTBOX_DOUBLE_CLICK_SELECT";break;
            default : wmessage = "UNKNOWN";break;
         }
      } else if (topic == TOPIC_MENU_ITEM) {
         wtopic = "TOPIC_MENU_ITEM";
         switch (msgs) {
            case MENU_ITEM_CLICKED : wmessage = "MENU_ITEM_CLICKED";break;
            default : wmessage = "UNKNOWN";break;
         }
      } else if (topic == TOPIC_RADIO_BUTTON) {
         wtopic = "TOPIC_RADIO_BUTTON";
         switch (msgs) {
            case RADIO_BTN_CLICKED : wmessage = "RADIO_BTN_CLICKED";break;
            default : wmessage = "UNKNOWN";break;
         }
      } else if (topic == TOPIC_SCROLLER) {
         wtopic = "TOPIC_SCROLLER";
         switch (msgs) {
            case SCROLLER_VALUE_CHANGED : wmessage = "SCROLLER_VALUE_CHANGED";break;
            default : wmessage = "UNKNOWN";break;
         }
      } else if (topic == TOPIC_SCROLLBAR) {
         wtopic = "TOPIC_SCROLLBAR";
         switch (msgs) {
            case SCROLLBAR_VALUE_CHANGED : wmessage = "SCROLLBAR_VALUE_CHANGED";break;
            default : wmessage = "UNKNOWN";break;
         }
      } else if (topic == TOPIC_SLIDER) {
         wtopic = "TOPIC_SLIDER";
         switch (msgs) {
            case SLIDER_VALUE_CHANGED : wmessage = "SLIDER_VALUE_CHANGED";break;
            default : wmessage = "UNKNOWN";break;
         }
      } else if (topic == TOPIC_TEXT_INPUT) {
         wtopic = "TOPIC_TEXT_INPUT";
         switch (msgs) {
            case TEXT_VALUE_CHANGED : wmessage = "TEXT_VALUE_CHANGED";break;
            case TEXT_VALUE_ENTERED : wmessage = "TEXT_VALUE_ENTERED";break;
            default : wmessage = "UNKNOWN";break;
         }
      } else if (topic == TOPIC_MINIMIZE_BUTTON) {
         wtopic = "TOPIC_MINIMIZE_BUTTON";
         switch (msgs) {
            case MINIMIZED : wmessage = "MINIMIZED";break;
            case MAXIMIZED : wmessage = "MAXIMIZED";break;
            default : wmessage = "UNKNOWN";break;
         }
      } else if (topic == TOPIC_CLOSE_BUTTON) {
         wtopic = "TOPIC_CLOSE_BUTTON";
         switch (msgs) {
            case SIGNAL_CLOSE : wmessage = "SIGNAL_CLOSE";break;
            default : wmessage = "UNKNOWN";break;
         }
      } else if (topic == TOPIC_TITLE_BAR) {
         wtopic = "TOPIC_TITLE_BAR";
         switch (msgs) {
            case TITLE_MINIMIZED : wmessage = "TITLE_MINIMIZED";break;
            case TITLE_MAXIMIZED : wmessage = "TITLE_MAXIMIZED";break;
            case TITLE_SIGNAL_CLOSE : wmessage = "TITLE_SIGNAL_CLOSE";break;
            default : wmessage = "UNKNOWN";break;
         }
      } else if (topic == TOPIC_DIALOG) {
         wtopic = "TOPIC_DIALOG";
         if (msgs == DIALOG_OKAY) {
            wmessage = "DIALOG_OKAY";
         } else {
            wmessage = StringPrintF("DIALOG_CLOSE(%s) , DIALOG_REDRAW_ME(%s) , DIALOG_REDRAW_ALL(%s) , "
                                    "DIALOG_INPUT_USED(%s) , DIALOG_TAKE_FOCUS(%s) , DIALOG_DISABLED(%s) , DIALOG_REMOVE_ME(%s)",
                                    ((msgs & DIALOG_CLOSE)?"Y":"N"),((msgs & DIALOG_REDRAW_ME)?"Y":"N"),
                                    ((msgs & DIALOG_REDRAW_ALL)?"Y":"N"),((msgs & DIALOG_INPUT_USED)?"Y":"N"),
                                    ((msgs & DIALOG_TAKE_FOCUS)?"Y":"N"),((msgs & DIALOG_DISABLED)?"Y":"N"),
                                    ((msgs & DIALOG_REMOVE_ME)?"Y":"N"));
         }
      } else if (topic == TOPIC_GUI) {
         wtopic = "TOPIC_GUI";
         switch (msgs) {
            case GUI_RESIZED : wmessage = "GUI_RESIZED";break;
            default : wmessage = "UNKNOWN";break;
         }
      }
   }
   os << "WidgetMsg : From widget (" << w << ") , Name = " << wname << " , Topic = " << wtopic << " , Message = " << wmessage;
   return os;
}





