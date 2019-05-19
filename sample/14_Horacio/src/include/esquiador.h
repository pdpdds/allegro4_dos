#ifndef ESQUIADOR_H
#define ESQUIADOR_H

#include "sprite.h"
#include "obstaculo.h"
#include "sprmanager.h"
#include "mensajes.h"
#include "hardware.h"
#include "sonido.h"

class Esquiador : public Sprite
{
	public:
		Esquiador();
		~Esquiador();
		void Avanza (int Tecla, int Speed, int YIndice, bool FinPista, int Margin);
		int Choque (Obstaculo *Indice, int YIndice);
	private:
        int YVirt, YTemp, Cont, Retorno, n;
        Obstaculo *Temp1;
		void Mueve (bool FinPista, int Margin, int YIndice, int Speed);
};

#endif
