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


#ifndef Sound_H
#define Sound_H

#include <allegro.h>


/// The FloatBuffer class stores an array of floating point values
class FloatBuffer {
protected :
   float* dat;
   int    datsize;
   
   void Initialize();

public :
   FloatBuffer(int size);
   virtual ~FloatBuffer();

   void FreeDat();
   void SetSize(int size);// size must be 2 or more
   bool Ready() const;
   
   virtual bool Verify() {return true;}

   float GetMax() const ;
   float GetMin() const ;

   void Plot(BITMAP* bmp , int color) const ;// uses full bitmap to plot data

   float operator[](int index) const ;
   float operator[](float percent) const ;
   
   void Set(int index , float value);
};




/// The WaveForm class stores an array of floating point values from -1.0f to 1.0f
class WaveForm : public FloatBuffer {

public :
   WaveForm(int size);

   bool Verify();

   void UseSineWave();// sounds good
   void UseSineSquaredWave();// very little sound
   void UseSineCubedWave();// no sound
   void UseSinePentaWave();// no sound
   void UseSawWave();// sounds synthesized
   void UseSquareWave();// starting pop sound and then quiet buzz sound
   void UseArcSineWave();// sounds good, but tinny
};


/// The VolumeForm class stores an array of floating point values from 0.0f to 1.0f
class VolumeForm : public FloatBuffer {
   
public :
   VolumeForm(int size);

   bool Verify();


   void UseLevelVolume(float percent_max);// [0.0f,1.0f]
   void UseLinearFadeIn();
   void UseLinearFadeOut();

};



/// Functions for making sounds

// Only 8 bit mono sounds seem to work properly with allegro
// 8 bit stereo and 16 bit sounds have pops and sometimes mix with almost no volume
SAMPLE* MakeSound(const WaveForm& wf , const VolumeForm& vf , float sound_freq , float duration ,
                  int bits , int stereo , int samples_per_sec);




#endif // Sound_H






