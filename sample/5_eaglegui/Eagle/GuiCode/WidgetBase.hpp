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


#ifndef WidgetBase_H
#define WidgetBase_H


#include <allegro.h>

#include "Eagle/Area.hpp"
#include "Eagle/GuiCode/WidgetColorset.hpp"
#include "Eagle/GuiCode/WidgetMessage.hpp"
#include "Eagle/Logging.hpp"

#include <string>

/** TODO : IDEA : WIP
   Adding in support for backgrounds / frames to widgets
   1) Add margins to each widget that default to 0
   
   2) Callback function to draw border/frame
   
   3) Base class for borders/frames, each widget stores a pointer to the base class
   
   4) Use composite widgets instead
      Widget A;
      BorderWidgetB;
      CompositeWidget C(A,B);

*/



/** Generic WidgetMsg return values */

extern const unsigned int TOPIC_DIALOG;

/** Bitfield values for messages returned to a dialog from the Update() and CheckInput() functions. */
enum DIALOG_MSGS {
   DIALOG_OKAY       = 0x00,// Also used to signify that there are no messages for the WidgetHandler::TakeNextMessage function.
   DIALOG_CLOSE      = 0x01,
   DIALOG_REDRAW_ME  = 0x02,// TODO : Unused, remove - WidgetHandler checks each widget's flags for redraw
   DIALOG_REDRAW_ALL = 0x04,/// For when the background needs to be cleared / redrawn
   DIALOG_INPUT_USED = 0x08,/// Play nice, a WidgetHandler will stop checking other widgets input after this is received.
   DIALOG_TAKE_FOCUS = 0x10,
   DIALOG_DISABLED   = 0x20,// Returned when you try to update or check the inputs of a disabled dialog.
   DIALOG_REMOVE_ME  = 0x40
};

enum WIDGET_FLAGS {
   FLAGS_NONE      = 0x000,
   ENABLED         = 0x001,   // Currently active or not
   VISIBLE         = 0x002,   // Whether anything is drawn
   HOVER           = 0x004,   // If it has mouse hover - don't steal the focus for hover!
   HASFOCUS        = 0x008,   // If it has the focus
   MOVEABLE        = 0x010,   // Whether it can be moved
   RESIZEABLE      = 0x020,   // Whether it can be resized
   NEEDS_REDRAW    = 0x040,   // Whether it should be redrawn before display?
   NEEDS_BG_REDRAW = 0x080,   // Whether it needs its background redrawn before being redrawn
   ALLOW_CLOSE     = 0x100,   // If it is allowed to close the dialog based on input
   ALLOW_OVERLAP   = 0x200    // If this widget is allowed to overlap its area for some reason
};

typedef unsigned int UINT;

extern const UINT BASIC_FLAGS;
extern const UINT DEFAULT_FLAGS;
extern const UINT DEFAULT_OVERLAP_FLAGS;


/// Widgets with higher display priority will display on top of widgets with lower display priority
/// When widgets have equal priority, the widget that last had the focus will display on top
enum WIDGET_DISPLAY_PRIORITY {
   LOWEST_DISPLAY_PRIORITY = 0,
   LOW_DISPLAY_PRIORITY = 64,
   MID_DISPLAY_PRIORITY = 128,
   HIGH_DISPLAY_PRIORITY = 192,
   HIGHEST_DISPLAY_PRIORITY = 255
};


enum MEMORY_MANAGEMENT_TYPE {
   DO_NOT_DELETE = 0,
   DELETE_WHEN_DONE = 1
};


/** User notes
   Widgets don't need to check for hover themselves, the WidgetHandlers do it for them. SetHoverState is called for
   appropriate widgets on each call to CheckInputs for the handlers, and SetHoverState will let the parent know if it should
   set it's own redraw flag so that none of the widget's NEEDS_REDRAW flags are missed.
   Widgets also don't need to handle the VISIBLE flag in their drawing routine as the handler will draw the background
   over their position if they are invisible and their NEEDS_REDRAW flag is set.
*/

class WidgetHandler;

class WidgetBase {

/** TODO : What do derived widgets need access to? */
protected :

   std::string     wname;
   WidgetBase*     parent;
   WidgetBase*     messagehandler;
   WidgetColorset  wc;
   WidgetColorset* wcols;// will generally be set to &parent's wc by
                         // the parent WidgetHandler, but will be &wc when wc is used for individual colorsets
                         /// NOTE : The WidgetColorset pointed to by this can have it's color depth changed
                         /// when you call SetColorDepth.

   BITMAP*    dest;   // 'Parent' bitmap, only necessary for calls to Display() with no parameters.
   Rectangle  area;   // Area of the destination to be drawn to, corresponds to a position on dest
   int minw;
   int minh;

   UINT flags;
   
   int display_priority;// The higher the priority value, the closer to the top of the drawing it will be

public :

   WidgetBase(std::string name);
   WidgetBase(std::string name , const Rectangle& position , UINT wflags = DEFAULT_FLAGS);
   WidgetBase(std::string name , BITMAP* surface , const Rectangle& position , UINT wflags = DEFAULT_FLAGS);
/// This constructor will never work correctly because WidgetHandler::AddWidget calls virtual functions on a base class object
///   WidgetBase(WidgetBase* dialog , BITMAP* surface , const Rectangle& position, UINT wflags = DEFAULT_FLAGS);

   virtual ~WidgetBase() {}

   /// Warning - All widget messages passed back to a WidgetHandler by the Update 
   ///           and CheckInputs functions that are not related to a dialog will be ignored.
   ///    Use QueueUserMessage for user notifications instead.

   /// Pure virtual methods, these must have a derived class counterpart
   virtual WidgetMsg Update(double tsec)=0; // logic handling with animation support

   virtual WidgetMsg CheckInputs(int msx , int msy)=0;// Pass it the mouse position relative to it's drawing target.
                                                      // Hover is taken care of by WidgetHandlers, if you need it to
                                                      // do something, override the virtual SetHoverState function.

   virtual void DisplayOn(BITMAP* bmp , int x , int y)=0;/// DisplayOn should always draw, and always 
                                                         /// clear the NEEDS_REDRAW flag by calling ClearRedrawFlag().


   /// In case widgets need to load/free resources, or something else - default return value is true
   virtual bool Init();// load resources
   virtual bool Ready();// status okay
   virtual bool Shutdown();// free resources - A bool value is probably not necessary, but just in case some widget needs it.
   
   virtual void QueueUserMessage(const WidgetMsg& wmsg);// For the top level WidgetHandlers to store messages to the user from 
                                                        // the widgets it is handling. Messages are passed up the chain using
                                                        // the parent pointer (or the messagehandler pointer if it is non-null).
                                                        /// Warning - if the widget has no parent, these messages just get lost,
                                                        /// and only in the case that the top level parent widget is a 
                                                        /// WidgetHandler do they get spooled for the user.
   
   inline void QueueUserMessage(WidgetBase* widget_address , UINT widget_topic , int messages);// Shortcut to virtual Q.U.M.


   /// Setters
   inline void SetParent(WidgetBase* p);
   inline void SetMessageHandler(WidgetBase* mh);

   
   virtual void SetWidgetColors(const WidgetColorset& colors);// Sets colors for this widget and all children widgets
                                                              // This widget now uses its own colors

   virtual void UseWidgetColorset(WidgetColorset* colorset);// Sets colors for this widget and all children widgets
   
   virtual void UseIndividualColorset();// Sets colors only for this widget
   
   virtual void SetDrawTarget(BITMAP* surface);

   virtual void SetDrawPos(int xpos , int ypos);
   virtual void SetDrawDimensions(int width , int height);
   virtual void SetArea(int xpos , int ypos , int width , int height);
   virtual void SetArea(const Rectangle& r);

   /// Virtual Setters, override them if you need to
   virtual void SetFlagStates(UINT FLAGS , bool state);/// (Which flags set to which state)
   
   /// Default behaviour for state setters          Behaviour regarding redraw state
   virtual void SetEnabledState      (bool state);// Sets bg redraw flag
   virtual void SetVisibilityState   (bool state);// Sets bg redraw flag
   virtual void SetHoverState        (bool state);// /* Does not set any redraw flag */ - OLD:Sets redraw flag and (false) sets bg redraw flag
   virtual void SetFocusState        (bool state);// Sets redraw flag and (false) sets bg redraw flag
   virtual void SetMoveableState     (bool state);// Does not set redraw flag
   virtual void SetResizeableState   (bool state);// Does not set redraw flag
   virtual void SetNeedsRedrawState  (bool state);
   virtual void SetNeedsBgRedrawState(bool state);// (true) sets redraw flag
   virtual void SetAllowCloseState   (bool state);// Does not set redraw flag
   virtual void SetAllowOverlapState (bool state);// Does not set redraw flag

   virtual void SetRedrawFlag();// Shortcut to SetNeedsRedrawState(true)
   virtual void SetBgRedrawFlag();// Shortcut to SetNeedsBgRedrawState(true)
   virtual void ClearRedrawFlag();// Shortcut to SetNeedsRedrawState(false) and SetNeedsBgRedrawState(false)

   virtual void SetRedrawAllFlag();// To tell parent widget handlers to redraw all widgets

   virtual void ShowWidget();// Makes the widget enabled and visible
   virtual void HideWidget();// Makes the widget disabled and invisible
   virtual void ToggleWidgetVisibility();// Toggles the enabled and visible state of the widget

   virtual bool AcceptsFocus() {return true;}
   virtual bool IsMouseOver(int msx , int msy) const;
   
   /// For widgets to ask their parent widget handlers to give them the focus
///   virtual bool GiveWidgetFocus(WidgetBase* widget);
   virtual bool GiveWidgetFocus(WidgetBase* widget , bool notify_parent = true);

   virtual bool SetColorDepth(int color_depth);// Changes the color depth of it's WidgetColorset

   virtual void SetFont(FONT* textfont) {}

   void SetMinWidth(int w);
   void SetMinHeight(int h);
   void SetMinDimensions(int w , int h);
   
   void SetDisplayPriority(int priority);

   virtual int AbsMinWidth() {return 1;}
   virtual int AbsMinHeight() {return 1;}
   
   virtual bool HasGui() {return false;}
   virtual WidgetHandler* GetGui() {return 0;}
   
   WidgetHandler* NearestParentGui();
   WidgetBase* Root();
   WidgetHandler* RootGui();


   /// Getters , some references for direct modification
   inline const std::string     Name() const ;
   inline WidgetBase*           Parent() const;
   inline WidgetColorset        Colorset() const;
   inline const WidgetColorset* WCols() const;
   virtual const Rectangle      Area() const;
   inline const int             MinWidth() const;
   inline const int             MinHeight() const;
   inline const UINT            Flags() const;
   inline const int             DisplayPriority() const;

   /// Display methods

   inline void Display();// Draws to the stored drawing target at the stored position
   
   /// Miscellaneous functions

   inline bool UsingOwnColors();
   
   void PrintBaseTo(std::ostream& os , Indenter& indent) const;
   void PrintTo(std::ostream& os) const;

   virtual void PrintTo(std::ostream& os , Indenter& indent) const;

   friend std::ostream& operator<<(std::ostream& os , const WidgetBase& wb);

};



/** #########   Inline WidgetBase functions ############ */

inline void WidgetBase::QueueUserMessage(WidgetBase* widget_address , UINT widget_topic , int messages) {
   QueueUserMessage(WidgetMsg(widget_address , widget_topic , messages));
}

inline void WidgetBase::SetParent(WidgetBase* p) {parent = p;}

inline void WidgetBase::SetMessageHandler(WidgetBase* mh) {messagehandler = mh;}

inline const std::string WidgetBase::Name() const  {return wname;}

inline WidgetBase* WidgetBase::Parent() const {return parent;}

inline WidgetColorset WidgetBase::Colorset() const {return wc;}

inline const WidgetColorset*  WidgetBase::WCols() const {return wcols;}

inline const int WidgetBase::MinWidth() const {return minw;}

inline const int WidgetBase::MinHeight() const {return minh;}

inline const UINT WidgetBase::Flags() const {return flags;}

inline const int WidgetBase::DisplayPriority() const {return display_priority;}

inline void WidgetBase::Display() {
   ASSERT(dest);
   DisplayOn(dest , 0 , 0);
}

inline bool WidgetBase::UsingOwnColors() {return (wcols == &wc);}

/// -----------------   Global functions   -----------------------------

bool DrawPriorityIsLess(WidgetBase* lhs , WidgetBase* rhs);


/**
/// ############################################################

/// Template for deriving new widgets from the WidgetBase class

/// ############################################################

class NewAwesomeWidget : public WidgetBase {

private :
   

protected :
   

public :

   /// For reference
///   WidgetBase();
///   WidgetBase(const Rectangle& position , UINT wflags = DEFAULT_FLAGS);
///   WidgetBase(BITMAP* surface , const Rectangle& position , UINT wflags = DEFAULT_FLAGS);

   /// Warning - All widget messages passed back to a WidgetHandler by the Update 
   ///           and CheckInputs functions that are not related to a dialog will be ignored.
   ///    Use QueueUserMessage for user notifications instead.

   /// Pure virtual methods, these must have a derived class counterpart
   virtual WidgetMsg Update (double tsec); // logic handling with animation support

   virtual WidgetMsg CheckInputs(int msx , int msy);// Pass it the mouse position relative to it's drawing target.
                                                      // Hover is taken care of by WidgetHandlers, if you need it to
                                                      // do something, override the virtual SetHoverState function.

   virtual void DisplayOn(BITMAP* bmp , int x , int y);/// DisplayOn should always draw, and always 
                                                         /// clear the NEEDS_REDRAW flag by calling ClearRedrawFlag().


   WidgetMsg NewAwesomeWidget::Update(double tsec) {
      UINT retmsg = DIALOG_OKAY;
      if (WidgetBase::flags & ENABLED) {
         // if (visible_portion_changes) {
         //    SetRedrawFlag();
         //    retmsg |= DIALOG_REDRAW_ME;
         // }
      }
      return WidgetMsg(this , TOPIC_DIALOG , retmsg);
   }

   WidgetMsg NewAwesomeWidget::CheckInputs(int msx , int msy) {
      UINT retmsg = DIALOG_OKAY;
      if (WidgetBase::flags & ENABLED) {
         // if (visible_portion_changes) {
         //    SetRedrawFlag();
         //    retmsg |= DIALOG_REDRAW_ME;
         // }
      }
      return WidgetMsg(this , TOPIC_DIALOG , retmsg);
   }

   void NewAwesomeWidget::DisplayOn(BITMAP* bmp , int x , int y) {
      if (WidgetBase::flags & VISIBLE) {
         
      }
      ClearRedrawFlag();
   }


   /// In case widgets need to load/free resources, or something else - default return value is true
///   virtual bool Init();// load resources
///   virtual bool Ready();// status okay
///   virtual bool Shutdown();// free resources - A bool value is probably not necessary, but just in case some widget needs it.
   
   /// Many other virtual functions that probably don't need to be overloaded, check WidgetBase class to be sure

};





//*/

#endif // WidgetBase_H

