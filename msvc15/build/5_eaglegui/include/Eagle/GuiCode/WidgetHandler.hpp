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


#ifndef WidgetHandler_H
#define WidgetHandler_H


#include "Eagle/GuiCode/WidgetBase.hpp"
#include "Eagle/BitmapHandler.hpp"
#include "Eagle/InputHandler.hpp"
#include "Eagle/GuiCode/TitleBar.hpp"
#include "Eagle/GuiCode/Camera.hpp"

#include <vector>
#include <list>
#include <map>



/// Used by WidgetHandler::TakeNextMessage to signify an empty message queue
extern const WidgetMsg NoMessages;


extern const unsigned int TOPIC_GUI;

enum GUI_MSGS {
   GUI_RESIZED = 0
};

/*
WidgetMsg wmsg;

// Use like this 
while ((wmsg = gui.TakeNextMessage()) != NoMessages) {...}
// Or like this
while (gui.HasMessages()) {
   wmsg = gui.TakeNextMessage();
   {...}
}
//*/


enum BACKGROUND_TYPE {
   BG_SOLID        = 0,// solid background color
   BG_ROUNDED      = 1,// rounded rectangular panel of background color
   BG_TRANSPARENT  = 2,// cleared to mask color
   BG_TRANSLUCENT  = 3,// cleared to zero alpha
   BG_IMAGE        = 4 // solid background image
};

enum FOCUS_DRAW_TYPE {
   FOCUS_INVISIBLE               = 0,
   FOCUS_HIGHLIGHT_OUTLINE       = 1,
   FOCUS_HIGHLIGHT_THICK_OUTLINE = 2,
   FOCUS_HIGHLIGHT_DOTTED        = 3,
   FOCUS_HIGHLIGHT_THICK_DOTTED  = 4
};


/// Container and controller class for handling widgets
/// WidgetHandler has a built in title bar and sub bitmap camera that send widget messages just like
/// any widget you add to the handler. Use the member functions for checking against their messages.
class WidgetHandler : public WidgetBase {

   typedef std::vector<WidgetBase*> WIDGETLIST;
   typedef WIDGETLIST::iterator WLIT;

   typedef std::list<WidgetMsg> MESSAGEQUEUE;
   typedef MESSAGEQUEUE::iterator MQIT;

   typedef std::map<WidgetBase* , bool> MEMTRACKER;
   typedef MEMTRACKER::iterator MTIT;

private :

   bool OwnsWidget(WidgetBase* widget);
   UINT WidgetIndex(WidgetBase* widget);
   WLIT InputListIterator(WidgetBase* widget);

   void ResetWidgetDrawingTargets();

   void RedrawRoundedBackground();
   
   void CheckRedraw(UINT widget_index);
   void CheckRedraw();

   void DrawFocus();

   void SortDrawListByPriority();// sorts from least drawing priority to highest drawing priority
   
   void AddToRemoveList(WidgetBase* w);
   void RemoveOldWidgets();
   

protected :

   BACKGROUND_TYPE background_type;
   BITMAP*         background_src;
   BitmapHandler   background;
   BitmapHandler   buffer;

   WIDGETLIST      wlist;
   WIDGETLIST      inputlist;// in order from top to bottom, not necessarily the reverse of drawlist's order
   WIDGETLIST      drawlist;// in order from back to front

   int             focus_index;
   bool            focus_start;
   WidgetBase*     wfocus;
   
   WidgetBase*     whover;
   
   MESSAGEQUEUE    mque;
   bool            clear_background;

   InputGroup      focus_cycle_forward;
   InputGroup      focus_cycle_backward;

   int             rounded_radius;
   double          rounding_percent;

   TitleBar        titlebar;
   TitleBar        mintitlebar;
   bool            minimized;
   
   FOCUS_DRAW_TYPE focus_type;

   std::list<Rectangle> dbg_list;//dirty background rectangle list
   
   Camera          cam;
   int             real_w;
   int             real_h;
   
   MEMTRACKER      memtrack;
   WIDGETLIST      removelist;
   
public :

   /// These 4 functions are only public because they have to be for things to work right
   bool HasWidget(WidgetBase* widget);// call this if you want, but you should already know the answer, includes sub GUIs.
   void        FocusStart();// starts the focus search over at the beginning(also the end)
   WidgetBase* NextFocus();// don't call directly
   WidgetBase* PreviousFocus();// don't call directly
   
   /// The focus cycling functions will only function if this is the top level widget handler
   void CycleFocusForward();
   void CycleFocusBackward();


   /// Constructors
   WidgetHandler(std::string name);

   // This constructor makes the title bar invisible
   WidgetHandler(std::string name , BITMAP* surface , const Rectangle& position , UINT wflags = DEFAULT_FLAGS);

   WidgetHandler(std::string name , FONT* title_font , std::string title_text , bool is_minimized , bool allow_movement ,
                  BITMAP* surface , const Rectangle& position , UINT wflags = DEFAULT_FLAGS);

   ~WidgetHandler();

   bool SetupBitmapHandlers(BACKGROUND_TYPE bg_type , BMP_MEM_TYPE buffer_memory_type , BMP_DRAW_TYPE buffer_draw_type);
   bool SetupBackgroundBuffer(BACKGROUND_TYPE bg_type);
   void SetBackgroundImage(BITMAP* bg);// bg will be stretched to fit the WidgetHandler's area and must be a memory bitmap
   void FreeImageBuffers();// Frees the BitmapHandler buffer and background so global WidgetHandler destruction doesn't crash.


   bool      HasMessages();
   WidgetMsg TakeNextMessage();
   void      ClearMessages();// Clears the message queue of any messages remaining

   void ClearDialog();// Removes all widgets from the dialog and destroys the ones marked for deletion. Global WH's should probably call this.
   void RemoveWidget(WidgetBase* w);// This must NOT be called from code of the widget that is being destroyed!
   virtual void AddWidget(WidgetBase* widget , bool delete_when_removed = false);
   WidgetHandler& operator<<(WidgetBase* widget);// Same effect as AddWidget(widget);
   
   virtual WidgetMsg Update (double tsec);
   virtual WidgetMsg CheckInputs(int msx , int msy);
   virtual void DisplayOn(BITMAP* bmp , int x , int y);

   virtual bool Init();
   virtual bool Ready();
   virtual bool Shutdown();

   
   virtual void QueueUserMessage(const WidgetMsg& wmsg);// For the top level WidgetHandlers to store messages to the user from 
                                                        // the widgets it is handling. Messages are passed up the chain using
                                                        // the parent pointer.

   virtual void SetWidgetColors(const WidgetColorset& colors);
   
   virtual void SetWidgetColor(const WIDGETCOLOR& wcolor , int color);

   virtual void UseWidgetColorset(WidgetColorset* colorset);

   virtual void SetDrawTarget(BITMAP* surface);


   virtual void SetDrawPos(int xpos , int ypos);
   virtual void SetDrawDimensions(int width , int height);
   virtual void SetArea(int xpos , int ypos , int width , int height);
   virtual void SetArea(const Rectangle& r);

   virtual void SetEnabledState(bool state);
   virtual void SetVisibilityState(bool state);
   virtual void SetNeedsRedrawState(bool state);
   virtual void SetRedrawFlag();


   virtual void SetFocusState(bool state);

   virtual bool GiveWidgetFocus(WidgetBase* widget , bool notify_parent = true);// notify_parent for internal use only

   virtual const Rectangle Area() const;
   
   virtual void SetRedrawAllFlag();

   virtual bool ChangeAttributes(int color_depth , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type);

   virtual bool SetColorDepth(int color_depth);// Changes the color depth of it's WidgetColorset and BitmapHandler
   bool ChangeToMemoryType(BMP_MEM_TYPE mem_type);

   virtual bool HasGui();
   virtual WidgetHandler* GetGui();

   void SetRoundingRadiusPercent(double percent);

   void SetTitleBarVisibility(bool visible);
   void SetTitleText(std::string text);
   void SetTitleFont(FONT* textfont);
   void SyncTitleBarPos();

   void Minimize();
   void Maximize();
   void ToggleMinimizeState();
   void SetMinimizeState(bool is_minimized);

   void SetFocusDrawType(FOCUS_DRAW_TYPE type , bool for_all_guis = false);
   void SetBlenderType(ALPHA_BLENDER_TYPE type);

   void MakeAreaDirty(Rectangle dirty_area);

   bool SetVirtualDimensions(int vwidth , int vheight);

   void AccMoveViewTlxTo(int xpos , int ypos , double time = 0.0);// decelerated move to position, instant move if time = 0.0
   void AccMoveViewCenterTo(int xpos , int ypos , double time = 0.0);// decelerated move to position, instant move if time = 0.0
   void AccMoveViewBy(int dx , int dy , double time = 0.0);// decelerated move by position delta
   
   void MoveViewTlxTo(int xpos , int ypos , double time = 0.0);// set velocity move to position, instant move if time = 0.0
   void MoveViewCenterTo(int xpos , int ypos , double time = 0.0);// set velocity move to position, instant move if time = 0.0
   void MoveViewBy(int dx , int dy , double time = 0.0);// set velocity move by position delta

   void AllowMiddleMouseButtonDrag(bool allow);// false by default, drag moves the view

   /// Getters
   WidgetBase*   CurrentFocus();
   WidgetBase*   CurrentHover();
   BMP_DRAW_TYPE GetDrawType();
   bool          SeeThrough();
   Rectangle     GetTitleBarArea() const;
   Rectangle     GetRealTitleBarArea() const;
   BITMAP*       BackgroundBitmap() {return (BITMAP*)background;}// For drawing the background only, pair with MakeAreaDirty().
   BITMAP*       BufferBitmap() {return (BITMAP*)buffer;}// For saving purposes only. Don't draw to this, or destroy it.

   bool          Minimized();


   bool AreaFree(Rectangle r , WidgetBase* widget);
   bool InView(WidgetBase* w);
   WidgetBase* GetWidgetAt(int x , int y);
   
   
   const WidgetMsg TitleCloseMessage() {return WidgetMsg(&titlebar , TOPIC_TITLE_BAR , TITLE_SIGNAL_CLOSE);}
   const WidgetMsg TitleMinimizedMessage() {return WidgetMsg(&titlebar , TOPIC_TITLE_BAR , TITLE_MINIMIZED);}
   const WidgetMsg TitleMaximizedMessage() {return WidgetMsg(&mintitlebar , TOPIC_TITLE_BAR , TITLE_MAXIMIZED);}
   
   const WidgetMsg ViewMovedMessage() {return WidgetMsg(&cam , TOPIC_CAMERA , CAMERA_VIEW_MOVED);}
   const WidgetMsg ViewDestReachedMessage() {return WidgetMsg(&cam , TOPIC_CAMERA , CAMERA_VIEW_DEST_REACHED);}


   virtual void PrintTo(std::ostream& os , Indenter& indent) const;
   friend std::ostream& operator<<(std::ostream& os , const WidgetHandler& wh);




};


#endif // WidgetHandler_H

