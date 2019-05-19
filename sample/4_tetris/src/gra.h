#ifndef gra_h
#define gra_h
#include "gra.h"
#include "silnik.h"
#include <allegro.h>

class gra : public silnik{

private:
BITMAP * bufor ;                     //tlo
BITMAP * k1 ;                        //kafelki..
BITMAP * k2 ;
BITMAP * k3 ;
BITMAP * k5 ;
int tablica_wynikow;


public:
gra();
void wlacz_allegro();
void zaladuj_bitmapy();
void rysuj();
void wylacz_allegro();

int nr_klocka;
int punkty;


};
#endif


