#ifndef CONST_H
#define CONST_H

#define VMAJOR 2        // Version data
#define VMINOR 0
#define VSTATUS "F-ML"
#define ANCHOPANTALLA 640  // Screen Width
#define ALTOPANTALLA 480   // Screen Height
#define CDEPTH 16         // Color Depth
#define CAMION1 0          // Sprite data
#define CAMION2 1
#define COCHE1 2
#define COCHE2 3
#define AMBULANCIA 4
#define MOTO 5
#define HORACIO 6
#define HORACIOCS 7
#define ESQUIADOR 8
#define BANDERA 9
#define MONTON 10
#define PINO 11
#define CHOQUE 12
#define BANDANCHA 13
#define BANDESTRECHA 14
#define META 15
#define METANCHA 16
#define METESTRECHA 17
#define METACHOQ 18
#define MUNECO 19
#define ARRIBA 0         // Key pressed data
#define ABAJO 1
#define IZQUIERDA 2
#define DERECHA 3
#define SELECC 4
#define ABORTK 5
#define ESPANOL 0       // Language data
#define ENGLISH 1
#define ASTURIANU 2
#define GALEGO 3
#define CATALA 4
#define ULTLENG 4
#define SINDINERO -1    // End-of-phase data
#define SINESQUIS -1
#define FINFASEOK 1
#define ABORT -127
#define ULTIMOSET 1    // Last graphic set
#define FICHEROOK 82


#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include "datos.h"
#include "typedefs.h"
#include <allegro.h>
#endif
