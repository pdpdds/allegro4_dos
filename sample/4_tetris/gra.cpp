#include "gra.h"
#include "silnik.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <allegro.h>

#define wys_tab 21
#define szer_tab 12

#ifdef _WIN32
#define szer_okna 800
#define wys_okna 600
#else
#define szer_okna 320
#define wys_okna 200
#endif

#define odleglosc_od_lewej_krawedzi 95
#define odleglosc_od_gornej_krawedzi 100
#define szerokosc_plytki 26
#define wysokosc_plytki 23

gra::gra()
{

	for (int a = 0; a < wys_tab; a++)
	{
		mapa[a][0] = 1;                //sciana lewa
		mapa[a][szer_tab - 1] = 1;      // sciana prawa

		for (int k = 1; k < szer_tab - 1; k++)
		{
			mapa[a][k] = 0;                    // pusta przestrzen
		}
	}

	for (int a = 0; a < szer_tab; a++)
	{
		mapa[wys_tab - 1][a] = 1;       //podloga
	}


	bufor = NULL;
	k1 = NULL;
	k2 = NULL;
	k3 = NULL;
	k5 = NULL;
	tablica_wynikow = 0;
}

void gra::rysuj()
{
	blit(bufor, screen, 0, 0, 0, 0, szer_okna, wys_okna);
	for (int i = 0; i < wys_tab; i++)                                     //wyswietlanie tablicy
	{
		for (int j = 0; j < szer_tab; j++)
		{
			if (mapa[i][j] == 1)masked_blit(k1, bufor, 0, 0, odleglosc_od_lewej_krawedzi + j * szerokosc_plytki, odleglosc_od_gornej_krawedzi + i * wysokosc_plytki, k1->w, k1->h);
			if (mapa[i][j] == 0) masked_blit(k2, bufor, 0, 0, odleglosc_od_lewej_krawedzi + j * szerokosc_plytki, odleglosc_od_gornej_krawedzi + i * wysokosc_plytki, k2->w, k2->h);
			if (mapa[i][j] == 3 || mapa[i][j] == 4) masked_blit(k3, bufor, 0, 0, odleglosc_od_lewej_krawedzi + j * szerokosc_plytki, odleglosc_od_gornej_krawedzi + i * wysokosc_plytki, k3->w, k3->h);
			if (mapa[i][j] == 5) masked_blit(k5, bufor, 0, 0, odleglosc_od_lewej_krawedzi + j * szerokosc_plytki, odleglosc_od_gornej_krawedzi + i * wysokosc_plytki, k2->w, k2->h);
		}
	}

	if (tablica_wynikow != punktacja) { tablica_wynikow = punktacja; std::cout << "Aktualny wynik: " << tablica_wynikow << std::endl; }
}
END_OF_FUNCTION(rysuj);

void gra::wlacz_allegro()
{
	allegro_init();
	install_keyboard();
	install_timer();
	set_color_depth(32);
#ifdef _WIN32
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, szer_okna, wys_okna, 0, 0);
#else
	set_gfx_mode(GFX_AUTODETECT, szer_okna, wys_okna, 0, 0);
#endif

	zaladuj_bitmapy();
}
END_OF_FUNCTION(wlacz_allegro);



void gra::zaladuj_bitmapy()
{

#ifdef _WIN32
	bufor = load_bmp("bufor.bmp", default_palette);
	k1 = load_bmp("k1.bmp", default_palette);
	k2 = load_bmp("k2.bmp", default_palette);
	k3 = load_bmp("k3.bmp", default_palette);
	k5 = load_bmp("k5.bmp", default_palette);
#else
	bufor = load_bmp("_bufor.bmp", default_palette);
	k1 = load_bmp("k1.bmp", default_palette);
	k2 = load_bmp("k2.bmp", default_palette);
	k3 = load_bmp("k3.bmp", default_palette);
	k5 = load_bmp("k5.bmp", default_palette);
#endif

	if (!bufor)
	{
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Nie moge utworzyc bufora !");
		allegro_exit();
	}
	if (!k1)
	{
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Nie moge zaladowac grafiki scianek !");
		allegro_exit();
	}
	if (!k2)
	{
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Nie moge zaladowac grafiki pola gry !");
		allegro_exit();
	}
	if (!k3)
	{
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Nie moge zaladowac grafiki klocka !");
		allegro_exit();
	}
	if (!k5)
	{
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Nie moge zaladowac grafiki nieruchomego klocka !");
		allegro_exit();
	}
}
END_OF_FUNCTION(zaladuj_bitmapy);


void gra::wylacz_allegro()
{
	destroy_bitmap(k2);
	destroy_bitmap(k3);
	destroy_bitmap(k1);
	destroy_bitmap(k5);
	destroy_bitmap(bufor);
	allegro_exit();
}
END_OF_FUNCTION(wylacz_allegro);



