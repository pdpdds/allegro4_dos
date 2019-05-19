#include "sprmanager.h"

extern Configuracion *Config;

SprManager::SprManager()
{
    FTime = false;
    Fichero = load_datafile ("datos.dat");
    
    switch (CDEPTH) {
           case 8:
                Trans = MASK_COLOR_8;
                break;
           case 15:
                Trans = MASK_COLOR_15;
                break;
           case 16:
                Trans = MASK_COLOR_16;
                break;
           case 24:
                Trans = MASK_COLOR_24;
                break;
           case 32:
                Trans = MASK_COLOR_32;
                break;
                }

    // Creaci? de bitmaps ***********************************************************************

    for (n=0; n<FotogramasVehiculo; n++) {
        Vehiculo  [0][n] = create_bitmap_ex(CDEPTH, AnchoCamion, AltoCamion);
        Vehiculo  [1][n] = create_bitmap_ex (CDEPTH,AnchoCamion, AltoCamion);
        Vehiculo  [2][n] = create_bitmap_ex (CDEPTH,AnchoCoche1, AltoCoche1);
        Vehiculo  [3][n] = create_bitmap_ex (CDEPTH,AnchoCoche2, AltoCoche2);
        Vehiculo  [4][n] = create_bitmap_ex (CDEPTH,AnchoAmbulancia, AltoAmbulancia);
        Vehiculo  [5][n] = create_bitmap_ex (CDEPTH,AnchoMoto, AltoMoto);
        Vehiculo  [6][n] = create_bitmap_ex (CDEPTH,AnchoCamion, AltoCamion);
        Vehiculo  [7][n] = create_bitmap_ex (CDEPTH,AnchoCamion, AltoCamion);
        Vehiculo  [8][n] = create_bitmap_ex (CDEPTH,AnchoCoche1, AltoCoche1);
        Vehiculo  [9][n] = create_bitmap_ex (CDEPTH,AnchoCoche2, AltoCoche2);
        Vehiculo [10][n] = create_bitmap_ex (CDEPTH,AnchoAmbulancia, AltoAmbulancia);
        Vehiculo [11][n] = create_bitmap_ex (CDEPTH,AnchoMoto, AltoMoto);
        }

    for (n=0; n<4; n++)
        for (m= 0; m<FotogramasHoracio; m++)
            Horacio [n][m] = create_bitmap_ex (CDEPTH, AnchoHoracio, AltoHoracio);
    
    for (n=0; n<4; n++)
        for (m= 0; m<FotogramasHoracio; m++)
            HoracioCS [n][m] = create_bitmap_ex (CDEPTH, AnchoHoracioCS, AltoHoracio);
    
    for (n = 0; n<FotogramasEsquiador; n++)
        Esquiador[0][n] = create_bitmap_ex (CDEPTH, 32, 44); // 32 x 44 Medidas del Sprite de Horac descend.
    
    for (n = 1; n < 5; n++) 
        for (m = 0; m<FotogramasEsquiador; m++) 
            Esquiador [n][m] = create_bitmap_ex (CDEPTH, AnchoEsquiador, AltoEsquiador);
    
    for (n = 0; n < FotogramasBand; n++) {
        Bandera [0][n] = create_bitmap_ex (CDEPTH, AnchoB1, AltoBand);
        Bandera [1][n] = create_bitmap_ex (CDEPTH, AnchoB2, AltoBand);}
    
    Monton = create_bitmap_ex (CDEPTH, AnchoMonton, AltoMonton);
    
    for (n = 0; n < 2; n++)
        Pino[n] = create_bitmap_ex (CDEPTH, AnchoPino, AltoPino);
    
    for (n = 0; n<FotogramasChoque; n++)
        Choque [n] = create_bitmap_ex (CDEPTH, AnchoChoque, AltoChoque);
    
    Meta [0] = create_bitmap_ex (CDEPTH, AnchoM1, AltoMeta);
    Meta [1] = create_bitmap_ex (CDEPTH, AnchoM2, AltoMeta); 
        
// Creaci? de m?caras *************************************************************************

    for (n=0;n<12;n++)
        for (m=0;m<FotogramasVehiculo;m++) 
            MVehiculo [n][m] = CreaMascara (Vehiculo[n][m], FotogramasVehiculo);

    for (n=0;n<4;n++)
        for (m=0;m<FotogramasHoracio;m++) {
            MHoracio [n][m] = CreaMascara (Horacio[n][m], FotogramasHoracio);
            MHoracioCS [n][m] = CreaMascara (HoracioCS[n][m], FotogramasHoracio);
            }

    for (n=0;n<5;n++)
        for (m=0;m<FotogramasEsquiador;m++)
            MEsquiador[n][m] = CreaMascara (Esquiador[n][m], FotogramasEsquiador);

    for (n=0;n<2;n++)
        for (m=0;m<FotogramasBand;m++)
            MBandera [n][m] = CreaMascara (Bandera [n][m], FotogramasBand);

    for (n=0;n<2;n++) {
        MPino[n] = CreaMascara (Pino[n],1);
        MMeta[n] = CreaMascara (Meta[n],1);
        }

    MMonton = CreaMascara (Monton,1);

    for (n = 0; n < FotogramasChoque; n++)
        MChoque[n] = CreaMascara (Choque[n], FotogramasChoque);
        
}

SprManager::~SprManager()
{
// Destrucci? de Bitmaps *****************************************************

    for (n=0; n<12; n++)
        for (m=0; m<FotogramasVehiculo; m++)
            destroy_bitmap (Vehiculo [n][m]);

    for (n=0; n<4; n++)
        for (m= 0; m<FotogramasHoracio; m++) {
            destroy_bitmap (Horacio [n][m]);
            destroy_bitmap (HoracioCS [n][m]); }
    
    for (n=0; n<5; n++)
        for (m= 0; m<FotogramasEsquiador; m++)
            destroy_bitmap (Esquiador [n][m]);

    for (n=0; n<2; n++) {
        destroy_bitmap (Meta [n]);
        destroy_bitmap (Pino [n]);
        for (m = 0; m<FotogramasBand; m++)
            destroy_bitmap (Bandera [n][m]);
    }
    
    for (n=0; n<FotogramasChoque; n++)
        destroy_bitmap (Choque [n]);  
        
    destroy_bitmap (Monton);
       
// Destrucci? de m?caras ************************************************************

    
    for (n=0; n<12; n++)
        for (m=0; m<FotogramasVehiculo; m++)
            DestrMascara (MVehiculo[n][m]);

    for (n=0; n<4; n++)
        for (m= 0; m<FotogramasHoracio; m++) {
            DestrMascara (MHoracio [n][m]);
            DestrMascara (MHoracioCS [n][m]);}

    for (n=0; n<5; n++)
        for (m= 0; m<FotogramasEsquiador; m++)
            DestrMascara (MEsquiador [n][m]);

    for (n=0; n<2; n++)
        for (m= 0; m<FotogramasBand; m++)
            DestrMascara (MBandera [n][m]);
            
    for (n=0; n<2; n++) {
        DestrMascara (MMeta [n]);
        DestrMascara (MPino [n]);
        }

    DestrMascara (MMonton);
    
    for (n = 0; n < FotogramasChoque; n++)
        DestrMascara (MChoque [n]);

// Destrucci? de sprites ***********************************************************

   DelSprites ();    

    unload_datafile (Fichero);
}

void SprManager::DelSprites () {
    // Destrucci? de sprites ***********************************************************
    
    if (!FTime) return;

    for (n = 0; n < 12; n++)
        for (m = 0; n < FotogramasVehiculo; n++)
            destroy_compiled_sprite (SVehiculo[n][m]);

    for (n=0; n<4; n++)
        for (m= 0; m<FotogramasHoracio; m++) {
            destroy_compiled_sprite (SHoracio[n][m]);
            destroy_compiled_sprite (SHoracioCS[n][m]);
            }
       
    for (n = 0; n < 5; n++) 
        for (m = 0; m<FotogramasEsquiador; m++) 
            destroy_compiled_sprite (SEsquiador [n][m]);
    
    for (n = 0; n < 2; n++)
        for (m = 0; m < FotogramasBand; m++)
            destroy_compiled_sprite (SBandera [n][m]);
        
    destroy_compiled_sprite (SMonton);
    
    for (n = 0; n < 2; n++) {
        destroy_compiled_sprite (SPino[n]);
        destroy_compiled_sprite (SMeta[n]);
        }
    
    for (n = 0; n<FotogramasChoque; n++)
        destroy_compiled_sprite (SChoque [n]);
}     

void SprManager::Init () {

   SprNum = DFSprites0 + Config->SetGraf();
    
    // Inicializaci? de Bitmaps ****************************************

    for (n=0; n<FotogramasVehiculo; n++) {         
        PixelOrigen = 0;
        blit ((BITMAP*)Fichero[SprNum].dat,Vehiculo [0][n],AnchoCamion*n,PixelOrigen,0,0,AnchoCamion, AltoCamion);
        PixelOrigen += AltoCamion;        
        blit ((BITMAP*)Fichero[SprNum].dat,Vehiculo [1][n],AnchoCamion*n,PixelOrigen,0,0,AnchoCamion, AltoCamion);
        PixelOrigen += AltoCamion;
        blit ((BITMAP*)Fichero[SprNum].dat,Vehiculo [2][n],AnchoCoche1*n,PixelOrigen,0,0,AnchoCoche1, AltoCoche1);
        PixelOrigen += AltoCoche1;
        blit ((BITMAP*)Fichero[SprNum].dat,Vehiculo [3][n],AnchoCoche2*n,PixelOrigen,0,0,AnchoCoche2, AltoCoche2);
        PixelOrigen += AltoCoche2;
        blit ((BITMAP*)Fichero[SprNum].dat,Vehiculo [4][n],AnchoAmbulancia*n,PixelOrigen,0,0,AnchoAmbulancia, AltoAmbulancia);
        PixelOrigen += AltoAmbulancia;
        blit ((BITMAP*)Fichero[SprNum].dat,Vehiculo [5][n],AnchoMoto*n,PixelOrigen,0,0,AnchoMoto, AltoMoto);
        PixelOrigen += AltoMoto;
        blit ((BITMAP*)Fichero[SprNum].dat,Vehiculo [6][n],AnchoCamion*n,PixelOrigen,0,0,AnchoCamion, AltoCamion);
        PixelOrigen += AltoCamion;
        blit ((BITMAP*)Fichero[SprNum].dat,Vehiculo [7][n],AnchoCamion*n,PixelOrigen,0,0,AnchoCamion, AltoCamion);
        PixelOrigen += AltoCamion;
        blit ((BITMAP*)Fichero[SprNum].dat,Vehiculo [8][n],AnchoCoche1*n,PixelOrigen,0,0,AnchoCoche1, AltoCoche1);
        PixelOrigen += AltoCoche1;
        blit ((BITMAP*)Fichero[SprNum].dat,Vehiculo [9][n],AnchoCoche2*n,PixelOrigen,0,0,AnchoCoche2, AltoCoche2);
        PixelOrigen += AltoCoche2;
        blit ((BITMAP*)Fichero[SprNum].dat,Vehiculo [10][n],AnchoAmbulancia*n,PixelOrigen,0,0,AnchoAmbulancia, AltoAmbulancia);
        PixelOrigen += AltoAmbulancia;
        blit ((BITMAP*)Fichero[SprNum].dat,Vehiculo [11][n],AnchoMoto*n,PixelOrigen,0,0,AnchoMoto, AltoMoto);
        PixelOrigen += AltoMoto;
        }

    for (n=0; n<4; n++){
        for (m= 0; m<FotogramasHoracio; m++)
            blit ((BITMAP*)Fichero[SprNum].dat,Horacio [n][m],AnchoHoracio*m,PixelOrigen,0,0,AnchoHoracio, AltoHoracio);
        PixelOrigen += AltoHoracio;
            }
    
    PixelOrigen -= 4 * AltoHoracio;
    
    for (n=0; n<4; n++) {
        for (m= 0; m<FotogramasHoracio; m++) {
               clear_to_color (HoracioCS[n][m], Trans);
               blit ((BITMAP*)Fichero[SprNum].dat,HoracioCS [n][m],AnchoHoracio*m,PixelOrigen,0,0,AnchoHoracio, AltoHoracio);
               masked_blit ((BITMAP*)Fichero[SprNum].dat,HoracioCS [n][m],(AnchoHoracio*FotogramasHoracio)+AnchoSki*m,448,AnchoHoracio-2,0,AnchoSki, AltoHoracio);
               }        
        PixelOrigen += AltoHoracio;
            }
    
    for (n = 0; n<FotogramasEsquiador; n++) 
        blit ((BITMAP*)Fichero[SprNum].dat, Esquiador [0][n], 32 * n, PixelOrigen, 0,0, 32, 44);
    PixelOrigen += 44;
    
    for (n = 1; n < 5; n++) {
        for (m = 0; m<FotogramasEsquiador; m++) {
            clear_to_color (Esquiador[n][m],Trans);
            blit ((BITMAP*)Fichero[SprNum].dat,Esquiador[n][m],AnchoEsquiador*m, PixelOrigen,0,0,AnchoEsquiador,AltoEsquiador);
            }
        PixelOrigen += AltoEsquiador;
        }
    
    for (n = 0; n < FotogramasBand; n++) {
        clear_to_color (Bandera[0][n],Trans);
        clear_to_color (Bandera[1][n],Trans);       
        blit ((BITMAP*)Fichero[SprNum].dat, Bandera[0][n], AnchoBand * 2 * n, PixelOrigen, 0, 0, AnchoBand, AltoBand);
        blit ((BITMAP*)Fichero[SprNum].dat, Bandera[0][n], (AnchoBand * 2 * n) + AnchoBand, PixelOrigen, AnchoB1 - AnchoBand, 0, AnchoBand, AltoBand);
        blit ((BITMAP*)Fichero[SprNum].dat, Bandera[1][n], AnchoBand * 2 * n, PixelOrigen, 0, 0, AnchoBand, AltoBand);
        blit ((BITMAP*)Fichero[SprNum].dat, Bandera[1][n], (AnchoBand * 2 * n) + AnchoBand, PixelOrigen, AnchoB2 - AnchoBand, 0, AnchoBand, AltoBand);
        }
    PixelOrigen += AltoBand;
    
    blit ((BITMAP*)Fichero[SprNum].dat, Monton, 0, PixelOrigen, 0, 0, AnchoMonton, AltoMonton);
    PixelOrigen += AltoMonton;
    
    for (n = 0; n < 2; n++)
        blit ((BITMAP*)Fichero[SprNum].dat, Pino[n], 0 + AnchoPino * n, PixelOrigen, 0, 0, AnchoPino, AltoPino);
    PixelOrigen += AltoPino;
    
    for (n = 0; n<FotogramasChoque; n++)
        blit ((BITMAP*)Fichero[SprNum].dat, Choque[n], AnchoChoque * n, PixelOrigen, 0,0, AnchoChoque, AltoChoque);
    PixelOrigen += AltoChoque;
    
    clear_to_color (Meta[0],Trans);
    clear_to_color (Meta[1],Trans);       
    blit ((BITMAP*)Fichero[SprNum].dat, Meta[0], 0, PixelOrigen, 0, 48, AnchoMeta, AltoMeta);
    blit ((BITMAP*)Fichero[SprNum].dat, Meta[0], 0, PixelOrigen, AnchoM1 - AnchoMeta, 48, AnchoMeta, AltoMeta);
    blit ((BITMAP*)Fichero[SprNum].dat, Meta[1], 0, PixelOrigen, 0, 48, AnchoMeta, AltoMeta);
    blit ((BITMAP*)Fichero[SprNum].dat, Meta[1], 0, PixelOrigen, AnchoM2 - AnchoMeta, 48, AnchoMeta, AltoMeta);
    PixelOrigen += 64;
    stretch_blit((BITMAP *)Fichero[SprNum].dat, Meta[0], 0, PixelOrigen, 128, 48, 0, 0, AnchoM1-17,48);
    stretch_blit((BITMAP *)Fichero[SprNum].dat, Meta[1], 0, PixelOrigen, 128, 48, 0, 0, AnchoM2-17,48);

    // Creaci? e inicializaci? de sprites ****************************************
    
    if (FTime) DelSprites(); // Si han sido iniciados los sprites, b?ralos;
       else                 // si no, 
      FTime = true;         // considera que ya se han iniciado.

    for (n = 0; n < 12; n++)
        for (m = 0; m < FotogramasVehiculo; m++)
            SVehiculo[n][m] = get_compiled_sprite (Vehiculo[n][m],0);

    for (n=0; n<4; n++)
        for (m= 0; m<FotogramasHoracio; m++) {
            SHoracio [n][m] = get_compiled_sprite (Horacio[n][m],0);
            SHoracioCS [n][m] = get_compiled_sprite (HoracioCS[n][m],0);
            }
       
    for (n = 0; n < 5; n++) 
        for (m = 0; m<FotogramasEsquiador; m++) 
            SEsquiador [n][m] = get_compiled_sprite (Esquiador [n][m],0);
    
    for (n = 0; n < 2; n++)
        for (m = 0; m < FotogramasBand; m++)
            SBandera [n][m] = get_compiled_sprite (Bandera [n][m],0);
        
    SMonton = get_compiled_sprite (Monton,0);
    
    for (n = 0; n < 2; n++) {
        SPino[n] = get_compiled_sprite (Pino[n],0);
        SMeta[n] = get_compiled_sprite (Meta[n],0);
        }
    
    for (n = 0; n<FotogramasChoque; n++)
        SChoque [n] = get_compiled_sprite (Choque [n],0);


// Inicializaci? de M?caras ******************************************************************

    for (n=0;n<12;n++)
        for (m=0;m<FotogramasVehiculo;m++)
            InitMascara (Vehiculo[n][m], MVehiculo[n][m]);

    for (n=0;n<4;n++)
        for (m=0;m<FotogramasHoracio;m++) {
            InitMascara (Horacio[n][m], MHoracio[n][m]);
            InitMascara (HoracioCS[n][m], MHoracioCS[n][m]);
            }

    for (n=0;n<5;n++)
        for (m=0;m<FotogramasEsquiador;m++)
            InitMascara (Esquiador[n][m], MEsquiador [n][m]);

    for (n=0;n<2;n++)
        for (m=0;m<FotogramasBand;m++)
            InitMascara (Bandera[n][m], MBandera[n][m]);

    for (n=0;n<2;n++)
        InitMascara (Pino[n], MPino[n]);

    InitMascara (Monton, MMonton);

    for (n=0;n<2;n++)
        InitMascara (Meta[n], MMeta[n],48);
/*
     mask *Prueba;
     Prueba = MHoracioCS[0][1];

     for (n = 0; n < Prueba->LongY; n++)
         for (m = 0; m < Prueba->Words; m++)
             for (o = 0; o < Tamano; o++)
                 if (Prueba->MaskData[n][m] & 0x0001 << o)
                    textout_ex (screen, font, "*",(m * Tamano + o)* 4, 4 * n, 
                               makecol(255,255,255),0);
     readkey();//*/

}

mask * SprManager::CreaMascara (BITMAP *BitMap, int Fotogr)
{
      mask *Retorno;
      int b1;
      Retorno = new mask;
      Retorno->LongX = BitMap->w;
      Retorno->LongY = BitMap->h;
      Retorno->Fotog = Fotogr;
      Retorno->Words = Retorno->LongX / Tamano;
      if (Retorno->LongX % Tamano) Retorno->Words += 1;
      Retorno->MaskData = new word*[Retorno->LongY];
      for (b1 = 0; b1 < Retorno->LongY; b1++)
          Retorno->MaskData[b1] = new word [Retorno->Words];
          
      return Retorno;
}

void SprManager::InitMascara (BITMAP *BitMap, mask *Mascara, int YOrig)
 {
    int b1,b2,b3;
    
    for (b1 = 0; b1 < Mascara->LongY; b1++)
        for (b2 = 0; b2 < Mascara->Words; b2++)
            Mascara->MaskData[b1][b2] = 0;
            
    for (b1 = YOrig; b1 < Mascara->LongY; b1++)
        for (b2 = 0; b2 < Mascara->Words; b2++)
            for (b3 = 0; b3 < Tamano; b3++)
                if ((getpixel (BitMap, (b2 * Tamano) + b3, b1) >= 0) &&
                    (getpixel (BitMap, (b2 * Tamano) + b3, b1) != Trans))
                    Mascara->MaskData[b1][b2] += (1 << b3);
}

int SprManager::DestrMascara (mask *Mascara)
{
    int b1;
    if (!Mascara) return -1;
    for (b1 = 0; b1 < Mascara->LongY; b1++)
        delete[] Mascara->MaskData[b1];
    delete[] Mascara->MaskData;
    delete Mascara;
    if (Mascara) return 1;
    return 0;
}

COMPILED_SPRITE * SprManager::Sprite (int Tipo, int Sentido, int Fotograma) {
     int a;
       
     switch (Tipo) {
            case HORACIO:
                 return SHoracio[Sentido][Fotograma];

            case HORACIOCS:
                 return SHoracioCS[Sentido][Fotograma];

            case ESQUIADOR:
                 return SEsquiador[Sentido][Fotograma];

            case BANDERA:
                 return SBandera[Sentido][Fotograma];

            case MONTON:
                 return SMonton;

            case PINO:
                 return SPino[Sentido];

            case CHOQUE:
                 return SChoque[Fotograma];

            case META:
                 return SMeta[Sentido];
      }

      a = Tipo;         // Entonces es un veh?ulo ... fijo! ;-)
      if (Sentido < 0) a += TotalVehiculos;
      return SVehiculo[a][Fotograma];
}

mask *SprManager::Mask (int Tipo, int Sentido, int Fotograma) {
     int a;
     
     switch (Tipo) {
            case HORACIO:
                 return MHoracio[Sentido][Fotograma];

            case HORACIOCS:
                 return MHoracioCS[Sentido][Fotograma];

            case ESQUIADOR:
                 return MEsquiador[Sentido][Fotograma];

            case BANDERA:
                 return MBandera[Sentido][Fotograma];

            case MONTON:
                 return MMonton;

            case PINO:
                 return MPino[Sentido];

            case CHOQUE:
                 return MChoque[Fotograma];

            case META:
                 return MMeta[Sentido];
     }

     a = Tipo;
     if (Sentido < 0) a += TotalVehiculos;
     return MVehiculo[a][Fotograma];
}
