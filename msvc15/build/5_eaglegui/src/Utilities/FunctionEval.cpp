

#include "Eagle.hpp"
#include <loadpng.h>

#include <string>
using std::string;

#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include <map>
using std::map;

#include <set>
using std::set;

#include <cstring>

#include "Graphing.hpp"

/** TODO
   - Add in simplify
   -   simplify sometimes creates two new functions - one from CustomFunction::Simplify and another
   -   simplify doesn't handle associative addition
   - Add simplification to Function::Parse
   -   Multiplication : Commutative - a*b = b*a
   -   Addition       : Associative - a + b = b + a
   
   - Add a substitution function to FunctionBase? So that function calls can be replaced by real arguments.
   
   
   - Add in derivative/integral
   - Add in graphing (in addition to plotting)
*/

int savenum = 1;

void DirectToConsole();

void Plot(FunctionBase* f , string name , double xmin , double xmax , double ymin , double ymax);


int main(int argc , char** argv) {
   
   if (!GetAllegroReady(true , true , true)) {return 1;}
   loadpng_init();
   
   int dcd = desktop_color_depth();
   if (dcd == 0) {dcd = 32;}
   
   GfxModeData gfxmode(GFX_AUTODETECT_WINDOWED , 800 , 600 , dcd);
   
   if (!ChangeRes(gfxmode , CRM_DIRECT)) {return 1;}
   set_display_switch_mode(SWITCH_PAUSE);
   
   SetupInputHandler();
   
   DirectToConsole();
   
   show_mouse(screen);
/**
   char buffer[128];
   for (int i = -60 ; i < 61 ; ++i) {
      sprintf(buffer , "1e%i" , i);
      double d = 0.0;
      sscanf(buffer , "%lf" , &d);
      printf("Before calling FormatDouble : %s , %.30lg\n" , buffer , d);
      FormatDouble(d);
      printf("\n");
   }
*/

   string s;
   bool quit = false;
   int w = 0;
   int h = 0;
   
   double xmin = -10.0;
   double xmax = 10.0;
   double ymin = -10.0;
   double ymax = 10.0;
   
   WidgetColorset wc = DefaultWidgetHSVColors(180.0 , 0.0);
   
   map<string , double> vars;
   
   
   ///Graph graph(...);
   
   cout << "Welcome to the function evaluator!" << endl;
   cout << "Type 'help' to list available commands." << endl;
   cout << "Enter input at prompt (>)." << endl;
   cout << endl;

   while (!close_program && !quit) {
      cout << ">";
      getline(cin , s);
      if (s.size() == 0) {
         cout << endl;
         continue;
      }
      /// Input types
      /// exit
      /// help
      /// help functions
      /// help naming
      /// help order
      /// clear
      /// clear variables
      /// clear functions
      /// variables
      /// functions
      /// change_res(w,h)
      /// set range(0.0 , 10.0 , -5.0 , 5.0)
      /// plot f
      /// simplify(function_name , new_function_name)
      /// x + y + z
      /// x = x + 2
      /// x = 5.0
      /// x =
      /// f(x,y,z) = x + y + z
      const char* cstr = s.c_str();
      if (strncmp("exit" , s.c_str() , 4) == 0) {
         quit = true;
         cout << "Select output window to quit." << endl;
      }
      else if (strncmp("help" , cstr , 4) == 0) {
         if (strlen(cstr) == 4) {
            cout << "Valid commands" << endl;
            cout << "-----------------------------------------------------------------------------" << endl;
            cout << "exit                       : Quit the program" << endl;
            cout << "help                       : Display available commands." << endl;
            cout << "help functions             : Display usage for built in functions." << endl;
            cout << "help naming                : Display acceptable naming conventions for" << endl;
            cout << "                           : variables and functions." << endl;
            cout << "help order                 : Display evaluation order for statements." << endl;
            cout << "variables                  : Display currently stored variable values." << endl;
            cout << "functions                  : Display currently defined functions." << endl;
            cout << "change_res(width,height)   : Change display to match width and height." << endl;
            cout << "set range(xmin , xmax , ymin , ymax)" << endl;
            cout << "                           : Sets the range to show for graphing/plotting." << endl;
            cout << "                           : Default range is x (-10.0,10.0) y (-10.0,10.0)." << endl;
            cout << "plot function_name         : Plots specified function using stored ranges." << endl;
            cout << "                           : Specified function must take zero or one variable." << endl;
            cout << "simplify(function_name , new_function_name)" << endl;
            cout << "                           : Simplify the function and name the new function." << endl;
            cout << "x + y + z                  : Evaluate value of statement." << endl;
            cout << "x = 5.0                    : Assign variable x the value of 5.0." << endl;
            cout << "x =                        : Undefine the variable x." << endl;
            cout << "f([p1],[p2],...) = p1 + p2 : Define new function named f that takes p1 and p2 and" << endl;
            cout << "                           : adds them together." << endl;
         }
         else if (strlen(cstr) > 5) {
            if (strncmp("help functions" , cstr , 14) == 0) {
               cout << "Available built in functions are (exp denotes a valid expression) :" << endl;
               cout << "-------------------------------------------------------------------------------" << endl;
               cout << "negation          : -exp" << endl;
               cout << "ceil              : ceil(exp)" << endl;
               cout << "floor             : floor(exp)" << endl;
               cout << "sine              : sin(exp)" << endl;
               cout << "cosine            : cos(exp)" << endl;
               cout << "tangent           : tan(exp)" << endl;
               cout << "arcsine           : asin(exp)" << endl;
               cout << "arccosine         : acos(exp)" << endl;
               cout << "arctangent        : atan(exp)" << endl;
               cout << "square root       : sqrt(exp)" << endl;
               cout << "absolute value    : fabs(exp)" << endl;
               cout << "logarithm base 10 : log10(exp)" << endl;
               cout << "natural logarithm : ln(exp)" << endl;
               cout << "addition          : a + b" << endl;
               cout << "subtraction       : a - b" << endl;
               cout << "multiplication    : a * b" << endl;
               cout << "division          : a / b" << endl;
               cout << "modulus           : fmod(number , divisor)" << endl;
               cout << "exponentiation    : a ^ b" << endl;
               cout << "                  : pow(base , exponent)" << endl;
               cout << "base logarithm    : logb(number , base)" << endl;
               cout << "arc tangent 2     : atan2(y,x)" << endl;
            }
            else if (strncmp("help naming" , cstr , 11) == 0) {
               cout << "Rules for naming variables and functions :" << endl;
               cout << "-------------------------------------------------------------------------------" << endl;
               cout << "1) Variables may only start with an underscore or with a upper or lower case" << endl;
               cout << "   letter." << endl;
               cout << "2) After 1) is satisfied, variables may then contain underscores, numerals, " << endl;
               cout << "   or upper or lower case letters." << endl;
            }
            else if (strncmp("help order" , cstr , 10) == 0) {
               cout << "Evaluation order of statements :" << endl;
               cout << "-------------------------------------------------------------------------------" << endl;
               cout << "1) All statements inside parentheses are evaluated first." << endl;
               cout << "2) All function arguments are evaluated next." << endl;
               cout << "3) Now all function calls are evaluated." << endl;
               cout << "4) After that the exponentiation operator ^ is evaluated right to left." << endl;
               cout << "5) All multiplication now takes place, from left to right." << endl;
               cout << "6) Now divisions, from left to right." << endl;
               cout << "7) Finally, addition and subtraction are performed in order from left to right." << endl;
            }
            else {
               cout << "Unknown help command!" << endl;
            }
         }
         else {
            cout << "Unknown help command!" << endl;
         }
      }
      else if (strncmp("clear" , cstr , 5) == 0) {
         if (strlen(cstr) == 5) {
            vars.clear();
            FreeUserFunctions();
         } else if (strlen(cstr) > 5) {
            if (strncmp("clear variables" , cstr , 15) == 0) {
               vars.clear();
            }
            else if (strncmp("clear functions" , cstr , 15) == 0) {
               FreeUserFunctions();
            }
            else {
               cout << "Unknown clear command!" << endl;
            }
         }
      }
      else if (strncmp("variables" , cstr , 9) == 0) {
         // print variables
         cout << "Currently stored variable values :" << endl;
         cout << "-------------------------------------------------------------------------------" << endl;
         if (vars.empty()) {cout << "No stored variables." << endl;}
         for (map<string , double>::iterator it = vars.begin() ; it != vars.end() ; ++it) {
            cout << it->first << " = " << FormatDouble(it->second) << endl;
         }
      }
      else if (strncmp("functions" , cstr , 9) == 0) {
         cout << "Currently defined custom functions :" << endl;
         cout << "-------------------------------------------------------------------------------" << endl;
         if (user_function_set.empty()) {cout << "No custom functions defined." << endl;}
         for (map<string , CustomFunction*>::iterator it = user_function_set.begin() ; it != user_function_set.end() ; ++it) {
            cout << (it->second)->PrintDefinition() << endl;
         }
      }
      else if (strncmp("change_res(" , cstr , 11) == 0) {
         if (2 != sscanf(s.c_str() + 11 , "%i,%i)" , &w , &h)) {
            cout << "Improper call to change_res." << endl;
         } else {
            //bool ChangeRes(int card , int width , int height , int color_depth ,
            //   CHANGE_RES_MODE change_res_mode = CRM_GFX_TEXT_ALLEGRO_MESSAGE);
            if ((w > 0) && (h > 0)) {
               GfxModeData oldgfxmode = gfxmode;
               gfxmode.SetResolution(w,h);
               if (!ChangeRes(gfxmode , CRM_DIRECT)) {
                  cout << "Restoring old mode..." << endl;
                  if (!ChangeRes(oldgfxmode , CRM_DIRECT)) {
                     cout << "Couldn't restore old mode! A graphics mode must be set before calling 'plot'." << endl;
                  } else {
                     gfxmode = oldgfxmode;
                  }
               }
               show_mouse(screen);
               DirectToConsole();
            } else {
               cout << "When calling change_res, width and height must be greater than zero." << endl;
            }
         }
      }
      else if (strncmp("set range(" , cstr , 10) == 0) {
         int i = 10;
         int n = 0;
         bool success = false;
         double x_min = 0.0 , x_max = 0.0 , y_min = 0.0 , y_max = 0.0;
         if (1 == sscanf(cstr + i , "%lg%n" , &x_min , &n)) {
            i += n;
            while (isspace(cstr[i])) {++i;}
            while (cstr[i] == ',') {++i;}
            if (1 == sscanf(cstr + i , "%lg%n" , &x_max , &n)) {
               i += n;
               while (isspace(cstr[i])) {++i;}
               while (cstr[i] == ',') {++i;}
               if (1 == sscanf(cstr + i , "%lg%n" , &y_min , &n)) {
                  i += n;
                  while (isspace(cstr[i])) {++i;}
                  while (cstr[i] == ',') {++i;}
                  if (1 == sscanf(cstr + i , "%lg%n" , &y_max , &n)) {
                     success = true;
                  }
               }
            }
         }
         if (success) {
            double tmp = 0.0;
            if (x_min > x_max) {
               tmp = x_min;
               x_min = x_max;
               x_max = tmp;
            }
            if (y_min > y_max) {
               tmp = y_min;
               y_min = y_max;
               y_max = tmp;
            }
///            cout << StringPrintF("%lg %lg %lg %lg" , x_min , x_max , y_min , y_max) << endl;
            if ((x_min == x_max) || (y_min == y_max)) {
               cout << "set range : Range error! Range must be non-zero!" << endl;
            } else {
               xmin = x_min;
               xmax = x_max;
               ymin = y_min;
               ymax = y_max;
               cout << StringPrintF("Graph range is now x {%lg , %lg} y {%lg , %lg}" , xmin , xmax , ymin , ymax) << endl;
            }
         }
         else {
            cout << "Error reading range values!" << endl;
         }
      }
      else if (strncmp("plot " , cstr , 5) == 0) {
         // Read plotting parameters
         char buffer[1024];
         int i = 5;
         int ncharsread1 = 0;
         if (1 != sscanf(cstr + i , "%[_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ]%n" , buffer , &ncharsread1)) {
            cout << "plot : Error reading function name." << endl;
         } else {
            i += ncharsread1;
            sscanf(cstr + i , "%[_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789]" , buffer + ncharsread1);
            map<string , CustomFunction*>::iterator it = user_function_set.find(buffer);
            FunctionBase* f = 0;
            if (it == user_function_set.end()) {
               cout << StringPrintF("plot : Function '%s' not found!" , buffer) << endl;
            } else {
               f = user_function_set[buffer];
               set<string> varset = f->InputSet();
               if (!((varset.size() == 0) || (varset.size() == 1))) {
                  cout << StringPrintF("plot : Number of input variables for '%s' not equal to 0 or 1!" , buffer) << endl;
               } else {
                  string var;
                  if (varset.size() == 1) {
                     var = *(varset.begin());
                  }
                  Plot(f , var , xmin , xmax , ymin , ymax);
                  DirectToConsole();
               }
            }
         }
         
      }
      else if (strncmp("simplify(" , cstr , 9) == 0) {
         char buffer[1024];
         int ncharsread1 = 0;
         int ncharsread2 = 0;
         int i = 9;
         if (1 != sscanf(cstr + i , "%[_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ]%n" , buffer , &ncharsread1)) {
            cout << "simplify : Couldn't read function name!" << endl;
         } else {
            i += ncharsread1;
            if (1 == sscanf(cstr + i , "%[_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789]%n" , buffer + ncharsread1 , &ncharsread2)) {
               i += ncharsread2;
            }
            CustomFunction* cf = 0;
            map<string , CustomFunction*>::iterator it = user_function_set.find(buffer);
            if (it == user_function_set.end()) {
               cout << StringPrintF("Function '%s' not defined!" , buffer) << endl;
            } else {
               cf = it->second;
               while (cstr[i] == ' ') {++i;}
               while (cstr[i] == ',') {++i;}
               while (cstr[i] == ' ') {++i;}
               if (1 != sscanf(cstr + i , "%[_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ]%n" , buffer , &ncharsread1)) {
                  cout << "simplify : Couldn't read new function name!" << endl;
               } else {
                  i += ncharsread1;
                  sscanf(cstr + i , "%[_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789]%n" , buffer + ncharsread1 , &ncharsread2);
                  CustomFunction* simp = new CustomFunction(buffer , cf->Simplify(vars) , false);
                  cout << simp->Print() << endl;
               }
            }
         }
      }
      else {
         // check for expressions, variable assignments, and function definitions
         int equalsindex = StringSearch(cstr , 0 , '=');
         int popenindex = StringSearch(cstr , 0 , '(');
         char namebuf[1024];
         int ncharsread1 = 0;
         
         if (equalsindex != -1) {
            // some kind of assignment
            if ((popenindex != -1) && (popenindex < equalsindex)) {
               // parenthesis involved left of equals sign - should be function definition
               CustomFunction* cf = 0;
               try {
                  cf = new CustomFunction(cstr);
                  cout << "Function '" << cf->Name() << "' registered. Definition :" << endl;
                  cout << cf->PrintDefinition() << endl;
               }
               catch (...) {}
               /// NOTE : The CustomFunction(string) constructor registers itself with user_function_set
               ///      : so this is NOT a memory leak (unless you fail to clean up user_function_set later).
            } else {
               // no parenthesis left of equals sign - variable assignment
               if (1 != sscanf(cstr , "%[_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ]%n" , namebuf , &ncharsread1)) {
                  cout << "Error reading variable name. See 'help naming' for help." << endl;
               }
               sscanf(cstr + ncharsread1 , "%[_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789]" , namebuf + ncharsread1);
               map<string , CustomFunction*>::iterator it = user_function_set.find(namebuf);
               if (it != user_function_set.end()) {
                  cout << namebuf << " is a named function. You cannot use that name for a variable at this time." << endl;
               } else {
                  // variable name is free
                  if (*(cstr + equalsindex + 1) == '\0') {
                     map<string , double>::iterator mit = vars.find(namebuf);
                     if (mit != vars.end()) {
                        vars.erase(mit);
                     }
                  }
                  else {
                     Function* f = 0;
                     try {
                        f = new Function(cstr + equalsindex + 1);
                        try {
                           double d = 0.0;
                           SOLVER_RESULT sr = f->Evaluate(vars , &d);
                           if (sr == RESULT_VALID) {
                              vars[namebuf] = d;
                              cout << StringPrintF("%s = %lg" , namebuf , d) << endl;
                           } else if (sr == RESULT_ERROR) {
                              cout << "Error evaluating '" << f->PrintExpression() << "'" << endl;
                           } else if (sr == RESULT_UNDEFINED) {
                              cout << "Result of '" << f->PrintExpression() << "' is undefined." << endl;
                           }
                        }
                        catch (...) {
                           delete f;
                           f = 0;
                        }
                        if (f) {delete f;}
                     }
                     catch (...) {}
                  }
               }
            }
         }
         else {
            // no assignment - assume this is an expression we need to evaluate
            Function* f = 0;
            try {
               f = new Function(cstr);
               double d = 0.0;
               try {
                  SOLVER_RESULT sr = f->Evaluate(vars , &d);
                  if (sr == RESULT_VALID) {
                     cout << FormatDouble(d) << endl;
                  }
                  else if (sr == RESULT_ERROR) {
                     cout << "An error occurred while evaluating '" << f->PrintExpression() << "'" << endl;
                  }
                  else if (sr == RESULT_UNDEFINED) {
                     cout << "The result of '" << f->PrintExpression() << "' is undefined." << endl;
                  }
               }
               catch (...) {
                  
               }
            }
            catch (...) {
               if (f) {
                  delete f;
                  f = 0;
               }
            }
            if (f) {
               delete f;
            }
         }
         
//         cout << "Unknown command!" << endl;
      }
   }
   
   
   
   
   FreeUserFunctions();
   
   allegro_exit();
   
   return 0;
}
END_OF_MAIN()



void DirectToConsole() {
   scare_mouse();
   acquire_screen();
   clear_to_color(screen , makecol(0,0,0));
   textout_centre_ex(screen , font , "Enter input on console window." , SCREEN_W/2 , SCREEN_H/2 - text_height(font)/2 , makecol(255,255,255) , -1);
   release_screen();
   unscare_mouse();
}



void Plot(FunctionBase* f , string name , double xmin , double xmax , double ymin , double ymax) {

   WidgetHandler gui("gui" , screen , Rectangle(0,0,SCREEN_W,SCREEN_H));
   if (!gui.SetupBitmapHandlers(BG_SOLID , MEMORY , SOLID)) {
      cout << "Error allocating gui's bitmap handlers." << endl;
      return;
   }
   
   Plotter plotter("plotter" , Rectangle(0,0,SCREEN_W,SCREEN_H));

   plotter.SetFunction(f);
   plotter.SetView(xmin , xmax , ymin , ymax);
   plotter.SetVariableName(name);

//   Button(std::string name , BUTTON_TYPE type , BTN_ACTION_TYPE atype , FONT* textfont , 
//         std::string label , const InputGroup& input , const Rectangle& position , UINT wflags = DEFAULT_FLAGS);
   Button quitbtn("quitbtn" , RECTANGLE_BTN , SPRING_BTN , font , "" , input_key_press(KEY_ESC) , Rectangle(0,0,1,1));
   Button savebtn("savebtn" , RECTANGLE_BTN , SPRING_BTN , font , "" , input_key_press(KEY_S) && input_key_held(KEY_ONLY_CTRL) , Rectangle(0,0,1,1));
   quitbtn.SetVisibilityState(false);
   savebtn.SetVisibilityState(false);
   
   
   gui << &plotter << &quitbtn << &savebtn;
   
   if (SetupAndStartTimer(BPS_TO_TIMER(60)) != 0) {
      cout << "Error setting up timer." << endl;
      return;
   }
   
   const WidgetMsg quit(&quitbtn , TOPIC_BUTTON_WIDGET , BUTTON_CLICKED);
   const WidgetMsg save(&savebtn , TOPIC_BUTTON_WIDGET , BUTTON_CLICKED);
   
   bool exit = false;
   
   SetTickCount(0);
   while (!exit && !close_program) {
      scare_mouse();
      acquire_screen();
      gui.Display();
      release_screen();
      unscare_mouse();
      while (GetTickCount() < 1) {rest(1);}
      double dt = GetDeltaTime();
      UpdateInputHandler(dt);
      gui.Update(dt);
      gui.CheckInputs(mx , my);
      while (gui.HasMessages()) {
         WidgetMsg wmsg = gui.TakeNextMessage();
//         cout << wmsg << endl;
         if (wmsg == quit) {
            exit = true;
            break;
         }
         if (wmsg == save) {
            char namebuf[1024];
            sprintf(namebuf , "plot%i.png" , savenum++);
            if (save_bitmap(namebuf , gui.BufferBitmap() , NULL) != 0) {
               cout << "save_bitmap failed - allegro reports " << allegro_error << endl;
            }
         }
      }
   }
   
}

