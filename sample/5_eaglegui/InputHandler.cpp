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


#include "Eagle/InputHandler.hpp"
#include "Eagle/timer_handler.h"

using std::vector;
using std::ostream;
using std::endl;
using std::string;
using std::map;
using std::ostringstream;



void SetupInputHandler() {
   SetupKeyHandler();
   SetupMouseHandler();
///   SetupJoystickHandler();/// TODO
}



void UpdateInputHandler(double dt_sec) {
   UpdateKeyHandler(dt_sec);
   UpdateMouseHandler(dt_sec);
///   UpdateJoystickHandler(dt_sec);/// TODO
}



/** Private utility arrays for Input::operator<<(ostream&...) and the input handler */

const char* srctext[NUMINPUTSRCS] = {
   "Keyboard",
   "Mouse",
   "Joystick"
};
const char* typetext[NUMINPUTTYPES] = {
   "Press",
   "Release",
   "Held",
   "Open",
   "Double Click"
};

/*
   PRESS RELEASE  HELD  OPEN  DOUBLE_CLICK
KB
MS
JS
*/


/** Joystick unimplemented yet... so if you're reading this, write one! */
const INPUT_HANDLER js_press        = 0;
const INPUT_HANDLER js_release      = 0;
const INPUT_HANDLER js_held         = 0;
const INPUT_HANDLER js_open         = 0;
const INPUT_HANDLER js_double_click = 0;

const INPUT_HANDLER input_handler[NUMINPUTSRCS][NUMINPUTTYPES] = {
   {key_press , key_release , key_held , key_open , key_double_click},
   {mb_press  , mb_release  , mb_held  , mb_open  , mb_double_click},
   {js_press  , js_release  , js_held  , js_open  , js_double_click}
};

const char* input_func_text[NUMINPUTSRCS][NUMINPUTTYPES] = {
   {"input_key_press"   , "input_key_release"   , "input_key_held"   , "input_key_open"   , "input_key_double_click"},
   {"input_mouse_press" , "input_mouse_release" , "input_mouse_held" , "input_mouse_open" , "input_mouse_double_click"},
   {"input_jstk_press"  , "input_jstk_release"  , "input_jstk_held"  , "input_jstk_open"  , "input_jstk_double_click" }
};


typedef const char* (*VALUE_TO_NAME_FUNC)(int);

/* TODO : Placeholder joystick button naming function because you haven't written it yet */
const char* joystickcode_to_name(int jsbtn) {
   return "[Unimplemented - Joystick Button Naming]";
}

const VALUE_TO_NAME_FUNC value_to_name_func[NUMINPUTSRCS] = {
   keycode_to_name,/** See the TODO in ostream& operator<<(ostream& os , const Input& input) below */
   mousecode_to_name,
   joystickcode_to_name
};





/** #################              Input class methods                ############## */

Input::Input() :
      src(KB) , type(PRESS) , value(KEY_NONE) , input_handler_func(input_handler[KB][PRESS])
      {}/** Evaluating an unassigned Input() object will always return false */



Input::Input(int source , int input_type , int input_value) :
      src(source) , type(input_type) , value(input_value) , input_handler_func(input_handler[source][input_type])
      {}



Input::Input(INPUTSRC source , INPUTTYPE input_type , int input_value) :
      src(source) , type(input_type) , value(input_value) , input_handler_func(input_handler[source][input_type])
      {}



void Input::AssignTo(int source , int input_type , int input_value) {
   src = source;
   type = input_type;
   value = input_value;
   input_handler_func = input_handler[source][input_type];
}



Input::operator bool() const {
   /** Which to use? */
   /* A : Function pointer array - is inlining even possible then? */
   ///return input_handler[src][type](value);
   /* A2 : Member function pointer - only one array lookup per assignment */
   return input_handler_func(value);
   /* B : Branching - inlined, but will the branching slow it down more than it gains? */
   switch (src) {
      case KB :
         switch (type) {
            case PRESS   : return key_press(value);
            case RELEASE : return key_release(value);
            case HELD    : return key_held(value);
            case OPEN    : return key_open(value);
         }
         break;
      case MS :
         switch (type) {
            case PRESS   : return mb_press(value);
            case RELEASE : return mb_release(value);
            case HELD    : return mb_held(value);
            case OPEN    : return mb_open(value);
         }
         break;
      case JS :
         /// TODO : Joystick handler unimplemented, once done, remove the next two lines
         ASSERT(0);
         return false;
         switch (type) {
            case PRESS   : return js_press(value);
            case RELEASE : return js_release(value);
            case HELD    : return js_held(value);
            case OPEN    : return js_open(value);
         }
         break;
   }
   return false;
}



bool Input::operator==(const Input& i) {
   return (
      (src == i.src) &&
      (type == i.type) &&
      (value == i.value) &&
      (input_handler_func == i.input_handler_func)
   );
}



bool Input::operator!=(const Input& i) {return !(*this == i);}



ostream& operator<<(ostream& os , const Input& input) {
   /** TODO : If the scancode enumeration is extended for things like KEY_CTRL_EITHER,
               then scancode_to_name will have to be extended with a wrapper function. */
   os << "Input <src = \"" << srctext[input.src] << "\" type = \"" << typetext[input.type];
   os << "\" value = \"" << value_to_name_func[input.src](input.value) << "\" >";
   return os;
}



ostream& Input::Shortname(ostream& os) const {
   os << input_func_text[src][type] << "(" << value_to_name_func[src](value) << ")";
   return os;
}





/** #################            InputGroup class methods               ################# */

void InputGroup::SetNodeLevel(int level) const {
   node_level = level;
   const int num_nodes = nodes.size();
   for (int i = 0 ; i < num_nodes ; ++i) {nodes[i].SetNodeLevel(level + 1);}
}



InputGroup::InputGroup() : all_apply(true) , is(true) , inputs() , nodes() , node_level(0) {}



InputGroup::InputGroup(const Input& i) : all_apply(true) , is(true) , inputs() , nodes() , node_level(0) {
   inputs.push_back(i);
}



InputGroup::InputGroup(const InputGroup& ig) :
            all_apply(ig.all_apply) , is(ig.is) , inputs() , nodes() , node_level(ig.node_level) {
/** TODO : where is this warning coming from? -
/mingw/include/c++/3.4.5/bits/stl_vector.h: In member function `std::vector<_Tp, _Alloc>& std::vector<_Tp, _Alloc>::operator=(const std::vector<_Tp, _Alloc>&) [with _Tp = InputGroup, _Alloc = std::allocator<InputGroup>]':
/mingw/include/c++/3.4.5/bits/stl_vector.h:715: warning: '__result' might be used uninitialized in this function
/mingw/include/c++/3.4.5/bits/stl_uninitialized.h:82: warning: '__cur' might be used uninitialized in this function
/mingw/include/c++/3.4.5/bits/stl_uninitialized.h:82: warning: '__cur' might be used uninitialized in this function
*/
   inputs = ig.inputs;
   nodes = ig.nodes;
}



InputGroup::InputGroup(bool use_and , bool use_is , const Input& i) :
            all_apply(use_and) , is(use_is) , inputs() , nodes() , node_level(0) {
   inputs.push_back(i);
}



InputGroup::InputGroup(bool use_and , bool use_is , const InputGroup& ig) :
            all_apply(use_and) , is(use_is) , inputs() , nodes() , node_level(0) {
   push_back(ig);
   /** Previous version, doesn't allow for consolidation 
   ig.SetNodeLevel(1);
   nodes.push_back(ig);
   */
}



void InputGroup::clear() {
   inputs.clear();
   nodes.clear();
}



InputGroup& InputGroup::push_back(const Input& i) {
   inputs.push_back(i);
   return *this;
}



InputGroup& InputGroup::push_back(const InputGroup& ig) {

   /** This function is Re-entrant */
   /** If the InputGroup to add is the same type as the current one,
         add their inputs to this InputGroup's input list and try to 
         add their nodes to this InputGroup's node list by re entering
         the function.
         If the InputGroup is not the same type then it needs to be added to
         the nodes list.
   */

   if (SameTypeAs(ig)) {
      const int num_new = ig.inputs.size();
      for (int i = 0 ; i < num_new ; ++i) {
         push_back(ig.inputs[i]);
      }
      const int num_new_groups = ig.nodes.size();
      for (int i = 0 ; i < num_new_groups ; ++i) {
         push_back(ig.nodes[i]);
      }
   } else {
      /* Original behavior */
      ig.SetNodeLevel(node_level + 1);
      nodes.push_back(ig);
   }

   return *this;
}


/** TODO : Working here - the behavior of && and || may be able to consolidate more inputs */

InputGroup InputGroup::operator&&(const InputGroup& ig) const {
   InputGroup clump(true , true , ig);
   clump.push_back(*this);
   return clump;
}



InputGroup InputGroup::operator||(const InputGroup& ig) const {
   InputGroup clump(false , true , ig);
   clump.push_back(*this);
   return clump;
}



InputGroup InputGroup::operator!() const {
   /** Return an InputGroup with the value of 'is' negated? Or a boolean value?      */
   /** Input group ==> " !(Input(A) || Input(B)) " ==> " !InputGroup(A || B) "       */
   /** As in, we don't want to leave a bool value in a grouping chain.               */
   /** This should still work with evaluations though, as "if (!InputGroup)" should  */
   /** end up as !InputGroup::operator bool.                                          */

   /* TODO : Can't this just return a reference to itself with the value of 'is' negated? */
   /*      - No, not for constant objects, so a new copy needs to be created and returned
   is = !is;
   return (*this);
   */
   InputGroup ig(*this);
   ig.is = !(ig.is);
   return ig;
}



InputGroup::operator bool() const {
   bool ret;
   const int num_inputs = inputs.size();
   const int num_nodes = nodes.size();
   if ((num_inputs + num_nodes) == 0) {
      return false;
      /// throw InputGroupError("Trying to evaluate an empty input group!");
   }
   if (all_apply) {
      ret = true;
      /** AND the inputs and nodes */
      for (int i = 0 ; i < num_inputs ; ++i) {
         /** If any are false, the whole AND group is false */
         if (!(bool)inputs[i]) {return IS(false);}
      }
      for (int i = 0 ; i < num_nodes ; ++i) {
         if (!(bool)nodes[i]) {return IS(false);}
      }
      
   } else {
      ret = false;
      /** OR the inputs and nodes */
      for (int i = 0 ; i < num_inputs ; ++i) {
         /** If any are true, the whole OR group is true */
         if (inputs[i]) {return IS(true);}
      }
      for (int i = 0 ; i < num_nodes ; ++i) {
         if (nodes[i]) {return IS(true);}
      }
   }
   return IS(ret);
}



ostream& operator<<(ostream& os , const InputGroup& ig) {
   Indenter indent(ig.node_level,3);
   Indenter indent_input(ig.node_level + 1 , 3);
   os << indent << "InputGroup [Logical ";
   os << (ig.all_apply?"AND":"OR") << " , " << (ig.is?"IS":"NOT") << "]" << endl;
   os << indent << "{" << endl;
   const int num_inputs = ig.inputs.size();
   const int num_nodes  = ig.nodes.size();
   for (int i = 0 ; i < num_inputs ; ++i) {
      os << indent_input << ig.inputs[i] << endl;
   }
   for (int i = 0 ; i < num_nodes ; ++i) {
      os << ig.nodes[i] << endl;
   }
   os << indent << "}" << endl;
   return os;
}



void InputGroup::WriteLines(vector<string>* strvec) {
   ostringstream ostr;
   Indenter indent(node_level,3);
   Indenter indent_input(node_level + 1 , 3);
   ostr << indent << "InputGroup [Logical ";
   ostr << (all_apply?"AND":"OR") << " , " << (is?"IS":"NOT") << "]";
   strvec->push_back(ostr.str());
   ostr.str("");
   ostr << indent << "{";
   strvec->push_back(ostr.str());
   ostr.str("");

   const int num_inputs = inputs.size();
   const int num_nodes  = nodes.size();
   for (int i = 0 ; i < num_inputs ; ++i) {
      ostr << indent_input << inputs[i];
      strvec->push_back(ostr.str());
      ostr.str("");
   }
   for (int i = 0 ; i < num_nodes ; ++i) {
      nodes[i].WriteLines(strvec);
//      ostr << ig.nodes[i] << endl;
   }
   ostr << indent << "}";
   strvec->push_back(ostr.str());
   ostr.str("");
}



ostream& InputGroup::ShowLogic(ostream& os) const {
   const int num_inputs = inputs.size();
   const int num_nodes = nodes.size();
///   const int num_items = inputs.size() + nodes.size();
///   int item_num = 0;
   
   if (!is) {os << "!";}
   os << "(";
   for (int i = 0 ; i < num_inputs ; ++i) {
      inputs[i].Shortname(os);
      if ((i + 1) != num_inputs) {
         if (all_apply) {
            os << " && ";
         } else {
            os << " || ";
         }
      }
///      ++item_num;
   }
   for (int i = 0 ; i < num_nodes ; ++i) {
      if (( num_inputs > 0) || (i > 0)) {
         if (all_apply) {
            os << " && ";
         } else {
            os << " || ";
         }
      }
      nodes[i].ShowLogic(os);
   }
   os << ")";
   return os;
}




/** ######   Global operators for joining Input objects into InputGroup objects   ######## */

InputGroup operator&&(const Input& i1 , const Input& i2) {
   return InputGroup(true , true , i1).push_back(i2);
}



InputGroup operator||(const Input& i1 , const Input& i2) {
   return InputGroup(false , true , i1).push_back(i2);
}



InputGroup operator&&(const Input& i , const InputGroup& ig) {
   InputGroup ignew(ig);
   return (ignew && i);
}



InputGroup operator||(const Input& i , const InputGroup& ig) {
   InputGroup ignew(ig);
   return (ignew || i);
}



InputGroup operator!(const Input& i) {
   return InputGroup(true , false , i);
}



bool InputGroup::PointerInput() const {
   for (unsigned int i = 0 ; i < inputs.size() ; ++i) {
      if ((inputs[i].Source() != KB) && (inputs[i].Type() == PRESS)) {return true;}
   }
   for (unsigned int i = 0 ; i < nodes.size() ; ++i) {
      if (nodes[i].PointerInput()) {return true;}
   }
   return false;
}



bool InputGroup::HasClickInput() const {
   for (unsigned int i = 0 ; i < inputs.size() ; ++i) {
      if (inputs[i].Type() == PRESS) {return true;}
   }
   for (unsigned int i = 0 ; i < nodes.size() ; ++i) {
      if (nodes[i].HasClickInput()) {return true;}
   }
   return false;
}



Input InputGroup::FindClickInput() const {
   const Input false_return = input_key_press(KEY_NONE);
   for (unsigned int i = 0 ; i < inputs.size() ; ++i) {
      if (inputs[i].Type() == PRESS) {return inputs[i];}
   }
   for (unsigned int i = 0 ; i < nodes.size() ; ++i) {
      if (nodes[i].FindClickInput() != false_return) {return nodes[i].FindClickInput();}
   }
   return false_return;
}



int AnyInputsPressed() {
   if (AnyKeysPressed() || AnyMbtnsPressed() /* TODO || AnyJsbtnsPressed() */) {
      return 1;
   }
   return 0;
}



int AnyInputsReleased() {
   if (AnyKeysReleased() || AnyMbtnsReleased() /* TODO || AnyJsbtnsReleased() */) {
      return 1;
   }
   return 0;
}



int AnyInputsHeld() {
   if (AnyKeysHeld() || AnyMbtnsHeld() /* TODO || AnyJsbtnsHeld() */) {
      return 1;
   }
   return 0;
}




/** Input recording functions for setting Inputs and InputGroups to key/mouse/joystick input */

void RecordInput(Input* input) {

///   A single input key/button is easy enough to record, just take the first key / mouse / joystick
///   press that occurs after the function is called. How to do it for modifier key combinations though?
///   If (input_press != Any modifier key) {Record the input along with any modifier keys present?}
///   UpdateInputHandler needs to be called from within the routine though, and what about
///   starting/stopping the timers?
   
   ASSERT(input);
   bool wait = true;
   bool skip = false;

   int src = KB;
   const int type = PRESS;
   int value = KEY_NONE;
   
   const int old_ticks = GetTickCount();

   /** Continually check inputs until a key is pressed */
   while (wait) {
      rest(16);
      UpdateInputHandler(0.16);
      if (AnyInputsPressed()) {
         /** Find the input that was pressed */
         for (int i = 1 ; i < KEY_MAX ; ++i) {
            if (key_press(i)) {
               src = KB;
               value = i;
               skip = true;
               break;
            }
         }
         if (!skip) {
            for (int i = 0 ; i < NUM_MBTNS ; ++i) {
               if (mb_press(i)) {
                  src = MS;
                  value = i;
                  skip = true;
                  break;
               }
            }
            /* Placeholder for future joystick code
            if (!skip) {
               for (int i = 0 ; i < NUM_JSBTNS ; ++i) {
                  if (js_press(i)) {
                     src = JS;
                     value = i;
                     break;
                  }
               }
            }
            */
         }
         input->AssignTo(src,type,value);
         wait = false;
      }
   }
   
   /* So the timer doesn't have a boatload of ticks on it when the function returns */
   SetTickCount(old_ticks);
}




/* From include\allegro\keyboard.h
   __allegro_KEY_LSHIFT       = 115,
   __allegro_KEY_RSHIFT       = 116,
   __allegro_KEY_LCONTROL     = 117,
   __allegro_KEY_RCONTROL     = 118,
   __allegro_KEY_ALT          = 119,
   __allegro_KEY_ALTGR        = 120,

*/
void RecordInputGroup(InputGroup* input_group) {
   ASSERT(input_group);
   
   InputGroup ig;
   bool wait = true;
   bool skip = false;
   bool ignore = false;

   int src = KB;
   const int type = PRESS;
   int value = KEY_NONE;
   
   const int old_ticks = GetTickCount();

   /** Continually check inputs until a key is pressed, skipping modifier key presses,
       and recording any other keys held at the time. This means you can have
       modifier keys included as held keys, but not other ones. To allow the greatest number
       of key combinations from the modifier keys, the held keys will only include the
       extensions to the allegro key array labeled KEY_ONLY_*.
   */
   while (wait) {
      rest(16);
      UpdateInputHandler(0.16);
      if (AnyInputsPressed()) {
         ignore = false;
         /* Skip modifier keys */
         for (int i = KEY_LSHIFT ; i <= KEY_ALTGR ; ++i) {
            if (key_press(i)) {ignore = true;}
         }
         if (!ignore) {
            /** Find the input that was pressed */
            for (int i = 1 ; i < KEY_MAX ; ++i) {
               if (key_press(i)) {
                  src = KB;
                  value = i;
                  skip = true;
                  break;
               }
            }
            if (!skip) {
               for (int i = 0 ; i < NUM_MBTNS ; ++i) {
                  if (mb_press(i)) {
                     src = MS;
                     value = i;
                     skip = true;
                     break;
                  }
               }
               /* Placeholder for future joystick code
               if (!skip) {
                  for (int i = 0 ; i < NUM_JSBTNS ; ++i) {
                     if (js_press(i)) {
                        src = JS;
                        value = i;
                        break;
                     }
                  }
               }
               */
            }
            ig.push_back(Input(src,type,value));
            /** Add any modifier keys currently held down */
            for (int i = KEY_ONLY_SHIFT ; i <= KEY_ONLY_CTRL_ALT ; ++i) {
               if (key_held(i)) {
                  ig.push_back(Input(KB, HELD , i));
               }
            }
            wait = false;
         }
      }
   }
   *input_group = ig;
   
   /* So the timer doesn't have a boatload of ticks on it when the function returns */
   SetTickCount(old_ticks);
}



void InputAssignments::clear() {
   task_input_map.clear();
}



InputGroup& InputAssignments::operator[](const string& str) {
   return task_input_map[str];
}



bool InputAssignments::Activated(const string& str) {/** TODO : Is there a better function name than Activated? */
   TIMIT timit = task_input_map.find(str);
   if (timit != task_input_map.end()) {
      return ((bool)(timit->second));
   }
   return false;// InputGroup not found in map
}






