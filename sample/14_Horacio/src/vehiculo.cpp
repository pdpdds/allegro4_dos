#include "vehiculo.h"

extern SprManager *M;
extern Hardware *H;
extern Sonido *SFX;

Vehiculo::Vehiculo(Vehiculo *Final, int Vel)
{
   Siguiente = NULL;
   Anterior = Final;

   Tipo = (int(rand()%11));
   
   if (Tipo > MOTO) Tipo-=6;
      
   if (Tipo == CAMION1 || Tipo == CAMION2) VelX = 1;
   if (Tipo == COCHE1 || Tipo == COCHE2) VelX = 2;
   if (Tipo == AMBULANCIA) VelX = 4;
   if (Tipo == MOTO) VelX = 3;
   VelY = 0; VelAlt = 0;    
   Sentido = (int(rand()%2));

   if (Sentido==1) x = -150;
     else {Sentido=-1; x=ANCHOPANTALLA+150;}

   Actualiza();

   y = int(rand()%(155-(Mascara->LongY))) + 62;
   if (Sentido<0) y+=164;

   Esperar = ColisionY(this);
   Ciclos0 = 0;
   Speed = Vel;
}

Vehiculo::~Vehiculo()
{
}

int Vehiculo::ColisionX (Vehiculo *Ciclo) {
    
    if (Ciclo) do {
       if (Ciclo != this)
          if (Colision(Ciclo, false)) {
             if (VelAlt) return VelAlt;                           
             if (VerY(true) / 2 > Ciclo->VerY(true) / 2) return 1;
                else return -1;
          }
       Ciclo = Ciclo->VerAnterior();
    } while (Ciclo);
    return 0;
}

bool Vehiculo::ColisionY (Vehiculo *Ciclo) {
    
    if (Ciclo) do {
       if (Ciclo != this)
          if (Colision(Ciclo, false))
             return true;
       Ciclo = Ciclo->VerAnterior();
    } while (Ciclo);
    return false;
}

bool Vehiculo::AumentaPosicion (Vehiculo* Final, int Ciclos) { // Aumenta la posicion de este veh?ulo

	         
     if (VelX - Ciclos < 1) return false;                 // Si no le toca moverse, vuelvete
     
     x += Sentido * Speed;                               // Muevete en el sentido que corresponda
     if (Tipo > CAMION2) VelY = ColisionX (Final);      // Colision horizontal? Solo si va m? r?ido que un cami? 
     if (VelY) {
        Ciclos0 = 0;                             // Si hay Colision horizontal, el n?de ciclos sin colision es 0, pq ... ¡Hay Colision!
        VelAlt = VelY;}
      else                                                // si no...
        if (++Ciclos0 > 4)                               // Si van m? de 4 ciclos sin colisi?...
           VelAlt = 0;                                    // La VelAlt vale 0... es decir... no te muevas verticalmente
           
     if (VelY != 0)                                     // Si la VelY es distinta de 0... (hay colisi? horizontal)
        x -= Sentido * Speed;                           // retrocede horizontalmente
        
     VelY = VelAlt;
     
     if (Ciclos < 1) {
        y += VelY;        
        if (ColisionY(Final)) {
           VelAlt = -VelAlt;
           y -= 2*VelY;
        }
        if (VerY(false)<65) {y=65; VelY = 1; VelAlt = 1;} 
        if (VerY(true)>380) {y=380-Mascara->LongY; VelY = -VelY;VelAlt = -VelAlt;}
        if (VerY(true)>221 && Sentido == 1) {y=221-Mascara->LongY; VelY=-VelY;VelAlt = -VelAlt;}
        if (VerY(false)<226 && Sentido == -1) {y=226; VelY=-VelY;VelAlt = -VelAlt;}
     }


     
     if ((VerX(false) > ANCHOPANTALLA && Sentido == 1) ||
         (VerX(true) < 0 && Sentido == -1)) {
             if (SFX->VerAmb() == 1) SFX->SAmb(true);
             return true;}
       else       
         if (Tipo == AMBULANCIA)
            SFX->SAmb (false);
            
     return false;
}

Vehiculo* Vehiculo::VerSiguiente () {
     return Siguiente;}

void Vehiculo::GrabarSiguiente (Vehiculo* Pointer) {
     Siguiente = Pointer;}

Vehiculo* Vehiculo::VerAnterior () {
     return Anterior;}

void Vehiculo::GrabarAnterior (Vehiculo* Pointer) {
     Anterior = Pointer;}

bool Vehiculo::MiraEspera () {
     return Esperar;}
     
