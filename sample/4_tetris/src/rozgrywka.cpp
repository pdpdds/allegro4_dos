#include "rozgrywka.h"
#include "gra.h"
#include <allegro.h>

void rozgrywka::play()
{
	gra k_gra;

	k_gra.wlacz_allegro();
	//#######__PETLA GLOWNA__##############//
	while (!key[KEY_ESC]) {

		k_gra.resp(k_gra.losuj());

		for (int a = wys_tab; a >= 0; a--) {
			for (int b = 0; b < szer_tab; b++) {
				k_gra.rysuj();
				k_gra.ruch(a, b);
				k_gra.czykoniec();
				k_gra.punkty = k_gra.kasuj_linie(a, b);
			}
		}
	}
	//####################################//
	k_gra.wylacz_allegro();

}
