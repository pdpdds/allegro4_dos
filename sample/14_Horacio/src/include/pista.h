#ifndef PISTA_H
#define PISTA_H

#include "constantes.h"
#include "hardware.h"
#include "obstaculo.h"

const int NivelMax = 3;

class Pista
{
	public:
		Pista();
		~Pista();
		void Init();
		void DeInit();
		bool Avanza (int Speed, int &Indice, int &Margin);
		void Dibuja ();
		void Borra ();
		void SetMargin (int Margin);
		Obstaculo *Indice ();
	private:
        Obstaculo  *Inicial, *Ind, *Temp1, *Temp2;
        int NumObs[NivelMax+1], EndPista [NivelMax+1], Punt, YInd, FinPista, Margen, NivelTemp, n;
};

#endif

