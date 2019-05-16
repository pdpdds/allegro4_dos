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

#ifndef InputHandler_H
#define InputHandler_H

/** Input Handler to consolidate key, mouse, and (joystick?) input */


#include <vector>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <map>

#include "Eagle/key_handler.h"
#include "Eagle/MouseHandler.hpp"
//#include "Eagle/joystick_handler.h" /// TODO : Contribute this if you like
#include "Eagle/Logging.hpp"




/** TODO : Add support for setting a double click duration? */


enum INPUTSRC {
   KB = 0 ,
   MS = 1 ,
   JS = 2 , /** TODO : JS1, JS2, JS3, ....??? */
   NUMINPUTSRCS = 3
};

enum INPUTTYPE {
   PRESS         = 0,
   RELEASE       = 1,
   HELD          = 2,
   OPEN          = 3,
   DOUBLE_CLICK  = 4,
   NUMINPUTTYPES = 5
};


void SetupInputHandler();

void UpdateInputHandler(double dt_sec);


/** Uniform input
    Input function name template :
Input input_SRC_TYPE(int value);
*/
typedef int (*INPUT_HANDLER)(int);

class Input {

private :
   int src;
   int type;
   int value;
   INPUT_HANDLER input_handler_func;

public :
   /** Constructors */
   Input();
   Input(int source , int input_type , int input_value);
   Input(INPUTSRC source , INPUTTYPE input_type , int input_value);

   /** Reassignment */
   void AssignTo(int source , int input_type , int input_value);

   /** Present time evaluation of input (NOTE : won't change until UpdateInput is called) */
   operator bool() const;
//   operator int() const;
   
   /** operator! can't be used for evaluation, it needs to return an InputGroup with NOT set */
   //  bool operator!() {return !(bool)(*this);} /*      (Implemented globally below)      */

   /** So you can compare Input objects */
   bool operator==(const Input& i);
   bool operator!=(const Input& i);

   /** Text output */
   friend std::ostream& operator<<(std::ostream& os , const Input& input);/// Almost like XML.
   std::ostream& Shortname(std::ostream& os) const ;

   /** Getters */
   const int Source() const {return src;}
   const int Type()   const {return type;}
   const int Value()  const {return value;}


};



/** TODO : If these function names change, update the input_func_text array in InputHandler.cpp */
inline Input input_key_press       (int al_key) {return Input(KB , PRESS        , al_key);}
inline Input input_key_release     (int al_key) {return Input(KB , RELEASE      , al_key);}
inline Input input_key_held        (int al_key) {return Input(KB , HELD         , al_key);}
inline Input input_key_open        (int al_key) {return Input(KB , OPEN         , al_key);}
inline Input input_key_double_click(int al_key) {return Input(KB , DOUBLE_CLICK , al_key);}

inline Input input_mouse_press       (int mbtn) {return Input(MS , PRESS        , mbtn);}
inline Input input_mouse_release     (int mbtn) {return Input(MS , RELEASE      , mbtn);}
inline Input input_mouse_held        (int mbtn) {return Input(MS , HELD         , mbtn);}
inline Input input_mouse_open        (int mbtn) {return Input(MS , OPEN         , mbtn);}
inline Input input_mouse_double_click(int mbtn) {return Input(MS , DOUBLE_CLICK , mbtn);}

/** TODO : Use input_jstk1_* thru input_jstk8_* ??? */
/** What about a function pointer array? input_jstk_press[0](JS_PRIMARY)*/
inline Input input_jstk_press       (int jsbtn) {return Input(JS , PRESS        , jsbtn);}
inline Input input_jstk_release     (int jsbtn) {return Input(JS , RELEASE      , jsbtn);}
inline Input input_jstk_held        (int jsbtn) {return Input(JS , HELD         , jsbtn);}
inline Input input_jstk_open        (int jsbtn) {return Input(JS , OPEN         , jsbtn);}
inline Input input_jstk_double_click(int jsbtn) {return Input(JS , DOUBLE_CLICK , jsbtn);}




/** How to implement the && || ! logic and chaining? With an object tree */

/** Tree class for logical grouping of Input evaluations and storage */

/** Trying to get a bool value from an empty input group will throw an exception, as
    returning true or false would be incorrect, since there's no logical test to evaluate */

class InputGroup {
private :
   bool all_apply;// True means this is operator AND - False means any apply (operator OR)
   bool is;       // False means is not (operator NOT)
   std::vector<Input>      inputs;
   std::vector<InputGroup> nodes;/** ??? This works somehow...? */
   mutable int node_level;

   void SetNodeLevel(int level) const;

   bool IS(bool basis) const {return (is?basis:!basis);}

   bool SameTypeAs(const InputGroup& ig) {return ((all_apply == ig.all_apply) && (is == ig.is));}

   /** TODO : Would equivalence operators be useful?
       InputGroups would probably have to be sorted first */
   bool operator==(const InputGroup& ig) const {return false;}

public :
   /** Constructors */
   InputGroup();
   InputGroup(const Input& i);
   InputGroup(const InputGroup& ig);
   explicit InputGroup(bool use_and , bool use_is , const Input& i);
   explicit InputGroup(bool use_and , bool use_is , const InputGroup& ig);

   /** Vector like behavior */
   void        clear();
   InputGroup& push_back(const Input& i);
   InputGroup& push_back(const InputGroup& ig);

   /** Logical grouping behavior */ /* TODO : Check that grouping of similar types groups inputs and groups */
   InputGroup operator&&(const InputGroup& ig) const ;
   InputGroup operator||(const InputGroup& ig) const ;
   InputGroup operator!() const ;

   /** Evaluation - Don't evaluate empty input groups! */
   operator bool() const;
   /* If it makes you feel better to call a function to evaluate the expression instead
      of just using "if (InputGroup object) {...}", then here you go : */
   bool operator()() const {return (bool)(*this);}

   /** Small utility function for checking whether the input is from a pointer source - it
         checks whether the input is from the mouse or joystick as well as being a button click.
         As long as there is at least one mouse or joystick click input, this will return true. */
   bool PointerInput() const;
   
   bool HasClickInput() const;
   Input FindClickInput() const;// returns input_key_press(KEY_NONE) if there is no click input in the group
   
   /** Text logging */
   friend std::ostream& operator<<(std::ostream& os , const InputGroup& ig);
   std::ostream& ShowLogic(std::ostream& os) const ;
   
   void WriteLines(std::vector<std::string>* strvec);

};

/** Global operators to create InputGroup objects from logically joined Inputs */
InputGroup operator&&(const Input& i1 , const Input& i2);
InputGroup operator||(const Input& i1 , const Input& i2);
InputGroup operator!(const Input& i);

InputGroup operator&&(const Input& i , const InputGroup& ig);
InputGroup operator||(const Input& i , const InputGroup& ig);


/** Input state checks (Not based on buffers, based on states) */
int AnyInputsPressed();
int AnyInputsReleased();
int AnyInputsHeld();


/** Input recording functions for setting Inputs and InputGroups to key/mouse/joystick input */
/** These are both blocking calls - they will wait forever until there is input */
void RecordInput(Input* input);
void RecordInputGroup(InputGroup* input_group);


/** InputAssignments and AssignedInput classes for storing currently assigned inputs with task names */
class AssignedInput {
private :
   std::string taskname;
   InputGroup input;
public :
   
};

class InputAssignments {
private :
   typedef std::map<std::string , InputGroup> TASKINPUTMAP;
   typedef TASKINPUTMAP::iterator TIMIT;

   TASKINPUTMAP task_input_map;
//   std::map<std::string , InputGroup> task_input_map;

public :
   InputAssignments() : task_input_map() {}
   
   /** Should support some map features, like [], should allow for testing the input from the task name */
   void clear();
   InputGroup& operator[](const std::string& str);
   bool Activated(const std::string& str);/** TODO : Is there a better function name than Activated? */

   
   
};

/** Usage example
void AssignPlayerDefaults(InputAssignment& player) {
   player.clear();
   player["Punch"]     = input_key_press(KEY_ONLY_SHIFT);
   player["Kick"]      = input_key_press(KEY_ONLY_ALT);
   player["Jump"]      = input_key_press(KEY_UP);
   player["Crouch"]    = input_key_held(KEY_DOWN);
   player["MoveLeft"]  = input_key_held(KEY_LEFT);
   player["MoveRight"] = input_key_held(KEY_RIGHT);
}

AssignPlayerDefaults(Player);

if (Player.Activated("Punch")) {...}
if (player["Punch"]) {...}  // This should work as well, [] returns an InputGroup& and if () should evaluate it
*/

#endif // InputHandler_H







