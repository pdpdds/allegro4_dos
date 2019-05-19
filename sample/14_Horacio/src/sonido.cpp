#include "sonido.h" // class's header file

extern Configuracion *Config;

Sonido::Sonido()
{
    Fichero = load_datafile ("datos.dat");
    Ski = (SAMPLE*)Fichero[DFSki].dat;
    Amb = (SAMPLE*)Fichero[DFAmb].dat;
    Ambul = 0;
}

Sonido::~Sonido()
{                
    unload_datafile (Fichero);
}

void Sonido::MMenu () {
     MStop();
     if (Config->VerSonido()) {
        MIDI *cancion;
        cancion = (MIDI *) Fichero[DFMusic1].dat;
        lock_midi(cancion);
        if (midi_pos<0) play_midi(cancion, 1);}
      else MStop ();
}

void Sonido::MNivel (int nivel)  {
     MStop();
     if (!Config->VerSonido()) return;
     while (nivel>3) nivel-=4;
     play_midi ((MIDI *) Fichero[DFMusicG0+nivel].dat,1);
}

void Sonido::MStop () {
     play_midi (NULL,0); }

void Sonido::SGolpe () {
     if (Config->VerSonido()) {
        play_sample ((SAMPLE *) Fichero[DFGolpe].dat,150,128,1000,0);
        play_sample ((SAMPLE *) Fichero[DFPajaros].dat,255,128,1600,0);
        }
}
        
void Sonido::SSki () {
     if (Config->VerSonido()){
        stop_sample(Ski);
        play_sample (Ski,100,128,1000,0);
        }
}

void Sonido::SAmb_ex (bool stop) {
     stop_sample(Amb);
     if (!stop && Config->VerSonido()){
        play_sample (Amb,100,128,1000,0);
        }
}

void Sonido::SAmb (bool stop) {
     if (stop && --Ambul == 0) stop_sample(Amb);
     if (Config->VerSonido() && !stop && Ambul==0){
        play_sample (Amb,100,128,1000,0);
        Ambul++;
        }
}

int Sonido::VerAmb () {
     return Ambul;}

void Sonido::SAhh () {
     if (Config->VerSonido())
        play_sample ((SAMPLE *) Fichero[DFAhh].dat,255,128,1000,0);
}     

void Sonido::SAplauso () {
     if (Config->VerSonido())
        play_sample ((SAMPLE *) Fichero[DFAplauso].dat,150,128,1000,0);
}     

void Sonido::SExtra () {
     if (Config->VerSonido())
        play_sample ((SAMPLE *) Fichero[DFExtra].dat,255,128,1000,0);
}     

void Sonido::SCash () {
     if (Config->VerSonido())
        play_sample ((SAMPLE *) Fichero[DFCash].dat,255,128,1000,0);
}     

void Sonido::SFin () {
     if (Config->VerSonido())
        play_sample ((SAMPLE *) Fichero[DFFinal].dat,150,128,1000,0);
}     
