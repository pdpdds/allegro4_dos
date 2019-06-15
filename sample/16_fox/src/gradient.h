/* gradient.h    header for code with gradient fills */

/* A.M.Hugo September 2001
   modified June 2002
      get_gradient_window_size() renamed to get_window_size_ex()
      new function  get_gradient_window_size()
      copy_gradient_list() renamed to copy_gradient_theme()
      default gradient _grad[] renamed to gradient[]
      default borderstyle _bs renamed to borderstyle
      colour3 renamed to txtcolour
      txtshadow colour added to schemes/gradients for shadowed text
      colourbi, colourbi2 added to themes/gradients for bilinear gradients
      disabled gradient added to gradient structures
      shadowed text support added to text justification selection
   modified July 2002
      WINDOWBUTTONS changed to WINDOWBUTTON
      support functions for windowbuttons added
      skin element added to WINDOWBUTTON structure for gradients and skins
      adjusted toolbar layout
      added gradients for barrel, cylindrical, pipecross, elliptical
      added gradients for pyramid, rectangular and bilinear
*/
      


#define  MAXGRADIENTS      24                      /* 42 backwards ?? */
#define  MAXCOLOURSCHEMES  100
#define  MAXCOLOURS        100

/* define gradient-skin elements */
/* QUICK-LAUNCH TOOLBAR */
#define  GRADIENTTOOLBAR         0
#define  GRADIENTTOOLBARLABEL    1
#define  GRADIENTTOOLBARBUTTON   2
#define  GRADIENTTOOLBARWINDOW   3
#define  GRADIENTTOOLBARICON     3   /* alternative name... */
#define  GRADIENTTOOLBARHANDLE   4
/* MENUBAR and MENU FRAME */
#define  GRADIENTMENUBAR         5
#define  GRADIENTMENUTITLE       6
#define  GRADIENTMENUHILITE      7
#define  GRADIENTMENUBULLET      8  /* obsolete - now its menu handle */
#define  GRADIENTMENUFRAME       9
/* WINDOW / FRAME */
#define  GRADIENTWINDOWTITLE     10
#define  GRADIENTWINDOWLABEL     11
#define  GRADIENTWINDOWBUTTON    12
#define  GRADIENTWINDOWFRAME     13
#define  GRADIENTWINDOWHANDLE    14
#define  GRADIENTWINDOWGRIP      15
/* MISC WINDOW OBJECTS */
#define  GRADIENTICONBAR         16
#define  GRADIENTICONBARBUTTON   17
#define  GRADIENTADDRESSBAR      18
#define  GRADIENTTEXTBOX         19
#define  GRADIENTSTATUSBAR       20
/* TOOLTIP, CLOCK and SCRATCHPAD */
#define  GRADIENTTOOLTIP         21
#define  GRADIENTCLOCK           22
#define  GRADIENTCOMMON          23  /* no longer used - scratchpad? */

/* Pick one to use as a generic button, and text entry field  */
#define  GRADIENTBUTTON          12
#define  GRADIENTENTRY           19

#define  GRADIENTMENUHANDLE      8

/* alternative name for Window Handle */
#define  GRADIENTWINDOWSILL      14

/* define gradient drawing flags TLIB CPVH GKSR*/

/* Button EDGE type: raised or sunken - bits 0, 1 */

#define  GRADIENTFLAT         0
#define  GRADIENTRAISED       1
#define  GRADIENTSUNK         2
#define  GRADIENTLIPPED       3

/* DRAW style: none, gradient or skinned - bits 2, 3 */
/* Of these, only GRADIENTTEXTURE applies to this gradient code.
   See SKIN.DOC for the skinning code. (future enhancement) */

#define  GRADIENTNODRAW       0    /* no gradient drawn */
#define  GRADIENTSKINNED      4    /* use skinbmp element (tiled) */
#define  GRADIENTTEXTURE      8    /* gradient as texture (normal) */
#define  GRADIENTMAPPED       12   /* gradient skin mapped by skinbmp  */

/* A gradient that is mapped or masked must be linked to a mask/map array
   which specifies how the gradient is to be drawn. This is a future
   project to provide shaped gradients instead of rectangular ones.
*/
/* GRADIENT type: Horizontal, Vertical, polar etc. - bits 4-7 */
/* Only valid if DRAW style is 8: GRADIENTTEXTURE or 12: GRADIENTMAPPED */

#define  GRADIENTSOLID        0    /* solid colour */
#define  GRADIENTHORIZONTAL   16
#define  GRADIENTVERTICAL     32
#define  GRADIENTDIAGONAL     48
#define  GRADIENTPOLAR        64
#define  GRADIENTCROSSDIAG    80
/* yet to be implemented */
#define  GRADIENTBARREL       96
#define  GRADIENTCYLINDRICAL  112
#define  GRADIENTPIPECROSS    128
#define  GRADIENTPYRAMID      144
#define  GRADIENTRECTANGLE    160
#define  GRADIENTELLIPTICAL   176
/* 208 224 240 are undefined (reserved?) */
#define  GRADIENTBILINEAR     192   // all four corners
#define  GRADIENTTRIVERTICAL  208   // top bot centre BANNER
#define  GRADIENTTRIHORIZON   224   // left right centre TRIFLAG
#define  GRADIENTTRILINEAR    240   // dont know...      QUADFLAG

/* special flags for all draw types - bits 8-11 */

#define  GRADIENTBEVELSTD     0    /* normal bevel if defined raised/sunk */
#define  GRADIENTBEVEL2       256  /* raised/sunk bevels doubled in width */
#define  GRADIENTINTERLACE    512  /* interlace: darken every second line */
#define  GRADIENTRIVETED      1024 /* use dots every four lines */
#define  GRADIENTTRANSPARENT  2048 /* 50% merge with background */

#define  GRADIENTLATTICE      1024 /* obsolete - same as GRADIENTRIVETED */

/* defines for edge drawing code */
#define  SUNKENDRAW      0
#define  RAISEDDRAW      1
#define  SUNKENDRAWTWO   2
#define  RAISEDDRAWTWO   3

/* shortcuts: x = RGVI; instead of
x = GRADIENTTEXTURE | GRADIENTRAISED | GRADIENTVERTICAL | GRADIENTINTERLACED;
*/
#define  RGS    9   // 1 8 0        Raised Gradient Solid
#define  SGS    10  // 2 8 0        Sunken Gradient Solid
#define  RGSI   521 // 1 8 0  512   Raised Gradient Solid Interlaced
#define  SGSI   522 // 2 8 0  512
#define  FGS    8   // 0 8 0        Flat  Gradient  Solid
#define  FGSI   520 // 0 8 0  512
#define  RGV   41   // 1 8 32       Raised Gradient Vertical
#define  SGV   42   // 2 8 32       Sunken
#define  RGH   25   // 1 8 16       Raised Gradient Horizontal
#define  SGH   26   // 2 8 16
#define  RGHI  537  // 1 8 16 512   Raised Gradient Horizontal Interlaced
#define  SGHI  538  // 2 8 16 512
#define  RGVI  553  // 1 8 32 512
#define  SGVI  554  // 2 8 32 512
#define  FGV   40   // 0 8 32       Flat Gradient Vertical
#define  FGH   24   // 0 8 16
#define  FGHI  536  // 0 8 16 512
#define  FGVI  552  // 0 8 32 512
#define  RGD   57   // 1 8 48       Raised Gradient Diagonal
#define  SGD   58   // 2 8 48
#define  RGDI  569  // 1 8 48 512
#define  SGDI  570  // 2 8 48 512
#define  FGD   56   // 0 8 48
#define  FGDI  568  // 0 8 48 512
#define  RGP   73   // 1 8 64       Raised Gradient Polar
#define  SGP   74   // 2 8 64
#define  RGPI  585  // 1 8 64 512
#define  SGPI  586  // 2 8 64 512
#define  FGP   72   // 0 8 64
#define  FGPI  584  // 0 8 64 512
#define  RGC   89   // 1 8 80       Raised Gradient CrossDiagonal
#define  SGC   90   // 2 8 80
#define  RGCI  601  // 1 8 80 512
#define  SGCI  602  // 2 8 80 512
#define  FGC   88   // 0 8 80
#define  FGCI  600  // 0 8 80 512

/* text justification definitions */

#define  JUSTIFYLEFT          0
#define  JUSTIFYCENTER        1
#define  JUSTIFYRIGHT         2
#define  JUSTIFYLEFTSHADOW    8
#define  JUSTIFYCENTERSHADOW  9
#define  JUSTIFYRIGHTSHADOW   10

#define  JUSTIFYSHADOWMASK   0x08

/* dyslexics of the world: UNTIE! */
#define  LEFTJUSTIFY          0
#define  CENTERJUSTIFY        1
#define  RIGHTJUSTIFY         2
#define  LEFTJUSTIFYSHADOW    8
#define  CENTERJUSTIFYSHADOW  9
#define  RIGHTJUSTIFYSHADOW   10


/* focus definitions */
#define  FOCUSDISABLED  0
#define  UNFOCUSSED     1
#define  FOCUSSED       2
#define  FOCUSPRESSED   3
/* alternative spelling */
#define  UNFOCUSED     1
#define  FOCUSED       2

/* bullet types for submenu indicator */

#define  BNONE    0
#define  BTRI     1
#define  BCIRCLE  2
#define  BROUND   3
#define  BSQUARE  4
#define  BDIAMOND 5

/* bullet is a menu check box? (we won't have 98 menus, will we?) */
#define  BCHECKBOXOFF  98
#define  BCHECKBOXON   99

/* which colour? */

#define  GRADIENTCOLOUR      0
#define  GRADIENTCOLOURTO    1
#define  GRADIENTTEXTCOLOUR  2
#define  GRADIENTTEXTSHADOW  3
#define  GRADIENTCOLOURBI    4
#define  GRADIENTCOLOURBI2   5

#define  GRADIENTCOLOUR3     2

#define  GRADIENTINVISIBLE   0
#define  GRADIENTVISIBLE     1

/* windows buttons flag:  0 0000 0000 0000 */
/*                        B LLLL CCCC RRRR */
/* LLLL  number of LHS buttons/icons at pitch=height (0-15)
   RRRR  number of RHS buttons  (0-15)
   CCCC  clock is present, allow CCCC * pitch. (0-15)
   B     buttons/clock are bordered with current bordercolour

   Titlebar/Toolbar Layout:
    -------------------------------
   | LLL icon  label   clock   RRR |
    -------------------------------
   This layout is drawn if the buttons parameter has a value > 0.
   Put it at zero if you wish to draw your own buttons etc.
*/

#define  BUTTONRIGHTMASK  0x000F
#define  BUTTONLEFTMASK   0x0F00
#define  BUTTONCLOCKMASK  0x00F0
#define  BUTTONBORDERFLAG 0x1000

/* COLOURSCHEME structure - we may need to add more colours for other
   gradient types such as bilinear gradients... oops! we just did. */

typedef struct COLOURSCHEME
{
   int colour;          /* first colour */
   int colourto;        /* smooth blend to this colour */
   int txtcolour;       /* pictograph colour or textcolor for labels */
   int txtshadow;       /* text shadow colour (if used) */
   int colourbi;        /* Extra colour for Bilinear or Trilinear */
   int colourbi2;       /* Extra colour for Biliear schemes */
   int method;          /* obsolete - only used in my theme editor */
}COLOURSCHEME;

/* NOTE The old "method" variable may soon be removed! */


extern COLOURSCHEME scheme[MAXCOLOURSCHEMES];

extern int  numschemes;

/* GRADIENT structure. Use one for each of the items we draw */

typedef struct gradient
{
   BITMAP *bmp;         /* optional bitmap to save recalc. else NULL */
   int fcs;             /* focussed colour scheme... */
   int ucs;             /* unfocussed    Only used during */
   int pcs;             /* pressed       initiallizing or editing */
   int dcs;             /* disabled scheme */
   int justify;         /* text justification for labels */
   /* FOCUSED options or "mouse-over" */
   int colour;          /* primary colour */
   int colourto;        /* second colour to merge to */
   int txtcolour;       /* pictograph colour or textcolor for labels */
   int txtshadow;       /* text shadow colour for labels */
   int colourbi;        /* Extra colour for Bilinear or Trilinear */
   int colourbi2;       /* Extra colour for Biliear schemes */
   int method;          /* draw method: raised gradient vertical etc */
   /* UNFOCUSED options: normal appearance if unselected */
   int ucolour;
   int ucolourto;
   int utxtcolour;
   int utxtshadow;      /* text shadow colour for labels */
   int ucolourbi;       /* Extra colour for Bilinear or Trilinear */
   int ucolourbi2;      /* Extra colour for Biliear schemes */
   int umethod;
   /* FOCUSPRESSED options (usually for buttons) */
   int pcolour;
   int pcolourto;
   int ptxtcolour;
   int ptxtshadow;      /* text shadow colour for labels */
   int pcolourbi;       /* Extra colour for Bilinear or Trilinear */
   int pcolourbi2;      /* Extra colour for Biliear schemes */
   int pmethod;
   /* FOCUSDISABLED options: if item is disabled */
   int dcolour;
   int dcolourto;
   int dtxtcolour;
   int dtxtshadow;      /* text shadow colour for labels */
   int dcolourbi;       /* Extra colour for Bilinear or Trilinear */
   int dcolourbi2;      /* Extra colour for Biliear schemes */
   int dmethod;
}GRADIENT;


extern GRADIENT gradient[MAXGRADIENTS];     /* internal default gradient */


/* GRADIENTWINDOW - a structure that defines how the window is built
   with two main parts.
   1. The items defined by a user, such as titlebar menubar etc.
   2  The items filled in by the drawing code to report back to the caller
      where the individual bits are located.
   Some of this will change... */

typedef struct GRADIENTWINDOW
{
   BITMAP *bgbmp;          // saved background
   int x, y, w, h;         // position, size of window
   int title;              // height of title bar in pixels, 0=none
   int menubar;            // normal height of menu bar 0=none
   int iconbar;            // height of icon bar 0=none
   int addrbar;            // height of address bar 0=none
   int statbar;            // height of status bar 0=none
   int handle;             // height of window handle bar/grips 0=none
   int button;             // buttons on title bar (LHS*256+clock*16+RHS)
   int wpaper;             // wallpaper present? 0=no 1=yes (don't fill)
   int icon;               // width (pixels) allowed for left icon bitmap
   int barhandles;         // width of 'handles' on LHS of menu/icon/add bars
   /* the following items are filled in during the window draw routine
      so that the user will know where to put buttons and text etc. */
   int textcol;            // user frame text colour in current colourdepth
   int shadcol;            // user frame text shadow in current colourdepth
   int mb, ib, ab, sb;     // current bar status 0=normal 1=minimized +2=redo
   int wx, wy, ww, wh;     // window title bar location x y w h
   int ckx, cky, ckw, ckh; // clock size (if requested in the title bar)
   int mx, my, mw, mh;     // menubar location
   int ix, iy, iw, ih;     // iconbar location
   int ax, ay, aw, ah;     // address bar location
   int ux, uy, uw, uh;     // users frame location
   int hx, hy, hw, hh;     // lower handle
   int sx, sy, sw, sh;     // status bar location
   int lgx, lgy, lgw, lgh; // left grip
   int rgx, rgy, rgw, rgh; // right grip
   int bx[10], by[10];     // button locations in title bar as drawn
   int bw[10], bh[10];     // or user values if you do your own
} GRADIENTWINDOW;


extern GRADIENTWINDOW gw;    /* default window using default gradients */



/* WINDOWBUTTONS - User controls and buttons, entryfields and sliders */

typedef struct WINDOWBUTTON
{
   int xp, yp, wp, hp;        // prescaled button location (default grid)
   int skin;                  // skin flag 0-23=gradient 24up=skin (future)
   int retkey;                // return key if pressed
   int idx;                   // variable edit[] index this button uses
   int vis;                   // visibility
   int box;                   // type of box: button, entry-box, text etc
   char *label;               // button text
   char *hint;                // help hint text for tooltip
   int x, y, w, h;            // actual button location as drawn
   int focus;                 // focus: focused unfocused pressed disabled
   BITMAP *bmp;               // if stored in external bitmap i.e skin
   int bx, by, bw, bh;        // where in the bitmap is the skin?
   char *data;                // your data or skin mapping data
} WINDOWBUTTON;

/* The 'skin' variable sets the gradient number to be used to draw the
   button if it is between 0 to 23.
   If "skin" flag is 24 or above, the gradient will not be drawn.
   Instead, the bitmap "bmp" will be used to blit the bmp image.
   The bmp bitmap should hold four images of the button, stacked vertically
   in order: Disabled Unfocused Focused and Pressed. Labels will not
   be drawn on skins if the label string is empty.
   The skin flag should set to 24+ ONLY if the skin bmp bitmap exists.
   If the bmp does not exist, gradient 0 will be used.
   Important Hint: set BITMAP *bmp to NULL at program startup.
*/

/* BORDER style  and miscellaneous leftovers */

typedef struct borderstyle
{
   int bordercolour;    /* border colour in hex format: RRGGBB */
   int borderwidth;     /* border around menu window toolbar, 0-2 */
   int bevelwidth;      /* frame bevel inside window, usually 0, 1 or 2 */
   int handlewidth;     /* height of handle bar/grip beneath window, 0-6 */
   int gripwidth;       /* width of the two edge grips on the handle, 20 */
   int bulletstyle;     /* style of menu bullet: none triangle circle... */
   int bulletpos;       /* bullet position: 0 = left else right */
} BORDERSTYLE;


extern BORDERSTYLE borderstyle;    /* default border when using gradient[] */

/* the following globals are set by set_border_style(...) */

extern int framebevel;     /* bevel inside window frame */
extern int borderwidth;    /* width (pixels) of window border */
extern int handlewidth;    /* height of handlebar/grips for resizing */
extern int gripwidth;      /* width of grips on LHS and RHS of handle */
extern int bordercolour;   /* border colour in current pixel depth */
extern int bulletstyle;    /* shape of menu bullet for submenu option */
extern int bulletpos;      /* bullet is left or right? */


extern char authortext[3][64];    /* credit where credit is due */

/* GRADIENTMENU structure for the menu drawing code */

typedef struct gradientmenu
{
   char *label;         /* title of the menu i.e. FILE */
   char *subm[20];      /* submenu options i.e. SAVE LOAD etc. */
   int num;             /* number of options */
   int sel;             /* current selection */
   int hook;            /* hook to? 0=none 1=menubar */
   int pitch;           /* options vertical pitch in pixels */
   int bull[20];        /* bullet for submenu 0=none 1-97=submenu 98=check */
   int retkey[20];      /* return key / function each option */
   int x, y, w, h;      /* total size incl borders, hook=0, frame if hook=1*/
   int fx, fy, fw, fh;  /* frame size inside borders */
} GRADIENTMENU;


/* Quick action TOOLBAR */

typedef struct TOOLBAR
{
   int status;                     /* open or closed? you decide! */
   int tbhigh;                     /* height of toolbar (normal) */
   int handle;                     /* width of handle (0 = none); */
   int leftmargin;                 /* space after  RHS buttons */
   int rightmargin;                /* space before LHS buttons */
   int clockhigh;                  /* height of clock in pixels */
   int clockwide;                  /* width of clock in pixels */
   int lbuttons;                   /* how many LHS buttons */
   int rbuttons;                   /*          RHS         */
   int butwide;                    /* button size */
   int buthigh;
   int icons;                      /* how many icons (toolbar window) */
   int iconhigh;                   /* icon size */
   int iconwide;
   int x[6],y[6],w[6],h[6];            /* toolbar locations etc */
   int bx[10],by[10],bw[10],bh[10];    /* button locations etc */
   int ix[10],iy[10],iw[10],ih[10];    /* icon locations etc */
}TOOLBAR;

/* x[] etc locations are for:
     0=handle 1=toolbar 2=label 3=available space 4=clock 5=user?
   bx[]  0..9 buttons from left to right
   ix[]  0..9 icons Left to Right
*/

extern char *gradient_name[];


/* ********** GRADIENT INITIALIZE AND FILE FUNCTIONS *************** */


/* Initialize the gradient theme system.
   This must be called first to set up the defaults.
   Without it, you'll have black text on black boxes.

   Sets up a nice (I think) ice blue gradient.
   Returns 0 if okay, -1 if there is a problem */

int init_gradient();    /* set up the default gradient */

/* --------------------------------------------------*/


/* GRADIENT - save and load routines for gradient themes */

int save_gradient(char *fname);     /* save internal gradient[] to a file */

/* --------------------------------------------------*/


/* save your own structures to a text file */

int export_gradient(char *fname, GRADIENT *grad,
      COLOURSCHEME *scheme, BORDERSTYLE *bstyle);

/* --------------------------------------------------*/

/* code to read the ASCII text colour-scheme/gradient theme file */

/* read a gradient theme into the default gradient/colourscheme/border
   structures and set the new borderstyle. */

int read_gradient(char *fname);

/* --------------------------------------------------*/


/* read the theme/gradient text file, into the specified structures.
   This does NOT set the new border style. */

int import_gradient(char *fname, GRADIENT *grad,
    COLOURSCHEME *scheme, BORDERSTYLE *bstyle);


/* -------- BORDER FUNCTIONS ------------------------*/

/* set the current border style to the contents of the BORDERSTYLE struct
   by setting a number of globals.
   Returns bordercolour in the current drawing bitdepth */

int set_border_style(BORDERSTYLE *bstyle);


/* --------------------------------------------------*/

/* ********** GRADIENT DRAWING FUNCTIONS *************** */


/* fill a solid box with sanity check: col is in current drawing colordepth */

int draw_gradient_solid(BITMAP *bmp, int x, int y, int w, int h, int col);

/* --------------------------------------------------*/

/* Draw a bevel on an existing gradient. e.g. a button:
   raised bevel: add 20% to each pixel top and left
                 sub 20% from pixels bottom and right
   sunken bevel is the reverse.
   type is RAISEDDRAW SUNKENDRAW RAISEDDRAWTWO or SUNKENDRAWTWO
*/

int draw_gradient_bevel(BITMAP *bmp, int x, int y, int w, int h, int type);


/* -----------  NORMAL (2 COLOUR) GRADIENTS  -----------------*/

/* DRAW A GRADIENT ON A BITMAP */

/* draw a gradient filled object on the given bitmap, at x, y
   width w & height h, colour1 merging to colour2.
   using the specified drawing method.

   This one function does all - 300 lines!

   Is this better than a dozen functions and 1000 lines all up?

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

int draw_colour_gradient(BITMAP *bmp, int x, int y, int w, int h,
          int colour1, int colour2, int method);

/* --------------------------------------------------*/

/* same as above but adds a label with text justification.
   If 'justify' has the shadow bit set it will use a grey shadow.
   Returns the start x coordinate for the text. (helps with editing) */

int draw_colour_gradient_labelled(BITMAP *bmp, int x, int y, int w, int h,
       int colour1, int colour2, int textcolour, int method,
       FONT *font, char *label, int justify);

/* --------------------------------------------------*/

/* Draw a gradient plus label with support for shadowed text.
   Note: The shadow text is only drawn if the justify bit is set. ( > 7 )
   Returns the start x coordinate for the text. (helps with editing) */

int draw_colour_gradient_label_shadowed(BITMAP *bmp, int xd, int yd, int wd, int hd,
       int colour1, int colour2, int textcolour, int shadcolour, int method,
       FONT *font, char *label, int justify);


/* --------- MASKED OR MAPPED CUSTOM GRADIENTS -------- */

/* These routines allow you to load a "texture map" picture and use it
   to define the gradien/colour merge pattern, using the colours
   specified in the gradient list.
   For best results, the greyscale conversion needs to have a wide dynamic
   range: 0-255. Low values will approximate colour1 and high valuues will
   show colour2.
   In the "masked" routines, a greyscale map value of zero is considered
   to indicate skipped pixels, allowing the background to show through.
   Thus odd shapes, embossing and custom gradients are possible.
*/

/* Load a picture file for a texture map and convert to geyscale.

Note: You MUST call load_gradient_texture_map() BEFORE using the
      texture mapped/masked routines.
*/

BITMAP *load_gradient_texture_map(char *fname);


/* --------------------------------------------------*/

int draw_colour_gradient_mapped(BITMAP *bmp, int x, int y, int w, int h,
    int colour1, int colour2, int method, BITMAP *greybmp);

/* Draw a coloured gradient filled object on the given bitmap, at x, y
   width w & height h, colour1 merging to colour2 using the specified
   texture map to determine the final merged colour. The method parameter
   provides info for a beveled edge and transparency.

   If the size is larger than the greyscale bitmap, it will be tiled.
   A value of 0 in the texture map will give colour1.
   A value of 255 in the texture map will give colour2.
   Intermediate values control the degree of colour merge.

   Programmers note: this function does it the hard way, one pixel
   at a time and may be quite slow.
   If you use this with large areas (particularly with a 486) it is
   much beter to draw the gradients to a memory bitmap on startup,
   then blit to the screen when required. This will be faster than
   drawing it pixel-by-pixel every time.
*/

/* --------------------------------------------------*/


int draw_colour_gradient_masked(BITMAP *bmp, int x, int y, int w, int h,
    int colour1, int colour2, int method, BITMAP *greybmp);

/* Draw a coloured gradient filled object on the given bitmap, at x, y
   width w & height h, colour1 merging to colour2 using the specified
   texture map to determine the final merged colour. The method parameter
   provides info for a beveled edge and transparency.

   A value of 0 in the texture map will cause the pixel to be skipped,
   allowing the background to show through unchanged.

   If the size is larger than the greyscale bitmap, it will be tiled.
   A value of 1 in the texture map will give colour1.
   A value of 255 in the texture map will give colour2.
   Intermediate values control the degree of colour merge.
*/

/* --------------------------------------------------*/


int draw_colour_gradient_enhanced(BITMAP *bmp, int x, int y, int w, int h,
    int colour1, int colour2, int method, BITMAP *greybmp);

/* Draw a coloured gradient filled object on the given bitmap, at x, y
   width w & height h, enhancing the contrast between colour1 and colour2
   using the specified texture map 'greymap' to determine the final merged
   colour. Colour1 is the basic colour, and colour2 is added or subtracted
   according to the greymap pixel. The method parameter provides info for
   a beveled edge and transparency.

   A value of 0 in the texture map will cause the pixel to be skipped,
   allowing the background to show through unchanged.

   If the size is larger than the greyscale bitmap, it will be tiled.
   A value of 128 in the texture map will give colour1.
   Values of 1-127 in the texture map will subtract colour2.
   Values of 129-255 in the texture map will add in colour2.
   Intermediate values control the degree of colour merge.
*/

/* --------------------------------------------------*/

/* Draw a masked/mapped item from the default gradient list gradient[index]
   Returns the text colour in the current bit depth. */


int draw_masked_gradient(BITMAP *bmp, int xd, int yd, int wd, int hd,
     int index, int focus, BITMAP *greybmp);

/* Draw a coloured gradient filled object on the given bitmap, at x, y
   width w & height h, with the default gradient[index], using the specified
   texture map to determine the final merged colour. The method parameter
   provides info for a beveled edge and transparency.

   A value of 0 in the texture map will cause the pixel to be skipped,
   allowing the background to show through unchanged.

   If the size is larger than the greyscale bitmap, it will be tiled.
   A value of 1 in the texture map will give colour1.
   A value of 255 in the texture map will give colour2.
   Intermediate values control the degree of colour merge.
*/

/* --------------------------------------------------*/

/* draw a mapped item from the default gradient list gradient[index]
   Returns the text colour in the current bit depth. */


int draw_mapped_gradient(BITMAP *bmp, int xd, int yd, int wd, int hd,
     int index, int focus, BITMAP *greybmp);

/* Draw a coloured gradient filled object on the given bitmap, at x, y
   width w & height h, with the default gradient[index], using the specified
   texture map to determine the final merged colour. The method parameter
   provides info for a beveled edge and transparency.

   If the size is larger than the greyscale bitmap, it will be tiled.
   A value of 0 in the texture map will give colour1.
   A value of 255 in the texture map will give colour2.
   Intermediate values control the degree of colour merge.
*/

/* --------- BILINEAR (4 COLOUR) GRADIENTS -----------*/

/* draw a bilinear gradient filled object on the given bitmap, at x, y
   width w & height h, with colour1 colour2 colour3 and colour4.
   If the method is NOT a bilinear one, it will default to the
   draw_colour_gradient function using the specified drawing method.
   Imagine the colours are on 4 corners  1---2
                                         3---4
   If you use this with large areas (particularly with a 486) it is
   much beter to draw the gradients to a memory bitmap on startup,
   then blit to the screen when required. This will be faster than
   drawing it pixel-by-pixel every time.
*/

int draw_bilinear_gradient(BITMAP *bmp, int x, int y, int w, int h,
          int colour1, int colour2, int colour3, int colour4, int method);

/* --------------------------------------------------*/

/* same as above but adds a label with text justification.
   If 'justify' has the shadow bit set it will use a grey shadow.
   Returns the start x coordinate for the text. (helps with editing) */

int draw_bilinear_gradient_labelled(BITMAP *bmp, int x, int y, int w, int h,
       int colour1, int colour2, int colour3, int colour4,
       int textcolour, int method, FONT *font, char *label, int justify);

/* --------------------------------------------------*/

/* Draw a bilinear gradient plus label with support for shadowed text.
   Note: The shadow text is only drawn if the justify bit is set. ( > 7 )
   Returns the start x coordinate for the text. (helps with editing) */

int draw_bilinear_gradient_label_shadowed(BITMAP *bmp, int xd, int yd,
   int wd, int hd, int colour1, int colour2, int colour3, int colour4,
   int textcolour, int shadcolour, int method,
   FONT *font, char *label, int justify);

/* --------------------------------------------------*/

/* do a label only - the text colour is already in current bitdepth.
   all this does is figure out a margin from the justify parameter.
   Returns the start x coordinate for the text. (helps with editing) */

int draw_gradient_label_only(BITMAP *bmp, int xd, int yd, int wd, int hd,
       int col, FONT *font, char *label, int justify);

/* --------------------------------------------------*/

/* draw an item from the default gradient list   gradient[index]
   Returns the text colour in the current bit depth. */

int draw_gradient(BITMAP *bmp, int xd, int yd, int wd, int hd,
     int index, int focus);

/* --------------------------------------------------*/

/* draw an indexed item from a specified gradient[]
   Returns the text colour in the current bit depth. */

int draw_gradient_index(BITMAP *bmp, int xd, int yd, int wd, int hd,
     int index, int focus, GRADIENT *grad);

/* --------------------------------------------------*/

/* draw a default gradient ( gradient[] ) with a text label.
   Returns the start x coordinate for the text. (helps with editing) */

int draw_gradient_labelled(BITMAP *bmp, int xd, int yd, int wd, int hd,
     int index, int focus, FONT *font, char *label);

/* --------------------------------------------------*/

/* draw a labelled gradient with the specified gradient[]
   Returns the start x coordinate for the text. (helps with editing) */

int draw_gradient_index_labelled(BITMAP *bmp, int xd, int yd, int wd, int hd,
     int index, int focus, FONT *font, char *label, GRADIENT *grad);


/* --------------------------------------------------*/

/* ************ WINDOW / DIALOG BOX ROUTINES ************* */

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
          int icon, int wallpaper, int barhandles);

/* --------------------------------------------------*/

/* Same as above, but sets the components of the specified
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
           int icon, int wallpaper, int barhandles, GRADIENTWINDOW *gwin);

/* --------------------------------------------------*/

/* Get the size and location of the users window frame.
   Uses the default GRADIENTWINDOW gw
   Returns the window text colour.
   Call this AFTER DRAWING THE WINDOW. */

int get_window_frame(int *x, int *y, int *w, int *h);

/* --------------------------------------------------*/

/* Get the size and location of the users window frame.
   Uses the specified GRADIENTWINDOW gwin
   Returns the window frame text colour.
   Call this AFTER DRAWING THE WINDOW. */

int get_gradient_window_frame(int *x, int *y, int *w, int *h,
                              GRADIENTWINDOW *gwin);

/* --------------------------------------------------*/

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
    int handle, GRADIENT *grad);

/* --------------------------------------------------*/

/* Using the default GRADIENTWINDOW gw and gradient[] structures
   Given w & h, the width and height of the desired usable frame,
   find the overall width & height of the window (winw, winh) */

int get_window_size(int w, int h, int *winw, int *winh);

/* --------------------------------------------------*/

/* Using a GRADIENTWINDOW structure and users GRADIENT.
   Given w & h, the width and height of the desired usable frame,
   find the overall width & height of the window (winw, winh) */

int get_gradient_window_size(int w, int h, int *winw, int *winh,
    GRADIENTWINDOW *gwin, GRADIENT *grad);


/* --------------------------------------------------*/

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
    int handle, int button, int icon, FONT *font, char *label);

/* --------------------------------------------------*/

/* Draw a window on the specified bitmap at location x, y and with a
   size w x h, using the default GRADIENTWINDOW gw, and the default
   gradient[].  Be sure to set_default_window_style() first!
   This function returns the user frame text colour. */

int draw_default_window(BITMAP *bmp, int x, int y, int w, int h,
                        int focus, FONT *font, char *label);

/* --------------------------------------------------*/

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
   int focus, FONT *font, char *label, GRADIENTWINDOW *gwin, GRADIENT *grad);

/* --------------------------------------------------*/

/* open a gradient window, saving background.

   Note: the GRADIENTWINDOW structure must already have the overall
   size stored in the structure at win->w and win->h, and the location
   of the window stored at win->x and win->y. This can be done
   manually or following a call to get_window_size(...).
   The background is saved using the win->bgbmp pointer.

   It returns the current text colour */

int open_gradient_window(BITMAP *bmp, int focus, FONT *font, char *name,
       GRADIENTWINDOW *win, GRADIENT *grad);

/* --------------------------------------------------*/

/* redraw a previously opened gradient/window, WITHOUT saving background.
   Use this to redraw with a different focus. */

int redraw_gradient_window(BITMAP *bmp, int focus, FONT *font, char *name,
       GRADIENTWINDOW *win, GRADIENT *grad);

/* --------------------------------------------------*/

/* close an opened gradient/window, restoring background contents */

int close_gradient_window(BITMAP *bmp, GRADIENTWINDOW *win);

/* --------------------------------------------------*/



/* ********* WINDOWBUTTON FUNCTIONS   *********** */

/* These functions use the WINDOWBUTTON structure to define and draw
   on screen buttons using gradient themes, or, "skins" stored in a bitmap.
*/

/* The set_windowbutton_userdata() function stores the user parameters
   xp, yp, wp and hp in the WINDOWBUTTON structure for later use by
   user supplied routines.
   These parameters are NOT used in the drawing routine and can be used
   for whatever you like. I use them in my theme editor to provide scaling
   so that the buttons and controls may be located properly with
   differing screen resolutions. For example, at 640x480, I use a scaling
   grid of 8 pixels per unit (in 800x600 it is 10 pixels) and multiply it
   with the values in xp etc to compute the default value for its location.

   wb      a pointer to the WINDOWBUTTON structure. ie &wb[EXITBOX]
*/

void set_windowbutton_userdata(int xp, int yp, int wp, int hp,
      WINDOWBUTTON *wb);

/* --------------------------------------------------*/


/* This set_windowbutton_data() function fills the WINDOWBUTTON structure
   with values for later use by the drawing routines.

   skin    if 0-23 the draw routine should gradients, 24+ use a "skin".
   retkey  the return key to use if the button is pressed.
   idx     an index into an external user-supplied array that is
           set up for values associated with this button.
   vis     visibility flag: 0 = not drawn, 1 = draw.
   box     the type of box or button this is.
           This can be any value you like, and is supplied for the use
           of your own routines to draw sliders or special controls.
   label   a string to be drawn as a label on the button.
           An empty string is not drawn. If a skin is used, the label
           will be drawn in the current colour depth for a. GRADIENTBUTTON
           using the text colour (with shadow if specified) in the
           gradient loaded at draw time.
   hint    a string to be used in a tooltip or help line.
   wb      a pointer to the WINDOWBUTTON structure. ie &wb[EXITBOX]
*/



void set_windowbutton_data(int skin, int retkey, int idx, int vis,
    int box, char *label, char *hint, WINDOWBUTTON *wb);

/* --------------------------------------------------*/


/* This set_windowbutton_location() function fills the WINDOWBUTTON structure
   with values for later use by the drawing routines.
   x, y    the location on the target bitmap.
   w, h    the size of the button or control.

   wb      a pointer to the WINDOWBUTTON structure. ie &wb[EXITBOX]
*/




void set_windowbutton_location(int x, int y, int w, int h, WINDOWBUTTON *wb);

/* --------------------------------------------------*/


/* The set_windowbutton_skin() function fills the WINDOWBUTTON structure
   with values derived from a "skin" for later use by the drawing routines.

   skinbmp       a bitmap containing appropriate images for use as a "skin".
   x, y, w, h    the location and size of the button images in skinbmp.
                 The location refers to the start of 4 images, stacked
                 vertically in order: Disabled (top), Unfocussed, Focussed
                 and Pressed. One of the four will be blitted to the
                 target bitmap by the drawing routine, depending on
                 the 'focus' option.

   wb      a pointer to the WINDOWBUTTON structure. ie &wb[EXITBOX]
*/

void set_windowbutton_skin(BITMAP *skinbmp, int x, int y, int w, int h,
    WINDOWBUTTON *wb);



/* --------------------------------------------------*/


/* The set_windowbutton_focus() function changes the focus option in
   the WINDOWBUTTON structure with a new value for the drawing routines.

   focus   the desired focus option 0-3 i.e.0=Disabled 2=FOCUSED
   wb      a pointer to the WINDOWBUTTON structure. ie &wb[EXITBOX]
*/

void set_windowbutton_focus(int focus, WINDOWBUTTON *wb);


/* --------------------------------------------------*/



/* The set_windowbutton_visible() function changes the visibility option in
   the WINDOWBUTTON structure with a new value for the drawing routines.

   visible  the desired visibility flag i.e. 0=invisible  1=visible
   wb       a pointer to the WINDOWBUTTON structure. ie &wb[EXITBOX]
*/

void set_windowbutton_visible(int visible, WINDOWBUTTON *wb);

/* --------------------------------------------------*/


/* This function draws the WINDOWBUTTON specified, on the bitmap at the
   supplied location and size using the wb=>skin index of the gradient
   array grad[]. It returns the text colour in the current colourdepth.
   If the skin flag is 0-23, the gradient functions are used.
   If the skin flag is 24++, a "skin" bitmap is used.
   A text label is added if the button has a valid label.

   x, y, w, h   the location and size on the target bitmap.
   focus        the desired focus option i.e. FOCUSSED
   wb           a pointer to the WINDOWBUTTON structure. ie &wb[EXITBOX]
*/


int draw_gradient_windowbutton(BITMAP *bmp, int x, int y, int w, int h,
    int focus, WINDOWBUTTON *wb, GRADIENT *grad);

/* --------------------------------------------------*/


/* This function draws the WINDOWBUTTON specified on the bitmap at the
   supplied location and size using the wb->skin index of the default
   gradient array. It returns the text colour in the current colourdepth.

   x, y, w, h   the location and size on the target bitmap.
   focus        the desired focus option i.e. FOCUSSED
   wb      a pointer to the WINDOWBUTTON structure. ie &wb[EXITBOX]
*/

int draw_default_windowbutton(BITMAP *bmp, int x, int y, int w, int h,
    int focus, WINDOWBUTTON *wb);



/* --------------------------------------------------*/


/* This function draws the WINDOWBUTTON specified on the bitmap at the
   location and size as specified in the WINDOWBUTTON structure
   using the wb->skin index of the default gradient array.
   It returns the text colour in the current colourdepth.


   focus   the desired focus option i.e. FOCUSSED
   wb      a pointer to the WINDOWBUTTON structure. ie &wb[EXITBOX]
*/

int draw_windowbutton(BITMAP *bmp, int focus, WINDOWBUTTON *wb);


/* ************** MENU FUNCTIONS ************** */

/* find the size of a menu frame using the current borderwidth
   As a bonus, it returns the width of the menu label plus an 8+8 margin */

int get_menu_frame_size(int *w, int *h, FONT *font, GRADIENTMENU *mnu);

/* --------------------------------------------------*/

/* find the size of the menu, so that you can save the background.
   We pass the x, y location to store in the menu (saves time later).
   Always call this before drawing a menu so that the sizes can be stored */

int get_gradient_menu_size(int x, int y, FONT *font, GRADIENTMENU *mnu);

/* --------------------------------------------------*/

/* draw menu titleblock and a frame holding menu items, on the bitmap
   at xd, yd given the vertical menu item pitch, number of submenus,
   font and title text, array of submenu names and array of bullet flags.

   This routine uses the default gradient list gradient[] */

int draw_gradient_menu_ex(BITMAP *bmp, int xd, int yd, int pitch, int hilite,
      int num, FONT *font, char *title, char *str[], int *bflag);

/* --------------------------------------------------*/

/* draw menu titleblock and a frame holding menu items, on the bitmap
   at xd, yd given the GRADIENTMENU object mnu, font and a user
   specified gradient list grad[] */

int draw_gradient_menu(BITMAP *bmp, int xd, int yd, FONT *font,
       GRADIENTMENU *mnu, GRADIENT *grad);

/* --------------------------------------------------*/

/* draw menu titleblock and a frame holding menu items, on the bitmap
   at x, y given the GRADIENTMENU object mnu, font and the default
   gradient list gradient[] */

int draw_default_menu(BITMAP *bmp, int x, int y, FONT *font,
       GRADIENTMENU *mnu);

/* --------------------------------------------------*/

/* find the total size of the menu, using explicit parameters, so that
   you can save the background. Returns label width plus 8+8 pix margin */

int get_gradient_menu_size_ex(int *w, int *h, int pitch, int num,
         FONT *font, char *title, char *str[], int *bflag);

/* -----------------------------------------------*/

/* ************* TOOLBAR FUNCTIONS *************** */


/* Draw a basic toolbar for users buttons, label, icons and a clock.
   Toolbars should always drawn FOCUSSED. Individual buttons may be
   redrawn with other focus options later, e.g. if pressed.

   tbar->x[] etc locations are for:
     0=handle 1=toolbar 2=label 3=icon space 4=clock 5... user

     First, using the internal default gradients... */

int draw_default_toolbar(BITMAP *bmp, int x, int y, int w, int h,
     int focus, FONT *font, char *label, TOOLBAR *tbar);

/* --------------------------------------------------*/

/* Next, using your own gradients */

int draw_gradient_toolbar(BITMAP *bmp, int xd, int yd, int wd, int hd,
     int focus, FONT *font, char *label, GRADIENT *grad, TOOLBAR *tbar);

/* --------------------------------------------------*/


/* ************* TOOLTIP FUNCTIONS *************** */

/* find the size of the tooltip frame, so that you can save the background */

int get_tooltip_dim(int lines, FONT *font, char *text[], int *w, int *h);

/* --------------------------------------------------*/

/* draw a tooltip at the specified location. It may have several
   lines of text. Use the specified gradient. */

int draw_gradient_tooltip(BITMAP *bmp, int xd, int yd, int wd, int hd,
     int lines, FONT *font, char *text[], GRADIENT *grad);

/* --------------------------------------------------*/

/* draw a tooltip at the specified location. It may have several
   lines of text. Use the default gradient ( gradient[] ). */

int draw_default_tooltip(BITMAP *bmp, int x, int y, int w, int h,
     int lines, FONT *font, char *text[]);

/* --------------------------------------------------*/


/* ************* SLIDER FUNCTIONS *************** */

/* lets see how easy it is to draw horiz and vertical sliders:
   Vertical slider:
   Draw on bitmap bmp, at x, y width w height h,
   by = button top y position, bh = button height,
   c1 c2 trench colours, c3 c4 button colours */

int draw_vert_slider(BITMAP *bmp, int x, int y, int w, int h,
    int by, int bh, int c1, int c2, int c3, int c4);

/* --------------------------------------------------*/

/* Horixontal slider:
   Draw on bitmap bmp, at x, y width w height h,
   bx = button left x position, bw = button width,
   c1 c2 trench colours, c3 c4 button colours, in RRGGBB format */

int draw_horiz_slider(BITMAP *bmp, int x, int y, int w, int h,
    int bx, int bw, int c1, int c2, int c3, int c4);


/* --------------------------------------------------*/

/* ************* HELPER FUNCTIONS *************** */

/* helper functions - used mostly for my editor: themedit.exe */

/* pick a colour between two colours c1 and c2. The new colour
   will be a ratio from c1 towards c2. colours are in hex 0xRRGGBB.
   The ratio is 0-255. This is used to create a subdued disabled
   colour from the normal colours. */

int get_new_colour(int c1, int c2, int ratio);

/* --------------------------------------------------*/

/* how many colour schemes are used in a gradient theme?
      This function records the highest colour scheme actually used */

int count_colour_schemes(GRADIENT *gradient);

/* --------------------------------------------------*/


int find_colours(COLOURSCHEME *sch);   /* returns count of colours used */

/* --------------------------------------------------*/

/* swaps colour with colourto in the scheme[num] */

int swap_scheme_colours(int num, COLOURSCHEME *sch);

/* --------------------------------------------------*/

/* copy one scheme to another. Returns text colour in curent bitdepth */

int copy_scheme(int src, int dest, COLOURSCHEME *sch);

 /* update all default gradient colours from the default colourschemes */

void update_all_colours();

/* --------------------------------------------------*/

 /* update all colours in the gradient list from its colourschemes */

void update_gradient_colours(GRADIENT *grad, COLOURSCHEME *scheme);

/* --------------------------------------------------*/

/*returns a gradient[idx] colour in the current pixel depth */

int get_gradient_colour(int idx, int whichcolour, int focus, GRADIENT *grad);

/* --------------------------------------------------*/

/*returns a colour scheme[index] colour in the current pixel depth */

int get_scheme_colour(int index, int whichcolour, COLOURSCHEME *sch);


/* --------------------------------------------------*/

/* copies a user defined gradient to the internal one _grad[] up to
   numgrads (must be 24, gradient elements) */

int set_gradient(GRADIENT *grad, int numgrads);


/* --------------------------------------------------*/

/* copy a gradient list (24 gradients) to another */

int copy_gradient_theme(GRADIENT *gradfrom, GRADIENT *gradto, int max);

/* --------------------------------------------------*/

/* copies a single gradient item in a gradient list */

void copy_gradient(int gsrc, int gdest, GRADIENT *grad);

/* --------------------------------------------------*/

/* exchange (swap) two gradient items in a gradient list */

void exchange_gradient(int gsrc, int gdest, GRADIENT *grad);

/* end of code */

