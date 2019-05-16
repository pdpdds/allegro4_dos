
//#define ALLEGRO_USE_CONSOLE //does nothing to allow cout or stdout by itself
#include "Eagle.hpp"

#include "DataSyncTest.hpp"

#include "InputHandlerTest.hpp"

#include "RecordInputTest.hpp"

#include "GeneralWidgetTest.hpp"

#include "DrawingTest.hpp"

#include "SoundTest.hpp"

#include "AnimationTest.hpp"

using std::endl;


#define NUM_BRANCHES 7

int (*branches[NUM_BRANCHES]) (int,char**) = {
   DataSyncTestMain,
   InputHandlerTestMain,
   RecordInputTestMain,
   GeneralWidgetTestMain,
   DrawingTestMain,
   SoundTestMain,
   AnimationTestMain
};

const char* branch_names[NUM_BRANCHES] = {
   "DataSyncTest",// 0
   "InputHandlerTest",// 1
   "RecordInputTest",// 2
   "GeneralWidgetTest",// 3
   "DrawingTest",// 4
   "SoundTest",// 5
   "AnimationTest"// 6
};

int branch_num = 3;

int main(int argc , char** argv) {
   if (!SendOutputToFile("GuiTest_Log.txt" , "Main Gui Test\n\n" , false)) {return 0;}
   for (int i = 0 ; i < argc ; ++i) {
      OutputLog() << "Argrument #" << i << " '" << argv[i] << "'" << endl;
   }
   
   
   bool show_usage = false;
   for (int i = 1 ; i < argc ; ++i) {
      if (strcmp(argv[i] , "-x") == 0) {
         /// Check against branch names
         if ((i + 1) < argc) {
            const int BUF_SIZE = 32;
            char buf[BUF_SIZE];
            for (int j = 0 ; j < BUF_SIZE ; ++j) {buf[j] = '\0';}
            show_usage = true;
            for (int j = 0 ; j < NUM_BRANCHES ; ++j) {
               sprintf(buf , "%i" , j);
               if ((strcmp(argv[i+1] , branch_names[j]) == 0) || (strcmp(argv[i+1] , buf) == 0)) {
                  branch_num = j;
                  show_usage = false;
                  break;
               }
            }
         } else {
            show_usage = true;
         }
         if (show_usage) {
         }
      }
      if (strcmp(argv[i] , "-h") == 0) {
         show_usage = true;
      }
   }
   if ((argc == 1) || show_usage) {
      OutputLog() << "Usage :" << endl;
      OutputLog() << "The -x switch is followed by a second argument with the name or " << endl;
      OutputLog() << "number of the test program you wish to run." << endl;
      OutputLog() << "Valid program names and numbers are : " << endl;
      Indenter indent(1,3);
      for (int j = 0 ; j < NUM_BRANCHES ; ++j) {
         OutputLog() << indent << j << " - " << branch_names[j] << endl;
      }
      OutputLog() << "Default program is " << branch_num << " - " << branch_names[branch_num] << endl;
      OutputLog() << "Launching default program." << endl;
   }
   
   int ret = branches[branch_num](argc,argv);
   allegro_exit();
   return ret;
//   return branches[branch_num](argc,argv);
}
END_OF_MAIN()





