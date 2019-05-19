#ifndef MENUS_H
#define MENUS_H

#include "hardware.h"
#include "mensajes.h"
#include "horacio.h"
#include "sprmanager.h"
#include "configuracion.h"
#include "hiscore.h"
#include "sonido.h"

class Menus
{
	public:
		Menus();
		~Menus();
		int MenuPpal ();
	private:
        int Altura, Tecla, Salida, CiclosHechos, OpcionAct, n, m, y[6];
        Horacio *Horace;
        void Prepara ();
        int MenuConfig ();
        void PpalTxt ();
        void CfgTxt ();
        void InitTemp ();
        void Desplaza (int Sentido, int OpMax);
        void Creditos ();
};

#endif
