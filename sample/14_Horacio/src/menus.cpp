#include "menus.h"

extern Hardware *H;
extern Mensajes *SMS;
extern SprManager *M;
extern Configuracion *Config;
extern HiScore *Record;
extern Sonido *SFX;

volatile unsigned int CiclosTempMenu;

void RetM (void) {
     CiclosTempMenu++;
}

END_OF_FUNCTION(RetM)

Menus::Menus()
{
 Horace = new Horacio;
 Horace->SetX(100);
 Horace->SetY(230);
 Horace->SetKF (10);
 LOCK_VARIABLE (CiclosTempMenu);
 LOCK_FUNCTION(RetM);
}

Menus::~Menus()
{
delete Horace;
}

int Menus::MenuPpal () {
    SFX->MMenu();
    InitTemp ();
    install_int_ex (RetM, BPS_TO_TIMER (100));
    Salida = 0;
    OpcionAct = 0;
    Prepara ();
    PpalTxt();
    H->CopyFondo();
    H->Fundido(1);
    InitTemp();
    H->VuelcaBuffer();
    while (!Salida) {
          while (CiclosHechos < CiclosTempMenu) {
          Horace->Borra();
          Horace->Dibuja();
          H->VuelcaBuffer();
          Tecla = H->Tecla();
          H->ClearKeyBuf();
          Desplaza(Tecla, 4);
          CiclosHechos++;
          if (Tecla == SELECC && !Salida) {
             switch (OpcionAct) {
                    case 0: // Iniciar juego
                         Salida = FINFASEOK;
                         break;
                    case 1: // Cambiar idioma
                         Config->ToggleIdioma();
                         break;
                    case 2: // Configuraci?
                         MenuConfig();
                         break;
                    case 3: // Ver r?ords
                         Prepara();
                         H->SetFont ("DFFontGame_DATA", "DFFontGame_PAL");
                         for (n = 0; n < 10; n++) {
                             char Temp [40], num[20];
                             strcpy (Temp, Record->LeeNombre(n));
                             while (strlen(Temp) < 9)
                                   strcat (Temp," ");
                             sprintf (num, " . . . . . . %i", Record->LeePuntos(n));
                             strcat (Temp, num);
                             H->TextOut(Temp,175,245 + 20*n, -1,-1);}                                
                         H->VuelcaBuffer();
                         H->EsperaTecla();
                         break;
                    case 4: // Cr?itos
                         H->Fundido (-1);
                         InitTemp();
                         Creditos();
                         break;
                    case 5: // Salida
                         Salida = ABORT;
                         H->Fundido (-1);
                         break;
                    }

             Horace->SetY(230);
             OpcionAct = 0;                         

             Prepara();
             PpalTxt();
             H->CopyFondo();
             H->VuelcaBuffer();
             H->ClearKeyBuf();
             InitTemp();}
          }
    }
    remove_int (RetM);
    SFX->MStop();
    return Salida;
}

int Menus::MenuConfig () {
    char Temp[1];
    InitTemp ();
    OpcionAct = 0;
    Prepara ();
    CfgTxt();
    H->CopyFondo();
    Horace->SetY(230);
    while (!false) {
          while (CiclosHechos < CiclosTempMenu) {
          Horace->Borra();
          Horace->Dibuja();
          H->VuelcaBuffer();
          Tecla = H->Tecla();
          H->ClearKeyBuf();
          Desplaza(Tecla, 4);
          CiclosHechos++;
          if (Tecla == SELECC) {
             switch (OpcionAct) {
                    case 0:  // Cambiar modo gr?ico
                         Config->ToggleWindowed();
                         H->DeInit();
                         H->InitGraph();
                         H->Init();
                         break;
                    case 1:  // Cambiar sonido
                         Config->ToggleSonido();
                         SFX->MMenu();
                         break;
                    case 2:  // Definir teclado
                         Prepara ();
                         H->SetFont ("DFFontMenu_DATA", "DFFontMenu_PAL");
                         H->TextOutCentre (SMS->Leer(34), 230, -1,-1);
                         H->SetFont ("DFFontGame_DATA", "DFFontGame_PAL");
                         for (n=0; n<6; n++) H->GrabarTecla (ARRIBA+n, 0); // Primero borra todas las teclas anteriores
                         for (n=0; n<6; n++) {
                             H->TextOutCentre(SMS->Leer(25+n), 280 + 24 * n, -1,-1);
                             H->VuelcaBuffer();
                             H->ClearKeyBuf();
                             do {
                                Tecla = H->VerTecla();
                                if (Tecla > 0)
                                   for (m = 0; m < n; m++)
                                       if (Tecla >> 8 == H->VerTecla(m)) // Mira a ver si esta tecla ya est?definida
                                          Tecla = -1;
                             } while (Tecla<0);
                             H->GrabarTecla (ARRIBA+n, Tecla);}
                         rest (1000);
                         break;                         
                    case 3:  // Cambiar set gr?ico
                         Config->ToggleSet();
                         M->Init();
                         break;
                    case 4:  // Cambiar Velocidad
                         Config->SetSpeed();
                         break;
                    case 5:  // Volver al men?Ppal
                         return 0;
                    }
          Prepara();
          CfgTxt();
          H->CopyFondo();
          InitTemp();}
          }
    }
    return 0;
}

void Menus::InitTemp () {
     CiclosHechos = 0;
     CiclosTempMenu = 0;}

void Menus::Prepara () {
    char Version[40];
    H->SetFondo(DFMenu);
    H->SetFont (DFFontMin_DATA);
    sprintf (Version, "V%i.%i%s", VMAJOR, VMINOR, VSTATUS);
    H->DrawFondoBuffer (0,0,ANCHOPANTALLA,ALTOPANTALLA,0,0);
    H->TextOut(Version, 0,0,0,-1);    
    H->SetFont ("DFFontNieve_DATA","DFFontNieve_Pal");
    Altura = H->TextHeight();
    Altura = int (0.70*Altura);
    H->TextOutCentre_ex (SMS->Leer (9), 480, 16, -1, -1);
    H->TextOutCentre_ex (SMS->Leer (10), 480, 16 + Altura , -1, -1);
    H->TextOutCentre_ex (SMS->Leer (11), 480, 16 + 2*Altura, -1, -1);
}

void Menus::PpalTxt () {
     H->SetFont ("DFFontMenu_DATA", "DFFontMenu_PAL");
     Altura = H->TextHeight();
     Altura = int (0.75*Altura);
     for (n = 0; n < 6; n++) {
         y [n] = 230 + Altura*n;    
         H->TextOut (SMS->Leer (12+n), 150, 225 + Altura*n, -1, -1);
         }
}

void Menus::CfgTxt () {
     char Temp[30], t2[5];
     H->SetFont ("DFFontMenu_DATA", "DFFontMenu_PAL");
     Altura = H->TextHeight();
     Altura = int (0.75*Altura);
     for (n = 0; n < 6; n++)
         y [n] = 230 + Altura*n;
     H->TextOut (SMS->Leer (18+Config->VerWindowed()), 150, 225, -1, -1);
     strcpy (Temp, SMS->Leer (20));
     strcat (Temp, SMS->Leer (23+Config->VerSonido()));
     H->TextOut (Temp, 150, 225 + Altura, -1,-1);
     H->TextOut (SMS->Leer (21), 150, 225 + 2 * Altura, -1, -1);
     strcpy (Temp, SMS->Leer (22));
     strcat (Temp, SMS->Leer (32+Config->SetGraf()));
     H->TextOut (Temp, 150, 225 + 3 * Altura, -1,-1);
     sprintf (Temp, "%s %i", SMS->Leer(39), Config->VerSpeed());
     H->TextOut (Temp, 150, 225 + 4 * Altura, -1, -1);
     H->TextOut (SMS->Leer(31), 150, 225 + 5 * Altura, -1,-1);
}
         
void Menus::Desplaza (int Sentido, int OpMax) {
     switch (Sentido) {
            case ABAJO:
                 if (OpcionAct > OpMax) return;
                 Horace->ActualizaMov(ABAJO);
                 OpcionAct++;
                 InitTemp();
                 while (Horace->VerY(false) < y[OpcionAct]) 
                   while (CiclosHechos < CiclosTempMenu && Horace->VerY(false) < y[OpcionAct]){
                       Horace->Borra();
                       Horace->Mueve();
                       Horace->Mueve();
                       Horace->Dibuja();
                       H->VuelcaBuffer();
                       CiclosHechos++;}
                 Horace->Borra();
                 Horace->ActualizaMov(DERECHA);
                 Horace->Dibuja();
                 H->ClearKeyBuf();
                 break;
                 
            case ARRIBA:
                 if (OpcionAct < 1) return;
                 Horace->ActualizaMov(ARRIBA);
                 OpcionAct--;
                 InitTemp();
                 while (Horace->VerY(false) > y[OpcionAct]) 
                   while (CiclosHechos < CiclosTempMenu && Horace->VerY(false) > y[OpcionAct]){
                       Horace->Borra();
                       Horace->Mueve();
                       Horace->Mueve();
                       Horace->Dibuja();
                       H->VuelcaBuffer();
                       CiclosHechos++;}
                 Horace->Borra();
                 Horace->ActualizaMov(DERECHA);
                 Horace->Dibuja();
                 H->ClearKeyBuf();
                 break;
            
            case SELECC:
                 Horace->ActualizaMov (DERECHA);
                 InitTemp();
                 install_int_ex (RetM, BPS_TO_TIMER (300));
                 while  (Horace->VerX(false) < 450)
                   while ((Horace->VerX(false) < 450) && // Repito esta condici? pq si no a veces sepasa de la coordenada 450 si
                   (CiclosHechos < CiclosTempMenu)){     // algun proceso de segundo plano ralentiza el ordenador y se carga la temporizaci?
                       Horace->Borra();
                       Horace->Mueve();
                       Horace->Mueve();
                       Horace->Dibuja();
                       H->VuelcaBuffer();
                       CiclosHechos++;}
                 Horace->SetX(100);
                 install_int_ex (RetM, BPS_TO_TIMER (100));
                 break;
     }
}

void Menus::Creditos () {
     int Indice = 0;

     H->SetFondo(DFCreditos);
     H->DrawFondoBuffer(0,0,ANCHOPANTALLA,ALTOPANTALLA,0,0);
     H->SetFont ("DFFontCredit_DATA", "DFFontCredit_PAL");
     
     H->Fundido(1);
     InitTemp();

     while (H->VerTecla() < 0)
        while (CiclosHechos<CiclosTempMenu) {
           H->DrawFondoBuffer(0,0,ANCHOPANTALLA,ALTOPANTALLA,0,0);
           for (n = 40; n < 91; n++)
               if ((Indice > (n-38) * H->TextHeight()) &&
                   ((ALTOPANTALLA-Indice) + (n * H->TextHeight()) > 0))
                         H->TextOutCentre (SMS->Leer(n), ALTOPANTALLA - Indice + ((n-38) * H->TextHeight()),-1,-1);
//           textprintf_ex (screen,font,600,0,-1,-1,"%i",Indice);
           if ((++Indice) > ALTOPANTALLA + H->TextHeight() + (52 * H->TextHeight())) // 52 Mensajes de cr?itos
              Indice = 0;
           H->VuelcaBuffer();
           CiclosHechos++;}
}
