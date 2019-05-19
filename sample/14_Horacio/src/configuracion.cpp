#include <fstream>
#include "configuracion.h"

Configuracion::Configuracion()
{
 Idioma = ESPANOL;
 Sound = true;
 SetGrafico = 0;
 Windowed = false;

 char config[5];
 ifstream fichin;
 fichin.open ("horacio.cfg");
 if (fichin.good()) {
     fichin.read (config,5);
     fichin.close();
     Idioma = config[0];
     Sound = config[1];
     SetGrafico = config[2];
     Windowed = config[3];
     Speed = config[4];
     }
  else
   Save();
     
}

Configuracion::~Configuracion()
{
}

int Configuracion::Save() {
 char config[5];
 config [0] = Idioma;
 config [1] = Sound;
 config [2] = SetGrafico;
 config [3] = Windowed;
 config [4] = Speed;
 ofstream fichout;
 fichout.open ("horacio.cfg");
 if (fichout.good()) {
     fichout.write (config,5);
     fichout.close();
     return 0;
     }
  else return 1;
    
}
      
void Configuracion::ToggleIdioma (){
     if (++Idioma>ULTLENG) Idioma = 0;
     Save();
     return;
}

void Configuracion::ToggleSonido (){
     Sound = !Sound;
     Save();
}

void Configuracion::ToggleWindowed (){
     Windowed = !Windowed;
     Save();
}

void Configuracion::ToggleSet () {
     if (++SetGrafico>ULTIMOSET)
        SetGrafico = 0;
     Save();
}

void Configuracion::SetSpeed () {
     Speed += 1;
     if (Speed > 6) Speed = 0;
     Save ();
}
