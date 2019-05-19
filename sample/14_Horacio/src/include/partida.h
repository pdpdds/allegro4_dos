#ifndef PARTIDA_H
#define PARTIDA_H

#include "hardware.h"
#include "mensajes.h"
#include "hiscore.h"
#include "sonido.h"
#include "configuracion.h"
#include "faseuno.h"
#include "fasedos.h"

class Partida
{
	public:
		Partida();
		~Partida();
		void Play ();
    private:
        FaseUno *F1;
        FaseDos *F2;
        int Fase1, Fase2, Salida, n;
        void GrabaRecord(int Number);
};

#endif

