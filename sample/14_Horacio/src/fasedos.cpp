#include "fasedos.h"

volatile unsigned int CiclosTempF2;

void Ret2 (void) {
     CiclosTempF2++;
}

END_OF_FUNCTION(Ret2)

extern Hardware *H;
extern Mensajes *SMS;
extern HiScore *Record;
extern Sonido *SFX;
extern int Dinero, Puntos, Vidas, Nivel, ProximaVida; 

FaseDos::FaseDos(int Spd)
{
  Speed = Spd + 1;
  YIndice = 0;
  Pausa = new Pause;
  Descenso = new Pista;
}

FaseDos::~FaseDos()
{
  delete Pausa;
}

void FaseDos::Inicia () {
     Horace = new Esquiador;
     Horace->SetKF (20 / Speed);
     Descenso->Init();
     H->cls(7);
     H->ClrFondo (7);
     Descenso->Dibuja();
     Horace->Dibuja();
     Marcadores();
     Choque = 0;
     H->VuelcaBuffer();
     LOCK_VARIABLE (CiclosTempF2);
     LOCK_FUNCTION(Ret2);
     InitTemp ();
     install_int_ex (Ret2, BPS_TO_TIMER (200 / Speed)); // Necesito 200 frames x segundo para cada píxel
}

void FaseDos::Marcadores () {
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

void FaseDos::InitTemp () {
     CiclosHechos = 0;
     CiclosTempF2 = 0;
}

void FaseDos::Choca () {             
     SFX->SGolpe();
     Horace->Borra();
     Horace->SetTipo(CHOQUE);
     Horace->Actualiza ();
     if (--Vidas)
        H->TextOutCentre (SMS->Leer(6),0,-1,1); // Los esquís están bien
       else
        H->TextOutCentre (SMS->Leer(7),0,-1,2); // Se han roto los esquís
     InitTemp();
     
     while (CiclosHechos<(500/Speed))
         while (CiclosHechos<CiclosTempF2 && CiclosHechos<(500/Speed)) {  // Pon el sprite del choque y haz q se mueva
               Horace->Borra();
               Descenso->Borra();
               Descenso->Dibuja();
               Horace->Dibuja ();
               H->TextOutCentre (SMS->Leer(6+(Vidas==0)),0,-1,1+(Vidas==0));
               H->VuelcaBuffer();
               CiclosHechos++;}
     InitTemp();
     Horace->SetTipo(ESQUIADOR);  // Deja de ser un choque y vuelve a ser esquiador
     Horace->Actualiza ();
}

int FaseDos::Pausar () {
    Descenso->Dibuja();
    Horace->Dibuja();
    Marcadores();
    H->CopyFondo();
    int Retorno = Pausa->PauseGame();

    H->SetFont ("DFFontGame_DATA", "DFFontGame_PAL");
    H->ClrFondo(7);
    H->DrawFondoBuffer (0,0,ANCHOPANTALLA,ALTOPANTALLA,0,0);
    Descenso->Dibuja();
    Marcadores();
    H->VuelcaBuffer();
    InitTemp();
    return Retorno;

}

int FaseDos::Ciclo () {
    Salida = 0;
    do {
     while (CiclosHechos<CiclosTempF2 && !Salida) {
        
        Descenso->Borra();
        Horace->Borra();
        
        FinPista = Descenso->Avanza(Speed, YIndice, Margin);
        
        if (!Choque) Tecla = H->Tecla();
           else {Tecla = Choque;
                Choque = 0;}
        if (Tecla == ABORTK) Salida = Pausar();
        Horace->Avanza(Tecla, Speed, YIndice, FinPista, Margin);

        
        Indice = Descenso->Indice();
       
        Choque = Horace->Choque (Indice, YIndice);

        if (Choque == PINO) {
           Descenso->Borra();           
           Choque = 0;
           Choca();
           if (Vidas == 0) 
              Salida = SINESQUIS;
           InitTemp();
           Descenso->SetMargin (400);
           Horace->SetY(H->TextHeight());
           H->cls (7);
           Descenso->Dibuja();}

        if (Choque == METACHOQ) {
           Descenso->Borra();           
           Choque = 0;
           Choca();
           if (Vidas == 0) 
              Salida = SINESQUIS;
            else
              Salida = FINFASEOK;
           InitTemp();}
        
        if (Choque == ABAJO) {          // Meta OK
           SFX->SAplauso();
           H->TextOutCentre (SMS->Leer(8),50,-1,1);
           Puntos +=100;
           H->VuelcaBuffer();}
           
        if (Puntos >= ProximaVida) { // Los puntos los añade al valorar el "Choque"
            ProximaVida += 1000;
            Dinero += 10;
            SFX->SExtra();}  
           
        if (FinPista && Horace->VerY(false) >= ALTOPANTALLA - 64)
           Salida = FINFASEOK;

        if (!Salida) {
           Horace->Dibuja();
           Descenso->Dibuja();
           Marcadores();                 
           H->VuelcaBuffer();
           }
           
        CiclosHechos++;}

        
        } while (!Salida);

    Descenso->DeInit();    
    delete Horace;
    remove_int (Ret2);

    if (Salida != ABORT) H->Rest (2000);

    return Salida;
}
