#ifndef PAUSE_H
#define PAUSE_H

#include "hardware.h"
#include "horacio.h"
#include "mensajes.h"

class Pause
{
	public:
		Pause();
		~Pause();
		int PauseGame ();
	private:
        Horacio *Selector;
        int CiclosHechos, Retorno, SalirBucle, OpcionAct, Tecla;
        void Rectangulo ();
        void Texto ();
        void InitTemp ();
        void Desplaza (int Sentido);

};

#endif // PAUSE_H
