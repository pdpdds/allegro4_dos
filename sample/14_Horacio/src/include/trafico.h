#ifndef TRAFICO_H
#define TRAFICO_H

#include "vehiculo.h"

class Trafico
{
	public:
		Trafico(int Vel);
		~Trafico();
		Vehiculo* Mueve();
        void NuevoVehiculo ();
        int Cuantos ();
        void Dibuja ();
        void Borra();
	private:
        void Destruye ();
        int NumVeh, Ciclos, Speed;
        Vehiculo *Inicial, *Final, *Temp1, *Temp2, *Bucle;
};

#endif
