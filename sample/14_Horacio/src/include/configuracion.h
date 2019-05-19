#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include "constantes.h"

using namespace std;


class Configuracion
{
	public:
		Configuracion();
		~Configuracion();
		const int VerIdioma () {return Idioma;}
		void ToggleIdioma ();
		const bool VerSonido () {return Sound;}
		void ToggleSonido ();
		const int SetGraf () {return SetGrafico;}
		void ToggleSet ();
		const bool VerWindowed () {return Windowed;}
		void ToggleWindowed ();
		const int VerSpeed () {return Speed;}
		void SetSpeed ();
		
	private:   
        int Idioma, SetGrafico, Speed;
        bool Sound, Windowed;
        int Save();
};

#endif
