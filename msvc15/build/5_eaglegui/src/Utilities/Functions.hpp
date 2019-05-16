

#ifndef Functions_H
#define Functions_H


#include <map>
#include <string>
#include <set>
#include <vector>

#include "Eagle.hpp"


class CustomFunction;

extern std::map<std::string , CustomFunction*> user_function_set;

void FreeUserFunctions();

std::set<std::string> GetFunctionSet();



/// ABSTRACT base class
class FunctionBase {
private :
   size_t num_args;
protected :
   std::set<std::string> inputset;
   
   void SetNumArgs(size_t number_of_args) {num_args = number_of_args;}
public :
   FunctionBase(size_t number_of_args) : num_args(number_of_args) , inputset() {}
   virtual ~FunctionBase() {}

   virtual SOLVER_RESULT Evaluate(const std::map<std::string , double>& valuemap , double* result)=0;
   
   virtual FunctionBase* Clone()=0;
   virtual std::vector<FunctionBase*> Terms()=0;
   virtual bool WillCall(FunctionBase* f)=0;
   virtual bool IsConstant()=0;
   virtual bool IsConstant(const std::map<std::string , double>& valuemap)=0;
   virtual bool IsCompoundStatement()=0;// compound statements - a + b , a - b , c/d
   virtual std::string Print()=0;
   virtual FunctionBase* Simplify(const std::map<std::string , double>& valuemap)=0;
   

   const std::set<std::string>& InputSet() {return inputset;}
   const size_t NumArgs() {return num_args;}
};



class ConstantFunction : public FunctionBase {
private :
   double c;
public :
   ConstantFunction(double constant) : FunctionBase(0) , c(constant) {}
   
   SOLVER_RESULT Evaluate(const std::map<std::string , double>& valuemap , double* result);
   
   FunctionBase* Clone() {return new ConstantFunction(c);}
   std::vector<FunctionBase*> Terms() {return std::vector<FunctionBase*>();}
   bool WillCall(FunctionBase* f) {return false;}
   bool IsConstant() {return true;}
   bool IsConstant(const std::map<std::string , double>& valuemap) {return true;}
   bool IsCompoundStatement() {return false;}
   std::string Print() {return FormatDouble(c);}
   FunctionBase* Simplify(const std::map<std::string , double>& valuemap);
};



class VariableFunction : public FunctionBase {
private :
   std::string var;
public :
   VariableFunction(const std::string& variable);
   
   SOLVER_RESULT Evaluate(const std::map<std::string , double>& valuemap , double* result);
   
   FunctionBase* Clone() {return new VariableFunction(var);}
   std::vector<FunctionBase*> Terms() {return std::vector<FunctionBase*>();}
   bool WillCall(FunctionBase* f) {return false;}
   bool IsConstant() {return false;}
   bool IsConstant(const std::map<std::string , double>& valuemap);
   bool IsCompoundStatement() {return false;}
   std::string Print() {return var;}
   FunctionBase* Simplify(const std::map<std::string , double>& valuemap);
};



/// ABSTRACT base class
class UnaryFunction : public FunctionBase {
protected :
   FunctionBase* term;
public :
   UnaryFunction(FunctionBase* term1);
   ~UnaryFunction() {delete term;}
   
   /// virtual double Evaluate(const std::map<std::string , double>& valuemap)=0;// STILL ABSTRACT!
   ///virtual FunctionBase* Clone()=0;// STILL ABSTRACT!
   std::vector<FunctionBase*> Terms();
   bool WillCall(FunctionBase* f);
   bool IsConstant();
   bool IsConstant(const std::map<std::string , double>& valuemap);
   bool IsCompoundStatement() {return false;}
   /// virtual std::string Print()=0;// STILL ABSTRACT!
   FunctionBase* Simplify(const std::map<std::string , double>& valuemap);
};



/// ABSTRACT base class
class BinaryFunction : public FunctionBase {
protected :
   FunctionBase* lterm;
   FunctionBase* rterm;
public :
   BinaryFunction(FunctionBase* leftterm , FunctionBase* rightterm);
   ~BinaryFunction() {delete lterm;delete rterm;}
   
   /// virtual double Evaluate(const std::map<std::string , double>& valuemap)=0;// STILL ABSTRACT!   
   ///virtual FunctionBase* Clone()=0;// STILL ABSTRACT!
   std::vector<FunctionBase*> Terms();
   bool WillCall(FunctionBase* f);
   bool IsConstant();
   bool IsConstant(const std::map<std::string , double>& valuemap);
   /// virtual bool IsCompoundStatement()=0;// STILL ABSTRACT!
   /// virtual std::string Print()=0;// STILL ABSTRACT!
   /// virtual FunctionBase* Simplify(const std::map<std::string , double>& valuemap)=0;// STILL ABSTRACT!
};



/// ------------------------------     Unary functions     ---------------------------------------


class NegateFunction : public UnaryFunction {
private :
   
public :
   NegateFunction(FunctionBase* term1) : UnaryFunction(term1) {}
   
   SOLVER_RESULT Evaluate(const std::map<std::string , double>& valuemap , double* result);
   FunctionBase* Clone() {return new NegateFunction(term->Clone());}
   std::string Print();
};



class CeilFunction : public UnaryFunction {
private :
   
public :
   CeilFunction(FunctionBase* term1) : UnaryFunction(term1) {}
   
   SOLVER_RESULT Evaluate(const std::map<std::string , double>& valuemap , double* result);
   FunctionBase* Clone() {return new CeilFunction(term->Clone());}
   std::string Print();
};



class FloorFunction : public UnaryFunction {
private :
   
public :
   FloorFunction(FunctionBase* term1) : UnaryFunction(term1) {}
   
   SOLVER_RESULT Evaluate(const std::map<std::string , double>& valuemap , double* result);
   FunctionBase* Clone() {return new FloorFunction(term->Clone());}
   std::string Print();
};


class SineFunction : public UnaryFunction {
private :
   
public :
   SineFunction(FunctionBase* term1) : UnaryFunction(term1) {}
   
   SOLVER_RESULT Evaluate(const std::map<std::string , double>& valuemap , double* result);
   FunctionBase* Clone() {return new SineFunction(term->Clone());}
   std::string Print();
};



class CosineFunction : public UnaryFunction {
private :
   
public :
   CosineFunction(FunctionBase* term1) : UnaryFunction(term1) {}
   
   SOLVER_RESULT Evaluate(const std::map<std::string , double>& valuemap , double* result);
   FunctionBase* Clone() {return new CosineFunction(term->Clone());}
   std::string Print();
};



class TangentFunction : public UnaryFunction {
private :

public :
   TangentFunction(FunctionBase* term1) : UnaryFunction(term1) {}
   
   SOLVER_RESULT Evaluate(const std::map<std::string , double>& valuemap , double* result);
   FunctionBase* Clone() {return new TangentFunction(term->Clone());}
   std::string Print();
};



class ArcSineFunction : public UnaryFunction {
private :

public :
   ArcSineFunction(FunctionBase* term1) : UnaryFunction(term1) {}
   
   SOLVER_RESULT Evaluate(const std::map<std::string , double>& valuemap , double* result);
   FunctionBase* Clone() {return new ArcSineFunction(term->Clone());}
   std::string Print();
};



class ArcCosineFunction : public UnaryFunction {
private :
   
public :
   ArcCosineFunction(FunctionBase* term1) : UnaryFunction(term1) {}
   
   SOLVER_RESULT Evaluate(const std::map<std::string , double>& valuemap , double* result);
   FunctionBase* Clone() {return new ArcCosineFunction(term->Clone());}
   std::string Print();
};



class ArcTangentFunction : public UnaryFunction {
private :
   
public :
   ArcTangentFunction(FunctionBase* term1) : UnaryFunction(term1) {}
   
   SOLVER_RESULT Evaluate(const std::map<std::string , double>& valuemap , double* result);
   FunctionBase* Clone() {return new ArcTangentFunction(term->Clone());}
   std::string Print();
};



class SquareRootFunction : public UnaryFunction {
private :
   
public :
   SquareRootFunction(FunctionBase* term1) : UnaryFunction(term1) {}
   
   SOLVER_RESULT Evaluate(const std::map<std::string , double>& valuemap , double* result);
   FunctionBase* Clone() {return new SquareRootFunction(term->Clone());}
   std::string Print();
};



class AbsoluteValueFunction : public UnaryFunction {
private :
   
public :
   AbsoluteValueFunction(FunctionBase* term1) : UnaryFunction(term1) {}
   
   SOLVER_RESULT Evaluate(const std::map<std::string , double>& valuemap , double* result);
   FunctionBase* Clone() {return new AbsoluteValueFunction(term->Clone());}
   std::string Print();
};



class Logarithm10Function : public UnaryFunction {
private :
   
public :
   Logarithm10Function(FunctionBase* term1) : UnaryFunction(term1) {}
   
   SOLVER_RESULT Evaluate(const std::map<std::string , double>& valuemap , double* result);
   FunctionBase* Clone() {return new Logarithm10Function(term->Clone());}
   std::string Print();
};



class NaturalLogarithmFunction : public UnaryFunction {
private :
   
public :
   NaturalLogarithmFunction(FunctionBase* term1) : UnaryFunction(term1) {}
   
   SOLVER_RESULT Evaluate(const std::map<std::string , double>& valuemap , double* result);
   FunctionBase* Clone() {return new NaturalLogarithmFunction(term->Clone());}
   std::string Print();
};



/// ---------------------------------     Binary functions     --------------------------------------




class AddFunction : public BinaryFunction {
private :
   
public :
   AddFunction(FunctionBase* lhs , FunctionBase* rhs) : BinaryFunction(lhs,rhs) {}
   
   SOLVER_RESULT Evaluate(const std::map<std::string , double>& valuemap , double* result);
   FunctionBase* Clone() {return new AddFunction(lterm->Clone() , rterm->Clone());}
   bool IsCompoundStatement() {return true;}
   std::string Print();
   FunctionBase* Simplify(const std::map<std::string , double>& valuemap);
};



class SubtractFunction : public BinaryFunction {
private :
   
public :
   SubtractFunction(FunctionBase* lhs , FunctionBase* rhs) : BinaryFunction(lhs,rhs) {}
   
   SOLVER_RESULT Evaluate(const std::map<std::string , double>& valuemap , double* result);
   FunctionBase* Clone() {return new SubtractFunction(lterm->Clone() , rterm->Clone());}
   bool IsCompoundStatement() {return true;}
   std::string Print();
   FunctionBase* Simplify(const std::map<std::string , double>& valuemap);
};



class MultiplyFunction : public BinaryFunction {
private :
   
public :
   MultiplyFunction(FunctionBase* lhs , FunctionBase* rhs) : BinaryFunction(lhs,rhs) {}
   
   SOLVER_RESULT Evaluate(const std::map<std::string , double>& valuemap , double* result);
   FunctionBase* Clone() {return new MultiplyFunction(lterm->Clone() , rterm->Clone());}
   bool IsCompoundStatement() {return false;}
   std::string Print();
   FunctionBase* Simplify(const std::map<std::string , double>& valuemap);
};



class DivideFunction : public BinaryFunction {
private :
   
public :
   DivideFunction(FunctionBase* num , FunctionBase* den) : BinaryFunction(num,den) {}
   
   SOLVER_RESULT Evaluate(const std::map<std::string , double>& valuemap , double* result);
   FunctionBase* Clone() {return new DivideFunction(lterm->Clone() , rterm->Clone());}
   bool IsCompoundStatement() {return true;}
   std::string Print();
   FunctionBase* Simplify(const std::map<std::string , double>& valuemap);
};



class ModulusFunction : public BinaryFunction {
private :
   
public :
   ModulusFunction(FunctionBase* lhs , FunctionBase* rhs) : BinaryFunction(lhs,rhs) {}
   
   SOLVER_RESULT Evaluate(const std::map<std::string , double>& valuemap , double* result);
   FunctionBase* Clone() {return new ModulusFunction(lterm->Clone() , rterm->Clone());}
   bool IsCompoundStatement() {return false;}
   std::string Print();
   FunctionBase* Simplify(const std::map<std::string , double>& valuemap);
};



class PowerFunction : public BinaryFunction {
private :
   
public :
   PowerFunction(FunctionBase* base , FunctionBase* exponent) : BinaryFunction(base,exponent) {}
   
   SOLVER_RESULT Evaluate(const std::map<std::string , double>& valuemap , double* result);
   FunctionBase* Clone() {return new PowerFunction(lterm->Clone() , rterm->Clone());}
   bool IsCompoundStatement() {return false;}
   std::string Print();
   FunctionBase* Simplify(const std::map<std::string , double>& valuemap);
};



class BaseLogarithmFunction : public BinaryFunction {
private :
   
public :
   BaseLogarithmFunction(FunctionBase* num , FunctionBase* base) : BinaryFunction(num,base) {}
   
   SOLVER_RESULT Evaluate(const std::map<std::string , double>& valuemap , double* result);
   FunctionBase* Clone() {return new BaseLogarithmFunction(lterm->Clone() , rterm->Clone());}
   bool IsCompoundStatement() {return false;}
   std::string Print();
   FunctionBase* Simplify(const std::map<std::string , double>& valuemap);
};



class ArcTangent2Function : public BinaryFunction {
private :
   
public :
   ArcTangent2Function(FunctionBase* y , FunctionBase* x) : BinaryFunction(y,x) {}
   
   SOLVER_RESULT Evaluate(const std::map<std::string , double>& valuemap , double* result);
   FunctionBase* Clone() {return new ArcTangent2Function(lterm->Clone() , rterm->Clone());}
   bool IsCompoundStatement() {return false;}
   std::string Print();
   FunctionBase* Simplify(const std::map<std::string , double>& valuemap);
};



/// ------------------------------      CustomFunction class     --------------------------------------



class CustomFunction : public FunctionBase {
private :
   std::string def;
   std::string name;
   std::vector<std::string> vars;
   std::vector<FunctionBase*> args;
   FunctionBase* eval;
   bool is_original;

   void Parse(const std::string& definition);

   // - The private constructor takes premade parameters and creates the definition by using PrintDefinition
   CustomFunction(std::set<std::string> new_inputset , std::string function_name , std::vector<std::string> variables , std::vector<FunctionBase*> arguments , FunctionBase* expression);

public :
   // 3 public constructors
   // - One defines a new function 'function_name(p1,p2,...) = p1 + p2 - p3 etc...
   CustomFunction(const std::string& definition);
   // - The second looks the function up in the user_function_set
   CustomFunction(const std::string& function_name , std::vector<FunctionBase*> arguments , bool cloneargs);
   // - The third defines a new function using a name and an evaluation function
   CustomFunction(std::string function_name , FunctionBase* expression , bool clone);

   ~CustomFunction();
   void Free();

   std::string PrintDefinition();
   
   std::string Name() {return name;}
   

   SOLVER_RESULT Evaluate(const std::map<std::string , double>& valuemap , double* result);

   FunctionBase* Clone() {return new CustomFunction(name , args , true);}
   std::vector<FunctionBase*> Terms();
   bool WillCall(FunctionBase* f);
   bool IsConstant();
   bool IsConstant(const std::map<std::string , double>& valuemap);
   bool IsCompoundStatement() {return false;}
   std::string Print();
   FunctionBase* Simplify(const std::map<std::string , double>& valuemap);
};



/// ------------------------------      Function class     --------------------------------------



/// Function class expression syntax :
// constants : -1 , 2.0 , 3.0e4 
// variables and function names : [_ + a-z + A-Z][_ + a-z + A-Z + 0-9] ( All other symbols prohibited )
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
// absolute value : fabs(exp)
// logarithm base 10 : log10(exp)
// natural logarithm : ln(exp)
/// Binary functions
// add : a + b
// subtract : a - b
// multiply : a * b
// divide : a / b
// modulus : fmod(x , y)
// power : a ^ b or pow(a,b)
// base logarithm : logb(number , base)
// arc tangent 2 : atan2(y,x)

/// Order of evaluation
// constants 0

// variables 1

// grouping ( ) 2

// all function arguments 3

// all function calls 4
// unary functions 4
// base logarithm 4
// arc tangent 2 4
// user functions 4

// power 5 right to left
// multiply 6
// divide 7 a*b/c*d

// add, subtract 8 a + b - c + d


class Function : public FunctionBase {
private :
   std::string exp;
   std::string parsestr;
   FunctionBase* root;
   std::map<std::string , FunctionBase*> terms;
   unsigned int termcount;
   char termbuf[128];
   std::set<std::string> function_set;
   
   void CleanUp();
   
   void ParseCheck();
   void ParseConstants();
   void ParseVariables();
   void ParseGroups();
   void ParseFunctionArguments();
   
   std::string ParseArgumentList(std::string list);// (a,b,c)
   std::vector<FunctionBase*> ParseArgumentListSymbols(std::string list);// ($f0$,$f1$,$f2$)
   void GetLeftAndRightTerms(const char* cstr , int index , int* storestart , int* storeend , FunctionBase** storeleft , FunctionBase** storeright);
   
   void ParseFunctionCalls();
   void ParseExponents();
   void ParseMultiply();
   void ParseDivide();
   void ParseSubtract();
   void ParseAdd();
   void ParseFinal();

   void RemoveTerm(std::string term);

   Function(Function* f);
   

public :
   Function(const std::string& expression);
   ~Function();

   void Free();
   void Parse(const std::string& expression);
   
   std::string PrintExpression() {return exp;}

   SOLVER_RESULT Evaluate(const std::map<std::string , double>& valuemap , double* result);

   FunctionBase* Clone() {return new Function(this);}
   std::vector<FunctionBase*> Terms();
   bool WillCall(FunctionBase* f);
   bool IsConstant() {return root->IsConstant();}
   bool IsConstant(const std::map<std::string , double>& valuemap);
   bool IsCompoundStatement() {return root->IsCompoundStatement();}
   std::string Print() {return root->Print();}
   FunctionBase* Simplify(const std::map<std::string , double>& valuemap);
};







#endif // Functions_H


