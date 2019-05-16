


#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>

#include <stdlib.h>

#include <stdio.h>

#include <time.h>

#include <stdarg.h>




Display* display = 0;
Window ourwindow = None;
FILE* logfile = 0;
char* input_buffer = 0;

FILE* iofile = 0;
unsigned int iofilesize = 0;

int print_usage = 0;

Time request_time;

// target atoms
Atom StringTargetAtom = None;
Atom TargetsTargetAtom = None;
Atom MultipleTargetAtom = None;
Atom TimestampTargetAtom = None;

Atom BitmapTargetAtom = None;
Atom PngTargetAtom = None;
Atom JpgTargetAtom = None;

// type atoms
Atom StringTypeAtom = None;
Atom AtomTypeAtom = None;
Atom IntegerTypeAtom = None;
Atom IncrTypeAtom = None;

XTextProperty xtext;



//FILE* Log(void);

//void CloseLog(void);

void logprintf(const char* format , ...);

int Close(int ret);

void GetTargetInfo(const char* target_str , Atom* target , Atom* type , int* format);

void QueryAvailableTargets(Atom selection , Atom** store_targets , int* store_num_targets);

void WaitForPaste(Atom selection , Atom target , Atom target_type , int format , const char* data , int data_size);

void QueryTargetAvailable(Atom selection , Atom target);

void RetrieveTarget(Atom selection , Atom target);

char* GetWindowName(Window w);



int main(int argc , char** argv) {
   
   // selection atoms
   Atom ClipboardAtom = None;
   Atom PrimaryAtom = None;
   
   Atom SelectionAtom = None;
   Atom TargetAtom = None;
   Atom TargetTypeAtom = None;
   int Format = 0;
   
   char* subarg = 0;
   int input_buffer_size = 0;
   int actual_buffer_size = 0;
   char c;
   
   int set = 0;
   int query = 0;
   int queryall = 0;
   int get = 0;
   
   pid_t pid = 0;
   
   Atom* store_targets = 0;
   int store_num_targets = 0;
   char* atom_str = 0;
   int i = 0;
//   int j = 0;
   
   
   display = XOpenDisplay(NULL);
   if (!display) {
      logprintf("Could not create a connection to the server.\n");
      printf("Could not create a connection to the server.\n");
      return 1;
   }

   ClipboardAtom = XInternAtom(display , "CLIPBOARD" , True);
   PrimaryAtom = XInternAtom(display , "PRIMARY" , True);
   
   StringTargetAtom = XInternAtom(display , "STRING" , True);
   TargetsTargetAtom = XInternAtom(display , "TARGETS" , True);
   MultipleTargetAtom = XInternAtom(display , "MULTIPLE" , True);
   TimestampTargetAtom = XInternAtom(display , "TIMESTAMP" , True);
   
   BitmapTargetAtom = XInternAtom(display , "image/bmp" , False);
   PngTargetAtom = XInternAtom(display , "image/png" , False);
   JpgTargetAtom = XInternAtom(display , "image/jpeg" , False);

   StringTypeAtom = XInternAtom(display , "STRING" , True);
   AtomTypeAtom = XInternAtom(display , "ATOM" , True);
   IntegerTypeAtom = XInternAtom(display , "INTEGER" , True);
   IncrTypeAtom = XInternAtom(display , "INCR" , True);

   if ((ClipboardAtom == None) ||
       (PrimaryAtom == None) ||
       (StringTargetAtom == None) ||
       (TargetsTargetAtom == None) ||
       (MultipleTargetAtom == None) ||
       (TimestampTargetAtom == None) ||
       (BitmapTargetAtom == None) ||
       (PngTargetAtom == None) ||
       (JpgTargetAtom == None) ||
       (StringTypeAtom == None) ||
       (AtomTypeAtom == None) ||
       (IntegerTypeAtom == None) ||
       (IncrTypeAtom == None)) 
   {
      if (ClipboardAtom == None) {logprintf("CLIPBOARD atom unknown.\n");}
      if (PrimaryAtom == None) {logprintf("PRIMARY atom unknown.\n");}
      if (StringTargetAtom == None) {logprintf("STRING atom unknown.\n");}
      if (TargetsTargetAtom == None) {logprintf("TARGETS atom unknown.\n");}
      if (MultipleTargetAtom == None) {logprintf("MULTIPLE atom unknown.\n");}
      if (TimestampTargetAtom == None) {logprintf("TIMESTAMP atom unknown.\n");}
      if (BitmapTargetAtom == None) {logprintf("image/bmp atom unknown.\n");}
      if (PngTargetAtom == None) {logprintf("image/png atom unknown.\n");}
      if (JpgTargetAtom == None) {logprintf("image/jpeg atom unknown.\n");}
      if (StringTypeAtom == None) {logprintf("STRING atom unknown.\n");}
      if (AtomTypeAtom == None) {logprintf("ATOM atom unknown.\n");}
      if (IntegerTypeAtom == None) {logprintf("INTEGER atom unknown.\n");}
      if (IncrTypeAtom == None) {logprintf("INCR atom unknown.\n");}
      return Close(1);
   }
   
   ourwindow = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0, 1, 1, 0, 0, 0);
   if (ourwindow == None) {
      logprintf("Could not create a window.\n");
      printf("Could not create a window.\n");
      return Close(1);
   }
   
   XSelectInput(display , ourwindow , PropertyChangeMask);

   request_time = CurrentTime;

   // command line options
      // pick selection -p for primary, -c for clipboard
      // set selection text -s=text
      // set selection image -s=[bmp|jpg|png]???
      // query whether selection text exists -q=text
      // query whether selection image exist -q=[bmp|jpg|png]
      // query all targets available for current selection -q=all
      // get selection text -g=text
      // get selection image -g=[bmp|jpg|png]
      // xcsi [-p|-c] [-q=[text|bmp|jpg|png] | -s=[text|bmp|jpg|png] | -g=[text|bmp|jpg|png]]
   
   if (argc < 3) {// selection and action not passed to command line
      print_usage = 1;
   } else {
      if (strcmp(argv[1] , "-p") == 0) {
         SelectionAtom = PrimaryAtom;
      } else if (strcmp(argv[1] , "-c") == 0) {
         SelectionAtom = ClipboardAtom;
      } else {
         printf("'%s' command option not recognized.\n" , argv[1]);
         print_usage = 1;
      }
      if (!print_usage) {
         if ((strlen(argv[2]) >= 6) && (strlen(argv[2]) <= 7)) {// TODO : if options change, these values may change as well
            subarg = argv[2] + 3;
            GetTargetInfo(subarg , &TargetAtom , &TargetTypeAtom , &Format);
            if (TargetAtom == None) {// not supported or unknown
               print_usage = 1;
            } else {
               if (strncmp(argv[2] , "-s=" , 3) == 0) {
                  set = 1;
               } else if (strncmp(argv[2] , "-q=all" , 6) == 0) {
                  queryall = 1;
               } else if (strncmp(argv[2] , "-q=" , 3) == 0) {
                  query = 1;
               } else if (strncmp(argv[2] , "-g=" , 3) == 0) {
                  get = 1;
               } else {
                  printf("'%s' command option not recognized.\n" , argv[2]);
                  print_usage = 1;
               }
            }
            
            if (argc >= 4) {
               if (set) {
                  iofile = fopen(argv[3] , "rb");
                  if (!iofile) {
                     logprintf("Could not open %s for reading.\n" , argv[3]);
                     printf("Could not open %s for reading.\n" , argv[3]);
                     return Close(2);
                  }
               } else if (get) {
                  iofile = fopen(argv[3] , "wb");
                  if (!iofile) {
                     logprintf("Could not open %s for writing.\n" , argv[3]);
                     printf("Could not open %s for writing.\n" , argv[3]);
                     return Close(2);
                  }
                  
               }
            }
         } else {
            print_usage = 1;
         }
      }
   }
   
   if (print_usage) {
      printf("Usage :\n");
      printf("xcsi selection action [iofile]\n");
      printf("selection :\n");
      printf("   -c     : Uses the clipboard selection.\n");
      printf("   -p     : Uses the primary selection.\n");
      printf("action    :\n");
      printf("   -s=target   : Sets the target selection and reads from the iofile if\n");
      printf("               : specified or else from stdin.\n");
      printf("   -q=target   : Queries whether the target selection is available.\n");
      printf("               : Returns \"available\" or \"not available\" to stdout.\n");
      printf("   -g=target   : Gets the target selection and outputs to iofile if\n");
      printf("               : specified or else to stdout.\n");
      printf("target    : May be text , bmp , png , or jpg.\n");
      printf("          : When used with -q=, the target may be all.\n");
      printf("[iofile]  : Optional file name argument used with the set or get action.\n");
      printf("          : Indicates the file to read from (set) or write to (get).\n");
      printf("\n");
      printf("Example :\n");
      printf("xcsi -c -s=bmp my_image.bmp\n");
      printf("\n");
      return Close(1);
   } else {
      if (set) {
         if (iofile) {
            // read the input from the iofile
            int iostart = ftell(iofile);
            int ioend = 0;
            if (fseek(iofile , 0 , SEEK_END) != 0) {
               logprintf("Could not seek to end of input file.\n");
               printf("Could not seek to end of input file.\n");
               return Close(3);
            }
            ioend = ftell(iofile);
            input_buffer_size = ioend - iostart;
            if (fseek(iofile , 0 , SEEK_SET) != 0) {
               logprintf("Could not seek to beginning of input file.\n");
               printf("Could not seek to beginning of input file.\n");
               return Close(3);
            }
            input_buffer = malloc(input_buffer_size);
            if (!input_buffer) {
               logprintf("Could not allocate input buffer.\n");
               printf("Could not allocate input buffer.\n");
               return Close(3);
            }
            for (i = 0 ; i < input_buffer_size ; ++i) {
               input_buffer[i] = fgetc(iofile);
            }
         } else {
            // read the input from stdin
            actual_buffer_size = 256*sizeof(char);
            input_buffer = (char*)malloc(actual_buffer_size);
            if (!input_buffer) {
               printf("Could not allocate buffer to read from stdin.\n");
               return Close(3);
            }
            while((c = getchar()) != EOF) {
               input_buffer[input_buffer_size] = c;
               ++input_buffer_size;
               if (input_buffer_size == actual_buffer_size) {
                  actual_buffer_size*=2;
                  input_buffer = (char*)realloc(input_buffer , actual_buffer_size);
                  if (!input_buffer) {
                     printf("Could not reallocate buffer to read from stdin.\n");
                     return Close(3);
                  }
               }
            }
            actual_buffer_size = input_buffer_size + sizeof(char);// trailing null
            input_buffer = (char*)realloc(input_buffer , actual_buffer_size);
            if (!input_buffer) {
               printf("Could not reallocate buffer to accomodate trailing NULL character.\n");
               return Close(3);
            }
            input_buffer[input_buffer_size] = '\0';
         }
         
         if (input_buffer_size == 0) {
            printf("No input.\n");
            logprintf("xcsi set failed, there was no input.\n");
            return Close(4);
         }
         
         /// Fork() here, so the parent returns and the child waits for paste events
         pid = fork();
         if (pid == -1) {
            printf("Failed to fork.\n");
            return Close(1);
         } else if (pid > 0) {
            // parent
            return 0;// Note, do not call Close() here, as then the display and input_buffer would be freed and
                     // the child process would fail, at least I think that's how fork works. TODO : find out.
         } else if (pid == 0) {
            // child
            WaitForPaste(SelectionAtom , TargetAtom , TargetTypeAtom , Format , input_buffer , input_buffer_size);
         } else {
            printf("Huh? What's up with fork()?\n");
            return Close(1);
         }
      } else if (queryall) {
         atom_str = XGetAtomName(display , SelectionAtom);
         QueryAvailableTargets(SelectionAtom , &store_targets , &store_num_targets);
         printf("%i Available Targets for the %s selection\n" , store_num_targets , atom_str);
         if (atom_str) {XFree(atom_str);}
         for (i = 0 ; i < store_num_targets ; ++i) {
            atom_str = XGetAtomName(display , store_targets[i]);
            printf("%s\n" , atom_str);
            if (atom_str) {
               XFree(atom_str);
               atom_str = 0;
            }
         }
         if (store_targets) {
            XFree(store_targets);
         }
      } else if (query) {
         QueryTargetAvailable(SelectionAtom , TargetAtom);
      } else if (get) {
         RetrieveTarget(SelectionAtom , TargetAtom);
      } else {
         return Close(1);
      }
   }
   return Close(0); 
};



/*
FILE* Log(void) {
   if (logfile) {return logfile;}
   logfile = fopen("xcsi_log.txt" , "a");
   if (logfile) {
      atexit(CloseLog);
   }
   return logfile;
}



void CloseLog(void) {
   if (logfile) {
      fflush(logfile);
      fclose(logfile);
      logfile = 0;
   }
}
*/
void logprintf(const char* format , ...) {
   va_list args;
   
   logfile = fopen("xcsi_log.txt" , "a");
   if (logfile) {
      va_start(args , format);
      vfprintf(logfile , format , args);
      va_end(args);
      fflush(logfile);
      fclose(logfile);
      logfile = 0;
   }
}



int Close(int ret) {
   if (iofile) {
      fclose(iofile);
      iofile = 0;
   }
   if (input_buffer) {
      free(input_buffer);
      input_buffer = 0;
   }
   if (display) {
      XCloseDisplay(display);
      display = 0;
   }
   return ret;
}



void GetTargetInfo(const char* target_str , Atom* target , Atom* type , int* format) {
   Atom ImageTypeAtom = StringTypeAtom;/// TODO : STRING , really? What Type should an image have?
   int ImageFormat = 8;/// TODO : 8 , really? What Format should an image have?
   
   if (!target || !type || !format) {return;}
   *target = None;
   *type = None;
   *format = 0;
   if (strcmp(target_str , "text") == 0) {
      *target = StringTargetAtom;
      *type = StringTypeAtom;
      *format = 8;
   } else if (strcmp(target_str , "all") == 0) {
      *target = TargetsTargetAtom;
      *type = AtomTypeAtom;
      *format = 32;
   } else if (strcmp(target_str , "bmp") == 0) {
      *target = BitmapTargetAtom;
      *type = ImageTypeAtom;
      *format = ImageFormat;
   } else if (strcmp(target_str , "png") == 0) {
      *target = PngTargetAtom;
      *type = ImageTypeAtom;
      *format = ImageFormat;
   } else if (strcmp(target_str , "jpg") == 0) {
      *target = JpgTargetAtom;
      *type = ImageTypeAtom;
      *format = ImageFormat;
   } else {
      printf("Unknown target '%s'.\n" , target_str);
      print_usage = 1;
   }
}


/* Retrieves all of the available TARGETS for the specified selection owner */
void QueryAvailableTargets(Atom selection , Atom** store_targets , int* store_num_targets) {
   Window selowner = None;
   Atom property = None;
   time_t start_time;
   time_t elapsed_time;
   time_t max_wait_time = 5;// will wait up to 5 seconds for the property to show up
   XEvent e;
   XSelectionEvent xse;
   char* str;
   
   Atom store_return_type = None;
   int store_return_format = 0;
   unsigned long store_num_items = 0;
   unsigned long store_bytes_left = 0;
   unsigned char* store_property = 0;
   
   int ret = 0;
   int i = 0;
   
   Atom atom;
   
   char* window_name = 0;
   
   
   
   if (store_targets) {*store_targets = 0;}
   if (store_num_targets) {*store_num_targets = 0;}
   
   selowner = XGetSelectionOwner(display , selection);
   if (selowner == None) {
      logprintf("QueryAvailableTargets info : No selection owner.\n");
      return;
   }
   /// TODO : Perhaps it might be useful to know all of the properties a selection owner window has.
   ///      : XListProperties would do this for us.
   /// TODO : See about finding the name of the application that is the selection owner -
   /// From Cut_and_Paste.pdf :
   /// selectinfo displays the id of the window that owns the selection and the name of the 
   /// application (determined by traversing up the window hierarchy until it finds a window with
   /// the WM_CLASS property set).
   
   window_name = GetWindowName(selowner);
   logprintf("QueryAvailableTargets info : selection owner window's name : %s\n" , window_name);
   if (window_name) {free(window_name);}

   property = XInternAtom(display , "CUT_BUFFER0" , True);
   if (property == None) {
      logprintf("QueryAvailableTargets failed : The atom for \"CUT_BUFFER0\" is not defined.\n");
      return;
   }
   
   // clear our window property
   XDeleteProperty(display , ourwindow , property);

   // request the selection
   XConvertSelection(display , selection , TargetsTargetAtom , property , ourwindow , request_time);
   XFlush(display);
   
   // wait for a SelectionNotify event
   start_time = time(NULL);
   
   while(1) {
      elapsed_time = time(NULL) - start_time;
      if (elapsed_time > max_wait_time) {
         logprintf("QueryAvailableTargets failed : Selection Notify event not received before\n");
         logprintf("                             : %li second timeout.\n" , max_wait_time);
         return;
      }
      XNextEvent(display , &e);
      if (e.type == SelectionNotify) {
         xse = e.xselection;
         if (xse.property == None) {
            // conversion failed
            logprintf("QueryAvailableTargets failed : Selection Notify event property set to None. Conversion failed.\n");
            return;
         } else if (xse.property == property) {
            // The selection owner stored the result of our request on our specified property
            break;
         } else {
            // The selection owner stored the result of our request on a different property!!!
            str = XGetAtomName(display , xse.property);
            logprintf("QueryAvailableTargets warning : The selection owner stored the result of our request\n");
            logprintf("                              : on a different property!!! (%s)\n" , str);
            if (str) {
               XFree(str);
               str = 0;
            }
            property = xse.property;
            break;
         }
      }
   }

   // read the property off of our own window
   // Find out how much data is there
   ret = XGetWindowProperty(display , ourwindow , property , 0 , 0 , False , AtomTypeAtom ,
                            &store_return_type , &store_return_format , &store_num_items , &store_bytes_left , &store_property);
   if (ret != Success) {
      logprintf("QueryAvailableTargets failed : Could not retrieve TARGETS property attempt #1.\n");
      return;
   }
   if (store_bytes_left <= 0) {
      logprintf("QueryAvailableTargets failed : No data to be read from the TARGETS property.\n");
      return;
   }
   // Read all the data at once
   ret = XGetWindowProperty(display , ourwindow , property , 0 , store_bytes_left , False , AtomTypeAtom ,
                            &store_return_type , &store_return_format , &store_num_items , &store_bytes_left , &store_property);
   if (ret != Success) {
      logprintf("QueryAvailableTargets failed : Could not retrieve TARGETS property attempt #2.\n");
      return;
   }
   
   logprintf("QueryAvailableTargets : %li targets available :\n" , store_num_items);
   for (i = 0 ; i < store_num_items ; ++i) {
      atom = ((Atom*)store_property)[i];
      str = XGetAtomName(display , atom);
      logprintf("   %s\n" , (str?str:"unknown atom"));
      if (str) {XFree(str);}
   }
   
   if (store_targets) {
      *store_targets = (Atom*)store_property;
   } else {
      if (store_property) {XFree(store_property);}
   }
   if (store_num_targets) {
      *store_num_targets = store_num_items;
   }
   return;
}


/// TODO : Log timestamp of request
void WaitForPaste(Atom selection , Atom target , Atom target_type , int format , const char* data , int data_size) {
   XEvent e , respond;
   XSelectionEvent xse_respond;
   XSelectionRequestEvent xsre;
   Atom requested_target = None;
   Atom target_property = None;
   Atom supported_targets[4] = {TargetsTargetAtom , TimestampTargetAtom , MultipleTargetAtom , target};
//   Atom return_property = None;
   Window requestor = None;
   
   Atom store_return_type = None;
   int store_return_format = 0;
   unsigned long store_num_items = 0;
   unsigned long store_bytes_left = 0;
   unsigned char* store_property = 0;
   
   int i = 0;
   
   char* window_name = 0;
   char* target_name = 0;
   
   
   if (!data || (data_size < 1)) {return;}
   
   /// Must support the following targets - TARGETS , TIMESTAMP , and MULTIPLE
   
   // Assert ownership of the specified selection
   XSetSelectionOwner(display , selection , ourwindow , request_time);
   XFlush(display);
   
   // Loop to serve selection requests until SelectionClear event is received
   while(1) {
      XNextEvent(display , &e);
//      if (e.type == CloseProgram) {break;}// Is there a close program event in X11? TODO : Find out.
      if (e.type == SelectionClear) {break;}
      if (e.type == SelectionRequest) {
         xsre = e.xselectionrequest;
         // Just for fun
         // XCloseDisplay(xsre.display);// I just shut down your connection to the server!
         requested_target = xsre.target;
         requestor = xsre.requestor;
         
         // Log the requestor and target
         target_name = XGetAtomName(display , requested_target);
         window_name = GetWindowName(requestor);
         logprintf("Selection Request received from %s - requested target : %s\n" , window_name , target_name);
         if (window_name) {free(window_name);}
         if (target_name) {XFree(target_name);}
         
         /// Generate response event
//   "The requestor, selection, time, and target arguments will be the same as those on the ConvertSelection request."
         xse_respond.type = SelectionNotify;
         //xse_respond.serial = ???;
         xse_respond.send_event = True;
         xse_respond.display = display;// Our display? Set by the server? ???
///         xse_respond.requestor = ourwindow;// I guess
         xse_respond.requestor = xsre.requestor;
         xse_respond.selection = selection;
         xse_respond.target = xsre.target;
         xse_respond.property = xsre.property;
         xse_respond.time = xsre.time;
         
         respond.type = SelectionNotify;
         respond.xselection = xse_respond;


         if (requested_target == TargetsTargetAtom) {
            XChangeProperty(display , requestor , xsre.property , AtomTypeAtom , 32 ,
                            PropModeReplace , (unsigned char*)supported_targets , 4);
         } else if (requested_target == TimestampTargetAtom) {
            XChangeProperty(display , requestor , xsre.property , IntegerTypeAtom , 32 ,
                            PropModeReplace , (unsigned char*)(&request_time) , 1);
         } else if (requested_target == MultipleTargetAtom) {
/*
 *  MULTIPLE - The MULTIPLE target atom is valid only when a property is specified on the ConvertSelection request.
    If the property argument in the SelectionRequest event is None and the target is MULTIPLE, it should be refused.

   When a selection owner receives a SelectionRequest (target==MULTIPLE) request, the contents of the property named
   in the request will be a list of atom pairs: the first atom naming a target and the second naming a property 
   ( None is not valid here). The effect should be as if the owner had received a sequence of SelectionRequest events
    (one for each atom pair) except that:
       o The owner should reply with a SelectionNotify only when all the requested conversions have been performed.
       o If the owner fails to convert the target named by an atom in the MULTIPLE property, it should replace that
         atom in the property with None . 
*/
            // first, check the property named in the MULTIPLE request
            if (xsre.property == None) {
               /// The property containing the target\property atom pairs was not specified, refuse the request
               xse_respond.property = None;
               respond.xselection = xse_respond;
            } else {
               /// Read the property to find out the target\property atom pairs
               XGetWindowProperty(display , xsre.requestor , xsre.property , 0 , 0 , False , AnyPropertyType,
                                  &store_return_type , &store_return_format , &store_num_items , &store_bytes_left , &store_property);
               XGetWindowProperty(display , xsre.requestor , xsre.property , 0 , store_bytes_left , False , AnyPropertyType,
                                  &store_return_type , &store_return_format , &store_num_items , &store_bytes_left , &store_property);
               for (i = 0 ; i < store_num_items/2 ; i+=2) {
                  requested_target = ((Atom*)store_property)[i];
                  target_property = ((Atom*)store_property)[i+1];
                  if (requested_target == TargetsTargetAtom) {
                     XChangeProperty(display , requestor , target_property , AtomTypeAtom , 32 ,
                                     PropModeReplace , (unsigned char*)supported_targets , 4);
                  } else if (requested_target == TimestampTargetAtom) {
                     XChangeProperty(display , requestor , target_property , IntegerTypeAtom , 32 ,
                                     PropModeReplace , (unsigned char*)(&request_time) , 1);
                  } else if (requested_target == target) {
                     XChangeProperty(display , requestor , target_property , target_type , format ,
                                     PropModeReplace , (unsigned char*)data , data_size);
                  } else {
                     ((Atom*)store_property)[i+1] = None;
                     XChangeProperty(display , requestor , xsre.property , AtomTypeAtom , 32 ,
                                     PropModeReplace , (unsigned char*)store_property , store_num_items);
                  }
               }
               if (store_property) {
                  XFree(store_property);
                  store_property = 0;
               }
            }
         } else if (requested_target == target) {
            XChangeProperty(display , requestor , xsre.property , target_type , format ,
                            PropModeReplace , (unsigned char*)data , data_size);
         } else {
            // We do not support this target
            xse_respond.property = None;
            respond.xselection = xse_respond;
         }
         XSendEvent(display , requestor , False , NoEventMask , &respond);
         XFlush(display);
      }
   }
}



void QueryTargetAvailable(Atom selection , Atom target) {
   Atom* targets = 0;
   int num_targets = 0;
   const char* output = "not available";
   int i = 0;
   
   QueryAvailableTargets(selection , &targets , &num_targets);
   
   if (targets && num_targets) {
      for (i = 0 ; i < num_targets ; ++i) {
         if (target == targets[i]) {
            output = "available";
         }
      }
   }
   if (targets) {XFree(targets);}
   printf("%s" , output);
}



void RetrieveTarget(Atom selection , Atom target) {
   Atom* targets = 0;
   int num_targets = 0;
   int num_bytes = 0;
   int i = 0;
   int target_available = 0;
   Window selowner = None;
   const char* property_name = "CUT_BUFFER0";
   Atom property = None;
   
   Atom store_return_type = None;
   int store_return_format = 0;
   unsigned long store_num_items = 0;
   unsigned long store_bytes_left = 0;
   unsigned char* store_property = 0;
   
   unsigned char* stored_data = 0;
   unsigned int num_bytes_stored_data = 0;
   unsigned int data_start = 0;
   
   unsigned char* data_to_write = 0;
   
   time_t start_time;
   time_t elapsed_time;
   time_t max_wait_time = 5;
   
   XEvent e;
   XSelectionEvent xse;
   XPropertyEvent xpe;
   
   char* str = 0;
   char* str2 = 0;
   char* str3 = 0;
   
   
   
   QueryAvailableTargets(selection , &targets , &num_targets);
   
   if (targets && num_targets) {
      for (i = 0 ; i < num_targets ; ++i) {
         if (target == targets[i]) {
            target_available = 1;
         }
      }
   }
   if (targets) {
      XFree(targets);
      targets = 0;
      num_targets = 0;
   }
   if (!target_available) {
      logprintf("RetrieveTarget warning : QueryAvailableTargets says this target is unavailable. Proceeding anyway.\n");
   }
   selowner = XGetSelectionOwner(display , selection);
   if (selowner == None) {
      logprintf("RetrieveTarget failure : No selection owner.\n");
   } else {
      property = XInternAtom(display , property_name , True);
      if (property == None) {
         logprintf("RetrieveTarget failure : %s atom is undefined.\n" , property_name);
         return;
      }
      str = XGetAtomName(display , selection);
      str2 = XGetAtomName(display , target);
      str3 = GetWindowName(selowner);
      logprintf("RetrieveTarget info : Retrieving selection %s with target %s from window %s\n" , str , str2 , str3);
      if (str) {XFree(str);str = 0;}
      if (str2) {XFree(str2);str2 = 0;}
      if (str3) {free(str3);str3 = 0;}
      
      XConvertSelection(display , selection , target , property , ourwindow , request_time);
      XFlush(display);
      // wait up to 5 seconds for a SelectionNotify event
      start_time = time(NULL);
      while(1) {
         elapsed_time = time(NULL) - start_time;
         if (elapsed_time > max_wait_time) {
            logprintf("RetrieveTarget warning : timeout exceeded. Checking property anyway.\n");
            break;
         }
         XNextEvent(display , &e);
         if (e.type == SelectionNotify) {
            xse = e.xselection;
            if (xse.property == None) {
               logprintf("RetrieveTarget failed : Selection Notify event property was set to None. Conversion failed.\n");
               return;
            } else if (xse.property == property) {
               // The selection owner stored the result of our request on our specified property
               break;
            } else {
               // The selection owner stored the result of our request on a different property!!!
               str = XGetAtomName(display , xse.property);
               logprintf("RetrieveTarget warning : The selection owner stored the result of our request\n");
               logprintf("                       : on a different property!!! (%s)\n" , str);
               if (str) {
                  XFree(str);
                  str = 0;
               }
               property = xse.property;
               break;
            }
         }
      }
      

      XGetWindowProperty(display , ourwindow , property , 0 , 0 , False , AnyPropertyType ,
                         &store_return_type , &store_return_format , &store_num_items , &store_bytes_left , &store_property);
      num_bytes = store_bytes_left;
      XGetWindowProperty(display , ourwindow , property , 0 , store_bytes_left , False , AnyPropertyType ,
                         &store_return_type , &store_return_format , &store_num_items , &store_bytes_left , &store_property);

      if (store_return_type == IncrTypeAtom) {
         /// TODO : Add support for INCR
         logprintf("Type of property retrieved is INCR. Starting INCR transfer.\n");
/*
2.7.2. INCR Properties
Requestors may receive a property of type INCR in response to any target that results in selection data.
This indicates that the owner will send the actual data incrementally. The contents of the INCR property will
be an integer, which represents a lower bound on the number of bytes of data in the selection. The requestor
and the selection owner transfer the data in the selection in the following manner.

The selection requestor starts the transfer process by deleting the (type==INCR) property forming the reply to the selection.

The selection owner then:

    * Appends the data in suitable-size chunks to the same property on the same window as the selection
      reply with a type corresponding to the actual type of the converted selection. The size should be
      less than the maximum-request-size in the connection handshake.
    * Waits between each append for a PropertyNotify (state==Deleted) event that shows that the requestor
      has read the data. The reason for doing this is to limit the consumption of space in the server.
    * Waits (after the entire data has been transferred to the server) until a PropertyNotify (state==Deleted)
      event that shows that the data has been read by the requestor and then writes zero-length data to the property. 

The selection requestor:

    * Waits for the SelectionNotify event.
    * Loops:
          o Retrieving data using GetProperty with the delete argument True .
          o Waiting for a PropertyNotify with the state argument NewValue . 
    * Waits until the property named by the PropertyNotify event is zero-length.
    * Deletes the zero-length property. 

The type of the converted selection is the type of the first partial property.
The remaining partial properties must have the same type. 
*/
         if (store_property) {XFree(store_property);}
         
         XDeleteProperty(display , ourwindow , property);
         /// Do we need to send an event to the selowner window here?
         
         start_time = time(NULL);
         while (1) {
            elapsed_time = time(NULL) - start_time;
            if (elapsed_time > max_wait_time) {
               logprintf("RetrieveTarget error - elapsed time exceeded timeout (%i).\n" , max_wait_time);
               break;
            }
            
            XNextEvent(display , &e);
            if (e.type == PropertyNotify) {
               xpe = e.xproperty;
/*

typedef struct {
	int type;		// PropertyNotify
	unsigned long serial;	// # of last request processed by server
	Bool send_event;	// true if this came from a SendEvent request
	Display *display;	// Display the event was read from
	Window window;
	Atom atom;
	Time time;
	int state;		// PropertyNewValue or PropertyDelete
} XPropertyEvent;
*/
               str = XGetAtomName(display , xpe.atom);
               str2 = GetWindowName(xpe.window);
               logprintf("PropertyNotify event received : send_event(%s) , window(%s) , atom(%s) , state(%s).\n",
                         ((xpe.send_event)?"True":"False") , str2 , str , 
                         ((xpe.state == PropertyNewValue)?"PropertyNewValue":((xpe.state == PropertyDelete)?"PropertyDelete":"Unknown")));
               if (str) {XFree(str);str = 0;}
               if (str2) {free(str2);str2 = 0;}
               
               if ((xpe.window == ourwindow) && (xpe.atom == property) && (xpe.state == PropertyNewValue)) {
                  XGetWindowProperty(display , ourwindow , property , 0 , 0 , False , AnyPropertyType ,
                                     &store_return_type , &store_return_format , &store_num_items , &store_bytes_left , &store_property);
                  num_bytes = store_bytes_left;
                  if (num_bytes == 0) {
                     logprintf("Received zero length property.\n");
                     XDeleteProperty(display , ourwindow , property);
                     break;
                  }
                  XGetWindowProperty(display , ourwindow , property , 0 , store_bytes_left , True , AnyPropertyType ,
                                     &store_return_type , &store_return_format , &store_num_items , &store_bytes_left , &store_property);
                  
                  num_bytes_stored_data += num_bytes;
                  /// TODO : Can realloc re allocate a NULL pointer?
                  stored_data = realloc(stored_data , num_bytes_stored_data);
                  if (!stored_data) {
                     logprintf("Failed to reallocate stored_data.\n");
                     break;
                  } else {
                     // append 
                     for (i = 0 ; i < num_bytes ; ++i) {
                        stored_data[data_start] = store_property[i];
                        ++data_start;
                     }
                  }
                  XFree(store_property);
                  store_property = 0;
               }
            }
            
         }
         data_to_write = stored_data;
      } else {
         data_to_write = store_property;
         num_bytes_stored_data = num_bytes;
      }

      if (data_to_write) {
         if (iofile) {
            // write to iofile
            for (i = 0 ; i < num_bytes_stored_data ; ++i) {
               putc(data_to_write[i] , iofile);
            }
            fflush(iofile);
         } else {
            // write to stdout
            for (i = 0 ; i < num_bytes_stored_data ; ++i) {
               putchar(data_to_write[i]);
            }
            fflush(stdout);
         }
      }
      if (stored_data) {
         free(stored_data);
      }
      if (store_property) {
         XFree(store_property);
      }
   }
}



char* GetWindowName(Window w) {
   int num_bytes = 0;
   char* str = 0;
   
   /// TODO : Maybe use XTextPropertyToStringList??? The text_prop type has to be STRING and format has to be 8
   ///      : for this to work though...
/*
Status XGetWMName(display, w, text_prop_return)
      Display *display;
      Window w;
      XTextProperty *text_prop_return;
*/
/*
Status XTextPropertyToStringList(text_prop, list_return, \
count_return)
       XTextProperty *text_prop;
       char ***list_return;
       int *count_return;

*/

   /// TODO : Probably doing this wrong, and may be leaking memory through the XTextProperty xtext
   XGetWMName(display , w , &xtext);
   
   num_bytes = xtext.nitems*(xtext.format/8)*sizeof(char);
   str = (char*)malloc(num_bytes + sizeof(char));
   memcpy(str , (void*)(xtext.value) , num_bytes);
   str[num_bytes] = '\0';
   return str;
}




