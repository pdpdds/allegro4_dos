/****************************************************************************/
/* JPTUI.H     : JPTUI header file (JeePy Textual User Interface)           */
/*--------------------------------------------------------------------------*/
/* Author      : DELPRAT Jean-Pierre            			                */
/* Created on  : 10-JAN-95				                                    */
/* Modified on : 13-DEC-02 (Michael)                                        */
/*--------------------------------------------------------------------------*/
/* Version     : 4.2                                                        */
/****************************************************************************/

#ifndef _JPTUI_H_
#define _JPTUI_H_

#define __TUIMAKER__




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : VERSION.H                            Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/*****************************************************************************/
/* VERSION                                                                   */
/*---------------------------------------------------------------------------*/
/* JPTUI Version                                                             */
/*---------------------------------------------------------------------------*/
/* Author      : DELPRAT Jean-Pierre                                         */
/*****************************************************************************/


#define JPTUI_VERSION         4
#define JPTUI_SUB_VERSION     0
#define JPTUI_SUB_SUB_VERSION k

#define JPTUI_VERSION_STRING "4.2"
#define JPTUI_DATE_STRING    "13-DEC-2002"

// Don't forget to change JPTUI.DEF and the .DIZ files, Jean-Pierre





/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : SETTINGS.H                           Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* SETTINGS                                                                 */
/*--------------------------------------------------------------------------*/
/* Directives de compilation de la librairie JPTUI                          */
/****************************************************************************/



// Si NDEBUG est dfini, les macros DEBUG ne font rien
// Sinon, elles permettent de faire des tests et d'arrter le programme.

//#define NDEBUG





/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : DEBUG.H                              Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* DEBUG                                                                    */
/*--------------------------------------------------------------------------*/
/* Debugging macros                                                         */
/****************************************************************************/



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                                 FUNCTIONS                              บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

 // Don't use the following functions ! Use the corresponding macros below.

 #ifndef NDEBUG

   void    ProgrammingError(int error_nb, const char *file, int line);

   // These functions display the available memory.

   void    ShowMemory();
   void    HideMemory();
   void    DisplayMemory();

 #endif

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                                  MACROS                                บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

 #ifdef NDEBUG

 #  define NO_DEBUG(instruction)        instruction
 #  define DEBUG(instruction)
 #  define DEBUG_STOP()
 #  define DEBUG_TEST(condition)
 #  define DEBUG_SHOW_MEMORY()
 #  define DEBUG_HIDE_MEMORY()
 #  define DEBUG_DISPLAY_MEMORY()

 #else

 #  define NO_DEBUG(instruction)
 #  define DEBUG(instruction)           instruction
 #  define DEBUG_STOP()                 ProgrammingError(0, __FILE__, __LINE__)
 #  define DEBUG_TEST(condition)        ((condition) ? (void)0 : DEBUG_STOP())
 #  define DEBUG_SHOW_MEMORY()          ShowMemory()
 #  define DEBUG_HIDE_MEMORY()          HideMemory()
 #  define DEBUG_DISPLAY_MEMORY()       DisplayMemory()

 #endif





/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : TYPES.H                              Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* TYPES                                                                    */
/*--------------------------------------------------------------------------*/
/* JPTUI types                                                              */
/*--------------------------------------------------------------------------*/
/* Author      : DELPRAT Jean-Pierre                                        */
/* Created on  : 06-DEC-94                                                  */
/* Modified on : 15-MAR-97                                                  */
/****************************************************************************/


// Current types not defined in C

typedef unsigned char        byte;
typedef unsigned short int   word;
typedef unsigned char        u_char;

// Language

typedef enum
{
  NONE=-1,

  ENGLISH=0,
  FRENCH,
  GERMAN,
  ITALIAN,
  SPANISH,
  DUTCH,
  GREEK
}
TLanguage;

#define FIRST_LANGUAGE  ENGLISH
#define LAST_LANGUAGE   GREEK


// Zone definition

typedef struct { int x1,y1,x2,y2; } TIntZone;

// Scroll bars of an object

typedef enum { SBC_NONE,
	       SBC_HORIZONTAL,
	       SBC_VERTICAL,
	       SBC_BOTH }
	     TScrollbarChoice;


// Shadows of an object

typedef enum {
	       SHC_NONE,
	       SHC_HORIZONTAL,
	       SHC_VERTICAL,
	       SHC_BOTH }
	     TShadowChoice;

// Object types

class                           TApplication;
typedef TApplication            *PApplication;

class                           TCheckBox;
typedef TCheckBox               *PCheckBox;

class				TComboBox;
typedef TComboBox	        *PComboBox;

class                           TDecimalEditZone;
typedef TDecimalEditZone        *PDecimalEditZone;

class                           TEditZone;
typedef TEditZone               *PEditZone;

class                           TIntegerEditZone;
typedef TIntegerEditZone        *PIntegerEditZone;

class                           TFileListBox;
typedef TFileListBox            *PFileListBox;

class                           TFrame;
typedef TFrame                  *PFrame;

class                           TGroup;
typedef TGroup                  *PGroup;

class                           TLabel;
typedef TLabel                  *PLabel;

class                           TList;
typedef TList                   *PList;

class                           TListBox;
typedef TListBox                *PListBox;

class                           TMenu;
typedef TMenu                   *PMenu;

class                           TMenuBar;
typedef TMenuBar                *PMenuBar;

class                           TObject;
typedef TObject                 *PObject;

class                           TPushButton;
typedef TPushButton             *PPushButton;

class                           TRadioGroup;
typedef TRadioGroup             *PRadioGroup;

class                           TRadioButton;
typedef TRadioButton            *PRadioButton;

class                           TScrollBar;
typedef TScrollBar              *PScrollBar;

class                           TSimpleList;
typedef TSimpleList             *PSimpleList;

class                           TTab;
typedef TTab                    *PTab;

class                           TTabGroup;
typedef TTabGroup               *PTabGroup;

class                           TWindow;
typedef TWindow                 *PWindow;

class                           TGauge;
typedef TGauge                  *PGauge;

class                           TTextZone;
typedef TTextZone               *PTextZone;

class 				THelpWindow;
typedef THelpWindow 	        *PHelpWindow;

class 				THelpTextZone;
typedef THelpTextZone           *PHelpTextZone;


// TUIMaker object mirror class declarations

  #ifdef __TUIMAKER__
  class VApplication;
  class VCheckBox;
  class VComboBox;
  class VDecimalEditZone;
  class VEditZone;
  class VGauge;
  class VIntegerEditZone;
  class VFrame;
  class VGroup;
  class VLabel;
  class VList;
  class VListBox;
  class VMenu;
  class VMenuBar;
  class VObject;
  class VPasswordEditZone;
  class VPushButton;
  class VRadioGroup;
  class VRadioButton;
  class VScrollBar;
  class VSimpleList;
  class VTab;
  class VTabGroup;
  class VTextZone;
  class VWindow;
  #endif

//---------------------------------------




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : CONST.H                              Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/*****************************************************************************/
/* CONST                                                                     */
/*---------------------------------------------------------------------------*/
/* JPTUI Constants                                                           */
/*---------------------------------------------------------------------------*/
/* Author      : DELPRAT Jean-Pierre                                         */
/* Created on  : 12/06/94                                                    */
/*****************************************************************************/



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                                 MACROS                                 บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

  #ifndef MIN
  #define MIN(a,b)    (((a) <= (b)) ? (a) : (b))
  #endif

  #ifndef MAX
  #define MAX(a,b)    (((a) >= (b)) ? (a) : (b))
  #endif

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                                CONSTANTS                               บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

// Attributes of the different objects of the user interface

#define DISABLED                     false
#define ENABLED      		     true

#define NOT_CHECKED 		     false
#define CHECKED     		     true

#define NOT_SORTED                   false
#define SORTED                       true

#define NOT_MODAL                    false
#define MODAL                        true

#define NO_INFO_BAR		     false
#define INFO_BAR                     true

#define NOT_MOVABLE                  false
#define MOVABLE                      true

#define STRING_NEEDNT_TO_BE_IN_LIST  false
#define STRING_MUST_BE_IN_LIST       true

// List items attributes
// (you can define several attributes at the same time
//  -> LI_DISABLED|LI_CHECKED...)

#define LI_NOATTR                    (0x0000)
#define LI_DISABLED                  (0x0001)
#define LI_CHECKED                   (0x0002)
#define LI_TOGGLE                    (0x0004)

// Object Type Definitions

#define OBJ_NONE           -1

#define OBJ_CHECK_BOX       0
#define OBJ_COMBO_LIST      1
#define OBJ_EDIT_ZONE       2
#define OBJ_FRAME_GROUP     3
#define OBJ_GAUGE           4
#define OBJ_LABEL           5
#define OBJ_LIST_BOX        6
#define OBJ_MENU            7
#define OBJ_MENU_BAR        8
#define OBJ_PUSH_BUTTON     9
#define OBJ_SCROLL_BAR     10
#define OBJ_TAB            11
#define OBJ_TAB_GROUP      12
#define OBJ_TEXT_ZONE      13
#define OBJ_WINDOW         14




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : SHORTCUT.H                           Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* SHORTCUT                                                                 */
/*--------------------------------------------------------------------------*/
/* Raccourcis clavier                                                       */
/****************************************************************************/



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                                CONSTANTES                              บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

#define SC_NONE		    0

#define SC_CTRL_A           CTRL_A
#define SC_CTRL_B           CTRL_B
#define SC_CTRL_C           CTRL_C
#define SC_CTRL_D           CTRL_D
#define SC_CTRL_E           CTRL_E
#define SC_CTRL_F           CTRL_F
#define SC_CTRL_G           CTRL_G
#define SC_CTRL_H           CTRL_H
#define SC_CTRL_I           CTRL_I
#define SC_CTRL_J           CTRL_J
#define SC_CTRL_K           CTRL_K
#define SC_CTRL_L           CTRL_L
#define SC_CTRL_M           CTRL_M
#define SC_CTRL_N           CTRL_N
#define SC_CTRL_O           CTRL_O
#define SC_CTRL_P           CTRL_P
#define SC_CTRL_Q           CTRL_Q
#define SC_CTRL_R           CTRL_R
#define SC_CTRL_S           CTRL_S
#define SC_CTRL_T           CTRL_T
#define SC_CTRL_U           CTRL_U
#define SC_CTRL_V           CTRL_V
#define SC_CTRL_W           CTRL_W
#define SC_CTRL_X           CTRL_X
#define SC_CTRL_Y           CTRL_Y
#define SC_CTRL_Z           CTRL_Z

#define SC_ALT_A            ALT_A
#define SC_ALT_B            ALT_B
#define SC_ALT_C            ALT_C
#define SC_ALT_D            ALT_D
#define SC_ALT_E            ALT_E
#define SC_ALT_F            ALT_F
#define SC_ALT_G            ALT_G
#define SC_ALT_H            ALT_H
#define SC_ALT_I            ALT_I
#define SC_ALT_J            ALT_J
#define SC_ALT_K            ALT_K
#define SC_ALT_L            ALT_L
#define SC_ALT_M            ALT_M
#define SC_ALT_N            ALT_N
#define SC_ALT_O            ALT_O
#define SC_ALT_P            ALT_P
#define SC_ALT_Q            ALT_Q
#define SC_ALT_R            ALT_R
#define SC_ALT_S            ALT_S
#define SC_ALT_T            ALT_T
#define SC_ALT_U            ALT_U
#define SC_ALT_V            ALT_V
#define SC_ALT_W            ALT_W
#define SC_ALT_X            ALT_X
#define SC_ALT_Y            ALT_Y
#define SC_ALT_Z            ALT_Z

#define SC_ALT_1            ALT_1
#define SC_ALT_2            ALT_2
#define SC_ALT_3            ALT_3
#define SC_ALT_4            ALT_4
#define SC_ALT_5            ALT_5
#define SC_ALT_6            ALT_6
#define SC_ALT_7            ALT_7
#define SC_ALT_8            ALT_8
#define SC_ALT_9            ALT_9
#define SC_ALT_0            ALT_0

#define SC_F1               F1
#define SC_F2               F2
#define SC_F3               F3
#define SC_F4               F4
#define SC_F5               F5
#define SC_F6               F6
#define SC_F7               F7
#define SC_F8               F8
#define SC_F9               F9
#define SC_F10              F10
#define SC_F11              F11
#define SC_F12              F12

#define SC_CTRL_F1          CTRL_F1
#define SC_CTRL_F2          CTRL_F2
#define SC_CTRL_F3          CTRL_F3
#define SC_CTRL_F4          CTRL_F4
#define SC_CTRL_F5          CTRL_F5
#define SC_CTRL_F6          CTRL_F6
#define SC_CTRL_F7          CTRL_F7
#define SC_CTRL_F8          CTRL_F8
#define SC_CTRL_F9          CTRL_F9
#define SC_CTRL_F10         CTRL_F10
#define SC_CTRL_F11         CTRL_F11
#define SC_CTRL_F12         CTRL_F12

#define SC_SHIFT_F1         SHIFT_F1
#define SC_SHIFT_F2         SHIFT_F2
#define SC_SHIFT_F3         SHIFT_F3
#define SC_SHIFT_F4         SHIFT_F4
#define SC_SHIFT_F5         SHIFT_F5
#define SC_SHIFT_F6         SHIFT_F6
#define SC_SHIFT_F7         SHIFT_F7
#define SC_SHIFT_F8         SHIFT_F8
#define SC_SHIFT_F9         SHIFT_F9
#define SC_SHIFT_F10        SHIFT_F10
#define SC_SHIFT_F11        SHIFT_F11
#define SC_SHIFT_F12        SHIFT_F12

#define SC_ALT_F1    	    ALT_F1
#define SC_ALT_F2           ALT_F2
#define SC_ALT_F3           ALT_F3
#define SC_ALT_F4           ALT_F4
#define SC_ALT_F5           ALT_F5
#define SC_ALT_F6           ALT_F6
#define SC_ALT_F7           ALT_F7
#define SC_ALT_F8           ALT_F8
#define SC_ALT_F9           ALT_F9
#define SC_ALT_F10          ALT_F10
#define SC_ALT_F11          ALT_F11
#define SC_ALT_F12          ALT_F12

#define SC_HOME		    HOME
#define SC_PGUP             PGUP
#define SC_END              END
#define SC_PGDN             PGDN
#define SC_BACKSPACE        BACKSPACE
#define SC_INSERT           INSERT
#define SC_DELETE           DELETE

#define SC_CTRL_HOME	    CTRL_HOME
#define SC_CTRL_PGUP        CTRL_PGUP
#define SC_CTRL_END         CTRL_END
#define SC_CTRL_PGDN        CTRL_PGDN
#define SC_CTRL_BACKSPACE   CTRL_BACKSPACE
#define SC_CTRL_INSERT      CTRL_INSERT
#define SC_CTRL_DELETE      CTRL_DELETE

#define SC_CTRL_UP          CTRL_UP
#define SC_CTRL_DOWN        CTRL_DOWN
#define SC_CTRL_LEFT        CTRL_LEFT
#define SC_CTRL_RIGHT       CTRL_RIGHT

#define SC_SHIFT_INSERT     SHIFT_INSERT
#define SC_SHIFT_DELETE     SHIFT_DELETE

#define SC_ALT_HOME         ALT_HOME
#define SC_ALT_END          ALT_END
#define SC_ALT_BACKSPACE    ALT_BACKSPACE
#define SC_ALT_DELETE       ALT_DELETE

#define SC_CTRL_SHIFT_UP    CTRL_SHIFT_UP
#define SC_CTRL_SHIFT_DOWN  CTRL_SHIFT_DOWN
#define SC_CTRL_SHIFT_LEFT  CTRL_SHIFT_LEFT
#define SC_CTRL_SHIFT_RIGHT CTRL_SHIFT_RIGHT


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                                FONCTIONS                               บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

char *GetShortCutLabel(int short_cut);
int  GetShortCutLabelLength(int short_cut);





/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : CURSOR.H                             Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* CURSOR                                                                   */
/*--------------------------------------------------------------------------*/
/* Text cursor functions   						    */
/****************************************************************************/



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                                 CONSTANTS                              บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

// Cursor style
// (style NO_CURSOR doesn't exist -> use HideTextCursor instead)

#define RECOVER_CURSOR 1
#define INSERT_CURSOR  2

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                                 FUNCTIONS                              บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

// Sets/Gets the cursor style

void    SetTextCursorStyle(int cursor_style);
int     GetTextCursorStyle();

// Hides/Shows the text cursor

void HideTextCursor();
void ShowTextCursor();

// Sets the cursor position with/without changing the cursor style
void SetTextCursorAt(int x,int y);
void SetTextCursorAt(int x,int y,int cursor_style);

bool TextCursorIsVisible();
int     GetXTextCursor();
int     GetYTextCursor();




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : ERRORS.H                             Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* ERRORS                                                                   */
/*--------------------------------------------------------------------------*/
/* Error functions							    */
/****************************************************************************/


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                               FUNCTIONS                                บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

// Aborts the program without displaying any message

void AbortProgram();

// Erreur anormale -> Quitte brutalement l'application (abort)

void ProgrammingError(int error_nb, const char *file, int line);

// Erreur fatale -> Quitte l'application (abort)

void FatalError(const char *message);

// Messages en mode texte (ferme l'cran "graphique" si besoin)

void TextError(const char *message);




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : FILES.H                              Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* FILES                                                                    */
/*--------------------------------------------------------------------------*/
/* File functions							    */
/****************************************************************************/



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                                CONSTANTS                              บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

// These constants define the maximal length of the buffers needed to
// hold the differents components of a file name (including the ending \0).

  #ifdef __TCPLUSPLUS__

  #define MAX_PATH   80    // X:\DIR\SUBDIR\FILE.EXT
  #define MAX_DRIVE  3     // X:
  #define MAX_DIR    66    // \DIR\SUBDIR
  #define MAX_FILE   13    // FILE.EXT

  #else

  #define MAX_PATH   261
  #define MAX_DRIVE  3
  #define MAX_DIR    260
  #define MAX_FILE   255

  #endif

// Returns true if the file 'file_name' exists

bool FileExists(const char *file_name);

// Returns the current directory of the current drive

char *GetCurrentDir();

// Returns the full path of a file or directory
// (or the root of the current drive in case of failure)
// full_path must be MAX_PATH bytes length (including /0)
// If a drive is indicated, this drive must exist
// Returns a bool which indicates success or failure

bool FullPath(char *full_path, const char *path);

/* Returns the full path of a file located in the directory of your .exe    */
/* file									    */
/* full_path must be MAX_PATH bytes length (including /0)                   */
/* file_name must be a file_name without directory information              */
/* (if file_name have some directory information, this file name is         */
/* returned unchanged)							    */

void FullPathOfFileInExeDir(char *full_path, const char *file_name);

// Checks whether a drive exists or not
// 0='A', 1='B' ...

bool DriveExists(int drive);




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : KEYBOARD.H                           Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* KEYBOARD                                                                 */
/*--------------------------------------------------------------------------*/
/* Fonctions de manipulation du clavier                                     */
/****************************************************************************/



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                                CONSTANTES                              บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/


// Codes des caractres non tendus

#define BACKSPACE	    8
#define TAB      	    9
#define RETURN   	   13


#define CTRL_A	            1
#define CTRL_B	 	    2
#define CTRL_C              3
#define CTRL_D              4
#define CTRL_E     	    5
#define CTRL_F              6
#define CTRL_G              7
#define CTRL_H              8
#define CTRL_I              9
#define CTRL_J             10
#define CTRL_K             11
#define CTRL_L             12
#define CTRL_M             13
#define CTRL_N             14
#define CTRL_O             15
#define CTRL_P             16
#define CTRL_Q             17
#define CTRL_R             18
#define CTRL_S             19
#define CTRL_T             20
#define CTRL_U             21
#define CTRL_V             22
#define CTRL_W             23
#define CTRL_X             24
#define CTRL_Y             25
#define CTRL_Z             26
#define ESC	           27

#define SPACE              32

#define CTRL_BACKSPACE    127

#define F1               4155
#define F2               4156
#define F3               4157
#define F4               4158
#define F5               4159
#define F6               4160
#define F7               4161
#define F8               4162
#define F9               4163
#define F10              4164

#define HOME             4167
#define UP               4168
#define PGUP             4169

#define LEFT             4171

#define RIGHT            4173

#define END              4175
#define DOWN             4176
#define PGDN             4177
#define INSERT           4178
#define DELETE           4179

#define F11              4229
#define F12              4230

#define SHIFT_TAB        4879

#define SHIFT_HOME       4935
#define SHIFT_UP         4936
#define SHIFT_PGUP       4937

#define SHIFT_LEFT       4939

#define SHIFT_RIGHT      4941

#define SHIFT_END        4943
#define SHIFT_DOWN       4944
#define SHIFT_PGDN       4945
#define SHIFT_INSERT     4946
#define SHIFT_DELETE     4947
#define SHIFT_F1         4948
#define SHIFT_F2         4949
#define SHIFT_F3         4950
#define SHIFT_F4         4951
#define SHIFT_F5         4952
#define SHIFT_F6         4953
#define SHIFT_F7         4954
#define SHIFT_F8         4955
#define SHIFT_F9         4956
#define SHIFT_F10        4957

#define SHIFT_F11        4999
#define SHIFT_F12        5000

#define CTRL_F1          5214
#define CTRL_F2          5215
#define CTRL_F3          5216
#define CTRL_F4          5217
#define CTRL_F5          5218
#define CTRL_F6          5219
#define CTRL_F7          5220
#define CTRL_F8          5221
#define CTRL_F9          5222
#define CTRL_F10         5223

#define CTRL_LEFT        5235
#define CTRL_RIGHT       5236
#define CTRL_END         5237
#define CTRL_PGDN        5238
#define CTRL_HOME        5239

#define CTRL_PGUP        5252

#define CTRL_F11         5257
#define CTRL_F12         5258

#define CTRL_UP          5261

#define CTRL_DOWN        5265
#define CTRL_INSERT      5266
#define CTRL_DELETE      5267

#define CTRL_SHIFT_LEFT  6003
#define CTRL_SHIFT_RIGHT 6004
#define CTRL_SHIFT_END   6005
#define CTRL_SHIFT_PGDN  6006
#define CTRL_SHIFT_HOME  6007

#define CTRL_SHIFT_PGUP  6020

#define CTRL_SHIFT_UP    6029

#define CTRL_SHIFT_DOWN  6033

#define ALT_BACKSPACE    6158

#define ALT_Q            6160
#define ALT_W            6161
#define ALT_E            6162
#define ALT_R            6163
#define ALT_T            6164
#define ALT_Y            6165
#define ALT_U            6166
#define ALT_I            6167
#define ALT_O            6168
#define ALT_P            6169
#define ALT_A            6174
#define ALT_S            6175
#define ALT_D            6176
#define ALT_F            6177
#define ALT_G            6178
#define ALT_H            6179
#define ALT_J            6180
#define ALT_K            6181
#define ALT_L            6182
#define ALT_Z            6188
#define ALT_X            6189
#define ALT_C            6190
#define ALT_V            6191
#define ALT_B            6192
#define ALT_N            6193
#define ALT_M            6194


#define ALT_F1           6248
#define ALT_F2           6249
#define ALT_F3           6250
#define ALT_F4           6251
#define ALT_F5           6252
#define ALT_F6           6253
#define ALT_F7           6254
#define ALT_F8           6255
#define ALT_F9           6256
#define ALT_F10          6257

#define ALT_1            6264
#define ALT_2            6265
#define ALT_3            6266
#define ALT_4            6267
#define ALT_5            6268
#define ALT_6            6269
#define ALT_7            6270
#define ALT_8            6271
#define ALT_9            6272
#define ALT_0            6273

#define ALT_F11          6283
#define ALT_F12          6284

#define ALT_HOME         6295
#define ALT_UP           6296
#define ALT_PGUP         6297

#define ALT_LEFT         6299

#define ALT_RIGHT        6301

#define ALT_END          6303
#define ALT_DOWN         6304
#define ALT_PGDN         6305

#define ALT_DELETE       6307


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                                  TYPES                                 บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

typedef struct {
  int       character;
  u_char    hot_character;  // Caractre majuscule si non extended
			    // ou caractre accompagn de ALT
} TKey;

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                                FONCTIONS                               บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

// Vide le buffer clavier

void ResetKeyboard();

// Indicates whether a key has been pressed

bool KeyPressed();

// Attend la frappe d'une touche. Retourne la touche frappe

void GetKey(TKey &key);

// Proprits d'une touche

bool IsPrintableKey(TKey key);   // 32 … 255
bool IsExtendedKey(TKey key);    // >255
bool IsAltKey(TKey key);         // ALT+...

// Obtient/Teste le caractre dit NULL

TKey GetNullKey();
bool IsNullKey(TKey key);

// Attend le relchement de la touche actuellement presse

void WaitKeyRelease();

// Attend un clic du bouton gauche de la souris ou l'appui sur une touche
// Retourne le caractre nul si aucun caractre n'a t frapp
// ou le caractre frapp sinon (cf KEYBOARD.H)

TKey WaitLeftClickOrKeyPressed();




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : JPDATA.H                             Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* JPDATA                                                                   */
/*--------------------------------------------------------------------------*/
/* JPTUI Data Access Functions                                              */
/****************************************************************************/


// Allocates a buffer and loads a data file into this buffer.
// This buffer must be freed by delete [].
// Returns a pointer to the buffer or NULL if an error occurs
// (no data file or incorrect data file).
// file_name must be at most 12 characters length

void LoadDataFile(const char *file_name, char *&buffer, long& size);

// Returns the message "Data file is invalid or missing"
// in the current language

const char *GetInvalidDataFileMessage();




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : MOUSE.H                              Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* MOUSE                                                                    */
/*--------------------------------------------------------------------------*/
/* Fonctions de manipulation de la souris                                   */
/****************************************************************************/



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                                CONSTANTES                              บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

// Etat des boutons de la souris

#define NO_BUTTON_PRESSED     0
#define LEFT_BUTTON_PRESSED   1
#define RIGHT_BUTTON_PRESSED  2

// Dsignation des boutons

#define LEFT_BUTTON       0
#define RIGHT_BUTTON      1

// Dure autorise pour faire un double click (en ms)

#define MAX_DOUBLE_CLICK_DURATION 200

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                                  TYPES                                 บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

// Mouse pointers

typedef enum {
	       MP_ARROW=0,
	       MP_HOURGLASS=1} TMousePointer;

// Mouse pointer aspect (displayed by JPTUI in a graphical or textual way, or
// displayed by the mouse driver)

typedef enum {
	       MPA_GRAPHICAL,
	       MPA_TEXTUAL,
	       MPA_MOUSE_DRIVER } TMousePointerAspect;

// 1er des caractres utiliss par le curseur graphique
// Fix: moved to header (used in ToScreenChar())
#define GMOUSE_1ST_CHAR 208

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                                 FONCTIONS                              บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

bool InitMouse();
void 	CloseMouse();
void    ShowMouse();
void    HideMouse();
void    GetMouseState(int &x,int &y,int &button_state);

// Changes the mouse pointer type to `cursor'.
// (useful only with a graphical pointer)

void    SetMousePointer(TMousePointer pointer);

// Returns the current mouse pointer type

TMousePointer GetMousePointer();

// Sets the aspect of the mouse pointer (graphical, textual, mouse driver...)

void SetMousePointerAspect(TMousePointerAspect aspect);

// Waits for the left button of the mouse to be released

void    WaitMouseLeftButtonRelease();




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : SCREEN.H                             Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* SCREEN                                                                   */
/*--------------------------------------------------------------------------*/
/* Screen functions							    */
/****************************************************************************/




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                                   TYPES                                บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

// Text mode

typedef enum {TEXTMODE_USER,

	      TEXTMODE_80x25_8,
	      TEXTMODE_80x25_9,
	      TEXTMODE_80x28_8,
	      TEXTMODE_80x28_9,
	      TEXTMODE_80x50_8,
	      TEXTMODE_80x50_9}
	      TTextMode;

// Font

typedef enum {FONT_STANDARD=0,
	      FONT_JPTUI}
	      TFont;


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                                CONSTANTES                              บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

// Background color

#define SCREEN_BACKGROUND      ((DARKGRAY<<4)+(unsigned)BLACK)

// Special graphical characters used by JPTUI

#define CHAR_UP_LEFT_CORNER      'ุ'  // 216
#define TEXT_UP_LEFT_CORNER      "ุ"  // 216

#define CHAR_UP_HLINE            'ร'  // 195
#define TEXT_UP_HLINE            "ร"  // 195

#define CHAR_UP_RIGHT_CORNER     'ม'  // 193
#define TEXT_UP_RIGHT_CORNER     "ม"  // 193

#define CHAR_MIDDLE_RIGHT_CORNER 'ู'  // 217
#define TEXT_MIDDLE_RIGHT_CORNER "ู"  // 217

#define CHAR_LEFT_VLINE          'ด'  // 180
#define TEXT_LEFT_VLINE          "ด"  // 180

#define CHAR_RIGHT_VLINE         'ย'  // 194
#define TEXT_RIGHT_VLINE         "ย"  // 194

#define CHAR_BOTTOM_LEFT_CORNER  'ภ'  // 192
#define TEXT_BOTTOM_LEFT_CORNER  "ภ"  // 192

#define CHAR_BOTTOM_HLINE        'ล'  // 197
#define TEXT_BOTTOM_HLINE        "ล"  // 197

#define CHAR_BOTTOM_RIGHT_CORNER 'ิ'  // 212
#define TEXT_BOTTOM_RIGHT_CORNER "ิ"  // 212

#define CHAR_TICK		 ''  // 158
#define TEXT_TICK		 ""

#define TEXT_ARROW_UP            "ศษ"
#define TEXT_ARROW_DOWN          "สห"
#define TEXT_ARROW_LEFT          "ฮฯ"
#define TEXT_ARROW_RIGHT         "ฬอ"

#define TEXT_CLOSE_BUTTON        "ฦว"

#define TEXT_CHECKED_RADIO_BUTTON    "ึถ "
#define TEXT_UNCHECKED_RADIO_BUTTON  "ีต "

#define TEXT_CHECKED_CHECK_BOX       "Ýฝ "
#define TEXT_UNCHECKED_CHECK_BOX     "Þพ "

#define CHAR_UP_BOTTOM_HLINES    'ื'  // 215
#define TEXT_UP_BOTTOM_HLINES    "ื"  // 215

#define CHAR_ALT_UP_LEFT_CORNER  'ฺ'  // 218

#define ADDON_CHAR_UP_LEFT_CORNER '๒'  // looks as ฺ 218 - really 242
#define ADDON_TEXT_UP_LEFT_CORNER "๒" 

#define ADDON_CHAR_UP_HLINE      '๓'  // looks as ร 195 - really 243
#define ADDON_TEXT_UP_HLINE      "๓" 

#define ADDON_CHAR_BOTTOM_LEFT_CORNER	'๔'  // looks like ภ 192 - really 244 
#define ADDON_TEXT_BOTTOM_LEFT_CORNER   "๔"
                                                              
#define ADDON_CHAR_MIDDLE_UP_DOUBLE_LINE  'ฦ'  // looks like ะ D0 - really 198
#define ADDON_TEXT_MIDDLE_UP_DOUBLE_LINE  "ฦ"

#define ADDON_CHAR_MIDDLE_UP_LINE  'ว'  // looks like ม C1 - really 199
#define ADDON_TEXT_MIDDLE_UP_LINE  "ว"

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                                 FUNCTIONS                              บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

extern int ToScreenChar(int ch);
// Fix: converts all redefined font symbols to '.'

// Waits until the video beams enters retrace mode

void    WaitRetrace();

// Set/Get character definition

void    GetSetCharDefinition(bool set,int nb_chars,char first_char,char *char_definition);

#define DefineChars(nb_chars,first_char,char_definition) GetSetCharDefinition(true,nb_chars,first_char,char_definition)
#define GetCharDefinition(nb_chars,first_char,char_definition) GetSetCharDefinition(false,nb_chars,first_char,char_definition)

// Sets/Returns the text mode used by JPTUI

void      SetTextMode(TTextMode text_mode);
void      SetUserTextMode(void (*set_mode_function)());
TTextMode GetTextMode();

// Returns the size of the screen in characters

int     GetScreenHeight();
int     GetScreenWidth();

// Sets/Returns the font used by JPTUI (standard or not)

void 	  SetFont(TFont font);
TFont	  GetFont();

// Returns the size of the font

int     GetFontHeight();
int     GetFontWidth();

// Initializes and closes the screen

void    InitScreen();
void    CloseScreen();
bool ScreenInitialized();

// - HideScreen temporarily disables the JPTUI screen.
//   Useful to run a DOS program/shell and to switch to graphical mode
// - UnhideScreen redisplays the JPTUI screen after a call to HideScreen()
//
void    HideScreen();
void    UnhideScreen();

// Clears the entire video screen/a rectangular area of the screen
// using character and attribute for the fill character

void    ClearScreen(char attribute,char character);
void    ClearPartOfScreen(int x1,int y1,int x2,int y2,
			  char attribute,char character);

// Copies a rectangular area of a buffer to the screen at (x,y)

void    PartOfBufferToScreen(int x,int y,
			     word *buffer,
			     int x1_part,int y1_part,
			     int part_width,int part_height,
			     int buffer_width);

// Draws a horizontal/vertical shadow

void    HorizontalShadow(int x1,int x2,int y);
void    VerticalShadow(int x,int y1,int y2);




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : SOUND.H                              Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* SOUND                                                                    */
/*--------------------------------------------------------------------------*/
/* Sound routines	                                                    */
/****************************************************************************/


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                                CONSTANTS                               บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

// Sound definitions

// If you want more sounds, define them with the sound() and delay()
// functions and send me their definition. I will add them to JPTUI.

typedef enum
{
  NO_SOUND=-1,
  ERROR_SOUND,

  BIP1,
  SOUND_UP1,
  ALARM1
}
TSound;



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                                 FUNCTIONS                              บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

// Plays a sound, which is selected with the argument `sound_type'.

void PlaySound(TSound sound_type);




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : SPCHARS.H                            Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* SPCHARS                                                                  */
/*--------------------------------------------------------------------------*/
/* Special chars of JPTUI (chars which depend on the font)  		    */
/****************************************************************************/




// Special chars

enum {
	SCH_WINDOW_UP_LEFT=0,
	SCH_WINDOW_UP,
	SCH_WINDOW_UP_RIGHT,
	SCH_WINDOW_LEFT,
	SCH_WINDOW_RIGHT,
	SCH_WINDOW_BOTTOM_LEFT,
	SCH_WINDOW_BOTTOM,
	SCH_WINDOW_BOTTOM_RIGHT,

	SCH_FRAME_UP_RIGHT,
	SCH_FRAME_LEFT,
	SCH_FRAME_RIGHT,
	SCH_FRAME_BOTTOM_LEFT,
	SCH_FRAME_BOTTOM,
	SCH_FRAME_BOTTOM_RIGHT,

	SCH_TEXTZONE_UP_LEFT,
	SCH_TEXTZONE_UP,
	SCH_TEXTZONE_UP_RIGHT,
	SCH_TEXTZONE_LEFT,
	SCH_TEXTZONE_RIGHT,
	SCH_TEXTZONE_BOTTOM_LEFT,
	SCH_TEXTZONE_BOTTOM,
	SCH_TEXTZONE_BOTTOM_RIGHT,

	SCH_TABGROUP_UP_LEFT,
	SCH_TABGROUP_UP,
	SCH_TABGROUP_UP_RIGHT,
	SCH_TABGROUP_LEFT,
	SCH_TABGROUP_RIGHT,
	SCH_TABGROUP_BOTTOM_LEFT,
	SCH_TABGROUP_BOTTOM,
	SCH_TABGROUP_BOTTOM_RIGHT,

	SCH_TAB_CAPTION_LEFT,

	SCH_LISTBOX_UP_LEFT,
	SCH_LISTBOX_UP,
	SCH_LISTBOX_UP_RIGHT,
	SCH_LISTBOX_LEFT,
	SCH_LISTBOX_RIGHT,
	SCH_LISTBOX_BOTTOM_LEFT,
	SCH_LISTBOX_BOTTOM,
	SCH_LISTBOX_BOTTOM_RIGHT,

	SCH_SHAFT_HORIZONTAL,
	SCH_SLIDER_HORIZONTAL,

	SCH_COMBOBOX_UP_RIGHT,
	SCH_COMBOBOX_BOTTOM_RIGHT,

	SCH_LIST_CHECK,

	SCH_LAST };

// Special strings

enum {
	SST_ARROW_UP=0,
	SST_ARROW_DOWN,
	SST_ARROW_LEFT,
	SST_ARROW_RIGHT,

	SST_SHAFT_VERTICAL,
	SST_SLIDER_VERTICAL,

	SST_CHECKBOX_CHECKED,
	SST_CHECKBOX_UNCHECKED,

	SST_RADIOBUTTON_CHECKED,
	SST_RADIOBUTTON_UNCHECKED,

	SST_WINDOW_CLOSE_BUTTON,

	SST_TEXTZONE_SB_CORNER,

	SST_ICON_STOP,
	SST_ICON_QUESTION,
	SST_ICON_EXCLAMATION,
	SST_ICON_INFORMATION,

	SST_LAST };

extern TFont JPFont;
extern char  JPSpecialChar  [SCH_LAST][2];
extern char *JPSpecialString[SST_LAST][2];


#define SPECIAL_CHAR(c)	 	(JPSpecialChar  [(int)c][(int)JPFont])
#define SPECIAL_STRING(s)	(JPSpecialString[(int)s][(int)JPFont])




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : STRINGS.H                            Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* STRINGS                                                                  */
/*--------------------------------------------------------------------------*/
/* Fonctions de manipulation des chanes de caractres (caption,text...)    */
/****************************************************************************/


#include <string.h>


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                                CONSTANTES                              บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

#define LONG_MAX_LENGTH    11    // Un long fait au plus 11 caractres
#define DOUBLE_MAX_LENGTH  24    // Un double fait au plus 24 caractres
                                 // (prvoyons large)

#define DOUBLE_NB_DIGITS   5     // Nombre de chiffres significatifs pour un
                                 // double

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                                 FONCTIONS                              บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

// Returns the last char of string (or 0 if string is "")

char EndOfString(const char *string);

// Conversion d'un chiffre hexa en nombre dcimal

int HexaToInt(char hexa_digit);

// Conversion d'une chane en un nombre du type indiqu.
// Retourne true si la chane tait valide.

bool StringToLong(const char *string, long &value);
bool StringToDouble(const char *string, double &value);

// Conversion d'un nombre en une chane
// (la chane doit tre alloue)

void LongToString(long value, char *string);
void DoubleToString(double value, char *string);

// Manipulation des lgendes des lments de l'interface
// (hot-key ventuelle prcde de ~, ~~ pour afficher ~)

char HotKey(const char *caption);        // Hot-key de caption
int  DisplayLength(const char *caption); // Longueur d'affichage de caption


// Manipulation des textes
// -> lignes spares par \n
// -> changement de couleur par [clx] o— x est la couleur d'affichage en hexa
//                          par [atxx] o— xx est l'attribut en hexa
// -> toute combinaison [..] ne fait pas partie des caractres affichs
// -> [[ pour afficher le caractre [

int  GetTextHeight(const char *text);    // Hauteur d'affichage de text
int  GetTextWidth(const char *text);     // Largeur d'affichage de text





/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : VOCAB.H                              Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* VOCAB                                                                    */
/*--------------------------------------------------------------------------*/
/* JPTui vocabulary functions                                               */
/****************************************************************************/




// Returns the current language

TLanguage GetLanguage();

// Loads vocabulary files specific to a language

bool   LoadVocabulary(TLanguage language);

// Removes vocabulary from memory

void      FreeVocabulary();

// Returns the chosen string in the current language

const char *GetString(int string_number);




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : WINDOWS.H                            Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* SCREEN                                                                   */
/*--------------------------------------------------------------------------*/
/* Microsoft Windows-related functions                                      */
/****************************************************************************/



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                                 FUNCTIONS                              บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

// Is the program running under Windows ?

bool RunningUnderWindows();

// Configures JPTUI to be ran in a DOS box under Windows

void    SetDosBoxMode();

// Configures JPTUI to be ran in full screen mode (under Windows or not)
// If the application is running in a window, it is forced to full screen

void    SetFullScreenMode();





/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : CALLBACK.HPP                         Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/


class TObject;

namespace jptui {

class Callback
{
public:
	typedef int	what_type;
	typedef int	item_type;
	virtual void	operator()(TObject* who, what_type what, item_type item) = 0;
	virtual ~Callback();
};

class CallbackHolder
{
public:
	CallbackHolder(TObject* who);
	void	set(Callback& callback, Callback::what_type what = 0);
	void	unset();
	bool	valid() const;
	void	operator()(Callback::item_type item = 0);
private:
	TObject*			who_;
	Callback*			callback_;
	Callback::what_type what_;
};

} // namespace jptui




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : BUTTONSBOX.HPP                         Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/


#include <memory>

namespace jptui {

// Window with buttons at the bottom

class ButtonsBox : public Callback
{
public:
	struct ButtonDescr
	{
		const char*	title_;	// 0 - end of buttons list
	};
	void		init(const char* title, int windowStyle,
				     const ButtonDescr* ds, 
					 int defaultButton = 0, int cancelButton = -1,
					 bool vertical = true,
					 size_t reserveX = 0, size_t reserveY = 0,
					 bool withInfoBar = false);	// reserve.. ignored for vertical
	virtual int	run();	// returns pressed button index
protected:
	static const int	MAX_BUTTONS_NUMBER = 10;
	std::auto_ptr<TWindow>		window_;
	std::auto_ptr<TPushButton>  buttons_[MAX_BUTTONS_NUMBER];
	int	run_;
	virtual void	operator()(TObject* who, what_type what, item_type item);
};

} // namespace jptui




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : MESSAGEBOX.HPP                         Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/



namespace jptui {

class MessageBox : public ButtonsBox
{
public:
	enum Icon 
	{ 
		NO_ICON, STOP_ICON, QUESTION_ICON,  EXCLAMATION_ICON, INFORMATION_ICON 
	};
	void	init(const char* title, int windowStyle, const char* text, 
				 Icon icon, const ButtonDescr* ds, 
				 int defaultButton, int cancelButton, 
			     const char* checkBoxTitle = 0, bool checkBoxValue = false);
	bool	checkBoxValue();
private:
	std::auto_ptr<TLabel>		iconLabel_;
	std::auto_ptr<TLabel>		textLabel_;
	std::auto_ptr<TCheckBox>	checkBox_;
	const char*	getIconText(Icon icon);
};

} // namespace jptui





/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : TAPPLICA.H                           Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* TAPPLICA                                                                 */
/*--------------------------------------------------------------------------*/
/* TApplication class : application                                         */
/*--------------------------------------------------------------------------*/
/* Most of the methods of this class are also provided as macros            */
/* (see JPAPPLI.H)                                                          */
/****************************************************************************/


#include <sys\timeb.h>

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                                 TYPES                                  บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

// List of windows

typedef struct window_node
{
  PWindow                   window;
  struct window_node        *next;
  struct window_node        *last;
} TWindowNode,*PWindowNode,*PWindowList;

// List of open windows

typedef struct open_window_node
{
  int                       window_number;
  int                       order;
  PObject                   owner;
  struct open_window_node   *next;
  struct open_window_node   *last;
} TOpenWindowNode,*POpenWindowNode,*POpenWindowList;


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                             CLASS DEFINITION                           บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

class TApplication
{
  /*----------------*/
  /* Friend classes */
  /*----------------*/

  friend class TObject;
  friend class TWindow;

  /*----------------*/
  /* Private fields */
  /*----------------*/

  private:

    // Application currently running
    // Only one application can be running

    static
    PApplication        f_current_application;

    // Windows of the application

    int                 f_nb_windows;
    PWindowList         f_window_list,
			f_last_window;

    PWindow             f_active_window;

    // Number and list of open windows by order of superposition

    int                 f_nb_open_windows;
    POpenWindowList     f_open_window_list,
			f_last_open_window;

    // Application state

    enum                { STOPPED,INITIALIZED,RUNNING }
			f_state;

    // Number of dialogs currently running

    int                 f_nb_running_dialogs;

    // The application or the current dialog
    // is going to be stopped as soon as possible

    bool             f_stop_wanted;

    // Callback processing is enabled

    bool             f_callbacks_enabled;

    // Double-click state

    enum    { FIRST_PRESSION,
	      FIRST_RELEASE,
	      SECOND_PRESSION,
	      SECOND_RELEASE
	    }
			f_double_click_state;

    // Last mouse button click time

    struct timeb        f_last_button_click_time;

    // The screen need to be refreshes
    // What needs to be refreshed

    bool             f_need_refresh;
    TIntZone            f_zone_to_refresh;
    int                 f_upper_changed_window_height;

    // Clipboard

    char                *f_clipboard;

  /*----------------*/
  /* Public methods */
  /*----------------*/

  public:

    // Constructor / Destructor

			TApplication(TLanguage language);
    virtual             ~TApplication();

    // Handler called in case a memory allocation fails :
    // stops the program

//	    static
//	    void        m_new_handler();

    // Handler called when the user use CTRL-BREAK

	    static
	    void        m_ctrl_break_handler(int);

    // Returns the running application

	    static
	    PApplication
			m_get_current_application() { return(f_current_application);};

    // Initializes and starts the application, runs a dialog,
    // processes events once, stops a dialog or the application

	    void        m_init();
	    void        m_run();
	    void        m_run_dialog();
	    void        m_process_events();
	    void        m_stop();


    // Refreshes :
    // - the part of the screen that has changed
    // - the whole screen

	    void        m_refresh();
	    void        m_refresh_all();

    // Redraws the contents of windows

	    void        m_redraw_windows();

    // Makes the next window (relatively to the currently-selected
    // window) active

	    bool     m_make_next_window_active();

    // Returns a pointer to the active window (NULL if none)

	    PWindow     m_get_active_window();

    // Clipboard reading and writing

	    char        *m_get_clipboard()    { return(f_clipboard);};
	    void        m_set_clipboard(const char *text);

    // Enables/Disables callback processing

	    void        m_enable_callbacks()  { f_callbacks_enabled=true;};
	    void        m_disable_callbacks() { f_callbacks_enabled=false;};
	    bool     m_callbacks_enabled() { return(f_callbacks_enabled);};


    // Callback invoked when no event occurs
    jptui::CallbackHolder	noEventAction_;

    // Callback invoked when the contents of the clipboard change.
    jptui::CallbackHolder	clipboardChangedAction_;

	class KeyCallback 
	{
	public:
		virtual bool	operator()(const TKey& key) = 0;
	};
	KeyCallback*	keyCallback() const;
	void	setKeyCallback(KeyCallback* keyCallback);

  /*-----------------*/
  /* Private methods */
  /*-----------------*/

  private:

    // The application main loop

	    void        m_main_loop();

    // Closes the application

	    void        m_close();

    // Display routines

	    void        m_refresh_window(int open_window_number,int x1,int y1,int x2,int y2);

    // To add, remove, open or close a window

	    int         m_add_window(PWindow window);
	    void        m_del_window(int window_number);

	    void        m_open_window(int window_number,PObject owner);
	    void        m_close_window(int window_number);


    // To add a zone to refresh

	    void        m_add_zone_to_refresh(int x1,int y1,int x2,int y2,int window_height);

    // Misc routines

	    PWindow     m_number_to_window(int window_number);
	    PWindowNode m_number_to_window_node(int window_number);

	    int         m_height_to_window_nb(int window_height);
	    int         m_window_nb_to_height(int window_nb);

	    bool     m_set_active_window(int window_number, bool even_if_current_modal=false);

    // Events

    virtual bool     m_left_button_pressed_event(int x,int y);
    virtual bool     m_left_button_double_click_event(int x,int y);
    virtual bool     m_key_pressed_event(TKey key);

	KeyCallback*	keyCallback_;
};




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : JPAPPLI.H                            Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* JPAPPLI                                                                  */
/*--------------------------------------------------------------------------*/
/* Fonctions de manipulation de l'application ralise avec la librairie    */
/* JPTUI                                                                    */
/****************************************************************************/




/******************************************/
/* Fonctions pour contr“ler l'application */
/******************************************/

// Application

#define JPApplication        (TApplication::f_current_application)

// Arrt sur manque de mmoire

#define JPOutOfMemoryExit()  (TApplication::m_new_handler())

// Dmarrage/arrt

#define JPInit()             ((TApplication::m_get_current_application())->m_init())
#define JPRun()              ((TApplication::m_get_current_application())->m_run())
#define JPRunDialog()        ((TApplication::m_get_current_application())->m_run_dialog())
#define JPProcessEvents()    ((TApplication::m_get_current_application())->m_process_events())
#define JPStop()             ((TApplication::m_get_current_application())->m_stop())

// Refreshes the display/the whole screen

#define JPRefresh()          ((TApplication::m_get_current_application())->m_refresh())
#define JPRefreshAll()       ((TApplication::m_get_current_application())->m_refresh_all())

// Redraws the contents of windows

#define JPRedrawWindows()    ((TApplication::m_get_current_application())->m_redraw_windows())

// Rend la fentre suivante active (retourne false si impossible)

#define JPMakeNextWindowActive() \
			     ((TApplication::m_get_current_application())->m_make_next_window_active())

// Returns a pointer to the active window

#define JPGetActiveWindow()  ((TApplication::m_get_current_application())->m_get_active_window())
// Clipboard

#define JPGetClipboard()     ((TApplication::m_get_current_application())->m_get_clipboard())
#define JPSetClipboard(text) ((TApplication::m_get_current_application())->m_set_clipboard(text))


// Autorisation/Interdiction des callbacks

#define JPEnableCallbacks()  ((TApplication::m_get_current_application())->m_enable_callbacks())
#define JPDisableCallbacks() ((TApplication::m_get_current_application())->m_disable_callbacks())
#define JPCallbacksEnabled() ((TApplication::m_get_current_application())->m_callbacks_enabled())

// Dfinition des callbacks

#define JPSetNoEventCallback(no_event_action,no_event_argument)  ((TApplication::m_get_current_application())->m_set_no_event_callback(no_event_action,no_event_argument))
#define JPSetClipboardChangedCallback(clipboard_changed_action,clipboard_changed_argument)  ((TApplication::m_get_current_application())->m_set_clipboard_changed_callback(clipboard_changed_action,clipboard_changed_argument))






/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : TOBJECT.H                            Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* TOBJECT                                                                  */
/*--------------------------------------------------------------------------*/
/* Objet TObject (type gnrique pour les lments de l'interface JPTUI     */
/****************************************************************************/


#include <string>

// Liste double-chane d'objets

typedef struct object_node
{
  PObject                    object;
  struct object_node        *next;
  struct object_node        *last;
} TObjectNode,*PObjectNode;


class TObject
{
  /*-------------*/
  /* Objets amis */
  /*-------------*/

  friend class TApplication;
  friend class TGroup;	// Pour que le groupe puisse contr“ler
			   			// les objets qu'il content
  /*---------------*/
  /* Champs privs */
  /*---------------*/

  private:

  /*-----------------*/
  /* Champs protgs */
  /*-----------------*/

  protected:

    // Fentre qui contient l'objet

    PWindow f_window;
    bool f_dynamic_window; // Fentre alloue dynamiquement par l'objet

    // Groupe qui contient l'objet (NULL si l'objet est une fentre)

    PObject f_parent;

    // Numro de l'objet dans la fentre

    int     f_number;

    // Type de l'objet (cf CONST.H)
    // -> pas forcment redfini pour les objets drivs

    int     f_type;

    // Coordonnes de l'objet p/r au groupe le contenant
    // et ses dimensions

    int     f_rel_x,
	    f_rel_y,
	    f_width,
	    f_height;

    // Couleur de fond

    unsigned
	    f_background;


    // Lgende de l'objet

    char   *f_caption;

    // Hot-Key de l'objet (0 si aucune)

    char    f_hot_key;

    // Etat de l'objet

    bool f_open;          // L'objet est ouvert
    bool f_focused;       // L'objet a le focus
    bool f_enabled;       // L'objet est activable

    // L'objet est simple
    // (non compos d'autres objets)

    bool f_simple;


    // L'objet gre les acclrateurs
    // Elments de l'objet
    // (Objet compos)

    int     f_nb_elements;
    PObjectNode
            f_element_list,
            f_last_element;

    // L'objet peut-il devenir activable

    bool f_can_normally_be_enabled; // Dpend de l'objet
    bool f_can_be_enabled;         // Etat actuel. Peut tre modifi
                                      // si f_default_can_be_enabled est true

    // L'aspect de l'objet varie-t-il quand il prend ou perd le focus ?

    bool f_focus_depending_aspect;

    // L'affichage des lments est autoris

    bool f_elements_display_enabled;

  /*--------------------*/
  /* Mthodes publiques */
  /*--------------------*/

  public:

    // Constructeur / Destructeur

		    TObject(PObject parent,
			    int type,
			    int rel_x,int rel_y,
			    int width,int height,
			    unsigned background,
			    const char *caption,
			    bool enabled,
			    bool focus_depending_aspect,
                            bool can_be_enabled,
			    bool simple);

    virtual         ~TObject();

    // Mthodes d'accs

	    int     m_get_number() { return (f_number);};
	    int     m_get_type()   { return (f_type); };
	    PObject m_get_parent()  { return (f_parent);  };
            PWindow m_get_window() { return(f_window);};

	    int     m_get_x_in_window();
	    int     m_get_y_in_window();

    virtual int     m_get_x();
    virtual int     m_get_y();

	    int     m_get_width()  { return (f_width); };
	    int     m_get_height() { return (f_height); };

	    // Changement des dimensionnements (si objet ferm uniquement)

	    void    m_set_size(int width,int height);

	    unsigned  m_get_background()     { return(f_background);};

 	    // Fix: added to change colors of another objects
	    void      m_set_background(unsigned a_backgorund) 
              { f_background = a_backgorund;};

	    char    *m_get_caption() { return (f_caption);};
    virtual void    m_set_caption(const char *caption);

	    char    m_get_hot_key(){ return (f_hot_key);};
	    bool m_is_open()    { return (f_open); };
	    bool m_has_focus()  { return (f_focused);};
	    bool m_is_enabled() { return (f_enabled); };

    virtual void    m_enable();
    virtual void    m_disable();

	    // Seulement efficace pour des objets par nature activable

	    void    m_set_can_be_enabled(bool can_be_enabled);


    virtual bool m_can_lose_focus();

    // Donne le focus … l'objet

    virtual bool m_set_focus();
    virtual bool m_set_focus_to_last_element(); // Pareil dans ce cas


    // Callback : Fonction appele lorsque l'objet perd le focus
    //            et son argument

	jptui::CallbackHolder	focusLostAction_;

    // Callback : Fonction appele lorsque l'objet prend le focus
    //            et son argument

	jptui::CallbackHolder	focusTakenAction_;

    // Callback invoked when the object is opened

	jptui::CallbackHolder	openedAction_;

    // Callback invoked when the object is closed

	jptui::CallbackHolder	closedAction_;

  /*--------------------*/
  /* Mthodes protges */
  /*--------------------*/

  protected:

    // Ajout d'un lment … l'objet

    virtual int     m_add_element(PObject object);
    virtual	    void    m_del_element(int object_number);

    PObject         m_object_number_to_element(int object_number);
    PObjectNode     m_object_number_to_element_node(int object_number);


    // Affichage

    virtual void    m_display();
    virtual void    m_display_focus_depending_part();
    virtual void    m_display_caption();
    virtual void    m_display_element(PObject element) { element->m_display();};

    // Attribution du focus … un lment de l'objet

    virtual bool m_set_focus_to_element(int object_number);

    // Prise/Perte du focus

    virtual void    m_lose_focus();

    virtual void    m_take_focus();

    // Ouverture/Fermeture de l'objet

    virtual void    m_set_open(bool open);

    // L'objet devient actif/inactif

    virtual void    m_set_active(bool active);

    // Callbacks

    virtual void    m_opened_callback();
    virtual void    m_closed_callback();

    // Evnements

    virtual bool m_left_button_pressed_event(int x,int y);
    virtual bool m_left_button_double_click_event(int x,int y);
    virtual bool m_key_pressed_event(TKey key);
    virtual bool m_short_cut_pressed_event(int short_cut);

    // Accs aux objets par les hritiers

	    void    m_set_element_open(PObject element,bool open) { element->m_set_open(open);};

	    bool m_left_button_pressed_event_on(PObject element,int x,int y){ return(element->m_left_button_pressed_event(x,y));};
	    bool m_left_button_dbl_click_event_on(PObject element,int x,int y){ return(element->m_left_button_double_click_event(x,y));};
	    bool m_key_pressed_event_on(PObject element,TKey key){ return(element->m_key_pressed_event(key));};
	    bool m_short_cut_pressed_event_on(PObject element,int short_cut){return(element->m_short_cut_pressed_event(short_cut));};

	    void    m_element_lose_focus(PObject element) { element->m_lose_focus();};
	    void    m_element_take_focus(PObject element) { element->m_take_focus();};

	    bool m_parent_set_focus_to_element(int element_nb) { return(f_parent->m_set_focus_to_element(element_nb));};

  /*------------------*/
  /* Mthodes prives */
  /*------------------*/

  private:

    // Sert … obtenir l'objet d'un groupe qui a le focus

    virtual PObject m_get_focused_object() { return(this);};

    // L'objet peut obtenir le focus lors d'un dplacement
    // avec les touches flches

    virtual bool m_focus_can_be_set_by_arrow_key() { return true; };

    // Permet de modifier l'objet de la fentre qui possdait le focus
    // avant que le focus ne change d'objet

    void	m_set_previous_focused_object();

};




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : TGROUP.H                             Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* TGROUP                                                                   */
/*--------------------------------------------------------------------------*/
/* Objet TGroup (Type gnrique d'objet comprenant d'autres objets dont un  */
/*               seul … la fois a le focus)                                 */
/****************************************************************************/






class TGroup:public TObject
{
  /*-------------*/
  /* Objets amis */
  /*-------------*/

  friend class TObject;	// Pour que les objets puissent se
		  				// rajouter au groupe...

  /*-----------------*/
  /* Champs protgs */
  /*-----------------*/

  protected:

    // Objet du groupe qui a le focus

    PObjectNode f_focused_element;

    // Indique si le groupe peut n'avoir aucun lment
    // avec le focus quand lui l'a

    bool f_need_focused_element;

  /*--------------------*/
  /* Mthodes publiques */
  /*--------------------*/

  public:

    // Constructeur / Destructeur

		    TGroup(PObject parent,
			   int type,
			   int rel_x,int rel_y,
			   int width,int height,
			   unsigned background,
			   const char *caption,
			   bool enabled,
			   bool focus_depending_aspect,
                           bool can_be_enabled,
			   bool need_focused_element);
    virtual         ~TGroup();

    // Mthodes d'accs

    virtual PObject m_get_focused_element();

    // Donne le focus au groupe (au 1er/dernier de ses objets qui le veut)

    virtual bool m_can_lose_focus();
    virtual bool m_set_focus();
    virtual bool m_set_focus_to_last_element();

    // Donne le focus (si possible) … celui qui prcde ou suit celui
    // qui a le focus

    virtual bool m_set_focus_to_previous_element();
    virtual bool m_set_focus_to_next_element();


    // Retourne l'objet du groupe qui a le focus

    virtual PObject m_get_focused_object();


  /*--------------------*/
  /* Mthodes protges */
  /*--------------------*/

  protected:

    virtual void    m_del_element(int object_number);

    // Attribution du focus … un objet du groupe

    virtual bool m_set_focus_to_element(int object_number);

    // Prise/Perte du focus

    virtual void    m_lose_focus();

    virtual void    m_take_focus();

    // Evnements

    virtual bool m_left_button_pressed_event(int x,int y);
    virtual bool m_left_button_double_click_event(int x,int y);
    virtual bool m_key_pressed_event(TKey key);

  /*------------------*/
  /* Mthodes prives */
  /*------------------*/

  private:

    // Evnements

    virtual bool m_tab_pressed_event();
    virtual bool m_shift_tab_pressed_event();

    // L'objet peut obtenir le focus lors d'un dplacement
    // avec les touches flches

    virtual bool m_focus_can_be_set_by_arrow_key() { return false; };
};




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : TWINDOW.H                            Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* TWINDOW                                                                  */
/*--------------------------------------------------------------------------*/
/* Objet TWindow (fentre)                                                  */
/****************************************************************************/


#include <conio.h>





/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                                CONSTANTES                              บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

// Position d'une chaine dans sa zone d'affichage

#define CENTERED        0
#define JUSTIFIED_LEFT  1
#define JUSTIFIED_RIGHT 2

#define CENTERED_LEFT   4
#define CENTERED_RIGHT  5

// Types des flches

#define ARROW_UP    0
#define ARROW_DOWN  1
#define ARROW_LEFT  2
#define ARROW_RIGHT 3

// Styles des fenetres

//#define DIALOG1 (LIGHTBLUE)
#define DIALOG1 (LIGHTBLUE + (LIGHTGREEN << TWINDOW_BRIGHT_ATTR_SHIFT))
#define DIALOG2 (LIGHTCYAN)
#define DIALOG3 (CYAN)

#define ALERT   (RED)
#define HELP    (GREEN)

// Fix: adding user defined colors
#define TWINDOW_COLOR_MASK	   0xF
#define TWINDOW_BRIGHT_ATTR_SHIFT  8
#define TWINDOW_NORMAL_ATTR_SHIFT  16

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                         DEFINITION DE L'OBJET                          บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

class TWindow:public TGroup
{
  /*-------------*/
  /* Objets amis */
  /*-------------*/

  friend class TApplication;	// Pour que l'application facilement manipuler ses
								// fentres
  friend class TObject;       
  friend class TPushButton;		// Pour grer les boutons spciaux

  /*------------------*/
  /* Protected fields */
  /*------------------*/

  protected:

    // Position de la fentre dans l'cran (1...)

    int         f_x,
		f_y;

    // There is an information bar

    bool     f_info_bar;

    // Couleur d'affichage du titre quand il a le focus

    unsigned    f_title_attribute;

    // Couleur d'affichage de la bordure

    unsigned    f_border_attribute;

    // Fix: to redefine shadows type
protected: TShadowChoice shadows; 
public:    void 	 set_shadows(TShadowChoice a_shadows)
	   { shadows = a_shadows; };

  /*----------------*/
  /* Private fields */
  /*----------------*/

  private:

    // Numro de la fentre dans l'application

    int         f_window_number;

    // If the information bar exits, message it displays

    char        *f_info_message;

    // La fentre est modale

    bool     f_modal;

    // La fentre est dplaable

    bool     f_movable;

    // La fentre est active

    bool     f_active;

    // Objet qui gre les acclrateurs de la fentre

    PObject     f_short_cut_handler;

    // Boutons spciaux de la fentre

    PPushButton f_default_button;  // Bouton par dfaut
    PPushButton f_ok_button;       // Bouton actuellement activable par RETURN
    PPushButton f_cancel_button;   // Bouton actuellement activable par ESC

    // Pour l'affichage dans la fentre
    // (n'apparat … l'cran qu'aprs un rafrachissement)

    word        *f_window_content; // Contenu de la fentre

    bool     f_content_changed; // Le contenu de la fentre a chang depuis
				   // le dernier rafrachissement

    TIntZone    f_changed_zone;    // si f_content_changed est true,
				   // zone de la fentre dont le contenu
				   // a t modifi depuis le dernier rafrachissement

    TIntZone    f_clip_window;     // Limites d'affichage du texte

    int         f_x_cursor;        // Coordonnes du curseur de la fentre
    int         f_y_cursor;        // (position d'affichage, pas curseur cran)
    word        *f_cursor_ptr;     // Pointeur sur le caractre courant

    int         f_text_attribute;  // Attribut d'affichage

    // Objet de la fentre qui avait le focus avant que le focus ne change
    // d'objet dans la fentre

    PObject     f_previous_focused_object;

    // Callback : Fonction appele lorsque l'on clique sur l'icone de
    //            fermeture de la fentre
    //            et son argument
	jptui::CallbackHolder	closeButtonPressedAction_;

  /*--------------------*/
  /* Mthodes publiques */
  /*--------------------*/

  public:

    // Constructeur / Destructeur

		     TWindow(unsigned style,
			     int x, int y,
			     int width, int height,
			     const char *caption = "",
			     bool info_bar=false,
			     bool modal=true,
			     bool movable=true);

    virtual  	     ~TWindow();

    // Mthodes d'accs

    virtual int      m_get_x()  { return (f_x); };
    virtual int      m_get_y()  { return (f_y); };

	    void     m_set_pos(int x,int y);

	    void     m_set_title_attr(unsigned attribute) {f_title_attribute=attribute;};
	    void     m_set_border_attr(unsigned attribute) {f_border_attribute=attribute;};

	    void     m_set_short_cut_handler(PObject handler) {f_short_cut_handler=handler;};
	    void     m_set_default_button(PPushButton button);
	    void     m_set_cancel_button(PPushButton button);

    // Donne le focus … la fentre (au 1er/dernier de ses objets qui le veut)
    // Sinon, la fentre seul (aucun objet) prend le focus

    virtual bool  m_set_focus();
    virtual bool  m_set_focus_to_last_element();

    // Retourne l'objet de la fentre qui avait le focus
    // avant que celui ne change d'objet

	    PObject  m_get_previous_focused_object() {return(f_previous_focused_object);};

    // Rend la fentre active

	    bool  m_activate();
	    bool  m_is_active() {return(f_active);};

    // Ouverture/Fermeture de la fentre
    // Si on appelle m_open sur une fentre dj… ouverte,
    // la fentre passe au premier plan

	    void     m_open();
	    void     m_close();

	    // ATTENTION : Ne pas utiliser en dehors du code des objets !
	    void     m_open_as_object_element(PObject object);


    // Modification du message d'information

	    void     m_set_info_message(const char *message);
	    const char *m_get_info_message();

    // Mthodes d'affichage dans la fentre

	    void     m_gotoxy(int x,int y);

	    unsigned m_get_normal_attr(unsigned background);
	    unsigned m_get_inverse_attr(unsigned background);
	    unsigned m_get_bright_attr(unsigned background);
	    unsigned m_get_inverse_bright_attr(unsigned background);
	    unsigned m_get_inactive_attr(unsigned background);
	    unsigned m_get_inverse_inactive_attr(unsigned background);

	    void     m_textattr(unsigned attribute);
	    void     m_set_normal_attr(unsigned background)           { m_textattr(m_get_normal_attr(background));};
	    void     m_set_inverse_attr(unsigned background)          { m_textattr(m_get_inverse_attr(background));};
	    void     m_set_bright_attr(unsigned background)           { m_textattr(m_get_bright_attr(background));};
	    void     m_set_inverse_bright_attr(unsigned background)   { m_textattr(m_get_inverse_bright_attr(background));};
	    void     m_set_inactive_attr(unsigned background)         { m_textattr(m_get_inactive_attr(background));};
	    void     m_set_inverse_inactive_attr(unsigned background) { m_textattr(m_get_inverse_inactive_attr(background));};

	    void     m_set_clip_window(int x,int y,int width,int height);
	    void     m_reset_clip_window();

	    void     m_cls(unsigned background);

	    void     m_putch(u_char character);
	    void     m_putnch(int nb_chars,u_char character);
	    void     m_puts(const char *string);

	    void     m_put_caption(const char *main_string, bool show_hot_key, int length, int position);
	    void     m_put_text(const char *text);
	    void     m_draw_frame(int x1,int y1,int x2,int y2,unsigned background, const char *caption, bool enabled);
	    void     m_display_arrow(int arrow_type);

	void	setCloseButtonPressedCallback(jptui::Callback& callback,
									  	  jptui::Callback::what_type what);
  /*--------------------*/
  /* Mthodes protges */
  /*--------------------*/

  protected:

    // Affichage

    virtual void     m_display();
    virtual void     m_display_focus_depending_part();
    virtual void     m_display_caption();
    virtual void     m_display_close_button(bool pressed);
    virtual void     m_display_info_bar();

    // Evnements

    virtual bool  m_left_button_pressed_event(int x,int y);
    virtual bool  m_key_pressed_event(TKey key);

  /*------------------*/
  /* Mthodes prives */
  /*------------------*/

  private:

    // Manipulation des boutons spciaux

	    void     m_set_ok_button(PPushButton button);
	    void     m_set_ok_button_to_default();
	    void     m_unset_ok_button();
	    void     m_button_destructed(PPushButton button);

    // Effacement de la fentre

    virtual void     m_clear();

    // Ajoute la fentre dans la zone d'cran … rafrachir

	    void     m_add_to_refresh_zone();

    // Ecriture directe … l'cran

	    void     m_part_to_screen(int x1,int y1,int x2,int y2);
	    void     m_shadows_to_screen(int x1,int y1,int x2,int y2);

    // Ouverture/Fermeture de l'objet

    virtual void     m_set_open(bool open);

    // L'objet devient actif/inactif

    virtual void    m_set_active(bool active);

    // Evnements

	    bool  m_close_button_pressed_event();

};




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : TCHKBOX.H                            Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* TCHKBOX                                                                  */
/*--------------------------------------------------------------------------*/
/* Objet TCheckBox (Case … cocher)                                          */
/****************************************************************************/




class TCheckBox:public TObject
{
  /*-----------------*/
  /* Champs protgs */
  /*-----------------*/

  protected:

    // CheckBox coche ou non

    bool f_checked;

  /*--------------------*/
  /* Mthodes publiques */
  /*--------------------*/

  public:

    // Constructeur / Destructeur

		    TCheckBox(PObject parent,
			      int rel_x,int rel_y,
			      int width, const char *caption,
			      bool checked=false,
                              bool enabled=true);

    virtual         ~TCheckBox();

    // Mthodes d'accs

	    bool m_is_checked() { return(f_checked); };
    virtual void    m_check();
    virtual void    m_uncheck();

    // Callback : Fonction appele si on coche la checkbox
	//            et son argument
	jptui::CallbackHolder	checkedAction_;

    // Callback : Fonction appele si on dcoche la checkbox
    //            et son argument
	jptui::CallbackHolder	uncheckedAction_;

  /*--------------------*/
  /* Mthodes protges */
  /*--------------------*/

    // La case … cocher est coche/dcoche

	    void    m_take_check();
	    void    m_lose_check();

  /*------------------*/
  /* Mthodes prives */
  /*------------------*/

  private:

    // Affichage

    virtual void    m_display_focus_depending_part();
    virtual void    m_display_caption();

    // Evnements

    virtual bool m_left_button_pressed_event(int x,int y);
    virtual bool m_key_pressed_event(TKey key);
};




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : TFRAME.H                             Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* TFRAME                                                                   */
/*--------------------------------------------------------------------------*/
/* Objet TFrame (Group d'objets entours d'un cadre)                        */
/****************************************************************************/




class TFrame:public TGroup
{

  /*--------------------*/
  /* Mthodes publiques */
  /*--------------------*/

  public:

    // Constructeur/Destructeur

		    TFrame(PObject parent,
			   int rel_x, int rel_y,
			   int width,int height,
			   const char *caption = "",
			   bool enabled=true);
    virtual         ~TFrame();

    // Fix: frame color redefinition
    void	     set_frame_color(unsigned a_color) 
      { frame_color = a_color; };

  /*--------------------*/
  /* Mthodes protges */
  /*--------------------*/

  protected:

    // Affichage

    virtual void    m_display_caption();

    // Fix: added frame color to be able to redefine
    unsigned	    frame_color;	
};





/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : TRDIOGRP.H                           Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* TRDIOGRP                                                                 */
/*--------------------------------------------------------------------------*/
/* Objet TRadioGroup (groupe de boutons radio)                              */
/****************************************************************************/





class TRadioGroup:public TFrame
{
  /*----------------*/
  /* Friend classes */
  /*----------------*/

  friend class TRadioButton;

  /*------------------*/
  /* Mthodes prives */
  /*------------------*/

  private:

    // Bouton radio du groupe qui est coch (NULL si aucun)

    PRadioButton f_checked_radio_button;

  /*--------------------*/
  /* Mthodes publiques */
  /*--------------------*/

  public:

    // Constructeur/Destructeur

		    TRadioGroup(PObject parent,
				int rel_x,int rel_y,
				int width,int height,
				char *caption="",
				bool enabled=true);

    virtual  	    ~TRadioGroup();

  /*------------------*/
  /* Mthodes prives */
  /*------------------*/

  private:

    // Changement de bouton coch

	    void m_change_checked_radio_button_to(int object_number);

    // Evnements

    virtual bool m_tab_pressed_event()  { return false; };
    virtual bool m_shift_tab_pressed_event() { return false; };


};




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : TRDIOBUT.H                           Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* TRDIOBUT                                                                 */
/*--------------------------------------------------------------------------*/
/* Objet TRadioButton (bouton radio)                                        */
/****************************************************************************/





class TRadioButton:public TCheckBox
{
  /*--------------------*/
  /* Mthodes publiques */
  /*--------------------*/

  public:

    // Constructeur / Destructeur

		    TRadioButton(PRadioGroup radio_group,
				 int rel_x,int rel_y,
				 int width, const char *caption,
				 bool checked=false,
                                 bool enabled=true);
    virtual         ~TRadioButton();

    // Mthodes d'accs

    virtual void    m_check();
    virtual void    m_uncheck();

    // Affichage

    virtual void    m_display_focus_depending_part();
    virtual void    m_display_caption();

    // Evnements

    virtual bool m_left_button_pressed_event(int x,int y);
    virtual bool m_key_pressed_event(TKey key);

  /*------------------*/
  /* Mthodes prives */
  /*------------------*/

  private:


};




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : TPUSHBUT.H                           Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* TPUSHBUT                                                                 */
/*--------------------------------------------------------------------------*/
/* Objet TPushButton (bouton … presser)                                     */
/****************************************************************************/



// Types des boutons (types cumulables par l'oprateur |)

#define  PB_NORMAL            0
#define  PB_DEFAULT           1  // 000000000001
#define  PB_CANCEL            2  // 000000000010

// Dure d'enfoncement d'un bouton lors de son appui par appel … m_press
// (en ms)

#define PRESSURE_TIME_LENGTH 75

class TPushButton:public TObject
{
  /*-------------*/
  /* Objets amis */
  /*-------------*/

  friend class TWindow;	// Pour que la fentre puisse accder directement
		   				// aux boutons spciaux

  /*------------------*/
  /* Protected fields */
  /*------------------*/

  protected:

    // Shadows

    TShadowChoice f_shadows;


  /*----------------*/
  /* Private fields */
  /*----------------*/

  private:

    // Bouton enfonc ou non

    bool f_pressed;

    // Bouton enfonc par action sur le clavier

    bool f_pressed_by_key;

    //    Bouton actionnable par RETURN
    // et bouton actionnable par ESCAPE

    bool f_ok_button;
    bool f_cancel_button;


  /*--------------------*/
  /* Mthodes publiques */
  /*--------------------*/

  public:

    // Constructeur / Destructeur

		    TPushButton(PObject parent,
				int rel_x,int rel_y,
				int width,
				const char *caption,
				int  type=PB_NORMAL,
				TShadowChoice shadows=SHC_BOTH,
				bool enabled=true);

    virtual	    ~TPushButton();

    // Appui sur le bouton

            void    m_press();


		jptui::CallbackHolder	pressedAction_;
  /*--------------------*/
  /* Mthodes protges */
  /*--------------------*/

  protected:

    // Prise/Perte du focus

    virtual void    m_lose_focus();
    virtual void    m_take_focus();


  /*------------------*/
  /* Mthodes prives */
  /*------------------*/

  private:

    // Affichage

    virtual void    m_display_focus_depending_part();
    virtual void    m_display_caption();

    // Le bouton devient/n'est plus
    // le ok_button ou le cancel_button

	    void    m_take_ok();
	    void    m_lose_ok();
	    void    m_take_cancel() { f_cancel_button=true; };
	    void    m_lose_cancel() { f_cancel_button=false; };

  protected: // Fix: to be able to redefine
    // Evnements

    virtual bool m_left_button_pressed_event(int x,int y);
    virtual bool m_key_pressed_event(TKey key);

};




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : TEDZONE.H                            Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* TEDZONE                                                                  */
/*--------------------------------------------------------------------------*/
/* Objet TEditZone (Zone d'dition sur une ligne)                           */
/****************************************************************************/



// Vitesse de dfilement du texte

#define EDITZONE_FIRST_SCROLL_SPEED  150
#define EDITZONE_SCROLL_SPEED        70

class TEditZone:public TObject
{
  /*-----------------*/
  /* Champs protgs */
  /*-----------------*/

  protected:

    // Longueur visible et longueur maximale du texte

    int     f_display_length,
	    f_max_length;

    // Modification du texte autorise

    bool f_modification_enabled;

    // Vrification de la validit de la chane active

    bool f_string_checking;

    // Longueur utile de la chane (sans les . finaux)

    int     f_useful_length;

    // Chane en cours de saisie (avec les . finaux)

    char    *f_string;

    // Chane par dfaut (sans les .finaux)

    char    *f_default_string;

    // Message affich si la saisie est invalide

    char    *f_error_message;


    // Mode insertion/recouvrement
    // (champ commun … toutes les instances de l'objet)

    static
    bool f_insert_mode;

    // Position du titre par rapport au texte

    int     f_xoffset_caption,
	    f_yoffset_caption;

    // Indice de chane du premier caractre visible

    int     f_first_visible_char;

    // Une zone est slectionne ?

    bool f_zone_selected;

    // Si oui :
    // -> caractres de position min et max de la zone slectionne

    int     f_min_selected_char;
    int     f_max_selected_char;

    // En cours de slection :
    // -> premier et dernier caractres slectionnes
    //    (par ordre de slection)

    int     f_first_selected_char;
    int     f_last_selected_char;

    // Position du cursor (indice dans la chane -> 0..f_useful_length)

    int     f_cursor_position;

    // Donnes au moment o— la zone de saisie prend le focus

    char    *f_string_before_focus;      // Chane avant modification
                                         // (sans les . finaux)

    // Chane pour annulation par ALT_BACKSPACE

    char    *f_undo_string;
    bool  f_undo_string_can_change;

    // Chane retourne par m_get_string (sans les . finaux)

    char    *f_return_string;

  /*--------------------*/
  /* Mthodes publiques */
  /*--------------------*/

  public:

    // Constructeur / Destructeur

     	            TEditZone(PObject parent,
		              int rel_x, int rel_y,
                              int xoffset_caption, int yoffset_caption,
			      const char *caption,
                              int display_length,
			      int max_length,
			      const char *string = "",
			      bool enabled=true);
    virtual         ~TEditZone();

    // Mthodes d'accs

	    void    m_set_cursor_at(int char_nb);

    virtual void    m_enable_modification()    {f_modification_enabled=true;};
    virtual void    m_disable_modification()   {f_modification_enabled=false;};

    virtual void    m_enable_string_checking() {f_string_checking=true;};
    virtual void    m_disable_string_checking(){f_string_checking=false;};

	    void    m_select_zone(int position1,int position2);
	    void    m_select_all()       {m_select_zone(0,f_useful_length);};
	    void    m_unselect();
            void    m_clear_selection();

    virtual void    m_set_string(const char *string);

    // La chane peut tre manipule tant que l'objet existe

	    char    *m_get_string();
	    int     m_get_string_length() { return(f_useful_length);};

    // Validit d'une chane pour la zone

    virtual bool m_string_valid(const char *string);

    // La zone peut-elle perdre le focus
    // (si non, affiche une boite de dialogue)

    virtual bool m_can_lose_focus();



    // Callback : Fonction appele … chaque modification de la chane
    //            et son argument
	jptui::CallbackHolder	stringChangedAction_;

            void set_password_mode(bool a_mode) 
            {
              in_password_mode = a_mode; 
              m_display_string();
            }; 

  /*--------------------*/
  /* Mthodes protges */
  /*--------------------*/

  protected:

    // Affichage

    virtual void    m_display();
    virtual void    m_display_focus_depending_part();
    virtual void    m_display_caption();
    virtual void    m_display_string();

    // Prise/Perte du focus

    virtual void    m_lose_focus();
    virtual void    m_take_focus();

    // L'objet devient actif/inactif

    virtual void    m_set_active(bool active);

    // Evnements

    virtual bool m_left_button_pressed_event(int x,int y);
    virtual bool m_left_button_double_click_event(int x,int y);
    virtual bool m_key_pressed_event(TKey key);

    // Caractres saisis

	    void    m_character_hit(int character);

	    void    m_left_hit(bool shift_pressed);
	    void    m_right_hit(bool shift_pressed);
	    void    m_home_hit(bool shift_pressed);
	    void    m_end_hit(bool shift_pressed);
	    void    m_alt_backspace_hit();
	    void    m_backspace_hit();      // Annule les modifications
	    void    m_delete_hit();
	    void    m_insert_hit();

    // Appel des callbacks

    virtual void    m_string_modified_callback();


  /*------------------*/
  /* Mthodes prives */
  /*------------------*/

  private:

    // Calculs/Oprations sur la chane

	    void    m_calc_useful_length();
    virtual bool m_character_valid(int character);
    virtual const char *m_get_error_message();

    // Dplacement du curseur

	    void    m_set_cursor_position(int cursor_position);
	    void    m_move_cursor_with_key_to(bool shift_pressed,int new_cursor_pos);

    // Rend le curseur visible

	    void    m_show_cursor();

    // Pour l'affichage
    bool in_password_mode; // to display every symbol as '*'

    virtual void    m_display_string_chars(unsigned background,int last_visible_char);

};




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : TDECEDZN.H                           Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* TDECEDZN                                                                 */
/*--------------------------------------------------------------------------*/
/* Objet TDecimalEditZone (Zone d'dition d'un nombre dcimal)              */
/****************************************************************************/


#include <values.h>



class TDecimalEditZone:public TEditZone
{
  /*---------------*/
  /* Champs privs */
  /*---------------*/

    // Valeurs limites autorises

    double  f_min_value,
	    f_max_value;

  /*--------------------*/
  /* Mthodes publiques */
  /*--------------------*/

  public:

    // Constructeur / Destructeur

		    TDecimalEditZone(PObject parent,
				     int rel_x, int rel_y,
				     int xoffset_caption, int yoffset_caption,
				     const char *caption,
				     int display_length,
				     int max_length,
				     double min_value=-MAXDOUBLE,double max_value=MAXDOUBLE,
				     const char *string = "0",
				     bool enabled=true);

    virtual         ~TDecimalEditZone();

            void    m_set_min_max_values(double min_value,double max_value);

            double  m_get_min_value() {return(f_min_value);};
            double  m_get_max_value() {return(f_min_value);};

    // Validit d'une chane pour la zone

    virtual bool m_string_valid(const char *string);

  /*------------------*/
  /* Mthodes prives */
  /*------------------*/

  private:

    // Validit de la saisie

    virtual bool m_character_valid(int character);
    virtual const char *m_get_error_message();

};




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : TINTEDZN.H                           Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* TINTEDZN                                                                 */
/*--------------------------------------------------------------------------*/
/* Objet TIntegerEditZone (Zone d'dition d'un nombre entier)               */
/****************************************************************************/


#include <values.h>


class TIntegerEditZone:public TEditZone
{
  /*---------------*/
  /* Champs privs */
  /*---------------*/

   // Valeurs limites autorises

    long    f_min_value,
	    f_max_value;

  /*--------------------*/
  /* Mthodes publiques */
  /*--------------------*/

  public:

    // Constructeur / Destructeur

		    TIntegerEditZone(PObject parent,
				     int rel_x, int rel_y,
				     int xoffset_caption, int yoffset_caption,
				     const char *caption,
				     int display_length,
				     int max_length,
				     long min_value=-MAXLONG-1,long max_value=MAXLONG,
				     const char *string="0",
				     bool enabled=true);


    virtual         ~TIntegerEditZone();

            void    m_set_min_max_values(long min_value,long max_value);

            long    m_get_min_value() {return(f_min_value);};
            long    m_get_max_value() {return(f_min_value);};


    // Validit d'une chane pour la zone

    virtual bool m_string_valid(const char *string);

  /*------------------*/
  /* Mthodes prives */
  /*------------------*/

  private:

    // Validit de la saisie

  private:
    virtual bool m_character_valid(int character);
    virtual const char *m_get_error_message();

};




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : TLABEL.H                             Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* TLABEL                                                                   */
/*--------------------------------------------------------------------------*/
/* Objet TLabel (Etiquette)                                                 */
/****************************************************************************/




class TLabel:public TObject
{
  /*---------------*/
  /* Champs privs */
  /*---------------*/

  private:

    char *f_text;   // Texte … afficher

  /*--------------------*/
  /* Mthodes publiques */
  /*--------------------*/

    // Constructeur / Destructeur

  public:
		    TLabel(PObject parent,
			   int rel_x,int rel_y,
			   const char *text);

		    TLabel(PObject parent,
			   int rel_x,int rel_y,
			   int width,int height,
			   const char *text="");

    virtual	    ~TLabel();

    // Mthodes d'accs

            void    m_set_text(const char *text);
	    char    *m_get_text() const {return(f_text);};

  /*--------------------*/
  /* Mthodes protges */
  /*--------------------*/

  protected:

    // Affichage

    virtual void    m_display();

  /*------------------*/
  /* Mthodes prives */
  /*------------------*/

  private:

};




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : TSCRBAR.H                            Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* TSCRBAR                                                                  */
/*--------------------------------------------------------------------------*/
/* Objet TScrollBar (Barre de dplacement)                                  */
/****************************************************************************/



#define SCROLLBAR_FIRST_SCROLL_SPEED  150
#define SCROLLBAR_SCROLL_SPEED        70

typedef enum {
	       SB_HORIZONTAL,
	       SB_VERTICAL }
	     TSbOrientation;

class TScrollBar:public TObject
{
  /*-----------------*/
  /* Champs protgs */
  /*-----------------*/

  protected:

    // Orientation de l'ascenseur

    TSbOrientation
            f_orientation;

    // Valeur courante, et valeurs minimale et maximale de la ScrollBar

    long    f_value;
    long    f_min_value;
    long    f_max_value;


    // Pour viter des calculs :
    // -> diffrence entre f_max_value et f_min_value
    // -> taille de la cage d'ascenseur moins celle de l'ascenseur

    long    f_values_range;
    int     f_empty_size;

    // Incrment de f_value
    // -> pour un petit changement (clic sur une flche)
    // -> pour un grand changement (clic dans la cage, … c“t de l'ascenseur)

    long    f_little_change;
    long    f_big_change;

  /*---------------*/
  /* Champs privs */
  /*---------------*/

  private:

    // Objet associ … l'ascenseur

    PObject f_related_object;

  /*--------------------*/
  /* Mthodes publiques */
  /*--------------------*/

  public:

    // Constructeur / Destructeur

		    TScrollBar(PObject parent,
                               TSbOrientation orientation,
			       int rel_x,int rel_y,
			       int length,
                               bool enabled=true);

    virtual         ~TScrollBar();

    // Mthodes d'accs

	    void    m_set_value(long value);
	    long    m_get_value() {return(f_value);};

	    void    m_set_min_max_values(long min_value,long max_value);
	    long    m_get_min_value() {return(f_min_value);};
	    long    m_get_max_value() {return(f_max_value);};

	    void    m_set_little_change(long little_change) { f_little_change=little_change;}
	    void    m_set_big_change(long big_change) { f_big_change   =big_change;}

	    // Callback : Fonction appele quand la valeur de l'ascenseur change
    	//            et son argument

		jptui::CallbackHolder	valueChangedAction_;

  /*--------------------*/
  /* Mthodes protges */
  /*--------------------*/

  protected:

    // Affichage

    virtual void    m_display();

    // Prise/perte du focus

    virtual void    m_take_focus();

    // Conversions valeur <-> position cran

	    int     m_value_to_position(long value);
	    long    m_position_to_value(int position);

  /*------------------*/
  /* Mthodes prives */
  /*------------------*/

  private:

    // Affichage

    virtual void    m_display_slider();
    virtual void    m_display_arrows();

    // Evnements

    virtual bool m_left_button_pressed_event(int x,int y);
    virtual bool m_arrow_pressed_event(int x,int y,int direction);
    virtual bool m_shaft_pressed_event(int x,int y,int direction);

};




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : TGAUGE.H                             Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* TGAUGE                                                                   */
/*--------------------------------------------------------------------------*/
/* Objet TGauge (jauge)                                                     */
/****************************************************************************/



typedef enum {GAUGE_PERCENT_STYLE,
              GAUGE_OUT_OF_STYLE}
  TGaugeStyle;

class TGauge:public TObject
{
  /*------------------*/
  /* Protected fields */
  /*------------------*/

  protected:

    long    f_max_value;
    TGaugeStyle  f_style;

  /*----------------*/
  /* Private fields */
  /*----------------*/

  private:

    long    f_value;


    int     f_old_text_length;

    unsigned  bright_gauge_attr;
    unsigned  normal_gauge_attr;
	

  /*--------------------*/
  /* Mthodes publiques */
  /*--------------------*/

    // Constructeur / Destructeur

  public:
		    TGauge(PObject parent,
			   int rel_x,int rel_y,
                           int width,
			   char *caption,
                           TGaugeStyle style=GAUGE_PERCENT_STYLE,
                           long max_value=100);

    virtual         ~TGauge();

    // Mthodes d'accs

    // Mthodes d'accs

    	    void    m_set_value(long value);
	    long    m_get_value() {return(f_value);};

	    void    m_set_max_value(long max_value);
	    long    m_get_max_value() {return(f_max_value);};

	    void    set_gauge_attributes(unsigned bright, unsigned normal)
            {
              bright_gauge_attr = bright; normal_gauge_attr = normal;
	      m_display_gauge();
            };


  /*--------------------*/
  /* Mthodes protges */
  /*--------------------*/

  protected:

  // Affichage

    virtual void    m_display();

  /*------------------*/
  /* Mthodes prives */
  /*------------------*/

  private:

  // Affichage

    virtual void    m_display_caption();
    virtual void    m_display_gauge();
};





/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : TLIST.H                              Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* TLIST                                                                    */
/*--------------------------------------------------------------------------*/
/* Objet TList (objet gnrique pour les objets reposant sur une liste de   */
/*              chanes)                                                    */
/****************************************************************************/



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                               CONSTANTES                               บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

// Vitesse de dfilement dans la liste

#define LIST_FIRST_SCROLL_SPEED   150
#define LIST_SCROLL_SPEED         70

// Sparateur

#define SEPARATOR           "ฤ"

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                                 TYPES                                  บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

// Objet pour les informations complmentaires sur les lments de la liste

class TListItemProperties
{
  public:

	     TListItemProperties();
    virtual ~TListItemProperties();
};

typedef TListItemProperties    *PListItemProperties;

// Type Liste double-chane d'lments de la liste

typedef struct item_node
{
  char               *label;
  int      	      attribute;
  char                hot_key;

  PListItemProperties properties;  // Peut tre NULL

  struct item_node *next,*last;
} TItemNode,*PItemNode;

// Un lment de la liste :
// -> string : chane de caractres ou SEPARATOR pour un sparateur
// -> item_attribute : attribut de l'lment
//    (LI_CHECKED|LI_DISABLED...)
// -> la hot key de l'lment
// -> le shortcut de l'lment
// -> un message d'aide qui s'affiche dans la barre d'information
//    de la fentre quand l'lment est slectionn

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                         DEFINITION DE L'OBJET                          บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/


class TList : public TObject, public jptui::Callback
{
  /*-----------------*/
  /* Champs protgs */
  /*-----------------*/

  protected:

    // Liste double-chane des chanes et son nombre d'lments

    PItemNode
	    f_item_list;
    int     f_nb_items;

    // Les lments de la liste sont tris par ordre alphabtique ou non

    bool f_sorted;

    // Attributs autoriss pour les lments de la liste

    int     f_allowed_item_attribute;

    // Si possible, il y a toujours un lment de slectionn

    bool f_always_one_item_selected;

    // Les hot-keys des lments de la liste sont actives

    bool f_item_hot_key_enabled;


    // Elments spciaux de la liste

    PItemNode
	    f_first_visible_item;       // Premier lment visible
    int     f_first_visible_item_index; // Son index (0 si aucun)

    PItemNode
	    f_last_item;                // Dernier lment de la liste
					// Son index vaut f_nb_strings

    int     f_selected_item_index;      // Elment de la liste slectionne
					// Son index (0 si aucun)

    PItemNode
            f_current_item;             // Un lment de la liste qui vient
    int     f_current_item_index;       // d'tre consult et son index
					// (sert pour acclerer la lecture
    // Position et hauteur de la liste (sans les bordures)

    int     f_list_rel_x,
	    f_list_rel_y;
    int     f_list_width,
	    f_list_height;

    // Ascenseur

    PScrollBar
	    f_scrollbar;

    // Chaine vide (affiche en cas d'absence d'lments)

    char     *f_empty_line;

    // Ligne sparatrice

    char     *f_separator_line;

  /*---------------*/
  /* Champs privs */
  /*---------------*/

    // Elment retourn par m_get_item

    char     *f_return_item;

  /*--------------------*/
  /* Mthodes publiques */
  /*--------------------*/

  public:

    // Constructeur / Destructeur

		    TList(PObject parent,
			  int type,
			  int rel_x,int rel_y,
			  int width,int height,
			  unsigned background,
			  const char *caption,
			  int  list_rel_x,int list_rel_y,
			  int  list_width,
			  int  list_height,
			  bool sorted,
			  int     allowed_item_attribute,
			  bool always_one_item_selected,
			  bool item_hot_key_enabled,
			  bool scrollbar,
			  int     scrollbar_x,
			  int     scrollbar_y,
			  int     scrollbar_height,
			  bool enabled);

    virtual         ~TList();

    // Mthodes d'accs

	    int     m_get_nb_items() { return (f_nb_items); };

	    char    *m_get_item(int  index);       // "" si index inexistant
	    int     m_get_item_index(const char *label); // 0 si inexistant

	    bool m_set_selected_item_index(int  index);
	    void    m_select_first_possible_item();
	    int     m_get_selected_item_index() { return (f_selected_item_index);};

	    void    m_set_first_visible_item_index(int  index);
	    int     m_get_first_visible_item_index() { return (f_first_visible_item_index);};

	    void    m_set_middle_visible_item_index(int  index);

	    void    m_set_last_visible_item_index(int  index);
	    int     m_get_last_visible_item_index();

	    int     m_get_list_height() {return(f_list_height);};
	    int     m_get_list_width()  {return(f_list_width);};
	    int     m_get_x_list()      {return(m_get_x()+f_list_rel_x);};
	    int     m_get_y_list()      {return(m_get_y()+f_list_rel_y);};

    // Callback : Fonction appele si l'lment slectionn change
    //            et son argument
	jptui::CallbackHolder	selectedItemChangedAction_;

    // Callback : Fonction appele si la partie visible de la liste
    //            change et son argument
	jptui::CallbackHolder	visiblePartChangedAction_;

    // Callback : Fonction appele si le nombre d'lments dans la liste
    //            change et son argument
	jptui::CallbackHolder	nbItemsChangedAction_;

    // Callback : Fonction appele si on coche un lment
    //            et son argument
	jptui::CallbackHolder	itemCheckChangedAction_;

    // Callback : Fonction appele si on a double-cliqu dans
    //            un lment de la liste et son argument
	jptui::CallbackHolder	itemDblClickAction_;

	    // Suppression d'lments de la liste

    virtual void    m_delete_item(int item_index);
    virtual void    m_clear_list();

    // Modification de l'attribut d'un item
    // (on peut fournir des combinaisons : LI_CHECKED|LI_DISABLED)

	    void    m_set_item_attribute(int item_index,int item_attribute);
	    void    m_unset_item_attribute(int item_index,int item_attribute);
	    bool m_item_attribute_is_set(int item_index,int item_attribute);

	    void    m_check_item(int item_index)      {m_set_item_attribute(item_index,LI_CHECKED);};
	    void    m_uncheck_item(int item_index)    {m_unset_item_attribute(item_index,LI_CHECKED);};
	    bool m_item_is_checked(int  item_index){return(m_item_attribute_is_set(item_index,LI_CHECKED));};

	    void    m_enable_item(int item_index)    {m_unset_item_attribute(item_index,LI_DISABLED);};
	    void    m_disable_item(int item_index)   {m_set_item_attribute(item_index,LI_DISABLED);};
	bool	m_item_is_enabled(int item_index) {
		return !m_item_attribute_is_set(item_index, LI_DISABLED);
	};

  /*--------------------*/
  /* Mthodes protges */
  /*--------------------*/

  protected:

    // Ajout d'lment … la liste
    // (retourne son index ou 0 s'il n'y a plus de place)

    virtual int     m_add_item_to_list(int index, const char *label, int attribute,PListItemProperties properties);

    // Affichage

    virtual void    m_display();
    virtual void    m_display_focus_depending_part();
    virtual void    m_display_list();
    virtual void    m_display_items(int first_index,int last_index);
    virtual void    m_display_item(int  item_index);

    // Modification de la taille de la liste

	    void    m_set_list_size(int list_width,int list_height);

	    int     m_get_list_needed_width();

    // Callbacks

    virtual void    m_selected_item_changed_callback();
    virtual void    m_visible_part_changed_callback();
    virtual void    m_nb_items_changed_callback();
    virtual void    m_item_dbl_clicked_callback(int index);

	void	operator()(TObject* who, what_type what, item_type item);

    virtual void    m_mouse_went_to_list_callback();

    // Evnements

    virtual bool m_left_button_pressed_event(int x,int y);
    virtual bool m_left_button_double_click_event(int x,int y);
    virtual bool m_key_pressed_event(TKey key);

    // Obtention d'un lment d'aprs son index

	    PItemNode
		    m_index_to_item(int index);


  /*------------------*/
  /* Mthodes prives */
  /*------------------*/

  private:

  // Affichage

    virtual void    m_display_item_node(PItemNode node,int index);
    virtual void    m_display_item_node_label(PItemNode node,bool show_hot_key);

	    void    m_change_item_attribute(PItemNode item_node,int item_index,int new_attribute);

  // Evnements

    virtual bool m_leave_event_if_mouse_leave_list(int x,int y);
    virtual void    m_item_hot_key_pressed_event(int item_index);


};





/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : TSMPLIST.H                           Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* TSMPLIST                                                                 */
/*--------------------------------------------------------------------------*/
/* Objet TSimpleList (Liste dont les lments n'ont pas de proprits       */
/*                    supplmentaires)                                      */
/****************************************************************************/


#include <stdlib.h>


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                                  TYPES                                 บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

// Elment de la liste

typedef struct {
  const char *label;
  int attribute;
} TListItem;

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                         DEFINITION DE L'OBJET                          บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

class TSimpleList:public TList
{
  /*--------------*/
  /* For TUIMaker */
  /*--------------*/

  #ifdef __TUIMAKER__
  friend class VComboBox;
  #endif


  /*--------------------*/
  /* Mthodes publiques */
  /*--------------------*/

  public:

    // Constructeur/Destructeur

		    TSimpleList(PObject parent,
				int type,
				int rel_x, int rel_y,
				int width, int height,
				unsigned background,
				const char *caption,
				int  list_rel_x,int list_rel_y,
				int  list_width,
				int  list_height,
				bool sorted,
				int     allowed_item_attribute,
				bool always_one_item_selected,
				bool item_hot_key_enabled,
				bool scrollbar,
				int     scrollbar_x,
				int     scrollbar_y,
				int     scrollbar_height,
				bool enabled);

    virtual         ~TSimpleList();


    // Ajout d'un lment … la liste
    // (retourne son index ou 0 s'il n'y a plus de place)

    virtual int     m_add_item(const char *label, int attribute=LI_NOATTR)             {return(TList::m_add_item_to_list(0,label,attribute,NULL));};
    virtual int     m_insert_item(int index, const char *label, int attribute=LI_NOATTR){return(TList::m_add_item_to_list(index,label,attribute,NULL));};

    // Ajout de plusieurs lments
    // Chaque lment est de type TListItem
    // Dans le dernier lment, item[].label doit valoir NULL

    // Retourne le nombre d'lments effectivement ajout

    virtual int     m_add_items(const TListItem items[]);


};




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : TLISTBOX.H                           Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* TLISTBOX                                                                 */
/*--------------------------------------------------------------------------*/
/* Objet TListBox (liste avec ascenseur)                                    */
/****************************************************************************/


#include <stdlib.h>


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                         DEFINITION DE L'OBJET                          บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

class TListBox:public TSimpleList
{
  /*--------------------*/
  /* Mthodes publiques */
  /*--------------------*/

  public:

    // Constructeur/Destructeur

		    TListBox(PObject parent,
			     int rel_x, int rel_y,
			     int width, int height,
			     const char *caption = "",
			     const TListItem items[]=NULL,
			     bool sorted=false,
			     bool enabled=true);
    virtual         ~TListBox();



  /*--------------------*/
  /* Mthodes protges */
  /*--------------------*/

  protected:

    // Affichage

    virtual void    m_display();
    virtual void    m_display_focus_depending_part();
    virtual void    m_display_caption();

    // Evnements

    virtual bool m_key_pressed_event(TKey key);

    // Callbacks

    virtual void    m_item_dbl_clicked_callback(int index);
};





/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : TTABGRP.H                            Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* TTABGRP                                                                  */
/*--------------------------------------------------------------------------*/
/* Objet TTabGroup (Groupe d'onglets)                                       */
/****************************************************************************/







class TTabGroup:public TGroup
{
  /*---------------*/
  /* Classes amies */
  /*---------------*/

  friend class TTab;

  /*---------------*/
  /* Champs privs */
  /*---------------*/

  private:

    // Nombre d'onglets en largeur

    int     f_nb_tabs_width;

    // Onglet visible

    PObjectNode
            f_visible_tab;

  /*-----------------*/
  /* Champs protgs */
  /*-----------------*/

  protected:


  /*--------------------*/
  /* Mthodes publiques */
  /*--------------------*/

  public:

    // Constructeur / Destructeur

		    TTabGroup(PObject parent,
			      int rel_x,int rel_y,
			      int width,int height,
			      unsigned background,
                              bool enabled=true);
    virtual         ~TTabGroup();

    // Mthodes d'accs

    // Donne le focus au groupe (au 1er/dernier de ses objets qui le veut)

    virtual bool m_set_focus();
    virtual bool m_set_focus_to_last_element();


  /*--------------------*/
  /* Mthodes protges */
  /*--------------------*/

  protected:


    // Ajout d'un onglet

    virtual int     m_add_element(PObject object);
    virtual void    m_del_element(int object_number);

    // Ouverture/Fermeture du groupe

    virtual void    m_set_open(bool open);


    // Affichage

    virtual void    m_display();

    // Evnements

    virtual bool m_left_button_pressed_event(int x,int y);
    virtual bool m_left_button_double_click_event(int x,int y);
    virtual bool m_key_pressed_event(TKey key);

  /*------------------*/
  /* Mthodes prives */
  /*------------------*/

  private:

    // Affichage

    virtual void    m_display_focus_depending_part();
            void    m_display_tab_caption(int tab_nb);

    // Donne le dbut et la largeur d'un des titres des onglets

            void    m_get_tab_caption_pos(int tab_nb,int &rel_x1,int &rel_x2);


    // Evnements

    virtual bool m_tab_pressed_event() { return false; };
    virtual bool m_shift_tab_pressed_event(){ return false; };

	    void    m_set_visible_tab(int tab_number);

    // L'objet peut obtenir le focus lors d'un dplacement
    // avec les touches flches

    virtual bool m_focus_can_be_set_by_arrow_key() { return true; };
};




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : TTAB.H                               Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* TTAB                                                                     */
/*--------------------------------------------------------------------------*/
/* Objet TTab (onglet)                                                      */
/****************************************************************************/




class TTab:public TGroup
{
  /*---------------*/
  /* Classes amies */
  /*---------------*/

  friend class TTabGroup;

  /*--------------------*/
  /* Mthodes publiques */
  /*--------------------*/

  public:

    // Constructeur / Destructeur

		    TTab(PTabGroup tab_group,
			 const char *caption);
    virtual  	    ~TTab();

    // Mthodes d'accs

    // Rend cet onglet visible

            void    m_set_visible();

    // Callback : Fonction appele lorsque l'onglet devient visible
    //            et son argument
	jptui::CallbackHolder	visibleAction_;

  /*--------------------*/
  /* Mthodes protges */
  /*--------------------*/

  protected:


    // Affichage

    virtual void    m_display();
    virtual void    m_display_focus_depending_part();
    virtual void    m_display_caption();

    // Prise/Perte du focus

    virtual void    m_take_focus();

  /*------------------*/
  /* Mthodes prives */
  /*------------------*/

    // L'objet peut obtenir le focus lors d'un dplacement
    // avec les touches flches

    virtual bool m_focus_can_be_set_by_arrow_key() { return true; };
};





/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : TTEXTZN.H                            Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* TTEXTZN                                                                  */
/*--------------------------------------------------------------------------*/
/* Objet TTextZone (Zone d'dition de texte)                                */
/****************************************************************************/




// Vitesse de dfilement du texte

#define TEXTZONE_FIRST_SCROLL_SPEED   150
#define TEXTZONE_SCROLL_SPEED         70

// Objet pour les informations complmentaires sur les lignes du texte

class TLineProperties
{
  public:

	     TLineProperties();
    virtual ~TLineProperties();
};

typedef TLineProperties    *PLineProperties;


// Type TText (Liste double-chane de chaines)

typedef struct string_node
{
  char               *string;
  PLineProperties    properties;  // Peut tre NULL

  struct string_node *next,*last;
} TStringNode,*PStringNode,*PText;


class TTextZone : public TObject, public jptui::Callback
{
  /*-----------------*/
  /* Champs protgs */
  /*-----------------*/

  protected:

    // Position /Dimensions de la zone de texte elle-mme p/r … l'objet

    int     f_text_rel_x,
	    f_text_rel_y;
    int     f_text_width,
	    f_text_height;

    // Nombre maximal de colonnes du texte
    // Nombre de lignes actuel du texte

    int     f_nb_cols;
    int     f_nb_lines;

    // Tab size (when using m_insert_line, m_set_line...)

    int     f_tab_size;

    // Modification du texte autorise

	bool	f_modification_enabled;

    // Texte contenue dans la TextBox

    PText   f_text;

    // Elments spciaux du texte

    PStringNode
            f_first_visible_string;     // Premire chane visible

    PStringNode
            f_last_string;              // Dernier lment de la liste

    PStringNode
            f_current_string;           // Une chane du texte qui vient
    int     f_current_string_line;      // d'tre utilise et sa ligne
                                        // (sert pour acclerer la lecture

    // Mode insertion/recouvrement
    // (champ commun … toutes les instances de l'objet)

//    static
    int     f_cursor_style;

    // Premire colonne,ligne visible

    int     f_first_visible_col,
	    f_first_visible_line;

    // Une zone est slectionne ?

	bool	f_zone_selected;

    // Si oui :
    // -> caractres de position min et max de la zone slectionne
    // Si f_min_selected_col vaut f_nb_cols+1,
    // seul le saut de ligne de f_min_selected_line est compris
    // Si f_max_selected_col vaut 0,
    // cela signifie que la zone slectionne comprend le saut de ligne de
    // la ligne prcdente  (f_max_selected_line-1)

    int     f_min_selected_line,
	    f_min_selected_col;

    int     f_max_selected_line,
	    f_max_selected_col;

    // En cours de slection :
    // -> premier et dernier caractres slectionnes
    //    (par ordre de slection)

    int     f_first_selected_col,
	    f_last_selected_col;

    int     f_first_selected_line,
	    f_last_selected_line;

    // Position du cursor

    int     f_cursor_col,
	    f_cursor_line;

    // Ascenseurs

    PScrollBar
	    f_vertical_scrollbar,
	    f_horizontal_scrollbar;

    // Chane retourne par m_get_line

    char    *f_return_string;

    // Buffer pour la zone retourne par m_get_zone()

    char    *f_zone_buffer;


public:

    // Constructeur / Destructeur

		    TTextZone(PObject parent,
		             int rel_x, int rel_y,
			     int width, int height,
			     const char *caption,
			     int max_text_nb_cols,
			     TScrollbarChoice scrollbars=SBC_VERTICAL,
			     bool enabled=true);
    virtual         ~TTextZone();

//	int		cursorStyle() const;
	void	setCursorStyle(int);

    // Mthodes d'accs

	    void    m_set_cursor_at(int col,int line,bool make_cursor_visible=true);
            void    m_get_cursor_pos(int &col,int &line);

	    int     m_get_nb_lines() { return(f_nb_lines);};
            int     m_get_nb_cols()  { return(f_nb_cols);};

	    int     m_get_text_rel_x() { return(f_text_rel_x);};
	    int     m_get_text_rel_y() { return(f_text_rel_y);};
	    int     m_get_text_width() { return(f_text_width);};
	    int     m_get_text_height() { return(f_text_height);};

	    void    m_set_tab_size(int tab_size);
	    int     m_get_tab_size() { return(f_tab_size);};

	    void    m_set_first_visible_col(int col);
	    void    m_set_first_visible_line(int line);

	    int     m_get_first_visible_col() {return(f_first_visible_col);};
	    int     m_get_first_visible_line() {return(f_first_visible_line);};

	    int     m_line_length(int line_nb);

    virtual void    m_enable_modification()   {f_modification_enabled=true;};
    virtual void    m_disable_modification()  {f_modification_enabled=false;};


    // Effacement de texte
    // - m_delete_zone : first_col peut valoir f_nb_cols+1 (… partir saut de
    //                   ligne) et last_col 0 (jusqu'au saut de ligne)

            void    m_clear_text();
            void    m_delete_line(int line_nb);
            void    m_clear_zone(int first_col,int first_line,int last_col,int last_line);

    // Modification du texte

	    void    m_set_line(int line_nb, const char *string)    {m_set_line(line_nb,string,NULL);};
	    void    m_insert_line(int line_nb, const char *string) {m_insert_line(line_nb,string,NULL);};
	    void    m_insert_text(const char *text);


    // Lecture de texte
    // - m_get_zone : first_col peut valoir f_nb_cols+1 et last_col 0
    //                Appeler m_free_zone quand on n'a plus besoin de la zone
    //                retourne
    // - m_free_zone_buffer libre le buffer allou par m_get_zone
    // - La chane retourne par m_get_line peut tre modifie sans
    //   consquence sur le texte

            char    *m_get_zone(int first_col,int first_line,int last_col,int last_line);
	    void    m_free_zone_buffer();
            char   *m_get_line(int line_nb);

    // Chargement/Sauvegarde du texte (retourne false si chec)

            bool	m_save_text(const char *filename);
            bool	m_load_text(const char *filename);

    // Modification de la partie slectionne du texte

	    void    m_select_zone(int pos1_col,int pos1_line,int pos2_col,int pos2_line);
	    void    m_unselect_zone();

	    // m_get_selected_zone n'a de sens que si m_zone_selected() est
	    // true

	    bool	m_zone_selected() { return(f_zone_selected); }
	    void    m_get_selected_zone(int &min_col,int &min_line,int &max_col,int &max_line);

    // Oprations sur la partie slectionne du texte

	    void    m_cut_selection();
	    void    m_copy_selection();
            void    m_paste();
	    void    m_clear_selection();

    // Retourne un pointeur sur une des lignes du texte
    // La ligne doit exister

	    PStringNode
		    m_line_to_string(int  line);

    // Callback : Fonction appele si le nombre de lignes dans le texte
    //            change et son argument

	jptui::CallbackHolder	nbLinesChangedAction_;

    // Callback : Fonction appele … chaque modification de la chane
    //            et son argument

	jptui::CallbackHolder	textChangedAction_;

    // Callback : Fonction appele si la position du curseur change
    //            et son argument

	jptui::CallbackHolder	cursorLineChangedAction_;
	jptui::CallbackHolder	cursorColChangedAction_;
	jptui::CallbackHolder	cursorPosChangedAction_;

    // Callback : Fonction appele si la partie visible (en hauteur)
    //            de la liste change et son argument

	jptui::CallbackHolder	visibleLinesChangedAction_;
	jptui::CallbackHolder	visibleColsChangedAction_;

    // Callback : Fonction appele s'il y a/n'y a plus de zone slectionne

	jptui::CallbackHolder	selectionStatusChangedAction_;

  /*--------------------*/
  /* Mthodes protges */
  /*--------------------*/

  protected:


	    void    m_set_line(int line_nb, const char *string, PLineProperties properties);
	    void    m_insert_line(int line_nb, const char *string, PLineProperties properties);

    // Affichage

    virtual void    m_display();
    virtual void    m_display_focus_depending_part();

	    void    m_display_lines(int first_line,int last_line);

    // Prise/Perte du focus

    virtual void    m_lose_focus();
    virtual void    m_take_focus();

    // L'objet devient actif/inactif

    virtual void    m_set_active(bool active);

    // Ouverture/Fermeture de l'objet

    virtual void    m_set_open(bool open);

    // Affichage

    virtual void    m_display_string_node(PStringNode node,int line);
    virtual void    m_display_string_part(const char *string, int string_length, int min_col, int max_col, unsigned attribute);


    // Selected part of a line (0,0 if none)

	    void	m_get_line_selected_part(int line,int &min_selected_col,int &max_selected_col);

	// Appel des callbacks
	void	operator()(TObject* who, what_type what, item_type item);

    virtual void    m_nb_lines_changed_callback();
    virtual void    m_text_changed_callback();
    virtual void    m_cursor_line_changed_callback();
    virtual void    m_cursor_col_changed_callback();
    virtual void    m_cursor_pos_changed_callback();
    virtual void    m_visible_lines_changed_callback();
    virtual void    m_visible_cols_changed_callback();
    virtual void    m_selection_status_changed_callback();

    // Evnements

    virtual bool	m_left_button_pressed_event(int x,int y);
    virtual bool	m_left_button_double_click_event(int x,int y);
    virtual bool	m_key_pressed_event(TKey key);

    virtual void    m_mouse_move_button_pressed(int x,int y,bool selection);

  /*------------------*/
  /* Mthodes prives */
  /*------------------*/

  private:

    // Affichage

    virtual void    m_display_caption();

	    void    m_display_text();

    // Caractres saisis

            void    m_character_hit(int character);

	    void    m_move_cursor_with_key_to(bool shift_pressed,int new_cursor_col,int new_cursor_line);

	    void    m_ctrl_y_hit();

	    void    m_left_hit(bool shift_pressed);
	    void    m_right_hit(bool shift_pressed);
	    void    m_up_hit(bool shift_pressed);
	    void    m_down_hit(bool shift_pressed);
	    void    m_ctrl_left_hit(bool shift_pressed);
	    void    m_ctrl_right_hit(bool shift_pressed);

	    void    m_home_hit(bool shift_pressed);
	    void    m_end_hit(bool shift_pressed);
	    void    m_ctrl_home_hit(bool shift_pressed);
	    void    m_ctrl_end_hit(bool shift_pressed);

	    void    m_page_up_hit(bool shift_pressed);
	    void    m_page_down_hit(bool shift_pressed);
	    void    m_ctrl_page_up_hit(bool shift_pressed);
	    void    m_ctrl_page_down_hit(bool shift_pressed);

            void    m_return_hit();
            void    m_backspace_hit();
            void    m_tab_hit();

	    void    m_insert_hit(bool shift_pressed);
	    void    m_ctrl_insert_hit();

	    void    m_delete_hit(bool shift_pressed);

    // Longueur utile d'une chaine

	    int m_useful_length(char *string);

    // Rduction/Expansion d'une ligne
    // (une ligne expanse a une longueur de f_nb_cols caractres,
    // alors qu'une ligne rduite ne comprend pas les espaces de fin de ligne)
    // Une ligne n'est expanse que quand la zone texte a le focus, et si
    // le curseur est situ sur cette ligne

            void    m_reduce_line(int line_nb);
            void    m_expand_line(int line_nb);

    // Modification de la position du curseur

            void    m_set_cursor_position(int col,int line,bool make_cursor_visible);

    // Taille du buffer ncessaire pour stocker une zone de texte

	    unsigned
		    m_get_zone_size(int first_col,int first_line,int last_col,int last_line);

    // Correction des limites d'une zone

            bool m_correct_zone_limits(int &first_col,int &first_line,int &last_col,int &last_line);

    // Recherche le prcdent alignement de texte dans les lignes prcdant
    // le curseur (dplacement suite … un backspace...)

            int     m_get_previous_tab(int max_value);

    // Insertion de n caractres identiques dans le texte

            void    m_insert_nch(int nb_chars,char character);

};





/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : TMENUBAR.H                           Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* TMENUBAR                                                                 */
/*--------------------------------------------------------------------------*/
/* Objet TMenuBar (barre de menus)                                          */
/****************************************************************************/





// Liste chane de menus

typedef struct menu_node
{
  PMenu                    menu;
  struct menu_node        *next;
} TMenuNode,*PMenuNode;


class TMenuBar:public TObject
{
  /*-------------*/
  /* Objets amis */
  /*-------------*/

  friend class TMenu;
  friend class TWindow;

  /*---------------*/
  /* Champs privs */
  /*---------------*/

  private:

    // Menus de l'objet

    int     f_nb_menus;
    PMenuNode
            f_menu_list,
            f_last_menu;

    // La barre de menu est en cours d'utilisation

    bool f_active;

    // Menu actif

    PMenu   f_active_menu;

    // Le curseur tait visible lors de l'ouverture du menu

    bool f_cursor_was_visible;

    // Message d'info de la fenetre avant d'ouvrir le menu */

    char    *f_window_info_message;

  /*-----------------*/
  /* Champs protgs */
  /*-----------------*/

  protected:

  /*--------------------*/
  /* Mthodes publiques */
  /*--------------------*/

  public:

    // Constructeur / Destructeur

		    TMenuBar(PWindow window);
    virtual 	   ~TMenuBar();

    // Mthodes d'accs

    virtual void    m_enable();
    virtual void    m_disable();

    // Donne le focus … la barre de menu

    virtual bool m_set_focus()                  { return false; };
    virtual bool m_set_focus_to_last_element()  { return false; };


  /*--------------------*/
  /* Mthodes protges */
  /*--------------------*/

  protected:

    // Ajout d'un menu

    virtual int     m_add_menu(PMenu menu);

    // Affichage

    virtual void    m_display();

    // Ouverture/Fermeture de l'objet

    virtual void    m_set_open(bool open);

    // Evnements

    virtual bool m_left_button_pressed_event(int x,int y);
    virtual bool m_key_pressed_event(TKey key);
    virtual bool m_short_cut_pressed_event(int shortcut);


  /*------------------*/
  /* Mthodes prives */
  /*------------------*/

  private:

    // Affichage

            void    m_display_menu_caption(int menu_nb);

    // Ouverture/Fermeture d'un menu

            bool m_open_menu(int menu_nb,bool select_first_possible);
            void    m_close_menu();

    // Donne le dbut et la largeur d'un des titres des onglets

            int     m_get_menu_caption_pos1(int menu_nb);
            void    m_get_menu_caption_pos(int menu_nb,int &rel_x1,int &rel_x2);

            void    m_activate_menu_bar();
            void    m_inactivate_menu_bar();

            PMenu   m_menu_number_to_menu(int menu_number);
            PMenuNode
                    m_menu_number_to_menu_node(int menu_number);

};




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : TMENU.H                              Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* TMENU                                                                    */
/*--------------------------------------------------------------------------*/
/* TMenu class (menus)                                                      */
/****************************************************************************/





/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                                  TYPES                                 บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

// Elment du menu

typedef struct {
  const char *label;
  int attribute;
  int short_cut;
  const char *help_message;
} TMenuItem;


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                             CLASS DEFINITION                           บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/


class TMenu:public TList
{
  /*----------------*/
  /* Friend classes */
  /*----------------*/

  friend class TMenuBar;

  /*----------------*/
  /* Private fields */
  /*----------------*/

  private:

    // Menu bar

    PMenuBar            f_menu_bar;

    // Menu number (!= object number)

    int     	        f_menu_nb;

    // The size of the menu needs to be calculated again

    bool 		f_size_to_adjust;

    // Width needed to display shortcuts
    // (only significant if  f_size_to_adjust==false)

    int     		f_short_cut_width;

  /*----------------*/
  /* Public methods */
  /*----------------*/

  public:

    // Constructor / Destructor

    // See TList.h for TMenuItem (m_add_items)

			TMenu(PMenuBar menu_bar,
			      const char *caption,
			      const TMenuItem items[]=NULL,
			      bool enabled=true);
    virtual             ~TMenu();

    // Returns the menu-bar of the menu

	    PMenuBar    m_get_menu_bar() { return(f_menu_bar);};

    // Disables the menu

    virtual void        m_disable();

    // Opens the menu. Returns false if it can't be open

	    bool     m_open();

    // Closes the menu

	    void        m_close();

    // Clicks an item as if the user had clicked it with the mouse,
    // pressed its hotkey...

	    void        m_click_item(int item_index);

    // Add a item to the menu
    // Returns its index or 0 if it couldn't have been added

    virtual int         m_add_item(const char *label, int attribute=LI_NOATTR,
                                   int short_cut = SC_NONE,
                                   const char *help_message = "")
    {return(m_insert_item(0,label,attribute,short_cut,help_message));};
    virtual int         m_insert_item(int index, const char *label, 
                                      int attribute = LI_NOATTR,
                                      int short_cut = SC_NONE,
                                      const char *help_message= "");

    // Add severals items to the menu
    // Each item is of type TListItem
    // For the last item, item[].label must be NULL

    // Returns the number of items really added

    virtual int         m_add_items(const TMenuItem items[]);

    // Deletes items

    virtual void        m_delete_item(int item_index);
    virtual void        m_clear_list();

    // Callback invoked when an item of the menu is clicked
	jptui::CallbackHolder	itemClickedAction_;

  /*-------------------*/
  /* Protected methods */
  /*-------------------*/

  protected:

    // Display routines

    virtual void        m_display();

    // Callbacks

    virtual void        m_selected_item_changed_callback();
    virtual void        m_nb_items_changed_callback();
    virtual void        m_item_clicked_callback(int item_index);

    // Events

    virtual bool     m_short_cut_pressed_event(int shortcut);


  /*-----------------*/
  /* Private methods */
  /*-----------------*/

  private:

    // Display routines

      virtual void      m_display_item_node_label(PItemNode node,bool show_hot_key);

    // Changes the size of the menu so that all its items can be seen

	    void        m_adjust_size();

    // Events

    virtual bool     m_leave_event_if_mouse_leave_list(int x,int y);
    virtual void        m_item_hot_key_pressed_event(int item_index);


};





/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : TCOMBBOX.H                           Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* TCOMBOBOX                                                                */
/*--------------------------------------------------------------------------*/
/* Objet TComboBox                                                          */
/****************************************************************************/




class TComboList;
class TComboBox:public TEditZone
{
  /*-------------*/
  /* Objets amis */
  /*-------------*/

  friend class TComboList;

  /*------------------*/
  /* Protected fields */
  /*------------------*/

  protected:

    // The edition-zone contents have to be in the list

    bool f_string_must_be_in_list;


  /*----------------*/
  /* Private fields */
  /*----------------*/

  private:

    // Dselectionne l'lment slectionn de la liste is le
    // contenu de la zone de saisie est modifi

    bool f_unselect_on_string_change;

    // Elment slectionne de la liste lorsqu'on l'ouvre

    int     f_selected_item_when_opening;

    // Liste

    PSimpleList f_list;

  /*--------------------*/
  /* Mthodes publiques */
  /*--------------------*/

  public:

    // Constructeur/Destructeur
		    TComboBox(PObject parent,
			     int rel_x,int rel_y,
			     int xoffset_caption,int yoffset_caption,
			     const char *caption,
			     int display_length,
			     int max_length,
			     const char *string,
			     int list_height,
			     TListItem items[],
			     bool string_must_be_in_list,
			     bool sorted=false,
			     bool enabled=true);
		    ~TComboBox();

    // Mthodes d'accs

    virtual void    m_enable();
    virtual void    m_disable();

    virtual void    m_enable_modification()   {return;};
    virtual void    m_disable_modification()  {return;};

    virtual void    m_set_string(const char *string);

    // Recopie d'un lment de la liste dans la zone d'dition

	    void    m_set_string_from_list_item(int item_index) {f_list->m_set_selected_item_index(item_index);};

    // Accs … la liste de la combo-box

	    PSimpleList m_get_list() {return(f_list);};

    // Callback : Fonction appele lorsque le contenu de la combo-box
    //            est valide
	jptui::CallbackHolder	stringValidatedAction_;

  /*--------------------*/
  /* Mthodes protges */
  /*--------------------*/

  protected:

    // Affichage

    virtual void    m_display();

    // Ouverture/Fermeture de l'objet

    virtual void    m_set_open(bool open);

    // Evnements

    virtual bool m_left_button_pressed_event(int x,int y);
    virtual bool m_left_button_double_click_event(int x,int y);
    virtual bool m_key_pressed_event(TKey key);

    // Appel des callbacks

    virtual void    m_string_modified_callback();
    virtual void    m_string_validated_callback();

  /*------------------*/
  /* Mthodes prives */
  /*------------------*/

  private:

    // Affichage

	    void    m_display_arrow();

    // Ouverture/Fermeture d'un menu

	    void    m_open_list();
	    void    m_close_list();

    // Slection d'un lment

	    void    m_select_item_if_none_is_selected();

    // Modification de la chane … partir de l'lment slectionn de la
    // liste (s'il y en a un)

	    void    m_set_string_from_selected_item();

};





/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : OKCANCELBOX.HPP                         Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/



namespace jptui {

class OkCancelBox : public ButtonsBox
{
public:
	void	init(const char* title,	size_t reserveX, size_t reserveY);
};

extern const ButtonsBox::ButtonDescr*	gOkCancelDs();

} // namespace jptui




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : INPUTBOX.HPP                         Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/



namespace jptui {

class InputBox : public OkCancelBox
{
public:
	void	init(const char* title, const char* defaultString, 
			     size_t displayLength, size_t maxLength, 
				 bool passwordMode = false);
	const char*	getString() const;
private:
	std::auto_ptr<TEditZone>	editZone_;
};

} // namespace jptui




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : MSGBOX.H                             Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

//
// Left for backward compatibility for now
//
/****************************************************************************/
/* MSGBOX                                                                   */
/*--------------------------------------------------------------------------*/
/* Opens a message box and waits for the user to press a button             */
/****************************************************************************/



  #ifdef __JPTUI__
  #else
  #endif

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                                 CONSTANTS                              บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/*--------------*/
/* window_style */
/*--------------*/

// Style (color) of the dialog box :

// DIALOG1, DIALOG2, HELP, ALERT...
// See TWindow.h (in JPTui.h)

/*----------------------------------------*/
/* type = buttons | icon | default button */
/*----------------------------------------*/

// buttons

#define MB_OK               0
#define MB_OKCANCEL         1     //0000 0000 0000 0001
#define MB_OKQUIT           2     //0000 0000 0000 0010
#define MB_ABORTRETRYIGNORE 3     //0000 0000 0000 0011
#define MB_YESNOCANCEL      4     //0000 0000 0000 0100
#define MB_YESNO	    	5     //0000 0000 0000 0101
#define MB_RETRYCANCEL      6     //0000 0000 0000 0110
// added
#define MB_CONTINUE         7
#define MB_YES_NO_REVIEW    8
#define MB_YES_NO_RETRY     9

// icon

#define MB_NOICON	    	0     //0000 0000 0001 0000
#define MB_ICONSTOP        16     //0000 0000 0001 0000
#define MB_ICONQUESTION    32     //0000 0000 0010 0000
#define MB_ICONEXCLAMATION 48     //0000 0000 0011 0000
#define MB_ICONINFORMATION 64     //0000 0000 0100 0000

// default button

#define MB_DEFBUTTON1       0
#define MB_DEFBUTTON2     256     //0000 0001 0000 0000
#define MB_DEFBUTTON3     512     //0000 0010 0000 0000

/*------------*/
/* sound_type */
/*------------*/

// sound to play when the dialog box opens :

// ERROR_SOUND, BIP1...
// See Sound.h (in JPTui.h)


/*----------------*/
/* Returned value */
/*----------------*/

#define OK_BUTTON		1
#define CANCEL_BUTTON	2
#define QUIT_BUTTON		3
#define ABORT_BUTTON	4
#define RETRY_BUTTON	5
#define IGNORE_BUTTON	6
#define YES_BUTTON	  	7
#define NO_BUTTON	  	8
#define CONTINUE_BUTTON	9
#define REVIEW_BUTTON	10

// CheckBox

#define MB_CHECKED_MASK	0x80

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                                 FUNCTIONS                              บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

int	MessageBox(const char* title, const char* message, int windowStyle,
			   int type = MB_OK, TSound soundType = NO_SOUND,
			   const char* checkBoxTitle = 0, bool checkBoxValue = false);





/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : TFILELB.H                            Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* TFILELB                                                                  */
/*--------------------------------------------------------------------------*/
/* TFileListBox class : locates & lists files according to a specified mask */
/****************************************************************************/



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                                  TYPES                                 บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/


typedef enum
{
  FLB_REFRESH_OK_FILE,
  FLB_REFRESH_OK_MASK,
  FLB_INVALID_DRIVE,
  FLB_NO_DISK_IN_DRIVE,
  FLB_INVALID_DIR,
  FLB_INVALID_FILE } TFLBError;

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                                 CONSTANTS                              บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

#define FLB_PARENT_DIR_CHAR    0x18
#define FLB_SUB_DIR_CHAR       0x19
#define FLB_FILE_CHAR		 ' '


class TFileListBox:public TListBox
{
  /*----------------*/
  /* Private fields */
  /*----------------*/

  private:

    bool             f_init_in_progress;

    char                f_current_dir[MAX_PATH];
    char                f_current_mask[MAX_FILE];

  /*----------------*/
  /* Public methods */
  /*----------------*/

  public:

    // Constructor/Destructor

			TFileListBox(PObject parent,
				     int rel_x, int rel_y,
				     int width, int height,
				     const char *dir_mask = "*.*");
			~TFileListBox();


    // Refreshes the list of files according to the specified mask

	    TFLBError m_refresh_file_list(const char *dir_mask);

    // Returns the current dir/mask/file

	    char        *m_get_current_dir() {return(f_current_dir);};
	    char        *m_get_current_mask() {return(f_current_mask);};

    // Refreshes the list of files according to a given item of the current
    // list

	    TFLBError   m_refresh_file_list_from_item(int item_index);

    // Callback invoked if no errors occurs when the list of files
    // is refreshed
	jptui::CallbackHolder	refreshDoneAction_;

    // Callback invoked when a file has been chosen by the user
	jptui::CallbackHolder	fileChosenAction_;

  /*-------------------*/
  /* Protected methods */
  /*-------------------*/

  protected:

    // Events

    virtual bool     m_key_pressed_event(TKey key);

    // Callbacks

    virtual void        m_item_dbl_clicked_callback(int index);

    // Functions called when an error occurs while refreshing the list of files

    virtual void        m_invalid_drive_error(int drive);
    virtual void        m_no_disk_in_drive_error(int drive);
    virtual void        m_invalid_dir_error(const char *dir);
    virtual void        m_invalid_file_error(const char *file);

};






/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : FILEBOX.H                            Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* FILEBOX                                                                  */
/*--------------------------------------------------------------------------*/
/* File selection box                                                       */
/****************************************************************************/


// Returns the full name of the selected file
// or "" if no file is selected

const char*	FileSelectionBox(const char* title, const char* mask,
		       				 const char* okButtonCaption = "",
		       				 const char* cancelButtonCaption = "");





/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บÛ฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿฿Ûบ*/
/*บÝ                          FILE : THELPWND.H                           Þบ*/
/*บÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛบ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/****************************************************************************/
/* THELPWND.H                                                               */
/*--------------------------------------------------------------------------*/
/* THelpWindow class                                                        */
/****************************************************************************/





/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                             CLASS DEFINITION                           บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

class THelpWindow : public TWindow, public jptui::Callback
{
  /*----------------*/
  /* Friend classes */
  /*----------------*/

  friend class THelpTextZone;

  /*----------------*/
  /* Private fields */
  /*----------------*/

  private:

    // Push-Buttons

    PPushButton
	    f_contents_button,
	    f_index_button,
	    f_back_button,
	    f_previous_button,
	    f_next_button;

    // Help text

    PHelpTextZone
	    f_help_text_zone;


  /*----------------*/
  /* Public methods */
  /*----------------*/

  public:

    // Constructor / Destructor


		     THelpWindow(int x ,int y,
			     int width, int height,
			     const char *caption= "",
			     bool modal=true,
			     bool movable=true);

    virtual  	     ~THelpWindow();

    // Access methods

	    bool m_load_help_file(const char *file_name);
	    void    m_unload_help_file();
	    bool m_display_topic(const char *topic, bool push_current_topic=true);

	    void    m_select_contents_topic();
	    void    m_select_index_topic();
	    void    m_select_back_topic();
	    void    m_select_previous_topic();
	    void    m_select_next_topic();

private:
	void	operator()(TObject* who, what_type what, item_type item);
};


#endif
