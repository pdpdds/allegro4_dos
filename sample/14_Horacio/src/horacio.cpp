#include "horacio.h"

extern SprManager *M;
extern Hardware *H;
extern int Vidas;

Horacio::Horacio(int Vel)
{
   VelX = 0;
   VelY = 0;
   x = ANCHOPANTALLA / 2;
   y = H->TextHeight();
   Indice = 112 * 2;
   Sentido = 1;
   Speed = Vel;
   SinEsqui();
   Actualiza();
}

Horacio::~Horacio()
{
}

void Horacio::ActualizaMov (int Tecla) {
     if (Tecla == ARRIBA && VerY() > H->TextHeight()) {
        SetVelY (-1 * Speed);
        SetVelX (0);}
     if (Tecla == ABAJO && VerY(true) < ALTOPANTALLA - 18) {
        SetVelY (1 * Speed);
        SetVelX (0);}
     if (Tecla == IZQUIERDA && VerX() > 0) {
        SetVelX (-1 * Speed);
        SetVelY (0);}
     if (Tecla == DERECHA && VerX(true) < ANCHOPANTALLA) {
        SetVelX (1 * Speed);
        SetVelY (0);}

     if (VelX < 0) Sentido = 0;
     if (VelX > 0) Sentido = 1;
     if (VelY < 0) Sentido = 2;
     if (VelY > 0) Sentido = 3;

     return;
}


void Horacio::Mueve () {   
     if ((!VelX) && (!VelY)) FotoMax = 0;
        else FotoMax = 1; // Si está parado, no uses los fotogramas*/       
                
     if ((x+VelX > 0) && (VerX(true)+VelX < ANCHOPANTALLA)) x+=VelX;
        else VelX = 0; // Si se sale de la pantalla, paralo
     if ((y+VelY > H->TextHeight())) y+=VelY;
        else VelY = 0; // Si se sale de la pantalla, paralo
     if (VerY(true)+VelY >= ALTOPANTALLA-18) {
        y = ALTOPANTALLA - 18 - Mascara->LongY;
        VelY = 0;}
}

bool Horacio::Atropello (Vehiculo *Ciclo) {

    if (!Ciclo) return false;

    Vehiculo *Temporal;
    bool Salir = false;

    do {

        if (Colision(Ciclo)) return true;
        Temporal = Ciclo->VerSiguiente();
        if (!Temporal) Salir = true;
           else Ciclo = Temporal;
        } while (!Salir);
        
        return false;
}

void Horacio::SinEsqui () {
     Tipo = HORACIO;
     Actualiza();
}

void Horacio::ConEsqui () {
     Tipo = HORACIOCS;
     Actualiza();
}
