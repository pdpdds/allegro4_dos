#include "partida.h"

extern Hardware *H;
extern Mensajes *SMS;
extern HiScore *Record;
extern Sonido *SFX;
extern Configuracion *Config;
extern int Dinero, Puntos, Vidas, Nivel, ProximaVida;

Partida::Partida()
{
    Dinero = 40;
    Puntos = 0;
    Vidas = 3;
    Nivel = 0;
    ProximaVida = 1000;
    Salida = 0;
    Fase1 = 0; Fase2 = 0;
    F1 = new FaseUno (1 + Config->VerSpeed());
    F2 = new FaseDos (Config->VerSpeed());
}

Partida::~Partida()
{
   delete F1;
   delete F2;
}

void Partida::Play () {
     
     H->SetFont ("DFFontGame_DATA","DFFontGame_PAL");

     do {
        H->Interzona (-1);
        SFX->MNivel (Nivel);
        F1->Inicia(Fase2);
        Fase1 = /*FINFASEOK; //*/F1->Ciclo();
        if ((Fase1 != SINDINERO) &&
            (Fase1 != ABORT)) {
           H->ClearKeyBuf(); 
           H->Interzona (1);
           F2->Inicia();
           Fase2 = F2->Ciclo();
           if (Fase2 == FINFASEOK) Nivel++;}
        } while ((Fase1 != SINDINERO) &&
                 (Fase1 != ABORT) &&
                 (Fase2 != ABORT));
 
     if (Fase1 == SINDINERO) {
        SFX->MStop();
        H->Interzona (1);
        F2->Inicia ();
        H->TextOutCentre (SMS->Leer(5),200,-1,2);
        H->VuelcaBuffer ();
        SFX->SFin ();
        H->Rest (5000);}
     
     H->CopyFondo ();   
     H->Fundido (-1);
     
     n = 0;
     while (n < 10)
         if (Puntos > Record->LeePuntos (n)) {
             GrabaRecord(n);
             n = 1000;}
         else n++;
}

void Partida::GrabaRecord (int Number) {
    bool Salir=false;
    char Nombre[11], CPuntos[10], Tecla = 0;
    unsigned int Puntero = 0, Altura, TempP;
    
    char Version[10], Temp[10], TempN[10];              // Este bloque prepara la pantalla
    H->SetFondo(DFMenu);                               // y pone el t?ulo
    H->SetFont ("DFFontMin_DATA","DFFontMin_PAL");
    strcpy (Version, "V. ");
    sprintf (Temp, "%i", VMAJOR);
    strcat (Version, Temp);
    strcat (Version, ".");
    sprintf (Temp, "%i", VMINOR);
    strcat (Version, Temp);
    strcat (Version, VSTATUS);
    H->DrawFondoBuffer (0,0,ANCHOPANTALLA,ALTOPANTALLA,0,0);
    H->TextOut(Version, 0,0,0,-1);    
    H->SetFont ("DFFontNieve_DATA","DFFontNieve_Pal");
    Altura = H->TextHeight();
    Altura = int (0.70*Altura);
    H->TextOutCentre_ex (SMS->Leer (9), 480, 16, -1, -1);
    H->TextOutCentre_ex (SMS->Leer (10), 480, 16 + Altura , -1, -1);
    H->TextOutCentre_ex (SMS->Leer (11), 480, 16 + 2*Altura, -1, -1);
    
    H->SetFont ("DFFontGame_DATA","DFFontGame_Pal");      // Este bloque pone el n?de puntos
    sprintf (CPuntos, "%i", Puntos);                      // y los mensajes de enhorabuena
    H->TextOutCentre (CPuntos, 400, -1, -1);
    H->SetFont ("DFFontMenu_DATA","DFFontMenu_Pal");    
    H->TextOutCentre (SMS->Leer (37), 250, -1, -1);
    H->TextOutCentre (SMS->Leer (38), 300, -1, -1);    
    H->CopyFondo();
    H->Fundido(1);
    
    H->ClearKeyBuf();
    
    while (Tecla != 13) {                              // y este se encarga del "Input"
          Tecla = H->VerTecla();
          if ((Tecla == 8) && Puntero)
             Nombre [--Puntero] = '\000';
          if ((Tecla >= '0' && Tecla <= 'z') && Puntero < 9) {
             Nombre [Puntero++] = Tecla;
             Nombre [Puntero] = '\000';}
          if (Tecla > 0) {
             H->DrawFondoBuffer(0,350,ANCHOPANTALLA,50,0,350);
             H->TextOutCentre(Nombre, 350, -1, -1);
             H->VuelcaBuffer();}
    }
    

    for (n = 9; n > Number; n--) {                   // Aqu?ajustamos el resto de la tabla
        TempP = Record->LeePuntos(n-1);
        strcpy (TempN, Record->LeeNombre(n-1));
        Record->GrabaNombre(TempN, n);
        Record->GrabaPuntos(TempP, n);}

    Record->GrabaNombre (Nombre, Number);            // Y por fin grabamos el record
    Record->GrabaPuntos (Puntos, Number);
    Record->Save();
    
    H->CopyFondo();
    H->Fundido (-1);

}
