//
// GALGUI
// Gregory McIntyre and Lucy Ding
//

#include "widget.h"
#include "window.h"


WIDGET::WIDGET(int x, int y, uint w, uint h, WINDOW & win) : mywindow(win) {
    myname = "";
    myx = x;
    myy = y + win.title_height();
    myw = w;
    myh = h;

    mywindow.add_widget(this);
}

const std::string& WIDGET::name() const {
    return myname;
}
int WIDGET::x() const {
    return myx;
}
int WIDGET::y() const {
    return myy;
}
uint WIDGET::w() const {
    return myw;
}
uint WIDGET::h() const {
    return myh;
}

void WIDGET::set_name(const std::string& newname) {
    myname = newname;
}

void WIDGET::mouse_down()  {}
void WIDGET::mouse_up()    {}
void WIDGET::mouse_click() {}
void WIDGET::mouse_drag_init() {}
void WIDGET::mouse_drag(uint ox, uint oy, uint mx, uint my) {}

void WIDGET::save() {}



// BUTTON ///////////////////////////////////////////////////////////////////

BUTTON::BUTTON(int x, int y, uint w, uint h, WINDOW & win, void (*proc)()) : WIDGET(x, y, w, h, win) {
    myprocedure = proc;
    mydepressed = false;
    mycancel = false;
    myokay = false;
    myicon = NULL;
}


void BUTTON::draw(BITMAP * dest) {
    int x2 = myx + myw - 1;
    int y2 = myy + myh - 1;

    if (mydepressed) {
        // microsoft windows style
        rect(dest, myx,     myy,     x2,     y2,     makecol16(50, 50, 50));     // outer border
        rect(dest, myx + 1, myy + 1, x2,     y2,     makecol16(120, 120, 120));  // inner border
        rectfill(dest, myx + 2, myy + 2, x2 - 1, y2 - 1, makecol16(180, 180, 180));  // middle

        if (myicon != NULL)
            draw_sprite(dest, myicon, myx + (myw - myicon->w)/2 + 2, myy + (myh - myicon->h)/2 + 2);
        textout_centre_ex(dest, mywindow.default_font(), myname.c_str(), myx + myw/2 + 2, myy + myh/2 - text_height(mywindow.default_font())/2 + 2, makecol16(0, 0, 0), -1);
    } else {
        // microsoft windows style

        // outer border
        vline(dest, myx, myy, y2, makecol16(240, 240, 240));  // left
        hline(dest, myx, myy, x2, makecol16(240, 240, 240));  // top
        vline(dest, x2,  myy, y2, makecol16(50, 50, 50));     // right
        hline(dest, myx, y2,  x2, makecol16(50, 50, 50));     // bottom

        // inner border
        vline(dest, myx + 1, myy + 1, y2 - 1, makecol16(190, 190, 190));  // left
        hline(dest, myx + 1, myy + 1, x2 - 1, makecol16(190, 190, 190));  // top
        vline(dest, x2 - 1,  myy + 1, y2 - 1, makecol16(150, 150, 150));  // right
        hline(dest, myx + 1, y2 - 1,  x2 - 1, makecol16(150, 150, 150));  // bottom

        rectfill(dest, myx + 1, myy + 1, x2 - 2, y2 - 2, makecol16(190, 190, 190));  // middle

        if (myicon != NULL)
            draw_sprite(dest, myicon, myx + (myw - myicon->w)/2, myy + (myh - myicon->h)/2);
        textout_centre_ex(dest, mywindow.default_font(), myname.c_str(), myx + myw/2, myy + myh/2 - text_height(mywindow.default_font())/2, makecol16(0, 0, 0), -1);
    }
}


void BUTTON::mouse_down() {
    mydepressed = true;
}
void BUTTON::mouse_up()   {
    mydepressed = false;
}
void BUTTON::mouse_click() {
    if (myprocedure != NULL)
        myprocedure();

    if (myokay)
        mywindow.okay();
    else if (mycancel)
        mywindow.cancel();
}


void BUTTON::set_icon(BITMAP * icon) {
    myicon = icon;
}


void BUTTON::make_okay() {
    myname = "Okay";
    myokay = true;
    mycancel = false;
}


void BUTTON::make_cancel() {
    myname = "Cancel";
    mycancel = true;
    myokay = false;
}



// CHECKBOX /////////////////////////////////////////////////////////////////

CHECKBOX::CHECKBOX(int x, int y, uint w, uint h, WINDOW & win, bool * checked) : WIDGET(x, y, w, h, win) {
    myoriginal_value = checked;
    mychecked = *checked;
    mydepressed = false;
}


bool CHECKBOX::checked() {
    return mychecked;
}


void CHECKBOX::draw(BITMAP * dest) {
    int x2 = myx + myw - 1;
    int y2 = myy + myh - 1;

// outer border
    hline(dest, myx, y2,  x2, makecol16(240, 240, 240));
    vline(dest, x2,  myy, y2, makecol16(240, 240, 240));
    hline(dest, myx, myy, x2, makecol16(150, 150, 150));
    vline(dest, myx, myy, y2, makecol16(150, 150, 150));

// inner border
    hline(dest, myx + 1, myy + 1, x2 - 1, makecol16(100, 100, 100));
    vline(dest, myx + 1, myy + 1, y2 - 1, makecol16(100, 100, 100));
//hline(dest, myx + 1, y2 - 1,  x2 - 1, makecol16(190, 190, 190));
//vline(dest, x2,  myy, y2, makecol16(220, 220, 220));

    if (mydepressed)
        rectfill(dest, myx + 2, myy + 2, x2 - 2, y2 - 2, makecol16(190, 190, 190));
    else
        rectfill(dest, myx + 2, myy + 2, x2 - 2, y2 - 2, makecol16(255, 255, 255));

    if (mychecked) {
        line(dest, myx + 3, myy + 3, x2 - 3,  y2 - 3, makecol16(0, 0, 0));
        line(dest, myx + 4, myy + 3, x2 - 3,  y2 - 4, makecol16(0, 0, 0));
        line(dest, myx + 3, myy + 4, x2 - 4,  y2 - 3, makecol16(0, 0, 0));
        line(dest, x2 - 3,  myy + 3, myx + 3, y2 - 3, makecol16(0, 0, 0));
        line(dest, x2 - 4,  myy + 3, myx + 3, y2 - 4, makecol16(0, 0, 0));
        line(dest, x2 - 3,  myy + 4, myx + 4, y2 - 3, makecol16(0, 0, 0));
    }
}


void CHECKBOX::mouse_down()  {
    mydepressed = true;
}
void CHECKBOX::mouse_up()    {
    mydepressed = false;
}
void CHECKBOX::mouse_click() {
    mychecked = !mychecked;
}


void CHECKBOX::save() {
    *myoriginal_value = mychecked;
}



// LABEL ////////////////////////////////////////////////////////////////////

LABEL::LABEL(int x, int y, uint w, uint h, WINDOW & win) : WIDGET(x, y, w, h, win) {
}


const char * LABEL::text() {
    return mytext.c_str();
}
void LABEL::set(const char * newtext) {
    mytext = newtext;
}


void LABEL::draw(BITMAP * dest) {
// textwrap! remember, remember

    textout_ex(dest, mywindow.default_font(), text(), myx, myy, makecol16(0, 0, 0), -1);
}



// TITLEBAR /////////////////////////////////////////////////////////////////

TITLEBAR::TITLEBAR(WINDOW & win) : WIDGET(3, 3 - win.title_height(), win.w() - 6, win.title_height(), win) {
    myname = win.name();
}


void TITLEBAR::draw(BITMAP * dest) {
    for (uint x = myx; x != (myx + myw - 1); ++x)
        vline(dest, x, myy, myh, makecol16(130 - 130*x/myw/2, 40 - 40*x/myw/2, 175 - 175*x/myw/2));
    textout_ex(dest, mywindow.default_font(), myname.c_str(), myx + 3, myy + (myh - text_height(mywindow.default_font()))/2, makecol16(255, 255, 255), -1);
}


void TITLEBAR::mouse_drag_init() {
    myox = mywindow.x();
    myoy = mywindow.y();
}


void TITLEBAR::mouse_drag(uint ox, uint oy, uint mx, uint my) {
    mywindow.move(myox + (mx - ox), myoy + (my - oy));
}

