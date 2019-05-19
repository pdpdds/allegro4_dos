/*************************************************************************
Horacio Esquiador - Horace goes skiing
(c) 1.982 Beam Software
Original code written by William Tang
Remake by Luis I. Garc? Ventura
Developed with Dev-C++ 4.9.9.2 & Allegro 4.2
http://www.bloodshed.net
http://alleg.sourceforge.net

Este es el primer juego de ordenador al que jugu?en mi vida, en 1.983
Dedicado a mi padre, Jose A. Garc?.

This is the first computer game that I played, in 1.983
Dedicated to my father, Jose A. Garc?.

***************************************************************************/

#include "main.h"

int main(int argc, char *argv[])
{
	Retorno = 0;
	ResMenu = 0;
	Config = new Configuracion;

	H = new Hardware;
	if (H->InitGraph() == 0) {
		H->Init();

		SFX = new Sonido;
		M = new SprManager;
		SMS = new Mensajes;
		Menu = new Menus;
		Record = new HiScore;

		M->Init();

		while (ResMenu != ABORT) {
			H->ClearKeyBuf();
			ResMenu = Menu->MenuPpal();

			if (ResMenu == FINFASEOK) {
				Game = new Partida;
				Game->Play();
				delete Game;
			}

		}

		delete Menu;
		delete Record;
		delete SMS;
		delete M;
		delete SFX;
	}
	else Retorno = 1;

	delete H;
	delete Config;

	exit(0);

	return Retorno;
}

END_OF_MAIN();
