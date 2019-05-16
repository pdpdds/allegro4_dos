

#include <cmath>
#include <cstring>
#include <cstdio>
#include <cassert>

#include <allegro.h>

#include "Functions.hpp"

using std::map;
using std::string;
using std::set;
using std::vector;


/// Function names

struct ltstr
{
   bool operator()(const char* s1, const char* s2) const
   {
      return strcmp(s1, s2) < 0;
   }
};

bool CompareFunctionBase(FunctionBase* t1 , FunctionBase* t2) {
   /// TODO : What is this for again?
   assert(false);
   if (t1->IsConstant()) {
      if (t2->IsConstant()) {
         double d1 = 0.0;
         double d2 = 0.0;
         map<string , double> vars;
         SOLVER_RESULT sr1 = t1->Evaluate(vars , &d1);
         SOLVER_RESULT sr2 = t2->Evaluate(vars , &d2);
         if (sr1 == RESULT_VALID) {
            
         }
         if ((sr1 == RESULT_VALID) && (sr2 == RESULT_VALID)) {
            return d1 < d2;
         } else {
            
         }
      } else {
         // t1 constant, t2 variable
         return false;
      }
   } else {
      if (t2->IsConstant()) {
         // t1 variable , t2 constant
         return true;
      } else {
         // t1 and t2 variable
         // ???
      }
   }
}

class CompareFunctionTerms {
private :
   static map<string , FunctionBase*>* terms;
public :
   static bool Compare(string term1 , string term2) {
      if (!terms) {throw EagleError("CompareFunctionTerms::Compare - terms is NULL!\n");}
      map<string , FunctionBase*>::iterator it1 = terms->find(term1);
      map<string , FunctionBase*>::iterator it2 = terms->find(term2);
      if (it1 == terms->end()) {throw EagleError("CompareFunctionTerms::Compare - term1 not found!\n");}
      if (it2 == terms->end()) {throw EagleError("CompareFunctionTerms::Compare - term2 not found!\n");}
      return CompareFunctionBase(it1->second , it2->second);
   }
   void SetTerms(map<string , FunctionBase*>* t) {terms = t;}
};

map<string , FunctionBase*>* CompareFunctionTerms::terms = 0;


const int NUM_UNARY_FUNCS = 12;

const char* unary_names[NUM_UNARY_FUNCS] = {
   "ceil",
   "floor",
   "sin",
   "cos",
   "tan",
   "asin",
   "acos",
   "atan",
   "sqrt",
   "fabs",
   "log10",
   "ln"
};

const int NUM_BINARY_FUNCS = 4;

const char* binary_names[NUM_BINARY_FUNCS] = {
   "logb",
   "atan2",
   "fmod",
   "pow"
};


map<string , CustomFunction*> user_function_set;
set<const char* , ltstr> unary_function_set(unary_names , unary_names + NUM_UNARY_FUNCS);
set<const char* , ltstr> binary_function_set(binary_names , binary_names + NUM_BINARY_FUNCS);



void FreeUserFunctions() {
   for (map<string , CustomFunction*>::iterator it = user_function_set.begin() ; it != user_function_set.end() ; ) {
      delete it->second;
      // When the CustomFunction destructor is called, it unregisters itself with user_function_set
      // and removes itself from the map
      it = user_function_set.begin();
   }
}



set<string> GetFunctionSet() {
   set<string> fset;
   for (map<string , CustomFunction*>::iterator it = user_function_set.begin() ; it != user_function_set.end() ; ++it) {
      fset.insert(it->first);
   }
   for (set<const char* , ltstr>::iterator it = unary_function_set.begin() ; it != unary_function_set.end() ; ++it) {
      fset.insert(string(*it));
   }
   for (set<const char* , ltstr>::iterator it = binary_function_set.begin() ; it != binary_function_set.end() ; ++it) {
      fset.insert(string(*it));
   }
   return fset;
}


/// -------------------------------     ConstantFunction      --------------------------------------------



SOLVER_RESULT ConstantFunction::Evaluate(const std::map<std::string , double>& valuemap , double* result) {
   ASSERT(result);
   *result = c;
   return RESULT_VALID;
}



FunctionBase* ConstantFunction::Simplify(const std::map<std::string , double>& valuemap) {
   return new ConstantFunction(c);
}



/// -------------------------------     VariableFunction      --------------------------------------------



VariableFunction::VariableFunction(const std::string& variable) :
      FunctionBase(0),
      var(variable)
{
   inputset.insert(var);
}



SOLVER_RESULT VariableFunction::Evaluate(const std::map<std::string , double>& valuemap , double* result) {
   ASSERT(result);
   if (valuemap.find(var) == valuemap.end()) {
      throw EagleError(StringPrintF("VariableFunction::Evaluate - %s not found in valuemap!" , var.c_str()));
   }
///   *result = valuemap[var];/// std::map doesn't have a const ValueType& operator[] const function.
/// Actually, it can't have a const operator[] because it might insert a new value into the map.
   *result = const_cast<std::map<std::string , double>&>(valuemap)[var];// We didn't alter it. Nyaaah!
   return RESULT_VALID;
}



bool VariableFunction::IsConstant(const std::map<std::string , double>& valuemap) {
   return valuemap.find(var) != valuemap.end();
}



FunctionBase* VariableFunction::Simplify(const std::map<std::string , double>& valuemap) {
   map<string , double>::const_iterator it = valuemap.find(var);
   if (it != valuemap.end()) {
      double d = 0.0;
      Evaluate(valuemap , &d);// var is in the map, result of Evaluate is guaranteed to be RESULT_VALID
      return new ConstantFunction(d);
   } else {
      return Clone();
   }
   // not reached
   return 0;
}



/// -------------------------------     UnaryFunction      --------------------------------------------



UnaryFunction::UnaryFunction(FunctionBase* term1) :
      FunctionBase(1),
      term(term1)
{
   ASSERT(term1);
   inputset.insert(term1->InputSet().begin() , term1->InputSet().end());
}



vector<FunctionBase*> UnaryFunction::Terms() {
   vector<FunctionBase*> v = term->Terms();
   v.push_back(term);
   return v;
}



bool UnaryFunction::WillCall(FunctionBase* f) {
   if (f == term) {return true;}
   return term->WillCall(f);
}



bool UnaryFunction::IsConstant() {
   return term->IsConstant();
}



bool UnaryFunction::IsConstant(const std::map<std::string , double>& valuemap) {
   return term->IsConstant(valuemap);
}



FunctionBase* UnaryFunction::Simplify(const std::map<std::string , double>& valuemap) {
   if (term->IsConstant(valuemap)) {
      double d = 0.0;
      SOLVER_RESULT sr = Evaluate(valuemap , &d);
      if (sr == RESULT_VALID) {
         return new ConstantFunction(d);
      } else {
         return 0;
      }
   }
   return Clone();
}



/// -------------------------------     BinaryFunction      --------------------------------------------



BinaryFunction::BinaryFunction(FunctionBase* leftterm , FunctionBase* rightterm) :
      FunctionBase(2),
      lterm(leftterm),
      rterm(rightterm)
{
   ASSERT(leftterm);
   ASSERT(rightterm);
   inputset.insert(leftterm->InputSet().begin() , leftterm->InputSet().end());
   inputset.insert(rightterm->InputSet().begin() , rightterm->InputSet().end());
}



vector<FunctionBase*> BinaryFunction::Terms() {
   vector<FunctionBase*> v = lterm->Terms();
   vector<FunctionBase*> v2 = rterm->Terms();
   v.insert(v.end() , v2.begin() , v2.end());
   v.push_back(lterm);
   v.push_back(rterm);
   return v;
}



bool BinaryFunction::WillCall(FunctionBase* f) {
   if (f == lterm) {return true;}
   if (f == rterm) {return true;}
   if (lterm->WillCall(f)) {return true;}
   if (rterm->WillCall(f)) {return true;}
   return false;
}



bool BinaryFunction::IsConstant() {
   return (lterm->IsConstant() && rterm->IsConstant());
}



bool BinaryFunction::IsConstant(const std::map<std::string , double>& valuemap) {
   return (lterm->IsConstant(valuemap) && rterm->IsConstant(valuemap));
}



/// -------------------------------     Unary Function classes      --------------------------------------------



// -------------------------------     NegateFunction      -------------------------------------------------



SOLVER_RESULT NegateFunction::Evaluate(const std::map<std::string , double>& valuemap , double* result) {
   ASSERT(result);
   double input;
   SOLVER_RESULT sr = term->Evaluate(valuemap , &input);
   if (sr != RESULT_VALID) {return sr;}
   *result = -input;
   return RESULT_VALID;
}



std::string NegateFunction::Print() {
   string output = "-";
   if (term->IsCompoundStatement()) {
      output += "(";
      output += term->Print();
      output += ")";
   } else {
      output += term->Print();
   }
   return output;
}



// -------------------------------      CeilFunction      ------------------------------------------



SOLVER_RESULT CeilFunction::Evaluate(const std::map<std::string , double>& valuemap , double* result) {
   ASSERT(result);
   double input;
   SOLVER_RESULT sr = term->Evaluate(valuemap , &input);
   if (sr != RESULT_VALID) {return sr;}
   *result = ceil(input);
   return RESULT_VALID;
}



std::string CeilFunction::Print() {
   string output = "ceil(";
   output += term->Print();
   output += ")";
   return output;
}



// --------------------------------     FloorFunction     --------------------------------------------



SOLVER_RESULT FloorFunction::Evaluate(const std::map<std::string , double>& valuemap , double* result) {
   ASSERT(result);
   double input;
   SOLVER_RESULT sr = term->Evaluate(valuemap , &input);
   if (sr != RESULT_VALID) {return sr;}
   *result = floor(input);
   return RESULT_VALID;
}



std::string FloorFunction::Print() {
   string output = "floor(";
   output += term->Print();
   output += ")";
   return output;
}



// -------------------------------      SineFunction     -------------------------------------------



SOLVER_RESULT SineFunction::Evaluate(const std::map<std::string , double>& valuemap , double* result) {
   ASSERT(result);
   double input;
   SOLVER_RESULT sr = term->Evaluate(valuemap , &input);
   if (sr != RESULT_VALID) {return sr;}
   *result = sin(input);
   return RESULT_VALID;
}



std::string SineFunction::Print() {
   string output = "sin(";
   output += term->Print();
   output += ")";
   return output;
}



// -------------------------------      CosineFunction     ------------------------------------------



SOLVER_RESULT CosineFunction::Evaluate(const std::map<std::string , double>& valuemap , double* result) {
   ASSERT(result);
   double input;
   SOLVER_RESULT sr = term->Evaluate(valuemap , &input);
   if (sr != RESULT_VALID) {return sr;}
   *result = cos(input);
   return RESULT_VALID;
}



std::string CosineFunction::Print() {
   string output = "cos(";
   output += term->Print();
   output += ")";
   return output;
}



// -------------------------------      TangentFunction    ---------------------------------------------



SOLVER_RESULT TangentFunction::Evaluate(const std::map<std::string , double>& valuemap , double* result) {
   ASSERT(result);
   double input;
   SOLVER_RESULT sr = term->Evaluate(valuemap , &input);
   if (sr != RESULT_VALID) {return sr;}
   *result = tan(input);
   return RESULT_VALID;
}



std::string TangentFunction::Print() {
   string output = "tan(";
   output += term->Print();
   output += ")";
   return output;
}



// ------------------------------     ArcSineFunction     -----------------------------------------



SOLVER_RESULT ArcSineFunction::Evaluate(const std::map<std::string , double>& valuemap , double* result) {
   ASSERT(result);
   double input;
   SOLVER_RESULT sr = term->Evaluate(valuemap , &input);
   if (sr != RESULT_VALID) {return sr;}
   if ((input < -1.0) || (input > 1.0)) {
      return RESULT_UNDEFINED;
   }
   *result = asin(input);
   return RESULT_VALID;
}



std::string ArcSineFunction::Print() {
   string output = "asin(";
   output += term->Print();
   output += ")";
   return output;
}



// -----------------------------   ArcCosineFunction     ---------------------------------------------



SOLVER_RESULT ArcCosineFunction::Evaluate(const std::map<std::string , double>& valuemap , double* result) {
   ASSERT(result);
   double input;
   SOLVER_RESULT sr = term->Evaluate(valuemap , &input);
   if (sr != RESULT_VALID) {return sr;}
   if ((input < -1.0) || (input > 1.0)) {
      return RESULT_UNDEFINED;
   }
   *result = acos(input);
   return RESULT_VALID;
}



std::string ArcCosineFunction::Print() {
   string output = "acos(";
   output += term->Print();
   output += ")";
   return output;
}



// ----------------------------     ArcTangentFunction      --------------------------------------------



SOLVER_RESULT ArcTangentFunction::Evaluate(const std::map<std::string , double>& valuemap , double* result) {
   ASSERT(result);
   double input;
   SOLVER_RESULT sr = term->Evaluate(valuemap , &input);
   if (sr != RESULT_VALID) {return sr;}
   *result = atan(input);
   return RESULT_VALID;
}



std::string ArcTangentFunction::Print() {
   string output = "atan(";
   output += term->Print();
   output += ")";
   return output;
}



// ----------------------------    SquareRootFunction     -----------------------------------------



SOLVER_RESULT SquareRootFunction::Evaluate(const std::map<std::string , double>& valuemap , double* result) {
   ASSERT(result);
   double input;
   SOLVER_RESULT sr = term->Evaluate(valuemap , &input);
   if (sr != RESULT_VALID) {return sr;}
   if (input < 0.0) {
      return RESULT_UNDEFINED;
   }
   *result = sqrt(input);
   return RESULT_VALID;
}



std::string SquareRootFunction::Print() {
   string output = "sqrt(";
   output += term->Print();
   output += ")";
   return output;
}


// ----------------------------    AbsoluteValueFunction    ------------------------------------------



SOLVER_RESULT AbsoluteValueFunction::Evaluate(const std::map<std::string , double>& valuemap , double* result) {
   ASSERT(result);
   double input;
   SOLVER_RESULT sr = term->Evaluate(valuemap , &input);
   if (sr != RESULT_VALID) {return sr;}
   *result = fabs(input);
   return RESULT_VALID;
}



std::string AbsoluteValueFunction::Print() {
   string output = "fabs(";
   output += term->Print();
   output += ")";
   return output;
}



// ---------------------------     Logarithm10Function     -----------------------------------------------



SOLVER_RESULT Logarithm10Function::Evaluate(const std::map<std::string , double>& valuemap , double* result) {
   ASSERT(result);
   double input;
   SOLVER_RESULT sr = term->Evaluate(valuemap , &input);
   if (sr != RESULT_VALID) {return sr;}
   if (input <= 0.0) {
      return RESULT_UNDEFINED;
   }
   *result = log10(input);
   return RESULT_VALID;
}



std::string Logarithm10Function::Print() {
   string output = "log10(";
   output += term->Print();
   output += ")";
   return output;
}



// ---------------------------     NaturalLogarithmFunction    --------------------------------------------



SOLVER_RESULT NaturalLogarithmFunction::Evaluate(const std::map<std::string , double>& valuemap , double* result) {
   ASSERT(result);
   double input;
   SOLVER_RESULT sr = term->Evaluate(valuemap , &input);
   if (sr != RESULT_VALID) {return sr;}
   if (input <= 0.0) {
      return RESULT_UNDEFINED;
   }
   *result = log(input);
   return RESULT_VALID;
}



std::string NaturalLogarithmFunction::Print() {
   string output = "ln(";
   output += term->Print();
   output += ")";
   return output;
}



/// -----------------------------------     Binary functions     -----------------------------------------



// -----------------------------------     AddFunction     ----------------------------------------------



SOLVER_RESULT AddFunction::Evaluate(const std::map<std::string , double>& valuemap , double* result) {
   ASSERT(result);
   double l,r;
   SOLVER_RESULT sr1 = lterm->Evaluate(valuemap , &l);
   SOLVER_RESULT sr2 = rterm->Evaluate(valuemap , &r);
   if ((sr1 != RESULT_VALID) || (sr2 != RESULT_VALID)) {
      if ((sr1 == RESULT_ERROR) || (sr2 == RESULT_ERROR)) {return RESULT_ERROR;} else {return RESULT_UNDEFINED;}
   }
   *result = l + r;
   return RESULT_VALID;
}



std::string AddFunction::Print() {
   string output;
   output += lterm->Print();
   output += " + ";
   output += rterm->Print();
   return output;
}



FunctionBase* AddFunction::Simplify(const std::map<std::string , double>& valuemap) {
   if (lterm->IsConstant(valuemap) && rterm->IsConstant(valuemap)) {
      double d = 0.0;
      SOLVER_RESULT sr = Evaluate(valuemap , &d);
      if (sr == RESULT_VALID) {
         return new ConstantFunction(d);
      } else {
         return 0;
      }
   } else {
      FunctionBase* l = lterm->Simplify(valuemap);
      FunctionBase* r = rterm->Simplify(valuemap);
      if (l && r) {
         return new AddFunction(l , r);
      } else {
         if (l) {delete l;}
         if (r) {delete r;}
         return 0;
      }
   }
   // not reached
   return 0;
}



// -----------------------------------    SubtractFunction     -----------------------------------------------



SOLVER_RESULT SubtractFunction::Evaluate(const std::map<std::string , double>& valuemap , double* result) {
   ASSERT(result);
   double l,r;
   SOLVER_RESULT sr1 = lterm->Evaluate(valuemap , &l);
   SOLVER_RESULT sr2 = rterm->Evaluate(valuemap , &r);
   if ((sr1 != RESULT_VALID) || (sr2 != RESULT_VALID)) {
      if ((sr1 == RESULT_ERROR) || (sr2 == RESULT_ERROR)) {return RESULT_ERROR;} else {return RESULT_UNDEFINED;}
   }
   *result = l - r;
   return RESULT_VALID;
}



std::string SubtractFunction::Print() {
   string output;
   output += lterm->Print();
   output += " - ";
   if (rterm->IsCompoundStatement()) {
      output += "(";
      output += rterm->Print();
      output += ")";
   } else {
      output += rterm->Print();
   }
   return output;
}



FunctionBase* SubtractFunction::Simplify(const std::map<std::string , double>& valuemap) {
   if (lterm->IsConstant(valuemap) && rterm->IsConstant(valuemap)) {
      double d = 0.0;
      SOLVER_RESULT sr = Evaluate(valuemap , &d);
      if (sr == RESULT_VALID) {
         return new ConstantFunction(d);
      } else {
         return 0;
      }
   } else {
      FunctionBase* l = lterm->Simplify(valuemap);
      FunctionBase* r = rterm->Simplify(valuemap);
      if (l && r) {
         return new SubtractFunction(l , r);
      } else {
         if (l) {delete l;}
         if (r) {delete r;}
         return 0;
      }
   }
   // not reached
   return 0;
}



// ----------------------------------    MultiplyFunction    -----------------------------------------------



SOLVER_RESULT MultiplyFunction::Evaluate(const std::map<std::string , double>& valuemap , double* result) {
   ASSERT(result);
   double l,r;
   SOLVER_RESULT sr1 = lterm->Evaluate(valuemap , &l);
   SOLVER_RESULT sr2 = rterm->Evaluate(valuemap , &r);
   if ((sr1 != RESULT_VALID) || (sr2 != RESULT_VALID)) {
      if ((sr1 == RESULT_ERROR) || (sr2 == RESULT_ERROR)) {return RESULT_ERROR;} else {return RESULT_UNDEFINED;}
   }
   *result = l*r;
   return RESULT_VALID;
}



// (a + b) * (c + d)
std::string MultiplyFunction::Print() {
   string output;
   if (lterm->IsCompoundStatement()) {
      output += "(";
      output += lterm->Print();
      output += ")";
   } else {
      output += lterm->Print();
   }
   output += " * ";
   if (rterm->IsCompoundStatement()) {
      output += "(";
      output += rterm->Print();
      output += ")";
   } else {
      output += rterm->Print();
   }
   return output;
}



FunctionBase* MultiplyFunction::Simplify(const std::map<std::string , double>& valuemap) {
   if (lterm->IsConstant(valuemap) && rterm->IsConstant(valuemap)) {
      double d = 0.0;
      SOLVER_RESULT sr = Evaluate(valuemap , &d);
      if (sr == RESULT_VALID) {
         return new ConstantFunction(d);
      } else {
         return 0;
      }
   } else {
      FunctionBase* l = lterm->Simplify(valuemap);
      FunctionBase* r = rterm->Simplify(valuemap);
      if (l && r) {
         return new MultiplyFunction(l , r);
      } else {
         if (l) {delete l;}
         if (r) {delete r;}
         return 0;
      }
   }
   // not reached
   return 0;
}



// ---------------------------------     DivideFunction     ----------------------------------------------



SOLVER_RESULT DivideFunction::Evaluate(const std::map<std::string , double>& valuemap , double* result) {
   ASSERT(result);
   double l,r;
   SOLVER_RESULT sr1 = lterm->Evaluate(valuemap , &l);
   SOLVER_RESULT sr2 = rterm->Evaluate(valuemap , &r);
   if ((sr1 != RESULT_VALID) || (sr2 != RESULT_VALID)) {
      if ((sr1 == RESULT_ERROR) || (sr2 == RESULT_ERROR)) {return RESULT_ERROR;} else {return RESULT_UNDEFINED;}
   }
   if (r == 0.0) {return RESULT_UNDEFINED;}
   *result = l/r;
   return RESULT_VALID;
}



// (a + b)/(c + d)
std::string DivideFunction::Print() {
   string output;
   if (lterm->IsCompoundStatement()) {
      output += "(";
      output += lterm->Print();
      output += ")";
   } else {
      output += lterm->Print();
   }
   output += " / ";
   if (rterm->IsCompoundStatement()) {
      output += "(";
      output += rterm->Print();
      output += ")";
   } else {
      output += rterm->Print();
   }
   return output;
}



FunctionBase* DivideFunction::Simplify(const std::map<std::string , double>& valuemap) {
   if (lterm->IsConstant(valuemap) && rterm->IsConstant(valuemap)) {
      double d = 0.0;
      SOLVER_RESULT sr = Evaluate(valuemap , &d);
      if (sr == RESULT_VALID) {
         return new ConstantFunction(d);
      } else {
         return 0;
      }
   } else {
      FunctionBase* l = lterm->Simplify(valuemap);
      FunctionBase* r = rterm->Simplify(valuemap);
      if (l && r) {
         return new DivideFunction(l , r);
      } else {
         if (l) {delete l;}
         if (r) {delete r;}
         return 0;
      }
   }
   // not reached
   return 0;
}



// ---------------------------------     ModulusFunction     -----------------------------------------------



SOLVER_RESULT ModulusFunction::Evaluate(const std::map<std::string , double>& valuemap , double* result) {
   ASSERT(result);
   double l,r;
   SOLVER_RESULT sr1 = lterm->Evaluate(valuemap , &l);
   SOLVER_RESULT sr2 = rterm->Evaluate(valuemap , &r);
   if ((sr1 != RESULT_VALID) || (sr2 != RESULT_VALID)) {
      if ((sr1 == RESULT_ERROR) || (sr2 == RESULT_ERROR)) {return RESULT_ERROR;} else {return RESULT_UNDEFINED;}
   }
   if (r == 0.0) {return RESULT_UNDEFINED;}
   *result = fmod(l,r);
   return RESULT_VALID;
}



// fmod(l,r)
string ModulusFunction::Print() {
   string output = "fmod(";
   output += lterm->Print();
   output += " , ";
   output += rterm->Print();
   output += ')';
   return output;
}



FunctionBase* ModulusFunction::Simplify(const std::map<std::string , double>& valuemap) {
   if (lterm->IsConstant(valuemap) && rterm->IsConstant(valuemap)) {
      double d = 0.0;
      SOLVER_RESULT sr = Evaluate(valuemap , &d);
      if (sr == RESULT_VALID) {
         return new ConstantFunction(d);
      } else {
         return 0;
      }
   } else {
      FunctionBase* l = lterm->Simplify(valuemap);
      FunctionBase* r = rterm->Simplify(valuemap);
      if (l && r) {
         return new ModulusFunction(l , r);
      } else {
         if (l) {delete l;}
         if (r) {delete r;}
         return 0;
      }
   }
   // not reached
   return 0;
}



// ---------------------------------     PowerFunction     ---------------------------------------------------



SOLVER_RESULT PowerFunction::Evaluate(const std::map<std::string , double>& valuemap , double* result) {
   ASSERT(result);
   double base,exp;
   SOLVER_RESULT sr1 = lterm->Evaluate(valuemap , &base);
   SOLVER_RESULT sr2 = rterm->Evaluate(valuemap , &exp);
   if ((sr1 != RESULT_VALID) || (sr2 != RESULT_VALID)) {
      if ((sr1 == RESULT_ERROR) || (sr2 == RESULT_ERROR)) {return RESULT_ERROR;} else {return RESULT_UNDEFINED;}
   }
//The pow() function returns base raised to the expth power.
// There's a domain error if base is zero and exp is less than or equal to zero.
// There's also a domain error if base is negative and exp is not an integer.
// There's a range error if an overflow occurs.   
   if ((base == 0.0) && (exp <= 0.0)) {
      return RESULT_UNDEFINED;
   }
   if ((base < 0.0) && (((float)(int)exp) != exp)) {
      return RESULT_UNDEFINED;
   }
   *result = pow(base,exp);
   return RESULT_VALID;
}



string PowerFunction::Print() {
   string output;
   if (lterm->IsCompoundStatement()) {
      output += '(';
      output += lterm->Print();
      output += ')';
   } else {
      output += lterm->Print();
   }
   output += " ^ ";
   if (rterm->IsCompoundStatement()) {
      output += '(';
      output += rterm->Print();
      output += ')';
   } else {
      output += rterm->Print();
   }
   return output;
}


FunctionBase* PowerFunction::Simplify(const std::map<std::string , double>& valuemap) {
   if (lterm->IsConstant(valuemap) && rterm->IsConstant(valuemap)) {
      double d = 0.0;
      SOLVER_RESULT sr = Evaluate(valuemap , &d);
      if (sr == RESULT_VALID) {
         return new ConstantFunction(d);
      } else {
         return 0;
      }
   } else {
      FunctionBase* l = lterm->Simplify(valuemap);
      FunctionBase* r = rterm->Simplify(valuemap);
      if (l && r) {
         return new PowerFunction(l , r);
      } else {
         if (l) {delete l;}
         if (r) {delete r;}
         return 0;
      }
   }
   // not reached
   return 0;
}



// -------------------------------------     BaseLogarithmFunction     --------------------------------------



SOLVER_RESULT BaseLogarithmFunction::Evaluate(const std::map<std::string , double>& valuemap , double* result) {
   ASSERT(result);
   double number,base;
   SOLVER_RESULT sr1 = lterm->Evaluate(valuemap , &number);
   SOLVER_RESULT sr2 = rterm->Evaluate(valuemap , &base);
   if ((sr1 != RESULT_VALID) || (sr2 != RESULT_VALID)) {
      if ((sr1 == RESULT_ERROR) || (sr2 == RESULT_ERROR)) {return RESULT_ERROR;} else {return RESULT_UNDEFINED;}
   }
   if ((number <= 0.0) || (base <= 0.0)) {
      return RESULT_UNDEFINED;
   }
   double num = log10(number);
   double den = log10(base);
   if (den == 0.0) {
      return RESULT_UNDEFINED;
   }
   *result = num/den;
   return RESULT_VALID;
}



string BaseLogarithmFunction::Print() {
   string output = "logb(";
   output += lterm->Print();
   output += " , ";
   output += rterm->Print();
   output += ')';
   return output;
}


FunctionBase* BaseLogarithmFunction::Simplify(const std::map<std::string , double>& valuemap) {
   if (lterm->IsConstant(valuemap) && rterm->IsConstant(valuemap)) {
      double d = 0.0;
      SOLVER_RESULT sr = Evaluate(valuemap , &d);
      if (sr == RESULT_VALID) {
         return new ConstantFunction(d);
      } else {
         return 0;
      }
   } else {
      FunctionBase* l = lterm->Simplify(valuemap);
      FunctionBase* r = rterm->Simplify(valuemap);
      if (l && r) {
         return new BaseLogarithmFunction(l , r);
      } else {
         if (l) {delete l;}
         if (r) {delete r;}
         return 0;
      }
   }
   // not reached
   return 0;
}



// ----------------------------     ArcTangent2Function     ----------------------------------------------



SOLVER_RESULT ArcTangent2Function::Evaluate(const std::map<std::string , double>& valuemap , double* result) {
   ASSERT(result);
   double y,x;
   SOLVER_RESULT sr1 = lterm->Evaluate(valuemap , &y);
   SOLVER_RESULT sr2 = rterm->Evaluate(valuemap , &x);
   if ((sr1 != RESULT_VALID) || (sr2 != RESULT_VALID)) {
      if ((sr1 == RESULT_ERROR) || (sr2 == RESULT_ERROR)) {return RESULT_ERROR;} else {return RESULT_UNDEFINED;}
   }
   *result = atan2(y,x);
   return RESULT_VALID;
}



string ArcTangent2Function::Print() {
   string output = "atan2(";
   output += lterm->Print();
   output += " , ";
   output += rterm->Print();
   output += ')';
   return output;
}


FunctionBase* ArcTangent2Function::Simplify(const std::map<std::string , double>& valuemap) {
   if (lterm->IsConstant(valuemap) && rterm->IsConstant(valuemap)) {
      double d = 0.0;
      SOLVER_RESULT sr = Evaluate(valuemap , &d);
      if (sr == RESULT_VALID) {
         return new ConstantFunction(d);
      } else {
         return 0;
      }
   } else {
      FunctionBase* l = lterm->Simplify(valuemap);
      FunctionBase* r = rterm->Simplify(valuemap);
      if (l && r) {
         return new ArcTangent2Function(l , r);
      } else {
         if (l) {delete l;}
         if (r) {delete r;}
         return 0;
      }
   }
   // not reached
   return 0;
}



/// -----------------------------     CustomFunction class     ---------------------------------------


//f(x,y,z) = x + 2*y + 3*z
void CustomFunction::Parse(const std::string& definition) {
   Free();
   def = definition;
   const char* cstr = def.c_str();
   char buffer[512];
   char c = '\0';
   int ncharsread1 = 0;
   int ncharsread2 = 0;
   int i = 0;

   // function_name(p1,p2) = function_definition
   
   // first, search for an = sign. If there is none, this is not a definition
   const char* eq = strchr(cstr , '=');
   if (!eq) {
      throw EagleError("CustomFunction::Parse - no assignment to a function definition!\n");
   }
   string sub = eq + 1;
   eval = new Function(sub);
   
   // second, get the function name
   // skip leading whitespace, if any
   while (isspace(cstr[i])) {
      ++i;
   }
   
   c = cstr[i];
   if ((c == '_') || isalpha(c)) {
      // valid character to start function name with
      if (1 != sscanf(cstr , "%[_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ]%n" , buffer , &ncharsread1)) {
         throw EagleError("CustomFunction::Parse - Lies! Lies!\n");
      }
      sscanf(cstr + ncharsread1 , "%[_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789]%n" , buffer + ncharsread1 , &ncharsread2);
      name = buffer;
      i += ncharsread1 + ncharsread2;
      // parse input variables now
      c = cstr[i];
      if (c == '(') {
         ++i;
         c = cstr[i];
         if (c != ')') {
            while (1) {
               const char* comma = strchr(cstr + i, ',');
               const char* pclose = strchr(cstr + i, ')');
               if (!pclose) {
                  throw EagleError("CustomFunction::Parse - no closing parenthesis for argument list!\n");
               }
               // if comma, stop there else stop at closing parenthesis
               const char* stop = 0;
               if (comma && comma < pclose) {
                  stop = comma;
               } else {
                  stop = pclose;
               }
               if (1 != sscanf(cstr + i , "%[_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ]%n" , buffer , &ncharsread1)) {
                  throw EagleError("CustomFunction::Parse - Beginning of input variable contains invalid characters!\n");
               }
               ncharsread2 = 0;
               sscanf(cstr + i + ncharsread1 , "%[_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789]%n" , buffer + ncharsread1 , &ncharsread2);
               i += ncharsread1 + ncharsread2;
               if (!((*(cstr + i) == ',') || (*(cstr + i) == ')'))) {
                  throw EagleError(StringPrintF(
                     "CustomFunction::Parse - Invalid character %c following variable name at index %i!\n" , *(cstr + i) , i
                  ));
               }
               vars.push_back(string(buffer));
               args.push_back(new Function(buffer));
               if (stop == pclose) {
                  break;
               } else {
                  ++i;
               }
            }
         }
      } else {
         throw EagleError("CustomFunction::Parse - function name must be followed by a variable list enclosed in parenthesis!\n");
      }
      SetNumArgs(vars.size());
      inputset.clear();
      for (i = 0 ; i < (int)args.size() ; ++i) {
         FunctionBase* f = args[i];
         inputset.insert(f->InputSet().begin() , f->InputSet().end());
      }
      if (inputset != eval->InputSet()) {
         throw EagleError("CustomFunction::Parse - Argument input set does not match expressions input set!\n");
      }
      
   } else {
      throw EagleError(StringPrintF(
         "CustomFunction::Parse - %c is an invalid character to start a function name with!\n" , c
      ));
   }
   
   // Okay, we've got a function name and the list of input variables and the function definition
   // See if this function name is registered already, if so, delete the CustomFunction assigned to
   // it and register this CustomFunction
   map<string , CustomFunction*>::iterator it = user_function_set.find(name);
   if (it != user_function_set.end()) {
      delete it->second;
   }
   user_function_set[name] = this;
}



CustomFunction::CustomFunction(std::set<std::string> new_inputset , std::string function_name , std::vector<std::string> variables , std::vector<FunctionBase*> arguments , FunctionBase* expression) :
      FunctionBase(0),
      def(),
      name(function_name),
      vars(variables),
      args(arguments),
      eval(expression),
      is_original(true)
{
   SetNumArgs(args.size());
   def = PrintDefinition();
   map<string , CustomFunction*>::iterator it = user_function_set.find(name);
   if (it != user_function_set.end()) {
      delete it->second;
   }
   user_function_set[name] = this;
}



CustomFunction::CustomFunction(const std::string& definition) :
      FunctionBase(0),// Use SetNumArgs later
      def(),
      name(),
      vars(),
      args(),
      eval(0),
      is_original(true)
{
   Parse(definition);
}



CustomFunction::CustomFunction(const std::string& function_name , std::vector<FunctionBase*> arguments , bool cloneargs) :
      FunctionBase(0),
      def(),
      name(),
      vars(),
      args(),
      eval(0),
      is_original(false)
{
   // lookup function name
   CustomFunction* f = 0;
   map<string,CustomFunction*>::iterator it = user_function_set.find(function_name);
   if (it == user_function_set.end()) {
      throw EagleError(StringPrintF(
         "CustomFunction::CustomFunction - %s not found in user function set!\n" , name.c_str()
      ));
   }
   name = function_name;
   f = it->second;
   ASSERT(f);
   size_t nargs = f->NumArgs();
   if (arguments.size() != nargs) {
      throw EagleError(StringPrintF(
         "CustomFunction::CustomFunction - %s function takes %u arguments, but was passed %u arguments!" , name.c_str() , nargs , arguments.size()
      ));
   }
   SetNumArgs(nargs);
   def = f->def;
   vars = f->vars;
   if (cloneargs) {
      for (size_t i = 0 ; i < arguments.size() ; ++i) {
         args.push_back(arguments[i]->Clone());
      }
   } else {
      args = arguments;
   }
   eval = f->eval->Clone();
   inputset.insert(eval->InputSet().begin() , eval->InputSet().end());
   
   for (size_t i = 0 ; i < vars.size() ; ++i) {
      for (set<string>::iterator sit = inputset.begin() ; sit != inputset.end() ; ++sit) {
         string input = *sit;
         bool found = false;
         for (size_t j = 0 ; j < vars.size() ; ++j) {
            if (vars[j] == input) {found = true;break;}
         }
         if (!found) {
            throw EagleError(StringPrintF(
               "CustomFunction::CustomFunction - %s input variable not found in function parameters. The function would fail if called!\n" , input.c_str()
            ));
         }
      }
   }
}



CustomFunction::CustomFunction(std::string function_name , FunctionBase* expression , bool clone) :
   FunctionBase(0),
   def(),
   name(function_name),
   vars(),
   args(),
   eval(0),
   is_original(true)
{
   ASSERT(expression);
   if (!expression) {
      throw EagleError("CustomFunction::CustomFunction(string , FunctionBase* , bool) - expression is NULL!\n");
   }
   if (clone) {
      eval = expression->Clone();
   } else {
      eval = expression;
   }
   inputset = eval->InputSet();
   for (set<string>::iterator it = inputset.begin() ; it != inputset.end() ; ++it) {
      string s = *it;
      vars.push_back(s);
      args.push_back(new Function(s));
   }
   SetNumArgs(args.size());
   def = PrintDefinition();
   map<string , CustomFunction*>::iterator it = user_function_set.find(name);
   if (it != user_function_set.end()) {
      delete it->second;
   }
   user_function_set[name] = this;
}



CustomFunction::~CustomFunction() {
   Free();
   if (is_original) {
      map<string , CustomFunction*>::iterator it = user_function_set.find(name);
      if (it != user_function_set.end()) {
         user_function_set.erase(it);
      }
   }
}



void CustomFunction::Free() {
   if (eval) {
      delete eval;
      eval = 0;
   }
   for (size_t i = 0 ; i < args.size() ; ++i) {
      delete args[i];
   }
   vars.clear();
   args.clear();
   
}



string CustomFunction::PrintDefinition() {
   string s = name;
   s += '(';
   for (size_t i = 0 ; i < vars.size() ; ++i) {
      s += vars[i];
      if (i != vars.size() - 1) {
         s += ',';
      }
   }
   s += ") = ";
   s += eval->Print();
   return s;
}


SOLVER_RESULT CustomFunction::Evaluate(const std::map<std::string , double>& valuemap , double* result) {
   ASSERT(result);
   std::map<std::string , double> vmap;
   if (args.size() != NumArgs()) {
      throw EagleError(StringPrintF(
         "CustomFunction::Evaluate - %s function was passed %u arguments, but takes %u! arguments!\n" , name.c_str() , args.size() , NumArgs()
      ));
   }
   for (size_t i = 0 ; i < NumArgs() ; ++i) {
      double output = 0.0;
      SOLVER_RESULT sr = args[i]->Evaluate(valuemap , &output);
      if (sr != RESULT_VALID) {return sr;}
      vmap[vars[i]] = output;
   }
   return eval->Evaluate(vmap , result);
}



vector<FunctionBase*> CustomFunction::Terms() {
   vector<FunctionBase*> v;
   for (size_t i = 0 ; i < args.size() ; ++i) {
      vector<FunctionBase*> v2 = args[i]->Terms();
      v.insert(v.end() , v2.begin() , v2.end());
      v.push_back(args[i]);
   }
   return v;
}



bool CustomFunction::WillCall(FunctionBase* f) {
   for (size_t i = 0 ; i < args.size() ; ++i) {
      if (f == args[i]) {return true;}
      if (args[i]->WillCall(f)) {return true;}
   }
   if (f == eval) {return true;}
   if (eval->WillCall(f)) {return true;}
   return false;
}



bool CustomFunction::IsConstant() {
   bool constant = true;
   for (size_t i = 0 ; i < args.size() ; ++i) {
      constant = constant && args[i]->IsConstant();
      if (!constant) {return false;}
   }
   return true;
}



bool CustomFunction::IsConstant(const std::map<std::string , double>& valuemap) {
   bool constant = true;
   for (size_t i = 0 ; i < args.size() ; ++i) {
      constant = constant && args[i]->IsConstant(valuemap);
      if (!constant) {return false;}
   }
   return true;
}



string CustomFunction::Print() {
   string output = name;
   output += '(';
   for (size_t i = 0 ; i < args.size() ; ++i) {
      output += args[i]->Print();
      if (i < args.size() - 1) {
         output += " , ";
      }
   }
   output += ')';
   return output;
}



FunctionBase* CustomFunction::Simplify(const std::map<std::string , double>& valuemap) {
   if (IsConstant(valuemap)) {
      double d = 0.0;
      SOLVER_RESULT sr = Evaluate(valuemap , &d);
      if (sr == RESULT_VALID) {
         return new ConstantFunction(d);
      } else {
         return 0;
      }
   }
   else {
      vector<FunctionBase*> simp_args;
      bool fail = false;
      for (size_t i = 0 ; i < args.size() ; ++i) {
         FunctionBase* f = args[i]->Simplify(valuemap);
         if (!f) {fail = true;break;}
         simp_args.push_back(f);
      }
      if (fail) {
         for (size_t i = 0 ; i < simp_args.size() ; ++i) {
            delete simp_args[i];
         }
         return 0;
      }
      map<string , double> vmap;
      vector<string> new_vars;
      vector<FunctionBase*> new_args;
      set<string> new_inputset;
      
      vector<string>::iterator varit = vars.begin();
      for (size_t i = 0 ; i < simp_args.size() ; ++i) {
         if (simp_args[i]->IsConstant()) {
            double output = 0.0;
            simp_args[i]->Evaluate(valuemap , &output);
            vmap[vars[i]] = output;
         } else {
            new_args.push_back(simp_args[i]);
            new_vars.push_back(*varit);
         }
         set<string> inset = simp_args[i]->InputSet();
         new_inputset.insert(inset.begin() , inset.end());
         ++varit;
      }
      FunctionBase* new_eval = eval->Simplify(vmap);
      if (!new_eval) {
         for (size_t i = 0 ; i < simp_args.size() ; ++i) {
            delete simp_args[i];
         }
         return 0;
      }
      
      return new CustomFunction(new_inputset , name + "_" , new_vars , simp_args , eval);
   }
   // not reached
   return 0;
}



/// -----------------------------     Function class     ---------------------------------------



void Function::CleanUp() {
   vector<FunctionBase*> roots;
   for (map<string , FunctionBase*>::iterator it1 = terms.begin() ; it1 != terms.end() ; ++it1) {
      FunctionBase* f1 = it1->second;
      bool willbecalled = false;
      for (map<string , FunctionBase*>::iterator it2 = terms.begin() ; it2 != terms.end() ; ++it2) {
         FunctionBase* f2 = it2->second;
         if (f2->WillCall(f1)) {
            willbecalled = true;
            break;
         }
      }
      if (!willbecalled) {
         roots.push_back(f1);
      }
   }
   for (size_t i = 0 ; i < roots.size() ; ++i) {
      delete roots[i];
   }
   terms.clear();
}



void Function::ParseCheck() {
   const char* cstr = parsestr.c_str();
   char c = '\0';
   int popencount = 0;
   int pclosecount = 0;


   // First, remove all whitespace from string
   for (string::iterator it = parsestr.begin() ; it != parsestr.end() ; ) {
      c = *it;
      if (isspace(c)) {
         it = parsestr.erase(it);
      } else {
         ++it;
      }
   }
   cstr = parsestr.c_str();

   // Second, make sure all parenthesis are matched
   // ()(
   // )()
   // (())
   int pstartindex = 0;
   for (size_t i = 0 ; i < strlen(cstr) ; ++i) {
      c = cstr[i];
      if (c == '(') {
         pstartindex = i;
         ++popencount;
      } else if (c == ')') {
         ++pclosecount;
         if (pclosecount > popencount) {
            throw EagleError(StringPrintF(
               "Function::ParseCheck - Unmatched closing parenthesis at index %i!\n" , i + 1
            ));
         }
      }
   }
   if (popencount != pclosecount) {
      throw EagleError(StringPrintF(
         "Function::ParseCheck - Unmatched opening parenthesis at index %i!\n" , pstartindex + 1
      ));
   }
   
   // Third, check for foreign symbols
   // acceptable symbols are .,_+-*/^()[a-z A-Z 0-9]
   /// sscanf can't be used here, because it interprets - and ^ as special commands
//   const char* accept = ",.+-*/^()_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
   
   for (size_t i = 0 ; i < strlen(cstr) ; ++i) {
      c = cstr[i];
      if (! ( isalpha(c) ||
              isdigit(c) ||
              (c == ',') ||
              (c == '.') ||
              (c == '+') ||
              (c == '-') ||
              (c == '*') ||
              (c == '/') ||
              (c == '^') ||
              (c == '(') ||
              (c == ')') ||
              (c == '_') )) {
         throw EagleError(StringPrintF(
            "Function::ParseCheck - Foreign character '%c' found at index %i in parse string!" , c , i + 1
         ));
      }
   }

#ifdef DEBUGFUNC
   OutputLog() << StringPrintF("After parse check , string is : '%s'\n" , parsestr.c_str());
#endif
}



void Function::ParseConstants() {
   // constants start with - or with a digit, and cannot start with anything else
   // (-1) a-1 a1 $f1$ a1-1
   string str = parsestr;
   const char* cstr = str.c_str();
   int numcharsread = 0;
   double constant = 0.0;
   int parenthesis = 0;

   for (int i = 0 ; i < (int)strlen(cstr) ; ++i) {
      if (cstr[i] == '(') {++parenthesis;}
      if (cstr[i] == ')') {--parenthesis;}
      
      if (!parenthesis) {
         if ((cstr[i] == '-') || isdigit(cstr[i])) {
            if (cstr[i] == '-') {
               // is minus sign - could be subtraction. If the character before it is [$ | a-z | A-Z | 0-9 | _ | )] then it is subtraction
               if (i > 0) {
                  if (!((cstr[i-1] == '$') ||
                        isalpha(cstr[i-1]) ||
                        isdigit(cstr[i-1]) ||
                        (cstr[i-1] == ')'))) 
                  {
                     // not subtraction, we have a negative number
                     // BUT, only if the number after the dash is a digit
                     if ((i + 1 < (int)strlen(cstr)) && isdigit(cstr[i + 1])) {
                        
                        if (1 != sscanf(cstr + i , "%lf%n" , &constant , &numcharsread)) {
                           CleanUp();
                           throw EagleError(StringPrintF(
                              "Function::ParseConstants 1 - sscanf failed to read the number!\n   %s , i = %i"
                           ));
                        }
                        sprintf(termbuf , "$f%u$" , termcount++);
                        terms[termbuf] = new ConstantFunction(constant);
///string& replace( size_type index1, size_type num1, const string& str, size_type index2, size_type num2 );
//replaces up to num1 characters of the current string (starting at index1) with up to num2 characters from str beginning at index2,
                        str.replace(i , numcharsread , string(termbuf) , 0 , strlen(termbuf));
///                        str.replace(i , numcharsread , termbuf);// This could fail if the number is longer than termbuf
                        cstr = str.c_str();
                        i += strlen(termbuf) - 1;
                     }
                  }
               } else {
                  // - at the beginning, we have a number
                  // IF the next character is a digit
                  if ((i + 1 < (int)strlen(cstr)) && isdigit(cstr[i + 1])) {
                     if (1 != sscanf(cstr + i , "%lf%n" , &constant , &numcharsread)) {
                        CleanUp();
                        throw EagleError("Function::ParseConstants 2 - sscanf failed to read the number!\n");
                     }
                     sprintf(termbuf , "$f%u$" , termcount++);
                     terms[termbuf] = new ConstantFunction(constant);
///string& replace( size_type index1, size_type num1, const string& str, size_type index2, size_type num2 );
//replaces up to num1 characters of the current string (starting at index1) with up to num2 characters from str beginning at index2,
                     str.replace(i , numcharsread , string(termbuf) , 0 , strlen(termbuf));
///                     str.replace(i , numcharsread , termbuf);// This could fail if the number is longer than termbuf
                     cstr = str.c_str();
                     i += strlen(termbuf) - 1;
                  }
               }
            } else {
               // is digit, is only a number if the character before it is a space or it is the first character
               // or if the character before it is a ( % * / + - ^
               if ((i == 0) || 
                   ((i > 0) && (cstr[i - 1] == ' ')) || 
                   ((i > 0) && (cstr[i - 1] == '(')) ||
                   ((i > 0) && (cstr[i - 1] == '%')) ||
                   ((i > 0) && (cstr[i - 1] == '*')) ||
                   ((i > 0) && (cstr[i - 1] == '/')) ||
                   ((i > 0) && (cstr[i - 1] == '+')) ||
                   ((i > 0) && (cstr[i - 1] == '^')) ||
                   ((i > 0) && (cstr[i - 1] == '-'))) {
                  if (1 != sscanf(cstr + i , "%lf%n" , &constant , &numcharsread)) {
                     CleanUp();
                     throw EagleError("Function::ParseConstants 3 - sscanf failed to read the number!\n");
                  }
                  sprintf(termbuf , "$f%u$" , termcount++);
                  terms[termbuf] = new ConstantFunction(constant);
///string& replace( size_type index1, size_type num1, const string& str, size_type index2, size_type num2 );
//replaces up to num1 characters of the current string (starting at index1) with up to num2 characters from str beginning at index2,
                  str.replace(i , numcharsread , string(termbuf) , 0 , strlen(termbuf));
                  cstr = str.c_str();
                  i += strlen(termbuf) - 1;
               }
            }
         }
      }
   }
#ifdef DEBUGFUNC
   OutputLog() << StringPrintF("After parsing constants, string is : '%s'\n" , str.c_str());
#endif
   parsestr = str;
}



void Function::ParseVariables() {
   string str = parsestr;
   const char* cstr = str.c_str();
   int numcharsread1 = 0;
   int numcharsread2 = 0;
   int items = 0;
   char buffer[256];
   int unused = 0;
   int parenthesis = 0;
   
   for (int i = 0 ; i < (int)strlen(cstr) ; ++i) {
      if (cstr[i] == '(') {++parenthesis;}
      if (cstr[i] == ')') {--parenthesis;}
      
      if (!parenthesis) {
         
         // variables must start with an alphabetical character, and may then further consist of any alphanumeric characters
         // the resulting name must not match a function name
         // variables may not come after a $ - no implicit multiplication!
         if (cstr[i] == '$') {
            // skip forward past the term
            ///int sscanf( const char *buffer, const char *format, ... );
            if (1 != sscanf(cstr + i , "$f%i$%n" , &unused , &numcharsread1)) {
               CleanUp();
               throw EagleError("Function::ParseVariables 1 - sscanf failed to read the symbol term!\n");
            }
            i += numcharsread1 - 1;
         } else if (isalpha(cstr[i]) || (cstr[i] == '_')) {
            if (1 != sscanf(cstr + i , "%[_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ]%n" , buffer , &numcharsread1)) {
               CleanUp();
               throw EagleError("Function::ParseVariables 2 - sscanf failed to read function name characters!\n");
            }
            numcharsread2 = 0;
            items = sscanf(cstr + i + numcharsread1 , "%[_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789]%n" , buffer + numcharsread1 , &numcharsread2);
            // see if this symbol name is a function - if not, then it is a variable
            bool is_function = false;
            for (set<string>::iterator it = function_set.begin() ; it != function_set.end() ; ++it) {
               const char* cstr2 = (*it).c_str();
               if ((strlen(buffer) == strlen(cstr2)) &&
                   (strncmp(buffer , cstr2 , strlen(cstr2)) == 0)) {
                  is_function = true;
                  break;
               }
            }
            if (!is_function) {
               // could be an undefined function
               if (cstr[i + numcharsread1 + numcharsread2] == '(') {
                  CleanUp();
                  throw EagleError(StringPrintF(
                     "Function::ParseVariables - function %s is undefined!\n" , buffer
                  ));
               }
               // We've got a variable!
               inputset.insert(buffer);
               sprintf(termbuf , "$f%u$" , termcount++);
               str.replace(i , numcharsread1 + numcharsread2 , string(termbuf) , 0 , strlen(termbuf));
               cstr = str.c_str();
               i += strlen(termbuf) - 1;
               terms[termbuf] = new VariableFunction(buffer);
            } else {
               // skip function name
               i += strlen(buffer) - 1;
            }
         }
      }
   }
#ifdef DEBUGFUNC
   OutputLog() << StringPrintF("After parsing variables, string is : '%s'\n" , str.c_str());
#endif
   parsestr = str;
}

/// a + log10(a + log10(b))
/// $f0$ + log10($f1$ + log10($f2))
/// $f0$ + log10($f1$ + $f3$)
/// $f0$ + log10($f4$)
/// $f0$ + $f5$
/// $f6$

/// a - (b + log10(c))
/// a-(b+log10(c))
/// $f0$ - ($f1$ + log10($f2$))
/// $f0$ - ($f1$ + $f3$)
/// $f0$ - $f4$
/// $f5$

void Function::ParseGroups() {
   string str = parsestr;
   const char* cstr = str.c_str();
   
   int pstart = -1;
   int pend = -1;
   int count = 0;
   int parenthesis = 0;
   int pstartcount = 0;
   
   bool not_function_call = true;
   char symbol[64];
   char buffer[512];
   int nitems = 0;
   int num = 0;
   int ncharsread1 = 0;
   int ncharsread2 = 0;
   char c = '\0';
   

   /// log10(a + (b + c)) + a - (b + c)
   /// log10(a + (b + c)) + $f0$ - (b + c)
   /// log10(a + (b + c)) + $f0$ - $f1$
   /// log10($f2$)
   /// $f3$

   for (size_t i = 0 ; i < strlen(cstr) ; ++i) {
      c = cstr[i];
      // skip $f#...$, skip whitespace
      
      // find first of [a-z _], make sure the character before it is not $
      // or find first of '('
      
      // constants and variables outside of parenthesis are $f#$'ed
      if (c == '$') {
         not_function_call = true;
         // skip $f#$ (function symbol)
         nitems = sscanf(cstr + i, "$f%i$%n" , &num , &ncharsread1);
         if (nitems == 0) {
            CleanUp();
            throw EagleError("Function::ParseGroups - Function symbol not read correctly!\n");
         }
         i += ncharsread1 - 1;
         continue;
      } else if (c == '(') {
         pstart = i;
         pstartcount = parenthesis;
         ++parenthesis;
         if (not_function_call) {
            // found start of parenthesis group
            // find end of parenthesis group
            // skip opening parenthesis
            // stop when closing parenthesis matches level of opening parenthesis
            for (size_t j = i + 1 ; j < strlen(cstr) ; ++j) {
               c = cstr[j];
               if (c == '(') {
                  ++parenthesis;
               } else if (c == ')') {
                  --parenthesis;
                  if (pstartcount == parenthesis) {
                     // found matching closing parenthesis
                     pend = j;
                     count = (pend - pstart) + 1;
                     string sub = str.substr(pstart + 1 , count - 2);
                     Function* f = new Function(sub);
                     sprintf(symbol , "$f%u$" , termcount++);
                     terms[symbol] = f;
                     str.replace(pstart , count , symbol);
                     cstr = str.c_str();
                     i += strlen(symbol) - 1;
                     break;
                  }
               }
            }
         } else {
            // is start of function call
            // skip to matching parenthesis
            for (size_t j = i + 1 ; j < strlen(cstr) ; ++j) {
               c = cstr[i];
               if (c == '(') {
                  ++parenthesis;
               } else if (c == ')') {
                  --parenthesis;
                  if (pstartcount == parenthesis) {
                     // found matching closing parenthesis
                     i = j;
                     break;
                  }
               }
            }
         }
      } else if (isalpha(c) || c == '_') {
         not_function_call = false;
         // start of function name
         // read function name and move past it
         nitems = sscanf(cstr + i , "%[_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ]%n" , buffer , &ncharsread1);
         if (nitems == 0) {
            CleanUp();
            throw EagleError("Function::ParseGroups - Lies! Lies!\n");
         }
         nitems += sscanf(cstr + i + ncharsread1 , "%[_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789]%n" , buffer + ncharsread1 , &ncharsread2);
         pstartcount = parenthesis;
         // check if it is a variable? No
         // all variables outside of parenthesis have been $f#...$'ed
         // check if it is a registered function name
         
         bool is_valid_function = false;
         for (set<string>::iterator it = function_set.begin() ; it != function_set.end() ; ++it) {
            if (strcmp(buffer , it->c_str()) == 0) {is_valid_function = true;break;}
         }
         if (is_valid_function) {
            if (cstr[i + ncharsread1 + ncharsread2] != '(') {
               CleanUp();
               throw EagleError(StringPrintF(
                  "Function %s called without arguments list! No parenthesis!\n" , buffer
               ));
            } else {
               ++parenthesis;
               for (size_t j = i + ncharsread1 + ncharsread2 + 1 ; j < strlen(cstr) ; ++j) {
                  c = cstr[j];
                  if (c == '(') {
                     ++parenthesis;
                  } else if (c == ')') {
                     --parenthesis;
                     if (pstartcount == parenthesis) {
                        // found matching parenthesis for function call
                        i = j;
                        break;
                     }
                  }
               }
            }
         } else {
            CleanUp();
            throw EagleError(StringPrintF(
               "Function::ParseGroups - Function %s is not a registered function!\n" , buffer
            ));
         }
      } else {
         not_function_call = true;
      }
   }

   
#ifdef DEBUGFUNC
   OutputLog() << StringPrintF("After parsing groups, string is : '%s'\n" , str.c_str());
#endif
   parsestr = str;
}


/// Unary functions :
// negate : -exp
// ceil : ceil(exp)
// floor : floor(exp)
// sine : sin(exp)
// cosine : cos(exp)
// tangent : tan(exp)
// arcsine : asin(exp)
// arccosine : acos(exp)
// arctangent : atan(exp)
// square root : sqrt(exp)
// absolute value : abs(exp)
// logarithm base 10 : log10(exp)
// natural logarithm : ln(exp)
/// Binary functions
// modulus : fmod(x , y)
// base logarithm : logb(number , base)
// arc tangent 2 : atan2(y,x)

void Function::ParseFunctionArguments() {
   string str = parsestr;
   const char* cstr = str.c_str();
   char c = '\0';
   
   char buffer[512];
   int num = 0;
   int ncharsread1 = 0;
   int ncharsread2 = 0;
   bool is_function_call = false;
   bool negation = false;
   
   int argstart = 0;
   int argend = 0;
   int count = 0;
   
   int fstart = 0;
   int fend = 0;
   int fcount = 0;
   
   /// valid expressions here
   // -exp
   // function_name(...)
   
   // find first function call then for each comma separated argument,
   // create a new Function based on the contents of the argument
   // skip $f#...$'s
   
   for (size_t i = 0 ; i < strlen(cstr) ; ++i) {
      c = cstr[i];
      if (c == '(') {
         if (is_function_call) {
            // parse function call arguments
            argstart = i;
            int index = MatchingParenthesisIndex(cstr + i);
            if (index == -1) {
               CleanUp();
               throw EagleError("Function::ParseFunctionArguments - Function call parameter list not closed with matching parenthesis!\n");
            }
            argend = i + index;
            count = (argend - argstart) + 1;
            string sub = str.substr(argstart , count);
            string replacestr = ParseArgumentList(sub);
            str.replace(argstart , count , replacestr , 0 , replacestr.size());
            i += replacestr.size() - 1;
            cstr = str.c_str();
         } else {
            CleanUp();
            // error, should not be any parenthesis groups left after ParseGroups is called
            throw EagleError("Function::ParseFunctionArguments - parenthesis group not part of function call found!\n");
         }
         is_function_call = false;
      } else if (c == '$') {
         // skip symbol
         if (1 != sscanf(cstr + i , "$f%i$%n" , &num , &ncharsread1)) {
            CleanUp();
            throw EagleError("Function::ParseFunctionArguments - Symbol encountered, but not read properly!");
         }
         i += ncharsread1 - 1;
         is_function_call = false;
      } else if (c == '-') {
         // check if this is subtraction or negation
         // $f0$+-$f1$ $f0$*-$f1$ [+-*/^] $f0$--ln(10) $f0$+--ln(10)
         // all variables and constants outside of parenthesis are $f#...$'ed
         negation = false;
         if (i == 0) {
            negation = true;
         } else {
            c = cstr[i-1];
            // ---5 -> --$f0$
            if ((c == '+') || (c == '-') || (c == '*') || (c == '/') || (c == '^')) {
               negation = true;
            }
         }
         // if negation, check if the argument is already parsed or not
         if (negation) {
            argstart = i;
            // ---$f0$
            // ---ln(2)
            // --$f0$
            // --ln(2)
            
            // argument may be symbol ($f#...$) or function call or further negation?
            // if argument is symbol, skip the symbol
            // if argument is function call, create a new Function with the function call as the string to parse
            ++i;
            
            if (cstr[i] == '-') {
               // further negation
               while (cstr[i] == '-') {
                  negation = !negation;
                  ++i;
               }
               argend = i;// ---
               if (negation) {
                  // odd number of negations
                  // replace with single negation
                  ++argstart;
               } else {
                  // even number of negations
                  // replace with ""
               }
               count = argend - argstart;
               str.erase(argstart , count);
               cstr = str.c_str();
               i = argstart;
            }
            
            
            c = cstr[i];
            if (c == '$') {
               if (1 != sscanf(cstr + i , "$f%i$%n" , &num , &ncharsread1)) {
                  CleanUp();
                  throw EagleError("Function::ParseFunctionArguments - Symbol encountered after negation, but not read properly!\n");
               }
               i += ncharsread1 - 1;
            } else if (isalpha(c) || (c == '_')) {
               fstart = i;
               // should be function name now
               if (1 != sscanf(cstr + i , "%[_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ]%n" , buffer , &ncharsread1)) {
                  CleanUp();
                  throw EagleError("Function::ParseFunctionArguments - Lies! Lies!\n");
               }
               ncharsread2 = 0;
               sscanf(cstr + i + ncharsread1 , "%[_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789]%n" , buffer + ncharsread1 , &ncharsread2);
               i += ncharsread1 + ncharsread2;
               c = cstr[i];
               if (c != '(') {
                  CleanUp();
                  throw EagleError(StringPrintF(
                     "Function::ParseFunctionArguments - Parenthesis enclosed argument list expected after function call to '%s'!\n" , buffer
                  ));
               }
               int index = MatchingParenthesisIndex(cstr + i);
               if (index == -1) {
                  CleanUp();
                  throw EagleError(StringPrintF(
                     "Function::ParseFunctionArguments - Argument list of %s function not closed!\n" , buffer
                  ));
               }
               fend = i + index;
               fcount = (fend - fstart) + 1;
               string sub = str.substr(fstart , fcount);
               Function* f = new Function(sub);
               sprintf(buffer , "$f%u$" , termcount++);
               terms[buffer] = f;
               str.replace(fstart , fcount , string(buffer) , 0 , strlen(buffer));
               cstr = str.c_str();
               i = fstart + strlen(buffer) - 1;
            } else {
               CleanUp();
               // Unexpected argument
               throw EagleError(StringPrintF(
                  "Function::ParseFunctionArguments - Unexpected argument '%c' to negation function at index %i!\n" , cstr[i] , i
               ));
            }
         }
         is_function_call = false;
      } else if (isalpha(c) || (c == '_')) {
         // skip name
         if (1 != sscanf(cstr + i , "%[_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ]%n" , buffer , &ncharsread1)) {
            CleanUp();
            throw EagleError("Function::ParseFunctionArguments - Lies 2! Lies 2!\n");
         }
         ncharsread2 = 0;
         int nitems = sscanf(cstr + i + ncharsread1 , "%[_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789]%n" , buffer + ncharsread1 , &ncharsread2);
         if (!((nitems == EOF) || (nitems == 0) || (nitems == 1))) {
            CleanUp();
            throw EagleError("Function::ParseFunctionArguments - Bizarre behaviour from sscanf!");
         }
         i += ncharsread1 + ncharsread2 - 1;
         is_function_call = true;
      } else {
         is_function_call = false;
      }
      
   }
   
#ifdef DEBUGFUNC
   OutputLog() << StringPrintF("After parsing function arguments, string is : '%s'\n" , str.c_str());
#endif
   parsestr = str;
}



string Function::ParseArgumentList(string list) {
   // (a,b,c) (1,ln(10),1e-34) ()
   string ret;
   const char* const cstr = list.c_str();
   int argstart = 0;
   int argend = 0;
   char buffer[64];

   if (cstr[0] != '(') {
      CleanUp();
      throw EagleError("Function::ParseArgumentList - List does not begin with an opening parenthesis!\n");
   }

   // find closing parenthesis
   int pcloseindex = MatchingParenthesisIndex(cstr);
   if (pcloseindex == -1) {
      CleanUp();
      throw EagleError("Function::ParseArgumentList - No matching parenthesis found!\n");
   }
   const char* start = cstr;
   const char* const pclose = cstr + pcloseindex;
   ret.append(1,'(');

   // parse argument list
   int startoffset = 0;
   while (1) {
      const char* comma = strchr(start + startoffset , ',');
      const char* stop = 0;
      if (comma && (comma < pclose)) {
         stop = comma;
         startoffset = 1;
      } else {
         stop = pclose;
      }
      argstart = (start - cstr) + 1;
      argend = (stop - cstr) - 1;
      int count = (argend - argstart) + 1;
      if (count) {
         string sub = list.substr(argstart , count);
         Function* f = new Function(sub);
         sprintf(buffer , "$f%u$" , termcount++);
         ret.append(buffer);
         terms[buffer] = f;
      }
      start = stop;
      if (stop == pclose) {
         break;
      } else {
         ret.append(1,',');
      }
   }
   ret.append(1,')');
   
   return ret;
}



vector<FunctionBase*> Function::ParseArgumentListSymbols(string list) {
   // ($f0$)
   const char* cstr = list.c_str();
   char buffer[64];
   int ncharsread = 0;
   size_t fnum = 0;
   vector<FunctionBase*> funvec;
   
   if (cstr[0] != '(') {
      CleanUp();
      throw EagleError("Function::ParseArgumentListSymbols - argument list does not start with an opening parenthesis!\n");
   }
   if (cstr[1] == ')') {
      return funvec;
   }
   for (size_t i = 1 ; i < strlen(cstr) ; ++i) {
      if (1 != sscanf(cstr + i , "$f%u$%n" , &fnum , &ncharsread)) {
         CleanUp();
         throw EagleError("Function::ParseArgumentListSymbols - Did not read function symbol correctly!\n");
      }
      sprintf(buffer , "$f%u$" , fnum);
      FunctionBase* f = terms[buffer];
      funvec.push_back(f);
      i += ncharsread;
      if (cstr[i] == ')') {
         break;
      }
   }
   return funvec;
}



void Function::GetLeftAndRightTerms(const char* cstr , int index , int* storestart , int* storeend , FunctionBase** storeleft , FunctionBase** storeright) {
   int b = index;
   int e = index;
   
   char buffer[64];
   
   int num = 0;
   int ncharsread = 0;
   
   ASSERT(storestart);
   ASSERT(storeend);
   ASSERT(storeleft);
   ASSERT(storeright);
   
   --b;
   ++e;
   if (cstr[b] != '$') {
      CleanUp();
      throw EagleError("Function::GetLeftAndRightTerms - character before index is not a $!\n");
   }
   if (cstr[e] != '$') {
      CleanUp();
      throw EagleError("Function::GetLeftAndRightTerms - character after index is not a $!\n");
   }
   --b;
   int offset = ReverseStringSearch(cstr , b , '$');
   if (offset == 1) {
      CleanUp();
      throw EagleError("Function::GetLeftAndRightTerms - Beginning of left function symbol not found!\n");
   }
   b += offset;
   
   // $f0$^$f1$
   // read left function symbol
   if (1 != sscanf(cstr + b , "$f%i$%n" , &num , &ncharsread)) {
      CleanUp();
      throw EagleError("Function::GetLeftAndRightTerms - Left function symbol not read correctly!\n");
   }
   if ((b + ncharsread) != index) {
      CleanUp();
      throw EagleError("Function::GetLeftAndRightTerms - Error : Left function symbol not the only characters left of index!\n");
   }
   sprintf(buffer , "$f%i$" , num);
   map<string , FunctionBase*>::iterator it = terms.find(buffer);
   if (it == terms.end()) {
      CleanUp();
      throw EagleError("Function::GetLeftAndRightTerms - Left function symbol not found in lookup table!\n");
   }
   *storestart = b;
   *storeleft = it->second;
   
   // read right function symbol
   if (1 != sscanf(cstr + e , "$f%i$%n" , &num , &ncharsread)) {
      CleanUp();
      throw EagleError("Function::GetLeftAndRightTerms - Right function symbol not read correctly!\n");
   }
   sprintf(buffer , "$f%i$" , num);
   it = terms.find(buffer);
   if (it == terms.end()) {
      CleanUp();
      throw EagleError("Function::GetLeftAndRightTerms - Right function symbol not found in lookup table!\n");
   }
   e += ncharsread - 1;
   *storeend = e;
   *storeright = it->second;
}



void Function::ParseFunctionCalls() {
   // Once all function arguments have been parsed, parse the function calls
   string str = parsestr;
   const char* cstr = str.c_str();
   
   char buffer[512];
   char c = '\0';
   
   int fstart = 0;
   int fend = 0;
   int fcount = 0;
   int argstart = 0;
   int argend = 0;
   int count = 0;
   int num;
   
   int ncharsread1 = 0;
   int ncharsread2 = 0;
   
   /// -$f1$^$f0$
   for (int i = 0 ; i < (int)strlen(cstr) ; ++i) {
      c = cstr[i];
      if (c == '$') {
         // skip function symbol
         if (1 != sscanf(cstr + i , "$f%i$%n" , &num , &ncharsread1)) {
            CleanUp();
            throw EagleError("Function::ParseFunctionCalls - Error reading function symbol!\n");
         }
         i += ncharsread1 - 1;
      } else if (c == '-') {
         // could be subtraction
         // if at start or preceeded by [+-*/^] then is subtraction
         bool is_negation = false;
         if (i == 0) {
            is_negation = true;
         } else {
            c = cstr[i - 1];
            if ((c == '+') || (c == '-') || (c == '*') || (c == '/') || (c == '^')) {
               is_negation = true;
            }
         }
         if (is_negation) {
            fstart = i;
            ++i;
            c = cstr[i];
            // next symbol should be a function symbol. It should not be a function call because ParseFunctionArguments should have $'ed it
            if (c == '$') {
               if (1 != sscanf(cstr + i , "$f%i$%n" , &num , &ncharsread1)) {
                  CleanUp();
                  throw EagleError("Function::ParseFunctionCalls - Error reading function symbol 2!\n");
               }
               // -$f#$
               fend = fstart + ncharsread1 + 1;
///               count = (fend - fstart) + 1;
               count = (fend - fstart);
               sprintf(buffer , "$f%i$" , num);
               FunctionBase* f = terms[buffer];
               FunctionBase* subfunc = new NegateFunction(f);
               sprintf(buffer , "$f%u$" , termcount++);
               terms[buffer] = subfunc;
               str.replace(fstart , count , string(buffer) , 0 , strlen(buffer));
               cstr = str.c_str();
               i += strlen(buffer) - 1;
            } else {
               CleanUp();
               throw EagleError("Function::ParseFunctionCalls - Expected function symbol after negation symbol!\n");
            }
         }
      } else if (isalpha(c) || (c == '_')) {
         fstart = i;
         if (1 != sscanf(cstr + i , "%[_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ]%n" , buffer , &ncharsread1)) {
            CleanUp();
            throw EagleError("Function::ParseFunctionCalls - Lies! Lies!\n");
         }
         ncharsread2 = 0;
         sscanf(cstr + i + ncharsread1 , "%[_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789]%n" , buffer + ncharsread1 , &ncharsread2);
         // look up function name in function_set
         if (function_set.find(string(buffer)) == function_set.end()) {
            CleanUp();
            throw EagleError(StringPrintF(
               "Function::ParseFunctionCalls - Function %s not found in function set!\n" , buffer
            ));
         }
         
         // get function argument list
         i += ncharsread1 + ncharsread2;
         
         if (cstr[i] != '(') {
            CleanUp();
            throw EagleError(StringPrintF(
               "Function::ParseFunctionCalls - Function %s not followed by parenthesis enclosed argument list!\n" , buffer
            ));
         }
         argstart = i;
         int index = MatchingParenthesisIndex(cstr + i);
         if (index == -1) {
            CleanUp();
            throw EagleError(StringPrintF(
               "Function::ParseFunctionCalls - Argument list to function %s not closed with matching parenthesis!\n" , buffer
            ));
         }
         argend = argstart + index;
         count = (argend - argstart) + 1;
         fend = argend;
         fcount = (fend - fstart) + 1;
         string sub = str.substr(argstart , count);
         vector<FunctionBase*> fargs = ParseArgumentListSymbols(sub);
         
         FunctionBase* newfunc = 0;
         
         // see if this is a user function
         map<string , CustomFunction*>::iterator it = user_function_set.find(string(buffer));
         if (it != user_function_set.end()) {
            // custom function
            try {
               newfunc = new CustomFunction(it->first , fargs , false);
            }
            catch (...) {
               CleanUp();
               throw;
            }
         } else {
            // built in functions
            // ceil : ceil(exp)
            // floor : floor(exp)
            // sine : sin(exp)
            // cosine : cos(exp)
            // tangent : tan(exp)
            // arcsine : asin(exp)
            // arccosine : acos(exp)
            // arctangent : atan(exp)
            // square root : sqrt(exp)
            // absolute value : abs(exp)
            // logarithm base 10 : log10(exp)
            // natural logarithm : ln(exp)
            // base logarithm : logb(number , base)
            // arc tangent 2 : atan2(y,x)
            // power : pow(b,e)
            if (fargs.size() == 1) {
               if (strcmp(buffer , "ceil") == 0) {
                  newfunc = new CeilFunction(fargs[0]);
               } else if (strcmp(buffer , "floor") == 0) {
                  newfunc = new FloorFunction(fargs[0]);
               } else if (strcmp(buffer , "sin") == 0) {
                  newfunc = new SineFunction(fargs[0]);
               } else if (strcmp(buffer , "cos") == 0) {
                  newfunc = new CosineFunction(fargs[0]);
               } else if (strcmp(buffer , "tan") == 0) {
                  newfunc = new TangentFunction(fargs[0]);
               } else if (strcmp(buffer , "asin") == 0) {
                  newfunc = new ArcSineFunction(fargs[0]);
               } else if (strcmp(buffer , "acos") == 0) {
                  newfunc = new ArcCosineFunction(fargs[0]);
               } else if (strcmp(buffer , "atan") == 0) {
                  newfunc = new ArcTangentFunction(fargs[0]);
               } else if (strcmp(buffer , "sqrt") == 0) {
                  newfunc = new SquareRootFunction(fargs[0]);
               } else if (strcmp(buffer , "abs") == 0) {
                  newfunc = new AbsoluteValueFunction(fargs[0]);
               } else if (strcmp(buffer , "log10") == 0) {
                  newfunc = new Logarithm10Function(fargs[0]);
               } else if (strcmp(buffer , "ln") == 0) {
                  newfunc = new NaturalLogarithmFunction(fargs[0]);
               } else {
                  throw EagleError(StringPrintF(
                     "Function::ParseFunctionCalls - Function %s passed one argument, but was not found in unary functions!\n" , buffer
                  ));
               }
            } else if (fargs.size() == 2) {
               if (strcmp(buffer , "logb") == 0) {
                  newfunc = new BaseLogarithmFunction(fargs[0] , fargs[1]);
               } else if (strcmp(buffer , "atan2") == 0) {
                  newfunc = new ArcTangent2Function(fargs[0] , fargs[1]);
               } else if (strcmp(buffer , "pow") == 0) {
                  newfunc = new PowerFunction(fargs[0] , fargs[1]);
               } else {
                  CleanUp();
                  throw EagleError(StringPrintF(
                     "Function::ParseFunctionCalls - Function %s passed two arguments, but was not found!\n" , buffer
                  ));
               }
            } else {
               CleanUp();
               throw EagleError(StringPrintF(
                  "Function::ParseFunctionCalls - Function %s passed %u arguments, and was not found!\n" , buffer , fargs.size()
               ));
            }
         }
         
         sprintf(buffer , "$f%u$" , termcount++);
         terms[buffer] = newfunc;
         str.replace(fstart , fcount , string(buffer) , 0 , strlen(buffer));
         i = fstart + strlen(buffer) - 1;
//         i += strlen(buffer) - 1;
         cstr = str.c_str();
      }
   }
   
#ifdef DEBUGFUNC
   OutputLog() << StringPrintF("After parsing function calls, string is '%s'\n" , str.c_str());
#endif
   parsestr = str;
}



/// Binary functions
// power : a ^ b right to left (a^b^c) = a^(b^c)
void Function::ParseExponents() {
   string str = parsestr;
   const char* cstr = str.c_str();
   
   char buffer[512];
   int start = 0;
   int end = 0;
   FunctionBase* base = 0;
   FunctionBase* exponent = 0;
   
   int i = (int)strlen(cstr) - 1;
   while (i >= 0) {
      int index = ReverseStringSearch(cstr , i , '^');
      if (index == 1) {break;}
      i += index;
      // cstr[i] == '^';
///   void GetLeftAndRightTerms(const char* cstr , int index , int* storestart , int* storeend , FunctionBase* storeleft , FunctionBase* storeright);
      FunctionBase* f = 0;
      try {
         GetLeftAndRightTerms(cstr , i , &start , &end , &base , &exponent);
         f = new PowerFunction(base , exponent);
      }
      catch (...) {
         /// TODO : Cleanup();
         throw;
      }
      sprintf(buffer , "$f%u$" , termcount++);
      terms[buffer] = f;
      int count = (end - start) + 1;
      str.replace(start , count , string(buffer) , 0 , strlen(buffer));
      cstr = str.c_str();
      i = start-1;
   }
   
#ifdef DEBUGFUNC
   OutputLog() << StringPrintF("After parsing exponents , string is '%s'\n" , str.c_str());
#endif
   parsestr = str;
}


/// Binary functions
// multiply : a * b
// divide : a / b
void Function::ParseMultiply() {
   
   string str = parsestr;
   const char* cstr = str.c_str();
   char buffer[64];
   
   int start = 0;
   int end = 0;
   FunctionBase* left = 0;
   FunctionBase* right = 0;
   FunctionBase* f = 0;
   bool addfunction = false;
   
   for (int i = 0 ; i < (int)strlen(cstr) ; ++i) {
      char c = cstr[i];
      addfunction = false;
      if (c == '*') {
         try {
            GetLeftAndRightTerms(cstr , i , &start , &end , &left , &right);
            f = new MultiplyFunction(left , right);
         }
         catch (...) {
            CleanUp();
            throw;
         }
         addfunction = true;
      }
      if (addfunction) {
         int count = (end - start) + 1;
         sprintf(buffer , "$f%u$" , termcount++);
         terms[buffer] = f;
         str.replace(start , count , string(buffer) , 0 , strlen(buffer));
         i = start + strlen(buffer) - 1;
         cstr = str.c_str();
      }
   }
#ifdef DEBUGFUNC
   OutputLog() << StringPrintF("After parsing multiply, string is : '%s'\n" , str.c_str());
#endif
   parsestr = str;
}



void Function::ParseDivide() {
   
   string str = parsestr;
   const char* cstr = str.c_str();
   char buffer[64];
   
   int start = 0;
   int end = 0;
   FunctionBase* left = 0;
   FunctionBase* right = 0;
   FunctionBase* f = 0;
   bool addfunction = false;
   
   for (int i = 0 ; i < (int)strlen(cstr) ; ++i) {
      char c = cstr[i];
      addfunction = false;
      if (c == '/') {
         try {
            GetLeftAndRightTerms(cstr , i , &start , &end , &left , &right);
            f = new DivideFunction(left , right);
         }
         catch (...) {
            CleanUp();
            throw;
         }
         addfunction = true;
      }
      if (addfunction) {
         int count = (end - start) + 1;
         sprintf(buffer , "$f%u$" , termcount++);
         terms[buffer] = f;
         str.replace(start , count , string(buffer) , 0 , strlen(buffer));
         i = start + strlen(buffer) - 1;
         cstr = str.c_str();
      }
   }
#ifdef DEBUGFUNC
   OutputLog() << StringPrintF("After parsing divide, string is : '%s'\n" , str.c_str());
#endif
   parsestr = str;
}


void Function::ParseSubtract() {
   /// All subtraction is turned into addition through negation
   /// a - b -> a + -b
   string str = parsestr;
   const char* cstr = str.c_str();
   char buffer[64];
   
   int start = 0;
   int end = 0;
   int count = 0;
   
   FunctionBase* left = 0;
   FunctionBase* right = 0;
   FunctionBase* f = 0;
   
   for (int i = 0 ; i < (int)strlen(cstr) ; ++i) {
      if (cstr[i] == '-') {
         try {
            GetLeftAndRightTerms(cstr , i , &start , &end , &left , &right);
            f = new NegateFunction(right);
         }
         catch (...) {
            CleanUp();
            throw;
         }
         str[i] = '+';
         cstr = str.c_str();
         start = i + 1;
         count = (end - start) + 1;
         sprintf(buffer , "$f%u$" , termcount++);
         terms[buffer] = f;
         str.replace(start , count , string(buffer) , 0 , strlen(buffer));
         cstr = str.c_str();
         i = start + (int)strlen(buffer) - 1;
      }
   }
#ifdef DEBUGFUNC
   OutputLog() << StringPrintF("After parsing subtraction, the string is : '%s'\n" , str.c_str());
#endif
   parsestr = str;
}



void Function::ParseAdd() {
   /// Should only be addition left
   /// We can reorder the terms in any way we choose
   
   string str = parsestr;
   const char* cstr = str.c_str();
   char buffer[64];
   
   int start = 0;
   int end = 0;
   int count = 0;
   
   FunctionBase* left = 0;
   FunctionBase* right = 0;
   FunctionBase* f = 0;
   
   /// First, collect all the terms
   //  $f0$ + $f1$
   /// Second, sort the terms
   /// Third, evaluate constants
   
   
   for (int i = 0 ; i < (int)strlen(cstr) ; ++i) {
      if (cstr[i] == '+') {
///   void GetLeftAndRightTerms(const char* cstr , int index , int* storestart , int* storeend , FunctionBase* storeleft , FunctionBase* storeright);
         try {
            GetLeftAndRightTerms(cstr , i , &start , &end , &left , &right);
            f = new AddFunction(left , right);
         }
         catch (...) {
            CleanUp();
            throw;
         }
         count = (end - start) + 1;
         sprintf(buffer , "$f%u$" , termcount++);
         terms[buffer] = f;
         str.replace(start , count , string(buffer) , 0 , strlen(buffer));
         i = start + strlen(buffer) - 1;
         cstr = str.c_str();
      }
   }
#ifdef DEBUGFUNC
   OutputLog() << StringPrintF("After parsing addition , string is : '%s'\n" , str.c_str());
#endif
   parsestr = str;
}



void Function::ParseFinal() {
   // Should only have one argument left, assign it to root
   const char* cstr = parsestr.c_str();
   int num = 0;
   int ncharsread = 0;
   
   if (1 != sscanf(cstr , "$f%i$%n" , &num , &ncharsread)) {
      CleanUp();
      throw EagleError(StringPrintF(
         "Function::ParseFinal - failed to read a valid function symbol from '%s'!\n" , cstr
      ));
   }
   if (ncharsread != (int)strlen(cstr)) {
      CleanUp();
      throw EagleError(StringPrintF(
         "Function::ParseFinal - '%s' is not a valid function symbol!\n" , cstr
      ));
   }
   root = terms[cstr];
   inputset = root->InputSet();
   SetNumArgs(inputset.size());
}



void Function::RemoveTerm(std::string term) {
   map<string , FunctionBase*>::iterator it = terms.find(term);
   if (it == terms.end()) {return;}
   FunctionBase* troot = it->second;
   vector<string> remove;
   remove.push_back(term);
   for (map<string , FunctionBase*>::iterator mit = terms.begin() ; mit != terms.end() ; ++mit) {
      if (troot->WillCall(mit->second)) {
         remove.push_back(mit->first);
      }
   }
   for (size_t i = 0 ; i < remove.size() ; ++i) {
      terms.erase(terms.find(remove[i]));
   }
   delete troot;
}



Function::Function(Function* f) :
      FunctionBase(0),
      exp(),
      parsestr(),
      root(0),
      terms(),
      termcount(0),
      termbuf(),
      function_set()
{
   if (!f) {
      throw EagleError("Function::Function(Function* f) -  f is NULL!\n");
   }
   if (!f->root) {
      throw EagleError("Function::Function(Function* f) - f->root is NULL!\n");
   }
   inputset = f->inputset;
   SetNumArgs(f->NumArgs());
   exp = f->exp;
   root = f->root->Clone();
   /** Cloning the terms is wrong, as they are never freed, and only refer to each function call made during evaluation
       I guess just leave terms empty, as they are not used once the function is parsed anyway.
   for (map<string , FunctionBase*>::iterator it = f->terms.begin() ; it != f->terms.end() ; ++it) {
      terms[it->first] = it->second->Clone();
   }
   */
   termcount = f->termcount;
   function_set = f->function_set;
}



Function::Function(const std::string& expression) :
      FunctionBase(0),
      exp(),
      parsestr(),
      root(0),
      terms(),
      termcount(0),
      termbuf(),
      function_set()
{
   Parse(expression);
}



Function::~Function() {Free();}



void Function::Free() {
   if (root) {
      delete root;
      root = 0;
   }
   exp = "";
   parsestr = "";
   terms.clear();
   termcount = 0;
}



void Function::Parse(const std::string& expression) {
   Free();
   exp = expression;
   parsestr = expression;
   function_set = GetFunctionSet();
#ifdef DEBUGFUNC
   OutputLog() << StringPrintF("Before parsing , the string is : '%s'\n" , parsestr.c_str());
#endif
   ParseCheck();
   ParseConstants();
   ParseVariables();
   ParseGroups();
   ParseFunctionArguments();
   ParseFunctionCalls();
   ParseExponents();
   ParseMultiply();
   ParseDivide();
   ParseSubtract();
   ParseAdd();
   ParseFinal();
}



SOLVER_RESULT Function::Evaluate(const std::map<std::string , double>& valuemap , double* result) {
   ASSERT(result);
   if (root) {
      return root->Evaluate(valuemap , result);
   }
   return RESULT_ERROR;
}



vector<FunctionBase*> Function::Terms() {
   if (!root) {
      throw EagleError("Function::Terms - root is NULL!\n");
   }
   return root->Terms();
}



bool Function::WillCall(FunctionBase* f) {
   if (f == root) {return true;}
   if (root->WillCall(f)) {return true;}
   return false;
}



bool Function::IsConstant(const std::map<std::string , double>& valuemap) {
   return root->IsConstant(valuemap);
}



FunctionBase* Function::Simplify(const std::map<std::string , double>& valuemap) {
   return root->Simplify(valuemap);
}




