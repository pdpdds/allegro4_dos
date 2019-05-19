#include "pause.h"

extern Hardware *H;
extern Mensajes *SMS;

volatile unsigned int CiclosTempPause;

void RetP (void) {
     CiclosTempPause++;
}

END_OF_FUNCTION(RetP)

Pause::Pause()
{
 LOCK_VARIABLE (CiclosTempPause);
 LOCK_FUNCTION(RetP);
}

Pause::~Pause()
{
}

void Pause::InitTemp () {
     CiclosTempPause = 0;
     CiclosHechos = 0;
}

void Pause::Rectangulo () {
     int n;
     
    for (n = 1; n < 10; n++) {  // Esto hace que crezca el rect?gulo
        H->DrawFondoBuffer(ANCHOPANTALLA/2 - n*19,ALTOPANTALLA/2 - n*8,n*38,n*16,ANCHOPANTALLA/2 - n*19,ALTOPANTALLA/2 - n*8);
        H->RectFill_ex (ANCHOPANTALLA/2 - n*19, ALTOPANTALLA/2 - n*8,
                     ANCHOPANTALLA/2 + n*19, ALTOPANTALLA/2 + n*8,
                     makecol (180,180,0));
        H->RectFill_ex (ANCHOPANTALLA/2 - n*18, ALTOPANTALLA/2 - n*7,
                     ANCHOPANTALLA/2 + n*18, ALTOPANTALLA/2 + n*7,
                     makecol (0,100,180));
        H->VuelcaBuffer();
        H->Rest (15);}
        
    H->DrawFondoBuffer(ANCHOPANTALLA/2 - 190, ALTOPANTALLA / 2 - 80, 190*2, 80 *2, ANCHOPANTALLA/2-190,ALTOPANTALLA/2-80);
    H->TransMode(170);
    H->RectFill_ex (ANCHOPANTALLA/2 - 190, ALTOPANTALLA/2 - 80,
                    ANCHOPANTALLA/2 + 190, ALTOPANTALLA/2 + 80,
                    makecol (180,180,0));
    H->RectFill_ex (ANCHOPANTALLA/2 - 180, ALTOPANTALLA/2 - 70,
                    ANCHOPANTALLA/2 + 180, ALTOPANTALLA/2 + 70,
                    makecol (0,100,180));   
    H->SolidMode();
}

void Pause::Texto () {
    H->SetFont ("DFFontMenu_DATA", "DFFontMenu_PAL");
    H->TextOut(SMS->Leer(35),195,195,-1,-1);
    H->TextOut(SMS->Leer(36),195,245,-1,-1);
    H->VuelcaBuffer();
    H->CopyFondo();
}

void Pause::Desplaza (int Sentido) {
     switch (Sentido) {
            case ABAJO:
                 if (OpcionAct > 0) return;
                 Selector->ActualizaMov(ABAJO);
                 OpcionAct++;
                 InitTemp();
                 while (Selector->VerY(false) < 250) 
                   while (CiclosHechos < CiclosTempPause && Selector->VerY(false) < 250){
                       Selector->Borra();
                       Selector->Mueve();
                       Selector->Dibuja();
                       H->VuelcaBuffer();
                       CiclosHechos++;}
                 Selector->Borra();
                 Selector->ActualizaMov(DERECHA);
                 Selector->Dibuja();
                 H->ClearKeyBuf();
                 break;
                 
            case ARRIBA:
                 if (OpcionAct < 1) return;
                 Selector->ActualizaMov(ARRIBA);
                 OpcionAct--;
                 InitTemp();
                 while (Selector->VerY(false) > 200) 
                   while (CiclosHechos < CiclosTempPause && Selector->VerY(false) > 200){
                       Selector->Borra();
                       Selector->Mueve();
                       Selector->Dibuja();
                       H->VuelcaBuffer();
                       CiclosHechos++;}
                 Selector->Borra();
                 Selector->ActualizaMov(DERECHA);
                 Selector->Dibuja();
                 H->ClearKeyBuf();
                 break;
                 
            case SELECC:
                 while (Selector->VerX() < 450)
                       while (CiclosHechos<CiclosTempPause && Selector->VerX() < 450) {
                       Selector->Borra();
                       Selector->Mueve();
                       Selector->Mueve();
                       Selector->Dibuja();
                       H->VuelcaBuffer();
                       CiclosHechos++;}

     }

}

int Pause::PauseGame () {
    
    Retorno = 0, SalirBucle = 0, OpcionAct = 0;

    Selector = new Horacio;
    Selector->SetKF (20);
    
    Rectangulo ();
    Texto();

    Selector->ActualizaMov(DERECHA);
    Selector->SetX(160);
    Selector->SetY(200);
    Selector->Dibuja();

    install_int_ex (RetP, BPS_TO_TIMER (200));
    
    InitTemp();
    while (!SalirBucle)
      while (CiclosHechos<CiclosTempPause && !SalirBucle) {
            Selector->Borra();
            Selector->Dibuja();
            Tecla = H->Tecla();
            Desplaza (Tecla);
            if (Tecla == SELECC) SalirBucle = true;
            H->VuelcaBuffer();
            CiclosHechos++;}
    

    if (OpcionAct == 1) Retorno = ABORT;

    remove_int (RetP);
    delete Selector;

    return Retorno;
}
