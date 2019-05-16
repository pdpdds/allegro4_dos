

#ifndef UtilityWidgets_H
#define UtilityWidgets_H

#include <string>

#include "Eagle/GuiCode/WidgetBase.hpp"


/// Adjuster, composite, divider widgets

enum WIDGET_GRAB_AREA {
   GRAB_NW = 0,
   GRAB_N = 1,
   GRAB_NE = 2,
   GRAB_E = 3,
   GRAB_SE = 4,
   GRAB_S = 5,
   GRAB_SW = 6,
   GRAB_W = 7,
   GRAB_MOVE = 8,
   GRAB_NONE = 9
};

WIDGET_GRAB_AREA GetGrabbedArea(WidgetBase* w , Rectangle area , int x , int y);

class WidgetHandler;

class WidgetAdjuster : public WidgetBase {
private :
   WidgetBase* grabbed;
   int mouse_owner_id;
   bool is_move;
   int x1;
   int x2;
   int y1;
   int y2;
   int sx;
   int sy;
   int smx;
   int smy;
   int* px;
   int* py;


public :
   
   WidgetAdjuster(std::string name);

   virtual WidgetMsg Update(double tsec);
   virtual WidgetMsg CheckInputs(int msx , int msy);
   virtual void DisplayOn(BITMAP* bmp , int x , int y);

   virtual void SetEnabledState(bool state);

};

enum SPLITTER_TYPE {
   SPLIT_LEFTRIGHT = 0,// one widget is to the left, the other is on the right
   SPLIT_TOPBOTTOM = 1// one widget is above, the other is below
};

typedef void (*SPLITTER_DRAWING_CALLBACK) (BITMAP* bmp , Rectangle r , SPLITTER_TYPE split_type , const WidgetColorset& wc);

void DefaultSplitterDraw(BITMAP* bmp , Rectangle r , SPLITTER_TYPE split_type , const WidgetColorset& wc);



class Splitter : public WidgetBase {
protected :
   int mouse_owner_id;
   WidgetBase* wupleft;
   WidgetBase* wdownright;
   WidgetBase* wztop;// Which widget is on top, and gets input first
   WidgetBase* wzbottom;// Which widget is on bottom, and gets input second
   SPLITTER_TYPE splitter_type;
   Rectangle gap;
   int gapsz;
   double split_percent;
   SPLITTER_DRAWING_CALLBACK drawing_callback;
   

   void SetupArea();

public :
   
   Splitter(std::string name , SPLITTER_TYPE type , WidgetBase* upleft , WidgetBase* downright , WidgetBase* gets_input_first ,
            int gapsize , int xpos , int ypos , SPLITTER_DRAWING_CALLBACK callback = DefaultSplitterDraw , UINT wflags = DEFAULT_FLAGS);

   virtual WidgetMsg Update(double tsec);
   virtual WidgetMsg CheckInputs(int msx , int msy);
   virtual void DisplayOn(BITMAP* bmp , int x , int y);

   virtual bool Init();
   virtual bool Ready();
   virtual bool Shutdown();

   void SetWidgets(WidgetBase* upleft , WidgetBase* downright , WidgetBase* gets_input_first);
   
};

#endif // UtilityWidgets_H

