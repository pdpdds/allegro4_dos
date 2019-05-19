#include "obstaculo.h"

extern Hardware *H;
extern SprManager *M;

Obstaculo::Obstaculo(int xo, int yo, int Tip)
{
 Tipo = Tip;
 Sentido = 0;

 if (Tip == BANDESTRECHA) {
         Tipo = BANDERA;
         Sentido = 0;}

 if (Tip == BANDANCHA) {
         Tipo = BANDERA;
         Sentido = 1;}

 if (Tip == METESTRECHA) {
         Tipo = META;
         Sentido = 0;}

 if (Tip == METANCHA) {
         Tipo = META;
         Sentido = 1;}
         
 if (Tip == MUNECO) {
         Tipo = PINO;
         Sentido = 1;}
         
 x = xo; y = yo;
 SetKF (20);
 Actualiza();
 Siguiente = NULL;
 Pasado = false;
}

Obstaculo::~Obstaculo()
{
}

Obstaculo* Obstaculo::VerSiguiente () {
     return Siguiente;}

void Obstaculo::GrabarSiguiente (Obstaculo *Pointer) {
     Siguiente = Pointer;}
         
void Obstaculo::Pasa () {
     Pasado = true;}
     
bool Obstaculo::VerPasado () {
     return Pasado;}

void Obstaculo::rDibuja (int YInd, int Margen) {
     int YTemp;
     YTemp = y;
     y -= YInd;
     y += Margen + H->TextHeight();
     if (y <= ALTOPANTALLA) Dibuja();
     y = YTemp;}

void Obstaculo::rBorra (int YInd, int Margen) {
     int YTemp;
     YTemp = y;
     y -= YInd;
     y += Margen + H->TextHeight();
     if (y <= ALTOPANTALLA) Borra();
     y = YTemp;}
