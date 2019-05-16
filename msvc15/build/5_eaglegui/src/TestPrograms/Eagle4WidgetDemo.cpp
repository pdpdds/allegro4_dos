


/**
   Official Eagle 4 demo
*/

#include "Eagle.hpp"

#include <string>
using std::string;
using std::endl;



int main(int argc , char** argv) {
   
   
   /// This block is a cheat so our destructors run before allegro's does
   {
   /** ----------------    General Setup   ------------------------- */
   
//   raise(SIGABRT);//ends program , 'Program exited with code 03'
//   raise(SIGILL);//ends program , 'Program exited with code 03'
//   raise(SIGTERM);//ends program , 'Program exited with code 03'
//   raise(SIGSEGV);//ends program , 'Program exited with code 03'
//   raise(SIGFPE);//ends program , 'Program exited with code 03'
//   raise(SIGINT);//ends program , 'Program exited with code 03'
//   raise(SIGTRAP);SIGTRAP is unknown to C, I think it's from Windows somewhere
//   abort();// ends program , 'Invalid parameter passed to C runtime function X2, Program exited with code 03'
//   assert(0);// Windows catches assertion and gives warning the assertion failed
///   int a = 1/0;/// gdb successfully catches SIGFPE and a backtrace of the stack can be performed

   if (!SendOutputToFile("GeneralWidgetTestMain_Log.txt" , "General Widget Test\n\n" , false)) {return 0;}
   
   if (!GetAllegroReady(true , true , true)) {return 0;}
   if (!DesktopInfoKnown()) {return 0;}
   SetupInputHandler();
   
   const int dw = 800;
   const int dh = 600;
   GfxModeData gfxmode(GFX_AUTODETECT_WINDOWED , dw , dh , DCD());

   gfx_mode_handler.Setup(SW_BACKGROUND , 0 , 0);
   int ret = gfx_mode_handler.ChangeToMode(gfxmode , CRM_DIRECT);
   if (ret != FULL_SUCCESS) {return 0;}

//   if (!ChangeRes(gfxmode , OutputLog() , 1)) {return 0;}
   LogGfxAbilitiesTo(OutputLog());
   OutputLog() << endl;
   
   
///   set_window_title("General Widget Test");

   
   SetAllegroPointerUse(true);
   
   MousePointerSetup pointer_setup;
   if (!pointer_setup.LoadPointersFromScript("Data/Config/DefaultPointers.txt")) {
      OutputLog() << "Loading pointers failed..." << endl;
   }
   int mouse_owner_id = AcquireMouseOwnerId();


   BitmapHandler buffer(create_bitmap(dw , dh) , SOLID);

   clear_to_color(screen , makecol(64,92,64));
   
   /** Test */
/*
   int j = 0;
   OutputLog() << "Drivers available :" << endl;
   while(_gfx_driver_list[j].driver) {
      GFX_DRIVER* gd = (GFX_DRIVER*)_gfx_driver_list[j].driver;
      OutputLog() << GraphicsCardName(gd->id) << endl;
      ++j;
   }
   OutputLog() << endl;
*/

   /** --------------   GUI setup    ----------------------------- */

   Input input_quit(input_key_press(KEY_ESC));
   
   BitmapHandler bg_pic;
   if (!bg_pic.LoadImage("Data/Images/k9_05.bmp" , NULL , MEMORY , SOLID)) {
      OutputLog() << "Failed to load k9_05.bmp" << endl;
      return 0;
   }


//   WidgetHandler gui("gui" , screen , Rectangle(0,0,dw,dh));
   WidgetHandler gui("gui" , font , "Main GUI" , false , false , screen , Rectangle(0,0,dw,dh));
   gui.SetTitleBarVisibility(false);
   gui.SetVirtualDimensions(2*dw , 2*dh);
   gui.AllowMiddleMouseButtonDrag(true);
   if (!gui.SetupBitmapHandlers(BG_SOLID , MEMORY , SOLID)) {
      OutputLog() << "gui.SetupBitmapHandlers failed." << endl;
      return 0;
   }
   gui.SetBackgroundImage(bg_pic);

   WidgetColorset wcolors = DefaultWidgetColors();
//   wcolors.Tint(127,0,255,127);
   wcolors.SetRGBAex(TXTCOL , 255 , 255 , 255 , 254 , 32);
   gui.SetWidgetColors(wcolors);
   
   

   WidgetHandler btn_panel("btn_panel" , font , "Button Panel" , false , true , NULL , Rectangle(dw/2,dh/2,dw/2,dh/2));// (400,300 , 400 X 300)
//   btn_panel.SetVirtualDimensions(dw,dh);
   if (!btn_panel.SetupBitmapHandlers(BG_ROUNDED , MEMORY , TRANSPARENT)) {
//   if (!btn_panel.SetupBitmapHandlers(BG_ROUNDED , MEMORY , TRANSLUCENT)) {
      OutputLog() << "btn_panel.SetupBitmapHandlers failed." << endl;
      return 0;
   }

   gui.AddWidget(&btn_panel);
   
//   BitmapHandler(const char* filepath , RGB* pal , BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type , ResourceRegistry* reg = &resource_registry);
   BitmapHandler upimage("Data/Images/Up.bmp" , NULL , MEMORY , TRANSPARENT , true , 0);
   BitmapHandler downimage("Data/Images/Down.bmp" , NULL , MEMORY , TRANSPARENT , true , 0);
   BitmapHandler disabledimage("Data/Images/Disabled.bmp" , NULL , MEMORY , TRANSPARENT , true , 0);
   BMP_DRAW_TYPE icon_draw_type = TRANSPARENT;
   if (btn_panel.GetDrawType() == TRANSLUCENT) {
      convert_to_alpha(upimage);
      convert_to_alpha(downimage);
      convert_to_alpha(disabledimage);
      icon_draw_type = TRANSLUCENT;
   }
//   IconButton(std::string name , ICON_DISPLAY_TYPE dtype , BUTTON_TYPE btype , BTN_ACTION_TYPE atype ,
//               const InputGroup& input , const Rectangle& position , UINT wflags = DEFAULT_FLAGS);
   IconButton icon1("icon1" , ICON_DISPLAY_CENTER , RECTANGLE_BTN , SPRING_BTN , input_key_press(KEY_NONE) , Rectangle(200,25,100,100));
//   IconButton icon1("icon1" , RECTANGLE_BTN , SPRING_BTN , input_key_press(KEY_A) , Rectangle(200 , 25 , 100 , 100));
   
   if (!icon1.SetImages(upimage , downimage , 0 , disabledimage , MEMORY , icon_draw_type)) {
      OutputLog() << "Loading images for the icon button failed." << endl;
   }

   if (btn_panel.GetDrawType() == TRANSLUCENT) {
      icon1.SetBlenderType(USE_ALPHA_LAYERING_BLENDER);
   }

   TextWidget tdw1("tdw1" , "TopLeft" , font , TEXT_LEFT   , TEXT_TOP     , 50 , 150);
   TextWidget tdw2("tdw2" , "Centered" , font , TEXT_CENTER , TEXT_VCENTER , 125 , 175);
   TextWidget tdw3("tdw3" , "BottomRight" , font , TEXT_RIGHT  , TEXT_BOTTOM  , 200 , 200);
   

   Button spr1("spr1" , RECTANGLE_BTN , SPRING_BTN , font , "&Rect" , input_key_press(KEY_R) , Rectangle(12 , 200 , 75 , 25));
   Button spr2("spr2" , CIRCLE_BTN    , SPRING_BTN , font , "&Circ"    , input_key_press(KEY_C) , Rectangle(112 , 200 , 75 , 25));
   Button spr3("spr3" , ROUNDED_BTN   , TOGGLE_BTN , font , "R&ound"  , input_key_press(KEY_O) , Rectangle(212 , 200 , 75 , 25));
   Button spr4("spr4" , ELLIPSE_BTN   , TOGGLE_BTN , font , "Ell&p"   , input_key_press(KEY_P) , Rectangle(312 , 200 , 75 , 25));

   Button quit_btn("quit_btn" , ROUNDED_BTN , SPRING_BTN , font , "&Quit" , input_key_press(KEY_Q) , Rectangle(150 , 250 , 100 , 50) , DEFAULT_FLAGS | ALLOW_CLOSE);


   WidgetColorset btn_panel_colors = DefaultWidgetColors();
   btn_panel_colors.SetAlpha((UCHAR)((float)255*1.0f));
//   Slider<int> slider1(-180,0,180,1,true,Rectangle(340 , 25 , 20 , 108));
   Slider<float> slider1("slider1" , 0.0 , 0.0 , 1.0 , 0.01 , true , Rectangle(340,25,20,108));

   Button adjust_btn("adjust_btn" , RECTANGLE_BTN , TOGGLE_BTN , font , "&Adjust" , input_key_press(KEY_A) , Rectangle(12 , 20 , 100 , 25));

   btn_panel << &icon1 << &tdw1 << &tdw2 << &tdw3 << &spr1 << &spr2 << &spr3 << &spr4 << &quit_btn << &slider1 << &adjust_btn;


   BITMAP* checkmark = load_bitmap("Data/Images/MenuIconCheckmark20x20.bmp" , NULL);
   MenuItem menuitem1("menuitem1" , checkmark , NULL , TRANSPARENT , true,
                      input_key_press(KEY_1) , font , "MenuItem &1" , "The First Menu Item" ,
                      NULL , 5 , true,
                      Rectangle(25,25,150,30));
   MenuItem menuitem2("menuitem2" , checkmark , NULL , TRANSPARENT , true,
                      input_key_press(KEY_2) , font , "MenuItem &2" , "The Second Menu Item" ,
                      NULL , 5 , true,
                      Rectangle(25,25,150,30));
   MenuItem menuitem3("menuitem3" , checkmark , NULL , TRANSPARENT , true,
                      input_key_press(KEY_3) , font , "MenuItem &3" , "The Third Menu Item" ,
                      NULL , 5 , true,
                      Rectangle(25,25,150,30));
   destroy_bitmap(checkmark);
   
   Menu menu1("menu1" , Rectangle(25,175,150,90));
   menu1.AddItem(&menuitem1);
   menu1.AddItem(&menuitem2);
   menu1.AddItem(&menuitem3 , 1);


   MenuItem menuitem4("menuitem4" , input_key_press(KEY_4) , font , "MenuItem &4" , "Item # Four" , NULL , Rectangle());
   MenuItem menuitem5("menuitem5" , input_key_press(KEY_5) , font , "MenuItem &5" , "Item # Five" , NULL , Rectangle());
   MenuItem menuitem6("menuitem6" , input_key_press(KEY_6) , font , "MenuItem &6" , "Item # Six" , NULL , Rectangle());// ,
//                     VISIBLE | MOVEABLE | RESIZEABLE | ALLOW_OVERLAP);
   menuitem6.SetEnabledState(false);
   
   Menu menu2("menu2" , Rectangle(25,25,150,90));
   menu2 << &menuitem4 << &menuitem5 << &menuitem6;

   Menu menu4("menu4" , Rectangle(25,25,150,90));
   MenuItem menuitem10("menuitem10" , input_key_press(KEY_NONE) , font , "Menu Item 10" , "Item # Ten" , NULL , Rectangle());
   MenuItem menuitem11("menuitem11" , input_key_press(KEY_NONE) , font , "Menu Item 11" , "Item # Eleven" , NULL , Rectangle());
   MenuItem menuitem12("menuitem12" , input_key_press(KEY_NONE) , font , "Menu Item 12" , "Item # Twelve" , NULL , Rectangle());
   
   menu4 << &menuitem10 << &menuitem11 << &menuitem12;

   Menu menu3("menu3" , Rectangle(25,25,150,90));
   
   MenuItem menuitem7("menuitem7" , input_key_press(KEY_7) , font , "MenuItem &7" , "Item # Seven" , NULL , Rectangle());
   MenuItem menuitem8("menuitem8" , input_key_press(KEY_8) , font , "MenuItem &8" , "Item # Eight" , NULL , Rectangle());
   MenuItem menuitem9("menuitem9" , input_key_press(KEY_9) , font , "MenuItem &9" , "Item # Nine" , &menu4 , Rectangle());

   menu3 << &menuitem7 << &menuitem8 << &menuitem9;

   MenuBarItem menubaritem1("menubaritem1" , font , "M&enu One" , input_key_press(KEY_E) , &menu1 , Rectangle(25,25,75,25));
   MenuBarItem menubaritem2("menubaritem2" , font , "Me&nu Two" , input_key_press(KEY_N) , &menu2 , Rectangle(25,25,75,25));
   MenuBarItem menubaritem3("menubaritem3" , font , "Men&u Three" , input_key_press(KEY_U) , &menu3 , Rectangle(25,25,100,25));

   MenuBar menubar("menubar" , Rectangle(25,25,125,25));
   menubar << &menubaritem1 << &menubaritem2 << &menubaritem3;

   menu1.SetPadding(3);
   menu2.SetPadding(3);
   menu3.SetPadding(3);
   menu4.SetPadding(3);
   
//   IconRadioButtonItem(std::string text_label , std::string name , ICON_DISPLAY_TYPE dtype , Rectangle position , UINT wflags = DEFAULT_FLAGS);
   IconRadioButtonItem rb1itema("Radio A" , "rb1itema" , ICON_DISPLAY_CENTER , Rectangle(0,0,100,40));
   IconRadioButtonItem rb1itemb("Radio B" , "rb1itemb" , ICON_DISPLAY_CENTER , Rectangle(0,0,100,40));
   IconRadioButtonItem rb1itemc("Radio C" , "rb1itemc" , ICON_DISPLAY_CENTER , Rectangle(0,0,100,40));

//   bool LoadImages(const char* up_pic_file , const char* down_pic_file , const char* hover_pic_file , const char* disabled_pic_file ,
//                     BMP_MEM_TYPE bmp_mem_type , BMP_DRAW_TYPE bmp_draw_type);
   rb1itema.LoadImages("Data/Images/RBA_Up.bmp" , "Data/Images/RBA_Down.bmp" , "Data/Images/RBA_Hover.bmp" , 0 , MEMORY , TRANSPARENT);
   rb1itemb.LoadImages("Data/Images/RBB_Up.bmp" , "Data/Images/RBB_Down.bmp" , "Data/Images/RBB_Hover.bmp" , 0 , MEMORY , TRANSPARENT);
   rb1itemc.LoadImages("Data/Images/RBC_Up.bmp" , "Data/Images/RBC_Down.bmp" , "Data/Images/RBC_Hover.bmp" , 0 , MEMORY , TRANSPARENT);
   
//   RadioButton(std::string name , RADIO_TYPE rtype , Rectangle position , int padding = 0 , UINT wflags = DEFAULT_FLAGS);
   RadioButton radio_btn1("radio_btn1" , RADIO_HORIZONTAL , Rectangle(50,75,300,40) , 0);
   radio_btn1 << &rb1itema << &rb1itemb << &rb1itemc;

   TextWidget rblabel("rblabel" , "Focus Draw Type" , font , TEXT_LEFT , TEXT_VCENTER , 50 , 143);
   
//   RadioButtonItem(std::string name , BUTTON_TYPE btype , FONT* textfont , std::string text_label ,
//                   int btnw , int btnh , int padding , Rectangle position , UINT wflags = DEFAULT_FLAGS);
   RadioButtonItem rb2itema("rb2itema" , HOVER_BUTTON , RECTANGLE_BTN , font , "Invisible" , 20 , 20 , 5 , Rectangle(0,0,0,0));
   RadioButtonItem rb2itemb("rb2itemb" , HOVER_BUTTON , RECTANGLE_BTN , font , "Highlight Outline" , 20 , 20 , 5 , Rectangle(0,0,0,0));
   RadioButtonItem rb2itemc("rb2itemc" , HOVER_BUTTON , RECTANGLE_BTN , font , "Highlight Thick Outline" , 20 , 20 , 5 , Rectangle(0,0,0,0));
   RadioButtonItem rb2itemd("rb2itemd" , HOVER_BUTTON , RECTANGLE_BTN , font , "Highlight Dotted" , 20 , 20 , 5 , Rectangle(0,0,0,0));
   RadioButtonItem rb2iteme("rb2iteme" , HOVER_BUTTON , RECTANGLE_BTN , font , "Highlight Thick Dotted" , 20 , 20 , 5 , Rectangle(0,0,0,0));

//   RadioButton(std::string name , RADIO_TYPE rtype , Rectangle position , int padding = 0 , UINT wflags = DEFAULT_FLAGS);
   RadioButton radio_btn2("radio_btn2" , RADIO_VERTICAL , Rectangle(50,150,150,100) , 0);
   radio_btn2 << &rb2itema << &rb2itemb << &rb2itemc << &rb2itemd << &rb2iteme;
   radio_btn2.SetActiveItem("Highlight Outline");

   ListBox list_box("list_box" , font , true , Rectangle(50,275,150,58));
   list_box << "Really long item that doesn't fit" << "Line 2" << "Line 3" << "Line 4" << "Line 5" << "Line 6";
   list_box << "Line 7" << "Line 8" << "Line 9" << "Line 10" << "Line 11";


   DropDownList ddl("ddl" , font , 3 , Rectangle(50,350,200,25) , DDL_OPEN_BELOW);
   ddl << "One" << "Two" << "Three" << "Four" << "Five" << "Really really long option";
//   ddl.SetItem("One");

   BitmapHandler viewer_pic;
   if (!viewer_pic.LoadImage("Data/Images/k9_05.bmp" , NULL , MEMORY , TRANSLUCENT)) {
      OutputLog() << "Failed to load k9_05.bmp" << endl;
      return 0;
   }
   SetAlphaTotal(viewer_pic , 127);

   OutputLog() << "viewer_pic : W = " << viewer_pic.W() << " , H = " << viewer_pic.H() << endl;

   BitmapViewer bitmap_viewer("bitmap_viewer" , &viewer_pic , 20 , Rectangle(50,400,300,175));

/*
   BitmapHandler membh1(400,300,MEMORY,SOLID);
   BitmapHandler vidbh1(400,300,VIDEO,SOLID);
   clear_to_color(membh1 , makecol(0,0,255));
   blit(membh1 , screen , 0 , 0 , 0 , 0 , 400 , 300);
   OutputLog() << "";
//*/

   WidgetHandler tab_gui1("tab_gui1" , font , "Tab One" , false , false , NULL , Rectangle(400,50,350,225));
   WidgetHandler tab_gui2("tab_gui2" , font , "Tab Two" , false , false , NULL , Rectangle(400,50,350,225));
   WidgetHandler tab_gui3("tab_gui3" , font , "Tab Three" , false , false , NULL , Rectangle(400,50,350,225));
   WidgetHandler tab_gui4("tab_gui4" , font , "Tab Four" , false , false , NULL , Rectangle(400,50,350,225));
   WidgetHandler tab_gui5("tab_gui5" , font , "Tab Five" , false , false , NULL , Rectangle(400,50,350,225));
   tab_gui1.SetupBitmapHandlers(BG_TRANSLUCENT , MEMORY , TRANSLUCENT);
   tab_gui2.SetupBitmapHandlers(BG_TRANSLUCENT , MEMORY , TRANSLUCENT);
   tab_gui3.SetupBitmapHandlers(BG_TRANSLUCENT , MEMORY , TRANSLUCENT);
   tab_gui4.SetupBitmapHandlers(BG_SOLID , MEMORY , SOLID);
   tab_gui5.SetupBitmapHandlers(BG_SOLID , MEMORY , SOLID);
   tab_gui1.SetTitleBarVisibility(false);
   tab_gui2.SetTitleBarVisibility(false);
   tab_gui3.SetTitleBarVisibility(false);
   tab_gui4.SetTitleBarVisibility(false);
   tab_gui5.SetTitleBarVisibility(false);
/**
   tab_gui1.SetBlenderType(USE_ALPHA_LAYERING_BLENDER);
   tab_gui2.SetBlenderType(USE_ALPHA_LAYERING_BLENDER);
   tab_gui3.SetBlenderType(USE_ALPHA_LAYERING_BLENDER);
   tab_gui4.SetBlenderType(USE_ALPHA_LAYERING_BLENDER);
   tab_gui5.SetBlenderType(USE_ALPHA_LAYERING_BLENDER);
//*/
   /// Color gui setup
   const WidgetColorset* wcol = gui.WCols();

   Slider<int> red_slider("red_slider" , 0,(*wcol)[SDCOL].R() , 255 , 1 , true , Rectangle(15,12,15,160));
   Slider<int> green_slider("green_slider" , 0,(*wcol)[SDCOL].G() , 255 , 1 , true , Rectangle(45,12,15,160));
   Slider<int> blue_slider("blue_slider" , 0,(*wcol)[SDCOL].B() , 255 , 1 , true , Rectangle(75,12,15,160));
   Slider<int> alpha_slider("alpha_slider" , 0,(*wcol)[SDCOL].A() , 255 , 1 , true , Rectangle(105,12,15,160));

   TextWidget rslabel("rslabel" , "R" , 15 , 175);
   TextWidget gslabel("gslabel" , "G" , 45 , 175);
   TextWidget bslabel("bslabel" , "B" , 75 , 175);
   TextWidget aslabel("aslabel" , "A" , 105 , 175);

   TextWidget rsvalue("rsvalue" , (*wcol)[SDCOL].R() , 15 , 190);
   TextWidget gsvalue("gsvalue" , (*wcol)[SDCOL].G() , 45 , 190);
   TextWidget bsvalue("bsvalue" , (*wcol)[SDCOL].B() , 75 , 190);
   TextWidget asvalue("asvalue" , (*wcol)[SDCOL].A() , 105 , 190);

//   RadioButtonItem(std::string name , BUTTON_TYPE btype , FONT* textfont , std::string text_label ,
//                   int btnw , int btnh , int padding , Rectangle position , UINT wflags = DEFAULT_FLAGS);
   RadioButtonItem rbi_sdcol("rbi_sdcol" , HOVER_BUTTON , ELLIPSE_BTN , font , "SDCOL" , 50 , 25 , 10 , Rectangle(0,0,0,0));
   RadioButtonItem rbi_bgcol("rbi_bgcol" , HOVER_BUTTON , ELLIPSE_BTN , font , "BGCOL" , 50 , 25 , 10 , Rectangle(0,0,0,0));
   RadioButtonItem rbi_mgcol("rbi_mgcol" , HOVER_BUTTON , ELLIPSE_BTN , font , "MGCOL" , 50 , 25 , 10 , Rectangle(0,0,0,0));
   RadioButtonItem rbi_fgcol("rbi_fgcol" , HOVER_BUTTON , ELLIPSE_BTN , font , "FGCOL" , 50 , 25 , 10 , Rectangle(0,0,0,0));
   RadioButtonItem rbi_hlcol("rbi_hlcol" , HOVER_BUTTON , ELLIPSE_BTN , font , "HLCOL" , 50 , 25 , 10 , Rectangle(0,0,0,0));
   RadioButtonItem rbi_txtcol("rbi_txtcol" , HOVER_BUTTON , ELLIPSE_BTN , font , "TXTCOL" , 50 , 25 , 10 , Rectangle(0,0,0,0));

//   RadioButton(std::string name , RADIO_TYPE rtype , Rectangle position , int padding = 0 , UINT wflags = DEFAULT_FLAGS);
   RadioButton color_radio("color_radio" , RADIO_VERTICAL , Rectangle(175,12,50,100) , 0);
///   RadioButton color_radio("color_radio" , RADIO_VERTICAL , ELLIPSE_BTN , font , 150 , 25 , 0.3 , Rectangle(175 , 12 , 150 , 150));
   color_radio << &rbi_sdcol << &rbi_bgcol << &rbi_mgcol << &rbi_fgcol << &rbi_hlcol << &rbi_txtcol;

   tab_gui1 << &red_slider << &green_slider << &blue_slider << &alpha_slider << &color_radio;
   tab_gui1 << &rslabel << &gslabel << &bslabel << &aslabel << &rsvalue << &gsvalue << &bsvalue << &asvalue;


   /// Tint gui setup
   Color tint_color(127 , 0 , 255 , 32);
   int tint_degree(64);
   WidgetColorset tinted_colors = DefaultWidgetColors();
   tinted_colors.Tint(tint_color , (unsigned int)tint_degree);
   tinted_colors[TXTCOL].SetRGBA(255,255,255,254);
   gui.UseWidgetColorset(&tinted_colors);
   Slider<int> tint_red_slider("tint_red_slider" , 0,tint_color.R() , 255 , 1 , true , Rectangle(15,12,15,160));
   Slider<int> tint_green_slider("tint_green_slider" , 0,tint_color.G() , 255 , 1 , true , Rectangle(45,12,15,160));
   Slider<int> tint_blue_slider("tint_blue_slider" , 0,tint_color.B() , 255 , 1 , true , Rectangle(75,12,15,160));
   Slider<int> tint_n_slider("tint_n_slider" , 0,tint_degree , 255 , true , 1 , Rectangle(105,12,15,160));

   TextWidget tint_rslabel("tint_rslabel" , "R" , 15 , 175);
   TextWidget tint_gslabel("tint_gslabel" , "G" , 45 , 175);
   TextWidget tint_bslabel("tint_bslabel" , "B" , 75 , 175);
   TextWidget tint_nslabel("tint_nslabel" , "N" , 105 , 175);

   TextWidget tint_rsvalue("tint_rsvalue" , tint_color.R() , 15 , 190);
   TextWidget tint_gsvalue("tint_gsvalue" , tint_color.G() , 45 , 190);
   TextWidget tint_bsvalue("tint_bsvalue" , tint_color.B() , 75 , 190);
   TextWidget tint_nsvalue("tint_nsvalue" , tint_degree    , 105 , 190);

   Button inverter("inverter" , ROUNDED_BTN , TOGGLE_BTN , font , "Invert" , input_key_press(KEY_NONE) , Rectangle(175 , 12 , 75 , 25));

   tab_gui2 << &tint_red_slider << &tint_green_slider << &tint_blue_slider << &tint_n_slider;
   tab_gui2 << &tint_rslabel << &tint_gslabel << &tint_bslabel << &tint_nslabel << &tint_rsvalue << &tint_gsvalue;
   tab_gui2 << &tint_bsvalue << &tint_nsvalue << &inverter;


   /// HSV gui setup
   WidgetColorset hsv_colors;
   float start_hue = 180.0f;
   float start_sat = 1.0f;
   Slider<float> hue_slider("hue_slider" , 0.0f , start_hue , 360.0f , 1.0f , false , Rectangle(10,10,225,10));
   Slider<float> sat_slider("sat_slider" , 0.0f , start_sat , 1.0f , 0.01f , false , Rectangle(10,30,225,10));

   Slider<float> vslider0("vslider0" , 0.0f , default_hsv_vcolors[SDCOL] , 1.0f , 0.01f , false , Rectangle(10,70,225,10));
   Slider<float> vslider1("vslider1" , 0.0f , default_hsv_vcolors[BGCOL] , 1.0f , 0.01f , false , Rectangle(10,90,225,10));
   Slider<float> vslider2("vslider2" , 0.0f , default_hsv_vcolors[MGCOL] , 1.0f , 0.01f , false , Rectangle(10,110,225,10));
   Slider<float> vslider3("vslider3" , 0.0f , default_hsv_vcolors[FGCOL] , 1.0f , 0.01f , false , Rectangle(10,130,225,10));
   Slider<float> vslider4("vslider4" , 0.0f , default_hsv_vcolors[HLCOL] , 1.0f , 0.01f , false , Rectangle(10,150,225,10));
   Slider<float> vslider5("vslider5" , 0.0f , default_hsv_vcolors[TXTCOL] , 1.0f , 0.01f , false , Rectangle(10,170,225,10));

   TextWidget hslabel("hslabel" , "Hue" , font , TEXT_RIGHT , TEXT_TOP , 350 , 10);
   TextWidget sslabel("sslabel" , "Sat" , font , TEXT_RIGHT , TEXT_TOP , 350 , 30);

   TextWidget vlabel("vlabel" , "Value" , 110 , 60);
   TextWidget vslabel0("vslabel0" , "SDCOL"  , font , TEXT_RIGHT , TEXT_TOP , 350 , 70);
   TextWidget vslabel1("vslabel1" , "BGCOL"  , font , TEXT_RIGHT , TEXT_TOP , 350 , 90);
   TextWidget vslabel2("vslabel2" , "MGCOL"  , font , TEXT_RIGHT , TEXT_TOP , 350 , 110);
   TextWidget vslabel3("vslabel3" , "FGCOL"  , font , TEXT_RIGHT , TEXT_TOP , 350 , 130);
   TextWidget vslabel4("vslabel4" , "HLCOL"  , font , TEXT_RIGHT , TEXT_TOP , 350 , 150);
   TextWidget vslabel5("vslabel5" , "TXTCOL" , font , TEXT_RIGHT , TEXT_TOP , 350 , 170);
   
   unsigned int min_width = 4;
   unsigned int num_decimals = 2;
   
   TextWidget huevalue("huevalue" , start_hue , min_width , num_decimals , 250 , 10);
   TextWidget satvalue("satvalue" , start_sat , min_width , num_decimals , 250 , 30);
   TextWidget vs0value("vs0value" , default_hsv_vcolors[SDCOL] , min_width , num_decimals , 250 , 70);
   TextWidget vs1value("vs1value" , default_hsv_vcolors[BGCOL] , min_width , num_decimals , 250 , 90);
   TextWidget vs2value("vs2value" , default_hsv_vcolors[MGCOL] , min_width , num_decimals , 250 , 110);
   TextWidget vs3value("vs3value" , default_hsv_vcolors[FGCOL] , min_width , num_decimals , 250 , 130);
   TextWidget vs4value("vs4value" , default_hsv_vcolors[HLCOL] , min_width , num_decimals , 250 , 150);
   TextWidget vs5value("vs5value" , default_hsv_vcolors[TXTCOL] , min_width , num_decimals , 250 , 170);

   tab_gui3 << &hue_slider << &sat_slider << &vslider0 << &vslider1 << &vslider2 << &vslider3 << &vslider4 << &vslider5;
   tab_gui3 << &hslabel << &sslabel << &vlabel << &vslabel0 << &vslabel1 << &vslabel2 << &vslabel3 << &vslabel4 << &vslabel5;
   tab_gui3 << &huevalue << &satvalue << &vs0value << &vs1value << &vs2value << &vs3value << &vs4value << &vs5value;

///   TextInput(TEXT_INPUT_TYPE type , FONT* textfont , int num_chars_wide , int max_num_chars ,
///               int xpos , int ypos , UINT wflags = DEFAULT_FLAGS);
   TextInput text_input1("text_input1" , TEXT_INPUT           , font , 12 , 32 , 15 , 15);
   TextInput text_input2("text_input2" , INTEGER_INPUT        , font , 12 , 32 , 15 , 45);
   TextInput text_input3("text_input3" , FLOATING_POINT_INPUT , font , 12 , 32 , 15 , 75);
   
   text_input2.SetIntegerRange(-255,0,255);
   text_input3.SetFloatRange(-137.0,0.0,137.5);

   tab_gui4 << &text_input1 << &text_input2 << &text_input3;

   FileSelector fselect("fselect" , font , true , true , "Open" , "C:\\ctwoplus\\progcode\\allegro\\GUI_Allegro2" ,
                        "" , "*" , Rectangle(0,0,350,250));
   
   tab_gui5 << &fselect;


///   Tab(string name , FONT* text_font , string text , WidgetHandler& tab_gui , UINT wflags = DEFAULT_FLAGS);
   Tab tab1("tab1" , font , "Color" , tab_gui1);
   Tab tab2("tab2" , font , "Tint"  , tab_gui2);
   Tab tab3("tab3" , font , "HSV"   , tab_gui3);
   Tab tab4("tab4" , font , "Input" , tab_gui4);
   Tab tab5("tab5" , font , "Files" , tab_gui5);

///   TabGroup(string name , int gui_width , int gui_height , const Rectangle& position , UINT wflags = DEFAULT_FLAGS);
   TabGroup tabgroup("tabgroup" , 350 , 250 , Rectangle(400,25,350,250));
   tabgroup << tab1 << tab2 << tab3 << tab4 << tab5;

   gui << &menubar << &radio_btn1 << &rblabel << &radio_btn2 << &list_box << &ddl << &bitmap_viewer << &tabgroup;

   /// Virtual page 2
   Camera reg_cam("reg_cam" , SOLID , bg_pic , Rectangle(0 , 0 , 350 , 250) , Rectangle(800 + 25 , 25 , 350 , 250) );
   SBCamera sb_cam("sb_cam" , bg_pic , SOLID , Rectangle(0 , 0 , 350 , 250) , Rectangle(800 + 425 , 25 , 350 , 250) );
   ZoomCamera zoom_cam1("zoom_cam1" , USE_SMOOTH_STRETCH , bg_pic , Rectangle(0 , 0 , 350 , 250) , Rectangle(800 + 25 , 325 , 350 , 250) );
   ZoomCamera zoom_cam2("zoom_cam2" , USE_ALLEGRO_STRETCH , bg_pic , Rectangle(0 , 0 , 350 , 250) , Rectangle(800 + 425 , 325 , 350 , 250) );
   
   gui << &reg_cam << &sb_cam << &zoom_cam1 << &zoom_cam2;


   /// Virtual page 3
   RadioButton pointer_radio("pointer_radio" , RADIO_VERTICAL , Rectangle(50 , 650 , 500,500));
   for (int i = 0 ; i < NUM_POINTERS ; ++i) {
//   RadioButtonItem(std::string name , BUTTON_TYPE btype , FONT* textfont , std::string text_label ,
//                   int btnw , int btnh , int padding , Rectangle position , UINT wflags = DEFAULT_FLAGS);
      RadioButtonItem* rbi = new RadioButtonItem("rbi" , HOVER_BUTTON , ROUNDED_BTN , font , pointer_names[i] , 32 , 16 , 0 , Rectangle(0,0,1,1));
///   virtual void AddItem(RadioButtonItemBase* rbtn , bool delete_when_done);
      pointer_radio.AddItem(rbi , true);
   }
   
   pointer_radio.SetActiveItem("NORMAL");
   
   WidgetAdjuster adjuster("adjuster");
   
   gui << &pointer_radio << &adjuster;
   
   
   /// Virtual page 4
   

   /** -------------    Gui Message testing setup    ----------------------- */
   

   const WidgetMsg spr1msg(&spr1 , TOPIC_BUTTON_WIDGET , BUTTON_CLICKED);
   const WidgetMsg spr2msg(&spr2 , TOPIC_BUTTON_WIDGET , BUTTON_CLICKED);
   const WidgetMsg spr3msg(&spr3 , TOPIC_BUTTON_WIDGET , BUTTON_CLICKED);
   const WidgetMsg spr4msg(&spr4 , TOPIC_BUTTON_WIDGET , BUTTON_CLICKED);

   const WidgetMsg slider1msg(&slider1 , TOPIC_SLIDER , SLIDER_VALUE_CHANGED);

   const WidgetMsg menuitem1msg(&menuitem1 , TOPIC_MENU_ITEM , MENU_ITEM_CLICKED);
   const WidgetMsg menuitem2msg(&menuitem2 , TOPIC_MENU_ITEM , MENU_ITEM_CLICKED);
   const WidgetMsg menuitem3msg(&menuitem3 , TOPIC_MENU_ITEM , MENU_ITEM_CLICKED);
   const WidgetMsg menuitem4msg(&menuitem4 , TOPIC_MENU_ITEM , MENU_ITEM_CLICKED);
   const WidgetMsg menuitem5msg(&menuitem5 , TOPIC_MENU_ITEM , MENU_ITEM_CLICKED);
   const WidgetMsg menuitem6msg(&menuitem6 , TOPIC_MENU_ITEM , MENU_ITEM_CLICKED);
   const WidgetMsg menuitem7msg(&menuitem7 , TOPIC_MENU_ITEM , MENU_ITEM_CLICKED);
   const WidgetMsg menuitem8msg(&menuitem8 , TOPIC_MENU_ITEM , MENU_ITEM_CLICKED);
   const WidgetMsg menuitem9msg(&menuitem9 , TOPIC_MENU_ITEM , MENU_ITEM_CLICKED);
   const WidgetMsg menuitem10msg(&menuitem10 , TOPIC_MENU_ITEM , MENU_ITEM_CLICKED);
   const WidgetMsg menuitem11msg(&menuitem11 , TOPIC_MENU_ITEM , MENU_ITEM_CLICKED);
   const WidgetMsg menuitem12msg(&menuitem12 , TOPIC_MENU_ITEM , MENU_ITEM_CLICKED);

   const WidgetMsg radio1msg(&radio_btn1 , TOPIC_RADIO_BUTTON , RADIO_BTN_CLICKED);
   const WidgetMsg radio2msg(&radio_btn2 , TOPIC_RADIO_BUTTON , RADIO_BTN_CLICKED);
   
   const WidgetMsg ddlmsg(&ddl , TOPIC_DDL , DDL_SELECTION_CHANGED);

   const WidgetMsg rsdelta(&red_slider , TOPIC_SLIDER , SLIDER_VALUE_CHANGED);
   const WidgetMsg gsdelta(&green_slider , TOPIC_SLIDER , SLIDER_VALUE_CHANGED);
   const WidgetMsg bsdelta(&blue_slider , TOPIC_SLIDER , SLIDER_VALUE_CHANGED);
   const WidgetMsg asdelta(&alpha_slider , TOPIC_SLIDER , SLIDER_VALUE_CHANGED);
   
   const WidgetMsg colselect(&color_radio , TOPIC_RADIO_BUTTON , RADIO_BTN_CLICKED);

   const WidgetMsg tint_rs_delta(&tint_red_slider , TOPIC_SLIDER , SLIDER_VALUE_CHANGED);
   const WidgetMsg tint_gs_delta(&tint_green_slider , TOPIC_SLIDER , SLIDER_VALUE_CHANGED);
   const WidgetMsg tint_bs_delta(&tint_blue_slider , TOPIC_SLIDER , SLIDER_VALUE_CHANGED);
   const WidgetMsg tint_ns_delta(&tint_n_slider , TOPIC_SLIDER , SLIDER_VALUE_CHANGED);

   const WidgetMsg invert_tint(&inverter , TOPIC_BUTTON_WIDGET , BUTTON_TOGGLED);   

   const WidgetMsg hs_delta(&hue_slider , TOPIC_SLIDER , SLIDER_VALUE_CHANGED);
   const WidgetMsg ss_delta(&sat_slider , TOPIC_SLIDER , SLIDER_VALUE_CHANGED);
   const WidgetMsg vs0_delta(&vslider0 , TOPIC_SLIDER , SLIDER_VALUE_CHANGED);
   const WidgetMsg vs1_delta(&vslider1 , TOPIC_SLIDER , SLIDER_VALUE_CHANGED);
   const WidgetMsg vs2_delta(&vslider2 , TOPIC_SLIDER , SLIDER_VALUE_CHANGED);
   const WidgetMsg vs3_delta(&vslider3 , TOPIC_SLIDER , SLIDER_VALUE_CHANGED);
   const WidgetMsg vs4_delta(&vslider4 , TOPIC_SLIDER , SLIDER_VALUE_CHANGED);
   const WidgetMsg vs5_delta(&vslider5 , TOPIC_SLIDER , SLIDER_VALUE_CHANGED);

   const WidgetMsg minimizemsg = btn_panel.TitleMinimizedMessage();
   const WidgetMsg maximizemsg = btn_panel.TitleMaximizedMessage();
   
   const WidgetMsg pointer_radio_msg(&pointer_radio , TOPIC_RADIO_BUTTON , RADIO_BTN_CLICKED);
   const WidgetMsg adjust_msg(&adjust_btn , TOPIC_BUTTON_WIDGET , BUTTON_TOGGLED);
   
/*
   OutputLog() << gui << endl;
   OutputLog() << tab_gui1 << endl;
   OutputLog() << tab_gui2 << endl;
   OutputLog() << tab_gui3 << endl;
   OutputLog() << tab_gui4 << endl;
   OutputLog() << tab_gui5 << endl;
*/

   if (!gui.Init()) {return 0;}
   if (!gui.Ready()) {return 0;}
   
//   OutputLog() << "Initial gui status after Init() and Ready() :" << endl << endl;
//   OutputLog() << gui << endl;
   char exe_file[1025];
   get_executable_name(exe_file , 1024);
   OutputLog() << "The name of the executable file running is '" << exe_file << "'" << endl;
   OutputLog() << "argv[0] is '" << argv[0] << "'" << endl;
   OutputLog() << "The executable file running " << (file_exists(exe_file , ~FA_DIREC , 0)?"exists":"does not exist") << endl;
/*
   BuildFileList("c:\\*" , FA_DIREC , FA_NONE);
   PrintFileList(OutputLog());
*/
   const char* path = "c:\\ctwoplus";
   if (file_exists(path , FA_DIREC , 0)) {
      OutputLog() << "The c:\\ctwoplus folder exists" << endl;
   } else {
      OutputLog() << "The c:\\ctwoplus folder does not exist" << endl;
   }

/*
   FileSystem fs;
   fs.ReadAllFolders();
   OutputLog() << "Printing file system..." << endl;
   fs.PrintFileSystem(OutputLog());
   OutputLog() << endl << fs << endl;
*/

/*
   list<Folder>& fl = fs.drives;
   for (list<Folder>::iterator it = fl.begin() ; it != fl.end() ; ++it) {
      Folder& f = *it;
      f.ReadFolder();
      f.PrintSubFolders(OutputLog());
   }
*/

   //string& replace( size_type index, size_type num1, size_type num2, char ch );
   string s = "";
   s.replace(0 , 1 , 1 , 'a');
   OutputLog() << "s is '" << s << "'" << endl;

   bool grabbed_mouse = false;
   int mouse_id = AcquireMouseOwnerId();

   show_mouse(screen);
   
   double dt = 0.0;
   if (SetupAndStartTimer(BPS_TO_TIMER(60)) != 0) {return 0;}
   
   /** TODO : Investigate context switching - when the window loses and regains focus, sometimes 
   it stops responding to input (Windows input)
   */
   while (!input_quit && !close_program) {
      while(GetTickCount() < 1) {rest(1);}
      
      dt = GetDeltaTime();
      UpdateInputHandler(dt);
      
      gui.Update(dt);
      WidgetMsg input_check = gui.CheckInputs(mx , my);
      if (input_check.Messages() & DIALOG_CLOSE) {
         OutputLog() << "DIALOG_CLOSE message sent from : " << input_check.From() << endl;
         if (input_check.From() == &btn_panel) {
            btn_panel.HideWidget();
         }
         if ((input_check.From() != &fselect) && (input_check.From() != &btn_panel)) {
            break;
         }
      }
      
      bool input_used = input_check.Messages() & DIALOG_INPUT_USED;
      
      while (gui.HasMessages()) {
         WidgetMsg wmsg = gui.TakeNextMessage();
         
///         OutputLog() << "Gui received message of [ " << wmsg << " ]" << endl;
         
         if (wmsg == spr1msg) {OutputLog() << "Rectangular Spring Button 1 pressed." << endl;}
         if (wmsg == spr2msg) {OutputLog() << "Circular Spring Button 2 pressed." << endl;}
         if (wmsg == spr3msg) {OutputLog() << "Rounded Spring Button 3 pressed." << endl;}
         if (wmsg == spr4msg) {OutputLog() << "Elliptical Spring Button 4 pressed." << endl;}
         
         if (wmsg == slider1msg) {
///            btn_panel_colors = *(gui.WCols());
///            btn_panel_colors.SetAlpha((UCHAR)((float)255*slider1.GetValue()));
///            btn_panel.UseWidgetColorset(&btn_panel_colors);
            tdw3.SetText(slider1.GetValue() , 3 , 2);
            btn_panel.SetRoundingRadiusPercent(slider1.GetValue());
            spr3.SetRoundingPercent(slider1.GetValue());
         }
         if (wmsg == menuitem1msg) {OutputLog() << "Menu item 1 pressed." << endl;}
         if (wmsg == menuitem2msg) {OutputLog() << "Menu item 2 pressed." << endl;}
         if (wmsg == menuitem3msg) {OutputLog() << "Menu item 3 pressed." << endl;}
         if (wmsg == menuitem4msg) {OutputLog() << "Menu item 4 pressed." << endl;}
         if (wmsg == menuitem5msg) {OutputLog() << "Menu item 5 pressed." << endl;}
         if (wmsg == menuitem6msg) {OutputLog() << "Menu item 6 pressed." << endl;}
         if (wmsg == menuitem7msg) {OutputLog() << "Menu item 7 pressed." << endl;}
         if (wmsg == menuitem8msg) {OutputLog() << "Menu item 8 pressed." << endl;}
         if (wmsg == menuitem9msg) {OutputLog() << "Menu item 9 pressed." << endl;}
         if (wmsg == menuitem10msg) {OutputLog() << "Menu item 10 pressed." << endl;}
         if (wmsg == menuitem11msg) {OutputLog() << "Menu item 11 pressed." << endl;}
         if (wmsg == menuitem12msg) {OutputLog() << "Menu item 12 pressed." << endl;}
         
         if (wmsg == radio1msg) {
            OutputLog() << "Radio button #1 - '" << radio_btn1.ActiveItem() << "' selected." << endl;
         }
         if (wmsg == radio2msg) {
            OutputLog() << "Radio button #2 - '" << radio_btn2.ActiveItem() << "' selected." << endl;
            FOCUS_DRAW_TYPE fdt = (FOCUS_DRAW_TYPE)radio_btn2.ActiveItemIndex();
            gui.SetFocusDrawType(fdt , true);
         }
         
         if (wmsg == ddlmsg) {
            OutputLog() << "Drop down list selection changed to " << ddl.GetSelection() << endl;
         }
         
         if ((wmsg == rsdelta) || (wmsg == gsdelta) || (wmsg == bsdelta) || (wmsg == asdelta)) {
            // color values changed for selected color
            WidgetColorset wc = *(gui.WCols());
            wc[color_radio.ActiveItemIndex()].SetRGBA(
               red_slider.GetValue() , green_slider.GetValue() , blue_slider.GetValue() , alpha_slider.GetValue());
            gui.SetWidgetColors(wc);
            if (wmsg == rsdelta) {rsvalue.SetText(red_slider.GetValue());}
            if (wmsg == gsdelta) {gsvalue.SetText(green_slider.GetValue());}
            if (wmsg == bsdelta) {bsvalue.SetText(blue_slider.GetValue());}
            if (wmsg == asdelta) {asvalue.SetText(alpha_slider.GetValue());}
         }
         
         if (wmsg == colselect) {
            // selected color changed
            // read values from selected color and set sliders and text to match
            Color col = (*(gui.WCols()))[(color_radio.ActiveItemIndex())];
            red_slider.SetValue(col.R());
            green_slider.SetValue(col.G());
            blue_slider.SetValue(col.B());
            alpha_slider.SetValue(col.A());
            rsvalue.SetText(col.R());
            gsvalue.SetText(col.G());
            bsvalue.SetText(col.B());
            asvalue.SetText(col.A());
         }
         if ((wmsg == tint_rs_delta) || (wmsg == tint_gs_delta) || (wmsg == tint_bs_delta) || (wmsg == tint_ns_delta) || (wmsg == invert_tint)) {
            // tint slider value changed , update tint , update labels
            tint_color.SetRGB(tint_red_slider.GetValue() , tint_green_slider.GetValue() , tint_blue_slider.GetValue());
            tinted_colors = DefaultWidgetColors();
            if (!inverter.ButtonUpState()) {tinted_colors.Invert();}
            tinted_colors.Tint(tint_color , (unsigned int)tint_n_slider.GetValue());
            if (!inverter.ButtonUpState()) {
               tinted_colors[TXTCOL].SetRGBA(0,0,0,255);
            } else {
               tinted_colors[TXTCOL].SetRGBA(255,255,255,254);
            }
            gui.UseWidgetColorset(&tinted_colors);
            tint_rsvalue.SetText(tint_red_slider.GetValue());
            tint_gsvalue.SetText(tint_green_slider.GetValue());
            tint_bsvalue.SetText(tint_blue_slider.GetValue());
            tint_nsvalue.SetText(tint_n_slider.GetValue());
            gui.QueueUserMessage(colselect);
         }
         
         if ((wmsg == hs_delta) || (wmsg == ss_delta) || (wmsg == vs0_delta) || 
             (wmsg == vs1_delta) || (wmsg == vs2_delta) || (wmsg == vs3_delta) || (wmsg == vs4_delta) || (wmsg == vs5_delta)) {
            set_default_hsv_vcolor(SDCOL  , vslider0.GetValue());
            set_default_hsv_vcolor(BGCOL  , vslider1.GetValue());
            set_default_hsv_vcolor(MGCOL  , vslider2.GetValue());
            set_default_hsv_vcolor(FGCOL  , vslider3.GetValue());
            set_default_hsv_vcolor(HLCOL  , vslider4.GetValue());
            set_default_hsv_vcolor(TXTCOL , vslider5.GetValue());
            huevalue.SetText(hue_slider.GetValue() , min_width , num_decimals);
            satvalue.SetText(sat_slider.GetValue() , min_width , num_decimals);
            vs0value.SetText(vslider0.GetValue() , min_width , num_decimals);
            vs1value.SetText(vslider1.GetValue() , min_width , num_decimals);
            vs2value.SetText(vslider2.GetValue() , min_width , num_decimals);
            vs3value.SetText(vslider3.GetValue() , min_width , num_decimals);
            vs4value.SetText(vslider4.GetValue() , min_width , num_decimals);
            vs5value.SetText(vslider5.GetValue() , min_width , num_decimals);
            hsv_colors = DefaultWidgetHSVColors(hue_slider.GetValue() , sat_slider.GetValue());
            gui.UseWidgetColorset(&hsv_colors);
            gui.QueueUserMessage(colselect);
         }



         
         if (wmsg == minimizemsg) {
            OutputLog() << "Button panel minimized." << endl;
         }
         if (wmsg == maximizemsg) {
            OutputLog() << "Button panel maximized." << endl;
         }
         
         if (wmsg == pointer_radio_msg) {
            MOUSE_POINTER mp = (MOUSE_POINTER)(pointer_radio.ActiveItemIndex());
            if (mp == POINTER_NORMAL) {
               ReleaseMousePointer(mouse_owner_id);
            }
            else {
               GrabMousePointer(mouse_owner_id);
               UseMousePointer(mp , mouse_owner_id);
            }
         }
         
         if (wmsg == adjust_msg) {
            adjuster.SetEnabledState(!adjust_btn.ButtonUpState());
            if (!adjust_btn.ButtonUpState()) {
               gui.GiveWidgetFocus(&adjuster);
            }
         }
      }
      
      if (!input_used) {
         
         
         if (input_key_press(KEY_L)) {
            OutputLog() << endl << "After pressing L, gui status is :" << endl << endl;
            OutputLog() << gui << endl;
         }
         
         if (input_key_press(KEY_T)) {
            if (grabbed_mouse) {
               ReleaseMousePointer(mouse_id);
            }
            else {
               GrabMousePointer(mouse_id);
               UseMousePointer(POINTER_BUSY , mouse_id);
            }
            grabbed_mouse = !grabbed_mouse;
         }
         double acctime = 3.0;
         if (input_key_press(KEY_1) && input_key_held(KEY_NO_MOD)) {
            gui.AccMoveViewTlxTo(0 , 0 , acctime);
         }
         if (input_key_press(KEY_2) && input_key_held(KEY_NO_MOD)) {
            gui.AccMoveViewTlxTo(dw , 0 , acctime);
         }
         if (input_key_press(KEY_3) && input_key_held(KEY_NO_MOD)) {
            gui.AccMoveViewTlxTo(0 , dh , acctime);
         }
         if (input_key_press(KEY_4) && input_key_held(KEY_NO_MOD)) {
            gui.AccMoveViewTlxTo(dw , dh , acctime);
         }
         
         double movetime = 0.0;
         if (input_key_press(KEY_1) && input_key_held(KEY_ONLY_SHIFT)) {
            gui.MoveViewTlxTo(0 , 0 , movetime);
         }
         if (input_key_press(KEY_2) && input_key_held(KEY_ONLY_SHIFT)) {
            gui.MoveViewTlxTo(dw , 0 , movetime);
         }
         if (input_key_press(KEY_3) && input_key_held(KEY_ONLY_SHIFT)) {
            gui.MoveViewTlxTo(0 , dh , movetime);
         }
         if (input_key_press(KEY_4) && input_key_held(KEY_ONLY_SHIFT)) {
            gui.MoveViewTlxTo(dw , dh , movetime);
         }

         if (input_key_press(KEY_1) && input_key_held(KEY_ONLY_CTRL)) {
            zoom_cam1.SetZoomValue(0);
            zoom_cam2.SetZoomValue(0);
         }
         if (input_key_press(KEY_2) && input_key_held(KEY_ONLY_CTRL)) {
            zoom_cam1.SetZoomValue(100);
            zoom_cam2.SetZoomValue(100);
         }
         if (input_key_press(KEY_3) && input_key_held(KEY_ONLY_CTRL)) {
            zoom_cam1.SetZoomValue(200);
            zoom_cam2.SetZoomValue(200);
         }
         if (input_key_press(KEY_4) && input_key_held(KEY_ONLY_CTRL)) {
            zoom_cam1.SetZoomValue(300);
            zoom_cam2.SetZoomValue(300);
         }
         if (input_key_press(KEY_F1) && input_key_held(KEY_ANY_SHIFT)) {
            btn_panel.ToggleWidgetVisibility();
         }
         if (input_key_press(KEY_F1) && input_key_held(KEY_NO_MOD)) {
            btn_panel.ToggleMinimizeState();
         }
         if (input_key_press(KEY_SPACE)) {
            OutputLog() << "Breakpoint for debugging." << endl;
         }
         if (input_key_press(KEY_S) && input_key_held(KEY_ANY_CTRL)) {
            scare_mouse();
            acquire_screen();
            save_bitmap("Screenie.bmp" , screen , NULL);
            release_screen();
            unscare_mouse();
         }
         
      }
      
      scare_mouse();
      acquire_screen();
      gui.Display();
      release_screen();
      unscare_mouse();
   }
   
   gui.Shutdown();
   
   sb_cam.Free();
   
   }
   
   allegro_exit();
   
   return 0;
}
END_OF_MAIN()


