#ifndef FASEDOS_H
#define FASEDOS_H


#include "constantes.h"
#include "pista.h"
#include "esquiador.h"
#include "hardware.h"
#include "mensajes.h"
#include "obstaculo.h"
#include "horacio.h"
#include "hiscore.h"
#include "sonido.h"
#include "pause.h"
#include <allegro.h>

class FaseDos
{
	public:
		FaseDos(int Spd);
		~FaseDos();
		void Inicia();
		int Ciclo ();
    private:
        Pista *Descenso;
        Esquiador *Horace;
        Obstaculo *Indice;
        Pause *Pausa;
        int Speed, Salida, Tecla, YIndice, Margin, Choque, CiclosHechos, n;
        bool FinPista;
        void Marcadores ();
        void InitTemp ();
        void Choca();
        int Pausar();
};

#endif
