#include "gra.h"
#include "silnik.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <allegro.h>

silnik::silnik()
{
	numer_klocka = 0;
	flag = true;
	punktacja = 0;
}

int silnik::kasuj_linie(int a, int b)
{
	if ((mapa[a][b] == 5) && (mapa[a][b + 1] == 5) && (mapa[a][b + 2] == 5) && (mapa[a][b + 3] == 5) && (mapa[a][b + 4] == 5) && (mapa[a][b + 5] == 5) && (mapa[a][b + 6] == 5) && (mapa[a][b + 7] == 5) && (mapa[a][b + 8] == 5) && (mapa[a][b + 9] == 5))
	{
		for (int k = 1; k < szer_tab - 1; k++) {
			mapa[a][k] = 0;
		}

		for (int a = wys_tab; a >= 0; a--) {
			for (int b = 0; b < szer_tab; b++) {
				if (mapa[a][b] == 5 && mapa[a + 1][b] == 0) { mapa[a + 1][b] = 5; mapa[a][b] = 0; }
			}
		}

		return punktacja = punktacja + 5;
	}
}

void silnik::odpal_klocek()
{

	switch (numer_klocka)
	{

	case 1: {      // kreska
		mapa[0][5] = 3;
		mapa[1][5] = 3;
		mapa[2][5] = 3;
		mapa[3][5] = 4;
		break; }

	case 2: {      //litera t
		mapa[0][4] = 3;
		mapa[0][5] = 3;
		mapa[0][6] = 3;
		mapa[1][5] = 4;
		break; }

	case 3: {    //kwadrat
		mapa[0][5] = 3;
		mapa[0][6] = 3;
		mapa[1][5] = 3;
		mapa[1][6] = 4;
		break; }

	case 4: {     //L
		mapa[0][5] = 3;
		mapa[1][5] = 3;
		mapa[2][5] = 3;
		mapa[2][6] = 4;
		break; }

	case 5: {    //odwrocone L
		mapa[0][5] = 3;
		mapa[1][5] = 3;
		mapa[2][5] = 4;
		mapa[2][4] = 3;
		break; }

	case 6: {    //  s
		mapa[0][5] = 3;
		mapa[1][5] = 4;
		mapa[0][6] = 3;
		mapa[1][4] = 3;
		break; }

	case 7: {    // z
		mapa[0][5] = 3;
		mapa[1][5] = 3;
		mapa[0][4] = 3;
		mapa[1][6] = 4; }
			break;
	}

}

void silnik::ruch(int a, int b)
{

	if (mapa[a][b] == 4 && mapa[a + 1][b] == 0) { warunek_spadania(a, b); }

	if (key[KEY_SPACE]) { obracaj(); rest(10); }
	if (key[KEY_RIGHT]) { w_prawo(); }
	if (key[KEY_LEFT]) { w_lewo(); }
	if (zamiana_na_czerwony(a, b)) { flag = true; }                   // jesli klocek sie zatrzyma , zamienia sie na czerwony oraz ustawia flag na true = pojawia sie nowy klocek
}

bool silnik::zamiana_na_czerwony(int a, int b)
{
	if ((mapa[a][b] == 4 || mapa[a][b] == 3) && (mapa[a + 1][b] == 1 || mapa[a + 1][b] == 5)) {

		for (int k = 0; k < wys_tab; k++) {
			for (int g = 0; g < szer_tab; g++) {

				if (mapa[k][g] == 4 || mapa[k][g] == 3) mapa[k][g] = 5;
			}
		}


		return true;
	}
	return false;
}

void silnik::czykoniec()
{

	for (int p = 0; p < szer_tab; p++) { if (mapa[0][p] == 5) { allegro_message("koniec gry, przegrales"); exit(1); } }
}

int silnik::losuj()
{
	srand(time(NULL));
	return ((rand() % 7) + 1);

}

void silnik::resp(int nr)
{

	if (flag) { numer_klocka = nr; odpal_klocek(); flag = false; }

}

void silnik::warunek_spadania(int a, int b)
{

	switch (numer_klocka)
	{

	case 1:
	{       mapa[a + 1][b] = mapa[a][b];  mapa[a][b] = 0;
	mapa[a][b] = mapa[a - 1][b]; mapa[a - 1][b] = 0;
	mapa[a - 1][b] = mapa[a - 2][b]; mapa[a - 2][b] = 0;
	mapa[a - 2][b] = mapa[a - 3][b]; mapa[a - 3][b] = 0;
	break; }
	case 11:
	{
		if (mapa[a + 1][b] == 0 && mapa[a + 1][b - 1] == 0 && mapa[a + 1][b - 2] == 0 && mapa[a + 1][b - 3] == 0) {

			mapa[a + 1][b] = mapa[a][b];   mapa[a][b] = 0;
			mapa[a + 1][b - 1] = mapa[a][b - 1]; mapa[a][b - 1] = 0;
			mapa[a + 1][b - 2] = mapa[a][b - 2]; mapa[a][b - 2] = 0;
			mapa[a + 1][b - 3] = mapa[a][b - 3]; mapa[a][b - 3] = 0;
		}           break; }
	case 2:
	{
		if (mapa[a][b - 1] == 0 && mapa[a][b + 1] == 0 && mapa[a + 1][b] == 0) {
			mapa[a + 1][b] = mapa[a][b]; mapa[a][b] = 0;
			mapa[a][b - 1] = mapa[a - 1][b - 1]; mapa[a - 1][b - 1] = 0;
			mapa[a][b + 1] = mapa[a - 1][b + 1]; mapa[a - 1][b + 1] = 0;
			mapa[a][b] = mapa[a - 1][b]; mapa[a - 1][b] = 0;
		}    break; }

	case 22:
	{
		if (mapa[a + 1][b] == 0 && mapa[a][b - 1] == 0) {
			mapa[a + 1][b] = mapa[a][b]; mapa[a][b] = 0;
			mapa[a][b - 1] = mapa[a - 1][b - 1]; mapa[a - 1][b - 1] = 0;
			mapa[a][b] = mapa[a - 1][b]; mapa[a - 1][b] = 0;
			mapa[a - 1][b] = mapa[a - 2][b]; mapa[a - 2][b] = 0;
		}    break; }

	case 222:
	{
		if (mapa[a + 1][b] == 0 && mapa[a + 1][b - 1] == 0 && mapa[a + 1][b - 2] == 0) {
			mapa[a + 1][b] = mapa[a][b]; mapa[a][b] = 0;
			mapa[a + 1][b - 1] = mapa[a][b - 1]; mapa[a][b - 1] = 0;
			mapa[a + 1][b - 2] = mapa[a][b - 2]; mapa[a][b - 2] = 0;
			mapa[a][b - 1] = mapa[a - 1][b - 1]; mapa[a - 1][b - 1] = 0;
		}    break; }

	case 2222:
	{
		if (mapa[a + 1][b] == 0 && mapa[a][b + 1] == 0) {
			mapa[a + 1][b] = mapa[a][b]; mapa[a][b] = 0;
			mapa[a][b] = mapa[a - 1][b]; mapa[a - 1][b] = 0;
			mapa[a - 1][b] = mapa[a - 2][b]; mapa[a - 2][b] = 0;
			mapa[a][b + 1] = mapa[a - 1][b + 1]; mapa[a - 1][b + 1] = 0;
		}    break; }

	case 3:
	{
		if (mapa[a + 1][b] == 0 && mapa[a + 1][b - 1] == 0) {
			mapa[a + 1][b] = mapa[a][b]; mapa[a][b] = 0;
			mapa[a + 1][b - 1] = mapa[a][b - 1]; mapa[a][b - 1] = 0;
			mapa[a][b] = mapa[a - 1][b]; mapa[a - 1][b] = 0;
			mapa[a][b - 1] = mapa[a - 1][b - 1]; mapa[a - 1][b - 1] = 0;
		}break; }

	case 4:
	{
		if (mapa[a + 1][b] == 0 && mapa[a + 1][b - 1] == 0) {
			mapa[a + 1][b] = mapa[a][b]; mapa[a][b] = 0;
			mapa[a + 1][b - 1] = mapa[a][b - 1]; mapa[a][b - 1] = 0;
			mapa[a][b - 1] = mapa[a - 1][b - 1]; mapa[a - 1][b - 1] = 0;
			mapa[a - 1][b - 1] = mapa[a - 2][b - 1]; mapa[a - 2][b - 1] = 0;
		}break; }

	case 44:
	{
		if (mapa[a + 1][b] == 0 && mapa[a][b + 1] == 0 && mapa[a][b + 2] == 0) {
			mapa[a + 1][b] = mapa[a][b]; mapa[a][b] = 0;
			mapa[a][b + 1] = mapa[a - 1][b + 1]; mapa[a - 1][b + 1] = 0;
			mapa[a][b + 2] = mapa[a - 1][b + 2]; mapa[a - 1][b + 2] = 0;
			mapa[a][b] = mapa[a - 1][b]; mapa[a - 1][b] = 0;
		}break; }

	case 444:
	{
		if (mapa[a + 1][b] == 0 && mapa[a - 1][b - 1] == 0) {
			mapa[a + 1][b] = mapa[a][b]; mapa[a][b] = 0;
			mapa[a][b] = mapa[a - 1][b]; mapa[a - 1][b] = 0;
			mapa[a - 1][b] = mapa[a - 2][b]; mapa[a - 2][b] = 0;
			mapa[a - 1][b - 1] = mapa[a - 2][b - 1]; mapa[a - 2][b - 1] = 0;
		}break; }

	case 4444:
	{
		if (mapa[a + 1][b] == 0 && mapa[a + 1][b - 1] == 0 && mapa[a + 1][b - 2] == 0) {
			mapa[a + 1][b] = mapa[a][b]; mapa[a][b] = 0;
			mapa[a + 1][b - 1] = mapa[a][b - 1]; mapa[a][b - 1] = 0;
			mapa[a + 1][b - 2] = mapa[a][b - 2]; mapa[a][b - 2] = 0;
			mapa[a][b] = mapa[a - 1][b]; mapa[a - 1][b] = 0;
		}break; }

	case 5:
	{
		if (mapa[a + 1][b] == 0 && mapa[a + 1][b - 1] == 0) {
			mapa[a + 1][b] = mapa[a][b]; mapa[a][b] = 0;
			mapa[a + 1][b - 1] = mapa[a][b - 1]; mapa[a][b - 1] = 0;
			mapa[a][b] = mapa[a - 1][b]; mapa[a - 1][b] = 0;
			mapa[a - 1][b] = mapa[a - 2][b]; mapa[a - 2][b] = 0;
		}    break; }

	case 55:
	{
		if (mapa[a + 1][b] == 0 && mapa[a + 1][b - 1] == 0 && mapa[a + 1][b - 2] == 0) {
			mapa[a + 1][b] = mapa[a][b]; mapa[a][b] = 0;
			mapa[a + 1][b - 1] = mapa[a][b - 1]; mapa[a][b - 1] = 0;
			mapa[a + 1][b - 2] = mapa[a][b - 2]; mapa[a][b - 2] = 0;
			mapa[a][b - 2] = mapa[a - 1][b - 2]; mapa[a - 1][b - 2] = 0;
		}    break; }
	case 555:
	{
		if (mapa[a + 1][b] == 0 && mapa[a - 1][b + 1] == 0) {
			mapa[a + 1][b] = mapa[a][b]; mapa[a][b] = 0;
			mapa[a][b] = mapa[a - 1][b]; mapa[a - 1][b] = 0;
			mapa[a - 1][b] = mapa[a - 2][b]; mapa[a - 2][b] = 0;
			mapa[a - 1][b + 1] = mapa[a - 2][b + 1]; mapa[a - 2][b + 1] = 0;
		}    break; }

	case 5555:
	{
		if (mapa[a + 1][b] == 0 && mapa[a][b - 1] == 0 && mapa[a][b - 2] == 0) {
			mapa[a + 1][b] = mapa[a][b]; mapa[a][b] = 0;
			mapa[a][b - 1] = mapa[a - 1][b - 1]; mapa[a - 1][b - 1] = 0;
			mapa[a][b - 2] = mapa[a - 1][b - 2]; mapa[a - 1][b - 2] = 0;
			mapa[a][b] = mapa[a - 1][b]; mapa[a - 1][b] = 0;
		}    break; }

	case 6:
	{
		if (mapa[a + 1][b] == 0 && mapa[a + 1][b - 1] == 0 && mapa[a][b + 1] == 0) {
			mapa[a + 1][b] = mapa[a][b]; mapa[a][b] = 0;
			mapa[a + 1][b - 1] = mapa[a][b - 1]; mapa[a][b - 1] = 0;
			mapa[a][b] = mapa[a - 1][b]; mapa[a - 1][b] = 0;
			mapa[a][b + 1] = mapa[a - 1][b + 1]; mapa[a - 1][b + 1] = 0;
		}    break; }

	case 66:
	{
		if (mapa[a + 1][b] == 0 && mapa[a][b - 1] == 0) {
			mapa[a + 1][b] = mapa[a][b]; mapa[a][b] = 0;
			mapa[a][b] = mapa[a - 1][b]; mapa[a - 1][b] = 0;
			mapa[a][b - 1] = mapa[a - 1][b - 1]; mapa[a - 1][b - 1] = 0;
			mapa[a - 1][b - 1] = mapa[a - 2][b - 1]; mapa[a - 2][b - 1] = 0;
		}    break; }

	case 7:
	{
		if (mapa[a + 1][b] == 0 && mapa[a + 1][b - 1] == 0 && mapa[a][b - 2] == 0) {
			mapa[a + 1][b] = mapa[a][b]; mapa[a][b] = 0;
			mapa[a + 1][b - 1] = mapa[a][b - 1]; mapa[a][b - 1] = 0;
			mapa[a][b - 1] = mapa[a - 1][b - 1]; mapa[a - 1][b - 1] = 0;
			mapa[a][b - 2] = mapa[a - 1][b - 2]; mapa[a - 1][b - 2] = 0;
		}    break; }
	case 77:
	{
		if (mapa[a + 1][b] == 0 && mapa[a][b + 1] == 0) {
			mapa[a + 1][b] = mapa[a][b]; mapa[a][b] = 0;
			mapa[a][b] = mapa[a - 1][b]; mapa[a - 1][b] = 0;
			mapa[a][b + 1] = mapa[a - 1][b + 1]; mapa[a - 1][b + 1] = 0;
			mapa[a - 1][b + 1] = mapa[a - 2][b + 1]; mapa[a - 2][b + 1] = 0;
		}    break; }

	}
}

void silnik::obracaj()
{
	rest(50);
	int y, x;
	for (int k = 0; k < wys_tab; k++) {
		for (int l = 0; l < szer_tab; l++) {
			if (mapa[k][l] == 3) { y = k; x = l; k = wys_tab; l = szer_tab; }
		}
	}

	if (numer_klocka == 1 && mapa[y][x - 3] == 0 && mapa[y][x - 2] == 0 && mapa[y][x - 1] == 0) {

		mapa[y][x - 3] = mapa[y][x];   mapa[y][x] = 0;
		mapa[y][x - 2] = mapa[y + 1][x]; mapa[y + 1][x] = 0;
		mapa[y][x - 1] = mapa[y + 2][x]; mapa[y + 2][x] = 0;
		mapa[y][x] = mapa[y + 3][x]; mapa[y + 3][x] = 0;
		chwilowy_numer = 11;
	}

	if (numer_klocka == 11 && mapa[y + 3][x + 3] == 0 && mapa[y + 2][x + 3] == 0 && mapa[y + 1][x + 3] == 0) {
		mapa[y + 3][x + 3] = mapa[y][x + 3]; mapa[y][x + 3] = 0;
		mapa[y + 2][x + 3] = mapa[y][x + 2]; mapa[y][x + 2] = 0;
		mapa[y + 1][x + 3] = mapa[y][x + 1]; mapa[y][x + 1] = 0;
		mapa[y][x + 3] = mapa[y][x];   mapa[y][x] = 0;
		chwilowy_numer = 1;
	}

	if (numer_klocka == 2 && mapa[y + 1][x] == 0 && mapa[y + 2][x + 1] == 0) {
		mapa[y][x] = 0;
		mapa[y][x + 2] = 0;
		mapa[y + 1][x + 1] = 3;
		mapa[y + 2][x + 1] = 4;
		mapa[y + 1][x] = 3;
		chwilowy_numer = 22;
	}

	if (numer_klocka == 22 && mapa[y + 1][x + 1] == 0)
	{
		mapa[y + 1][x + 1] = 4;
		mapa[y + 2][x] = 0;
		chwilowy_numer = 222;
	}

	if (numer_klocka == 222 && mapa[y + 2][x] == 0)
	{
		mapa[y + 1][x - 1] = 0;
		mapa[y + 1][x + 1] = 3;
		mapa[y + 2][x] = 4;
		chwilowy_numer = 2222;
	}

	if (numer_klocka == 2222 && mapa[y + 1][x - 1] == 0)
	{
		mapa[y][x] = 0;
		mapa[y + 1][x - 1] = 3;
		chwilowy_numer = 2;
	}

	if (numer_klocka == 3)
	{
		//debugger//
		chwilowy_numer = 3;
	}

	if (numer_klocka == 4 && mapa[y + 2][x - 1] == 0 && mapa[y + 3][x - 1] == 0)
	{
		mapa[y][x] = 0;
		mapa[y + 1][x] = 0;
		mapa[y + 2][x + 1] = 3;
		mapa[y + 2][x - 1] = 3;
		mapa[y + 3][x - 1] = 4;
		chwilowy_numer = 44;
	}

	if (numer_klocka == 44 && mapa[y + 1][x + 2] == 0 && mapa[y + 2][x + 2] == 0)
	{
		mapa[y][x] = 0;
		mapa[y + 1][x] = 0;
		mapa[y + 1][x + 2] = 3;
		mapa[y + 2][x + 2] = 4;
		chwilowy_numer = 444;
	}

	if (numer_klocka == 444 && mapa[y + 2][x] == 0 && mapa[y + 2][x - 1] == 0)
	{
		mapa[y][x] = 0;
		mapa[y][x + 1] = 0;
		mapa[y + 2][x] = 3;
		mapa[y + 2][x - 1] = 3;
		chwilowy_numer = 4444;
	}

	if (numer_klocka == 4444 && mapa[y][x - 1] == 0 && mapa[y - 1][x - 1] == 0)
	{
		mapa[y][x] = 0;
		mapa[y + 1][x - 2] = 0;
		mapa[y][x - 1] = 3;
		mapa[y - 1][x - 1] = 3;
		chwilowy_numer = 4;
	}

	if (numer_klocka == 5 && mapa[y + 1][x - 2] == 0 && mapa[y + 2][x - 2] == 0)
	{
		mapa[y][x] = 0;
		mapa[y + 1][x] = 0;
		mapa[y + 1][x - 2] = 3;
		mapa[y + 2][x - 2] = 3;
		chwilowy_numer = 55;
	}

	if (numer_klocka == 55 && mapa[y + 2][x] == 0 && mapa[y][x + 1] == 0)
	{
		mapa[y + 1][x + 1] = 0;
		mapa[y + 1][x + 2] = 0;
		mapa[y][x] = 3;
		mapa[y + 2][x] = 4;
		mapa[y][x + 1] = 3;
		chwilowy_numer = 555;
	}

	if (numer_klocka == 555 && mapa[y][x - 1] == 0 && mapa[y + 1][x + 1] == 0)
	{
		mapa[y + 1][x] = 0;
		mapa[y + 2][x] = 0;
		mapa[y][x - 1] = 3;
		mapa[y + 1][x + 1] = 4;
		chwilowy_numer = 5555;
	}

	if (numer_klocka == 5555 && mapa[y + 1][x + 1] == 0 && mapa[y - 1][x + 2] == 0)
	{
		mapa[y][x] = 0;
		mapa[y][x + 1] = 0;
		mapa[y - 1][x + 2] = 3;
		mapa[y + 1][x + 1] = 3;
		chwilowy_numer = 5;
	}

	if (numer_klocka == 6 && mapa[y + 1][x + 1] == 0 && mapa[y + 2][x + 1] == 0)
	{
		mapa[y + 1][x - 1] = 0;
		mapa[y][x + 1] = 0;
		mapa[y + 1][x] = 3;
		mapa[y + 1][x + 1] = 3;
		mapa[y + 2][x + 1] = 4;
		chwilowy_numer = 66;
	}

	if (numer_klocka == 66 && mapa[y + 2][x] == 0 && mapa[y + 2][x - 1] == 0)
	{
		mapa[y][x] = 0;
		mapa[y + 2][x + 1] = 0;
		mapa[y + 2][x] = 4;
		mapa[y + 2][x - 1] = 3;
		chwilowy_numer = 6;
	}

	if (numer_klocka == 7 && mapa[y + 1][x] == 0 && mapa[y + 2][x] == 0)
	{
		mapa[y][x] = 0;
		mapa[y + 1][x + 2] = 0;
		mapa[y + 1][x] = 3;
		mapa[y + 2][x] = 4;
		chwilowy_numer = 77;
	}

	if (numer_klocka == 77 && mapa[y][x - 1] == 0 && mapa[y + 1][x + 1] == 0)
	{
		mapa[y + 1][x - 1] = 0;
		mapa[y + 2][x - 1] = 0;
		mapa[y][x - 1] = 3;
		mapa[y + 1][x + 1] = 4;
		chwilowy_numer = 7;
	}

	numer_klocka = chwilowy_numer;
}

void silnik::w_prawo()
{

	switch (numer_klocka)
	{

	case 1:
	{
		for (int a = 0; a < wys_tab; a++) {
			for (int b = szer_tab; b > 0; b--) {
				if ((mapa[a][b] == 3) && (mapa[a][b + 1] == 0 && mapa[a + 1][b + 1] == 0 && mapa[a + 2][b + 1] == 0 && mapa[a + 3][b + 1] == 0))
				{
					mapa[a][b + 1] = mapa[a][b];   mapa[a][b] = 0;
					mapa[a + 1][b + 1] = mapa[a + 1][b];   mapa[a + 1][b] = 0;
					mapa[a + 2][b + 1] = mapa[a + 2][b];   mapa[a + 2][b] = 0;
					mapa[a + 3][b + 1] = mapa[a + 3][b];   mapa[a + 3][b] = 0;
					a = wys_tab;
				}
			}
		}  break; }

	case 11:
	{for (int a = 0; a < wys_tab; a++) {
		for (int b = szer_tab; b > 0; b--) {
			if ((mapa[a][b] == 4) && (mapa[a][b + 1] == 0))
			{
				mapa[a][b + 1] = mapa[a][b];   mapa[a][b] = 0;
				mapa[a][b] = mapa[a][b - 1];   mapa[a][b - 1] = 0;
				mapa[a][b - 1] = mapa[a][b - 2];   mapa[a][b - 2] = 0;
				mapa[a][b - 2] = mapa[a][b - 3];   mapa[a][b - 3] = 0;
				a = wys_tab;
			}
		}
	}    break; }
	case 2:
	{
		for (int a = 0; a < wys_tab; a++) {
			for (int b = szer_tab; b > 0; b--) {
				if ((mapa[a][b] == 3) && (mapa[a][b + 1] == 0 && mapa[a + 1][b] == 0))
				{
					mapa[a][b + 1] = mapa[a][b];   mapa[a][b] = 0;
					mapa[a][b] = mapa[a][b - 1];   mapa[a][b - 1] = 0;
					mapa[a][b - 1] = mapa[a][b - 2]; mapa[a][b - 2] = 0;
					mapa[a + 1][b] = mapa[a + 1][b - 1];   mapa[a + 1][b - 1] = 0;
					a = wys_tab;
				}
			}
		}
		break;
	}
	case 22:
	{
		for (int a = 0; a < wys_tab; a++) {
			for (int b = szer_tab; b > 0; b--) {
				if ((mapa[a][b] == 3) && (mapa[a][b + 1] == 0 && mapa[a + 1][b + 1] == 0 && mapa[a + 2][b + 1] == 0))
				{
					mapa[a][b + 1] = mapa[a][b];   mapa[a][b] = 0;
					mapa[a + 1][b + 1] = mapa[a + 1][b];   mapa[a + 1][b] = 0;
					mapa[a + 1][b] = mapa[a + 1][b - 1]; mapa[a + 1][b - 1] = 0;
					mapa[a + 2][b + 1] = mapa[a + 2][b];   mapa[a + 2][b] = 0;
					a = wys_tab;
				}
			}
		}
		break;
	}

	case 222:
	{
		for (int a = 0; a < wys_tab; a++) {
			for (int b = szer_tab; b > 0; b--) {
				if ((mapa[a][b] == 3) && (mapa[a + 1][b + 2] == 0 && mapa[a][b + 1] == 0))
				{
					mapa[a][b + 1] = mapa[a][b];   mapa[a][b] = 0;
					mapa[a + 1][b + 2] = mapa[a + 1][b + 1];   mapa[a + 1][b + 1] = 0;
					mapa[a + 1][b + 1] = mapa[a + 1][b]; mapa[a + 1][b] = 0;
					mapa[a + 1][b] = mapa[a + 1][b - 1];   mapa[a + 1][b - 1] = 0;
					a = wys_tab;
				}
			}
		}
		break;
	}

	case 2222:
	{
		for (int a = 0; a < wys_tab; a++) {
			for (int b = szer_tab; b > 0; b--) {
				if ((mapa[a][b] == 3) && (mapa[a][b + 1] == 0 && mapa[a + 2][b + 1] == 0 && mapa[a + 1][b + 2] == 0))
				{
					mapa[a][b + 1] = mapa[a][b];   mapa[a][b] = 0;
					mapa[a + 1][b + 2] = mapa[a + 1][b + 1];   mapa[a + 1][b + 1] = 0;
					mapa[a + 1][b + 1] = mapa[a + 1][b]; mapa[a + 1][b] = 0;
					mapa[a + 2][b + 1] = mapa[a + 2][b];   mapa[a + 2][b] = 0;
					a = wys_tab;
				}
			}
		}
		break;
	}


	case 3:
	{for (int a = 0; a < wys_tab; a++) {
		for (int b = szer_tab; b > 0; b--) {
			if (mapa[a][b] == 3 && mapa[a][b + 1] == 0 && mapa[a + 1][b + 1] == 0)
			{
				mapa[a][b + 1] = mapa[a][b]; mapa[a][b] = 0;
				mapa[a][b] = mapa[a][b - 1]; mapa[a][b - 1] = 0;
				mapa[a + 1][b + 1] = mapa[a + 1][b]; mapa[a + 1][b] = 0;
				mapa[a + 1][b] = mapa[a + 1][b - 1]; mapa[a + 1][b - 1] = 0;   a = wys_tab;
			}
		}
	}break;    }

	case 4:
	{for (int a = 0; a < wys_tab; a++) {
		for (int b = szer_tab; b > 0; b--) {
			if (mapa[a][b] == 3 && mapa[a][b + 1] == 0 && mapa[a + 1][b + 1] == 0 && mapa[a + 2][b + 2] == 0)
			{
				mapa[a][b + 1] = mapa[a][b]; mapa[a][b] = 0;
				mapa[a + 1][b + 1] = mapa[a + 1][b]; mapa[a + 1][b] = 0;
				mapa[a + 2][b + 2] = mapa[a + 2][b + 1]; mapa[a + 2][b + 1] = 0;
				mapa[a + 2][b + 1] = mapa[a + 2][b]; mapa[a + 2][b] = 0;   a = wys_tab;
			}
		}
	}break;    }

	case 44:
	{for (int a = 0; a < wys_tab; a++) {
		for (int b = szer_tab; b > 0; b--) {
			if (mapa[a][b] == 3 && mapa[a][b + 1] == 0 && mapa[a + 1][b - 1] == 0)
			{
				mapa[a][b + 1] = mapa[a][b]; mapa[a][b] = 0;
				mapa[a][b] = mapa[a][b - 1]; mapa[a][b - 1] = 0;
				mapa[a][b - 1] = mapa[a][b - 2]; mapa[a][b - 2] = 0;
				mapa[a + 1][b - 1] = mapa[a + 1][b - 2]; mapa[a + 1][b - 2] = 0;   a = wys_tab;
			}
		}
	}break;    }
	case 444:
	{for (int a = 0; a < wys_tab; a++) {
		for (int b = szer_tab; b > 0; b--) {
			if (mapa[a][b] == 3 && mapa[a][b + 1] == 0 && mapa[a + 1][b + 1] == 0 && mapa[a + 2][b + 1] == 0)
			{
				mapa[a][b + 1] = mapa[a][b]; mapa[a][b] = 0;
				mapa[a][b] = mapa[a][b - 1]; mapa[a][b - 1] = 0;
				mapa[a + 1][b + 1] = mapa[a + 1][b]; mapa[a + 1][b] = 0;
				mapa[a + 2][b + 1] = mapa[a + 2][b]; mapa[a + 2][b] = 0;   a = wys_tab;
			}
		}
	}break;    }

	case 4444:
	{for (int a = 0; a < wys_tab; a++) {
		for (int b = szer_tab; b > 0; b--) {
			if (mapa[a][b] == 3 && mapa[a][b + 1] == 0 && mapa[a + 1][b + 1] == 0)
			{
				mapa[a][b + 1] = mapa[a][b]; mapa[a][b] = 0;
				mapa[a + 1][b + 1] = mapa[a + 1][b]; mapa[a + 1][b] = 0;
				mapa[a + 1][b] = mapa[a + 1][b - 1]; mapa[a + 1][b - 1] = 0;
				mapa[a + 1][b - 1] = mapa[a + 1][b - 2]; mapa[a + 1][b - 2] = 0;   a = wys_tab;
			}
		}
	}break;    }

	case 5:
	{for (int a = 0; a < wys_tab; a++) {
		for (int b = szer_tab; b > 0; b--) {
			if (mapa[a][b] == 3 && mapa[a][b + 1] == 0 && mapa[a + 1][b + 1] == 0 && mapa[a + 2][b + 1] == 0)
			{
				mapa[a][b + 1] = mapa[a][b]; mapa[a][b] = 0;
				mapa[a + 1][b + 1] = mapa[a + 1][b]; mapa[a + 1][b] = 0;
				mapa[a + 2][b + 1] = mapa[a + 2][b]; mapa[a + 2][b] = 0;
				mapa[a + 2][b] = mapa[a + 2][b - 1]; mapa[a + 2][b - 1] = 0;   a = wys_tab;
			}
		}
	}break;    }

	case 55:
	{for (int a = 0; a < wys_tab; a++) {
		for (int b = szer_tab; b > 0; b--) {
			if (mapa[a][b] == 3 && mapa[a][b + 1] == 0 && mapa[a + 1][b + 3] == 0)
			{
				mapa[a][b + 1] = mapa[a][b]; mapa[a][b] = 0;
				mapa[a + 1][b + 3] = mapa[a + 1][b + 2]; mapa[a + 1][b + 2] = 0;
				mapa[a + 1][b + 2] = mapa[a + 1][b + 1]; mapa[a + 1][b + 1] = 0;
				mapa[a + 1][b + 1] = mapa[a + 1][b]; mapa[a + 1][b] = 0;   a = wys_tab;
			}
		}
	}break;    }

	case 555:
	{for (int a = 0; a < wys_tab; a++) {
		for (int b = szer_tab; b > 0; b--) {
			if (mapa[a][b] == 3 && mapa[a][b + 1] == 0 && mapa[a + 1][b] == 0 && mapa[a + 2][b] == 0)
			{
				mapa[a][b + 1] = mapa[a][b]; mapa[a][b] = 0;
				mapa[a][b] = mapa[a][b - 1]; mapa[a][b - 1] = 0;
				mapa[a + 1][b] = mapa[a + 1][b - 1]; mapa[a + 1][b - 1] = 0;
				mapa[a + 2][b] = mapa[a + 2][b - 1]; mapa[a + 2][b - 1] = 0;   a = wys_tab;
			}
		}
	}break;    }

	case 5555:
	{for (int a = 0; a < wys_tab; a++) {
		for (int b = szer_tab; b > 0; b--) {
			if (mapa[a][b] == 3 && mapa[a][b + 1] == 0 && mapa[a + 1][b + 1] == 0)
			{
				mapa[a][b + 1] = mapa[a][b]; mapa[a][b] = 0;
				mapa[a][b] = mapa[a][b - 1]; mapa[a][b - 1] = 0;
				mapa[a + 1][b + 1] = mapa[a + 1][b]; mapa[a + 1][b] = 0;
				mapa[a][b - 1] = mapa[a][b - 2]; mapa[a][b - 2] = 0;   a = wys_tab;
			}
		}
	}break;    }

	case 6:
	{for (int a = 0; a < wys_tab; a++) {
		for (int b = szer_tab; b > 0; b--) {
			if (mapa[a][b] == 3 && mapa[a][b + 1] == 0 && mapa[a + 1][b] == 0)
			{
				mapa[a][b + 1] = mapa[a][b]; mapa[a][b] = 0;
				mapa[a][b] = mapa[a][b - 1]; mapa[a][b - 1] = 0;

				mapa[a + 1][b] = mapa[a + 1][b - 1]; mapa[a + 1][b - 1] = 0;
				mapa[a + 1][b - 1] = mapa[a + 1][b - 2]; mapa[a + 1][b - 2] = 0; a = wys_tab;
			}
		}
	}break;    }

	case 66:
	{for (int a = 0; a < wys_tab; a++) {
		for (int b = szer_tab; b > 0; b--) {
			if (mapa[a][b] == 3 && mapa[a][b + 1] == 0 && mapa[a + 1][b + 2] == 0 && mapa[a + 2][b + 2] == 0)
			{
				mapa[a][b + 1] = mapa[a][b]; mapa[a][b] = 0;
				mapa[a + 1][b + 2] = mapa[a + 1][b + 1]; mapa[a + 1][b + 1] = 0;
				mapa[a + 1][b + 1] = mapa[a + 1][b]; mapa[a + 1][b] = 0;
				mapa[a + 2][b + 2] = mapa[a + 2][b + 1]; mapa[a + 2][b + 1] = 0; a = wys_tab;
			}
		}
	}break;    }

	case 7:
	{for (int a = 0; a < wys_tab; a++) {
		for (int b = szer_tab; b > 0; b--) {
			if (mapa[a][b] == 3 && mapa[a][b + 1] == 0 && mapa[a + 1][b + 2] == 0)
			{
				mapa[a][b + 1] = mapa[a][b]; mapa[a][b] = 0;
				mapa[a][b] = mapa[a][b - 1]; mapa[a][b - 1] = 0;

				mapa[a + 1][b + 2] = mapa[a + 1][b + 1]; mapa[a + 1][b + 1] = 0;
				mapa[a + 1][b + 1] = mapa[a + 1][b]; mapa[a + 1][b] = 0; a = wys_tab;
			}
		}
	}break;    }

	case 77:
	{for (int a = 0; a < wys_tab; a++) {
		for (int b = szer_tab; b > 0; b--) {
			if (mapa[a][b] == 3 && mapa[a][b + 1] == 0 && mapa[a + 1][b + 1] == 0 && mapa[a + 2][b] == 0)
			{
				mapa[a][b + 1] = mapa[a][b]; mapa[a][b] = 0;
				mapa[a + 1][b + 1] = mapa[a + 1][b]; mapa[a + 1][b] = 0;
				mapa[a + 1][b] = mapa[a + 1][b - 1]; mapa[a + 1][b - 1] = 0;
				mapa[a + 2][b] = mapa[a + 2][b - 1]; mapa[a + 2][b - 1] = 0; a = wys_tab;
			}
		}
	}break;    }

	}
	rest(100);
}

void silnik::w_lewo()
{
	switch (numer_klocka)
	{
	case 1:
	{
		for (int a = 0; a < wys_tab; a++) {
			for (int b = 0; b < szer_tab; b++) {
				if ((mapa[a][b] == 3) && (mapa[a][b - 1] == 0 && mapa[a + 1][b - 1] == 0 && mapa[a + 2][b - 1] == 0 && mapa[a + 3][b - 1] == 0))
				{
					mapa[a][b - 1] = mapa[a][b];   mapa[a][b] = 0;
					mapa[a + 1][b - 1] = mapa[a + 1][b];   mapa[a + 1][b] = 0;
					mapa[a + 2][b - 1] = mapa[a + 2][b];   mapa[a + 2][b] = 0;
					mapa[a + 3][b - 1] = mapa[a + 3][b];   mapa[a + 3][b] = 0;
					a = wys_tab;
				}
			}
		}
		break;
	}

	case 11:
	{for (int a = 0; a < wys_tab; a++) {
		for (int b = 0; b < szer_tab; b++) {
			if ((mapa[a][b] == 3) && (mapa[a][b - 1] == 0))
			{
				mapa[a][b - 1] = mapa[a][b];   mapa[a][b] = 0;
				mapa[a][b] = mapa[a][b + 1];   mapa[a][b + 1] = 0;
				mapa[a][b + 1] = mapa[a][b + 2];   mapa[a][b + 2] = 0;
				mapa[a][b + 2] = mapa[a][b + 3];   mapa[a][b + 3] = 0;
				a = wys_tab;
			}
		}
	}    break; }
	case 2:
	{
		for (int a = 0; a < wys_tab; a++) {
			for (int b = 0; b < szer_tab; b++) {
				if ((mapa[a][b] == 3) && (mapa[a][b - 1] == 0 && mapa[a + 1][b] == 0))
				{
					mapa[a][b - 1] = mapa[a][b];   mapa[a][b] = 0;
					mapa[a][b] = mapa[a][b + 1];   mapa[a][b + 1] = 0;
					mapa[a][b + 1] = mapa[a][b + 2]; mapa[a][b + 2] = 0;
					mapa[a + 1][b] = mapa[a + 1][b + 1];   mapa[a + 1][b + 1] = 0;
					a = wys_tab;
				}
			}
		}
		break;
	}
	case 22:
	{
		for (int a = 0; a < wys_tab; a++) {
			for (int b = 0; b < szer_tab; b++) {
				if ((mapa[a][b] == 3) && (mapa[a][b - 1] == 0 && mapa[a + 2][b - 1] == 0 && mapa[a + 1][b - 2] == 0))
				{
					mapa[a][b - 1] = mapa[a][b];   mapa[a][b] = 0;
					mapa[a + 1][b - 2] = mapa[a + 1][b - 1];   mapa[a + 1][b - 1] = 0;
					mapa[a + 1][b - 1] = mapa[a + 1][b]; mapa[a + 1][b] = 0;
					mapa[a + 2][b - 1] = mapa[a + 2][b];   mapa[a + 2][b] = 0;
					a = wys_tab;
				}
			}
		}
		break;
	}

	case 222:
	{
		for (int a = 0; a < wys_tab; a++) {
			for (int b = 0; b < szer_tab; b++) {
				if ((mapa[a][b] == 3) && (mapa[a][b - 1] == 0 && mapa[a + 1][b - 2] == 0))
				{
					mapa[a][b - 1] = mapa[a][b];   mapa[a][b] = 0;
					mapa[a + 1][b - 2] = mapa[a + 1][b - 1];   mapa[a + 1][b - 1] = 0;
					mapa[a + 1][b - 1] = mapa[a + 1][b]; mapa[a + 1][b] = 0;
					mapa[a + 1][b] = mapa[a + 1][b + 1];   mapa[a + 1][b + 1] = 0;
					a = wys_tab;
				}
			}
		}
		break;
	}

	case 2222:
	{
		for (int a = 0; a < wys_tab; a++) {
			for (int b = 0; b < szer_tab; b++) {
				if ((mapa[a][b] == 3) && (mapa[a][b - 1] == 0 && mapa[a + 1][b - 1] == 0 && mapa[a + 2][b - 1] == 0))
				{
					mapa[a][b - 1] = mapa[a][b];   mapa[a][b] = 0;
					mapa[a + 1][b - 1] = mapa[a + 1][b];   mapa[a + 1][b] = 0;
					mapa[a + 2][b - 1] = mapa[a + 2][b]; mapa[a + 2][b] = 0;
					mapa[a + 1][b] = mapa[a + 1][b + 1];   mapa[a + 1][b + 1] = 0;
					a = wys_tab;
				}
			}
		}
		break;
	}

	case 3:
	{for (int a = 0; a < wys_tab; a++) {
		for (int b = 0; b < szer_tab; b++) {
			if (mapa[a][b] == 3 && mapa[a][b - 1] == 0 && mapa[a + 1][b - 1] == 0)
			{
				mapa[a][b - 1] = mapa[a][b]; mapa[a][b] = 0;
				mapa[a][b] = mapa[a][b + 1]; mapa[a][b + 1] = 0;
				mapa[a + 1][b - 1] = mapa[a + 1][b]; mapa[a + 1][b] = 0;
				mapa[a + 1][b] = mapa[a + 1][b + 1]; mapa[a + 1][b + 1] = 0;   a = wys_tab;
			}
		}
	}break;    }
	case 4:
	{for (int a = 0; a < wys_tab; a++) {
		for (int b = 0; b < szer_tab; b++) {
			if (mapa[a][b] == 3 && mapa[a][b - 1] == 0 && mapa[a + 1][b - 1] == 0 && mapa[a + 2][b - 1] == 0)
			{
				mapa[a][b - 1] = mapa[a][b]; mapa[a][b] = 0;
				mapa[a + 1][b - 1] = mapa[a + 1][b]; mapa[a + 1][b] = 0;
				mapa[a + 2][b - 1] = mapa[a + 2][b]; mapa[a + 2][b] = 0;
				mapa[a + 2][b] = mapa[a + 2][b + 1]; mapa[a + 2][b + 1] = 0;   a = wys_tab;
			}
		}
	}break;    }

	case 44:
	{for (int a = 0; a < wys_tab; a++) {
		for (int b = 0; b < szer_tab; b++) {
			if (mapa[a][b] == 3 && mapa[a][b - 1] == 0 && mapa[a + 1][b - 1] == 0)
			{
				mapa[a][b - 1] = mapa[a][b]; mapa[a][b] = 0;
				mapa[a + 1][b - 1] = mapa[a + 1][b]; mapa[a + 1][b] = 0;
				mapa[a][b] = mapa[a][b + 1]; mapa[a][b + 1] = 0;
				mapa[a][b + 1] = mapa[a][b + 2]; mapa[a][b + 2] = 0;   a = wys_tab;
			}
		}
	}break;    }

	case 444:
	{for (int a = 0; a < wys_tab; a++) {
		for (int b = 0; b < szer_tab; b++) {
			if (mapa[a][b] == 3 && mapa[a][b - 1] == 0 && mapa[a + 1][b] == 0 && mapa[a + 2][b] == 0)
			{
				mapa[a][b - 1] = mapa[a][b]; mapa[a][b] = 0;
				mapa[a + 1][b] = mapa[a + 1][b + 1]; mapa[a + 1][b + 1] = 0;
				mapa[a + 2][b] = mapa[a + 2][b + 1]; mapa[a + 2][b + 1] = 0;
				mapa[a][b] = mapa[a][b + 1]; mapa[a][b + 1] = 0;   a = wys_tab;
			}
		}
	}break;    }

	case 4444:
	{for (int a = 0; a < wys_tab; a++) {
		for (int b = 0; b < szer_tab; b++) {
			if (mapa[a][b] == 3 && mapa[a][b - 1] == 0 && mapa[a + 1][b - 3] == 0)
			{
				mapa[a][b - 1] = mapa[a][b]; mapa[a][b] = 0;
				mapa[a + 1][b - 3] = mapa[a + 1][b - 2]; mapa[a + 1][b - 2] = 0;
				mapa[a + 1][b - 2] = mapa[a + 1][b - 1]; mapa[a + 1][b - 1] = 0;
				mapa[a + 1][b - 1] = mapa[a + 1][b]; mapa[a + 1][b] = 0;   a = wys_tab;
			}
		}
	}break;    }

	case 5:
	{for (int a = 0; a < wys_tab; a++) {
		for (int b = szer_tab; b > 0; b--) {
			if (mapa[a][b] == 3 && mapa[a][b - 1] == 0 && mapa[a + 1][b - 1] == 0 && mapa[a + 2][b - 2] == 0)
			{
				mapa[a][b - 1] = mapa[a][b]; mapa[a][b] = 0;
				mapa[a + 1][b - 1] = mapa[a + 1][b]; mapa[a + 1][b] = 0;
				mapa[a + 2][b - 2] = mapa[a + 2][b - 1]; mapa[a + 2][b - 1] = 0;
				mapa[a + 2][b - 1] = mapa[a + 2][b]; mapa[a + 2][b] = 0;   a = wys_tab;
			}
		}
	}break;    }

	case 55:
	{for (int a = 0; a < wys_tab; a++) {
		for (int b = szer_tab; b > 0; b--) {
			if (mapa[a][b] == 3 && mapa[a][b - 1] == 0 && mapa[a + 1][b - 1] == 0)
			{
				mapa[a][b - 1] = mapa[a][b]; mapa[a][b] = 0;
				mapa[a + 1][b - 1] = mapa[a + 1][b]; mapa[a + 1][b] = 0;
				mapa[a + 1][b] = mapa[a + 1][b + 1]; mapa[a + 1][b + 1] = 0;
				mapa[a + 1][b + 1] = mapa[a + 1][b + 2]; mapa[a + 1][b + 2] = 0;   a = wys_tab;
			}
		}
	}break;    }

	case 555:
	{for (int a = 0; a < wys_tab; a++) {
		for (int b = szer_tab; b > 0; b--) {
			if (mapa[a][b] == 3 && mapa[a][b - 1] == 0 && mapa[a + 1][b - 1] == 0 && mapa[a + 2][b - 1] == 0)
			{
				mapa[a][b - 1] = mapa[a][b]; mapa[a][b] = 0;
				mapa[a + 1][b - 1] = mapa[a + 1][b]; mapa[a + 1][b] = 0;
				mapa[a + 2][b - 1] = mapa[a + 2][b]; mapa[a + 2][b] = 0;
				mapa[a][b] = mapa[a][b + 1]; mapa[a][b + 1] = 0;   a = wys_tab;
			}
		}
	}break;    }

	case 5555:
	{for (int a = 0; a < wys_tab; a++) {
		for (int b = szer_tab; b > 0; b--) {
			if (mapa[a][b] == 3 && mapa[a][b - 1] == 0 && mapa[a + 1][b + 1] == 0)
			{
				mapa[a][b - 1] = mapa[a][b]; mapa[a][b] = 0;
				mapa[a][b] = mapa[a][b + 1]; mapa[a][b + 1] = 0;
				mapa[a][b + 1] = mapa[a][b + 2]; mapa[a][b + 2] = 0;
				mapa[a + 1][b + 1] = mapa[a + 1][b + 2]; mapa[a + 1][b + 2] = 0;   a = wys_tab;
			}
		}
	}break;    }

	case 6:
	{for (int a = 0; a < wys_tab; a++) {
		for (int b = 0; b < szer_tab; b++) {
			if (mapa[a][b] == 3 && mapa[a][b - 1] == 0 && mapa[a + 1][b - 2] == 0)
			{
				mapa[a][b - 1] = mapa[a][b]; mapa[a][b] = 0;
				mapa[a][b] = mapa[a][b + 1]; mapa[a][b + 1] = 0;
				mapa[a + 1][b - 2] = mapa[a + 1][b - 1]; mapa[a + 1][b - 1] = 0;
				mapa[a + 1][b - 1] = mapa[a + 1][b]; mapa[a + 1][b] = 0; a = wys_tab;
			}
		}
	}break;    }

	case 66:
	{for (int a = 0; a < wys_tab; a++) {
		for (int b = 0; b < szer_tab; b++) {
			if (mapa[a][b] == 3 && mapa[a][b - 1] == 0 && mapa[a + 1][b - 1] == 0 && mapa[a + 2][b] == 0)
			{
				mapa[a][b - 1] = mapa[a][b]; mapa[a][b] = 0;
				mapa[a + 1][b - 1] = mapa[a + 1][b]; mapa[a + 1][b] = 0;
				mapa[a + 1][b] = mapa[a + 1][b + 1]; mapa[a + 1][b + 1] = 0;
				mapa[a + 2][b] = mapa[a + 2][b + 1]; mapa[a + 2][b + 1] = 0; a = wys_tab;
			}
		}
	}break;    }

	case 7:
	{for (int a = 0; a < wys_tab; a++) {
		for (int b = 0; b < szer_tab; b++) {
			if (mapa[a][b] == 3 && mapa[a][b + 1] == 3 && mapa[a + 1][b + 1] == 3 && mapa[a][b - 1] == 0 && mapa[a + 1][b] == 0)
			{
				mapa[a][b - 1] = mapa[a][b]; mapa[a][b] = 0;
				mapa[a + 1][b] = mapa[a + 1][b + 1]; mapa[a + 1][b + 1] = 0;
				mapa[a][b] = mapa[a][b + 1]; mapa[a][b + 1] = 0;
				mapa[a + 1][b + 1] = mapa[a + 1][b + 2]; mapa[a + 1][b + 2] = 0;

				a = wys_tab;
			}
		}
	}break;    }

	case 77:
	{for (int a = 0; a < wys_tab; a++) {
		for (int b = 0; b < szer_tab; b++) {
			if (mapa[a][b] == 3 && mapa[a][b - 1] == 0 && mapa[a + 1][b - 2] == 0 && mapa[a + 2][b - 2] == 0)
			{
				mapa[a + 1][b - 2] = mapa[a + 1][b - 1]; mapa[a + 1][b - 1] = 0;
				mapa[a][b - 1] = mapa[a][b]; mapa[a][b] = 0;
				mapa[a + 1][b - 1] = mapa[a + 1][b]; mapa[a + 1][b] = 0;
				mapa[a + 2][b - 2] = mapa[a + 2][b - 1]; mapa[a + 2][b - 1] = 0;

				a = wys_tab;
			}
		}
	}break;    }


	}
	rest(100);
}




