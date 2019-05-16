

#include "SoundTest.hpp"

#include "Eagle.hpp"

using std::endl;


int SoundTestMain(int argc , char** argv) {
   
   OutputLog() << "Sound test main started." << endl;
   if (!SendOutputToFile("SoundTestMain_Log.txt" , "Sound Test\n\n" , false)) {return 0;}
   
   if (!GetAllegroReady(true , true , true)) {return 0;}
   if (!DesktopInfoKnown()) {return 0;}
   SetupInputHandler();
   
   set_mixer_quality(2);
   if (install_sound(DIGI_AUTODETECT , MIDI_NONE , NULL) != 0) {
      OutputLog() << "Could not install sound." << endl;
      return 0;
   }
   
   OutputLog() << "Mixer quality is " << get_mixer_quality() << endl;
   OutputLog() << "Mixer frequency is " << get_mixer_frequency() << endl;
   OutputLog() << "Mixer bit depth is " << get_mixer_bits() << endl;
   OutputLog() << "Mixer channels is " << get_mixer_channels() << endl;
   OutputLog() << "Number of mixer voices is " << get_mixer_voices() << endl;
   OutputLog() << "Mixer buffer length is " << get_mixer_buffer_length() << endl;
   
   set_volume(255,255);
   
   const int dw = 800;
   const int dh = 600;
   GfxModeData gfxmode(GFX_AUTODETECT_WINDOWED , dw , dh , DCD());

   gfx_mode_handler.Setup(SW_BACKGROUND , 0 , 0);
   int ret = gfx_mode_handler.ChangeToMode(gfxmode , CRM_DIRECT);
   if (ret != FULL_SUCCESS) {return 0;}   
   
   set_window_title("Sound Test");

	BitmapHandler buffer(create_bitmap(dw , dh) , SOLID);
	clear(buffer);
	
	const int green = makecol(0,255,0);
   const int orange = makecol(255,127,0);
   const int blue = makecol(0,0,255);
   const int cyan = makecol(0,255,255);
   const int white = makecol(255,255,255);
   const int purple = makecol(127,0,255);
   const int magenta = makecol(255,0,127);
   
   const int WFSIZE = 1024;
   WaveForm wf1(WFSIZE);
   WaveForm wf2(WFSIZE);
   WaveForm wf3(WFSIZE);
   WaveForm wf4(WFSIZE);
   WaveForm wf5(WFSIZE);
   WaveForm wf6(WFSIZE);
   WaveForm wf7(WFSIZE);
   wf1.UseArcSineWave();
   wf2.UseSineWave();
   wf3.UseSineSquaredWave();
   wf4.UseSineCubedWave();
   wf5.UseSinePentaWave();
   wf6.UseSquareWave();
   wf7.UseSawWave();
   
   wf1.Verify();
   wf2.Verify();
   wf3.Verify();
   wf4.Verify();
   wf5.Verify();
   wf6.Verify();
   wf7.Verify();
   
   VolumeForm vf1(32);
   VolumeForm vf2(512);
   vf1.UseLevelVolume(1.0f);
   vf2.UseLinearFadeOut();

   wf1.Plot(buffer , orange);
   wf2.Plot(buffer , green);
   wf3.Plot(buffer , blue);
   wf4.Plot(buffer , cyan);
   wf5.Plot(buffer , white);
   wf6.Plot(buffer , purple);
   wf7.Plot(buffer , magenta);
   
   buffer.DrawTo(screen , 0 , 0);
   clear_keybuf();
//   int keyread = readkey();

///SAMPLE* MakeSound(const WaveForm& wf , const VolumeForm& vf , float sound_freq , float duration ,
///                  int bits , int stereo , int samples_per_sec);
   SAMPLE* Sound1 = MakeSound(wf1 , vf2 , 440.0f , 1.0f , 8 , 0 , 44100);
   SAMPLE* Sound2 = MakeSound(wf2 , vf2 , 440.0f , 1.0f , 8 , 0 , 44100);
   SAMPLE* Sound3 = MakeSound(wf3 , vf2 , 440.0f , 1.0f , 8 , 0 , 44100);
   SAMPLE* Sound4 = MakeSound(wf4 , vf2 , 440.0f , 1.0f , 8 , 0 , 44100);
   SAMPLE* Sound5 = MakeSound(wf5 , vf2 , 440.0f , 1.0f , 8 , 0 , 44100);
   SAMPLE* Sound6 = MakeSound(wf6 , vf2 , 440.0f , 1.0f , 8 , 0 , 44100);
   SAMPLE* Sound7 = MakeSound(wf7 , vf2 , 440.0f , 1.0f , 8 , 0 , 44100);
   
   if (!Sound1 || !Sound2 || !Sound3 || !Sound4) {
      if (!Sound1) {OutputLog() << "Sound1 not created properly." << endl;}
      if (!Sound2) {OutputLog() << "Sound2 not created properly." << endl;}
      if (!Sound3) {OutputLog() << "Sound3 not created properly." << endl;}
      if (!Sound4) {OutputLog() << "Sound4 not created properly." << endl;}
      if (!Sound5) {OutputLog() << "Sound5 not created properly." << endl;}
      if (!Sound6) {OutputLog() << "Sound6 not created properly." << endl;}
      if (!Sound7) {OutputLog() << "Sound7 not created properly." << endl;}
      if (Sound1) {destroy_sample(Sound1);}
      if (Sound2) {destroy_sample(Sound2);}
      if (Sound3) {destroy_sample(Sound3);}
      if (Sound4) {destroy_sample(Sound4);}
      if (Sound5) {destroy_sample(Sound5);}
      if (Sound6) {destroy_sample(Sound6);}
      if (Sound7) {destroy_sample(Sound7);}
      return 0;
   }
   
/*
   ASSERT(exists("c:\\ctwoplus\\progcode\\allegro\\GUI_Allegro2\\sounds\\bomb_x.wav"));
   SAMPLE* bombsound = load_sample("c:\\ctwoplus\\progcode\\allegro\\GUI_Allegro2\\sounds\\bomb_x.wav");
   SAMPLE* helicoptersound = load_sample("sounds\\helicopter.wav");
   SAMPLE* thundersound = load_sample("sounds\\thunder_x.wav");
   
   if (!bombsound || !helicoptersound || !thundersound) {
      if (!bombsound)       {OutputLog() << "Failed to load bomb sound" << endl;}
      if (!helicoptersound) {OutputLog() << "Failed to load helicopter sound" << endl;}
      if (!thundersound)    {OutputLog() << "Failed to load thunder sound" << endl;}
      if (bombsound)       {destroy_sample(bombsound);}
      if (helicoptersound) {destroy_sample(helicoptersound);}
      if (thundersound)    {destroy_sample(thundersound);}
      destroy_sample(Sound1);
      destroy_sample(Sound2);
      destroy_sample(Sound3);
      destroy_sample(Sound4);
      destroy_sample(Sound5);
      destroy_sample(Sound6);
      destroy_sample(Sound7);
      return 0;
   }
*/
   
   while (!input_key_press(KEY_ESC)) {
///int play_sample(const SAMPLE *spl, int vol, int pan, int freq, int loop);
      if (input_key_press(KEY_1)) {
         int voice1 = play_sample(Sound1 , 255 , 127 , 1000 , 0);
         OutputLog() << "Voice #" << voice1 << " allocated to play Sound1." << endl;
      }
      if (input_key_press(KEY_2)) {
         int voice2 = play_sample(Sound2 , 255 , 127 , 1000 , 0);
         OutputLog() << "Voice #" << voice2 << " allocated to play Sound2." << endl;
      }
      if (input_key_press(KEY_3)) {
         int voice3 = play_sample(Sound3 , 255 , 127 , 1000 , 0);
         OutputLog() << "Voice #" << voice3 << " allocated to play Sound3." << endl;
      }
      if (input_key_press(KEY_4)) {
         int voice4 = play_sample(Sound4 , 255 , 127 , 1000 , 0);
         OutputLog() << "Voice #" << voice4 << " allocated to play Sound4." << endl;
      }
      if (input_key_press(KEY_5)) {
         int voice5 = play_sample(Sound5 , 255 , 127 , 1000 , 0);
         OutputLog() << "Voice #" << voice5 << " allocated to play Sound5." << endl;
      }
      if (input_key_press(KEY_6)) {
         int voice6 = play_sample(Sound6 , 255 , 127 , 1000 , 0);
         OutputLog() << "Voice #" << voice6 << " allocated to play Sound6." << endl;
      }
      if (input_key_press(KEY_7)) {
         int voice7 = play_sample(Sound7 , 255 , 127 , 1000 , 0);
         OutputLog() << "Voice #" << voice7 << " allocated to play Sound7." << endl;
      }
/*
      if (input_key_press(KEY_A)) {
         int voice8 = play_sample(bombsound , 255 , 127 , 1000 , 0);
         OutputLog() << "Voice #" << voice8 << " allocated to play bomb sound." << endl;
      }
      if (input_key_press(KEY_S)) {
         int voice9 = play_sample(helicoptersound , 255 , 127 , 1000 , 0);
         OutputLog() << "Voice #" << voice9 << " allocated to play helicopter sound." << endl;
      }
      if (input_key_press(KEY_D)) {
         int voice10 = play_sample(thundersound , 255 , 127 , 1000 , 0);
         OutputLog() << "Voice #" << voice10 << " allocated to play thunder sound." << endl;
      }
*/
      
      rest(50);
      UpdateInputHandler(0.05);
   }
   
   destroy_sample(Sound1);
   destroy_sample(Sound2);
   destroy_sample(Sound3);
   destroy_sample(Sound4);
   destroy_sample(Sound5);
   destroy_sample(Sound6);
   destroy_sample(Sound7);
/*
   destroy_sample(bombsound);
   destroy_sample(helicoptersound);
   destroy_sample(thundersound);
*/
   
   return 0;
}

