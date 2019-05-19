#ifndef VEHICULO_H
#define VEHICULO_H

#include "constantes.h"
#include "sprite.h"
#include "sprmanager.h"
#include "sonido.h"

class Vehiculo : public Sprite 
{
	public:
		Vehiculo(Vehiculo *Final, int Vel);
		~Vehiculo();
		bool AumentaPosicion (Vehiculo* Final, int Ciclos);
		Vehiculo* VerSiguiente ();
		void GrabarSiguiente (Vehiculo* Pointer);
		Vehiculo* VerAnterior ();
		void GrabarAnterior (Vehiculo* Pointer);
		bool MiraEspera ();

	private:       
        int VelAlt, Ciclos0, Speed;
        bool Esperar;
        Vehiculo *Siguiente, *Anterior;

        int ColisionX (Vehiculo *Ciclo);
        bool ColisionY (Vehiculo *Ciclo);
};

#endif
