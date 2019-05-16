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


#include <allegro.h>
#include "Eagle/Sound.hpp"

#include <cmath>

#ifdef _WIN32
#define M_PI       3.14159265358979323846   // pi
#endif


/// ----------------------------  FloatBuffer class --------------------------------




void FloatBuffer::Initialize() {
   for (int i = 0 ; i < datsize ; ++i) {dat[i] = 0.0f;}
}



FloatBuffer::FloatBuffer(int size) :
      dat(0),
      datsize(0)
{
   SetSize(size);
}



FloatBuffer::~FloatBuffer() {
   FreeDat();
}
   


void FloatBuffer::FreeDat() {
   if (dat) {
      delete [] dat;
      dat = 0;
      datsize = 0;
   }
}



void FloatBuffer::SetSize(int size) {
   ASSERT(size >= 2);
   FreeDat();
   dat = new float[size];
   datsize = size;
   Initialize();
}



bool FloatBuffer::Ready() const {
   if (dat) {return true;}
   return false;
}



float FloatBuffer::GetMax() const {
   ASSERT(dat);
   float max = dat[0];
   for (int i = 0 ; i < datsize ; ++i) {
      float val = dat[i];
      if (val > max) {max = val;}
   }
   return max;
}



float FloatBuffer::GetMin() const {
   ASSERT(dat);
   float min = dat[0];
   for (int i = 0 ; i < datsize ; ++i) {
      float val = dat[i];
      if (val < min) {min = val;}
   }
   return min;
}



void FloatBuffer::Plot(BITMAP* bmp , int color) const {
   ASSERT(bmp);
   ASSERT(dat);
   const float ymax = GetMax();
   const float ymin = GetMin();
   const float yh = ymax - ymin;
   const float max = datsize - 1;
   const float w = (float)(bmp->w);
   const float h = (float)(bmp->h);
   const int by = bmp->h - 1;
   float xpercent1 = ((float)0)/max;
   float ypercent1 = (dat[0] - ymin)/yh;
   int x1 = (int)(w*xpercent1);
   int y1 = by - (int)(h*ypercent1);
   acquire_bitmap(bmp);
   for (int i = 0 ; i < datsize - 1 ; ++i) {
      float xpercent2 = ((float)(i+1))/max;
      float ypercent2 = (dat[i+1] - ymin)/yh;
      int x2 = (int)(w*xpercent2);
      int y2 = by - (int)(h*ypercent2);
      line(bmp , x1 , y1 , x2 , y2 , color);
      xpercent1 = xpercent2;
      ypercent1 = ypercent2;
      x1 = x2;
      y1 = y2;
   }
   release_bitmap(bmp);
}



float FloatBuffer::operator[](int index) const {
   ASSERT(dat);
   ASSERT(index >= 0);
   ASSERT(index < datsize);
   return dat[index];
}



float FloatBuffer::operator[](float percent) const {
   ASSERT(dat);
   ASSERT(percent >= 0.0f);
   ASSERT(percent <= 1.0f);
   float index = percent*(float)(datsize - 1);
   double ipart = 0.0;
   float frac = modf(index , &ipart);
   float oneminusfrac = 1.0f - frac;
   int i1 = (int)index;
   int i2 = i1 + 1;
   return (oneminusfrac*dat[i1] + frac*dat[i2]);
}



void FloatBuffer::Set(int index , float value) {
   ASSERT(dat);
   ASSERT(index >= 0);
   ASSERT(index < datsize);
   dat[index] = value;
}




/// ---------------------------------- WaveForm class ---------------------------------------



WaveForm::WaveForm(int size) :
      FloatBuffer(size)
{}
   


bool WaveForm::Verify() {
   float max = GetMax();
   float min = GetMin();
   ASSERT(max <= 1.0f);
   ASSERT(min >= -1.0f);
   if ((max > 1.0f) || (min < -1.0f)) {return false;}
   return true;
}



void WaveForm::UseSineWave() {
   ASSERT(dat);
   const float max = (float)(datsize - 1);
   const float twopi = 2.0f*M_PI;
   for (int i = 0 ; i < datsize ; ++i) {
      const float step = (float)i;
      const float percent = step/max;
      const float angle = percent*twopi;
      const float sine = sin(angle);
      dat[i] = sine;
   }
}



void WaveForm::UseSineSquaredWave() {
   ASSERT(dat);
   const float max = (float)(datsize - 1);
//   const float twopi = 2.0f*M_PI;
   for (int i = 0 ; i < datsize ; ++i) {
      const float step = (float)i;
      const float percent = step/max;
      const float angle = percent*M_PI;
      const float sine = sin(angle);
      dat[i] = -1.0f + 2.0f*sine*sine;
   }
}


void WaveForm::UseSineCubedWave() {
   ASSERT(dat);
   const float max = (float)(datsize - 1);
   const float twopi = 2.0f*M_PI;
   for (int i = 0 ; i < datsize ; ++i) {
      const float step = (float)i;
      const float percent = step/max;
      const float angle = percent*twopi;
      const float sine = sin(angle);
      dat[i] = sine*sine*sine;
   }
}



void WaveForm::UseSinePentaWave() {
   ASSERT(dat);
   const float max = (float)(datsize - 1);
   const float twopi = 2.0f*M_PI;
   for (int i = 0 ; i < datsize ; ++i) {
      const float step = (float)i;
      const float percent = step/max;
      const float angle = percent*twopi;
      const float sine = sin(angle);
      dat[i] = sine*sine*sine*sine*sine;
   }
}



void WaveForm::UseSawWave() {
   ASSERT(dat);
   const float max = (float)(datsize - 1);
   for (int i = 0 ; i < datsize ; ++i) {
      const float step = (float)i;
      const float percent = step/max;
      float value = -1.0f + 4.0f*percent;
      if (value > 1.0f) {
         value = 2.0f - value;
//            value = 1.0f - (value - 1.0f);
      }
      dat[i] = value;
   }
}



void WaveForm::UseSquareWave() {
   for (int i = 0 ; i < datsize/2 ; ++i) {
      dat[i] = 1.0f;
   }
   for (int i = datsize/2 ; i < datsize ; ++i) {
      dat[i] = -1.0f;
   }
}



void WaveForm::UseArcSineWave() {
   ASSERT(dat);
   const float max = (float)(datsize/2);
   const float piovertwo = M_PI/2.0f;
   for (int i = 0 ; i <= datsize/2 ; ++i) {
      const float percent = ((float)i)/max;
      const float val = -1.0f + percent*2.0f;
      const float arcsine = asin(val);
      const float asval = arcsine/piovertwo;
      dat[i] = asval;
   }
   for (int i = datsize/2 ; i < datsize ; ++i) {
      // datsize/2 - (i - datsize/2) // datsize - i
      dat[i] = dat[datsize - i];
   }
}




/// ---------------------------------- VolumeForm class -------------------------------------




VolumeForm::VolumeForm(int size) :
      FloatBuffer(size)
{}



bool VolumeForm::Verify() {
   float max = GetMax();
   float min = GetMin();
   ASSERT(max <= 1.0f);
   ASSERT(min >= 0.0f);
   if ((max > 1.0f) || (min < 0.0f)) {return false;}
   return true;
}



void VolumeForm::UseLevelVolume(float percent_max) {
   ASSERT(dat);
   ASSERT(percent_max >= 0.0f);
   ASSERT(percent_max <= 1.0f);
   for (int i = 0 ; i < datsize ; ++i) {
      dat[i] = percent_max;
   }
}



void VolumeForm::UseLinearFadeIn() {
   ASSERT(dat);
   const float max = (float)(datsize - 1);
   for (int i = 0 ; i < datsize ; ++i) {
      const float percent = ((float)i)/max;
      dat[i] = percent;
   }
}



void VolumeForm::UseLinearFadeOut() {
   ASSERT(dat);
   const float max = (float)(datsize - 1);
   for (int i = 0 ; i < datsize ; ++i) {
      const float percent = ((float)i)/max;
      dat[i] = 1.0f - percent;
   }
}



/// ------------------- Functions for making sounds -----------------------------------------



SAMPLE* MakeSound(const WaveForm& wf , const VolumeForm& vf , float sound_freq , float duration ,
                  int bits , int stereo , int samples_per_sec) {
   ASSERT(wf.Ready());
   ASSERT(vf.Ready());
   ASSERT(sound_freq >= 1.0f);
   ASSERT(duration > 0.0f);
   ASSERT((bits == 8) || (bits == 16));
   ASSERT(samples_per_sec >= 1);
   // create sample
   // find buffer length
   const int num_samples = (int)((float)samples_per_sec*duration);
   ASSERT(num_samples > 1);
   const int bytes_per_sample = (bits/8)*(stereo?2:1);
   const int len = num_samples*bytes_per_sample;
// SAMPLE *create_sample(int bits, int stereo, int freq, int len);
   SAMPLE* sample = create_sample(bits , stereo , samples_per_sec , len);
   if (!sample) {return NULL;}
   // write sample data
   // sample data is always unsigned, when data is stereo then data is interleaved with left value first
   float time = 0.0f;
   float time_percent = 0.0f;
//   float wfpercent = 0.0f;
//   float vfpercent = 0.0f;
//   float value_percent = 0.0f;
   int max_value = 0;
   const int max_samples = num_samples - 1;
   const float wave_duration = 1.0f/sound_freq;
   float wave_percent = 0.0f;
   float wave_value = 0.0f;
   float vol_value = 0.0f;
   int value = 0;
   int index = 0;
   switch (bits) {
      case 8 :
         {
         unsigned char* dat = (unsigned char*)sample->data;
         max_value = 255;
         for (int i = 0 ; i < num_samples ; ++i) {
            time_percent = (float)i/max_samples;
            time = duration*time_percent;
//          double fmod( double x, double y );
            wave_percent = fmod(time , wave_duration);
            wave_value = wf[wave_percent];// -1.0f to 1.0f
            vol_value = vf[time_percent];// 0.0f to 1.0f
            wave_value *= vol_value;// scale wave according to volume
            wave_value = (wave_value + 1.0f)/2.0f;// 0.0f to 1.0f
            value = (int)((float)max_value*wave_value);
            ASSERT(index < len);
            dat[index] = value;
            ++index;
            if (stereo) {
               ASSERT(index < len);
               dat[index] = value;
               ++index;
            }
         }
         break;
         }
      case 16 :
         {
         unsigned short* dat = (unsigned short*)sample->data;
         max_value = 65535;
         for (int i = 0 ; i < num_samples ; ++i) {
            time_percent = (float)i/max_samples;
            time = duration*time_percent;
//          double fmod( double x, double y );
            wave_percent = fmod(time , wave_duration);
            wave_value = wf[wave_percent];// -1.0f to 1.0f
            vol_value = vf[time_percent];// 0.0f to 1.0f
            wave_value *= vol_value;// scale wave according to volume
            wave_value = (wave_value + 1.0f)/2.0f;// 0.0f to 1.0f
            value = (int)((float)max_value*wave_value);
            ASSERT(index < len);
            dat[index] = value;
            ++index;
            if (stereo) {
               ASSERT(index < len);
               dat[index] = value;
               ++index;
            }
         }
         break;
         }
      default :
         return NULL;
   }
   return sample;
}







