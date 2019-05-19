#include "esquiador.h"

extern SprManager *M;
extern Hardware *H;
extern Mensajes *SMS;
extern Sonido *SFX;
extern int Puntos, Nivel;

Esquiador::Esquiador()
{
 Tipo = ESQUIADOR;
 Sentido = 0;
 x = ANCHOPANTALLA / 2;
 y = H->TextHeight();
 YVirt = 0;
 Actualiza();
 Cont = 0;
}

Esquiador::~Esquiador()
{
}

void Esquiador::Avanza (int Tecla, int Speed, int YIndice, bool FinPista, int Margin) {
     
     if (Margin) YVirt = -1000;  // Si queda Margen... no hay que valorar las colisiones
     
     if (Tecla == ABAJO)
        Sentido = 0;
      else            
          switch (Sentido) {
                 case 0:
                      if (Tecla == IZQUIERDA) Sentido = 3;
                      if (Tecla == DERECHA) Sentido = 1;
                      break;
                 case 1:
                      if (Tecla == IZQUIERDA) Sentido = 0;
                      if (Tecla == DERECHA) Sentido = 2;
                      break;
                 case 2:
                      if (Tecla == IZQUIERDA) Sentido = 1;
                      break;
                 case 3:
                      if (Tecla == IZQUIERDA) Sentido = 4;
                      if (Tecla == DERECHA) Sentido = 0;
                      break;
                 case 4:
                      if (Tecla == DERECHA) Sentido = 3;
                      break;
                 }
     
     if (Tecla>=0)
        Cont = 0;
        else
            if ((++Cont > 20) && ((Sentido == 4) || (Sentido == 2))) {  // Si lleva más de 20 ciclos "subiendo" sin mantener la tecla pulsada, haz que deje de subir
               Cont = 0;
               Sentido -= 1;}
     
     if (FinPista && (Sentido == 4 || Sentido == 2)) Sentido -= 1;

     Actualiza();
     
     switch (Sentido) {
            case 0:
                 VelY = Speed;
                 VelX = 0;
                 break;
            case 1:
                 VelY = 0;
                 VelX = Speed;
                 break;
            case 2:
                 VelY = -Speed;
                 VelX = Speed;
                 break;
            case 3:
                 VelY = 0;
                 VelX = -Speed;
                 break;
            case 4:
                 VelY = -Speed;
                 VelX = -Speed;
                 break;
     }
     
     if (FinPista) VelY = 2 * Speed;             // Si se acaba la pista, solo tienes que poner la Vel Vertical
        else if (!Margin) YVirt += Speed;
    
     Mueve(FinPista, Margin, YIndice, Speed);
}

void Esquiador::Mueve (bool FinPista, int Margin, int YIndice, int Speed) {
     
     x += VelX;

     if (VerX (false) < 0) {
        x = 1;
        VelX = 0;
        VelY = Speed;
        Sentido = 0;}
        
     if (VerX(true) >= ANCHOPANTALLA) {
        x = ANCHOPANTALLA - Mascara->LongX;
        x--;
        VelX = 0;
        VelY = Speed;
        Sentido = 0;}

     y += VelY;

     if (VerY (false) < H->TextHeight()) {
        y = H->TextHeight();
        VelY = 0;
        Sentido--;}
        
     if ((VerY(true) >= ALTOPANTALLA / 2) && (!FinPista)) {
        y = (ALTOPANTALLA / 2) - Mascara->LongY;
        VelY = 0;}
        
     if (!Margin) YVirt += VelY;
        else
            if (VerY(true) > Margin) YVirt = YIndice + ((y-Margin)-H->TextHeight());
     
     Actualiza();
}

int Esquiador::Choque (Obstaculo *Indice, int YIndice) {
    
    Temp1 = Indice;
    Retorno = 0;
    YTemp = y;
    y = YVirt;
    
    while  (Temp1 && 
           (Temp1->VerY(true) > YIndice) && 
           (Temp1->VerY(false) < YIndice + ALTOPANTALLA - H->TextHeight())) {
                               
           switch (Temp1->VerTipo()) {
                  
                  case PINO:
                       if ((Temp1->VerY(true) < VerY(false)) &&
                           (!Temp1->VerPasado())) Temp1->Pasa();
                       if ((!Temp1->VerPasado()) &&
                           (Colision (Temp1)))
                          Retorno = PINO;
                       break;
                       
                  case BANDERA:
                       if ((Colision (Temp1, false)) &&
                          (VerX(false) + (VerLongX()/2) >= Temp1->VerX(false)) &&
                          (VerX(true) - (VerLongX() /2) <= Temp1->VerX(true)) &&
                          (VerY(true) >= Temp1->VerY(false) + (Mascara->LongY/2)))
                                      if (!Temp1->VerPasado()) {
                                         Puntos += 10;
                                         Temp1->Pasa();}
                       if ((Temp1->VerY(true) < VerY(false)) &&
                           (!Temp1->VerPasado())) {
                              Puntos -= 10 * (Nivel+1);
                              SFX->SAhh();
                              Temp1->Pasa();}
                       break;
                  
                  case MONTON:
                       if (Colision (Temp1)) {
                          if (Sentido > 2) // a la Izqda.
                             Retorno = IZQUIERDA;
                          if ((Sentido > 0) && (Sentido < 3)) 
                             Retorno = DERECHA;
                          if (Sentido == 0)
                             if ((VerX(true) / 2 < Temp1->VerX(true) / 2))
                                Retorno = IZQUIERDA;
                               else
                                Retorno = DERECHA;
                          SFX->SSki();
                       }
                       break;
                  
                  case META:
                       if (Colision (Temp1))
                          Retorno = METACHOQ;
                         else
                       if ((Colision (Temp1, false)) && (!Temp1->VerPasado()) &&
                          (VerX(false) + (VerLongX()/2) >= Temp1->VerX(false)) &&
                          (VerX(true) - (VerLongX() /2) <= Temp1->VerX(true)) &&
                          (VerY(true) >= (Temp1->VerY(false) + 48 + int (Mascara->LongY / 2)))) {
                          Retorno = ABAJO;  // Ese 48 es el tamaño de la pancarta en el set grafico antiguo
                          Temp1->Pasa();
                       break;}                         
           }

           if (!Retorno)
              Temp1 = Temp1->VerSiguiente();
           else Temp1 = NULL;}

    y = YTemp;
    return Retorno;
}

