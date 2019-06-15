/* gradient.c   code for gradient fills and menu/window drawing.

   Master file: Do not edit in other applications.

   A. M. Hugo September 2001

   Modified version 0.4.0, June 2002

   http://amhugo.tripod.com/gradient.html
   http://www.geocities.com/amhugo/gradient.html

   GRADIENT:  an allegro gradient theme generator!

   In its simplest form, GRADIENT Themes can provide neat subtle colour
   gradients for your dialogs, frames, help screens and menus.

   More complex functions can provide a complete application window (frame)
   with a title bar, menu bar, icon bar, address bar, status line and
   window handles and edge grips just like you see on web browsers.

   Gradient themes are built up on several layers:
   1. Colours - defined in RRGGBB hex format
   2. Colour schemes - combinations of several colours
   3. Gradients - combines colour schemes with a gradient drawing methods.
   4. Themes - combinations of gradients, colours and borderstyles:
               24 gradients to make a complete theme.


   Once a theme/gradient is loaded, it can be used for the next step:

   1. Windows with fancy title bars icons and the like.
   2. Toolbars for quick action tasks
   3. Dialogs for user input and reporting
   4. Menus: pop up and pull down
   5. Buttons, widgets and other controls

   The complete colour scheme can be changed with just a few lines of code.

   So, how do I use it?

   All you need to do is add gradient.c to your project file list and
   include "gradient.h" in any source file which uses GRADIENT functions.

   Operates in hicolour or truecolour modes only (15, 16, 24, 32 bit).

   LESSON 1.

   First, initialize the gradient system **AFTER** setting a graphics mode:

   int x, y, width, height, titlebar = 20;
   int menubar = 20, addrbar = 0, iconbar = 0, statbar = 20, handlebar = 4;
   char name[] = { "My Program Version 1.1" };
   BITMAP *userbmp;

   // set graphics mode & colour depth first... (code not shown)
   // then initialize the gradient theme, setup and draw the window...

   init_gradient();
   set_default_window_style(titlebar, menubar, addrbar, iconbar, statbar,
                     grips, titlebuttons, icon, wallpaper, barhandles)
   textcol = draw_default_window(screen, 0, 0, SCREEN_W, SCREEN_H,
                   FOCUSSED, font, name);

   This will initialize the gradient system with a default aqua gradient,
   set the sizes for the default GRADIENTWINDOW structure ( gw ),
   then use the default gradient to draw the main window with your values
   for the name, size and pixel height of the window components, such as
   the titlebar, menu bar etc. "textcol" returns the colour of the text
   to be drawn in the user frame. The draw_default_window(...) function
   fills in the window structure with the location of drawn items, including
   the dimensions of the users window or frame.

   Next, get the location of the user frame (the area under the titlebars)
   and use the gradient functions to draw text, menus, buttons and controls.


   get_window_frame(&x, &y, &width, &height);
   userbmp = create_sub_bitmap(screen, x, y, width, height);
   if (userbmp != NULL)
   {
      textprintf(userbmp, font, 10, 10, textcol, "This is my window.");
      draw_gradient(userbmp, 10, 30, 50, 20, GRADIENTBUTTON, FOCUSSED);
   }

   Once the window is drawn, you can add buttons and controls to the
   user frame (using "userbmp" to provide clipping) or to other parts of
   the window by using the info stored in the GRADIENTWINDOW gw.

   LESSON 2.

   Very likely, you would like to have several windows and dialogs in
   your program. If so, the default window ( gw ) will not suffice since
   it will be overwritten with each new window or dialog. It would be
   better to use your own GRADIENT array and GRADIENTWINDOW variables,
   reserving one GRADIENTWINDOW structure for each dialog, help window etc.
   It would also be nice to have a Toolbar at the bottom of the screen
   for special functions and quick launch icons.

   #define LISTWIN     0
   #define HELPWIN     1
   #define DIALWIN     2
   #define ALERTWIN    3
   #define MAINWIN     4

   GRADIENT my_gradient[MAXGRADIENTS];
   COLOURSCHEME my_scheme[MAXCOLOURSCHEMES];

   //                     bcolour  bw  bev han grip bullet pos
   BORDERSTYLE bstyle = { 0x000070,  1, 2,  5,  20,   1 ,   2};

   TOOLBAR maintbar =
   {    1,  26,   10,  4,   4,  16, 144, 0, 1,  20, 20,   1, 24, 24 };
   //                  margins   h  w   lhs rhs  w   h       w   h
   // open high handle LHS RHS   clock  butons butsize icon iconsize

   GRADIENTWINDOW gwin[5] =
   {
   // bmp  x   y   w   h  titl mb ibar ad st han but wp icon barhandle
   {NULL, 120,120,324,248, 20,  0,  0,  0, 20, 4, 0,  0, 0, 0}, // list box
   {NULL, 100,100,440,280, 20,  0,  0,  0, 20, 0, 0,  0, 0, 0}, // help win
   {NULL, 200,140,440,280, 20,  0,  0,  0, 20, 0, 0,  0, 0, 0}, // dialogue
   {NULL, 200,160,440,280, 20, 16, 32,  0, 16, 0, 0,  0, 0, 0}, // alert win
   {NULL,   0,  0,640,454, 24, 26,  0,  0, 20, 0,112, 0,32,10}  // main win
   };

   With the above declarations, make the following adjustments to the
   code above ...

   init_gradient();                                     // set defaults
   get_default_theme(my_gradient, my_scheme, &bstyle);  // copy it to mine
   set_borderstyle(&bstyle);                            // set my borders

   textcol = draw_gradient_window(screen, 0, 0, SCREEN_W, SCREEN_H-26,
                   FOCUSSED, font, name, &gwin[MAINWIN], my_gradient);
   draw_gradient_toolbar(screen, 0, SCREEN_H-26, SCREEN_W, 26, FOCUSSED,
             font, "Toolbar", my_gradient, &maintbar);
   get_gradient_window_frame(&x, &y, &width, &height, gwin[MAINWIN]);
   userbmp = create_sub_bitmap(screen, x, y, width, height);
   if (userbmp != NULL)
   {
      textprintf(userbmp, font, 10, 10, textcol, "This is my window.");
      draw_gradient_index(userbmp, 10, 30, 50, 20, GRADIENTBUTTON,
                            UNFOCUSSED, my_gradient);
      // put rest of your draw code here..
   }
   else  panic_stations("Universe crashed!");

   Lesson 3.

   For many applications, redrawing a window or dialog each time it is
   needed can be a real slowdown, especially for 486's or slow pentiums.
   Hence I suggest creating a memory buffer the size of your window/dialog
   and drawing the gradients to that. A window redraw will only need
   a single blit(...) function instead of a pixel-by-pixel redraw.
   If this is done at program start-up, much time can be saved. The
   following code uses your own gradients as in Lesson 2.  Substitute
   gradient[], scheme[] and borderstyle to use the default theme.
   This example initializes a help window.

   BITMAP *helpbmp = NULL;
   int helptextcol;

   int setup_helpscreen(char *name, int width, int height, )
   {
      int textcol;

      if (helpbmp != NULL)   destroy_bitmap(helpbmp);
      helpbmp = create_bitmap(width, height);
      textcol = draw_gradient_window(helpbmp, 0, 0, width, height,
                   FOCUSSED, font, name, &gwin[HELPWIN], my_gradient);
      return textcol;
   }

   // later, in main()...
   helptextcol = setup_helpscreen("Help Screen", 200, 300);

   // later, in a help() function, don't draw_gradient_window(), do this...
   blit(helpbmp, screen, 0, 0, x, y, helpbmp->w, helpbmp->h);
   textprintf(screen, font, 10, 10, textcol, "This is my help window.");
   // more helpful code here...

   Also, if you want to change the theme to say. a fire engine red theme,
   first load the theme file with either read_gradient(...) if you are
   using the defaults, or import_gradient() if using your owm structures.
   Set the border_style and call this setup routine to redraw the buffers
   and then a single blit(...) function will present it to the user.

   import_gradient("themes\fiery.csg", my_gradient, my_scheme, &bstyle)
   set_borderstyle(&bstyle);
   helptextcol = setup_helpscreen("Help Screen", 200, 300);

   Another tip: If you find that most of your buttons are of a similar size,
   create bitmaps in memory for the most common sizes. Make them for
   FOCUSDIABLED, UNFOCUSSED, FOCUSSED and FOCUSPRESSED varieties and use
   a blit(...) instead of a redraw when you need them. These also need to
   be created in the start-up code and redone if the theme changes.


   HERE_ENDETH_THE_LESSONS  ( CLASS_DISMISSED )

   Americanizations:

   Note: Throughout this code, I consistently spell the word 'colour' the
   English way, i.e. properly!
   This poses some benefits for me:
   1. My code does not conflict with other graphics libraries which
      make extensive use of the typographical error: "color"...
   2. I don't make so many typos, myself.
   3. I don't need to learn "harder tok emurrican"
   If this offends, use grep or a text editor to search/replace to your
   hearts content.

   I also have some weird views on allegro's antiquated coding style, but
   that's another story for another day...
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <allegro.h>
#include "gradient.h"


/* COLOURS - if defined individually in gradient files */

int numcolours = 0;

int colRGB[MAXCOLOURS];


/* COLOUR SCHEMES - combinations of colours */

COLOURSCHEME scheme[MAXCOLOURSCHEMES];

int numschemes = 0;


/* Colour & colour range information:
   This is for the optional component of the saved gradient file.
   Info on colours and colour ranges can be used to generate
   optimised palettes in 8 bit modes (an exersize for the reader) */

int logrange[100][2];          /* log of colour range information */

int numranges = 0;             /* number of colour ranges in gradient */


/* GRADIENTS - combinations of colour schemes and drawing methods */

/* The generic GRADIENT array - gradient[]: This array constitutes a theme.
   The gradient you have when you don't have a gradient!
   This object is used when drawing with a default gradient list. */

GRADIENT gradient[MAXGRADIENTS];



/* WINDOWS - drawing windows with menus, icon bars and the users' frame */

/* The generic GRADIENTWINDOW - gw:
   The window you have when you don't have a window!
   This struct is filled when drawing with the default gradient[] */

GRADIENTWINDOW gw;



/* BORDERS - values for drawing window borders, menu bullets etc. */

/*                          bcolour  bw  bev han grip bull bpos */
BORDERSTYLE borderstyle = { 0x000050,  1, 2,  5,  20, 2, 2 };

int bordercolour = 80;
int borderwidth = 1;
int bevelwidth = 2;
int handlewidth = 5;
int gripwidth = 20;
int bulletstyle = 2;
int bulletpos = 2;

/* who is to blame? */

char authortext[3][64] =          /* placing credits in the gradient file */
{
   "Derived from a Linux Blackbox theme by",
   "an unknown author.",
   "Modified by A.M.Hugo 2002 amhugo@optusnet.com.au"
};


/* list of names for each gradient or theme element */

char *gradient_name[] = {
"Toolbar",        "Toolbar Label",
"Toolbar Button", "Toolbar Window",
"Toolbar Handle", "Menu Bar",
"Menu Title",     "Menu Hilite",
"BarHandle",      "Menu Frame",
"Window Title",   "Window Label",
"Window Button",  "Window Frame",
"Window Sill",    "Window Grip",
"Iconbar",        "Iconbar Button",
"Address Bar",    "Text Entry",
"Status Bar",     "Tool Tip",
"Clock",          "Common",
"",
""
};


/*  Default Aqua Theme (from aqua.csg) - use this to define
    and generate the defaults. Used in init_gradient(); */

static int grdata[] =
{
   12, 24, 8, 12,      // schemes gradients ranges colours
// COLOURSCHEME enties
//  colour   colourto  textcol  grad
   0xbdd2ff, 0x2a3599, 0x000000, RGV,
   0xbdd2ff, 0x6aaae6, 0x000000, SGV,
   0x6aaae6, 0xbdd2ff, 0x000000, RGV,
   0xaaaaaa, 0xffffff, 0x000000, RGV,
   0x6aaae6, 0xffffff, 0x000000, RGV,
   0xffffff, 0x6aaae6, 0x000000, RGV,
   0xaaaaaa, 0x7f7f7f, 0xffffff, RGV,
   0xaaaaaa, 0x7f7f7f, 0x555555, SGV,
   0x7f7f7f, 0xaaaaaa, 0x000000, RGV,
   0xb7b7b7, 0x6a94b7, 0x000000, RGV,
   0x6a94b7, 0xffffff, 0x000000, RGV,
   0x17578c, 0x6aaae6, 0x000000, RGV,
// GRADIENT entries
// fc uc pc  fg  ug  pg jst
    0, 0, 0, 41, 41, 41, 0, // Toolbar
    1, 1, 1, 56, 56, 56, 1, // Toolbar Label
    2,10, 1, 73, 72, 74, 0, // Toolbar Button
    2, 2, 2, 88, 88, 88, 0, // Toolbar Window
    1, 3, 1, 73, 57, 73, 0, // Toolbar Handle
    5, 1, 1, 41, 41, 41, 0, // Menu Bar
    4, 4, 4, 73, 73, 73, 0, // Menu Title
    5, 5, 5, 56, 56, 56, 0, // Menu Hilite
    3, 3, 3, 41, 40, 42, 2, // Menu Bullet
    2, 2, 2, 88, 88, 88, 0, // Menu Frame
    0, 2, 2,552, 40, 40, 0, // Window Title
    2, 9, 2, 90, 88, 90, 1, // Window Label
    2,10, 1, 73, 72, 74, 0, // Window Button
    1, 1, 1, 57, 40, 40, 0, // Window Frame
    1, 3, 1, 73, 57, 73, 0, // Window Handle
    1, 3, 1, 25, 57, 25, 0, // Window Grip
    2, 2, 2, 25, 24,  8, 0, // Iconbar
    2,10, 1, 73, 72, 74, 0, // Iconbar Button
    2, 2, 2, 26, 24,  8, 0, // Address Bar
    4, 2, 2, 42, 42, 42, 0, // Text Entry
    2, 2, 2,  8,  8,  8, 0, // Status Bar
    5, 2, 2, 41, 41, 41, 0, // Tool Tip
   11,11,11, 90, 90, 90, 0, // Clock
    0, 0, 0, 41, 41, 41, 1, // Common
// Border colours
    0x2589db, 1, 2, 4, 2, 2, // Bcolour Bwidth Bevel Handle Bullet Bpos
// [optional] Colour Range information
// This can be used to optimize an 8 bit palette
    0xbdd2ff, 0x2a3599,
    0xbdd2ff, 0x6aaae6,
    0xaaaaaa, 0xffffff,
    0x6aaae6, 0xffffff,
    0xaaaaaa, 0x7f7f7f,
    0xb7b7b7, 0x6a94b7,
    0x6a94b7, 0xffffff,
    0x17578c, 0x6aaae6,
//
// [optional] Colours actually used
// in constructing this gradient
//
    0xbdd2ff, 0x2a3599, 0x000000, 0x6aaae6,
    0xaaaaaa, 0xffffff, 0x7f7f7f, 0x555555,
    0xb7b7b7, 0x6a94b7, 0x17578c, 0xffffff
};


char themebuf[256];      /* buffer for reading theme text files */

/* sine table for barrel, cylinder, elliptical and pipecross gradients */

int sinetable[256] =
{
0, 3, 6, 9, 12, 15, 18, 21, 25, 28, 31, 34, 37, 40, 43, 46, 
49, 53, 56, 59, 62, 65, 68, 71, 74, 77, 80, 83, 86, 89, 92, 95, 
97, 100, 103, 106, 109, 112, 115, 117, 120, 123, 126, 128, 131, 134, 136, 139,
142, 144, 147, 149, 152, 155, 157, 159, 162, 164, 167, 169, 171, 174, 176, 178,
181, 183, 185, 187, 189, 191, 193, 195, 197, 199, 201, 203, 205, 207, 209, 211,
212, 214, 216, 217, 219, 221, 222, 224, 225, 227, 228, 230, 231, 232, 234, 235,
236, 237, 238, 239, 241, 242, 243, 244, 244, 245, 246, 247, 248, 249, 249, 250,
251, 251, 252, 252, 253, 253, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 254, 254, 254, 253, 253, 252, 252, 251,
251, 250, 249, 249, 248, 247, 246, 245, 244, 244, 243, 242, 241, 239, 238, 237,
236, 235, 234, 232, 231, 230, 228, 227, 225, 224, 222, 221, 219, 217, 216, 214,
212, 211, 209, 207, 205, 203, 201, 199, 197, 195, 193, 191, 189, 187, 185, 183,
181, 178, 176, 174, 171, 169, 167, 164, 162, 159, 157, 155, 152, 149, 147, 144,
142, 139, 136, 134, 131, 128, 126, 123, 120, 117, 115, 112, 109, 106, 103, 100,
97, 95, 92, 89, 86, 83, 80, 77, 74, 71, 68, 65, 62, 59, 56, 53, 
49, 46, 43, 40, 37, 34, 31, 28, 25, 21, 18, 15, 12, 9, 6, 3
};

/*
char radiodata[16][16] =
{
   {"                "},
   {"      000       "},
   {"    0000000     "},
   {"  00000000000   "},
   {" 0000000000000  "},
   {" 0000000000000  "},
   {"000000000000000 "},
   {"000000000000000 "},
   {"000000000000000 "},
   {"000000000000000 "},
   {" 0000000000000  "},
   {" 0000000000000  "},
   {"  00000000000   "},
   {"    0000000     "},
   {"      000       "},
   {"                "}
};
*/

/* create scheme/gradient/border from above grdata[] */

int create_default_gradient()
{
   int cx, x = 0;
   int numgradients;       /* usually 24 but who knows? */

   numschemes   = grdata[x++];
   numgradients = grdata[x++];
   numranges    = grdata[x++];
   numcolours   = grdata[x++];
   /* schemes */
   for (cx = 0; cx < numschemes; cx++)
   {
      scheme[cx].colour   = grdata[x++];
      scheme[cx].colourto = grdata[x++];
      scheme[cx].txtcolour  = grdata[x++];
      scheme[cx].method     = grdata[x++];
      scheme[cx].txtshadow  = 0x7f7f7f;
      scheme[cx].colourbi   = scheme[cx].colourto;
      scheme[cx].colourbi2  = scheme[cx].colour;
   }
   /* disabled scheme guesswork */
   scheme[cx].colour   = 0x80baf6;
   scheme[cx].colourto = 0xa0caf6;
   scheme[cx].txtcolour  = 0x7f7f7f;
   scheme[cx].method     = RGV;
   scheme[cx].txtshadow  = 0xd0daf6;
   scheme[cx].colourbi   = scheme[cx].colourto;
   scheme[cx].colourbi2  = scheme[cx].colour;
   numschemes++;
   /* gradients */
   for (cx = 0; cx < numgradients; cx++)
   {
      gradient[cx].bmp = NULL;
      gradient[cx].fcs = grdata[x++];
      gradient[cx].ucs = grdata[x++];
      gradient[cx].pcs = grdata[x++];
      gradient[cx].method  = grdata[x++];
      gradient[cx].umethod = grdata[x++];
      gradient[cx].pmethod = grdata[x++];
      gradient[cx].justify = grdata[x++];
      gradient[cx].dcs = numschemes-1;
      gradient[cx].dmethod = gradient[cx].umethod;
   }
// Border colours
   borderstyle.bordercolour = grdata[x++];
   borderstyle.borderwidth  = grdata[x++];
   borderstyle.bevelwidth   = grdata[x++];
   borderstyle.handlewidth  = grdata[x++];
   borderstyle.bulletstyle  = grdata[x++];
   borderstyle.bulletpos    = grdata[x++];
   /* colour ranges */
   for (cx = 0; cx < numranges; cx++)
   {
      logrange[cx][0] = grdata[x++];
      logrange[cx][1] = grdata[x++];
   }
   /* Individual colours */
   for (cx = 0; cx < numcolours; cx++)
   {
      colRGB[cx] = grdata[x++];
   }
   update_all_colours();   /* copy schemes to gradient colours */

   if (colRGB[numcolours - 1] != 0xffffff) return -1; /* sanity check */
   return x;
}


/* update gradient colour entries fro the colourschemes */

void update_gradient_colours(GRADIENT *grad, COLOURSCHEME *scheme)
{
   int num, sch;

   for (num = 0; num < MAXGRADIENTS; num++)
   {
      sch = grad[num].fcs;
      if (sch >= MAXCOLOURSCHEMES || sch < 0) sch = MAXCOLOURSCHEMES - 1;
      grad[num].colour    = scheme[sch].colour;
      grad[num].colourto  = scheme[sch].colourto;
      grad[num].txtcolour = scheme[sch].txtcolour;
      grad[num].txtshadow = scheme[sch].txtshadow;
      grad[num].colourbi  = scheme[sch].colourbi;
      grad[num].colourbi2 = scheme[sch].colourbi2;
      sch = grad[num].ucs;
      if (sch >= MAXCOLOURSCHEMES || sch < 0) sch = MAXCOLOURSCHEMES - 1;
      grad[num].ucolour    = scheme[sch].colour;
      grad[num].ucolourto  = scheme[sch].colourto;
      grad[num].utxtcolour = scheme[sch].txtcolour;
      grad[num].utxtshadow = scheme[sch].txtshadow;
      grad[num].ucolourbi  = scheme[sch].colourbi;
      grad[num].ucolourbi2 = scheme[sch].colourbi2;
      sch = grad[num].pcs;
      if (sch >= MAXCOLOURSCHEMES || sch < 0) sch = MAXCOLOURSCHEMES - 1;
      grad[num].pcolour    = scheme[sch].colour;
      grad[num].pcolourto  = scheme[sch].colourto;
      grad[num].ptxtcolour = scheme[sch].txtcolour;
      grad[num].ptxtshadow = scheme[sch].txtshadow;
      grad[num].pcolourbi  = scheme[sch].colourbi;
      grad[num].pcolourbi2 = scheme[sch].colourbi2;
      sch = grad[num].dcs;
      if (sch >= MAXCOLOURSCHEMES || sch < 0) sch = MAXCOLOURSCHEMES - 1;
      grad[num].dcolour    = scheme[sch].colour;
      grad[num].dcolourto  = scheme[sch].colourto;
      grad[num].dtxtcolour = scheme[sch].txtcolour;
      grad[num].dtxtshadow = scheme[sch].txtshadow;
      grad[num].dcolourbi  = scheme[sch].colourbi;
      grad[num].dcolourbi2 = scheme[sch].colourbi2;
   }
}


  /* update default gradient colours from schemes */

void update_all_colours()  /* update default gradient colours from schemes */
{
   update_gradient_colours(gradient, scheme);
}


/* ********* INITIALIZE FUNCTIONS   *********** */


/* Initialize the gradient system.
   This must be called first to set up the defaults.
   Without it, you'll have black text on black boxes.

   Sets up a nice (I think) ice blue gradient.
   Returns 0 if okay, -1 if there is a problem */

int init_gradient()    /* set up the default gradient */
{
   int x;

   x = create_default_gradient();
   set_border_style(&borderstyle);
   return x;
}


/* search for colours and put them in a colour list */

int find_colours(COLOURSCHEME *sch)
{
   int cx, cz, x, c1, numcols = 0;

   for (cx = 0; cx < numschemes; cx++)
   {
      x = -1;
      c1 = sch[cx].colour;
      for (cz = 0; cz < numcols; cz++)
      {
         if (colRGB[cz] == c1) x++;
      }
      if (x < 0)
      {
         colRGB[numcols] = c1;
         numcols++;
      }
      x = -1;
      c1 = sch[cx].colourto;
      for (cz = 0; cz < numcols; cz++)
      {
         if (colRGB[cz] == c1) x++;
      }
      if (x < 0)
      {
         colRGB[numcols] = c1;
         numcols++;
      }
      x = -1;
      c1 = sch[cx].txtcolour;
      for (cz = 0; cz < numcols; cz++)
      {
         if (colRGB[cz] == c1) x++;
      }
      if (x < 0)
      {
         colRGB[numcols] = c1;
         numcols++;
      }
      x = -1;
      c1 = sch[cx].txtshadow;
      for (cz = 0; cz < numcols; cz++)
      {
         if (colRGB[cz] == c1) x++;
      }
      if (x < 0)
      {
         colRGB[numcols] = c1;
         numcols++;
      }
      x = -1;
      c1 = sch[cx].colourbi;
      for (cz = 0; cz < numcols; cz++)
      {
         if (colRGB[cz] == c1) x++;
      }
      if (x < 0)
      {
         colRGB[numcols] = c1;
         numcols++;
      }
      x = -1;
      c1 = sch[cx].colourbi2;
      for (cz = 0; cz < numcols; cz++)
      {
         if (colRGB[cz] == c1) x++;
      }
      if (x < 0)
      {
         colRGB[numcols] = c1;
         numcols++;
      }
   }
   return numcols;
}

/* pick a colour between two colours c1 and c2 in RRGGBB format.
   The new colour will be a ratio from c1 towards c2. The ratio is 0-255.
   Returns the new colour in RRGGBB format. */

int get_new_colour(int c1, int c2, int ratio)
{
   int r, g, b, r1, g1, b1, x;

   r  = (c1 & 0xFF0000) >> 16;
   g  = (c1 & 0xFF00) >> 8;
   b  = c1 & 0xFF;
   r1 = (c2 & 0xFF0000) >> 16;
   g1 = (c2 & 0xFF00) >> 8;
   b1 = c2 & 0x00FF;
   x = ((r1 - r) * ratio) / 255;
   r += x;
   x = ((g1 - g) * ratio) / 255;
   g += x;
   x = ((b1 - b) * ratio) / 255;
   b += x;
   return (r << 16) + (g << 8) + b;
}


/* ********* THEME/GRADIENT FILE FUNCTIONS   *********** */

/* GRADIENT - save and load routines */

int save_gradient(char *fname)     /* save internal gradient[] to a file */
{
   return export_gradient(fname, gradient, scheme, &borderstyle);
}



int export_gradient(char *fname, GRADIENT *grad,
      COLOURSCHEME *scheme, BORDERSTYLE *bstyle)
{
   int cx, cy, c1, c2, tmp;
   FILE *fp;

   count_colour_schemes(grad);  /* find last scheme actually used? */

   fp = fopen(fname, "wt");

   fprintf(fp, "/// Gradient Theme File: %s\n//\n", fname);
   fprintf(fp, "// Lines starting with / ! ; are ignored\n!\n");
   fprintf(fp, "/  Lines starting with A B C G R S are accepted.\n!\n");
   fprintf(fp, "!  A       Author credits, 3 lines, first 56 characters.\n");
   fprintf(fp, "!  B       Border colours and sizes.\n");
   fprintf(fp, "!  C       Colours in #RRGGBB format, MAX is 99.\n");
   fprintf(fp, "!  R       Colour Ranges     MAX is 99.\n");
   fprintf(fp, "!  S       Colour Schemes,   MAX is 99.\n");
   fprintf(fp, "!  G       Gradient specifications, 24.\n!\n");

   for (cx = 0; cx < 3; cx++)   fprintf(fp, "A %s\n", authortext[cx]);

   fprintf(fp, "!\n! NOTE Colour Schemes MUST come before Gradients!\n!\n");
   fprintf(fp, "!\n// #  colour  colourto  textcol  tshadow  colourbi colourbi2\n");

   for (cx = 0; cx < numschemes; cx++)
   {
      fprintf(fp, "S %2d  #%06x  #%06x  #%06x  #%06x  #%06x  #%06x\n",
         cx, scheme[cx].colour, scheme[cx].colourto, scheme[cx].txtcolour,
         scheme[cx].txtshadow,  scheme[cx].colourbi, scheme[cx].colourbi2);
   }
   fprintf(fp, "//\n// Total colour schemes: %d \n//\n", numschemes);
   fprintf(fp, "//\n// GRADIENT entries\n//\n");
   fprintf(fp, "// Colourscheme: Focus Unfocus Pressed Disabled fc uc pc dc\n");
   fprintf(fp, "// Gradients:    Focus Unfocus Pressed Disabled + Justify\n");
   fprintf(fp, "// # fc uc pc dc   fg   ug   pg   dg  jst\n");

   for (cx = 0; cx < 24; cx++)
   {
      fprintf(fp, "G %2d %2d %2d %2d %2d %4d %4d %4d %4d %2d // %s\n",
      cx, grad[cx].fcs, grad[cx].ucs, grad[cx].pcs, grad[cx].dcs,
      grad[cx].method, grad[cx].umethod, grad[cx].pmethod, grad[cx].dmethod,
      grad[cx].justify, gradient_name[cx]);
   }
   fprintf(fp, "//\n//\n");
   fprintf(fp, "// bordercolour %06x\n",   bstyle->bordercolour);
   fprintf(fp, "// borderwidth  %d\n",     bstyle->borderwidth);
   fprintf(fp, "// bevelwidth   %d\n",     bstyle->bevelwidth);
   fprintf(fp, "// handlewidth  %d\n", bstyle->handlewidth);
   fprintf(fp, "// bullet style %d\n", bstyle->bulletstyle);
   fprintf(fp, "// bullet position %d 0=left 2=right\n//\n", bstyle->bulletpos);
   fprintf(fp, "B #%06x %d %d %d %d %d // Bcolour Bwidth Bevel Handle Bullet Bpos\n//\n",
      bstyle->bordercolour, bstyle->borderwidth, bstyle->bevelwidth,
      bstyle->handlewidth, bstyle->bulletstyle, bstyle->bulletpos);

   fprintf(fp, "//\n// [optional] Colour Range information\n");
   fprintf(fp, "// This can be used to optimize an 8 bit palette\n//\n");
   numranges = 0;
   for (cx = 0; cx < numschemes; cx++)
   {
      tmp = -1;
      c1 = scheme[cx].colour;
      c2 = scheme[cx].colourto;
      for (cy = 0; cy < numranges; cy++)
      {
        if ((logrange[cy][0] == c1) && (logrange[cy][1] == c2)) tmp++;
        if ((logrange[cy][1] == c1) && (logrange[cy][0] == c2)) tmp++;
      }
      if (tmp < 0)
      {
         logrange[numranges][0] = c1;
         logrange[numranges][1] = c2;
         fprintf(fp, "R %2d  #%06x  #%06x\n", numranges, c1, c2);
         numranges++;
      }
   }
   fprintf(fp, "//\n// Total colour ranges: %d \n//\n", numranges);

   numcolours = find_colours(scheme);    /* update: in case of edits */

   fprintf(fp, "//\n// [optional] Colours actually used\n");
   fprintf(fp, "// in constructing this gradient\n//\n");
   for (cx = 0; cx < numcolours; cx++)
   {
      fprintf(fp, "C %2d  #%06x\n", cx, colRGB[cx]);
   }
   fprintf(fp, "//\n// Total colours: %d \n//\n", numcolours);

   fclose(fp);

   return 0;
}



/* code to read the ASCII text colour-scheme/gradient file, above */

/* read a gradient theme into the default gradient/colourscheme/border
   structures and sets the new borderstyle. */

int read_gradient(char *fname)
{
   int x;

   x = import_gradient(fname, gradient, scheme, &borderstyle);
   if (x == 0)   set_border_style(&borderstyle);
   return x;
}


/* read the theme/gradient text file, into the specified structures */

int import_gradient(char *fname, GRADIENT *grad,
    COLOURSCHEME *scheme, BORDERSTYLE *bstyle)
{
   int cx, i, cs, num, max, anum = 0;
   int c1, c2, c3, c4, c5, c6, col=0, fcs, ucs, pcs, dcs, jst;
   int fg, ug, pg, dg;
   FILE *fp;

   fp = fopen(fname, "rt");
   if (fp == NULL)
   {
      sprintf(themebuf, "FAILED to find %s", fname);
      return 1;
   }
   /* set up flags if disabled gradient is missing */
   for (cx = 0; cx < MAXGRADIENTS; cx++)     grad[cx].dcs = 0;
   for (cx = 0; cx < MAXCOLOURSCHEMES; cx++) scheme[cx].method = 0;

   numcolours = 0;
   numschemes = 0;
   numranges  = 0;

   while (fgets(themebuf, 200, fp) != 0)
   {
      if (strlen(themebuf) > 2 && *themebuf != '/' &&
           *themebuf != '!' && *themebuf != ';')
      {
         if (*themebuf == 'A')    /* Author! Author! credits... */
         {
            max = strlen(themebuf);
            if (max > 62) max = 62;
            for (cx = 2; cx < max; cx++)
            {
               authortext[anum][cx-2] = themebuf[cx];
               if (themebuf[cx] == 9)                /* check tabs */
                  authortext[anum][cx-2] = ' ';
               if (themebuf[cx] == 10)               /* check DOS CR EOL */
                  authortext[anum][cx-2] = 0;
            }
            authortext[anum][cx-2] = 0;
            anum++;
            if (anum > 2) anum = 2;
         }
         if (*themebuf == 'B')         /* border details */
         {
            i = sscanf(themebuf+1, " #%x %d %d %d %d %d",
                  &col, &c1, &c2, &c3, &c4, &c5);
            bstyle->bordercolour = col;
            if (i > 1)   bstyle->borderwidth  = c1;
            if (i > 2)   bstyle->bevelwidth   = c2;
            if (i > 3)   bstyle->handlewidth  = c3;
            if (i > 4)   bstyle->bulletstyle  = c4;
            if (i > 5)   bstyle->bulletpos    = c5;
         }
         if (*themebuf == 'C')          /* colours */
         {
            sscanf(themebuf+1, " %d #%x", &col, &c1);
            colRGB[col] = c1;
            if (col >= numcolours)  numcolours = col+1;
         }
         if (*themebuf == 'S')          /* scheme - colour combinations */
         {                              /* stored as #RRGGBB hex */
            i = sscanf(themebuf+1, " %d #%x #%x #%x #%x #%x #%x",
                        &col, &c1, &c2, &c3, &c4, &c5, &c6);
            if (col >= MAXCOLOURSCHEMES || col < 0) col = MAXCOLOURSCHEMES-1;
            scheme[col].colour   = c1;
            scheme[col].colourto = c2;
            scheme[col].txtcolour  = c3;
            /* adjust old format file by inventing txtshadow... etc */
            if (i > 4)  scheme[col].txtshadow = c4;
            else        scheme[col].txtshadow = 0x7f7f7f;
            if (i > 5)  scheme[col].colourbi = c5;
            else        scheme[col].colourbi = scheme[col].colourto;
            if (i > 6)  scheme[col].colourbi2 = c6;
            else        scheme[col].colourbi2 = scheme[col].colour;
            if (col >= numschemes)  numschemes = col+1;
         }
         if (*themebuf == 'G')          /* gradient elements */
         {
            i = sscanf(themebuf+1, " %d %d %d %d %d %d %d %d %d %d",
               &col, &fcs, &ucs, &pcs, &dcs, &fg, &ug, &pg, &dg, &jst);

            if (col >= MAXGRADIENTS || col < 0)  col = MAXGRADIENTS - 1;
            grad[col].fcs = fcs % MAXCOLOURSCHEMES;
            grad[col].ucs = ucs % MAXCOLOURSCHEMES;
            grad[col].pcs = pcs % MAXCOLOURSCHEMES;
            /* adjust for old/new file format (old didnt have disabled) */
            if (i < 9)    /* old format - fcs ucs pcs  fg ug pg jst */
            {             /* maps to      fcs ucs pcs dcs fg ug  pg dg jst */
               if (i > 5)  grad[col].method = dcs;
               if (i > 6)  grad[col].umethod = fg;
               if (i > 7)  grad[col].pmethod = ug;
               if (i > 8)  grad[col].justify = pg;
               grad[col].dmethod = grad[col].umethod;
               /* no disabled scheme so make up our own later on */
               grad[col].dcs = 99;    // was  grad[col].ucs;
               scheme[ucs].method++;  // flag the unfocussed colour scheme
               dcs = ucs;             // fix up incorrect dcs
            }
            else
            {
               grad[col].dcs = dcs % MAXCOLOURSCHEMES;
               grad[col].method = fg;
               grad[col].umethod = ug;
               grad[col].pmethod = pg;
               grad[col].dmethod = dg;
               if (i > 9)  grad[col].justify = jst;
            }
         }
         if (*themebuf == 'R')          /* optional colour ranges */
         {
            sscanf(themebuf+1, " %d #%x #%x", &col, &c1, &c2);
            if (col >= 100 || col < 0)   col = 99;
            logrange[col][0] = c1;
            logrange[col][1] = c2;
            if (col >= numranges)  numranges = col+1;
         }
      }
   }
   fclose(fp);

   /* check if new disabled schemes are required for old format files */
   num = numschemes;

   for (cx = 0; cx < numschemes; cx++)
   {
      if (scheme[cx].method)     /* has it been flagged for processing? */
      {
         scheme[num].colour = get_new_colour(scheme[cx].colour,
                      scheme[cx].colourto, 100);
         scheme[num].colourto = get_new_colour(scheme[cx].colour,
                      scheme[cx].colourto, 200);
         scheme[num].txtcolour = get_new_colour(scheme[cx].txtcolour,
                                   scheme[cx].txtshadow, 100);
         scheme[num].txtshadow = get_new_colour(scheme[cx].txtcolour,
                                   scheme[cx].txtshadow, 200);
         scheme[num].colourbi = get_new_colour(scheme[cx].colourbi,
                      scheme[cx].colourbi2, 100);
         scheme[num].colourbi2 = get_new_colour(scheme[cx].colourbi,
                      scheme[cx].colourbi2, 200);
         scheme[cx].method = num;    /* store disabled scheme number */
         num++;
      }
   }
   /* fix up gradients */
   for (cx = 0; cx < MAXGRADIENTS; cx++)
   {
      cs = grad[cx].ucs;
      if (grad[cx].dcs == 99)        /* disabled colour scheme missing? */
      {
         grad[cx].dcs = scheme[cs].method;
         if (grad[cx].dcs >= numschemes) numschemes = grad[cx].dcs + 1;
      }
   }
   update_gradient_colours(grad, scheme);   /* fix all gradient colours */
   for (cx = 0; cx < numschemes; cx++)  scheme[cx].method = RGV;

   sprintf(themebuf, "%s Gradient loaded.", fname);
   return 0;
}


/* *************** BORDER FUNCTIONS   **************** */

/* set the current border style to the contents of the BORDERSTYLE struct
   Returns bordercolour in the current drawing bitdepth */

int set_border_style(BORDERSTYLE *bstyle)
{
   int x;

   borderwidth  =  bstyle->borderwidth;
   bordercolour =  bstyle->bordercolour;
   bevelwidth   =  bstyle->bevelwidth;
   handlewidth  =  bstyle->handlewidth;
   bulletstyle  =  bstyle->bulletstyle;
   bulletpos    =  bstyle->bulletpos;
   gripwidth    = 20;

   x = bordercolour;
   bordercolour = makecol((x & 0xFF0000) >> 16, (x & 0xFF00) >> 8, x & 0xFF);

   return bordercolour;
}



/* *************** GRADIENT DRAWING FUNCTIONS   **************** */


/* fill a solid box with sanity check: col is in current drawing colordepth */

int draw_gradient_solid(BITMAP *bmp, int xd, int yd, int wd, int hd, int col)
{
   if (wd == 0 || hd == 0 || bmp == NULL) return -1;

   rectfill(bmp, xd, yd, xd+wd-1, yd+hd-1, col);
   return 0;
}



/* raised bevel: add 20% to each pixel top and left
                 sub 20% from pixels bottom and right
   sunken bevel is the reverse.
*/

int draw_gradient_bevel(BITMAP *bmp, int xd, int yd, int wd, int hd, int type)
{
   int cx, cy, f1, f2, col, oldpix = 0;
   int r, g, b, r1, g1, b1, newpix = 0;

   f1 = 200;    /* assume SUNKENDRAW */
   f2 = 300;
   if (type == RAISEDDRAW)
   {
      f1 = 300;
      f2 = 200;
   }
   if (type == RAISEDDRAWTWO)
   {
      f1 = 350;
      f2 = 150;
   }
   if (type == SUNKENDRAWTWO)
   {
      f1 = 150;
      f2 = 350;
   }
   for (cx = 0; cx < wd; cx++)          /* top line */
   {
      col = getpixel(bmp, xd+cx, yd);
      if (col != oldpix)
      {
         oldpix = col;
         r1 = getr(col);          /* reprocess only if pixel has changed */
         g1 = getg(col);
         b1 = getb(col);
         r = (r1 * f1) / 256;
         if (r > 255) r = 255;
         g = (g1 * f1) / 256;
         if (g > 255) g = 255;
         b = (b1 * f1) / 256;
         if (b > 255) b = 255;
         newpix = makecol(r, g, b);
      }
      putpixel(bmp, xd+cx, yd, newpix);
   }
   oldpix = 0;                         /* bug fix! */
   newpix = 0;
   for (cx = 0; cx < wd; cx++)
   {
      col = getpixel(bmp, xd+cx, yd+hd-1);      /* Oh! Bottom! */
      if (col != oldpix)
      {
         oldpix = col;        /* reprocess only if pixel has changed */
         r1 = getr(col);
         g1 = getg(col);
         b1 = getb(col);
         r = (r1 * f2) / 256;
         if (r > 255) r = 255;
         g = (g1 * f2) / 256;
         if (g > 255) g = 255;
         b = (b1 * f2) / 256;
         if (b > 255) b = 255;
         newpix = makecol(r, g, b);
      }
      putpixel(bmp, xd+cx, yd+hd-1, newpix);
   }

   oldpix = 0;
   newpix = 0;
   for (cy = 1; cy < hd-1; cy++)           /* lefty pix */
   {
      col = getpixel(bmp, xd, yd+cy);
      if (col != oldpix)
      {
         oldpix = col;
         r1 = getr(col);
         g1 = getg(col);
         b1 = getb(col);
         r = (r1 * f1) / 256;
         if (r > 255) r = 255;
         g = (g1 * f1) / 256;
         if (g > 255) g = 255;
         b = (b1 * f1) / 256;
         if (b > 255) b = 255;
         newpix = makecol(r, g, b);
      }
      putpixel(bmp, xd, yd+cy, newpix);
   }
   oldpix = 0;
   newpix = 0;
   for (cy = 1; cy < hd-1; cy++)            /* eyes right */
   {
      col = getpixel(bmp, xd+wd-1, yd+cy);
      if (col != oldpix)
      {
         oldpix = col;
         r1 = getr(col);
         g1 = getg(col);
         b1 = getb(col);
         r = (r1 * f2) / 256;
         if (r > 255) r = 255;
         g = (g1 * f2) / 256;
         if (g > 255) g = 255;
         b = (b1 * f2) / 256;
         if (b > 255) b = 255;
         newpix = makecol(r, g, b);
      }
      putpixel(bmp, xd+wd-1, yd+cy, newpix);
   }

   return 0;
}

/* helper function to bevel a gradient */

int do_bevels(BITMAP *bmp, int xd, int yd, int wd, int hd, int edge, int bevel)
{
   /* We do this a pixel at a time, in case it's a graduated line...
      if double bevelled, we do this twice... what a pain! */

   if (edge & GRADIENTRAISED)
   {
      if (bevel & GRADIENTBEVEL2)
      {
         draw_gradient_bevel(bmp, xd, yd, wd, hd, RAISEDDRAWTWO);
         xd++;
         yd++;
         wd -= 2;
         hd -= 2;
      }
      draw_gradient_bevel(bmp, xd, yd, wd, hd, RAISEDDRAW);
      xd++;
      yd++;
      wd -= 2;
      hd -= 2;
   }
   if (edge & GRADIENTSUNK)
   {
      if (bevel & GRADIENTBEVEL2)
      {
         draw_gradient_bevel(bmp, xd, yd, wd, hd, SUNKENDRAWTWO);
         xd++;
         yd++;
         wd -= 2;
         hd -= 2;
      }
      draw_gradient_bevel(bmp, xd, yd, wd, hd, SUNKENDRAW);
   }
   return 0;
}



/* draw a gradient filled object on the given bitmap, at xd, yd
   width wd & height hd, colour1 merging to colour2.
   using the specified drawing method.

   colour1, colour2 in RRGGBB format.

   method is a bitfield combination of:
   edge  = method & 3;       0=flat 1=raised 2=sunk 3=lipped
   style = method & 12;      0=nodraw 4=skinned 8=gradient 12=mapped
   grad  = method & 0x0F0;   0=solid 16=horiz 32=vert 48=diag 64=polar..
   bevel = method & 0xFF00;  0=std 256=bevel2 512=interlace 1024=rivets
                             2048=transparent

   If style is nodraw, nothing is done!
   This function relies on the 'style' portion == 8 ( GRADIENTTEXTURE )
   See gradient.h for appropriate defines for this bitfield.

   This one function does all - 450 lines!

   Is this better than a dozen functions and 1200 lines all up?

   Programmers note: With the exception of SOLID and VERTICAL
   gradients, this function does it the hard way, one pixel
   at a time. It should be possible to optimize this using the
   allegro write_line() calls. I have not tried this with
   3.9.32 (the one I use).

   If you use this with large areas (particularly with a 486) it is
   much beter to draw the gradients to a memory bitmap on startup,
   then blit to the screen when required. This will be faster than
   drawing it pixel-by-pixel every time.
*/

int draw_colour_gradient(BITMAP *bmp, int xd, int yd, int wd, int hd,
          int colour1, int colour2, int method)
{
   int cx = 0, cy = 0, x = 0, y = 0, w = 0, h = 0, col = 0, icol = 0, col1 = 0, col2 = 0, oldpix = 0;
   int r1 = 0, r2 = 0, b1 = 0, b2 = 0, g1 = 0, g2 = 0, rd = 0, gd = 0, bd = 0, r3 = 0, g3 = 0, b3 = 0;
   int r = 0, g = 0, b = 0, ry = 0, rx = 0, ryc = 0, ryb = 0, rxb = 0, rxp = 0, ryp = 0, rr = 0;
   int style = 0, edge = 0, grad = 0, bevel = 0, ilace = 0;
//   int rad, dp;

   if (wd < 1 || hd < 1 || bmp == NULL) return -1;

   edge  = method & 3;      /* 0=flat 1=raised 2=sunk 3=lipped */
   style = method & 12;     /* 0=nodraw 4=skinned 8=gradient 12=mapped */
   grad  = method & 0x0F0;  /* 0=solid 16=horiz 32=vert 48=diag 64=polar.. */
   bevel = method & 0xFF00; /* 0=std 256=bevel2 512=interlace 1024=rivets
                               2048=transparent... */

   if (grad > 176)  grad = 48;  /* substitute diagonal for >= bilinear */
                                /* because they are don't belong here! */
   if (style == 0) return -1;     /* oops... nothing to do! */

   x = xd;
   y = yd;
   w = wd;
   h = hd;
   r1 = (colour1 & 0xFF0000) >> 16;
   g1 = (colour1 & 0xFF00)   >> 8;
   b1 = colour1 & 0xFF;
   r2 = (colour2 & 0xFF0000) >> 16;
   g2 = (colour2 & 0xFF00)   >> 8;
   b2 = colour2 & 0xFF;

   rd = r2 - r1;
   gd = g2 - g1;
   bd = b2 - b1;
   col1 = makecol(r1, g1, b1);

   /* configure interlace function */
   ilace = 2000;                   /* bugfix, was 0, and caused problems */
   if (bevel & GRADIENTINTERLACE)
   {
      ilace = 2;
      if (bevel & GRADIENTRIVETED) ilace = 3;
   }
   else    if (bevel & GRADIENTRIVETED) ilace = 4;

   /* first, draw quickies */

   if (grad == GRADIENTSOLID && ((bevel & GRADIENTTRANSPARENT) == 0))
   /* draw solid or interlaced solid */
   {
      /* TODO: check for transparency */
      rectfill(bmp, x, y, x+w-1, y+h-1, col1);
      if (ilace < 4)
      {
         r3 = (r1 * 200)/256;         /* solid interlace colour */
         b3 = (b1 * 200)/256;
         g3 = (g1 * 200)/256;
         icol = makecol(r3, g3, b3);

         y = yd;
         for (cy = 0; cy < hd; cy++)
         {
            if ((cy % ilace == 0) && (cy > 0))
               hline(bmp, xd, y, xd+wd-1, icol);
            y++;
         }
      }
      if (ilace == 4) /* pinholes: If SOLID, colour is constant all lines */
      {
         /* since this is a solid, every pixel is r1 g1 b1 col1 */
         col2 = makecol((r1*200)/256, (g1*200)/256, (b1*200)/256);
         r = (r1 * 300) / 256;
         g = (g1 * 300) / 256;
         b = (b1 * 300) / 256;
         if (r > 255) r = 255;
         if (g > 255) g = 255;
         if (b > 255) b = 255;
         col = makecol(r, g, b);
         for (cy = 3; cy < hd-3; cy += 4)
         {
            y = yd+cy;
            for (cx = 3; cx < wd-3; cx += 4)
            {                   /* rivets...  bright/dark pixels */
               x = xd + cx;
               putpixel(bmp, x++, y, col);
               putpixel(bmp, x, y+1, col2);
            }
         }
      }
      do_bevels(bmp, xd, yd, wd, hd, edge, bevel);
      return 0;
   }
   if (grad == GRADIENTRECTANGLE)
   {
      /* TODO: check for transparency */
      if (w >= h)
      {
         for (cx = 0; cx < w/2; cx++)
         {
            cy = (cx * h) / w;
            rx = (512 * cx) / w;
            r = r1 + rd * rx / 256;
            g = g1 + gd * rx / 256;
            b = b1 + bd * rx / 256;
            rect(bmp, x+cx, y+cy, x+w-1-cx, y+h-1-cy, makecol(r,g,b));
         }
      }
      else
      {
         for (cy = 0; cy < h/2; cy++)
         {
            cx = (cx * w) / h;
            rx = (512 * cy) / h;
            r = r1 + rd * rx / 256;
            g = g1 + gd * rx / 256;
            b = b1 + bd * rx / 256;
            rect(bmp, x+cx, y+cy, x+w-1-cx, y+h-1-cy, makecol(r,g,b));
         }
      }
      /* after-market interlace */
      if (ilace < 4)
      {
         for (cy = ilace; cy < hd-2; cy+=ilace)
         {
            oldpix = 0;                         /* bug fix! */
            col2 = 0;
            for (cx = 0; cx < wd; cx++)
            {
               col = getpixel(bmp, xd+cx, yd+cy);
               if (col != oldpix)
               {
                  oldpix = col;   /* reprocess only if pixel has changed */
                  r1 = getr(col);
                  g1 = getg(col);
                  b1 = getb(col);
                  r = (r1 * 200) / 256;
                  g = (g1 * 200) / 256;
                  b = (b1 * 200) / 256;
                  col2 = makecol(r, g, b);
               }
               putpixel(bmp, xd+cx, yd+cy, col2);
            }
         }
      }
      if (ilace == 4)           /* rivet...rivet */
      {
         for (cy = ilace; cy < hd-2; cy+=ilace)
         {
            oldpix = 0;                         /* bug fix! */
            col2 = 0;
            col1 = 0;
            for (cx = ilace; cx < wd-3; cx+=ilace)
            {
               col = getpixel(bmp, xd+cx, yd+cy);
               if (col != oldpix)
               {
                  oldpix = col;   /* reprocess only if pixel has changed */
                  r1 = getr(col);
                  g1 = getg(col);
                  b1 = getb(col);
                  r = (r1 * 300) / 256;
                  if (r > 255) r = 255;
                  g = (g1 * 300) / 256;
                  if (g > 255) g = 255;
                  b = (b1 * 300) / 256;
                  if (b > 255) b = 255;
                  col2 = makecol(r, g, b);
                  col1 = makecol((r1*200)/256, (g1*200)/256, (b1*200)/256);
               }
               putpixel(bmp, xd+cx, yd+cy, col2);
               putpixel(bmp, xd+cx+1, yd+cy, col1);
            }
         }
      }
   }
   else        /* handle other gradients: horiz vert diag polar */
   {
      rr = (int)hypot(wd / 2.0, hd / 2.0); /* polar hypotenuse */
      y = yd;
      for (cy = 0; cy < hd; cy++)
      {
         x = xd;
         if (hd < 2)             /* avoid divide by zero errors */
         {
            ry = ryc = 128;
         }
         else             /* vertical ratio as part of 256 */
         {
            ry = (256 * cy) / (hd - 1);             /* linear */
            ryc = (256 * (h-cy-1)) / (hd - 1);      /* reverse linear */
            ryb = sinetable[ry & 0xFF];             /* barrel, pipe */
            ryp = (512 * cy) / (hd - 1);            /* pyramid */
            if (cy >= hd / 2) ryp = (512 * (hd - cy -1) / (hd - 1));
         }
         /* check if this is an interlace/Rivets line */
         if (ilace < 5 && (cy % ilace == 0) && cy > 0 && cy < hd-2)
         {
            for (cx = 0; cx < wd; cx++)    /* yes it is */
            {
               r = r1;
               g = g1;
               b = b1;
               /* find r g b at each pixel */
               if (wd < 2)
               {
                  rx = 128;
                  rxp = 256;
               }
               else
               {
                  rx = (256 * cx)/ (wd-1);
                  rxp = (512 * cx) / (wd - 1);
                  if (cx >= wd / 2)  rxp = (512 * (wd - cx -1) / (wd - 1));
               }
               rxb = sinetable[rx & 0xFF];
               if (grad == GRADIENTSOLID)
               {
                  r = r1;
                  g = g1;
                  b = b1;
               }
               /* horiz: */
               if (grad == GRADIENTHORIZONTAL)
               {
                  r = r1 + rd * rx / 256;
                  g = g1 + gd * rx / 256;
                  b = b1 + bd * rx / 256;
               }
               if (grad == GRADIENTCYLINDRICAL)
               {
                  r = r1 + rd * rxb / 256;
                  g = g1 + gd * rxb / 256;
                  b = b1 + bd * rxb / 256;
               }
               /* vert:  */
               if (grad == GRADIENTVERTICAL)
               {
                  r = r1 + rd * ry / 256;
                  g = g1 + gd * ry / 256;
                  b = b1 + bd * ry / 256;
               }
               if (grad == GRADIENTBARREL)
               {
                  r = r1 + rd * ryb / 256;
                  g = g1 + gd * ryb / 256;
                  b = b1 + bd * ryb / 256;
               }
               /* diag:  */
               if (grad == GRADIENTDIAGONAL)
               {
                  r = r1 + rd * (rx + ry) / 512;
                  g = g1 + gd * (rx + ry) / 512;
                  b = b1 + bd * (rx + ry) / 512;
               }
               /* cross diag:  */
               if (grad == GRADIENTCROSSDIAG)
               {
                  r = r1 + rd * (rx + ryc) / 512;
                  g = g1 + gd * (rx + ryc) / 512;
                  b = b1 + bd * (rx + ryc) / 512;
               }
               if (grad == GRADIENTPIPECROSS)
               {
                  if (rxb > ryb)
                  {
                     r = r1 + rd * rxb / 256;
                     g = g1 + gd * rxb / 256;
                     b = b1 + bd * rxb / 256;
                  }
                  else
                  {
                     r = r1 + rd * ryb / 256;
                     g = g1 + gd * ryb / 256;
                     b = b1 + bd * ryb / 256;
                  }
               }
               if (grad == GRADIENTELLIPTICAL)
               {
                  r = r1 + rd * (rxb + ryb) / 512;
                  g = g1 + gd * (rxb + ryb) / 512;
                  b = b1 + bd * (rxb + ryb) / 512;
               }
               if (grad == GRADIENTPYRAMID)
               {
//                  r = r1 + rd * (rxb * ryb) / 65536;
//                  g = g1 + gd * (rxb * ryb) / 65536;
//                  b = b1 + bd * (rxb * ryb) / 65536;
                  r = r1 + rd * (rxp + ryp) / 512;
                  g = g1 + gd * (rxp + ryp) / 512;
                  b = b1 + bd * (rxp + ryp) / 512;
               }
               /* polar: */
               if (grad == GRADIENTPOLAR)
               {
                  /*  original is better but takes too long
                  rad= (int)hypot((double)(wd/2.0 - cx), (double)(hd/2.0 - cy));
                  if (rad > rr)   rad = rr;
                  dp = (256 * rad) / rr;
                  r = r2 - (rd * dp) / 256;
                  g = g2 - (gd * dp) / 256;
                  b = b2 - (bd * dp) / 256;
                  */
                  r = r1 + rd * (rxb * ryb) / 65536;
                  g = g1 + gd * (rxb * ryb) / 65536;
                  b = b1 + bd * (rxb * ryb) / 65536;
               }
               if (r > 255) r = 255;
               if (g > 255) g = 255;
               if (b > 255) b = 255;
               if (r < 0) r = 0;
               if (g < 0) g = 0;
               if (b < 0) b = 0;
               /* handle interlace/Rivets: adjust rgb by 20% up or down */
               if (bevel & GRADIENTINTERLACE)
               {
                  r = (r * 200) / 256;
                  g = (g * 200) / 256;
                  b = (b * 200) / 256;
               }
               if (ilace == 4)   /* a rivetting experience? */
               {
                  if (cx > 1)
                  {
                     if (cx % ilace == 0)     /* brighten this pixel */
                     {
                        r = (r * 300) / 256;
                        if (r > 255) r = 255;
                        g = (g * 300) / 256;
                        if (g > 255) g = 255;
                        b = (b * 300) / 256;
                        if (b > 255) b = 255;
                     }
                     if (cx % ilace == 1)     /* darken this pixel */
                     {
                        r = (r * 200) / 256;
                        g = (g * 200) / 256;
                        b = (b * 200) / 256;
                     }
                  }
               }
               if (bevel & GRADIENTTRANSPARENT)
               {
                  /* merge rgb with background */
                  col = getpixel(bmp, x, y);
                  r += getr(col);
                  g += getg(col);
                  b += getb(col);
                  r /= 2;
                  g /= 2;
                  b /= 2;
               }
               putpixel(bmp, x, y, makecol(r, g, b));
               x++;
            }
         }
         else         /* non-interlaced lines */
         {
            for (cx = 0; cx < wd; cx++)
            {
               if (wd < 2)
               {
                  rx = 128;
                  rxp = 256;
               }
               else
               {
                  rx = (256 * cx)/ (wd-1);
                  rxp = (512 * cx) / (wd - 1);
                  if (cx >= wd / 2) rxp = (512 * (wd - cx -1) / (wd - 1));
               }
               rxb = sinetable[rx & 0xFF];
               /* find r g b at each pixel */
               /* horiz: */
               if (grad == GRADIENTHORIZONTAL)
               {
                  r = r1 + rd * rx / 256;
                  g = g1 + gd * rx / 256;
                  b = b1 + bd * rx / 256;
               }
               if (grad == GRADIENTSOLID)
               {
                  r = r1;
                  g = g1;
                  b = b1;
               }
               if (grad == GRADIENTCYLINDRICAL)
               {
                  r = r1 + rd * rxb / 256;
                  g = g1 + gd * rxb / 256;
                  b = b1 + bd * rxb / 256;
               }
               /* vert:  */
               if (grad == GRADIENTVERTICAL)
               {
                  r = r1 + rd * ry / 256;
                  g = g1 + gd * ry / 256;
                  b = b1 + bd * ry / 256;
               }
               if (grad == GRADIENTBARREL)
               {
                  r = r1 + rd * ryb / 256;
                  g = g1 + gd * ryb / 256;
                  b = b1 + bd * ryb / 256;
               }
               /* diag:  */
               if (grad == GRADIENTDIAGONAL)
               {
                  r = r1 + rd * (rx + ry) / 512;
                  g = g1 + gd * (rx + ry) / 512;
                  b = b1 + bd * (rx + ry) / 512;
               }
               /* cross diag:  */
               if (grad == GRADIENTCROSSDIAG)
               {
                  r = r1 + rd * (rx + ryc) / 512;
                  g = g1 + gd * (rx + ryc) / 512;
                  b = b1 + bd * (rx + ryc) / 512;
               }
               if (grad == GRADIENTPIPECROSS)
               {
                  if (rxb > ryb)
                  {
                     r = r1 + rd * rxb / 256;
                     g = g1 + gd * rxb / 256;
                     b = b1 + bd * rxb / 256;
                  }
                  else
                  {
                     r = r1 + rd * ryb / 256;
                     g = g1 + gd * ryb / 256;
                     b = b1 + bd * ryb / 256;
                  }
               }
               if (grad == GRADIENTELLIPTICAL)
               {
                  r = r1 + rd * (rxb + ryb) / 512;
                  g = g1 + gd * (rxb + ryb) / 512;
                  b = b1 + bd * (rxb + ryb) / 512;
               }
               if (grad == GRADIENTPYRAMID)
               {
                  /*
                  r = r1 + rd * (rxb * ryb) / 65536;
                  g = g1 + gd * (rxb * ryb) / 65536;
                  b = b1 + bd * (rxb * ryb) / 65536;
                  */
                  r = r1 + rd * (rxp + ryp) / 512;
                  g = g1 + gd * (rxp + ryp) / 512;
                  b = b1 + bd * (rxp + ryp) / 512;
                  /*
                  if (rxp > ryp)
                  {
                  r = r1 + rd * (rxp ) / 256;
                  g = g1 + gd * (rxp ) / 256;
                  b = b1 + bd * (rxp ) / 256;
                  }
                  else
                  {
                  r = r1 + rd * (ryp) / 256;
                  g = g1 + gd * (ryp) / 256;
                  b = b1 + bd * (ryp) / 256;
                  }
                  */
               }
               /* polar: */
               if (grad == GRADIENTPOLAR)
               {
                  /*  original was sharper but is too time consuming.
                  rad= (int)hypot((double)(wd/2.0 - cx), (double)(hd/2.0 - cy));
                  dp = (256 * rad) / rr;
                  r = r2 - (rd * dp) / 256;
                  g = g2 - (gd * dp) / 256;
                  b = b2 - (bd * dp) / 256;
                  */
                  r = r1 + rd * (rxb * ryb) / 65536;
                  g = g1 + gd * (rxb * ryb) / 65536;
                  b = b1 + bd * (rxb * ryb) / 65536;
               }
               if (r > 255) r = 255;
               if (g > 255) g = 255;
               if (b > 255) b = 255;
               if (r < 0) r = 0;
               if (g < 0) g = 0;
               if (b < 0) b = 0;
               if (bevel & GRADIENTTRANSPARENT)
               {
                  /* merge rgb with background */
                  col = getpixel(bmp, x, y);
                  r += getr(col);
                  g += getg(col);
                  b += getb(col);
                  r /= 2;
                  g /= 2;
                  b /= 2;
               }
               putpixel(bmp, x, y, makecol(r, g, b));
               x++;
            }
         }
         y++;
      }
   }
   /* handle bevelled edges last: */
   do_bevels(bmp, xd, yd, wd, hd, edge, bevel);
   return 0;
}


/* same as above but adds a label with text justification.
   This does not fully support shadowed text. If justify is > 7, a
   substitute "shadow" is drawn, rather than add a new parameter.
   I suppose it is better than nothing...
   Returns the start x coordinate for the text. (helps with editing) */

int draw_colour_gradient_labelled(BITMAP *bmp, int xd, int yd, int wd, int hd,
       int colour1, int colour2, int textcolour, int method,
       FONT *font, char *label, int justify)
{
   int col, x, y, margin;
   int wt, ht, r, g, b;

   draw_colour_gradient(bmp, xd, yd, wd, hd, colour1, colour2, method);

   /* try to find a pleasing margin */
   margin = 10;
   wt = text_length(font, label);
   ht = text_height(font);
   if ((wd - wt) < 20) margin = (wd - wt) / 2;
   if (margin < 0)     margin = 0;

   x = xd + margin;                 /* default: left justify with margin */
   y = yd + (hd - ht) / 2;

   if ((justify & 0x07) == JUSTIFYRIGHT)      x = xd + wd - wt - margin;
   if ((justify & 0x07) == JUSTIFYCENTER)     x = xd + (wd - wt) / 2;
   if (x < 0)  x = 0;

   if (justify & 0x08)  /* support shadows */
   {
      col = makecol(127, 127, 127);
      textout(bmp, font, label, x+1, y+1, col);
   }
   r = (textcolour & 0xFF0000) >> 16;
   g = (textcolour & 0xFF00)   >> 8;
   b = textcolour & 0xFF;
   col = makecol(r, g, b);
   textout(bmp, font, label, x, y, col);
   return x;
}



/* Draw a gradient plus label with support for shadowed text.
   Note: The shadow text is only drawn if the justify bit is set. ( > 7 )
   Returns the start x coordinate for the text. (helps with editing) */

int draw_colour_gradient_label_shadowed(BITMAP *bmp, int xd, int yd, int wd, int hd,
       int colour1, int colour2, int textcolour, int shadcolour, int method,
       FONT *font, char *label, int justify)
{
   int col, x, y, margin, gap = 1;
   int wt, ht, r, g, b;

   draw_colour_gradient(bmp, xd, yd, wd, hd, colour1, colour2, method);

   /* try to find a pleasing margin */
   margin = 10;
   wt = text_length(font, label);
   ht = text_height(font);
   if ((wd - wt) < 20) margin = (wd - wt) / 2;
   if (margin < 0)     margin = 0;

   x = xd + margin;                 /* default: left justify with margin */
   y = yd + (hd - ht) / 2;

   if ((justify & 0x07) == JUSTIFYRIGHT)      x = xd + wd - wt - margin;
   if ((justify & 0x07) == JUSTIFYCENTER)     x = xd + (wd - wt) / 2;
   if (x < 0)  x = 0;

   if (justify & 0x08)  /* support shadows */
   {
      if (justify & 0x10)   gap = 2;           /* experiment... */
      r = (shadcolour & 0xFF0000) >> 16;
      g = (shadcolour & 0xFF00)   >> 8;
      b = shadcolour & 0xFF;
      col = makecol(r, g, b);
      textout(bmp, font, label, x+gap, y+gap, col);
   }
   r = (textcolour & 0xFF0000) >> 16;
   g = (textcolour & 0xFF00)   >> 8;
   b = textcolour & 0xFF;
   col = makecol(r, g, b);
   textout(bmp, font, label, x, y, col);
   return x;
}



/* draw a bilinear gradient filled object on the given bitmap, at xd, yd
   width wd & height hd, colour1, colour2 merging to colour3, colour4.
   using the specified drawing method.
   Imagine the colours in the four corners: 1----2
                                            3----4
   Programmers note: this function does it the hard way, one pixel
   at a time. It should be possible to optimize this using the
   allegro write_line() calls. I have not tried this with
   3.9.32 (the one I use).

   If you use this with large areas (particularly with a 486) it is
   much beter to draw the gradients to a memory bitmap on startup,
   then blit to the screen when required. This will be faster than
   drawing it pixel-by-pixel every time.
*/

int draw_bilinear_gradient(BITMAP *bmp, int xd, int yd, int wd, int hd,
          int colour1, int colour2, int colour3, int colour4, int method)
{
   int cx, cy, x, y, w, h, col;
   int r1, r2, b1, b2, g1, g2, r3, g3, b3, r4, g4, b4;
   int rd12, gd12, bd12, rd13, gd13, bd13, rd14, gd14, bd14; /* diffs */
   int r, g, b, ry, rx, f1, f2, f3;
   int draw, edge, grad, bevel, ilace;

   if (wd < 1 || hd < 1 || bmp == NULL) return -1;

   edge  = method & 3;      /* 0=flat 1=raised 2=sunk 3=lipped */
   draw  = method & 12;     /* 0=nodraw 4=skinned 8=gradient 12=masked */
   grad  = method & 0x0F0;  /* 0=solid 16=horiz 32=vert 48=diag 64=polar.. */
   bevel = method & 0xFF00; /* 0=std 256=bevel2 512=interlace 1024=rivets
                               2048=transparent... */

   /* we assume that method is set for bilinear gradient */
   /* if not, call the other routine */
   if (grad < GRADIENTBILINEAR)
   {
      draw_colour_gradient(bmp, xd, yd, wd, hd, colour1, colour2, method);
      return 0;
   }
   if (draw == 0) return -1;     /* oops... nothing to do! */

   x = xd;
   y = yd;
   w = wd;
   h = hd;
   r1 = (colour1 & 0xFF0000) >> 16;
   g1 = (colour1 & 0xFF00)   >> 8;
   b1 = colour1 & 0xFF;
   r2 = (colour2 & 0xFF0000) >> 16;
   g2 = (colour2 & 0xFF00)   >> 8;
   b2 = colour2 & 0xFF;
   r3 = (colour3 & 0xFF0000) >> 16;
   g3 = (colour3 & 0xFF00)   >> 8;
   b3 = colour3 & 0xFF;
   r4 = (colour4 & 0xFF0000) >> 16;
   g4 = (colour4 & 0xFF00)   >> 8;
   b4 = colour4 & 0xFF;

   rd12 = r2 - r1;
   gd12 = g2 - g1;
   bd12 = b2 - b1;
   rd13 = r3 - r1;
   gd13 = g3 - g1;
   bd13 = b3 - b1;
   rd14 = r4 - r1;
   gd14 = g4 - g1;
   bd14 = b4 - b1;

   /* configure interlace function */
   ilace = 2000;                   /* bugfix, was 0, and caused problems */
   if (bevel & GRADIENTINTERLACE)
   {
      ilace = 2;
      if (bevel & GRADIENTRIVETED) ilace = 3;
   }
   else    if (bevel & GRADIENTRIVETED) ilace = 4;

   if(draw == 8)
   {
      y = yd;
      for (cy = 0; cy < hd; cy++)
      {
         x = xd;
         if (hd < 2)             /* avoid divide by zero errors */
         {
            ry = 128;
         }
         else             /* vertical ratio as part of 256 */
         {
            ry = (256 * cy) / (hd - 1);             /* linear */
         }
         /* check if this is an interlace/Rivets line */
         if (ilace < 5 && (cy % ilace == 0) && cy > 0)
         {
            for (cx = 0; cx < wd; cx++)    /* yes it is */
            {
               r = r1;
               g = g1;
               b = b1;
               /* find r g b at each pixel */
               if (wd < 2) rx = 128;
               else        rx = (256 * cx)/ (wd-1);
               if (grad == GRADIENTBILINEAR)
               {
                  f1 = (rx * (256 - ry)) / 256;
                  f2 = (ry * (256 - rx)) / 256;
                  f3 = (rx * ry) / 256;
                  r = r1 + (rd12 * f1) / 256 + (rd13 * f2) / 256
                     + (rd14 * f3) / 256;
                  g = g1 + (gd12 * f1) / 256 + (gd13 * f2) / 256
                     + (gd14 * f3) / 256;
                  b = b1 + (bd12 * f1) / 256 + (bd13 * f2) / 256
                     + (bd14 * f3) / 256;
               }
               else    /* impossible? use vertical if we botch it */
               {
                  r = r1 + rd12 * ry / 256;
                  g = g1 + gd12 * ry / 256;
                  b = b1 + bd12 * ry / 256;
               }
               if (r > 255) r = 255;
               if (g > 255) g = 255;
               if (b > 255) b = 255;
               if (r < 0) r = 0;
               if (g < 0) g = 0;
               if (b < 0) b = 0;
               /* handle interlace/Rivets: adjust rgb by 20% up or down */
               if (bevel & GRADIENTINTERLACE)
               {
                  r = (r * 200) / 256;
                  g = (g * 200) / 256;
                  b = (b * 200) / 256;
               }
               if (ilace == 4)   /* a rivetting experience? */
               {
                  if (cx > 1)
                  {
                     if (cx % ilace == 0)     /* brighten this pixel */
                     {
                        r = (r * 300) / 256;
                        if (r > 255) r = 255;
                        g = (g * 300) / 256;
                        if (g > 255) g = 255;
                        b = (b * 300) / 256;
                        if (b > 255) b = 255;
                     }
                     if (cx % ilace == 1)     /* darken this pixel */
                     {
                        r = (r * 200) / 256;
                        g = (g * 200) / 256;
                        b = (b * 200) / 256;
                     }
                  }
               }
               if (bevel & GRADIENTTRANSPARENT)
               {
                  /* merge rgb with background */
                  col = getpixel(bmp, x, y);
                  r += getr(col);
                  g += getg(col);
                  b += getb(col);
                  r /= 2;
                  g /= 2;
                  b /= 2;
               }
               putpixel(bmp, x, y, makecol(r, g, b));
               x++;
            }
         }
         else         /* non-interlaced lines */
         {
            for (cx = 0; cx < wd; cx++)
            {
               if (wd < 2) rx = 128;
               else        rx = (256 * cx)/ (wd-1);
               /* find r g b at each pixel */
               if (grad == GRADIENTBILINEAR)
               {
                  f1 = (rx * (256 - ry)) / 256;
                  f2 = (ry * (256 - rx)) / 256;
                  f3 = (rx * ry) / 256;
                  r = r1 + (rd12 * f1) / 256 + (rd13 * f2) / 256
                     + (rd14 * f3) / 256;
                  g = g1 + (gd12 * f1) / 256 + (gd13 * f2) / 256
                     + (gd14 * f3) / 256;
                  b = b1 + (bd12 * f1) / 256 + (bd13 * f2) / 256
                     + (bd14 * f3) / 256;
               }
               else    /* impossible? use vertical if we botch it */
               {
                  r = r1 + rd12 * ry / 256;
                  g = g1 + gd12 * ry / 256;
                  b = b1 + bd12 * ry / 256;
               }
               if (r > 255) r = 255;
               if (g > 255) g = 255;
               if (b > 255) b = 255;
               if (r < 0) r = 0;
               if (g < 0) g = 0;
               if (b < 0) b = 0;
               if (bevel & GRADIENTTRANSPARENT)
               {
                  /* merge rgb with background */
                  col = getpixel(bmp, x, y);
                  r += getr(col);
                  g += getg(col);
                  b += getb(col);
                  r /= 2;
                  g /= 2;
                  b /= 2;
               }
               putpixel(bmp, x, y, makecol(r, g, b));
               x++;
            }
         }
         y++;
      }
   }
   /* handle bevelled edges last:
      We do this a pixel at a time, in case it's a graduated line...
      if double bevelled, we do this twice... what a pain! */

   if (edge & GRADIENTRAISED)
   {
      if (bevel & GRADIENTBEVEL2)
      {
         draw_gradient_bevel(bmp, xd, yd, wd, hd, RAISEDDRAWTWO);
         xd++;
         yd++;
         wd -= 2;
         hd -= 2;
      }
      draw_gradient_bevel(bmp, xd, yd, wd, hd, RAISEDDRAW);
      xd++;
      yd++;
      wd -= 2;
      hd -= 2;
   }
   if (edge & GRADIENTSUNK)
   {
      if (bevel & GRADIENTBEVEL2)
      {
         draw_gradient_bevel(bmp, xd, yd, wd, hd, SUNKENDRAWTWO);
         xd++;
         yd++;
         wd -= 2;
         hd -= 2;
      }
      draw_gradient_bevel(bmp, xd, yd, wd, hd, SUNKENDRAW);
   }
   return 0;
}



/* same as above but adds a label with text justification.
   This does not fully support shadowed text. If justify is > 7, a
   substitute "shadow" is drawn, rather than add a new parameter.
   I suppose it is better than nothing...
   Returns the start x coordinate for the text. (helps with editing) */

int draw_bilinear_gradient_labelled(BITMAP *bmp, int xd, int yd, int wd,
       int hd, int colour1, int colour2, int colour3, int colour4,
       int textcolour, int method, FONT *font, char *label, int justify)
{
   int col, x, y, margin;
   int wt, ht, r, g, b;

   draw_bilinear_gradient(bmp, xd, yd, wd, hd, colour1, colour2,
      colour3, colour4, method);

   /* try to find a pleasing margin */
   margin = 10;
   wt = text_length(font, label);
   ht = text_height(font);
   if ((wd - wt) < 20) margin = (wd - wt) / 2;
   if (margin < 0)     margin = 0;

   x = xd + margin;                 /* default: left justify with margin */
   y = yd + (hd - ht) / 2;

   if ((justify & 0x07) == JUSTIFYRIGHT)      x = xd + wd - wt - margin;
   if ((justify & 0x07) == JUSTIFYCENTER)     x = xd + (wd - wt) / 2;
   if (x < 0)  x = 0;

   if (justify & 0x08)  /* support shadows */
   {
      col = makecol(127, 127, 127);
      textout(bmp, font, label, x+1, y+1, col);
   }
   r = (textcolour & 0xFF0000) >> 16;
   g = (textcolour & 0xFF00)   >> 8;
   b = textcolour & 0xFF;
   col = makecol(r, g, b);
   textout(bmp, font, label, x, y, col);
   return x;
}



/* Draw a gradient plus label with support for shadowed text.
   Note: The shadow text is only drawn if the justify bit is set. ( > 7 )
   Returns the start x coordinate for the text. (helps with editing) */

int draw_bilinear_gradient_label_shadowed(BITMAP *bmp, int xd, int yd,
       int wd, int hd, int colour1, int colour2, int colour3, int colour4,
       int textcolour, int shadcolour, int method,
       FONT *font, char *label, int justify)
{
   int col, x, y, margin, gap = 1;
   int wt, ht, r, g, b;

   draw_bilinear_gradient(bmp, xd, yd, wd, hd, colour1, colour2,
      colour3, colour4, method);

   /* try to find a pleasing margin */
   margin = 10;
   wt = text_length(font, label);
   ht = text_height(font);
   if ((wd - wt) < 20) margin = (wd - wt) / 2;
   if (margin < 0)     margin = 0;

   x = xd + margin;                 /* default: left justify with margin */
   y = yd + (hd - ht) / 2;

   if ((justify & 0x07) == JUSTIFYRIGHT)      x = xd + wd - wt - margin;
   if ((justify & 0x07) == JUSTIFYCENTER)     x = xd + (wd - wt) / 2;
   if (x < 0)  x = 0;

   if (justify & 0x08)  /* support shadows */
   {
      if (justify & 0x10)   gap = 2;           /* experiment... */
      r = (shadcolour & 0xFF0000) >> 16;
      g = (shadcolour & 0xFF00)   >> 8;
      b = shadcolour & 0xFF;
      col = makecol(r, g, b);
      textout(bmp, font, label, x+gap, y+gap, col);
   }
   r = (textcolour & 0xFF0000) >> 16;
   g = (textcolour & 0xFF00)   >> 8;
   b = textcolour & 0xFF;
   col = makecol(r, g, b);
   textout(bmp, font, label, x, y, col);
   return x;
}



/* do a label only - the text colour is already in current bitdepth.
   all this does is figure out a margin from the justify parameter.
   This does not fully support shadowed text. If justify > 7 a
   substitute "shadow" is drawn, rather than add a new parameter.
   I suppose it is better than nothing...
   Returns the start x coordinate for the text. (helps with editing) */

int draw_gradient_label_only(BITMAP *bmp, int xd, int yd, int wd, int hd,
       int col, FONT *font, char *label, int justify)
{
   int x, y, margin;
   int wt, ht;

   /* try to find a pleasing margin */
   margin = 10;
   wt = text_length(font, label);
   ht = text_height(font);
   if ((wd - wt) < 20) margin = (wd - wt) / 2;
   if (margin < 0)     margin = 0;

   x = xd + margin;                 /* default: left justify with margin */
   y = yd + (hd - ht) / 2;

   if ((justify & 0x07) == JUSTIFYRIGHT)      x = xd + wd - wt - margin;
   if ((justify & 0x07) == JUSTIFYCENTER)     x = xd + (wd - wt) / 2;
   if (x < 0)  x = 0;

   if (justify & 0x08)  /* support shadows */
      textout(bmp, font, label, x+1, y+1, makecol(127, 127, 127));

   textout(bmp, font, label, x, y, col);
   return x;
}



/* draw an item from the default gradient list   gradient[index]
   Returns the text colour in the current bit depth. */

int draw_gradient(BITMAP *bmp, int xd, int yd, int wd, int hd,
     int index, int focus)
{
   int col, col2, col3, col4, tc, method;

   if (index < 0) index = 0;
   if (index >= MAXGRADIENTS) index = MAXGRADIENTS;

   col  = gradient[index].colour;
   col2 = gradient[index].colourto;
   tc   = gradient[index].txtcolour;
   col3 = gradient[index].colourbi;
   col4 = gradient[index].colourbi2;
   method = gradient[index].method;
   if (focus == UNFOCUSSED)
   {
      col    = gradient[index].ucolour;
      col2   = gradient[index].ucolourto;
      tc     = gradient[index].utxtcolour;
      col3   = gradient[index].ucolourbi;
      col4   = gradient[index].ucolourbi2;
      method = gradient[index].umethod;
   }
   if (focus == FOCUSPRESSED)
   {
      col    = gradient[index].pcolour;
      col2   = gradient[index].pcolourto;
      tc     = gradient[index].ptxtcolour;
      col3   = gradient[index].pcolourbi;
      col4   = gradient[index].pcolourbi2;
      method = gradient[index].pmethod;
   }
   if (focus == FOCUSDISABLED)
   {
      col    = gradient[index].dcolour;
      col2   = gradient[index].dcolourto;
      tc     = gradient[index].dtxtcolour;
      col3   = gradient[index].dcolourbi;
      col4   = gradient[index].dcolourbi2;
      method = gradient[index].dmethod;
   }
   if (method & GRADIENTBILINEAR)
      draw_bilinear_gradient(bmp, xd, yd, wd, hd,
         col, col2, col3, col4, method);
   else
      draw_colour_gradient(bmp, xd, yd, wd, hd, col, col2, method);

   return makecol((tc & 0xFF0000) >> 16, (tc & 0xFF00) >> 8, tc & 0xFF);
}



/* draw a default gradient ( gradient[] ) with a text label.
   This supports shadowed text if justify has the shadow bit is set. 0x08
   Returns the start x coordinate for the text. (helps with editing) */

int draw_gradient_labelled(BITMAP *bmp, int xd, int yd, int wd, int hd,
     int index, int focus, FONT *font, char *label)
{
   int col, shad, justify, x, y, margin;
   int wt, ht, r, g, b, jstshadow;

   draw_gradient(bmp, xd, yd, wd, hd, index, focus);

   col     = gradient[index].txtcolour;
   shad    = gradient[index].txtshadow;
   justify = gradient[index].justify & 0x07;
   jstshadow = gradient[index].justify & JUSTIFYSHADOWMASK;
   if (focus == UNFOCUSSED)
   {
      col     = gradient[index].utxtcolour;
      shad    = gradient[index].utxtshadow;
   }
   if (focus == FOCUSPRESSED)
   {
      col     = gradient[index].ptxtcolour;
      shad    = gradient[index].ptxtshadow;
   }
   if (focus == FOCUSDISABLED)
   {
      col     = gradient[index].dtxtcolour;
      shad    = gradient[index].dtxtshadow;
   }
   /* try to find a pleasing margin */
   margin = 10;
   wt = text_length(font, label);
   ht = text_height(font);
   if ((wd - wt) < 20) margin = (wd - wt) / 2;
   if (margin < 0)     margin = 0;

   x = xd + margin;                 /* default: left justify with margin */
   y = yd + (hd - ht) / 2;

   if (justify == JUSTIFYRIGHT)      x = xd + wd - wt - margin;
   if (justify == JUSTIFYCENTER)     x = xd + (wd - wt) / 2;
   if (x < 0)  x = 0;
   if (focus == FOCUSPRESSED)
   {
      x++;
      y++;
   }
   if (jstshadow)
   {
      r = (shad & 0xFF0000) >> 16;
      g = (shad & 0xFF00)   >> 8;
      b = shad & 0xFF;
      textout(bmp, font, label, x+1, y+1, makecol(r,g,b));
   }
   r = (col & 0xFF0000) >> 16;
   g = (col & 0xFF00)   >> 8;
   b = col & 0xFF;
   textout(bmp, font, label, x, y, makecol(r,g,b));
   return x;
}




/* draw an indexed item from a specified gradient[]
   Returns the text colour in the current bit depth. */

int draw_gradient_index(BITMAP *bmp, int xd, int yd, int wd, int hd,
     int index, int focus, GRADIENT *grad)
{
   int col, col2, col3, col4, tc, method;

   col    = grad[index].colour;
   col2   = grad[index].colourto;
   tc     = grad[index].txtcolour;
   col3 = gradient[index].colourbi;
   col4 = gradient[index].colourbi2;
   method = grad[index].method;
   if (focus == UNFOCUSSED)
   {
      col    = grad[index].ucolour;
      col2   = grad[index].ucolourto;
      tc     = grad[index].utxtcolour;
      col3   = gradient[index].ucolourbi;
      col4   = gradient[index].ucolourbi2;
      method = grad[index].umethod;
   }
   if (focus == FOCUSPRESSED)
   {
      col    = grad[index].pcolour;
      col2   = grad[index].pcolourto;
      tc     = grad[index].ptxtcolour;
      col3   = gradient[index].pcolourbi;
      col4   = gradient[index].pcolourbi2;
      method = grad[index].pmethod;
   }
   if (focus == FOCUSDISABLED)
   {
      col    = grad[index].dcolour;
      col2   = grad[index].dcolourto;
      tc     = grad[index].dtxtcolour;
      col3   = gradient[index].dcolourbi;
      col4   = gradient[index].dcolourbi2;
      method = grad[index].dmethod;
   }
   if (method & GRADIENTBILINEAR)
      draw_bilinear_gradient(bmp, xd, yd, wd, hd,
         col, col2, col3, col4, method);
   else
      draw_colour_gradient(bmp, xd, yd, wd, hd, col, col2, method);

   return makecol((tc & 0xFF0000) >> 16, (tc & 0xFF00) >> 8, tc & 0xFF);
}


/* draw a labelled gradient with the specified gradient[]
   Returns the start x coordinate for the text. (helps with editing) */

int draw_gradient_index_labelled(BITMAP *bmp, int xd, int yd, int wd, int hd,
     int index, int focus, FONT *font, char *label, GRADIENT *grad)
{
   int col, shad, justify, jstshadow, x, y, margin;
   int wt, ht, r, g, b;

   draw_gradient_index(bmp, xd, yd, wd, hd, index, focus, grad);

   col     = grad[index].txtcolour;
   shad    = grad[index].txtshadow;
   justify = grad[index].justify & 0x07;
   jstshadow = grad[index].justify & JUSTIFYSHADOWMASK;
   if (focus == UNFOCUSSED)
   {
      col     = grad[index].utxtcolour;
      shad    = grad[index].utxtshadow;
   }
   if (focus == FOCUSPRESSED)
   {
      col     = grad[index].ptxtcolour;
      shad    = grad[index].ptxtshadow;
   }
   if (focus == FOCUSDISABLED)
   {
      col     = grad[index].dtxtcolour;
      shad    = grad[index].dtxtshadow;
   }
   /* try to find a pleasing margin */
   margin = 10;
   wt = text_length(font, label);
   ht = text_height(font);
   if ((wd - wt) < 20) margin = (wd - wt) / 2;
   if (margin < 0)     margin = 0;

   x = xd + margin;                 /* default: left justify with margin */
   y = yd + (hd - ht) / 2;

   if (justify == JUSTIFYRIGHT)      x = xd + wd - wt - margin;
   if (justify == JUSTIFYCENTER)     x = xd + (wd - wt) / 2;
   if (x < 0)  x = 0;
   if (focus == FOCUSPRESSED)
   {
      x++;
      y++;
   }
   if (jstshadow)
   {
      r = (shad & 0xFF0000) >> 16;
      g = (shad & 0xFF00)   >> 8;
      b = shad & 0xFF;
      textout(bmp, font, label, x+1, y+1, makecol(r,g,b));
   }
   r = (col & 0xFF0000) >> 16;
   g = (col & 0xFF00)   >> 8;
   b = col & 0xFF;
   textout(bmp, font, label, x, y, makecol(r,g,b));
   return x;
}

/* *********** MASKED and MAPPED custom gradients *********** */

/* Load a file for a texture map and convert to geyscale.

Note: You MUST call load_gradient_texture_map() BEFORE using the
      texture mapped/masked routines.
*/


BITMAP *load_gradient_texture_map(char *fname)
{
   int cx, cy, col, x;
   int w, h;
   BITMAP *temp = NULL;
   BITMAP *greybmp = NULL;
   PALETTE mypal;

   temp = load_bitmap(fname, mypal);
   if (temp == NULL)  return NULL;
   if (bitmap_color_depth(temp) == 8)  select_palette(mypal);
   w = temp->w;
   h = temp->h;
   if (w < 1 || h < 1)
   {
      destroy_bitmap(temp);
      if (bitmap_color_depth(temp) == 8)  unselect_palette();
      return NULL;
   }
   greybmp = create_bitmap_ex(8, w, h);
   if (greybmp == NULL)
   {
      destroy_bitmap(temp);
      if (bitmap_color_depth(temp) == 8)  unselect_palette();
      return NULL;
   }
   for (cy = 0; cy < h; cy++)
   {
      for (cx = 0; cx < w; cx++)
      {
         col = getpixel(temp, cx, cy);
         x = (getr(col) + getg(col) + getb(col)) / 3;
         greybmp->line[cy][cx] = (unsigned char)x;
      }
   }
   if (bitmap_color_depth(temp) == 8)  unselect_palette();
   return greybmp;
}

int draw_colour_gradient_mapped(BITMAP *bmp, int xd, int yd, int wd, int hd,
    int colour1, int colour2, int method, BITMAP *greybmp)
{
   int cx, cy, r, g, b, k, edge, bevel;
   int x, y, w, h, r1, g1, b1, col;
   int r2, g2, b2, rd, gd, bd;

   if (w == 0 || h == 0) return 0;
   if (greybmp == NULL)
      return draw_colour_gradient(bmp,xd,yd,wd,hd,colour1,colour2,method);

   edge  = method & 3;      /* 0=flat 1=raised 2=sunk 3=lipped */
   bevel = method & 0xFF00; /* 0=std 256=bevel2 (interlace, rivets not used)
                               2048=transparent... */
   x = 0;
   y = 0;
   w = greybmp->w;
   h = greybmp->h;
   r1 = (colour1 & 0xFF0000) >> 16;
   g1 = (colour1 & 0xFF00)   >> 8;
   b1 = colour1 & 0xFF;
   r2 = (colour2 & 0xFF0000) >> 16;
   g2 = (colour2 & 0xFF00)   >> 8;
   b2 = colour2 & 0xFF;

   rd = r2 - r1;
   gd = g2 - g1;
   bd = b2 - b1;

   if (bevel & GRADIENTTRANSPARENT)   /* add in parent pixels */
   {
      for (cy = 0; cy < hd; cy++)
      {
         y = cy % h;
         for (cx = 0; cx < wd; cx++)
         {
            x = cx % w;
            k = (unsigned)greybmp->line[y][x];
            r = r1 + (rd * k) / 256;
            g = g1 + (gd * k) / 256;
            b = b1 + (bd * k) / 256;
            /* merge rgb with background */
            col = getpixel(bmp, xd+cx, yd+cy);
            r += getr(col);
            g += getg(col);
            b += getb(col);
            r /= 2;
            g /= 2;
            b /= 2;
            putpixel(bmp, xd+cx, yd+cy, makecol(r, g, b));
         }
      }
   }
   else
   {
      for (cy = 0; cy < hd; cy++)
      {
         y = cy % h;
         for (cx = 0; cx < wd; cx++)
         {
            x = cx % w;
            k = (unsigned)greybmp->line[y][x];
            r = r1 + (rd * k) / 256;
            g = g1 + (gd * k) / 256;
            b = b1 + (bd * k) / 256;
            putpixel(bmp, xd+cx, yd+cy, makecol(r, g, b));
         }
      }
   }
   do_bevels(bmp, xd, yd, wd, hd, edge, bevel);
   return 0;
}



int draw_colour_gradient_masked(BITMAP *bmp, int xd, int yd, int wd, int hd,
    int colour1, int colour2, int method, BITMAP *greybmp)
{
   int cx = 0, cy = 0, r = 0, g = 0, b = 0, k = 0, edge = 0, bevel = 0;
   int x = 0, y = 0, w = 0, h = 0, r1 = 0, g1 = 0, b1 = 0;
   int r2 = 0, g2 = 0, b2 = 0, col = 0, rd = 0, gd = 0, bd = 0;

   if (w == 0 || h == 0) return 0;
   if (greybmp == NULL)
      return draw_colour_gradient(bmp,xd,yd,wd,hd,colour1,colour2,method);


   edge  = method & 3;      /* 0=flat 1=raised 2=sunk 3=lipped */
   bevel = method & 0xFF00; /* 0=std 256=bevel2 (interlace, rivets not used)
                               2048=transparent... */
   x = 0;
   y = 0;
   w = greybmp->w;
   h = greybmp->h;
   r1 = (colour1 & 0xFF0000) >> 16;
   g1 = (colour1 & 0xFF00)   >> 8;
   b1 = colour1 & 0xFF;
   r2 = (colour2 & 0xFF0000) >> 16;
   g2 = (colour2 & 0xFF00)   >> 8;
   b2 = colour2 & 0xFF;

   rd = r2 - r1;
   gd = g2 - g1;
   bd = b2 - b1;

   if (bevel & GRADIENTTRANSPARENT)   /* add in parent pixels */
   {
      for (cy = 0; cy < hd; cy++)
      {
         y = cy % h;
         for (cx = 0; cx < wd; cx++)
         {
            x = cx % w;
            k = (unsigned)greybmp->line[y][x];
            if (k)
            {
               r = r1 + (rd * k) / 256;
               g = g1 + (gd * k) / 256;
               b = b1 + (bd * k) / 256;
               /* merge rgb with background */
               col = getpixel(bmp, xd+cx, yd+cy);
               r += getr(col);
               g += getg(col);
               b += getb(col);
               r /= 2;
               g /= 2;
               b /= 2;
               putpixel(bmp, xd+cx, yd+cy, makecol(r, g, b));
            }
         }
      }
   }
   else
   {
      for (cy = 0; cy < hd; cy++)
      {
         y = cy % h;
         for (cx = 0; cx < wd; cx++)
         {
            x = cx % w;
            k = (unsigned)greybmp->line[y][x];
            if (k)
            {
               r = r1 + (rd * k) / 256;
               g = g1 + (gd * k) / 256;
               b = b1 + (bd * k) / 256;
               putpixel(bmp, xd+cx, yd+cy, makecol(r, g, b));
            }
         }
      }
   }
   /* bevels only make sense if edges are NOT masked! Oh well... */
   do_bevels(bmp, xd, yd, wd, hd, edge, bevel);
   return 0;
}

/* Draw an enhanced gradient using a texture map to define high and low
   pixels. The texture map has the following characteristics:

   Pixel values of 0 (zero) indicate skipped pixels.
   Pixel value 128 is considered the base colour.
   Pixel values 1-127 will decrease the colour, 1 being darkest.
   Pixel values 229-255 will increase the colour, 255 being brightest.
*/

int draw_colour_gradient_enhanced(BITMAP *bmp, int xd, int yd, int wd, int hd,
    int colour1, int colour2, int method, BITMAP *greybmp)
{
   int cx, cy, r, g, b, k, edge, bevel;
   int x, y, w, h, r1, g1, b1;
   int r2, g2, b2, col, rd, gd, bd;

   if (w == 0 || h == 0) return 0;
   if (greybmp == NULL)
      return draw_colour_gradient(bmp,xd,yd,wd,hd,colour1,colour2,method);

   edge  = method & 3;      /* 0=flat 1=raised 2=sunk 3=lipped */
   bevel = method & 0xFF00; /* 0=std 256=bevel2 (interlace, rivets not used)
                               2048=transparent... */
   x = 0;
   y = 0;
   w = greybmp->w;
   h = greybmp->h;
   r1 = (colour1 & 0xFF0000) >> 16;
   g1 = (colour1 & 0xFF00)   >> 8;
   b1 = colour1 & 0xFF;
   r2 = (colour2 & 0xFF0000) >> 16;
   g2 = (colour2 & 0xFF00)   >> 8;
   b2 = colour2 & 0xFF;

   rd = r2 - r1;
   gd = g2 - g1;
   bd = b2 - b1;

   if (bevel & GRADIENTTRANSPARENT)   /* add in parent pixels */
   {
      for (cy = 0; cy < hd; cy++)
      {
         y = cy % h;
         for (cx = 0; cx < wd; cx++)
         {
            x = cx % w;
            k = (unsigned)greybmp->line[y][x];
            if (k)
            {
               k -= 128;
               r = r1 + (rd * k) / 128;
               g = g1 + (gd * k) / 128;
               b = b1 + (bd * k) / 128;
               if (r < 0) r = 0;
               if (g < 0) g = 0;
               if (b < 0) b = 0;
               if (r > 255) r = 255;
               if (g > 255) g = 255;
               if (b > 255) b = 255;
               /* merge rgb with background */
               col = getpixel(bmp, xd+cx, yd+cy);
               r += getr(col);
               g += getg(col);
               b += getb(col);
               r /= 2;
               g /= 2;
               b /= 2;
               putpixel(bmp, xd+cx, yd+cy, makecol(r, g, b));
            }
         }
      }
   }
   else
   {
      for (cy = 0; cy < hd; cy++)
      {
         y = cy % h;
         for (cx = 0; cx < wd; cx++)
         {
            x = cx % w;
            k = (unsigned)greybmp->line[y][x];
            if (k)
            {
               k -= 128;
               r = r1 + (rd * k) / 128;
               g = g1 + (gd * k) / 128;
               b = b1 + (bd * k) / 128;
               if (r < 0) r = 0;
               if (g < 0) g = 0;
               if (b < 0) b = 0;
               if (r > 255) r = 255;
               if (g > 255) g = 255;
               if (b > 255) b = 255;
               putpixel(bmp, xd+cx, yd+cy, makecol(r, g, b));
            }
         }
      }
   }
   /* bevels only make sense if edges are NOT masked! Oh well... */
   do_bevels(bmp, xd, yd, wd, hd, edge, bevel);
   return 0;
}



/* draw a mapped item from the default gradient list gradient[index]
   Returns the text colour in the current bit depth. */

int draw_mapped_gradient(BITMAP *bmp, int xd, int yd, int wd, int hd,
     int index, int focus, BITMAP *greybmp)
{
   int col, col2, tc, method;

   if (index < 0) index = 0;
   if (index >= MAXGRADIENTS) index = MAXGRADIENTS;

   col  = gradient[index].colour;
   col2 = gradient[index].colourto;
   tc   = gradient[index].txtcolour;
   method = gradient[index].method;
   if (focus == UNFOCUSSED)
   {
      col    = gradient[index].ucolour;
      col2   = gradient[index].ucolourto;
      tc     = gradient[index].utxtcolour;
      method = gradient[index].umethod;
   }
   if (focus == FOCUSPRESSED)
   {
      col    = gradient[index].pcolour;
      col2   = gradient[index].pcolourto;
      tc     = gradient[index].ptxtcolour;
      method = gradient[index].pmethod;
   }
   if (focus == FOCUSDISABLED)
   {
      col    = gradient[index].dcolour;
      col2   = gradient[index].dcolourto;
      tc     = gradient[index].dtxtcolour;
      method = gradient[index].dmethod;
   }
   draw_colour_gradient_mapped(bmp, xd, yd, wd, hd,
      col, col2, method, greybmp);

   return makecol((tc & 0xFF0000) >> 16, (tc & 0xFF00) >> 8, tc & 0xFF);
}

/* draw a masked/mapped item from the default gradient list gradient[index]
   Returns the text colour in the current bit depth. */

int draw_masked_gradient(BITMAP *bmp, int xd, int yd, int wd, int hd,
     int index, int focus, BITMAP *greybmp)
{
   int col, col2, tc, method;

   if (index < 0) index = 0;
   if (index >= MAXGRADIENTS) index = MAXGRADIENTS;

   col  = gradient[index].colour;
   col2 = gradient[index].colourto;
   tc   = gradient[index].txtcolour;
   method = gradient[index].method;
   if (focus == UNFOCUSSED)
   {
      col    = gradient[index].ucolour;
      col2   = gradient[index].ucolourto;
      tc     = gradient[index].utxtcolour;
      method = gradient[index].umethod;
   }
   if (focus == FOCUSPRESSED)
   {
      col    = gradient[index].pcolour;
      col2   = gradient[index].pcolourto;
      tc     = gradient[index].ptxtcolour;
      method = gradient[index].pmethod;
   }
   if (focus == FOCUSDISABLED)
   {
      col    = gradient[index].dcolour;
      col2   = gradient[index].dcolourto;
      tc     = gradient[index].dtxtcolour;
      method = gradient[index].dmethod;
   }
   draw_colour_gradient_masked(bmp, xd, yd, wd, hd,
      col, col2, method, greybmp);

   return makecol((tc & 0xFF0000) >> 16, (tc & 0xFF00) >> 8, tc & 0xFF);
}


/* ***********  WINDOW / DIALOG BOX ROUTINES ************ */

/* This routine sets the sizes of the window components in the default
   GRADIENTWINDOW gw, prior to a call to draw_default_window() or
   get_default_window_size().

   titlebar    the height of the title bar at the top        (0 = none)
   menubar     the height of the menu below the title bar    (0 = none)
   iconbar     the height of the iconbar below it            (0 = none)
   addrbar     the height of an address bar if any           (0 = none)
   statbar     a status bar (for help lines etc) below the users frame
   grips       the height of lower edge grips and handle, if any
   titlebuttons   specifies the titlebar buttons and clock. (0=none)
                  leftbuttons*256 + clockblocks*16 + rightbuttons.
                  (one clockblock is 16 pixels)
   icon        specifies the width of an icon placed left of the title label
                 so that it can be drawn in later.
   wallpaper   specifies whether a wallpaper should show in the user frame
                 1=yes. If zero, the user frame is filled with the gradient
                 theme.
   barhandles  specifies the size of a minimize handle on the left of the
                 menubar, iconbar and addressbar in order to provide a
                 minimize toolbar function. 0=none 1++=pixel width.
                 (future enhancement).
   */

void set_window_style(int titlebar, int menubar, int iconbar,
          int addrbar, int statbar, int grips, int titlebuttons,
          int icon, int wallpaper, int barhandles)
{
   gw.title = titlebar;        /* store these things for drawtime */
   gw.menubar = menubar;
   gw.iconbar = iconbar;
   gw.addrbar = addrbar;
   gw.statbar = statbar;
   gw.handle = grips;
   gw.button = titlebuttons;
   gw.icon = icon;
   gw.mb = 0;
   gw.ib = 0;
   gw.ab = 0;
   gw.sb = 0;
   gw.wpaper = wallpaper;
   gw.barhandles = barhandles;
}


/* This routine sets the sizes of the window components in the users
   GRADIENTWINDOW gwin, prior to a call to draw_gradient_window() or
   get_gradient_window_size().

   titlebar    the height of the title bar at the top        (0 = none)
   menubar     the height of the menu below the title bar    (0 = none)
   iconbar     the height of the iconbar below it            (0 = none)
   addrbar     the height of an address bar if any           (0 = none)
   statbar     a status bar (for help lines etc) below the users frame
   grips       the height of lower edge grips and handle, if any
   titlebuttons   specifies the titlebar buttons and clock. (0=none)
                  leftbuttons*256 + clockblocks*16 + rightbuttons.
                  (one clockblock is 16 pixels)
   icon        specifies the width of an icon placed left of the title label
                 so that it can be drawn in later.
   wallpaper   specifies whether a wallpaper should show in the user frame
                 1=yes. If zero, the user frame is filled with the gradient
                 theme. If non-zero, the user frame is not filled.
   barhandles  specifies the size of a minimize handle on the left of the
                 menubar, iconbar and addressbar in order to provide a
                 minimize toolbar function. 0=none 1++=pixel width.
                 (future enhancement).
   */

void set_gradient_window_style(int titlebar, int menubar, int iconbar,
           int addrbar, int statbar, int grips, int titlebuttons,
           int icon, int wallpaper, int barhandles, GRADIENTWINDOW *gwin)
{
   gwin->title = titlebar;        /* store these things for drawtime */
   gwin->menubar = menubar;
   gwin->iconbar = iconbar;
   gwin->addrbar = addrbar;
   gwin->statbar = statbar;
   gwin->handle = grips;
   gwin->button = titlebuttons;
   gwin->icon = gwin->icon;
   gwin->mb = 0;
   gwin->ib = 0;
   gwin->ab = 0;
   gwin->sb = 0;
   gwin->wpaper = wallpaper;
   gwin->barhandles = barhandles;
}


/* Get the size and location of the users window frame.
   Uses the default GRADIENTWINDOW gw
   Returns the window text colour.
   Call this AFTER DRAWING THE WINDOW. */

int get_window_frame(int *x, int *y, int *w, int *h)
{
   *x = gw.ux;
   *y = gw.uy;
   *w = gw.uw;
   *h = gw.uh;
   return gw.textcol;
}


/* Get the size and location of the users window frame.
   Uses the specified GRADIENTWINDOW gwin
   Returns the window text colour.
   Call this AFTER DRAWING THE WINDOW. */


int get_gradient_window_frame(int *x, int *y, int *w, int *h,
                              GRADIENTWINDOW *gwin)
{
   *x = gwin->ux;
   *y = gwin->uy;
   *w = gwin->uw;
   *h = gwin->uh;
   return gwin->textcol;
}


/*
   Suppose you want to find out how big to make the window so that
   it will contain the bits you want and still have a certain size
   area to draw on, inside the window frame.

   Given w & h, the width and height of the desired usable frame,
   find the overall width & height of the window (winw, winh) to allow for:
   title         the height of the title bar at the top        (0 = none)
   menubar       the height of the menu bar below the title    (0 = none)
   iconbar       the height of the iconbar below it (0 = none)
   addbar        the height of an address bar if any
   statbar       a status bar (for help lines etc) below the users frame
   handle        the height of lower handle and edge grips if any

   Note 1: Two variables are set up when a gradient theme is loaded
            or when set_border_style(...); is called...

   borderwidth   number of pixels that outline the window
   bevelwidth    number of pixels that outline the users frame

   This means that the size of the window, necessary to have w*h area
   available to the user, may change slightly when the theme is changed.

   Note 2: if borderwidth is not zero, there will be an additional border
   drawn below the title bar, and above the handle/grips. A border is
   NOT drawn around the iconbar, menu bar, address bar and the status bar.

   This function returns the window frame text colour in the GRADIENT.

   NB: Before any windows are drawn, make sure you set the borders first
   with set_border_style(...) so that an appropriate border colour and width
   will be used.
*/


int get_window_size_ex(int w, int h, int *winw, int *winh,
    int title, int iconbar, int menubar, int addbar, int statbar,
    int handle, GRADIENT *grad)
{
   int y, col;

   y = h + iconbar + menubar + addbar + statbar +
              ( borderwidth + bevelwidth) * 2;
   if (title)   y += title + borderwidth;
   if (handle)  y += handle + borderwidth;
   *winh = y;
   *winw = w + (borderwidth + bevelwidth) * 2;

   col = grad[GRADIENTWINDOWFRAME].txtcolour;

   return makecol((col & 0xFF0000) >> 16, (col & 0xFF00) >> 8, col & 0xFF);
}



int get_gradient_window_size(int w, int h, int *winw, int *winh,
    GRADIENTWINDOW *gwin, GRADIENT *grad)
{
   int y, col;

   y = h + gwin->iconbar + gwin->menubar + gwin->addrbar +
       gwin->statbar + (borderwidth + bevelwidth) * 2;

   if (gwin->title)   y += gwin->title + borderwidth;
   if (gwin->handle)  y += gwin->handle + borderwidth;
   *winh = y;
   *winw = w + (borderwidth + bevelwidth) * 2;

   col = grad[GRADIENTWINDOWFRAME].txtcolour;

   return makecol((col & 0xFF0000) >> 16, (col & 0xFF00) >> 8, col & 0xFF);
}



int get_window_size(int w, int h, int *winw, int *winh)
{
   return get_gradient_window_size(w, h, winw, winh, &gw, gradient);
}


/* draw_gradient_window_ex(...) draws a window with explicit parameters
   using the default gradient list gradient[].

   This procedure will draw a standard window with a title block, menubar,
   iconbar, addressbar, user frame, status bar and lower handle/grip.

   This function returns the window frame text colour in the current
   bit depth.

   Drawn on bitmap at xd, hd: width & height as specified:
   focus       decides between alternate gradient styles and
               can be FOCUSSED UNFOCUSSED or FOCUSPRESSED, although
               only the first two make any sense for windows.
   title       the height of the title bar at the top        (0 = none)
   menubar     the height of the menu below the title bar    (0 = none)
   iconbar     the height of the iconbar below it            (0 = none)
   addbar      the height of an address bar if any           (0 = none)
   statbar     a status bar (for help lines etc) below the users frame
   handle      the height of lower handle and edge grips if any
   button      specifies the titlebar buttons and clock. (0=none)
                  leftbuttons*256 + clockblocks*16 + rightbuttons.
                  (one clockblock is 16 pixels)
   icon        specifies the width of an icon placed left of the title label
                  so that it can be drawn in later.
   font        is the allegro FONT pointer
   label       the text string for the window label (in title bar)

   Fills a default window structure ( gw ) with the user's drawable frame
   dimensions (x y w h) and locations of title bar, icon, menubar, iconbar,
   address bar, status bar, handle, grips.

   The window structure is always the default GRADIENTWINDOW gw; and is
   overwritten every time this procedure is called. So copy out the
   info you need in order to draw on the various window objects.

   See draw_gradient_window(...) to use a GRADIENTWINDOW etc. of your own.
*/

int draw_gradient_window_ex(BITMAP *bmp, int xd, int yd, int wd, int hd,
    int focus, int title, int menubar, int iconbar, int addbar, int statbar,
    int handle, int button, int icon, FONT *font, char *label)
{
   int cx, x, y, w, h, num, col;
   int lx, ly, lw, lh, fh, bnum = 0;

   focus %= 3;
   x = xd;
   y = yd;
   w = wd;
   h = hd;
   if (borderwidth)         /* first draw the outline if required */
   {
      for (cx = 0; cx < borderwidth; cx++)
      {
         rect(bmp, x, y, x+w-1, y+h-1, bordercolour);
         x++;
         y++;
         w -= 2;
         h -= 2;
      }
   }
   /* x y w h are now dimensions inside the border lines if any */
   gw.wx = x;
   gw.wy = y;
   gw.ww = w;
   gw.wh = title;
   /* draw title block */
   lx = x + icon + 8;         /* find default title bar label location */
   lh = text_height(font) + 4;
   ly = y + (title - lh) / 2;
   lw = w - icon - 16;
   if (title)
   {
      draw_gradient(bmp, x, y, w, title, GRADIENTWINDOWTITLE, focus);
      /* fill in other parts of titlebar here */
      if (button & BUTTONRIGHTMASK)
      {
         num = button & BUTTONRIGHTMASK;
         for (cx = 0; cx < num; cx++)
         {
            draw_gradient(bmp, lx+lw-title+4, y+2, title-4, title-4,
               GRADIENTWINDOWBUTTON, focus);
            gw.bx[bnum] = lx+lw-title+4;      /* fill in button values */
            gw.by[bnum] = y+2;
            gw.bw[bnum] = title-4;
            gw.bh[bnum++] = title-4;
            lw -= title;
         }
      }
      if (button & BUTTONLEFTMASK)
      {
         num = (button & BUTTONLEFTMASK) >> 8;
         for (cx = 0; cx < num; cx++)
         {
            draw_gradient(bmp, lx, y+2, title-4, title-4,
               GRADIENTWINDOWBUTTON, focus);
            gw.bx[bnum] = lx;      /* fill in button values */
            gw.by[bnum] = y+2;
            gw.bw[bnum] = title-4;
            gw.bh[bnum++] = title-4;
            lw -= title;
            lx += title;
         }
      }
      if (button & BUTTONCLOCKMASK)
      {
         num = (button & BUTTONCLOCKMASK) >> 4;
         draw_gradient(bmp, lx+lw-title*num, ly, title*num, lh,
            GRADIENTCLOCK, focus);
         gw.ckx = lx+lw-title*num;      /* fill in clock values */
         gw.cky = ly;
         gw.ckw = title*num;
         gw.ckh = lh;
         lw -= title * num + 4;
      }
      draw_gradient_labelled(bmp, lx, ly, lw, lh,
            GRADIENTWINDOWLABEL, focus, font, label);
      h -= title;
      y += title;
      if (borderwidth)
      {
         for (cx = 0; cx < borderwidth; cx++)
         {
            hline(bmp, x, y, x+w-1, bordercolour);
            y++;
            h--;
         }
      }
   }
   gw.mx = x;
   gw.my = y;
   gw.mw = w;
   gw.mh = menubar;
   if (menubar)
   {
      draw_gradient(bmp, x, y, w, menubar, GRADIENTMENUBAR, focus);
      y += menubar;
      h -= menubar;
   }
   gw.ix = x;
   gw.iy = y;
   gw.iw = w;
   gw.ih = iconbar;
   if (iconbar)
   {
      draw_gradient(bmp, x, y, w, iconbar, GRADIENTICONBAR, focus);
      y += iconbar;
      h -= iconbar;
   }
   gw.ax = x;
   gw.ay = y;
   gw.aw = w;
   gw.ah = addbar;
   if (addbar)
   {
      draw_gradient(bmp, x, y, w, addbar, GRADIENTADDRESSBAR, focus);
      y += addbar;
      h -= addbar;
   }
   /* rest of the window, find users frameheight */
   fh = h - statbar;
   if (handle) fh -= handle + borderwidth;
   gw.ux = x;
   gw.uy = y;
   gw.uw = w;
   gw.uh = 0;
   if (h > handle)
   {
      draw_gradient(bmp, x, y, w, fh, GRADIENTWINDOWFRAME, focus);
      lx = x + bevelwidth;
      ly = y + bevelwidth;
      lw = w - 2 * bevelwidth;
      lh = fh - 2 * bevelwidth;
      gw.ux = lx;
      gw.uy = ly;
      gw.uw = lw;
      gw.uh = lh;
      if (bevelwidth)
      {
         rect(bmp, lx, ly, lx+lw-1, ly+lh-1, bordercolour);
      }
      y += fh;
      h -= fh;
   }
   /* statusbar */
   gw.sx = x;
   gw.sy = y;
   gw.sw = w;
   gw.sh = statbar;
   if (statbar)
   {
      draw_gradient(bmp, x, y, w, statbar, GRADIENTSTATUSBAR, focus);
      y += statbar;
      h -= statbar;
   }
   /* lower grips and handle */
   if (borderwidth)
   {
      for (cx = 0; cx < borderwidth; cx++)
      {
         hline(bmp, x, y, x+w-1, bordercolour);
         y++;
      }
   }
   h = handle;
   gw.hx = x + gripwidth;
   gw.hy = y;
   gw.hw = w - 2 * gripwidth;
   gw.hh = h;
   gw.lgx = x;
   gw.lgy = y;
   gw.lgw = gripwidth;
   gw.lgh = h;
   gw.rgx = x + w - gripwidth;
   gw.rgy = y;
   gw.rgw = gripwidth;
   gw.rgh = h;
   if (w  > 2 * gripwidth)
   {
      draw_gradient(bmp, x, y, gripwidth, handle, GRADIENTWINDOWGRIP, focus);
      draw_gradient(bmp, x + w - gripwidth, y, gripwidth, handle, GRADIENTWINDOWGRIP, focus);
      x += gripwidth;
      w -= 2 * gripwidth;
   }
   draw_gradient(bmp, x, y, w, handle, GRADIENTWINDOWHANDLE, focus);

   col = gradient[GRADIENTWINDOWFRAME].txtshadow;

   gw.shadcol =
          makecol((col & 0xFF0000) >> 16, (col & 0xFF00) >> 8, col & 0xFF);
   col = gradient[GRADIENTWINDOWFRAME].txtcolour;

   gw.textcol =
         makecol((col & 0xFF0000) >> 16, (col & 0xFF00) >> 8, col & 0xFF);
   return gw.textcol;
}



/* draw a window using a custom GRADIENTWINDOW and the users GRADIENT list,
   on the bitmap bmp at location xd, yd and size wd x hd,
   and assumes that a border style has already been set.

   This procedure will draw a standard window with a title block, menubar,
   iconbar, addressbar, user frame, status bar and lower handle/grip.

   This function returns the window frame text colour in the current
   bit depth.

   Drawn on bitmap at xd, hd: width & height as specified:
   focus       decides between alternate gradient styles and
               can be FOCUSSED UNFOCUSSED or FOCUSPRESSED, although
               only the first two make any sense for windows.
   font        is the allegro FONT pointer for the window label
   label       the text string for the window label (in title bar)
   gwin        a pointer to a GRADIENTWINDOW structure containing
               title bar, menubar, iconbar, addressbar, statusbar,
               buttons, icon and handles.
   grad        a GRADIENT structure array

   Fills the window structure ( gwin ) with the user's drawable frame
   dimensions (x y w h) and locations of title bar, icon, menubar, iconbar,
   address bar, status bar, handle, grips etc.
*/



int draw_gradient_window(BITMAP *bmp, int xd, int yd, int wd, int hd,
    int focus, FONT *font, char *label, GRADIENTWINDOW *gwin, GRADIENT *grad)
{
   int title, menubar, iconbar, addbar, statbar;
   int handle, button, icon, barhand, bnum = 0;
   int cx, x, y, w, h, num, col;
   int lx, ly, lw, lh, fh;

   title = gwin->title;        /* the things we do to save typing... */
   menubar = gwin->menubar;
   iconbar = gwin->iconbar;
   addbar = gwin->addrbar;
   statbar = gwin->statbar;
   handle = gwin->handle;
   button = gwin->button;
   icon = gwin->icon;
   /* fix up sizes if barhandles are present AND active */
   barhand = gwin->barhandles;
   if (barhand)
   {
      if (gwin->mb)  menubar = barhand;
      if (gwin->ib)  iconbar = barhand;
      if (gwin->ab)  addbar  = barhand;
   }
   focus %= 4;
   gwin->x = xd;
   gwin->y = yd;
   gwin->w = wd;
   gwin->h = hd;
   x = xd;
   y = yd;
   w = wd;
   h = hd;
   if (borderwidth)         /* first draw the outline if required */
   {
      for (cx = 0; cx < borderwidth; cx++)
      {
         rect(bmp, x, y, x+w-1, y+h-1, bordercolour);
         x++;
         y++;
         w -= 2;
         h -= 2;
      }
   }
   /* x y w h are now dimensions inside the border lines if any */
   gwin->wx = x;
   gwin->wy = y;
   gwin->ww = w;
   gwin->wh = title;
   gwin->ckx = 0;        /* just in case there is no clock */
   gwin->cky = 0;
   gwin->ckw = 0;
   gwin->ckh = 0;
   /* draw title block */
   lx = x + icon + 8;         /* find default title bar label location */
   lh = text_height(font) + 4;
   ly = y + (title - lh) / 2;
   lw = w - icon - 16;
   if (title)
   {
      draw_gradient_index(bmp, x, y, w, title, GRADIENTWINDOWTITLE,
        focus, grad);
      /* fill in other parts of titlebar here */
      if (button & BUTTONRIGHTMASK)
      {
         num = button & BUTTONRIGHTMASK;
         for (cx = 0; cx < num; cx++)
         {
            draw_gradient_index(bmp, lx+lw-title+4, y+2, title-4, title-4,
               GRADIENTWINDOWBUTTON, focus, grad);
            gwin->bx[bnum] = lx+lw-title+4;      /* fill in button values */
            gwin->by[bnum] = y+2;
            gwin->bw[bnum] = title-4;
            gwin->bh[bnum++] = title-4;
            lw -= title;
         }
      }
      if (button & BUTTONLEFTMASK)
      {
         num = (button & BUTTONLEFTMASK) >> 8;
         for (cx = 0; cx < num; cx++)
         {
            draw_gradient_index(bmp, lx, y+2, title-4, title-4,
               GRADIENTWINDOWBUTTON, focus, grad);
            gwin->bx[bnum] = lx;      /* fill in button values */
            gwin->by[bnum] = y+2;
            gwin->bw[bnum] = title-4;
            gwin->bh[bnum++] = title-4;
            lw -= title;
            lx += title;
         }
      }
      if (button & BUTTONCLOCKMASK)
      {
         num = (button & BUTTONCLOCKMASK) >> 4;
         draw_gradient_index(bmp, lx+lw-title*num, ly, title*num, lh,
            GRADIENTCLOCK, focus, grad);
         gwin->ckx = lx+lw-title*num;      /* fill in clock values */
         gwin->cky = ly;
         gwin->ckw = title*num;
         gwin->ckh = lh;

         lw -= title * num + 4;
      }
      draw_gradient_index_labelled(bmp, lx, ly, lw, lh,
            GRADIENTWINDOWLABEL, focus, font, label, grad);
      h -= title;
      y += title;
      if (borderwidth)
      {
         for (cx = 0; cx < borderwidth; cx++)
         {
            hline(bmp, x, y, x+w-1, bordercolour);
            y++;
            h--;
         }
      }
   }
   gwin->mx = x;    /* menubar location includes handle, if present */
   gwin->my = y;
   gwin->mw = w;
   gwin->mh = menubar;
   if (menubar)
   {
      if (barhand)
      {
         draw_gradient_index(bmp, x, y, barhand, menubar, GRADIENTMENUHANDLE,
           focus, grad);
      }
      draw_gradient_index(bmp, x+barhand, y, w-barhand, menubar,
         GRADIENTMENUBAR, focus, grad);
      y += menubar;
      h -= menubar;
   }
   gwin->ix = x;
   gwin->iy = y;
   gwin->iw = w;
   gwin->ih = iconbar;
   if (iconbar)
   {
      if (barhand)
      {
         draw_gradient_index(bmp, x, y, barhand, iconbar, GRADIENTMENUHANDLE,
           focus, grad);
      }
      draw_gradient_index(bmp, x+barhand, y, w-barhand, iconbar,
         GRADIENTICONBAR, focus, grad);
      y += iconbar;
      h -= iconbar;
   }
   gwin->ax = x;
   gwin->ay = y;
   gwin->aw = w;
   gwin->ah = addbar;
   if (addbar)
   {
      if (barhand)
      {
         draw_gradient_index(bmp, x, y, barhand, addbar, GRADIENTMENUHANDLE,
           focus, grad);
      }
      draw_gradient_index(bmp, x+barhand, y, w-barhand, addbar,
         GRADIENTADDRESSBAR, focus, grad);
      y += addbar;
      h -= addbar;
   }
   /* rest of the window, find users frameheight */
   fh = h - statbar;
   if (handle) fh -= handle + borderwidth;
   gwin->ux = x;
   gwin->uy = y;
   gwin->uw = w;
   gwin->uh = 0;
   if (h > handle)
   {              /* check if a wallpaper is present (don't overdraw it) */
      if (gwin->wpaper == 0)
         draw_gradient_index(bmp, x, y, w, fh, GRADIENTWINDOWFRAME,
            focus, grad);
      else
         rect(bmp, x, y, x+w-1, y+h-1, bordercolour);
      lx = x + bevelwidth;
      ly = y + bevelwidth;
      lw = w - 2 * bevelwidth;
      lh = fh - 2 * bevelwidth;
      gwin->ux = lx;
      gwin->uy = ly;
      gwin->uw = lw;
      gwin->uh = lh;
      if (bevelwidth)
      {
         rect(bmp, lx, ly, lx+lw-1, ly+lh-1, bordercolour);
      }
      y += fh;
      h -= fh;
   }
   /* statusbar */
   gwin->sx = x;
   gwin->sy = y;
   gwin->sw = w;
   gwin->sh = statbar;
   if (statbar)
   {
      draw_gradient_index(bmp, x, y, w, statbar, GRADIENTSTATUSBAR,
         focus, grad);
      y += statbar;
      h -= statbar;
   }
   /* lower grips and handle */
   if (borderwidth)
   {
      for (cx = 0; cx < borderwidth; cx++)
      {
         hline(bmp, x, y, x+w-1, bordercolour);
         y++;
      }
   }
   h = handle;
   gwin->hx = x + gripwidth;
   gwin->hy = y;
   gwin->hw = w - 2 * gripwidth;
   gwin->hh = h;
   gwin->lgx = x;
   gwin->lgy = y;
   gwin->lgw = gripwidth;
   gwin->lgh = h;
   gwin->rgx = x + w - gripwidth;
   gwin->rgy = y;
   gwin->rgw = gripwidth;
   gwin->rgh = h;
   if (w  > 2 * gripwidth)
   {
      draw_gradient_index(bmp, x, y, gripwidth, handle, GRADIENTWINDOWGRIP,
         focus, grad);
      draw_gradient_index(bmp, x + w - gripwidth, y, gripwidth, handle,
         GRADIENTWINDOWGRIP, focus, grad);
      x += gripwidth;
      w -= 2 * gripwidth;
   }
   draw_gradient_index(bmp, x, y, w, handle, GRADIENTWINDOWHANDLE,
      focus, grad);

   col = grad[GRADIENTWINDOWFRAME].txtshadow;

   gwin->shadcol =
          makecol((col & 0xFF0000) >> 16, (col & 0xFF00) >> 8, col & 0xFF);
   col = grad[GRADIENTWINDOWFRAME].txtcolour;

   gwin->textcol =
          makecol((col & 0xFF0000) >> 16, (col & 0xFF00) >> 8, col & 0xFF);
   return gwin->textcol;
}


/* Draw a window on the specified bitmap at location x, y and with a
   size w x h, using the default GRADIENTWINDOW gw, and the default
   gradient/theme gradient[].  Be sure to set_default_window_style() first!
   This function returns the user frame text colour. */

int draw_default_window(BITMAP *bmp, int x, int y, int w, int h,
                        int focus, FONT *font, char *label)
{
   return draw_gradient_window(bmp, x, y, w, h, focus, font, label,
                              &gw, gradient);
}




/* open a gradient window, saving background.

   Note: the GRADIENTWINDOW structure must already have the overall
   size stored in the structure at win->w and win->h, and the location
   of the window stored at win->x and win->y. This can be done
   manually or following a call to get_window_size(...).
   The background is saved using the win->bgbmp pointer.

   It returns the current text colour */

int open_gradient_window(BITMAP *bmp, int focus, FONT *font, char *name,
       GRADIENTWINDOW *win, GRADIENT *grad)
{
   int txtcol;

   if (win->bgbmp == NULL)
      win->bgbmp = create_bitmap(win->w, win->h);

   blit(bmp, win->bgbmp, win->x, win->y, 0, 0, win->w, win->h);

   txtcol = draw_gradient_window(bmp, win->x, win->y, win->w, win->h, focus,
      font, name, win, grad);

   return txtcol;
}


/* redraw a previously opened gradient/window, WITHOUT saving background.
   Use this to redraw with a different focus. */

int redraw_gradient_window(BITMAP *bmp, int focus, FONT *font, char *name,
       GRADIENTWINDOW *win, GRADIENT *grad)
{
   int txtcol;

   txtcol = draw_gradient_window(bmp, win->x, win->y, win->w, win->h, focus,
      font, name, win, grad);

   return txtcol;
}


/* close an opened gradient/window, restoring background contents */

int close_gradient_window(BITMAP *bmp, GRADIENTWINDOW *win)
{
   blit(win->bgbmp, bmp, 0, 0, win->x, win->y, win->w, win->h);

   destroy_bitmap(win->bgbmp);
   win->bgbmp = NULL;
   return 0;
}


/* ********* WINDOWBUTTON FUNCTIONS   *********** */

/* These functions use the WINDOWBUTTON structure to define and draw
   on screen buttons using gradient themes or "skins" stored in a bitmap.
*/

/* The set_windowbuton_userdata() function stores the user parameters
   xp, yp, wp and hp in the WINDOWBUTTON structure for later use by
   user supplied routines.
   These parameters are NOT used in the drawing routine and can be used
   for whatever you like. I use them in my theme editor to provide scaling
   so that the buttons and controls may be located properly with
   differing screen resolutions. For example, at 640x480, I use a scaling
   grid of 8 pixels per unit (in 800x600 it is 10 pixels) and multiply it
   with the values in xp etc to compute the dfault value for its location.

   wb      a pointer to the WINDOWBUTTON structure. ie &wb[EXITBOX]
*/

void set_windowbutton_userdata(int xp, int yp, int wp, int hp,
      WINDOWBUTTON *wb)
{
   wb->xp = xp;
   wb->yp = yp;
   wb->wp = wp;
   wb->hp = hp;
}

/* This set_windowbuton_data() function fills the WINDOWBUTTON structure
   with values for later use by the drawing routines.

   skin    if 0-23, the draw routine should use gadients, 24++ a "skin".
   retkey  the return key to use if the button is pressed.
   idx     an index into an external user-supplied array that is
           set up for values associated with this button.
   vis     visibility flag: 0 = not drawn, 1 = draw.
   box     the type of box or button this is.
           This can be any value you like, and is supplied for the use
           of your own routines to draw sliders or special controls.
   label   a string to be drawn as a label on the button.
           An empty string is not drawn. If a skin is used, the label
           will be drawn in the current colour depth for a gradient type
           as specified by the wb->grtype parameter, eg. GRADIENTBUTTON
           using the text colour (with shadow if specified) in the
           gradient used at draw time.
   hint    a string to be used in a tooltip or help line.
   wb      a pointer to the WINDOWBUTTON structure. ie &wb[EXITBOX]
*/

void set_windowbutton_data(int skin, int retkey, int idx, int vis,
    int box, char *label, char *hint, WINDOWBUTTON *wb)
{
   wb->skin = skin;
   wb->retkey = retkey;
   wb->idx = idx;
   wb->vis = vis;
   wb->box = box;
   wb->label = label;
   wb->hint = hint;
}

/* This set_windowbuton_location() function fills the WINDOWBUTTON structure
   with values for later use by the drawing routines.
   x, y    the location on the target bitmap.
   w, h    the size of the button or control.

   wb      a pointer to the WINDOWBUTTON structure. ie &wb[EXITBOX]
*/

void set_windowbutton_location(int x, int y, int w, int h, WINDOWBUTTON *wb)
{
   wb->x = x;
   wb->y = y;
   wb->w = w;
   wb->h = h;
}

/* The set_windowbuton_skin() function fills the WINDOWBUTTON structure
   with values derived from a "skin" for later use by the drawing routines.

   skinbmp       a bitmap containing appropriate images for use as a "skin".
   x, y, w, h    the location and size of the button images.
                 The location refers to the start of 4 images, stacked
                 vertically in order: Disabled, Unfocussed, Focussed
                 and Pressed. One of the four will be blitted to the
                 target bitmap by the drawing routine, depending on
                 the 'focus' option.

   wb      a pointer to the WINDOWBUTTON structure. ie &wb[EXITBOX]
*/

void set_windowbutton_skin(BITMAP *skinbmp, int x, int y, int w, int h,
    WINDOWBUTTON *wb)
{
   wb->bx = x;
   wb->by = y;
   wb->bw = w;
   wb->bh = h;
}

/* The set_windowbuton_focus() function changes the focus option in
   the WINDOWBUTTON structure with a new value for the drawing routines.

   focus   the desired focus option i.e. FOCUSSED
   wb      a pointer to the WINDOWBUTTON structure. ie &wb[EXITBOX]
*/

void set_windowbutton_focus(int focus, WINDOWBUTTON *wb)
{
   wb->focus = focus;
}

/* The set_windowbuton_visible() function changes the visibility option in
   the WINDOWBUTTON structure with a new value for the drawing routines.

   visible  the desired visibility flag i.e. 0=invisible  1=visible
   wb       a pointer to the WINDOWBUTTON structure. ie &wb[EXITBOX]
*/

void set_windowbutton_visible(int visible, WINDOWBUTTON *wb)
{
   wb->vis = visible;
}


/* This function draws the WINDOWBUTTON specified, on the bitmap at the
   supplied location and size using the wb=>grtype index of the gradient
   array grad[]. It returns the text colour in the current colourdepth.
   If the skin flag is 0, the gradient functions are used.
   If the skin flag is nonzero, a "skin" bitmap is used.
   A text label is added if the button has a valid label.

   x, y, w, h   the location and size on the target bitmap.
   focus        the desired focus option i.e. FOCUSSED
   wb           a pointer to the WINDOWBUTTON structure. ie &wb[EXITBOX]
*/

int draw_gradient_windowbutton(BITMAP *bmp, int x, int y, int w, int h,
    int focus, WINDOWBUTTON *wb, GRADIENT *grad)
{
   int index, col;

   index = wb->skin;
   if (index >= MAXGRADIENTS && wb->bmp == NULL) index = 0;
   col = get_gradient_colour(index, GRADIENTTEXTCOLOUR, focus, grad);

   /* first, check for a skin */
   if ((wb->skin >= MAXGRADIENTS) && (wb->bmp != NULL))  /* if it exists */
   {
      masked_blit(wb->bmp, bmp, wb->bx, wb->by+focus*wb->bh, x, y, w, h);
      if (*wb->label)
         draw_gradient_label_only(bmp, x, y, w, h,
           col, font, wb->label, grad[index].justify);
   }
   else        /* otherwise draw a gradient.. */
   {
      if (*wb->label)
         draw_gradient_index_labelled(bmp, x, y, w, h, index, focus,
            font, wb->label, grad);
      else
         draw_gradient_index(bmp, x, y, w, h, index, focus, grad);
   }
   return col;
}


/* This function draws the WINDOWBUTTON specified on the bitmap at the
   supplied location and size using the wb->skin index of the default
   gradient array. It returns the text colour in the current colourdepth.

   x, y, w, h   the location and size on the target bitmap.
   focus        the desired focus option i.e. FOCUSSED
   wb      a pointer to the WINDOWBUTTON structure. ie &wb[EXITBOX]
*/

int draw_default_windowbutton(BITMAP *bmp, int x, int y, int w, int h,
    int focus, WINDOWBUTTON *wb)
{
   return draw_gradient_windowbutton(bmp, x, y, w, h, focus, wb, gradient);

}


/* This function draws the WINDOWBUTTON specified on the bitmap at the
   location and size as specified in the WINDOWBUTTON structure
   using the wb->skin index of the default gradient array.
   It returns the text colour in the current colourdepth.


   focus   the desired focus option i.e. FOCUSSED
   wb      a pointer to the WINDOWBUTTON structure. ie &wb[EXITBOX]
*/

int draw_windowbutton(BITMAP *bmp, int focus, WINDOWBUTTON *wb)
{
   return draw_gradient_windowbutton(bmp, wb->x, wb->y, wb->w, wb->h,
      focus, wb, gradient);
}


/* *************** MENU FUNCTIONS   **************** */


/*
typedef struct gradientmenu
{
   char *title;         // title
   char *subm[20];      // options
   int hook;            // hook to? 0=none 1=menubar
   int num;             // number of options
   int sel;             // current selection
   int retkey[20];      // return key or function each option
   int x, y, w, h;      // total size incl borders, hook=0, frame if hook=1
   int fx, fy, fw, fh;  // frame size inside borders
   int pitch;           // options pitch
   int bull[20];        // bullet for next menu 0=none 1,,19=submenu
} GRADIENTMENU;
*/

/* find the size of a menu frame using the current borderwidth
   As a bonus, it returns the width of the menu label plus an 8+8 margin */

int get_menu_frame_size(int *w, int *h, FONT *font, GRADIENTMENU *mnu)
{
   int cx, wd, bullet = 0;
   int wt, len;

   wt = text_length(font, mnu->label);
   wd = wt;
   /* check menu options for size and bullets */
   for (cx = 0; cx < mnu->num; cx++)
   {
      len = text_length(font, mnu->subm[cx]);
      if (len > wd)  wd = len;
      if (mnu->bull[cx])  bullet = 1;
   }
   wd += 16;     /* add minimum margins, 8+8 */
   wt += 16;
   if (bullet)  wd += mnu->pitch;     /* add bullet width; */
   *w = wd;

   *h = borderwidth * 2 + mnu->num * mnu->pitch;
   mnu->fw = wd;
   mnu->fh = *h;
   return wt;
}


/* find the size of the menu, so that you can save the background.
   We pass the x, y location to store in the menu (saves time later).
   Always call this before drawing a menu so that the sizes can be stored */

int get_gradient_menu_size(int x, int y, FONT *font, GRADIENTMENU *mnu)
{
   int hd, wd, wt;

   wt = get_menu_frame_size(&wd, &hd, font, mnu);
   if (wd < wt) wd = wt;
   /*
   mnu->fh = hd;
   mnu->fw = wd;
   */
   hd += (borderwidth + mnu->pitch);
   mnu->x = x;
   mnu->y = y;
   mnu->w = wd;
   mnu->h = hd;
   mnu->fx = x;
   mnu->fy = y + borderwidth + mnu->pitch;
   return wt;
}



/* draw menu titleblock and a frame holding menu items, on the bitmap
   at xd, yd given the vertical menu item pitch, number of submenus,
   font and title text, array of submenu names and array of bullet flags.

   This routine uses the default gradient list gradient[] */

int draw_gradient_menu_ex(BITMAP *bmp, int xd, int yd, int pitch, int hilite,
      int num, FONT *font, char *title, char *str[], int *bflag)
{
   int cx, x, y, w, h, hd, wd, bull = 0;
   int xt, yt, wt, ht, len, yoffs, jst, marg = 8;
   int index, r, g, b, col, bullet, coltex, colhi;
   int bullwidth = pitch;

   jst = gradient[GRADIENTMENUTITLE].justify;
/*   if (jst == 0)   jst = gradient[GRADIENTMENUBULLET].justify; */

   col = gradient[GRADIENTMENUFRAME].txtcolour;
   r = (col & 0xFF0000) >> 16;
   g = (col & 0xFF00)   >> 8;
   b = col & 0xFF;
   coltex = makecol(r, g, b);
   col = gradient[GRADIENTMENUHILITE].txtcolour;
   r = (col & 0xFF0000) >> 16;
   g = (col & 0xFF00)   >> 8;
   b = col & 0xFF;
   colhi = makecol(r, g, b);

   wt = text_length(font, title);
   wd = wt;
   for (cx = 0; cx < num; cx++)
   {
      len = text_length(font, str[cx]);
      if (len > wd)  wd = len;
      if (bflag[cx])  bull = 1;
   }
   wd += marg * 2;
   if (bull)  wd += bullwidth;
   x = xd;
   y = yd;
   w = wd;
   hd = borderwidth * 3 + (num + 1) * pitch;
   h = hd;
   if (borderwidth)
   {
      for (cx = 0; cx < borderwidth; cx++)
      {
         rect(bmp, x, y, x+w-1, y+h-1, bordercolour);
         x++;
         y++;
         w -= 2;
         h -= 2;
      }
   }
   /* x y w h are now dimensions inside the border lines if any */
   /* draw menu title */
   index = GRADIENTMENUTITLE;
   draw_gradient_labelled(bmp, x, y, w, pitch, index, FOCUSSED, font, title);
   y += pitch;
   h -= pitch;
   if (borderwidth)
   {
      for (cx = 0; cx < borderwidth; cx++)
      {
         hline(bmp, x, y, x+w-1, bordercolour);
         y++;
         h--;
      }
   }
   /* x y w h = rest of the menu frame */
   index = GRADIENTMENUFRAME;
   draw_colour_gradient(bmp, x, y, w, h, gradient[index].colour,
        gradient[index].colourto, gradient[index].method);
   for (cx = 0; cx < num; cx++)   /* draw submenu items with highlight */
   {
   /* find text justification, colour & position */
   /*
      | lefttext       |  |B| lefttext       |   | lefttext       |B|
      |m  centertext  m|  |B|m  centertext  m|   |   centertext   |B|
      |      righttext |  |B|      righttext |   |      righttext |B|
      x   xt    nobull    leftbull               rightbull
                left         center           right
      nobull   xt=x+m      x+(w-wt)/2       x+w-m-wt
      leftbull    x+B+m    x+B+(w-B-wt)/2     x+w-m-wt
      rightbull   x+m      x+(w-B-wt)/2       x+w-B-m-wt
   */
      index = GRADIENTMENUFRAME;
      xt = x + marg;            /* left justify */
      if (bull && bulletpos == JUSTIFYLEFT)
         xt += bullwidth;
      yt = y + cx * pitch;
      wt = text_length(font, str[cx]);
      ht = text_height(font);
      yoffs = (pitch - ht) / 2;
      if (gradient[index].justify == JUSTIFYRIGHT)
      {
         xt = x + w - wt - marg;
         if (bull && bulletpos == JUSTIFYRIGHT)
           xt -= bullwidth;
      }
      if (gradient[index].justify == JUSTIFYCENTER)
      {
         xt = x + (w - wt) / 2;
         if (bull && bulletpos == JUSTIFYLEFT)
           xt = x + bullwidth + (w - bullwidth - wt) / 2;
         if (bull && bulletpos == JUSTIFYRIGHT)
           xt = x + ( w - bullwidth - wt) / 2;
      }
      if (hilite == cx)
      {
         draw_colour_gradient(bmp, x, yt, w, pitch,
           gradient[GRADIENTMENUHILITE].colour,
           gradient[GRADIENTMENUHILITE].colourto,
           gradient[GRADIENTMENUHILITE].method);
         textout(bmp, font, str[cx], xt, yt+yoffs, colhi);
      }
      else
      {
         textout(bmp, font, str[cx], xt, yt+yoffs, coltex);
      }
      /* now the bullet pictograph or tick mark */
      if (bull && bflag[cx])
      {
         index = GRADIENTMENUBULLET;
         if (bulletpos == JUSTIFYLEFT) xt = x;
         else                          xt = x + w - bullwidth;
         if (hilite == cx)
            col = colhi;
         else col = coltex;
         /* and the pictograph */
         bullet = bulletstyle & 0x07;
         if (bflag[cx] >= BCHECKBOXOFF) bullet = bflag[cx];

         xt += pitch/2;
         yt += pitch/2;
         if (bullet == BCIRCLE)
            circle(bmp, xt, yt, pitch/4, col);
         if (bullet == BROUND)
            circlefill(bmp, xt, yt, pitch/5, col);
         if (bullet == BSQUARE)
            rectfill(bmp, xt-2, yt-2, xt+2, yt+2, col);
         if (bullet == BTRI)
            triangle(bmp, xt-3, yt-3, xt-3, yt+3, xt+3, yt, col);
         if (bullet == BDIAMOND)
         {
            putpixel(bmp, xt, yt-4, col);
            putpixel(bmp, xt, yt+4, col);
            hline(bmp, xt-1, yt-3, xt+1, col);
            hline(bmp, xt-1, yt+3, xt+1, col);
            hline(bmp, xt-2, yt-2, xt+2, col);
            hline(bmp, xt-2, yt+2, xt+2, col);
            hline(bmp, xt-3, yt-1, xt+3, col);
            hline(bmp, xt-3, yt+1, xt+3, col);
            hline(bmp, xt-4, yt, xt+4, col);
         }
         if (bullet == BCHECKBOXON)
         {
            line(bmp, xt-2, yt, xt, yt+2, col);
            line(bmp, xt, yt+2, xt+4, yt-2, col);
            yt++;
            line(bmp, xt-2, yt, xt, yt+2, col);
            line(bmp, xt, yt+2, xt+4, yt-2, col);
         }
      }
   }

   return 0;
}


/* draw menu titleblock and a frame holding menu items, on the bitmap
   at xd, yd given the GRADIENTMENU object mnu, font and a user
   specified gradient list grad[] */

int draw_gradient_menu(BITMAP *bmp, int xd, int yd, FONT *font,
       GRADIENTMENU *mnu, GRADIENT *grad)
{
   int cx, x, y, w, h, hd, wd, bull = 0;
   int xt, yt, wt, ht, yoffs, jst, marg = 8;
   int index, r, g, b, col, bullet, coltex, colhi;
   int pitch;
   int hilite;
   int num;

   num = mnu->num;
   pitch = mnu->pitch;
   hilite = mnu->sel;
   jst = grad[GRADIENTMENUTITLE].justify;
   /* Normal and Hilite text colour */
   col = grad[GRADIENTMENUFRAME].txtcolour;
   r = (col & 0xFF0000) >> 16;
   g = (col & 0xFF00)   >> 8;
   b = col & 0xFF;
   coltex = makecol(r, g, b);
   col = grad[GRADIENTMENUHILITE].txtcolour;
   r = (col & 0xFF0000) >> 16;
   g = (col & 0xFF00)   >> 8;
   b = col & 0xFF;
   colhi = makecol(r, g, b);
   /* determine sizes */
   wt = get_gradient_menu_size(xd, yd, font, mnu);
   wd = mnu->w;
   hd = mnu->h;
   x = xd;
   y = yd;
   w = wd;
   h = hd;
   /* draw border */
   if (borderwidth)
   {
      for (cx = 0; cx < borderwidth; cx++)
      {
         rect(bmp, x, y, x+w-1, y+h-1, bordercolour);
         x++;
         y++;
         w -= 2;
         h -= 2;
      }
   }
   for (cx = 0; cx < num; cx++)   /* draw submenu items with highlight */
   {
      if (mnu->bull[cx] > 0) bull++;
   }
   /* x y w h are now dimensions inside the border lines if any */
   /* draw menu title */
   index = GRADIENTMENUTITLE;
   draw_gradient_index_labelled(bmp, x, y, w, pitch, index,
      FOCUSSED, font, mnu->label, grad);
   y += pitch;
   h -= pitch;
   if (borderwidth)
   {
      for (cx = 0; cx < borderwidth; cx++)
      {
         hline(bmp, x, y, x+w-1, bordercolour);
         y++;
         h--;
      }
   }
   /* x y w h = rest of the menu frame */
   index = GRADIENTMENUFRAME;
   draw_colour_gradient(bmp, x, y, w, h, grad[index].colour,
        grad[index].colourto, grad[index].method);
   for (cx = 0; cx < num; cx++)   /* draw submenu items with highlight */
   {
   /* find text justification, colour & position */
   /*
      | lefttext       |  |B| lefttext       |   | lefttext       |B|
      |m  centertext  m|  |B|m  centertext  m|   |   centertext   |B|
      |      righttext |  |B|      righttext |   |      righttext |B|
      x   xt    nobull    leftbull               rightbull
                left         center           right
      nobull   xt=x+m      x+(w-wt)/2       x+w-m-wt
      leftbull    x+B+m    x+B+(w-B-wt)/2     x+w-m-wt
      rightbull   x+m      x+(w-B-wt)/2       x+w-B-m-wt
   */
      index = GRADIENTMENUFRAME;
      xt = x + marg;            /* left justify */
      if (bull && bulletpos == JUSTIFYLEFT)
         xt += pitch;
      yt = y + cx * pitch;
      wt = text_length(font, mnu->subm[cx]);
      ht = text_height(font);
      yoffs = (pitch - ht) / 2;
      if (grad[index].justify == JUSTIFYRIGHT)
      {
         xt = x + w - wt - marg;
         if (bull && bulletpos == JUSTIFYRIGHT)
           xt -= pitch;
      }
      if (grad[index].justify == JUSTIFYCENTER)
      {
         xt = x + (w - wt) / 2;
         if (bull && bulletpos == JUSTIFYLEFT)
           xt = x + pitch + (w - pitch - wt) / 2;
         if (bull && bulletpos == JUSTIFYRIGHT)
           xt = x + ( w - pitch - wt) / 2;
      }
      if (hilite == cx)
      {
         draw_colour_gradient(bmp, x, yt, w, pitch,
            grad[GRADIENTMENUHILITE].colour,
            grad[GRADIENTMENUHILITE].colourto,
            grad[GRADIENTMENUHILITE].method);
         textout(bmp, font, mnu->subm[cx], xt, yt+yoffs, colhi);
      }
      else
      {
         textout(bmp, font, mnu->subm[cx], xt, yt+yoffs, coltex);
      }
      /* now the bullet pictograph */
      if (bull && mnu->bull[cx])
      {
         if (bulletpos == JUSTIFYLEFT) xt = x;
         else                          xt = x + w - pitch;
         if (hilite == cx)   col = colhi;
         else                col = coltex;
         /* and the pictograph */
         bullet = bulletstyle & 0x07;
         if (mnu->bull[cx] >= BCHECKBOXOFF) bullet = mnu->bull[cx];

         xt += pitch/2;
         yt += pitch/2;
         if (bullet == BCIRCLE)
            circle(bmp, xt, yt, pitch/4, col);
         if (bullet == BROUND)
            circlefill(bmp, xt, yt, pitch/5, col);
         if (bullet == BSQUARE)
            rectfill(bmp, xt-2, yt-2, xt+2, yt+2, col);
         if (bullet == BTRI)
            triangle(bmp, xt-3, yt-3, xt-3, yt+3, xt+3, yt, col);
         if (bullet == BDIAMOND)
         {
            putpixel(bmp, xt, yt-4, col);
            putpixel(bmp, xt, yt+4, col);
            hline(bmp, xt-1, yt-3, xt+1, col);
            hline(bmp, xt-1, yt+3, xt+1, col);
            hline(bmp, xt-2, yt-2, xt+2, col);
            hline(bmp, xt-2, yt+2, xt+2, col);
            hline(bmp, xt-3, yt-1, xt+3, col);
            hline(bmp, xt-3, yt+1, xt+3, col);
            hline(bmp, xt-4, yt, xt+4, col);
         }
         if (bullet == BCHECKBOXON)
         {
            line(bmp, xt-2, yt, xt, yt+2, col);
            line(bmp, xt, yt+2, xt+4, yt-2, col);
            yt++;
            line(bmp, xt-2, yt, xt, yt+2, col);
            line(bmp, xt, yt+2, xt+4, yt-2, col);
         }
      }
   }

   return 0;
}




/* draw menu titleblock and a frame holding menu items, on the bitmap
   at x, y given the GRADIENTMENU object mnu, font and the default
   gradient list gradient[] */

int draw_default_menu(BITMAP *bmp, int x, int y, FONT *font,
       GRADIENTMENU *mnu)
{
   return draw_gradient_menu(bmp, x, y, font, mnu, gradient);
}



/* find the total size of the menu, using explicit parameters, so that
   you can save the background. Returns label width plus 8+8 pix margin */

int get_gradient_menu_size_ex(int *w, int *h, int pitch, int num, FONT *font,
      char *title, char *str[], int *bflag)
{
   int cx, hd, wd, bull = 0;
   int wt, len;

   wt = text_length(font, title);
   wd = wt;
   for (cx = 0; cx < num; cx++)
   {
      len = text_length(font, str[cx]);
      if (len > wd)  wd = len;
      if (bflag[cx])  bull = 1;
   }
   wd += 16;
   wt += 16;
   if (bull)  wd += pitch;
   *w = wd;

   hd = borderwidth * 3 + (num + 1) * pitch;
   *h = hd;
   return wt;
}


/* *************** TOOLBAR FUNCTIONS   **************** */


/* Draw a basic toolbar for users buttons, label, icons and a clock.
   Toolbars should always drawn FOCUSSED. Individual buttons may be
   redrawn with other focus options later, e.g. if pressed.

   tbar->x[] etc locations are for:
     0=handle 1=toolbar 2=label 3=icon space 4=clock 5... user

     First, using the internal default gradients... */

int draw_default_toolbar(BITMAP *bmp, int x, int y, int w, int h,
     int focus, FONT *font, char *label, TOOLBAR *tbar)
{
   return draw_gradient_toolbar(bmp, x, y, w, h, focus,
                         font, label, gradient, tbar);
}

/* Next, using your own gradients */

int draw_gradient_toolbar(BITMAP *bmp, int xd, int yd, int wd, int hd,
     int focus, FONT *font, char *label, GRADIENT *grad, TOOLBAR *tbar)
{
   int cx = 0, x = 0, y = 0, w = 0, h = 0, bnum = 0;
   int lx=0, ly = 0, lw = 0, lh = 0;
   int handle = tbar->handle;

   x = xd;
   y = yd;
   w = wd;
   h = hd;

   if (borderwidth)         /* first draw the outline if required */
   {
      for (cx = 0; cx < borderwidth; cx++)
      {
         rect(bmp, x, y, x+w-1, y+h-1, bordercolour);
         x++;
         y++;
         w -= 2;
         h -= 2;
      }
   }
   /* x y w h are now dimensions inside the border lines if any */
   tbar->x[0] = x;       /* handle dimensions */
   tbar->y[0] = y;
   tbar->w[0] = handle;
   tbar->h[0] = h;
   if (handle)
   {
      draw_gradient_index(bmp, x, y, handle, h,
         GRADIENTTOOLBARHANDLE, focus, grad);
      x += handle;
      w -= handle;
   }
   tbar->x[1] = x;       /* main toolbar size */
   tbar->y[1] = y;
   tbar->w[1] = w;
   tbar->h[1] = h;

   /* draw toolbar block */
   draw_gradient_index(bmp, x, y, w, h, GRADIENTTOOLBAR, focus, grad);

   /* fill in other parts of toolbar later...
      ----------------------------------------------------------------
      | H |< mar>label< icons ><buttons> < available ><Rbut>clock<mar>|
      0---1------2----ix-------bx--------3-----------bx?----4---------
   */
   x += tbar->leftmargin;
   w -= tbar->leftmargin;
   tbar->x[2] = x;       /* TBLABELDIM label dimensions */
   tbar->y[2] = y;
   tbar->w[2] = 0;
   tbar->h[2] = h;
   if (*label)
   {
      lx = x;            /* find default toolbar label location */
      lw = text_length(font, label)+6;
      lh = text_height(font)+4;
      ly = y + (h - lh) / 2;
      draw_gradient_index_labelled(bmp, lx, ly, lw, lh,
            GRADIENTTOOLBARLABEL, focus, font, label, grad);
      tbar->x[2] = lx;       /* label dimensions */
      tbar->y[2] = ly;
      tbar->w[2] = lw;
      tbar->h[2] = lh;
      x += lw;
      w -= lw;
   }
   x += tbar->leftmargin;
   w -= tbar->leftmargin;
   lw = tbar->iconwide;
   lh = tbar->iconhigh;
   /* put sensible values in first icon in case none are drawn now */
   tbar->ix[0] = x;
   tbar->iw[0] = lw;
   tbar->iy[0] =  y + (h - lh) / 2;
   tbar->ih[0] = lh;
   if (tbar->icons)
   {
      lw = tbar->iconwide;
      lh = tbar->iconhigh;
      for (cx = 0; cx < tbar->icons; cx++)
      {
         lx = x;                    /* find icon */
         ly = y + (h - lh) / 2;
         draw_gradient_index(bmp, lx, ly, lw, lh, GRADIENTTOOLBARWINDOW,
            focus, grad);
         tbar->ix[cx] = lx;       /* TBICONDIM record icon dimensions */
         tbar->iy[cx] = ly;
         tbar->iw[cx] = lw;
         tbar->ih[cx] = lh;
         x += lw;
         w -= lw;
      }
   }
   bnum = 0;
   lw = tbar->butwide;
   lh = tbar->buthigh;
   x += tbar->leftmargin;
   w -= tbar->leftmargin;
   /* put sensible values in first button in case none are drawn now */
   tbar->bx[0] = x;
   tbar->bw[0] = lw;
   tbar->by[0] =  y + (h - lh) / 2;
   tbar->bh[0] = lh;
   if (tbar->lbuttons)
   {
      for (cx = 0; cx < tbar->lbuttons; cx++)
      {
         lx = x;    /* find button */
         ly = y + (h - lh) / 2;
         draw_gradient_index(bmp, lx, ly, lw, lh, GRADIENTTOOLBARBUTTON,
            focus, grad);
         tbar->bx[bnum] = lx;       /* TBBUTTONDIM button dimensions */
         tbar->by[bnum] = ly;
         tbar->bw[bnum] = lw;
         tbar->bh[bnum] = lh;
         x += lw+4;
         w -= lw+4;
         bnum++;
      }
   }
   tbar->x[3] = x;       /* available space */
   tbar->y[3] = y;
   tbar->w[3] = w;
   tbar->h[3] = h;
   tbar->x[4] = 0;       /* clock dimensions */
   tbar->y[4] = 0;
   tbar->w[4] = 0;
   tbar->h[4] = 0;
   if (tbar->clockwide)
   {
      lw = tbar->clockwide;
      lx = x + w - tbar->rightmargin - lw;    /* find clock location */
      lh = tbar->clockhigh;
      ly = y + (h - lh) / 2;
      draw_gradient_index(bmp, lx, ly, lw, lh, GRADIENTCLOCK, focus, grad);
      tbar->x[4] = lx;                        /* clock dimensions */
      tbar->y[4] = ly;
      tbar->w[4] = lw;
      tbar->h[4] = lh;
      tbar->w[3] -= lw;         /* a clock reduces available space */
   }
   lx -= tbar->leftmargin;
   w -= tbar->leftmargin;
   if (tbar->rbuttons)
   {
      lw = tbar->butwide;
      lh = tbar->buthigh;
      for (cx = 0; cx < tbar->rbuttons; cx++)
      {
         lx -= lh;    /* find button */
         ly = y + (h - lh) / 2;
         draw_gradient_index(bmp, lx, ly, lw, lh, GRADIENTTOOLBARBUTTON,
            focus, grad);
         if (bnum < 10)
         {
            tbar->bx[bnum] = lx;       /* more button dimensions */
            tbar->by[bnum] = ly;
            tbar->bw[bnum] = lw;
            tbar->bh[bnum] = lh;
            tbar->w[3] -= lw;
            bnum++;
         }
      }
   }
   return 0;
}

/* *************** TOOLTIP FUNCTIONS   **************** */


/* find the size of the tooltip frame, so that you can save the background */

int get_tooltip_dim(int lines, FONT *font, char *text[], int *wide, int *high)
{
   int cx, w, lw = 0;
   char **cp;

   cp = text;
   for (cx = 0; cx < lines; cx++)
   {
      w = text_length(font, *cp++);
      if (w > lw) lw = w;
   }
   *wide = lw + 8 + borderwidth * 2;
   *high = lines*text_height(font) + 8 + borderwidth * 2;
   return 0;
}



/* draw a tooltip at the specified location. It may have several
   lines of text. Use the specified gradient. */

int draw_gradient_tooltip(BITMAP *bmp, int xd, int yd, int wd, int hd,
     int lines, FONT *font, char *text[], GRADIENT *grad)
{
   int cx, x, y, w, h;
   int lx, ly, lh, col;
   int focus = FOCUSSED;

   x = xd;
   y = yd;
   w = wd;
   h = hd;

   if (borderwidth)         /* first draw the outline if required */
   {
      for (cx = 0; cx < borderwidth; cx++)
      {
         rect(bmp, x, y, x+w-1, y+h-1, bordercolour);
         x++;
         y++;
         w -= 2;
         h -= 2;
      }
   }
   /* x y w h are now dimensions inside the border lines if any */
   lx = grad[GRADIENTTOOLTIP].txtcolour;
   col = makecol((lx & 0xFF0000) >> 16, (lx & 0xFF00) >> 8, lx & 0xFF);
   /* draw tooltip block */
   draw_gradient_index(bmp, x, y, w, h, GRADIENTTOOLTIP, focus, grad);

   /* fill in the text   */
   lx = x + 4;            /* find default text location */
   lh = text_height(font)+2;
   ly = y + 4;
   for (cx = 0; cx < lines; cx++)
   {
      textout(bmp, font, text[cx], lx, ly, col);
      ly += lh;
   }
   return 0;
}


/* draw a tooltip at the specified location. It may have several
   lines of text. Use the default gradient ( gradient[] ). */

int draw_default_tooltip(BITMAP *bmp, int x, int y, int w, int h,
     int lines, FONT *font, char *text[])
{
   return draw_gradient_tooltip(bmp, x, y, w, h, lines, font, text, gradient);
}

/* *************** SLIDERS   **************** */

/* lets see how easy it is to draw horiz and vertical sliders:
   Vertical slider:
   Draw on bitmap bmp, at x, y width w height h,
   by = button top y position, bh = button height,
   c1 c2 trench colours, c3 c4 button colours */

int draw_vert_slider(BITMAP *bmp, int x, int y, int w, int h,
    int by, int bh, int c1, int c2, int c3, int c4)
{
   int ey;                       /* end button y coordinate */

   if (bh > (h-2)) bh = h-2;     /* check button fits in trench */
   ey = y + h - 2;
   if (by < (y+1)) by = y+1;
   if ((by+bh) > ey) by = ey-bh-2;
   // draw the trench SUNKEN GRADIENT HORIZONTAL
   draw_colour_gradient(bmp, x, y, w, h, c1, c2, SGH);
   // draw the button RAISED GRADIENT HORIZONTAL+BEVEL2
   draw_colour_gradient(bmp, x+1, by, w-2, bh, c3, c4, RGH+256);
   return 0;
}



/* Horixontal slider:
   Draw on bitmap bmp, at x, y width w height h,
   bx = button left x position, bw = button width,
   c1 c2 trench colours, c3 c4 button colours, in RRGGBB format */

int draw_horiz_slider(BITMAP *bmp, int x, int y, int w, int h,
    int bx, int bw, int c1, int c2, int c3, int c4)
{
   int ex;                         /* button ends at ex */

   if (bw > (w-2)) bw = w-2;       /* button fits inside trench */
   ex = x + w - 2;
   if (bx < (x+1)) bx = x+1;
   if ((bx+bw) > ex) bx = ex-bw-2;
   // draw the trench! SUNKEN GRADIENT VERTICAL
   draw_colour_gradient(bmp, x, y, w, h, c1, c2, SGV);
   // draw the button RAISED GRADIENT VERTICAL BEVEL2
   draw_colour_gradient(bmp, bx, y+1, bw, h-2, c3, c4, RGV+256);
   return 0;
}


/* *************** HELPER FUNCTIONS   **************** */

/*returns a gradient colour in the current pixel depth */

int get_gradient_colour(int idx, int whichcolour, int focus, GRADIENT *grad)
{
   int col;

   col = grad[idx].txtcolour;        /* default is focus text colour */
   if (whichcolour == GRADIENTCOLOUR)     col = grad[idx].colour;
   if (whichcolour == GRADIENTCOLOURTO)   col = grad[idx].colourto;
   if (whichcolour == GRADIENTTEXTSHADOW) col = grad[idx].txtshadow;
   if (whichcolour == GRADIENTCOLOURBI)   col = grad[idx].colourbi;
   if (whichcolour == GRADIENTCOLOURBI2)  col = grad[idx].colourbi2;
   if (focus == UNFOCUSSED)
   {
      col = grad[idx].utxtcolour;
      if (whichcolour == GRADIENTCOLOUR)     col = grad[idx].ucolour;
      if (whichcolour == GRADIENTCOLOURTO)   col = grad[idx].ucolourto;
      if (whichcolour == GRADIENTTEXTSHADOW) col = grad[idx].utxtshadow;
      if (whichcolour == GRADIENTCOLOURBI)   col = grad[idx].ucolourbi;
      if (whichcolour == GRADIENTCOLOURBI2)  col = grad[idx].ucolourbi2;
   }
   if (focus == FOCUSPRESSED)
   {
      col = grad[idx].ptxtcolour;
      if (whichcolour == GRADIENTCOLOUR)     col = grad[idx].pcolour;
      if (whichcolour == GRADIENTCOLOURTO)   col = grad[idx].pcolourto;
      if (whichcolour == GRADIENTTEXTSHADOW) col = grad[idx].ptxtshadow;
      if (whichcolour == GRADIENTCOLOURBI)   col = grad[idx].pcolourbi;
      if (whichcolour == GRADIENTCOLOURBI2)  col = grad[idx].pcolourbi2;
   }
   if (focus == FOCUSDISABLED)
   {
      col = grad[idx].dtxtcolour;
      if (whichcolour == GRADIENTCOLOUR)     col = grad[idx].dcolour;
      if (whichcolour == GRADIENTCOLOURTO)   col = grad[idx].dcolourto;
      if (whichcolour == GRADIENTTEXTSHADOW) col = grad[idx].dtxtshadow;
      if (whichcolour == GRADIENTCOLOURBI)   col = grad[idx].dcolourbi;
      if (whichcolour == GRADIENTCOLOURBI2)  col = grad[idx].dcolourbi2;
   }
   return makecol((col & 0xFF0000) >> 16, (col & 0xFF00) >> 8, col & 0xFF);
}


/*returns a colour scheme colour in the current pixel depth */

int get_scheme_colour(int index, int whichcolour, COLOURSCHEME *sch)
{
   int col;

   col = sch[index].txtcolour;        /* default is text colour */
   if (whichcolour == GRADIENTCOLOUR)     col = sch[index].colour;
   if (whichcolour == GRADIENTCOLOURTO)   col = sch[index].colourto;
   if (whichcolour == GRADIENTTEXTSHADOW) col = sch[index].txtshadow;
   if (whichcolour == GRADIENTCOLOURBI)   col = sch[index].colourbi;
   if (whichcolour == GRADIENTCOLOURBI2)  col = sch[index].colourbi2;

   return makecol((col & 0xFF0000) >> 16, (col & 0xFF00) >> 8, col & 0xFF);
}


/* copy one scheme to another. Returns text colour in curent bitdepth */

int copy_scheme(int src, int dest, COLOURSCHEME *sch)
{
   int col;

   sch[dest].colour   = sch[src].colour;
   sch[dest].colourto = sch[src].colourto;
   sch[dest].txtcolour  = sch[src].txtcolour;
   sch[dest].txtshadow   = sch[src].txtshadow;
   sch[dest].colourbi   = sch[src].colourbi;
   sch[dest].colourbi2 = sch[src].colourbi2;
   col = sch[src].txtcolour;

   return makecol((col & 0xFF0000) >> 16, (col & 0xFF00) >> 8, col & 0xFF);
}


/* copies an external theme (gradient) to the internal gradient[]/scheme[] */

int set_gradient(GRADIENT *my_gradient, int max)
{
   int cx, cs;

   for (cx = 0; cx < max; cx++)
   {
      gradient[cx].bmp = my_gradient[cx].bmp;
      gradient[cx].justify = my_gradient[cx].justify;
      gradient[cx].method = my_gradient[cx].method;
      gradient[cx].colour = my_gradient[cx].colour;
      gradient[cx].colourto = my_gradient[cx].colourto;
      gradient[cx].txtcolour = my_gradient[cx].txtcolour;
      gradient[cx].txtshadow = my_gradient[cx].txtshadow;
      gradient[cx].colourbi  = my_gradient[cx].colourbi;
      gradient[cx].colourbi2 = my_gradient[cx].colourbi2;
      gradient[cx].umethod = my_gradient[cx].umethod;
      gradient[cx].ucolour = my_gradient[cx].ucolour;
      gradient[cx].ucolourto = my_gradient[cx].ucolourto;
      gradient[cx].utxtcolour = my_gradient[cx].utxtcolour;
      gradient[cx].utxtshadow = my_gradient[cx].utxtshadow;
      gradient[cx].ucolourbi  = my_gradient[cx].ucolourbi;
      gradient[cx].ucolourbi2 = my_gradient[cx].ucolourbi2;
      gradient[cx].pmethod = my_gradient[cx].pmethod;
      gradient[cx].pcolour = my_gradient[cx].pcolour;
      gradient[cx].pcolourto = my_gradient[cx].pcolourto;
      gradient[cx].ptxtcolour = my_gradient[cx].ptxtcolour;
      gradient[cx].ptxtshadow = my_gradient[cx].ptxtshadow;
      gradient[cx].pcolourbi  = my_gradient[cx].pcolourbi;
      gradient[cx].pcolourbi2 = my_gradient[cx].pcolourbi2;
      gradient[cx].dmethod = my_gradient[cx].dmethod;
      gradient[cx].dcolour = my_gradient[cx].dcolour;
      gradient[cx].dcolourto = my_gradient[cx].dcolourto;
      gradient[cx].dtxtcolour = my_gradient[cx].dtxtcolour;
      gradient[cx].dtxtshadow = my_gradient[cx].dtxtshadow;
      gradient[cx].dcolourbi  = my_gradient[cx].dcolourbi;
      gradient[cx].dcolourbi2 = my_gradient[cx].dcolourbi2;
      gradient[cx].fcs = my_gradient[cx].fcs;
      gradient[cx].ucs = my_gradient[cx].ucs;
      gradient[cx].pcs = my_gradient[cx].pcs;
      gradient[cx].dcs = my_gradient[cx].dcs;
   }
   numschemes = 0;
   for (cx = 0; cx < 24; cx++)
   {
      cs = my_gradient[cx].dcs;
      if (cs >= MAXCOLOURSCHEMES || cs < 0) cs = MAXCOLOURSCHEMES - 1;
      if (numschemes <= cs) numschemes = cs+1;
      scheme[cs].colour   = my_gradient[cx].dcolour;
      scheme[cs].colourto = my_gradient[cx].dcolourto;
      scheme[cs].txtcolour  = my_gradient[cx].dtxtcolour;
      scheme[cs].txtshadow  = my_gradient[cx].dtxtshadow;
      scheme[cs].colourbi  = my_gradient[cx].dcolourbi;
      scheme[cs].colourbi2 = my_gradient[cx].dcolourbi2;
      cs = my_gradient[cx].fcs;
      if (cs >= MAXCOLOURSCHEMES || cs < 0) cs = MAXCOLOURSCHEMES - 1;
      if (numschemes <= cs) numschemes = cs+1;
      scheme[cs].colour   = my_gradient[cx].colour;
      scheme[cs].colourto = my_gradient[cx].colourto;
      scheme[cs].txtcolour  = my_gradient[cx].txtcolour;
      scheme[cs].txtshadow  = my_gradient[cx].txtshadow;
      scheme[cs].colourbi  = my_gradient[cx].colourbi;
      scheme[cs].colourbi2 = my_gradient[cx].colourbi2;
      cs = my_gradient[cx].ucs;
      if (cs >= MAXCOLOURSCHEMES || cs < 0) cs = MAXCOLOURSCHEMES - 1;
      if (numschemes <= cs) numschemes = cs+1;
      scheme[cs].colour   = my_gradient[cx].ucolour;
      scheme[cs].colourto = my_gradient[cx].ucolourto;
      scheme[cs].txtcolour  = my_gradient[cx].utxtcolour;
      scheme[cs].txtshadow  = my_gradient[cx].utxtshadow;
      scheme[cs].colourbi  = my_gradient[cx].ucolourbi;
      scheme[cs].colourbi2 = my_gradient[cx].ucolourbi2;
      cs = my_gradient[cx].pcs;
      if (cs >= MAXCOLOURSCHEMES || cs < 0) cs = MAXCOLOURSCHEMES - 1;
      if (numschemes <= cs) numschemes = cs+1;
      scheme[cs].colour   = my_gradient[cx].pcolour;
      scheme[cs].colourto = my_gradient[cx].pcolourto;
      scheme[cs].txtcolour  = my_gradient[cx].ptxtcolour;
      scheme[cs].txtshadow  = my_gradient[cx].ptxtshadow;
      scheme[cs].colourbi  = my_gradient[cx].pcolourbi;
      scheme[cs].colourbi2 = my_gradient[cx].pcolourbi2;
   }
   return 0;
}


/* Count how many colour schemes are used in a gradient theme.
      This function records the highest colour scheme actually used */

int count_colour_schemes(GRADIENT *my_gradient)
{
   int cx, cs;

   numschemes = 0;
   for (cx = 0; cx < 24; cx++)
   {
      cs = my_gradient[cx].fcs;
      if (cs >= MAXCOLOURSCHEMES || cs < 0) cs = MAXCOLOURSCHEMES - 1;
      if (numschemes <= cs) numschemes = cs+1;
      cs = my_gradient[cx].ucs;
      if (cs >= MAXCOLOURSCHEMES || cs < 0) cs = MAXCOLOURSCHEMES - 1;
      if (numschemes <= cs) numschemes = cs+1;
      cs = my_gradient[cx].pcs;
      if (cs >= MAXCOLOURSCHEMES || cs < 0) cs = MAXCOLOURSCHEMES - 1;
      if (numschemes <= cs) numschemes = cs+1;
      cs = my_gradient[cx].dcs;
      if (cs >= MAXCOLOURSCHEMES || cs < 0) cs = MAXCOLOURSCHEMES - 1;
      if (numschemes <= cs) numschemes = cs+1;
   }
   return numschemes;
}


/* swaps colour with colourto in the scheme[num] */

int swap_scheme_colours(int num, COLOURSCHEME *sch)
{
   int tmp;

   tmp = sch[num].colour;
   sch[num].colour   = sch[num].colourto;
   sch[num].colourto = tmp;
   return sch[num].colour;
}



int copy_gradient_theme(GRADIENT *gradfrom, GRADIENT *gradto, int max)
{
   int cx;

   for (cx = 0; cx < max; cx++)
   {
      gradto[cx].bmp = gradfrom[cx].bmp;
      gradto[cx].justify = gradfrom[cx].justify;
      gradto[cx].method = gradfrom[cx].method;
      gradto[cx].colour = gradfrom[cx].colour;
      gradto[cx].colourto = gradfrom[cx].colourto;
      gradto[cx].txtcolour = gradfrom[cx].txtcolour;
      gradto[cx].txtshadow = gradfrom[cx].txtshadow;
      gradto[cx].colourbi  = gradfrom[cx].colourbi;
      gradto[cx].colourbi2 = gradfrom[cx].colourbi2;
      gradto[cx].umethod = gradfrom[cx].umethod;
      gradto[cx].ucolour = gradfrom[cx].ucolour;
      gradto[cx].ucolourto = gradfrom[cx].ucolourto;
      gradto[cx].utxtcolour = gradfrom[cx].utxtcolour;
      gradto[cx].utxtshadow = gradfrom[cx].utxtshadow;
      gradto[cx].ucolourbi  = gradfrom[cx].ucolourbi;
      gradto[cx].ucolourbi2 = gradfrom[cx].ucolourbi2;
      gradto[cx].pmethod = gradfrom[cx].pmethod;
      gradto[cx].pcolour = gradfrom[cx].pcolour;
      gradto[cx].pcolourto = gradfrom[cx].pcolourto;
      gradto[cx].ptxtcolour = gradfrom[cx].ptxtcolour;
      gradto[cx].ptxtshadow = gradfrom[cx].ptxtshadow;
      gradto[cx].pcolourbi  = gradfrom[cx].pcolourbi;
      gradto[cx].pcolourbi2 = gradfrom[cx].pcolourbi2;
      gradto[cx].dmethod = gradfrom[cx].dmethod;
      gradto[cx].dcolour = gradfrom[cx].dcolour;
      gradto[cx].dcolourto = gradfrom[cx].dcolourto;
      gradto[cx].dtxtcolour = gradfrom[cx].dtxtcolour;
      gradto[cx].dtxtshadow = gradfrom[cx].dtxtshadow;
      gradto[cx].dcolourbi  = gradfrom[cx].dcolourbi;
      gradto[cx].dcolourbi2 = gradfrom[cx].dcolourbi2;
      gradto[cx].dcs = gradfrom[cx].dcs;
      gradto[cx].fcs = gradfrom[cx].fcs;
      gradto[cx].ucs = gradfrom[cx].ucs;
      gradto[cx].pcs = gradfrom[cx].pcs;
   }
   return 0;
}


void copy_gradient(int gsrc, int gdest, GRADIENT *grad)
{
   grad[gdest].dcs = grad[gsrc].dcs;
   grad[gdest].fcs = grad[gsrc].fcs;
   grad[gdest].ucs = grad[gsrc].ucs;
   grad[gdest].pcs = grad[gsrc].pcs;
   grad[gdest].colour   = grad[gsrc].colour;
   grad[gdest].colourto = grad[gsrc].colourto;
   grad[gdest].txtcolour = grad[gsrc].txtcolour;
   grad[gdest].txtshadow = grad[gsrc].txtshadow;
   grad[gdest].colourbi  = grad[gsrc].colourbi;
   grad[gdest].colourbi2 = grad[gsrc].colourbi2;
   grad[gdest].ucolour   = grad[gsrc].ucolour;
   grad[gdest].ucolourto = grad[gsrc].ucolourto;
   grad[gdest].utxtcolour = grad[gsrc].utxtcolour;
   grad[gdest].utxtshadow = grad[gsrc].utxtshadow;
   grad[gdest].ucolourbi  = grad[gsrc].ucolourbi;
   grad[gdest].ucolourbi2 = grad[gsrc].ucolourbi2;
   grad[gdest].pcolour   = grad[gsrc].pcolour;
   grad[gdest].pcolourto = grad[gsrc].pcolourto;
   grad[gdest].ptxtcolour  = grad[gsrc].ptxtcolour;
   grad[gdest].ptxtshadow = grad[gsrc].ptxtshadow;
   grad[gdest].pcolourbi  = grad[gsrc].pcolourbi;
   grad[gdest].pcolourbi2 = grad[gsrc].pcolourbi2;
   grad[gdest].dcolour   = grad[gsrc].dcolour;
   grad[gdest].dcolourto = grad[gsrc].dcolourto;
   grad[gdest].dtxtcolour  = grad[gsrc].dtxtcolour;
   grad[gdest].dtxtshadow = grad[gsrc].dtxtshadow;
   grad[gdest].dcolourbi  = grad[gsrc].dcolourbi;
   grad[gdest].dcolourbi2 = grad[gsrc].dcolourbi2;
   grad[gdest].method  = grad[gsrc].method;
   grad[gdest].dmethod  = grad[gsrc].dmethod;
   grad[gdest].umethod = grad[gsrc].umethod;
   grad[gdest].pmethod = grad[gsrc].pmethod;
   grad[gdest].justify = grad[gsrc].justify;
   grad[gdest].bmp = grad[gsrc].bmp;
}


void exchange_gradient(int gsrc, int gdest, GRADIENT *grad)
{
   int tmp;
   BITMAP *tmpbmp;

   tmp = grad[gdest].dcs;
   grad[gdest].dcs = grad[gsrc].dcs;
   grad[gsrc].dcs = tmp;
   tmp = grad[gdest].fcs;
   grad[gdest].fcs = grad[gsrc].fcs;
   grad[gsrc].fcs = tmp;
   tmp = grad[gdest].ucs;
   grad[gdest].ucs = grad[gsrc].ucs;
   grad[gsrc].ucs = tmp;
   tmp = grad[gdest].pcs;
   grad[gdest].pcs = grad[gsrc].pcs;
   grad[gsrc].pcs = tmp;
   tmp = grad[gdest].colour;
   grad[gdest].colour   = grad[gsrc].colour;
   grad[gsrc].colour = tmp;
   tmp = grad[gdest].colourto;
   grad[gdest].colourto = grad[gsrc].colourto;
   grad[gsrc].colourto = tmp;
   tmp = grad[gdest].txtcolour;
   grad[gdest].txtcolour  = grad[gsrc].txtcolour;
   grad[gsrc].txtcolour = tmp;
   tmp = grad[gdest].txtshadow;
   grad[gdest].txtshadow  = grad[gsrc].txtshadow;
   grad[gsrc].txtshadow = tmp;
   tmp = grad[gdest].colourbi;
   grad[gdest].colourbi   = grad[gsrc].colourbi;
   grad[gsrc].colourbi = tmp;
   tmp = grad[gdest].colourbi2;
   grad[gdest].colourbi2 = grad[gsrc].colourbi2;
   grad[gsrc].colourbi2 = tmp;
   tmp = grad[gdest].ucolour;
   grad[gdest].ucolour   = grad[gsrc].ucolour;
   grad[gsrc].ucolour = tmp;
   tmp = grad[gdest].ucolourto;
   grad[gdest].ucolourto = grad[gsrc].ucolourto;
   grad[gsrc].ucolourto = tmp;
   tmp = grad[gdest].utxtcolour;
   grad[gdest].utxtcolour  = grad[gsrc].utxtcolour;
   grad[gsrc].utxtcolour = tmp;
   tmp = grad[gdest].utxtshadow;
   grad[gdest].utxtshadow  = grad[gsrc].utxtshadow;
   grad[gsrc].utxtshadow = tmp;
   tmp = grad[gdest].ucolourbi;
   grad[gdest].ucolourbi   = grad[gsrc].ucolourbi;
   grad[gsrc].ucolourbi = tmp;
   tmp = grad[gdest].ucolourbi2;
   grad[gdest].ucolourbi2 = grad[gsrc].ucolourbi2;
   grad[gsrc].ucolourbi2 = tmp;
   tmp = grad[gdest].pcolour;
   grad[gdest].pcolour   = grad[gsrc].pcolour;
   grad[gsrc].pcolour = tmp;
   tmp = grad[gdest].pcolourto;
   grad[gdest].pcolourto = grad[gsrc].pcolourto;
   grad[gsrc].pcolourto = tmp;
   tmp = grad[gdest].ptxtcolour;
   grad[gdest].ptxtcolour  = grad[gsrc].ptxtcolour;
   grad[gsrc].ptxtcolour = tmp;
   tmp = grad[gdest].ptxtshadow;
   grad[gdest].ptxtshadow  = grad[gsrc].ptxtshadow;
   grad[gsrc].ptxtshadow = tmp;
   tmp = grad[gdest].pcolourbi;
   grad[gdest].pcolourbi   = grad[gsrc].pcolourbi;
   grad[gsrc].pcolourbi = tmp;
   tmp = grad[gdest].pcolourbi2;
   grad[gdest].pcolourbi2 = grad[gsrc].pcolourbi2;
   grad[gsrc].pcolourbi2 = tmp;
   tmp = grad[gdest].dcolour;
   grad[gdest].dcolour   = grad[gsrc].dcolour;
   grad[gsrc].dcolour = tmp;
   tmp = grad[gdest].dcolourto;
   grad[gdest].dcolourto = grad[gsrc].dcolourto;
   grad[gsrc].dcolourto = tmp;
   tmp = grad[gdest].dtxtcolour;
   grad[gdest].dtxtcolour  = grad[gsrc].dtxtcolour;
   grad[gsrc].dtxtcolour = tmp;
   tmp = grad[gdest].dtxtshadow;
   grad[gdest].dtxtshadow  = grad[gsrc].dtxtshadow;
   grad[gsrc].dtxtshadow = tmp;
   tmp = grad[gdest].dcolourbi;
   grad[gdest].dcolourbi   = grad[gsrc].dcolourbi;
   grad[gsrc].dcolourbi = tmp;
   tmp = grad[gdest].dcolourbi2;
   grad[gdest].dcolourbi2 = grad[gsrc].dcolourbi2;
   grad[gsrc].dcolourbi2 = tmp;
   tmp = grad[gdest].dmethod;
   grad[gdest].dmethod  = grad[gsrc].dmethod;
   grad[gsrc].dmethod = tmp;
   tmp = grad[gdest].method;
   grad[gdest].method  = grad[gsrc].method;
   grad[gsrc].method = tmp;
   tmp = grad[gdest].umethod;
   grad[gdest].umethod = grad[gsrc].umethod;
   grad[gsrc].umethod = tmp;
   tmp = grad[gdest].pmethod;
   grad[gdest].pmethod = grad[gsrc].pmethod;
   grad[gsrc].pmethod = tmp;
   tmp = grad[gdest].justify;
   grad[gdest].justify = grad[gsrc].justify;
   grad[gsrc].justify = tmp;
   tmpbmp = grad[gdest].bmp;
   grad[gdest].bmp = grad[gsrc].bmp;
   grad[gsrc].bmp = tmpbmp;
}


/* end of code */

