#ifndef SONIDO_H
#define SONIDO_H

#include "datos.h"
#include "configuracion.h"
#include <allegro.h>

class Sonido
{
	public:
		Sonido();
		~Sonido();
		void MMenu ();
		void MNivel (int nivel);
		void MStop ();
		void SGolpe ();
		void SSki ();
		void SAmb_ex (bool stop = false);
		void SAmb (bool stop);
		int VerAmb ();
		void SAhh ();
		void SAplauso ();
		void SExtra ();
		void SCash ();
		void SFin ();
	
	private:
        DATAFILE *Fichero;
        SAMPLE *Ski, *Amb;
        bool Sonar;
        int Ambul;
};

#endif
