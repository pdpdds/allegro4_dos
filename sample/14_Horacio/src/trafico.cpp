#include "trafico.h"

//extern Hardware *H;

Trafico::Trafico(int Vel)
{
     Inicial = NULL;
     Final = NULL;
     NumVeh = 0;
     Ciclos = -1;
     Speed = Vel;
}

Trafico::~Trafico()
{
     Destruye ();
}

void Trafico::NuevoVehiculo () {
//     do {
        Temp1 = new Vehiculo (Final, Speed);
        if (Temp1->MiraEspera ()) {
           delete Temp1;
           return;}
//           } while (Temp1 == NULL);
     
     if (++NumVeh == 1) // Si es el primer vehiculo
        Inicial = Temp1; // Consideralo como Inicio de la lista
      else Final->GrabarSiguiente(Temp1); // Si no lo es, es el siguiente al ?timo
     
     Final = Temp1;
}

Vehiculo* Trafico::Mueve () {
     bool Salir = false;
     if (!NumVeh) return NULL;
     
     if (++Ciclos == 4) Ciclos = 0;
    
     Bucle = Inicial;
     do {
         if (Bucle->AumentaPosicion(Final, Ciclos)) {// Mueve el vehiculo, y si se sale de la pantalla...
            if (--NumVeh == 0) { delete Bucle; Inicial = NULL; Final = NULL; return NULL;} // Si es el ?timo, borralo y ya has terminado
              else { // Si quedan m? veh?ulos...
                   if (Bucle == Inicial) {Inicial = Bucle->VerSiguiente();
                                          Inicial->GrabarAnterior (NULL);
                                          delete Bucle;
                                          return Inicial;} // Si es el primero,  borralo y welve
                   if (Bucle == Final) {Final = Bucle->VerAnterior();
                                        Final->GrabarSiguiente(NULL);
                                        delete Bucle;
                                        return Inicial;} // Si es el ultimo,  borralo y welve
                   Temp1 = Bucle->VerAnterior();
                   Temp2 = Bucle->VerSiguiente();
                   Temp1->GrabarSiguiente(Temp2);
                   Temp2->GrabarAnterior(Temp1);
                   delete Bucle;
                   Bucle = Temp1;
                   }
             }
         Temp1 = Bucle->VerSiguiente();
         if (!Temp1) Salir = true;
            else Bucle = Temp1;
        } while (!Salir);
//        Final = Bucle; Lo he dejado como REM pq en realidad, el ultimo no se modifica salvo que se borre, y ya esta implementado
        return Inicial;
}

void Trafico::Dibuja () {  // Dibuja los coches existentes
     if (!NumVeh) return;
     Bucle = Inicial;
     do {
         Bucle->Dibuja();
         Temp1 = Bucle->VerSiguiente ();
         if (Temp1) Bucle = Temp1; } while (Temp1);
     return;
}

void Trafico::Borra () {  // Borra los coches dibujados
     if (!NumVeh) return;
     Bucle = Inicial;
     do {
         Bucle->Borra();
         Temp1 = Bucle->VerSiguiente ();
         if (Temp1) Bucle = Temp1; } while (Temp1);
     return;
}

int Trafico::Cuantos () {
    return NumVeh;}

void Trafico::Destruye () { // Destruye todo
     if (!NumVeh) return;
     Bucle = Inicial;     
     Temp1 = Bucle->VerSiguiente();
     delete Bucle;
     while (Temp1) {
           Bucle = Temp1;
           Temp1 = Bucle->VerSiguiente();
           delete Bucle;}
     return;
}
