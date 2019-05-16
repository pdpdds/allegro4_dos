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


#ifndef IconButton_H
#define IconButton_H


#include "Eagle/GuiCode/ButtonWidget.hpp"
#include "Eagle/BitmapHandler.hpp"



enum ICON_PIC_TYPE {
   UP_PIC       = 0,
   DOWN_PIC     = 1,
   HOVER_PIC    = 2,
   DISABLED_PIC = 3
};

#define NUM_ICON_PICS 4

enum ICON_DISPLAY_TYPE {
   ICON_DISPLAY_CENTER  = 0,
   ICON_DISPLAY_STRETCH = 1
};


class IconButton : public Button {
   
protected :
   ICON_DISPLAY_TYPE icon_display_type;
   BitmapHandler pics[NUM_ICON_PICS];
   BitmapHandler stretch_pics[NUM_ICON_PICS];
   AreaGroup clickarea;
   STRETCH_TYPE stype;

   bool RefreshStretchedImages();
   bool RefreshStretchedImage(ICON_PIC_TYPE type);

public :

   IconButton(std::string name);// ICON_DISPLAY_CENTER , RECTANGLE_BTN , TOGGLE_BTN , input_key_press(KEY_NONE)

   // RECTANGLE_BTN , TOGGLE_BTN , input_key_press(KEY_NONE)
   IconButton(std::string name , ICON_DISPLAY_TYPE dtype , Rectangle position , UINT wflags = DEFAULT_FLAGS);

   IconButton(std::string name , ICON_DISPLAY_TYPE dtype , BUTTON_SHAPE shape , BTN_ACTION_TYPE atype ,
               const InputGroup& input , const Rectangle& position , UINT wflags = DEFAULT_FLAGS);

   ~IconButton() {}

   /// Overloads from Button class

   virtual WidgetMsg CheckInputs(int msx , int msy);
   virtual void DisplayOn(BITMAP* bmp , int x , int y);

//   virtual void SetDrawPos(int xpos , int ypos);
   virtual void SetDrawDimensions(int width , int height);
   virtual void SetArea(int xpos , int ypos , int width , int height);
   virtual void SetArea(const Rectangle& r);

   virtual void SetHoverState(bool state);
   virtual void SetRedrawFlag() {SetBgRedrawFlag();}
   
   virtual bool AcceptsFocus() {return true;}
   
   /// Member functions
   
   /* LoadImage* uses the current color depth that allegro is in */
   /* LoadImage* and SetImage* accept null for the file or bitmap pointer and will free the image in that case.
      The hover image and the disabled image are both optional, but the up and down image should be set. */
   bool LoadImages(const char* up_pic_file , const char* down_pic_file , const char* hover_pic_file , const char* disabled_pic_file ,
                     BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type);
   bool LoadImage(ICON_PIC_TYPE icon_pic_type , const char* pic_file , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type);

   bool SetImages(BITMAP* up_pic , BITMAP* down_pic , BITMAP* hover_pic , BITMAP* disabled_pic  ,
                  BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type);// clones BITMAP*s
   
   bool SetImage(ICON_PIC_TYPE icon_pic_type , BITMAP* bmp , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type);// clones BITMAP*

   bool SetImage(ICON_PIC_TYPE icon_pic_type , const BitmapHandler& bh);// clones bitmap

   void FreeImages();/// Must be called for global IconButtons so their images are freed before allegro exits
   
   AreaGroup& ClickArea() {return clickarea;}// Set areas local to the images
   
   void SetBlenderType(ALPHA_BLENDER_TYPE type);
   void SetStretchType(STRETCH_TYPE type);
   
};



#endif // IconButton_H

