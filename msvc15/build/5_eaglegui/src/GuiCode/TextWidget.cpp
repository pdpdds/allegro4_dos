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


#include "Eagle/GuiCode/TextWidget.hpp"

using std::string;



void TextWidget::ResetTextPos() {
   int tlx = 0;
   int tly = 0;
   int width = text_length(text_font , text.c_str());
   int height = text_height(text_font);
   
   switch (texthpos) {
      case TEXT_LEFT :
         tlx = text_x;
         break;
      case TEXT_CENTER :
         tlx = text_x - width/2;
         break;
      case TEXT_RIGHT :
         tlx = text_x - width;
         break;
   }
   switch (textvpos) {
      case TEXT_TOP :
         tly = text_y;
         break;
      case TEXT_VCENTER :
         tly = text_y - height/2;
         break;
      case TEXT_BOTTOM :
         tly = text_y - height;
         break;
   }
   WidgetBase::SetArea(tlx,tly,width,height);
}



TextWidget::TextWidget(string name , string text_str , int tx , int ty , UINT wflags) :
      WidgetBase(name , NULL , Rectangle(0,0,0,0) , wflags),
      text(),
      text_font(font),
      texthpos(TEXT_LEFT),
      textvpos(TEXT_TOP),
      text_x(tx) , text_y(ty)
{
   ASSERT(font);
   SetText(text_str);
}



TextWidget::TextWidget(string name , int text_val , int tx , int ty , UINT wflags) :
      WidgetBase(name , NULL , Rectangle(0,0,0,0) , wflags),
      text(),
      text_font(font),
      texthpos(TEXT_LEFT),
      textvpos(TEXT_TOP),
      text_x(tx) , text_y(ty)
{
   ASSERT(font);
   SetText(text_val);
}



TextWidget::TextWidget(string name , double text_val , int tx , int ty , UINT wflags) :
      WidgetBase(name , NULL , Rectangle(0,0,0,0) , wflags),
      text(),
      text_font(font),
      texthpos(TEXT_LEFT),
      textvpos(TEXT_TOP),
      text_x(tx) , text_y(ty)
{
   ASSERT(font);
   SetText(text_val);
}



TextWidget::TextWidget(string name , double text_val , unsigned int min_width , unsigned int num_decimals ,
                       int tx , int ty , UINT wflags) :
      WidgetBase(name , NULL , Rectangle(0,0,0,0) , wflags),
      text(),
      text_font(font),
      texthpos(TEXT_LEFT),
      textvpos(TEXT_TOP),
      text_x(tx) , text_y(ty)
{
   ASSERT(font);
   SetText(text_val , min_width , num_decimals);
}



TextWidget::TextWidget(string name , string text_str , FONT* textfont , TEXT_HPOS text_hpos , TEXT_VPOS text_vpos ,
                        int tx , int ty ,UINT wflags) :
      WidgetBase(name , NULL , Rectangle(0,0,0,0) , wflags),
      text() ,
      text_font(textfont),
      texthpos(text_hpos),
      textvpos(text_vpos),
      text_x(tx) , text_y(ty)
{
   ASSERT(text_font);
   SetText(text_str);
}




TextWidget::TextWidget(string name , int text_val , FONT* textfont , TEXT_HPOS text_hpos , TEXT_VPOS text_vpos ,
                        int tx , int ty ,UINT wflags) :
      WidgetBase(name , NULL , Rectangle(0,0,0,0) , wflags),
      text(),
      text_font(textfont),
      texthpos(text_hpos),
      textvpos(text_vpos),
      text_x(tx) , text_y(ty)
{
   ASSERT(text_font);
   SetText(text_val);
}



TextWidget::TextWidget(string name , double text_val , FONT* textfont , TEXT_HPOS text_hpos , TEXT_VPOS text_vpos ,
                        int tx , int ty ,UINT wflags) :
      WidgetBase(name , NULL , Rectangle(0,0,0,0) , wflags),
      text(),
      text_font(text_font),
      texthpos(text_hpos),
      textvpos(text_vpos),
      text_x(tx) , text_y(ty)
{
   ASSERT(text_font);
   SetText(text_val);
}



TextWidget::TextWidget(string name , double text_val , unsigned int min_width , unsigned int num_decimals ,
                        FONT* textfont , TEXT_HPOS text_hpos , TEXT_VPOS text_vpos ,
                        int tx , int ty , UINT wflags) :
      WidgetBase(name , NULL , Rectangle(0,0,0,0) , wflags),
      text(),
      text_font(text_font),
      texthpos(text_hpos),
      textvpos(text_vpos),
      text_x(tx) , text_y(ty)
{
   ASSERT(textfont);
   SetText(text_val , min_width , num_decimals);
}



WidgetMsg TextWidget::Update(double tsec) {
   return WidgetMsg(this , TOPIC_DIALOG , DIALOG_OKAY);
}



WidgetMsg TextWidget::CheckInputs(int msx , int msy) {
   return WidgetMsg(this , TOPIC_DIALOG , DIALOG_OKAY);
}



void TextWidget::DisplayOn(BITMAP* bmp , int x , int y) {
   Rectangle bg = area;
   bg.MoveBy(x,y);
//   bg.Draw(bmp , (*wcols)[HLCOL]);/// debug only
   textout_ex(bmp , text_font , text.c_str() , bg.X() , bg.Y() , (*wcols)[TXTCOL] , -1);
   ClearRedrawFlag();
}



void TextWidget::SetDrawPos(int xpos , int ypos) {
   int dx = xpos - area.X();
   int dy = ypos - area.Y();
   SetTextPos(text_x + dx , text_y + dy);
}



void TextWidget::SetDrawDimensions(int width , int height) {
   /// does nothing
}



void TextWidget::SetArea(int xpos , int ypos , int width , int height) {
   int dx = xpos - area.X();
   int dy = ypos - area.Y();
   SetTextPos(text_x + dx , text_y + dy);
}



void TextWidget::SetArea(const Rectangle& r) {
   int dx = r.X() - area.X();
   int dy = r.Y() - area.Y();
   SetTextPos(text_x + dx , text_y + dy);
}



void TextWidget::SetFont(FONT* textfont) {
   ASSERT(textfont);
   text_font = textfont;
   ResetTextPos();
}



void TextWidget::SetText(string textstr) {
   text = textstr;
   ResetTextPos();
}



void TextWidget::SetText(const char* textstr) {
   SetText(string(textstr));
}



void TextWidget::SetText(int value) {
   char buf[64];
   for (int i = 0 ; i < 64 ; ++i) {buf[i] = '\0';}
   sprintf(buf , "%i" , value);
   SetText(buf);
}



void TextWidget::SetText(double value) {
   char buf[128];
   for (int i = 0 ; i < 128 ; ++i) {buf[i] = '\0';}
   sprintf(buf , "%lf" , value);
   SetText(buf);
}



void TextWidget::SetText(double value , unsigned int min_width , unsigned int num_decimals) {
   char prebuf[64];
   char buf[128];
   // %[min_width].[num_decimals]lf
   sprintf(prebuf , "%%%u.%ulf" , min_width , num_decimals);
   sprintf(buf , prebuf , value);
//   OutputLog() << prebuf << endl << buf << endl;
   SetText(buf);
}



void TextWidget::SetTextPos(int tx , int ty) {
   text_x = tx;
   text_y = ty;
   ResetTextPos();
}



void TextWidget::SetTextPosType(TEXT_HPOS hpos , TEXT_VPOS vpos) {
   texthpos = hpos;
   textvpos = vpos;
   ResetTextPos();
}



void TextWidget::SetTextPosEx(int tx , int ty , TEXT_HPOS hpos , TEXT_VPOS vpos) {
   text_x = tx;
   text_y = ty;
   texthpos = hpos;
   textvpos = vpos;
   ResetTextPos();
}





