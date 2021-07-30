/****************************************************************************/
/* JPDEMO                                                                   */
/*--------------------------------------------------------------------------*/
/* JPTUI Demonstration                                                      */
/*--------------------------------------------------------------------------*/
/* Author     : DELPRAT Jean-Pierre (jpdelprat@teaser.fr)                   */
/* Created on : 15-APR-96                                                   */
/* Modified on: 13-Dec-02                                                   */
/****************************************************************************/


//-- Includes --

#include <dos.h>
#include <iostream>
#include <stdlib.h>

#include "jptui.h"

using namespace std;

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                                 CONSTANTS                              บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

//-- String numbers (used by multi-language support)

#define VOC_LANGUAGE 1000
#define VOC_HELLO    1001

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                                WIDGETS                                 บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/


//-- APPLICATION -------------------------------------------------------------

TApplication  JPTuiDemo(ENGLISH);

//-- MAIN WINDOW -------------------------------------------------------------

TWindow       MainWindow          (DIALOG1,
				   2,2,77,22,
				   " ",
				   INFO_BAR,
				   NOT_MODAL);

TPushButton   QuitButton          (&MainWindow,
				   33,19,10,
				   "~Quit",
				   PB_CANCEL);

//-- MENUS -------------------------------------------------------------------

TMenuBar      MenuBar             (&MainWindow);

TMenuItem     MenuItemsMenuItems[]=

				  { {"~Unchecked item",
				     LI_TOGGLE,
				     SC_CTRL_F1,
				     "Help about 'Unchecked item' : it's a toggle"},

				    {"~Checked item",
				     LI_CHECKED,
				     SC_ALT_F1,
				     "Help about 'Checked item' : it's not a toggle"},

				    {SEPARATOR,
				     LI_NOATTR,
				     SC_NONE,
				     ""},

				    {"Disabled ~item",
				     LI_TOGGLE|LI_CHECKED|LI_DISABLED,
				     SC_SHIFT_F1,
				     "Help about 'Disabled item'"},

				    {NULL,
				     LI_NOATTR,
				     SC_NONE,
				     ""} };

TMenu	      MenuItemsMenu       (&MenuBar,
				   "~Menu items",
				   MenuItemsMenuItems);


TMenuItem     DialogBoxesMenuItems[]=

				  { {"~Message Box",
				     LI_NOATTR,
				     SC_ALT_HOME,
				     "This menu item opens two message boxes"},

				    {"~Input Box",
				     LI_NOATTR,
				     SC_ALT_END,
				     "This menu item opens an input box"},

				    {SEPARATOR,
				     LI_NOATTR,
				     SC_NONE,
				     ""},

				    {"~File Selection Box",
				     LI_NOATTR,
				     SC_F3,
				     "This menu item opens a file selection box"},

				    {NULL,
				     LI_NOATTR,
				     SC_NONE,
				     ""} };

TMenu	      DialogBoxesMenu	  (&MenuBar,
				   "Dialo~g",
				   DialogBoxesMenuItems);


TMenuItem     OptionMenuItems[]=
				  { {"Dos ~box under windows",
				     LI_TOGGLE,
				     SC_CTRL_B,
				     "Configure JPTUI to be ran in a DOS box under Windows"},

				    {"~Full screen",
				     LI_TOGGLE|LI_CHECKED,
				     SC_CTRL_F,
				     "Configure JPTUI to be ran full screen"},

				    {SEPARATOR,
				     LI_NOATTR,
				     SC_NONE,
				     ""},
				    {"~Graphical mouse pointer",
				     LI_TOGGLE|LI_CHECKED,
				     SC_CTRL_G,
				     "Select this to have a graphical mouse pointer drawn by JPTUI"},

				    {"~Textual mouse pointer",
				     LI_TOGGLE,
				     SC_CTRL_T,
				     "Select this to have a textual mouse pointer drawn by JPTUI"},

				    {"\"~Driver-displayed\" mouse pointer",
				     LI_TOGGLE,
				     SC_CTRL_D,
				     "Select this to have the mouse pointer drawn by the mouse driver"},

				    {SEPARATOR,
				     LI_NOATTR,
				     SC_NONE,
				     ""},

				    {"~Standard font",
				     LI_TOGGLE,
				     SC_CTRL_S,
				     "Select this to use the standard font"},

				    {"~JPTUI font",
				     LI_TOGGLE|LI_CHECKED,
				     SC_CTRL_J,
				     "Select this to use JPTUI special font"},

				    {SEPARATOR,
				     LI_NOATTR,
				     SC_NONE,
				     ""},

				    {"Text mode 80x25, 8 pixels per character",
				     LI_NOATTR,
				     SC_ALT_1,
				     "Sets the text mode to 80x25 with a 8-pixel-wide font"},

				    {"Text mode 80x25, 9 pixels per character",
				     LI_NOATTR|LI_CHECKED,
				     SC_ALT_2,
				     "Sets the text mode to 80x25 with a 9-pixel-wide font"},

				    {"Text mode 80x28, 8 pixels per character",
				     LI_NOATTR,
				     SC_ALT_3,
				     "Sets the text mode to 80x28 with a 8-pixel-wide font"},

				    {"Text mode 80x28, 9 pixels per character",
				     LI_NOATTR,
				     SC_ALT_4,
				     "Sets the text mode to 80x28 with a 9-pixel-wide font"},

				    {"Text mode 80x50, 8 pixels per character",
				     LI_NOATTR,
				     SC_ALT_5,
				     "Sets the text mode to 80x50 with a 8-pixel-wide font"},

				    {"Text mode 80x50, 9 pixels per character",
				     LI_NOATTR,
				     SC_ALT_6,
				     "Sets the text mode to 80x50 with a 9-pixel-wide font"},

				    {"Text mode 132x25 (Cirrus Logic only)" ,
				     LI_NOATTR,
				     SC_ALT_7,
				     "Sets the text mode to 132x25 (use only with a Cirrus Logic card)"},

				    {"Text mode 132x43 (Cirrus Logic only)" ,
				     LI_NOATTR,
				     SC_ALT_8,
				     "Sets the text mode to 132x43 (use only with a Cirrus Logic card)"},

				    {SEPARATOR,
				     LI_NOATTR,
				     SC_NONE,
				     ""},

				    {"Shows available memory",
				     LI_TOGGLE,
				     SC_NONE,
				     "Select this item to display free memory (only possible in debug version)"},

				    {NULL,
				     LI_NOATTR,
				     SC_NONE,
				     ""}  };

TMenu         OptionMenu          (&MenuBar,
				   "~Options",
				   OptionMenuItems);


TMenuItem     LanguageMenuItems[]={ {"Multi-~language support",
				     LI_NOATTR,
				     SC_CTRL_L,
				     "This menu item starts a demonstration of multi-language support"},

				    {NULL,
				     LI_NOATTR,
				     SC_NONE,
				     ""}  };

TMenu         LanguageMenu        (&MenuBar,
				   "La~nguage",
				   LanguageMenuItems);

TMenuItem     HelpMenuItems[]=
				  { {"~Help",
				     LI_NOATTR,
				     SC_F1,
				     "This menu item opens a hypertext help windows"},

				    {NULL,
				     LI_NOATTR,
				     SC_NONE,
				     ""}  };

TMenu         HelpMenu	          (&MenuBar,
				   "~Help",
				   HelpMenuItems);

//-- TAB GROUP ---------------------------------------------------------------

TTabGroup     TabGroup            (&MainWindow,
				   2,3,72,15,DIALOG2);

//-- BUTTONS -----------------------------------------------------------------

TTab	      ButtonTab	          (&TabGroup,
				   "B~uttons");

TLabel        PushButtonLabel     (&ButtonTab,
				   4,3,
				   "Push Button                              :");

TPushButton   PushButton          (&ButtonTab,
				   50,3,17,
				   "Push Button ~w");

TLabel        CancelPushButtonLabel
				  (&ButtonTab,
				   4,5,
				   "Cancel Push Button (pushed by <esc>)     :  Try Quit button below");

TLabel        DefaultPushButtonLabel
				  (&ButtonTab,
				   4,7,
				   "Default Push Button (pushed by <return>  :\nwhen no other button has the focus)");

TPushButton   DefaultPushButton   (&ButtonTab,
				   50,7,17,
				   "Push Button ~y",
				   PB_DEFAULT);

TLabel        DisabledPushButtonLabel
				  (&ButtonTab,
				   4,10,
				   "Disabled Push Button                     :");

TPushButton   DisabledPushButton  (&ButtonTab,
				   50,10,17,
				   "Push Button ~z",
				   PB_NORMAL,
				   SHC_BOTH,
				   DISABLED);


//-- CHECK BOXES -------------------------------------------------------------

TTab	      CheckBoxTab	  (&TabGroup,
				   "C~heck Boxes");

TCheckBox     CheckBox1		  (&CheckBoxTab,
				   8,4,20,
				   "Check Box ~1");

TCheckBox     CheckBox2		  (&CheckBoxTab,
				   8,5,20,
				   "Check Box ~2",
				   CHECKED);

TCheckBox     CheckBox3		  (&CheckBoxTab,
				   8,7,20,
				   "Check Box ~3",
				   NOT_CHECKED,
				   DISABLED);

TRadioGroup   RadioGroup          (&CheckBoxTab,
				   42,2,20,8,"~Radio Buttons");

TRadioButton  RadioButton1	  (&RadioGroup,
				   1,2,17,
				   "Radio Button ~x");

TRadioButton  RadioButton2	  (&RadioGroup,
				   1,3,17,
				   "Radio Button ~y",
				   CHECKED);


TRadioButton  RadioButton3	  (&RadioGroup,
				   1,5,17,
				   "Radio Button ~z",
				   NOT_CHECKED,
				   DISABLED);

//-- EDIT ZONES --------------------------------------------------------------

TTab	      EditZoneTab	  (&TabGroup,
				   "~Edit Zones");

TEditZone     EditZone     	  (&EditZoneTab,
				   42,1,
				   -40,0,
				   "Edit ~Zone (enter any string) :",
				   25,255,
				   "Enter a long string");

TEditZone     DisabledEditZone    (&EditZoneTab,
				   42,2,
				   -40,0,
				   "Di~sabled Edit Zone           :",
				   25,255,
				   "You can't enter anything",
				   DISABLED);

TIntegerEditZone
	      IntegerEditZone
				  (&EditZoneTab,
				   52,4,
				   -50,0,
				   "~Integer Edit Zone (enter any integer value) :",
				   15,255,
				   0,10);

TDecimalEditZone
	      DecimalEditZone
				  (&EditZoneTab,
				   52,5,
				   -50,0,
				   "~Decimal Edit Zone (enter any decimal value) :",
				   15,255,
				   -3.2,10.5);


TTextZone     TextZone            (&EditZoneTab,
				   2,7,
				   68,6,
				   "~Text Zone",
				   255);

//-- LISTS -------------------------------------------------------------------

TTab	      ListTab	          (&TabGroup,
				   "~Lists");

TListItem     ListBoxItems[]=     {{"Item 1",LI_NOATTR},
				   {"Item 2",LI_NOATTR},
				   {"Item 3",LI_DISABLED},
				   {SEPARATOR,LI_NOATTR},
				   {"Items",LI_NOATTR},
				   {"are not",LI_NOATTR},
				   {"automatically",LI_NOATTR},
				   {"sorted",LI_NOATTR},
				   {SEPARATOR,LI_NOATTR},
				   {"Items",LI_NOATTR},
				   {"are not",LI_NOATTR},
				   {"automatically",LI_NOATTR},
				   {"sorted",LI_NOATTR},
				   {SEPARATOR,LI_NOATTR},
				   {"Items",LI_NOATTR},
				   {"are not",LI_NOATTR},
				   {"automatically",LI_NOATTR},
				   {"sorted",LI_NOATTR},
				   {NULL,LI_NOATTR}};

TListBox      ListBox             (&ListTab,
				  2,2,
				  20,10,
				  "List Bo~x",
				  ListBoxItems);


TListItem     SortedListBoxItems[]=
				  {{"Item 1",LI_NOATTR},
				   {"Item 2",LI_NOATTR},
				   {"Item 3",LI_DISABLED},
				   {SEPARATOR,LI_NOATTR},
				   {"Items"},
				   {"are",LI_NOATTR},
				   {"automatically",LI_NOATTR},
				   {"sorted",LI_NOATTR},
				   {NULL,LI_NOATTR}};


TListBox      SortedListBox       (&ListTab,
				  24,2,
				  20,10,
				  "~Sorted List Box",
				  SortedListBoxItems,
				  SORTED);

TListItem     CheckListBoxItems[]={{"Any item can be"  ,LI_NOATTR},
				   {"checked and can"  ,LI_CHECKED},
				   {"be a toggle or "  ,LI_TOGGLE},
				   {"not"              ,LI_CHECKED},
				   {SEPARATOR          ,LI_NOATTR},
				   {"It's a toggle"    ,LI_TOGGLE|LI_CHECKED},
				   {"It's not a toggle",LI_NOATTR},
				   {SEPARATOR          ,LI_NOATTR},
				   {"Items can be"     ,LI_TOGGLE},
				   {"toggled by double",LI_TOGGLE},
				   {"clicking or by"   ,LI_TOGGLE},
				   {"pressing <SPACE>" ,LI_TOGGLE},
				   {NULL,LI_NOATTR}};

TListBox      CheckListBox       (&ListTab,
				  46,2,
				  23,10,
				  "~Check List Box",
				  CheckListBoxItems);

//-- MISC IN MAIN WINDOW------------------------------------------------------

TTab	      MiscTab	          (&TabGroup,
				  "Mi~sc");

TFrame        Frame1              (&MiscTab,
				  2,2,
				  40,11,
				  "This is a ~Frame");

TPushButton   GaugeStartButton    (&Frame1,
				  13,2,12,
				  "~Push here");

TFrame        Frame2              (&Frame1,
				  2,4,
				  36,6,
				  "This is ~another Frame");

TGauge        Gauge               (&Frame2,
				  2,2,32,
				  "Gauge : work in progress");


TLabel        ComboBoxLabel       (&MiscTab,
                                   45,3,
                                   "Combo-Boxes :");

TComboBox     ComboBox1           (&MiscTab,
				   45,6,
				   0,-1,
				   "F~ree string",
				   15,20,
				   "Free String",
				   6,
				   ListBoxItems,
				   STRING_NEEDNT_TO_BE_IN_LIST,
				   NOT_SORTED,
				   ENABLED);

TComboBox     ComboBox2           (&MiscTab,
				   45,9,
				   0,-1,
				   "S~tring in list",
				   15,20,
				   "Free String",
				   6,
				   SortedListBoxItems,
				   STRING_MUST_BE_IN_LIST,
				   SORTED,
				   ENABLED);

//-- HELP WINDOWS ------------------------------------------------------------

THelpWindow       HelpWindow     (2,1,56,24,
				   "JPTUI Help Example",
				   NOT_MODAL,
				   MOVABLE);

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                             MISC FUNCTIONS                             บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

//-- To set non standard Cirrus Logic text modes --

void SetCirrusLogicMode132x25()
{
  union REGS regs;

  regs.x.ax=0x14;
  int86(0x10, &regs, &regs);
}

void SetCirrusLogicMode132x43()
{
  union REGS regs;

  regs.x.ax=0x54;
  int86(0x10, &regs, &regs);
}


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                               CALLBACKS                                บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

//-- Callback called when an item of the dialog menu is clicked --

void DialogMenuItemClickedCall(PObject, int item_nb, const char*)
{
	jptui::InputBox ib;

  switch (item_nb)
    {
      case 1 : MessageBox("Message Box ","This is a message box",ALERT,MB_ICONSTOP);
	       MessageBox("","This is another message box",HELP,MB_ICONINFORMATION|MB_ABORTRETRYIGNORE,ERROR_SOUND);
	       break;
      case 2 : 
			ib.init("Please enter what you want", "Default Value", 20, 30);
			ib.run();
	       	break;
      case 4 : MessageBox("You have chosen",
			  FileSelectionBox("Load","*.*"),
			  DIALOG1);
    }

}

//-- Callback called when an item of the option menu is clicked --

void OptionMenuItemClickedCall(PObject, int item_nb, const char*)
{
  static int old_selected_mode=2;
  static int old_selected_mouse_aspect=4;
  static int old_selected_font=9;
  static int old_selected_text_mode=12;

  switch (item_nb)
    {
      case 1 : SetDosBoxMode();				  break;
      case 2 : SetFullScreenMode();			  break;

      case 4 : SetMousePointerAspect(MPA_GRAPHICAL);      break;
      case 5 : SetMousePointerAspect(MPA_TEXTUAL);        break;
      case 6 : SetMousePointerAspect(MPA_MOUSE_DRIVER);   break;

      case 8 : SetFont(FONT_STANDARD);			  break;
      case 9 : SetFont(FONT_JPTUI);                       break;

      case 11: SetTextMode(TEXTMODE_80x25_8);  		  break;
      case 12: SetTextMode(TEXTMODE_80x25_9);             break;
      case 13: SetTextMode(TEXTMODE_80x28_8);  		  break;
      case 14: SetTextMode(TEXTMODE_80x28_9);  		  break;
      case 15: SetTextMode(TEXTMODE_80x50_8);  		  break;
      case 16: SetTextMode(TEXTMODE_80x50_9);  		  break;
      case 17: SetUserTextMode(SetCirrusLogicMode132x25); break;
      case 18: SetUserTextMode(SetCirrusLogicMode132x43); break;

      case 20: if (OptionMenu.m_item_is_checked(item_nb))
		 DEBUG_SHOW_MEMORY();
	       else
		 DEBUG_HIDE_MEMORY();
	       break;

    }

  if ((item_nb>=1) && (item_nb<=2))
    {
      OptionMenu.m_uncheck_item(old_selected_mode);
      old_selected_mode=item_nb;
      OptionMenu.m_check_item(old_selected_mode);


      OptionMenu.m_uncheck_item(old_selected_mouse_aspect);
      OptionMenu.m_uncheck_item(old_selected_font);

      switch (item_nb)
	{
	  case 1 : old_selected_font=8;
		   old_selected_mouse_aspect=6;
		   break;

	  case 2 : old_selected_font=9;
		   old_selected_mouse_aspect=4;
		   break;
	}

      OptionMenu.m_check_item(old_selected_mouse_aspect);
      OptionMenu.m_check_item(old_selected_font);
    }


  if ((item_nb>=4) && (item_nb<=6))
    {
      OptionMenu.m_uncheck_item(old_selected_mouse_aspect);
      old_selected_mouse_aspect=item_nb;
      OptionMenu.m_check_item(old_selected_mouse_aspect);
    }

  if ((item_nb>=8) && (item_nb<=9))
    {
      OptionMenu.m_uncheck_item(old_selected_font);
      old_selected_font=item_nb;
      OptionMenu.m_check_item(old_selected_font);
    }

  if ((item_nb>=11) && (item_nb<=18))
    {
      OptionMenu.m_uncheck_item(old_selected_text_mode);
      old_selected_text_mode=item_nb;
      OptionMenu.m_check_item(old_selected_text_mode);
    }
}

//-- Callback called when an item of the language menu is clicked --

void LanguageMenuItemClickedCall(PObject, int item_nb, const char*)
{
  int i;
  bool cancel=false;

  if (item_nb==1)
    {
      for (i=FIRST_LANGUAGE;i<=LAST_LANGUAGE;i++)
	{
	  if (i==(TLanguage)GREEK)
	  {
	    HideScreen();  // To change the font
	    LoadVocabulary(GREEK);
	    UnhideScreen();
	  }
	  else
	    LoadVocabulary((TLanguage)(i-FIRST_LANGUAGE));


	  if (MessageBox(GetString(VOC_LANGUAGE),
		     GetString(VOC_HELLO),
		     DIALOG1,MB_ICONEXCLAMATION|MB_OKCANCEL)
	      ==CANCEL_BUTTON)
	    cancel=true;

	  if (i==(TLanguage)GREEK)
	  {
	    HideScreen();  // To restore the font
	    LoadVocabulary(ENGLISH);
	    UnhideScreen();
	  }

	  if (cancel)
	    break;
	}

      LoadVocabulary(ENGLISH);
    }
}

//-- Callback called when an item of the help menu is clicked --

void HelpMenuItemClickedCall(PObject, int, const char*)
{
  // Opens the window or brings it to front

  HelpWindow.m_open();
}

//-- Callback called when the button located near the gauge is pressed

void GaugeStartButtonPressedCall(PObject, const char*)
{

  long value=Gauge.m_get_value();
  int inc=(value==0)?1:-1;

  value+=inc;

  while ((value>=0) && (value<=100))
    {
      Gauge.m_set_value(value);
      JPRefresh();
      delay(10);
      value+=inc;
    }


  MenuItemsMenu.m_set_focus();
}

//-- Callback called when the quit button or the close button of the main
//   windows are pressed

void QuitCall(PObject, const char*)
{
  JPStop();
}

//-- Test callback -- emits a BEEP when called

//void BipCall(PObject /*sender*/,char */*arg*/)
//{
//  PlaySound(BIP1);
//}

namespace {

class CallbacksWrapper : public jptui::Callback
{
public:
	void	operator()(TObject* who, what_type what, item_type item);
};

CallbacksWrapper	callbacksWrapper;

} // namespace

void	CallbacksWrapper::operator()(TObject* who, what_type what, item_type item)
{
	if (what == 0) {
		DialogMenuItemClickedCall(who, item, 0);
	} else if (what == 1) {
		OptionMenuItemClickedCall(who, item, 0);
	} else if (what == 2)  {
		LanguageMenuItemClickedCall(who, item, 0);
	} else if (what == 3)  {
		HelpMenuItemClickedCall(who, item, 0);
	} else if (what == 4)  {
		GaugeStartButtonPressedCall(who, 0);
	} else if (what == 5)  {
		QuitCall(who, 0);
//	} else if (what == 5)  {
//		BipCall(who, 0);
	}
}

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป*/
/*บ                            MAIN PROGRAM                                บ*/
/*ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

int main(int argc, char**)
{
  char *main_window_caption="JPTUI Demo - Running under @@@@@@@";

  // Has the program been called with arguments

  if (argc!=1)
      {
	cout << "\nJPTUI Demo for JPTUI " << JPTUI_VERSION_STRING;
	cout << "\nBy Jean-Pierre Delprat, " << JPTUI_DATE_STRING;
	cout << "\nShows what you can do with JPTUI.\n\n";

	cout << "Usage : JPDEMO [/?]\n\n";
	cout << "        /? : Displays this help\n\n";
	exit(EXIT_SUCCESS);
      }

  // Is the program running under Windows or not ?

  strcpy(strchr(main_window_caption,'@'), RunningUnderWindows() ? "Windows" : "Dos");
  MainWindow.m_set_caption(main_window_caption);

  // Forces full screen mode

//  SetFullScreenMode();

  // JPTUI initialization

  JPInit();

  // Callbacks declaration

  MainWindow.setCloseButtonPressedCallback(callbacksWrapper, 5);	// QuitCall
  QuitButton.pressedAction_.set(callbacksWrapper, 5);	// QuitCall

  DialogBoxesMenu.itemClickedAction_.set(callbacksWrapper, 0);	// DialogMenuItemClickedCall
  OptionMenu.itemClickedAction_.set(callbacksWrapper, 1);	// OptionMenuItemClickedCall
  LanguageMenu.itemClickedAction_.set(callbacksWrapper, 2);	//LanguageMenuItemClickedCall
  HelpMenu.itemClickedAction_.set(callbacksWrapper, 3);	// HelpMenuItemClickedCall

  GaugeStartButton.pressedAction_.set(callbacksWrapper, 4);	// GaugeStartButtonPressedCall

  // Misc initializations

  // -> The window info bar

  MainWindow.m_set_info_message("Welcome to JPTUI !!!");

  // -> The contents of the text zone

  TextZone.m_insert_text("This is a text-editing zone. You can use:\n" \
			 "\t. CTRL-Y to delete a line\n" \
			 "\t. CTRL-INS,SHIFT-DEL & SHIFT-INSERT to copy, cut and paste\n\n"\
			 "Feel free to type whatever you want...");

  // -> The help system
  //    (The choice of the topic is optional. The default topic is
  //     the table of contents if any, or the index otherwise)

  if (HelpWindow.m_load_help_file("JPDEMO.HLP"))
    HelpWindow.m_display_topic("Help System");

  // We open the main window...

  MainWindow.m_open();

  // ...and we launch the application

  JPRun();

  // Once the program has been stopped (see QuitCall)...

  exit(EXIT_SUCCESS);

  return(0);
}
