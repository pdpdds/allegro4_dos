

#ifndef Graphing_H
#define Graphing_H

#include "Eagle.hpp"
#include "Functions.hpp"



typedef SOLVER_RESULT (*EVALFUNC)(const std::map<std::string , double>& , double*);

class FuncAdapter {
private :
   static FunctionBase* f;
public :
   FuncAdapter() {}
   
   void SetFunction(FunctionBase* function) {f = function;}
   
   static SOLVER_RESULT Evaluate(const std::map<std::string , double>& valuemap , double* result) {
      return f->Evaluate(valuemap , result);
   }
};

class Plotter : public WidgetBase {
private :
   double xmin , xmax , xrange;
   double ymin , ymax , yrange;
   double centerx , centery;
   double viewx_to_x_scale;
   double viewy_to_y_scale;
   double x_to_viewx_scale;
   double y_to_viewy_scale;
   
   FunctionBase* f;
   FuncAdapter adapter;
   bool use_adapter;
   EVALFUNC eval;

   std::map<std::string , double> valuemap;
   std::string var;
   
   bool show_axes;
   
   bool ready;

   bool drag;
   int vxstart;
   int vystart;

   void ResetScales();
   bool CheckReady();

public :
   
   Plotter(std::string name , Rectangle position);
   
   void SetView(double x_min , double x_max , double y_min , double y_max);
   
   void SetFunction(EVALFUNC function);
   void SetFunction(FunctionBase* function);
   void SetVariableName(std::string name) {var = name;}
   
   double x_to_viewx(double x);
   double y_to_viewy(double y);
   double viewx_to_x(double viewx);
   double viewy_to_y(double viewy);

   void Plot(BITMAP* bmp , int x , int y);
   
   /// Redefined virtual functions from WidgetBase
   
   WidgetMsg Update(double tsec);
   WidgetMsg CheckInputs(int msx , int msy);
   void DisplayOn(BITMAP* bmp , int x , int y);

};


#endif // #ifndef Graphing_H
