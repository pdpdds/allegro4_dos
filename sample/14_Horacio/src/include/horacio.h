#ifndef HORACIO_H
#define HORACIO_H

#include "sprite.h" 
#include "vehiculo.h"

class Horacio : public Sprite
{
	public:
		Horacio(int Vel=1);
		~Horacio();
		void ActualizaMov (int Tecla);
		void Mueve ();
        bool Atropello (Vehiculo *Ciclo);
        void SinEsqui ();
        void ConEsqui ();
	private:
	    int Indice;
	    int Speed;
	    int n,m;
	    
};

#endif
