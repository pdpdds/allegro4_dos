

#include "DataSyncTest.hpp"

//#include "Eagle\DataSync.hpp"

#include <cstdio>
#include <iostream>
using std::cout;
using std::endl;

class TestClass {
   
};

int DataSyncTestMain(int argc , char** argv) {
   
/** Test two for built in types - Fails on <T> Op <T>, and <BuiltInType> Op <T>
   int i = 1;
   DataSync<int> sync_i(&i);
   
   int test = 1 + sync_i;
   int test2 = sync_i + 2;
   int test3 = sync_i + sync_i;
   printf("(i,sync_i,test,test2,test3) = {%i , %i , %i , %i , %i}\n" , i , (int)sync_i , test , test2 , test3);
//*/

/** Test one for built in types 
   short s = 1;
   int i = 2;
   float f = 3.0f;
   double d = 4.0;
   
   DataSync<short>  sync_s(&s);
   DataSync<int>    sync_i(&i);
   DataSync<float>  sync_f(&f);
   DataSync<double> sync_d(&d);
   
   
   DataSynchronizer sync_master;
   DataSynchronizer sync_int;
   DataSynchronizer sync_flop;
   
   sync_int.push_back(&sync_s);
   sync_int.push_back(&sync_i);
   
   sync_flop.push_back(&sync_f);
   sync_flop.push_back(&sync_d);
   
   sync_master.push_back(&sync_int);
   sync_master.push_back(&sync_flop);
   
   printf("\nOutputting Initial Data Values...\n\n");
   printf("    (s,i,f,d) = {%hi , %i , %f , %lf}\n" , s,i,f,d);
   printf("Sync(s,i,f,d) = {%hi , %i , %f , %lf}\n" , (short)sync_s , (int)sync_i , (float)sync_f , (double)sync_d);
   
   s = 5;
   i = 6;
   f = 7.0f;
   d = 8.0;
   
   sync_master.RefreshFromSource();
   printf("\nRefreshing Data...\n\n");
   printf("    (s,i,f,d) = {%hi , %i , %f , %lf}\n" , s,i,f,d);
   printf("Sync(s,i,f,d) = {%hi , %i , %f , %lf}\n" , (short)sync_s , (int)sync_i , (float)sync_f , (double)sync_d);
   
   
   printf("\nAssigning New Data Directly To DataSyncs...\n\n");
   sync_s = 9;
   sync_i = 10;
   sync_f = 11.0f;
   sync_d = 12.0;
   printf("    (s,i,f,d) = {%hi , %i , %f , %lf}\n" , s,i,f,d);
   printf("Sync(s,i,f,d) = {%hi , %i , %f , %lf}\n" , (short)sync_s , (int)sync_i , (float)sync_f , (double)sync_d);
   
   printf("\nTesting Assignment From DataSyncs To Built In Variable Types...\n\n");
   sync_s = 13;
   sync_i = 14;
   sync_f = 15.0f;
   sync_d = 16.0;
   s = sync_s;
   i = sync_i;
   f = sync_f;
   d = sync_d;
   printf("    (s,i,f,d) = {%hi , %i , %f , %lf}\n" , s,i,f,d);
   printf("Sync(s,i,f,d) = {%hi , %i , %f , %lf}\n" , (short)sync_s , (int)sync_i , (float)sync_f , (double)sync_d);

   printf("\nTesting Second Assignment From DataSyncs To Built In Variable Types...\n\n");
   short s2  = sync_s;
   int i2    = sync_i;
   float f2  = sync_f;
   double d2 = sync_d;
   sync_s = 17;
   sync_i = 18;
   sync_f = 19.0f;
   sync_d = 20.0;
   printf("    (s2,i2,f2,d2) = {%hi , %i , %f , %lf}\n" , s2,i2,f2,d2);
   printf("    (s ,i ,f ,d ) = {%hi , %i , %f , %lf}\n" , s,i,f,d);
   printf("Sync(s ,i ,f ,d ) = {%hi , %i , %f , %lf}\n" , (short)sync_s , (int)sync_i , (float)sync_f , (double)sync_d);
//*/

   return 0;
}

