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


#include <cstdlib>

#include "Eagle/GuiCode/TextInput.hpp"
#include "Eagle/InputHandler.hpp"
#include "Eagle/Error.hpp"

using std::string;



const unsigned int TOPIC_TEXT_INPUT = NextFreeTopicId();



const char* ToString(TEXT_INPUT_TYPE type) {
   switch (type) {
      case TEXT_INPUT : return "TEXT_INPUT";
      case INTEGER_INPUT : return "INTEGER_INPUT";
      case FLOATING_POINT_INPUT : return "FLOATING_POINT_INPUT";
   }
   return "Unknown TEXT_INPUT_TYPE";
}



/// ----------------------------------  TextInput class   ------------------------------------------



//bool EditParser(string& str , int key_read , UINT& caretp , bool& insert_mode ,
//                bool& selection_made , UINT& begin_select , UINT& end_select) {
bool TextInput::EditParser(int key_read) {
   /// Keys monitored are BACKSPACE , DELETE , (shift) LEFT , (shift) RIGHT , HOME , END , INSERT
   int scancode = key_read >> 8;
   bool key_used = false;
   
   UINT start_select = (begin_select < end_select)?begin_select:end_select;
   UINT finish_select = (begin_select > end_select)?begin_select:end_select;
   UINT select_diff = finish_select - start_select;
   switch (scancode) {
      case KEY_BACKSPACE :
         if (selection_made) {
            str.erase(start_select , select_diff);
            selection_made = false;
            if ((int)start_select >= char_scroll) {
               if ((char_scroll + chars_wide) > (int)str.size()) {
                  char_scroll = str.size() - chars_wide;
               }
            } else {
               char_scroll -= select_diff;
            }
            caretp = start_select;
            begin_select = end_select = caretp;
         } else {
            if (str.size() && caretp >= 1) {
               str.erase(caretp - 1 , 1);
               if ((int)caretp > char_scroll) {
                  if ((char_scroll + chars_wide) > (int)str.size()) {
                     char_scroll = str.size() - chars_wide;
                  }
               } else {
                  --char_scroll;
               }
               --caretp;
            }
         }
         key_used = true;
         break;
      case KEY_DEL :
         if (selection_made) {
            str.erase(start_select , select_diff);
            selection_made = false;
            if ((int)start_select >= char_scroll) {
               if ((char_scroll + chars_wide) > (int)str.size()) {
                  char_scroll = str.size() - chars_wide;
               }
            } else {
               char_scroll -= select_diff;
            }
            caretp = start_select;
            begin_select = end_select = caretp;
         } else {
            if (str.size() && caretp < str.size()) {
               str.erase(caretp , 1);
               if ((char_scroll + chars_wide) > (int)str.size()) {
                  char_scroll = str.size() - chars_wide;
               }
            }
         }
         key_used = true;
         break;
      case KEY_LEFT :
         if (input_key_held(KEY_ANY_SHIFT)) {
            if (caretp > 0) {
               if ((int)caretp == char_scroll) {
                  --char_scroll;
               }
               --end_select;
               --caretp;
               selection_made = (begin_select != end_select);
            }
         } else {
            if (caretp > 0) {
               if ((int)caretp == char_scroll) {
                  --char_scroll;
               }
               --caretp;
               selection_made = false;
               begin_select = end_select = caretp;
            }
         }
         key_used = true;
         break;
      case KEY_RIGHT :
         if (input_key_held(KEY_ANY_SHIFT)) {
            if (caretp < str.size()) {
               if ((int)caretp == (char_scroll + chars_wide)) {
                  ++char_scroll;
               }
               ++end_select;
               ++caretp;
               selection_made = (begin_select != end_select);
            }
         } else {
            if (caretp < str.size()) {
               if ((int)caretp == (char_scroll + chars_wide)) {
                  ++char_scroll;
               }
               ++caretp;
               selection_made = false;
               begin_select = end_select = caretp;
            }
         }
         key_used = true;
         break;
      case KEY_HOME :
         caretp = 0;
         if ((int)str.size() < chars_wide) {
            char_scroll = (int)str.size() - chars_wide;
         } else {
            char_scroll = 0;
         }
         selection_made = false;
         begin_select = end_select = caretp;
         key_used = true;
         break;
      case KEY_END :
         caretp = str.size();
         char_scroll = (int)str.size() - chars_wide;
         selection_made = false;
         begin_select = end_select = caretp;
         key_used = true;
         break;
      case KEY_INSERT :
         insert_mode = !insert_mode;
         key_used = true;
         break;
   }
   if (key_used) {ResetCaret();}
   return key_used;
}



bool TextInput::TextParser(int key_read) {
   bool key_used = EditParser(key_read);
   if (!key_used) {
      int ascii = key_read & 0xff;
      if (!iscntrl(ascii)) {
         key_used = true;
         char ch[2] = {(char)ascii , '\0'};
         if (selection_made) {
            UINT start_select = (begin_select < end_select)?begin_select:end_select;
            UINT finish_select = (begin_select > end_select)?begin_select:end_select;
            UINT select_diff = finish_select - start_select;
            str.erase(start_select , select_diff);
            selection_made = false;
            if ((int)start_select >= char_scroll) {
               if ((char_scroll + chars_wide) > (int)str.size()) {
                  char_scroll = str.size() - chars_wide;
               }
            } else {
               char_scroll -= select_diff;
            }
            caretp = start_select;
            begin_select = end_select = caretp;
            str.insert(caretp , ch);
            ++char_scroll;
            ++caretp;
         } else {
            if (insert_mode) {
               if ((int)str.size() < max_char) {
                  str.insert(caretp , ch);
                  ++char_scroll;
                  ++caretp;
               }
            } else {
               if ((int)caretp < max_char) {
                  //string& replace( size_type index, size_type num1, size_type num2, char ch );
                  //replaces up to num1 characters in the current string (beginning at index) with num2 copies of ch,
                  str.replace(caretp , 1 , 1 , (char)ascii);
                  if ((int)caretp == (char_scroll + chars_wide)) {
                     ++char_scroll;
                  }
                  ++caretp;
               }
            }
         }
      }
   }
   return key_used;
}



//bool IntegerParser(string& str , int key_read , UINT& caretp , bool& insert_mode,
//                bool& selection_made , UINT& begin_select , UINT& end_select);
bool TextInput::IntegerParser(int key_read) {
   // [-][123]
   int ascii = key_read & 0xff;
//   bool key_used = EditParser(str , key_read , caretp , insert_mode , selection_made , begin_select , end_select);
   bool key_used = EditParser(key_read);
   if (!key_used && ((ascii == '-') || isdigit(ascii))) {
      UINT start_select = (begin_select < end_select)?begin_select:end_select;
      UINT finish_select = (begin_select > end_select)?begin_select:end_select;
      UINT select_diff = finish_select - start_select;
      
      if ((ascii == '-') && ((caretp == 0) || (selection_made && (start_select == 0)))) {
         char ch[2] = {'-' , '\0'};
         if (selection_made) {
            str.erase(start_select , select_diff);
            selection_made = false;
            if ((int)start_select >= char_scroll) {
               if ((char_scroll + chars_wide) > (int)str.size()) {
                  char_scroll = str.size() - chars_wide;
               }
            } else {
               char_scroll -= select_diff;
            }
            caretp = start_select;
            begin_select = end_select = caretp;
            str.insert(caretp , ch);
            ++char_scroll;
            ++caretp;
         } else {
            if (insert_mode) {
               if ((int)str.size() < max_char) {
                  if ((str.size() == 0) || (str.size() && str.at(0) != '-')) {
                     str.insert(caretp , ch);
                     ++char_scroll;
                     ++caretp;
                  }
               }
            } else {
               str.replace(caretp , 1 , 1 , '-');
               if ((int)caretp == (char_scroll + chars_wide)) {
                  ++char_scroll;
               }
               ++caretp;
            }
         }
      }
      if (isdigit(ascii)) {
         char ch[2] = {(char)ascii , '\0'};
         if (selection_made) {
            str.erase(start_select , select_diff);
            selection_made = false;
            if ((int)start_select >= char_scroll) {
               if ((char_scroll + chars_wide) > (int)str.size()) {
                  char_scroll = str.size() - chars_wide;
               }
            } else {
               char_scroll -= select_diff;
            }
            caretp = start_select;
            begin_select = end_select = caretp;
            str.insert(caretp , ch);
            ++char_scroll;
            ++caretp;
         } else {
            if (insert_mode) {
               if ((int)str.size() < max_char) {
                  // can't insert a numeral before the minus sign
                  if ((str.size() == 0) ||
                     (str.size() && (caretp == 0) && (str.at(0) != '-')) ||
                     (caretp > 0)) {
                     str.insert(caretp , ch);
                     ++char_scroll;
                     ++caretp;
                  }
               }
            } else {
               if ((int)caretp < max_char) {
                  str.replace(caretp , 1 , 1 , (char)ascii);
                  if ((int)caretp == (char_scroll + chars_wide)) {
                     ++char_scroll;
                  }
                  ++caretp;
               }
            }
         }
      }
      key_used = true;
   }
   return key_used;
}



//bool FloatingPointParser(string& str , int key_read , UINT& caretp , bool& insert_mode,
//                bool& selection_made , UINT& begin_select , UINT& end_select);
bool TextInput::FloatingPointParser(int key_read) {
   // [-][123][.][123][eE][-][123]
   int ascii = key_read & 0xff;
//   bool key_used = EditParser(str , key_read , caretp , insert_mode , selection_made , begin_select , end_select);
   bool key_used = EditParser(key_read);
   if (!key_used && ((ascii == '-') || (ascii == '.') || (ascii == 'e') || (ascii == 'E') || (isdigit(ascii)))) {
      UINT start_select = (begin_select < end_select)?begin_select:end_select;
      UINT finish_select = (begin_select > end_select)?begin_select:end_select;
      UINT select_diff = finish_select - start_select;
      char ch[2] = {(char)ascii , '\0'};
      if (selection_made) {
         str.erase(start_select , select_diff);
         selection_made = false;
         if ((int)start_select >= char_scroll) {
            if ((char_scroll + chars_wide) > (int)str.size()) {
               char_scroll = str.size() - chars_wide;
            }
         } else {
            char_scroll -= select_diff;
         }
         caretp = start_select;
         begin_select = end_select = caretp;
         str.insert(caretp , ch);
         ++char_scroll;
         ++caretp;
      } else {
         if (insert_mode) {
            if ((int)str.size() < max_char) {
               str.insert(caretp , ch);
               ++char_scroll;
               ++caretp;
            }
         } else {
            if ((int)caretp < max_char) {
               str.replace(caretp , 1 , 1 , (char)ascii);
               if ((int)caretp == (char_scroll + chars_wide)) {
                  ++char_scroll;
               }
               ++caretp;
            }
         }
      }
      key_used = true;
   }
   return key_used;
}



void TextInput::ResetDimensions() {
   ASSERT(text_font);
   padding = text_height(text_font)/4;
   int w = 2*(padding + 1) + chars_wide*text_length(text_font , " ");// 6 + chars_wide*8 for 8x8 font
   int h = 2*(padding + 1) + text_height(text_font);// 14 for 8x8 font
   WidgetBase::SetDrawDimensions(w,h);
}



void TextInput::RefreshIntValue() {
   SetIntegerValue(atoi(str.c_str()));
}



void TextInput::RefreshFloatValue() {
   SetFloatValue(atof(str.c_str()));
}



void TextInput::ResetCaret() {
   time_count = 0.0;
   caret_on = true;
   SetRedrawFlag();
}



void TextInput::RoundFloatValue() {
   throw NotFinishedError("RoundFloatValue not finished yet.");
   if (round_floats) {
      int decimal_point_index = str.find_first_of('.');
      if (decimal_point_index != string::npos) {
         int current_num_decimals = str.size() - (decimal_point_index + 1);
         long long int numbers = 0;
         int digit = 0;
         int tens = 1;
         for (int i = str.size() - 1 ; i >= 0 ; --i) {
            digit = str[i] - '0';
            if (str[i] != '.' && str[i] != '-') {
               numbers += digit*tens;
               tens++;
            }
            if (str[i] == '-') {
               numbers *= -1;
            }
         }
         if (num_decimal_digits > current_num_decimals) {
            // need to add zeros to the str
            int num_to_add = num_decimal_digits - current_num_decimals;
            for (int i = 0 ; i < num_to_add ; ++i) {
               str.push_back('0');
            }
         }
         else if (num_decimal_digits < current_num_decimals) {
            /// WORKING HERE!!!
            // need to truncate and round
            int length = str.size() - (numbers < 0)?1:0 - (str.find_first_of('-') == string::npos)?0:1;
            int decimal_index = decimal_point_index - (str.find_first_of('-') == string::npos)?0:1;
            
            string temp_str = "";
            long long int digits = numbers;
            for (int i = 0 ; i < length ; ++i) {
               /// WORKING HERE!
            }
         }
      }
      else {
         // no decimal point, add one
         str.push_back('.');
         for (int i = 0 ; i < num_decimal_digits ; ++i) {
            str.push_back('0');
         }
      }
      RefreshFloatValue();
   }
}



TextInput::TextInput(string name , TEXT_INPUT_TYPE type , FONT* textfont , int num_chars_wide , int max_num_chars ,
                     int xpos , int ypos , UINT wflags) :
      WidgetBase(name , NULL , Rectangle(xpos,ypos,0,0) , wflags),
      input_type(type),
      str(),
      text_font(textfont),
      max_char(0),chars_wide(0),char_scroll(0),padding(0),
      insert_mode(true),selection_made(false),caret_on(false),begin_selection(false),
      scroll_left(false),scroll_right(false),
      caretp(0),begin_select(0),end_select(0),
      time_count(0.0),caret_period(0.0),scroll(0.0),scroll_rate(0.0),
      int_min(0),int_value(0),int_max(0),
      float_min(0.0),float_value(0.0),float_max(0.0),
      round_floats(false),
      num_decimal_digits(0)
{
   ASSERT(textfont);
   if (max_num_chars < 1) {max_num_chars = 1;}
   if (num_chars_wide > max_num_chars) {num_chars_wide = max_num_chars;}
   if (num_chars_wide < 1) {num_chars_wide = 1;}
   max_char = max_num_chars;
   chars_wide = num_chars_wide;
   ResetDimensions();
   SetCaretPeriod();
   SetScrollRate();
   switch (input_type) {
      case TEXT_INPUT :
         SetTextValue("");
         break;
      case INTEGER_INPUT :
         SetIntegerValue(0);
         break;
      case FLOATING_POINT_INPUT :
         SetFloatValue(0.0);
         break;
   }
}



WidgetMsg TextInput::Update (double tsec) {
   UINT retmsg = DIALOG_OKAY;
   if (WidgetBase::flags & HASFOCUS) {
      time_count += tsec;
      if (time_count > caret_period) {
         while (time_count > caret_period) {
            time_count -= caret_period;
            caret_on = !caret_on;
         }
         retmsg |= DIALOG_REDRAW_ME;
         SetRedrawFlag();
      }
      if (scroll_left || scroll_right) {
         double dir = (scroll_left)?-1.0:1.0;
         scroll += (dir*scroll_rate*tsec);
         int scroll_delta = (int)scroll;
         if (scroll_delta) {
            scroll -= (double)scroll_delta;
            char_scroll += scroll_delta;
            if (char_scroll <= (-chars_wide)) {
               char_scroll = -chars_wide + 1;
            }
            if (char_scroll > ((int)str.size() - chars_wide)) {
               char_scroll = (int)str.size() - chars_wide;
            }
            retmsg |= DIALOG_REDRAW_ME;
            SetRedrawFlag();
         }
         
      }
   } else {
      if (caret_on) {
         caret_on = false;
         retmsg |= DIALOG_REDRAW_ME;
         SetRedrawFlag();
      }
   }
   return WidgetMsg(this , TOPIC_DIALOG , retmsg);
}



WidgetMsg TextInput::CheckInputs(int msx , int msy) {
   UINT retmsg = DIALOG_OKAY;
   
   const int tx = area.X() + 1 + padding;
   const int tlx = tx - char_scroll*text_length(text_font , " ");// TODO? Works for fixed width fonts only
   const int dx = msx - tlx;
   const int cdx = dx + text_length(text_font , " ")/2;
   
   if (WidgetBase::flags & ENABLED) {
      if (input_mouse_press(LMB)) {
         if (area.Contains(msx,msy)) {
            retmsg |= DIALOG_TAKE_FOCUS;
            retmsg |= DIALOG_REDRAW_ME;
            retmsg |= DIALOG_INPUT_USED;
            SetRedrawFlag();
            clear_keybuf();
            int caret_pos = cdx / text_length(text_font , " ");
            if (caret_pos < 0) {caret_pos = 0;}
            if (caret_pos > (char_scroll + chars_wide)) {caret_pos = char_scroll + chars_wide;}
            caretp = (unsigned int)caret_pos;
            begin_select = caretp;
            end_select = caretp;
            begin_selection = true;
            return WidgetMsg(this , TOPIC_DIALOG , retmsg);
         }
      }
      if (flags & HASFOCUS) {
         if (begin_selection) {
            if (input_mouse_held(LMB) || input_mouse_release(LMB)) {
               retmsg |= DIALOG_INPUT_USED;
               int finish_select = cdx / text_length(text_font , " ");
               if (finish_select < 0) {finish_select = 0;}
               if (finish_select > (char_scroll + chars_wide)) {finish_select = char_scroll + chars_wide;}
               if ((int)end_select != finish_select) {
                  retmsg |= DIALOG_REDRAW_ME;
                  SetRedrawFlag();
               }
               end_select = (unsigned int)finish_select;
               caretp = end_select;
               selection_made = (begin_select != end_select);
               scroll_left = scroll_right = false;
               if (input_mouse_held(LMB)) {
                  if (msx <= area.X()) {
                     scroll_left = true;
                  }
                  if (msx >= area.BRX()) {
                     scroll_right = true;
                  }
               }
               if (input_mouse_release(LMB)) {
                  begin_selection = false;
               }
            }
         }
         
         
         
         bool key_read = false;
         
         if (input_key_press(KEY_ENTER) || input_key_press(KEY_ENTER_PAD)) {
            key_read = true;
            clear_keybuf();
            switch (input_type) {
                  case TEXT_INPUT :
                     break;
                  case INTEGER_INPUT :
                     RefreshIntValue();
                     break;
                  case FLOATING_POINT_INPUT :
                     RefreshFloatValue();
                     break;
            }
            QueueUserMessage(this , TOPIC_TEXT_INPUT , TEXT_VALUE_CHANGED);
            QueueUserMessage(this , TOPIC_TEXT_INPUT , TEXT_VALUE_ENTERED);
         } else {
            while (keypressed()) {
               int rkey = readkey();
               switch (input_type) {
                  case TEXT_INPUT :
                     if (TextParser(rkey)) {key_read = true;}break;
                  case INTEGER_INPUT :
                     if (IntegerParser(rkey)) {key_read = true;}break;
                  case FLOATING_POINT_INPUT :
                     if (FloatingPointParser(rkey)) {key_read = true;}break;
               }
            }
            if (key_read) {
               QueueUserMessage(this , TOPIC_TEXT_INPUT , TEXT_VALUE_CHANGED);
            }
         }
         if (key_read) {
            retmsg |= DIALOG_REDRAW_ME;
            retmsg |= DIALOG_INPUT_USED;
            SetBgRedrawFlag();
         }
      }
   }
   return WidgetMsg(this , TOPIC_DIALOG , retmsg);
}



void TextInput::DisplayOn(BITMAP* bmp , int x , int y) {
   if (flags & VISIBLE) {
      Rectangle r = area;
      r.MoveBy(x,y);
      r.Fill(bmp , (*wcols)[BGCOL]);
      r.Draw(bmp , (*wcols)[FGCOL]);
      Rectangle inner = r;
      inner.Shrink(1);
      Rectangle t(r.X() + 1 + padding , r.Y() + 1 + padding , r.W() - 2*(1 + padding) , r.H() - 2*(1 + padding));
      int tlx = t.X() - char_scroll*text_length(text_font , " ");// TODO? Works for fixed width fonts only
      Rectangle old_clip = GetClippingRectangle(bmp);
      inner.SetClipRect(bmp);
      textout_ex(bmp , text_font , str.c_str() , tlx , t.Y() , (*wcols)[TXTCOL] , -1);
      if (selection_made) {
         UINT start_select = (begin_select < end_select)?begin_select:end_select;
         UINT finish_select = (begin_select > end_select)?begin_select:end_select;
         UINT select_diff = finish_select - start_select;
         string str1(str.substr(0,start_select));
         string str2(str.substr(start_select , select_diff));
         int htlx = tlx + text_length(text_font , str1.c_str());
         Rectangle ht(htlx,t.Y(),text_length(text_font , str2.c_str()) , text_height(text_font));
         ht.Fill(bmp , (*wcols)[HLCOL]);
         textout_ex(bmp , text_font , str2.c_str() , htlx , t.Y() , (*wcols)[BGCOL] , -1);
      }
      if (caret_on) {
         string cstr(str.substr(0,caretp));
         int cx = tlx + text_length(text_font , cstr.c_str());
         if (insert_mode) {
            vline(bmp , cx-1 , t.Y() , t.Y() + text_height(text_font) , (*wcols)[TXTCOL]);
            vline(bmp , cx , t.Y() , t.Y() + text_height(text_font) , (*wcols)[TXTCOL]);
         } else {
            hline(bmp , cx , t.Y() + text_height(text_font) , cx + text_length(text_font , " ") - 1 , (*wcols)[TXTCOL]);
            hline(bmp , cx , t.Y() + text_height(text_font) + 1 , cx + text_length(text_font , " ") - 1 , (*wcols)[TXTCOL]);
         }
      }
      old_clip.SetClipRect(bmp);
   }
   ClearRedrawFlag();
}



void TextInput::SetDrawDimensions(int width , int height) {
   int char_area_width = width - 2*(1 + padding);
   int cw = char_area_width / text_length(text_font , " ");
   if (cw < 1) {cw = 1;}
   chars_wide = cw;
   ResetDimensions();
}



void TextInput::SetArea(int xpos , int ypos , int width , int height) {
   SetDrawPos(xpos,ypos);
   SetDrawDimensions(width,height);
}



void TextInput::SetArea(const Rectangle& r) {
   SetDrawPos(r.X() , r.Y());
   SetDrawDimensions(r.W() , r.H());
}



void TextInput::SetFont(FONT* textfont) {
   ASSERT(textfont);
   ResetDimensions();
}



void TextInput::SetNumCharsWide(int num) {
   if (num < 1) {num = 1;}
   chars_wide = num;
   ResetDimensions();
}



void TextInput::SetCaretPeriod(double time_on) {
   const double minimum = 0.1;
   if (time_on < minimum) {time_on = minimum;}
   caret_period = time_on;
}



void TextInput::SetScrollRate(double chars_per_second) {
   const double minimum = 1.0;
   if (chars_per_second < minimum) {chars_per_second = minimum;}
   scroll_rate = chars_per_second;
}



void TextInput::SetTextValue(string text) {
   input_type = TEXT_INPUT;
   str = text;
   if ((int)str.size() > chars_wide) {
      char_scroll = 0;
   } else {
      char_scroll = (int)str.size() - chars_wide;
   }
   caretp = begin_select = end_select = 0;
   selection_made = false;
   begin_selection = false;
   SetRedrawFlag();
}



void TextInput::SetIntegerValue(int value) {
   input_type = INTEGER_INPUT;
   if (value < int_min) {value = int_min;}
   if (value > int_max) {value = int_max;}
   int_value = value;
   char buf[64];
   sprintf(buf , "%i" , value);
   str = buf;
   if ((int)str.size() > chars_wide) {
      char_scroll = 0;
   } else {
      char_scroll = (int)str.size() - chars_wide;
   }
   caretp = begin_select = end_select = 0;
   selection_made = false;
   begin_selection = false;
   SetRedrawFlag();
}



void TextInput::SetFloatValue(double value) {
   input_type = FLOATING_POINT_INPUT;
   if (value < float_min) {value = float_min;}
   if (value > float_max) {value = float_max;}
   float_value = value;
   char buf[128];
   sprintf(buf , "%lf" , value);
   str = buf;
   if ((int)str.size() > chars_wide) {
      char_scroll = 0;
   } else {
      char_scroll = (int)str.size() - chars_wide;
   }
   caretp = begin_select = end_select = 0;
   selection_made = false;
   begin_selection = false;
   SetRedrawFlag();
}



void TextInput::SetIntegerRange(int min , int value , int max) {
   if (max < min) {max = min;}
   int_min = min;
   int_max = max;
   SetIntegerValue(value);
}



void TextInput::SetFloatRange(double min , double value , double max) {
   if (max < min) {max = min;}
   float_min = min;
   float_max = max;
   SetFloatValue(value);
}



void TextInput::RoundFloats(bool use_rounding) {
   round_floats = use_rounding;
   RoundFloatValue();
}



void TextInput::SetNumDecimalDigits(UINT num_digits) {
  num_decimal_digits = num_digits;
  RoundFloatValue();
}



string TextInput::GetTextValue() {
   ASSERT(input_type == TEXT_INPUT);
   return str;
}



int TextInput::GetIntMin() {
   ASSERT(input_type == INTEGER_INPUT);
   return int_min;
}



int TextInput::GetIntValue() {
   ASSERT(input_type == INTEGER_INPUT);
   return (atoi(str.c_str()));
}



int TextInput::GetIntMax() {
   ASSERT(input_type == INTEGER_INPUT);
   return int_max;
}



double TextInput::GetFloatMin() {
   ASSERT(input_type == FLOATING_POINT_INPUT);
   return float_min;
}



double TextInput::GetFloatValue() {
   ASSERT(input_type == FLOATING_POINT_INPUT);
   return (atof(str.c_str()));
}



double TextInput::GetFloatMax() {
   ASSERT(input_type == FLOATING_POINT_INPUT);
   return float_max;
}






