
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int SetClipboardString(const char* cstr);
char* GetNewClipboardString();



int main(int argc , char** argv) {

   char* newstr = 0;
   const char* str = "Test String";
   const char* test = str;
   if (argc > 1) {
      test = argv[1];
   }
   if (SetClipboardString(test) != 0) {
      printf("Failed to set clipboard string.\n");
      return 1;
   }

   newstr = GetNewClipboardString();
   if (!newstr) {
      printf("Failed to get new clipboard string.\n");
      return 1;
   }

   printf("Clipboard string is '%s'.\n" , newstr);

   if (strcmp(test , newstr) != 0) {
      printf("Didn't get the same string back!\n");
   } else {
      printf("Success!\n");
   }
   free(newstr);

   return 0;
}


int SetClipboardString(const char* cstr) {

   int count = 0;
   int ret = 0;
   int len = strlen(cstr);
   const char* command = "xclip -selection c";
   FILE* wpipe = popen(command , "w");

   if (!wpipe) {return 1;}

   count = fwrite(cstr , sizeof(char) , len , wpipe);

   ret = pclose(wpipe);

   if ((ret != 0) || (count != len)) {
      return 1;
   }
   return 0;
}


char* GetNewClipboardString() {

   const char* command = "xclip -o -selection c";
   FILE* rpipe = 0;
   char* newstr = 0;
   int size = 64;
   int i = 0;
   char c = '\0';

   rpipe = popen(command , "r");
   if (!rpipe) {return 0;}

   newstr = (char*)malloc(size + 1);
   if (!newstr) {
      pclose(rpipe);
      return 0;
   }
   for (i = 0 ; i < size + 1 ; ++i) {newstr[i] = '\0';}
   i = 0;

   while (1) {
      c = fgetc(rpipe);
      if (c == EOF) {break;}
      newstr[i++] = c;
      newstr[i] = '\0';
      if (i + 1 == size) {
         size *= 2;
         newstr = (char*)realloc(newstr , size + 1);
         if (!newstr) {
            pclose(rpipe);
            return 0;
         }
      }
   }

   pclose(rpipe);

   return newstr;
}

