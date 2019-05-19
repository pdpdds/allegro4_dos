#ifndef SPRMANAGER_H
#define SPRMANAGER_H

#include "constantes.h"
#include "configuracion.h"
#include <allegro.h>

const int FotogramasVehiculo = 4;
const int FotogramasHoracio = 2;
const int FotogramasEsquiador = 1;
const int FotogramasChoque = 3;
const int FotogramasBand = 4;
const int TotalVehiculos = 6;

const int AltoCamion = 48;
const int AltoCoche1 = 32;
const int AltoCoche2 = 32;
const int AltoAmbulancia = 32;
const int AltoMoto = 32;
const int AltoHoracio = 32;
const int AltoEsquiador = 40;
const int AltoBand = 32;
const int AltoMonton = 10;
const int AltoPino = 64;
const int AltoChoque = 45;
const int AltoMeta = 112;

const int AnchoCamion = 112;
const int AnchoCoche1 = 64;
const int AnchoCoche2 = 64;
const int AnchoAmbulancia = 64;
const int AnchoMoto = 32;
const int AnchoHoracio = 32;
const int AnchoHoracioCS = 46;
const int AnchoSki = 16;
const int AnchoEsquiador = 42;
const int AnchoBand = 14;
const int AnchoB1 = 84;
const int AnchoB2 = 130;
const int AnchoMonton = 48;
const int AnchoPino = 48;
const int AnchoChoque = 48;
const int AnchoMeta = 25;
const int AnchoM1 = 100;
const int AnchoM2 = 150;

extern const int Tamano;

class SprManager
{
	public:
		SprManager();
		~SprManager();
		COMPILED_SPRITE * Sprite (int Tipo, int Sentido, int Fotograma);
		mask * Mask (int Tipo, int Sentido, int Fotograma);
        void Init();

	private:
        DATAFILE *Fichero;
        BITMAP *Vehiculo [12][FotogramasVehiculo], *Horacio [4][FotogramasHoracio], *HoracioCS [4][FotogramasHoracio],
               *Esquiador [5][FotogramasEsquiador], *Choque [FotogramasChoque], *Bandera[2][FotogramasBand], *Monton, *Pino[2],
               *Meta[2];
        COMPILED_SPRITE *SVehiculo [12][FotogramasVehiculo], *SHoracio [4][FotogramasHoracio], *SHoracioCS [4][FotogramasHoracio],
                   *SEsquiador [5][FotogramasEsquiador], *SChoque [FotogramasChoque], *SBandera[2][FotogramasBand], *SMonton, *SPino[2],
                   *SMeta[2];
        mask   *MVehiculo [12][FotogramasVehiculo], *MHoracio [4][FotogramasHoracio], *MHoracioCS [4][FotogramasHoracio],
               *MEsquiador [5][FotogramasEsquiador], *MChoque [FotogramasChoque], *MBandera[2][FotogramasBand], *MMonton, *MPino[2],
               *MMeta[2];
        int n, m, o, p, q; // Variables para los bucles for
        int PixelOrigen, WordsAncho, Alto, Ancho, Trans, SetGraf, SprNum;
        bool FTime;
        void DelSprites ();
        mask * CreaMascara (BITMAP *BitMap, int Fotogr);
        void InitMascara (BITMAP *BitMap, mask *Mascara, int YOrig = 0);        
        int DestrMascara (mask *Mascara);
};

#endif
