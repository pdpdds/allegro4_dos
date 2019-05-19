#include "faseuno.h"
#include <allegro.h>

volatile unsigned int CiclosTempF1;

void Ret1 (void) {
     CiclosTempF1++;
}

END_OF_FUNCTION(Ret1)

extern Hardware *H;
extern SprManager *M;
extern Mensajes *SMS;
extern Configuracion *Config;
extern HiScore *Record;
extern Sonido *SFX;

extern int Dinero, Puntos, Vidas, Nivel, ProximaVida;

FaseUno::FaseUno(int Spd)
{
  Speed = Spd + 1;
  Traffic = new Trafico(Speed);
  Horace = new Horacio(Speed);
  Inicial = NULL;
  PuntosInt = 0;
  SetGraf = Config->SetGraf();
  Horace->SetKF (40 / Speed);
  MueveHoracio = true;
  Pausa = new Pause;
}

FaseUno::~FaseUno()
{
  delete Traffic;
  delete Horace;
  delete Pausa;
  remove_int (Ret1);
}

void FaseUno::Inicia (int Esquis) {
     H->SetFondo (DFCarretera0 + SetGraf);
     H->DrawFondoBuffer (0,0,ANCHOPANTALLA,ALTOPANTALLA,0,0);
     
     if (Esquis == FINFASEOK) {
        Horace->SetY(428);
        Horace->ConEsqui();}
      else {
        Horace->SetY(H->TextHeight()+1);
        Horace->SinEsqui();}
        
     Horace->SetVelY (0);
     Horace->SetVelX (0);
     Horace->Dibuja();
     Marcadores();
     if (Traffic->Cuantos() == 0) {
        for (n=0;n<500;n++) {
            if (Traffic->Cuantos()<10)
               Traffic->NuevoVehiculo();
               Inicial = Traffic->Mueve();}
     }
     Traffic->Dibuja();
     H->VuelcaBuffer();
     H->ClearKeyBuf();
     LOCK_VARIABLE (CiclosTempF1);
     LOCK_FUNCTION(Ret1);
     InitTemp ();
     install_int_ex (Ret1, BPS_TO_TIMER (400 / Speed)); // Necesito 200 frames x segundo
}

void FaseUno::LeeTeclado () {
     Tecla = H->Tecla();
     if (Tecla == ABORTK) Salir = Pausar();
     Horace->ActualizaMov (Tecla);
     return;
}

void FaseUno::SacaAmbulancia () {

     Ambulance = new Ambulancia(Speed);
     n = 0;
     InitTemp ();
     H->TextOutCentre (SMS->Leer(3),0,-1,2);
     Dinero -=10;
     SFX->SAmb_ex();
     while (!n)
       while (!n && CiclosHechos < CiclosTempF1) {
           Ambulance->Borra();
           n = Ambulance->Avanza();
           Ambulance->Dibuja();
           H->VuelcaBuffer();
           CiclosHechos++;}
     
     Horace->Borra();
     delete Ambulance;
     H->DrawFondoBuffer (0,0,ANCHOPANTALLA,H->TextHeight(),0,0);
     if (Horace->VerTipo() == HORACIO) 
        Horace->SetY(H->TextHeight());
      else
        Horace->SetY(428);
     Horace->SetVelY (0);     
}

void FaseUno::Marcadores () {
     char *Marcador, *Temp;

     Marcador = new char [64];
     Temp = new char [10];

     if (Dinero<0) Dinero = 0;

     sprintf (Marcador, "%s%i          %s%i          %s%i ", SMS->Leer (0),Dinero,
                                                             SMS->Leer (1),Puntos,
                                                             SMS->Leer (2),Record->LeePuntos(0)); 
     H->TextOutCentre (Marcador, 0,-1,7);
     delete Marcador;
     delete Temp;
}

void FaseUno::AjustaPuntos () {     
      if ((Horace->VerY(true) > 65) && (Horace->VerY(false) < 380) &&
          (Horace->VerVelY()!=0))
         PuntosInt++;
      if (PuntosInt == 40 / Speed) {
         Puntos+=5;
         if (Puntos >= ProximaVida) {
            ProximaVida += 1000;
            Dinero += 10;
            SFX->SExtra();}
         PuntosInt = 0;}
}

void FaseUno::InitTemp () {
     CiclosHechos = 0;
     CiclosTempF1 = 0;
}

int FaseUno::Pausar () {    
    Traffic->Dibuja();
    Horace->Dibuja();
    Marcadores();
    H->CopyFondo();
    int Retorno = Pausa->PauseGame();    

    H->SetFont ("DFFontGame_DATA", "DFFontGame_PAL");
    H->SetFondo (DFCarretera0 + SetGraf);
    H->DrawFondoBuffer (0,0,ANCHOPANTALLA,ALTOPANTALLA,0,0);
    Traffic->Dibuja();
    Horace->Dibuja();
    Marcadores();
    H->VuelcaBuffer();
    InitTemp();
    return Retorno;
}
    
int FaseUno::Ciclo () {
  
  Salir = 0;
  
  do {
   while (!Salir && CiclosHechos < CiclosTempF1) {

      if (Traffic->Cuantos()<18+Nivel && (int(rand()%100)) <= Nivel) Traffic->NuevoVehiculo();

      Traffic->Borra();
      Horace->Borra();

      Inicial = Traffic->Mueve();
      if (MueveHoracio) Horace->Mueve();
      MueveHoracio = !MueveHoracio;

      Marcadores ();
      LeeTeclado();

      Horace->Dibuja();
      Traffic->Dibuja();
      
      AjustaPuntos();
      PausaFin = false;

      if ((Inicial) && (Horace->Atropello(Inicial))) {
         SacaAmbulancia();  
         H->Rest (100);
         H->ClearKeyBuf();
         InitTemp();}
       
      if ((Horace->VerY(true) > 458) && 
          (Horace->VerX(false) > 280) &&
          (Horace->VerX(false) < 310) &&
          (Horace->VerTipo () == HORACIO) &&
          (Dinero >= 10)) {
               Dinero -=10;
               H->TextOutCentre (SMS->Leer(4),0,-1,1);
               Horace->Borra();
               Horace->ConEsqui();
               Horace->Dibuja();
               H->VuelcaBuffer();
               SFX->SCash();
               Vidas = 3;
               H->Rest (1000);
               H->DrawFondoBuffer (0,0,ANCHOPANTALLA,H->TextHeight(),0,0);               
               InitTemp ();
               }

      if ((Horace->VerY(false) < H->TextHeight() + 8) &&
          (Horace->VerTipo() == HORACIOCS))
          Salir = FINFASEOK;

      if (Dinero<0) Salir = SINDINERO;
      CiclosHechos++;
      H->VuelcaBuffer();     
//      }vsync();  // Si quiero poner el vsync, hay que dejarlo, junto con el VuelcaBuffer fuera del ciclo ppal, que solo se ejecuta las x veces x seg que queramos
  }

  } while (!Salir);
  SFX->SAmb_ex(true);
  remove_int (Ret1);
  

  return Salir;
}
