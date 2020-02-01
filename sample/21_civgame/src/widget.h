#ifndef _WIDGET_H_
#define _WIDGET_H_

#include "types.h"
#include <string>
#include <allegro.h>

class WINDOW;

class WIDGET {
    public:
        WIDGET(int x, int y, uint w, uint h, WINDOW & win);
        const std::string& name() const;
        int x() const;
        int y() const;
        uint w() const;
        uint h() const;
        void set_name(const std::string& newname);
        virtual void draw(BITMAP * dest) = 0; // draw at x(), y() on dest
        virtual void mouse_down(); // the mouse is being pressed on top of this
        virtual void mouse_up(); // the mouse is no longer being pressed on top of this
        virtual void mouse_click(); //  the mouse was clicked on this
        virtual void mouse_drag_init();
        virtual void mouse_drag(uint ox, uint oy, uint mx, uint my);
        virtual void save(); // save information to original locations (eg. checkboxes)
    protected:
        std::string myname;
        int myx, myy;
        uint myw, myh;
        WINDOW & mywindow;
};



class BUTTON : public WIDGET {
    public:
        BUTTON(int x, int y, uint w, uint h, WINDOW & win, void (*proc)());
        void draw(BITMAP * dest);
        void mouse_down();
        void mouse_up();
        void mouse_click();
        void set_icon(BITMAP * icon); // give this button an icon
        void make_okay(); // make this button an okay button for its parent window
        void make_cancel(); // make this button a cancel button for its parent window
    protected:
        void (*myprocedure)();
        bool mydepressed;
        bool mycancel;
        bool myokay;
        BITMAP * myicon;
};



class CHECKBOX : public WIDGET {
    public:
        CHECKBOX(int x, int y, uint w, uint h, WINDOW & win, bool * checked);
        void draw(BITMAP * dest);
        void mouse_down();
        void mouse_up();
        void mouse_click();
        void save();
        bool checked(); // checked/unchecked state
    protected:
        bool * myoriginal_value;
        bool mychecked;
        bool mydepressed;
};



class LABEL : public WIDGET {
    public:
        LABEL(int x, int y, uint w, uint h, WINDOW & win);
        void draw(BITMAP * dest);
        const char * text(); // this label's text
        void set(const char * newtext); // set this label's text
    protected:
        std::string mytext;
};



class TITLEBAR : public WIDGET {
    public:
        TITLEBAR(WINDOW & win);
        void draw(BITMAP * dest);
        void mouse_drag_init();
        void mouse_drag(uint ox, uint oy, uint mx, uint my);
    protected:
        uint myox;
        uint myoy;
};


#endif

