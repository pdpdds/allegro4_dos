#ifndef silnik_h
#define silnik_h
#include "gra.h"
#include "silnik.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#define wys_tab 21
#define szer_tab 12

class silnik
{    private:
int chwilowy_numer;                  //zmienna pomocnicza wykorzystywana przy obracaniu klocka
int numer_klocka;                 //  pierwsza cyfra - numer klocka, kazda kolejna w zaleznosci od obrotu

void odpal_klocek();                  //rysuje nowy klocek po wylosowaniu
void obracaj();                      // obraca 7 klockow na 19kombinacji
void w_prawo();                         // schemat skrecania w prawo dla 19kombinacji klockow
void w_lewo();                           //schemat skrecania w lewo dla 19 kombinacji klockow
void warunek_spadania(int a, int b);       //warunki spadania dla 19 kominacji klockow

bool zamiana_na_czerwony(int a, int b);      //zamienia figure na kolor czerwony po zatrzymaniu sie


     public:
int mapa[wys_tab][szer_tab];                //pole gry
bool flag;                          // czy losuje sie kolejny
int punktacja;                        // punkty

silnik();                                    // konstruktor
void czykoniec();                             //sprawdza czy w pierwszej linii nie ma nieruchomego klocka
int kasuj_linie(int a, int b);                         //kasuje linie gdy jest calkowicie zapelniona
void resp(int x);                                  // sprawdza czy mozna wylosowac nowy klocek
int losuj();                                         // funkcja ktora losuje nowy klocek
void ruch(int a, int b);                      // posredniczy kadzemu mozliwemu ruchowi
};
#endif
