//
// GALGUI
// Gregory McIntyre and Lucy Ding
//

#include "window.h"


WINDOW::WINDOW(int x, int y, uint w, uint h, BITMAP * scrbuffer) {
    mytitle_height = 16;
    myx = x;
    myy = y;
    myw = w;
    myh = h;
    mydone = false;
    myokayed = false;
    mymoveable = true;
    myfont = font;
    mybackground = NULL;
    myscrbuffer = scrbuffer;

    mytitlebar = new TITLEBAR(*this);

    mybackup = create_bitmap(w, h);
    blit(scrbuffer, mybackup, x, y, 0, 0, w, h);

    mybuffer = create_bitmap(w, h);
}


WINDOW::~WINDOW() {
    if (mybackup != NULL)
        destroy_bitmap(mybackup);

    if (mybuffer != NULL)
        destroy_bitmap(mybuffer);
}


char * WINDOW::name() {
    return myname;
}
int WINDOW::x() {
    return myx;
}
int WINDOW::y() {
    return myy;
}
uint WINDOW::w() {
    return myw;
}
uint WINDOW::h() {
    return myh;
}
uint WINDOW::title_height() {
    return mytitle_height;
}
FONT * WINDOW::default_font() {
    return myfont;
}


bool WINDOW::mouse_over(WIDGET & w) {
    int mx = mouse_x;
    int my = mouse_y;

    return ((mx >= (myx + w.x()) && (mx <= (myx + w.x() + w.w())))
            && (my >= (myy + w.y()) && (my <= (myy + w.y() + w.h()))));
}


void WINDOW::set_name(const char * newname) {
    strncpy(myname, newname, 127);
    mytitlebar->set_name(newname);
}


void WINDOW::set_font(FONT * f)            {
    myfont = f;
}
void WINDOW::set_background(BITMAP * back) {
    mybackground = back;
}


void WINDOW::move(int newx, int newy) {
    if (mymoveable) {
        /*
        if (newx < -mytitlebar->x())
          newx = -mytitlebar->x();
        else if (newx > myscrbuffer->w)
          newx = myscrbuffer->w;
        if (newy < -mytitlebar->y())
          newy = -mytitlebar->y();
        else if (newy > myscrbuffer->h)
          newy = myscrbuffer->h;
        */
        int ox = myx, oy = myy;

        // copy what was behind the window back again
        blit(mybackup, myscrbuffer, 0, 0, myx, myy, myw, myh);

        // make a new backup
        blit(myscrbuffer, mybackup, newx, newy, 0, 0, myw, myh);

        myx = newx;
        myy = newy;

        draw(myscrbuffer);

        show_mouse(myscrbuffer);
        blit(myscrbuffer, screen, ox, oy, ox, oy, myw, myh);
        blit(myscrbuffer, screen, myx, myy, myx, myy, myw, myh);
        show_mouse(NULL);
    }
}


void WINDOW::draw(BITMAP * dest) {
// draw

// microsoft windows style

// draw main back
    if (mybackground == NULL)
        rectfill(mybuffer, 0, 0, myw, myh, makecol16(190, 190, 190));
    else {
        for (int y = 0; y < myh; y += mybackground->h)
            for (int x = 0; x < myw; x += mybackground->w)
                blit(mybackground, mybuffer, 0, 0, x, y, mybackground->w, mybackground->h);
    }

// draw widgets

    WIDGET * w;
    for (WIDGETLIST::iterator i = mywidgets.begin(); i != mywidgets.end(); ++i) {
        w = *i;
        set_clip_rect(mybuffer, w->x(), w->y(), w->x() + w->w() - 1, w->y() + w->h() - 1);
        w->draw(mybuffer);
    }
    set_clip_rect(mybuffer, 0, 0, mybuffer->w, mybuffer->h);

// draw border
    rect(mybuffer, 0, 0, myw - 1, myh - 1, makecol16(50, 50, 50));
    rect(mybuffer, 0, 0, myw - 2, myh - 2, makecol16(100, 100, 100));
    rect(mybuffer, 1, 1, myw - 3, myh - 3, makecol16(150, 150, 150));

    blit(mybuffer, dest, 0, 0, myx, myy, myw, myh);
}


void WINDOW::run() {
    WIDGETLIST::iterator i;

    bool m_down = false;  // mouse down
    bool m_over = false;  // mouse over
    bool m_drag = false;
    int m_pos = mouse_pos;

    WIDGET * activew = NULL;
    WIDGET * overw = NULL;  // widget the mouse is over

    int mx = 0, my = 0;

    do {
        draw(myscrbuffer);

        show_mouse(myscrbuffer);
        blit(myscrbuffer, screen, myx, myy, myx, myy, myw, myh);
        blit(myscrbuffer, screen, mx - 5, my - 5, mx - 5, my - 5, 20, 20);
        mx = mouse_x;
        my = mouse_y;
        blit(myscrbuffer, screen, mx - 5, my - 5, mx - 5, my - 5, 20, 20);
        show_mouse(NULL);

        // what widget is the mouse over?
        overw = NULL;
        for (i = mywidgets.begin(); i != mywidgets.end(); ++i) {
            if (mouse_over(**i))
                overw = *i;
        }

        if (activew != NULL) {
            if (overw != activew)
                activew->mouse_up();
            else
                activew->mouse_down();
        }

        // figure out mouse button status from previously stored mouse info
        if (!m_down && (mouse_b & 1)) {
            // mouse button has just been pressed
            m_down = true;
            activew = overw;
            if (activew != NULL)
                activew->mouse_down();
        }

        if (m_down && !(mouse_b & 1)) {
            // mouse button has just been un-pressed
            m_down = false;
            m_drag = false;
            if (activew != NULL) {
                activew->mouse_up();
                if (activew == overw)
                    activew->mouse_click();
            }
            activew = NULL;
        }

        if ((m_down) && (mouse_b & 1) && (mouse_pos != m_pos) && (!m_drag)) {
            // mouse has just started dragging
            m_drag = true;
            activew = overw;
            if (activew != NULL)
                activew->mouse_drag_init();
        }

#define X(x) ((x >> 16) & 0xFFFF)
#define Y(x) (x & 0xFFFF)

        if (m_drag) {
            // mouse is still dragging something
            if (activew != NULL)
                activew->mouse_drag(X(m_pos), Y(m_pos), X(mouse_pos), Y(mouse_pos));
        } else
            m_pos = mouse_pos;

        if (key[KEY_ENTER])
            okay();
        else if (key[KEY_ESC])
            cancel();
    } while (!mydone);

    if (!myokayed) {
        // save information
        for (i = mywidgets.begin(); i != mywidgets.end(); ++i)
            (*i)->save();
    }
}


void WINDOW::add_widget(WIDGET * w) {
    mywidgets.push_back(w);
}


void WINDOW::okay() {
    mydone = true;
    myokayed = true;
}


void WINDOW::cancel() {
    mydone = true;
    myokayed = false;
}


void WINDOW::fix() {
    mymoveable = false;
}

