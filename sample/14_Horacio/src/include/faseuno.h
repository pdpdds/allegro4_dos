#ifndef FASEUNO_H
#define FASEUNO_H

#include "constantes.h"
#include "trafico.h"
#include "hardware.h"
#include "sprmanager.h"
#include "mensajes.h"
#include "horacio.h"
#include "vehiculo.h"
#include "ambulancia.h"
#include "hiscore.h"
#include "sonido.h"
#include "pause.h"
#include <allegro.h>

class FaseUno
{
	public:
		FaseUno(int Spd);
		~FaseUno();
		void Inicia (int Esquis = 0);
		int Ciclo ();
	private:
        Vehiculo *Inicial;
        Trafico *Traffic;
        Horacio *Horace;
        Ambulancia *Ambulance;
        Pause *Pausa;
        int PuntosInt, Tecla, SetGraf, n, Salir, CiclosHechos, Speed;
        bool PausaFin, MueveHoracio;
        void LeeTeclado ();
        void SacaAmbulancia ();
        void Marcadores ();
        void AjustaPuntos ();
        void InitTemp ();
        int Pausar ();
        
};

#endif // FASEUNO_H

