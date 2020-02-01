#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "types.h"
#include "widget.h"
#include <vector>
#include <allegro.h>

typedef std::vector<WIDGET *> WIDGETLIST;

class WINDOW {
    public:
        WINDOW(int x, int y, uint w, uint h, BITMAP * scrbuffer);
        ~WINDOW();
        char * name(); // window name (in titlebar)
        int x();
        int y();
        uint w();  // width
        uint h();  // height
        FONT * default_font(); // default window font used by all widgets
        uint title_height(); // height of the titlebar
        bool mouse_over(WIDGET & w); // whether or not the mouse is over the given widget
        void set_name(const char * newname); // set window name (will appear in titlebar)
        void set_font(FONT * f); // set window default font
        void set_background(BITMAP * back); // set window background bitmap
        void draw(BITMAP * dest); // draw on dest at x(), y()
        void run(); // main window loop
        void move(int newx, int newy); // set window position
        void fix(); // make unable to be dragged around
        void okay(); // stop executing update widget loop, save everything
        void cancel(); // stop executing update widget loop, don't save
        void add_widget(WIDGET * w); // add a widget to this window (used by widgets to add themselves)
    protected:
        char myname[128];
        int myx, myy;
        uint myw, myh;
        int mytitle_height;
        bool mydone;
        bool myokayed;
        bool mymoveable;
        WIDGETLIST mywidgets;
        FONT * myfont;
        BITMAP * mybuffer;
        BITMAP * mybackground;
        BITMAP * mybackup;
        BITMAP * myscrbuffer;
        WIDGET * mytitlebar;
};

#endif

