#include "pista.h"

extern Hardware *H;
extern int Puntos, Nivel;

Pista::Pista()
{
                 
     NumObs [0] = 43;
     NumObs [1] = 76;
     NumObs [2] = 85;
     NumObs [3] = 191;
                 
     EndPista[0] = 3000;
     EndPista[1] = 5000;
     EndPista[2] = 6000;
     EndPista[3] = 8100;
     
}


Pista::~Pista() {
}

void Pista::Init () {
     

     short int ObjData [] = {
// ********************************************** FASE 0
         10,10,PINO,
         566,10,PINO,
         80,80, PINO,
         486,80,PINO,
         10,150,PINO,
         566,150,PINO,
         240,300,BANDANCHA,
         300,400,BANDANCHA,
         150,450,PINO,
         80,450,MONTON,
         10,500,PINO,
         566,600,PINO,
         100,700,MONTON,
         100,750,BANDANCHA,
         150,850, BANDANCHA,
         10,900,PINO,
         500,925,MUNECO,
         200, 950, BANDANCHA,
         320,1000,MONTON,
         150, 1050,BANDANCHA,
         120,1100,PINO,
         180,1200,PINO,
         230, 1300, MONTON,
         400,1400,PINO,
         250,1400,PINO,
         320,1600,PINO,
         10,1600,PINO,
         150,1600,BANDESTRECHA,
         280, 1600, PINO,
         200, 1700, BANDANCHA,
         400, 1750, PINO,
         250, 1800, BANDANCHA,
         50, 1850, PINO,
         300, 1900, BANDANCHA,
         200,1900,PINO,
         375, 2000, MONTON,
         350, 2100, BANDANCHA,
         500, 2100, PINO,
         300, 2200, BANDANCHA,
         250, 2300, BANDANCHA,
         200, 2400, BANDANCHA,
         320, 2500, MONTON,
         280,2700,METANCHA,
// ********************************************** FASE 1                     
         10,10,PINO,
         566,10,PINO,
         80,80, PINO,
         486,80,MUNECO,
         300,80, MONTON,
         10,150,PINO,
         566,150,PINO,
         300,150,BANDANCHA,
         250,175,PINO,
         460,175,PINO,
         250,225,PINO,
         460,225,PINO,
         100,250,PINO,
         320,250,BANDANCHA,
         500,250,PINO,
         200,275,PINO,
         350,325,MONTON,
         280,350,BANDESTRECHA,
         50,320,PINO,
         570,320,PINO,
         230,450,BANDESTRECHA,
         100,500,PINO,
         500,500,PINO,
         0,550,PINO,
         350,550,MONTON,
         500,600,PINO,
         10,650,MONTON,         
         550,800,PINO,
         330,1000,PINO,
         320,1100,BANDANCHA,
         30,1100,PINO,
         460,1150,PINO,
         340,1200,BANDANCHA,
         370,1275,BANDESTRECHA,         
         150,1300,PINO,
         320,1350,BANDANCHA,         
         280,1425,BANDANCHA,
         100,1450,PINO,
         150,1470,PINO,         
         250,1500,BANDESTRECHA,
         260,1600,MONTON,
         250,1675,BANDESTRECHA,
         200,1800,PINO,
         400,1800,PINO,
         300,1900,MONTON,
         300,2000,BANDANCHA,
         150,2175,PINO,
         460,2175,PINO,
         50,2225,MUNECO,
         500,2225,PINO,
         0,2250,PINO,
         100,2500,MONTON,
         300,2600,BANDESTRECHA,
         350,2700,BANDANCHA,
         420,2750,MONTON,
         300,2800,BANDESTRECHA,
         500,2825,PINO,
         0,2950,PINO,
         200,3000,MONTON,
         100,3050,PINO,
         500,3070,PINO,
         50,3150,PINO,
         300,3300,BANDANCHA,
         350,3400,BANDANCHA,
         300,3500,BANDANCHA,
         250,3600,BANDANCHA,
         225,3725,BANDESTRECHA,
         175,3800,BANDANCHA,
         225,3900,BANDANCHA,
         275,4000,BANDANCHA,
         325,4100,BANDANCHA,
         375,4200,BANDANCHA,
         325,4300,BANDANCHA,
         350,4400,BANDESTRECHA,
         375,4500,BANDANCHA,    
         200, 4700, METANCHA,
// ********************************************** FASE 2
         50, 40, PINO,
         550,40, PINO,
         130,100, PINO,
         470,100,PINO,
         210,160,PINO,
         390,160,PINO,
         300,250,PINO,
         280,370,BANDANCHA,
         330,410,MONTON,
         300,450,BANDESTRECHA,
         550,450,PINO,
         330,500,MONTON,
         200,500,MUNECO,
         250,550,PINO,
         50,570,PINO,
         150,720,BANDESTRECHA,
         170,770,BANDESTRECHA,
         210,830,BANDESTRECHA,
         400,840,PINO,
         170,900,BANDESTRECHA,
         550,900,MUNECO,
         150,960,BANDESTRECHA,
         120,1000,MONTON,
         0,1050,PINO,
         150,1250,PINO,
         400,1250,BANDANCHA,
         450,1350,BANDANCHA,
         350,1450,BANDANCHA,
         350,1550,MONTON,
         50, 1550, PINO,
         60,1600,PINO,
         70,1650,PINO,
         230,1650,PINO,
         200,1700,BANDANCHA,
         80,1750,PINO,
         250,1800,MONTON,
         400,1800,MUNECO,
         50,1900,PINO,
         100,1950,PINO,
         550,1950,PINO,
         150,2000,PINO,
         500,2000,PINO,
         300,2200,BANDESTRECHA,
         330,2300,BANDESTRECHA,
         350,2400,BANDESTRECHA,
         370,2500,MONTON,
         60,2600,PINO,
         70,2700,PINO,
         550,2700,PINO,
         500,2750,PINO,
         250,2800,PINO,
         300,2900,PINO,
         300,3100,BANDANCHA,
         0,3110,PINO,
         40,3200,PINO,
         350,3200,BANDANCHA,
         50,3250,PINO,
         400,3300,BANDANCHA,
         60,3300,PINO,
         100,3350,PINO,
         200,3400,MUNECO,
         420,3400,BANDESTRECHA,
         450,3500,BANDANCHA,
         300,3600,PINO,
         280,3700,PINO,
         75,3850,PINO,
         530,3900,PINO,
         500,4000,BANDANCHA,
         450,4100,BANDANCHA,
         400,4190, BANDANCHA,
         350,4280,BANDANCHA,
         370,4300,PINO,
         150,4400,MUNECO,
         310,4420,BANDANCHA,
         290,4510,BANDESTRECHA,
         240,4600,BANDANCHA,
         200,4800,BANDANCHA,
         300,4900,BANDANCHA,
         350,5000,BANDANCHA,
         400,5100,BANDANCHA,
         430,5200,BANDESTRECHA,
         470,5300,BANDANCHA,
         500,5400,BANDANCHA,
         450,5500,BANDANCHA,
         280,5700,METESTRECHA,
// ********************************************** FASE 3
         0  ,0  ,PINO,
         70 ,50 ,PINO,
         140,100,PINO,
         210,150,PINO,
         280,200,PINO,
         100,220,MUNECO,
         350,250,PINO,
         420,250,PINO,
         490,250,PINO,
         560,350,PINO,
         490,400,PINO,
         420,400,PINO,
         350,450,PINO,
         420,450,MUNECO,
         50 ,460,PINO,
         120,480,PINO,
         300,500,MONTON,
         300,530,PINO,
         40 ,550,PINO,
         500,560,PINO,
         400,590,PINO,
         450,600,PINO,
         280,650,BANDANCHA,
         0  ,650,PINO,
         550,650,PINO,
         70 ,700,PINO,
         490,700,PINO,
         300,700,MONTON,
         140,750,PINO,
         420,750,PINO,
         300,750,BANDESTRECHA,
         210,800,PINO,
         350,800,PINO,
         140,850,PINO,
         420,850,PINO,
         290,850,BANDESTRECHA,
         0,900,MUNECO,
         70,900,PINO,
         490,900,PINO,
         340,950,BANDESTRECHA,
         355,1000,MONTON,
         160,1060,PINO,
         140,1150,BANDANCHA,
         300,1150,PINO,
         300,1200,PINO,
         160,1240,BANDESTRECHA,
         310,1250,PINO,
         190,1320,BANDESTRECHA,
         400,1360,MUNECO,
         210,1400,BANDESTRECHA,
         190,1450,BANDESTRECHA,
         200,1550,MONTON,
         170,1570,BANDANCHA,
         280,1580,PINO,
         150,1650,BANDESTRECHA,
         160,1750,MONTON,
         10 ,1770,MUNECO,
         450,1800,PINO,
         350,1850,BANDANCHA,
         50,1860,PINO,
         500,1870,PINO,
         100,1880,PINO,
         100,1900,PINO,
         150,1950,PINO,
         370,1950,BANDANCHA,
         345,2000,PINO,
         460,2000,PINO,
         390,2080,BANDESTRECHA,
         370,2150,BANDANCHA,
         390,2260,BANDESTRECHA,
         400,2350,PINO,
         400,2400,PINO,
         400,2450,PINO,
         400,2500,PINO,
         400,2550,PINO,
         50 ,2600,MUNECO,
         400,2600,PINO,
         450,2600,BANDESTRECHA,
         400,2650,PINO,
         450,2650,BANDESTRECHA,
         100,2700,MONTON,
         400,2700,PINO,
         450,2700,BANDESTRECHA,
         200,2750,PINO,
         400,2750,PINO,
         450,2750,BANDESTRECHA,
         290,2800,MONTON,
         350,2800,PINO,
         450,2800,BANDESTRECHA,
         300,2850,PINO,
         450,2850,BANDESTRECHA,
         250,2900,PINO,
         50 ,2950,MONTON,
         200,3000,PINO,
         150,3100,PINO,
         350,3100,BANDANCHA,
         100,3200,PINO,
         300,3200,BANDANCHA,
         150,3200,PINO,
         50 ,3300,MONTON,
         200,3400,PINO,
         170,3500,BANDANCHA,
         400,3500,MUNECO,
         450,3600,PINO,
         200,3600,BANDANCHA,
         500,3620,PINO,
         10 ,3660,PINO,
         150,3700,PINO,         
         400,3700,BANDANCHA,
         50,3780,PINO,
         450,3800,BANDANCHA,
         470,3900,MONTON,
         430,3950,PINO,
         500,3950,BANDANCHA,
         530,4050,PINO,
         80, 4100,PINO,
         120,4130,MUNECO,
         530,4150,BANDESTRECHA,
         540,4200,MONTON,
         100,4220,PINO,
         500,4250,BANDESTRECHA,
         50, 4340, PINO,
         550,4340, PINO,
         130,4400, PINO,
         470,4400,PINO,
         210,4460,PINO,
         390,4460,PINO,
         300,4600,PINO,
         150,4700,BANDANCHA,
         300,4700,PINO,
         400,4700,BANDANCHA,
         300,4800,PINO,
         170,4900,BANDESTRECHA,
         300,4900,PINO,
         450,4900,BANDESTRECHA,
         50,5000,PINO,
         300,5000,PINO,
         550,5000,PINO,
         100,5050,BANDANCHA,
         300,5000,PINO,
         500,5100,MUNECO,
         300,5100,PINO,
         150,5140,BANDESTRECHA,
         450,5150,MONTON,
         160,5200,MONTON,
         300,5200,PINO,
         340,5200,MUNECO,
         210,5250,BANDESTRECHA,
         170,5300,BANDESTRECHA,
         300,5300,PINO,
         400,5350,PINO,
         120,5350,BANDESTRECHA,
         450,5350,PINO,
         180,5400,BANDESTRECHA,
         300,5400,PINO,
         450,5400,PINO,
         200,5450,BANDESTRECHA,
         350,5450,MONTON,
         300,5500,PINO,
         150,5510,BANDESTRECHA,
         170,5550,MONTON,
         300,5600,PINO,
         450,5600,MONTON,
         150,5650,BANDESTRECHA,
         500,5650,PINO,
         210,5700,BANDESTRECHA,
         300,5700,PINO,
         550,5700,PINO,
         170,5750,BANDESTRECHA,
         120,5800,BANDESTRECHA,
         300,5800,PINO,
         300,5900,PINO,
         300,6000,PINO,
         320,6100,MONTON,
         280,6130,BANDANCHA,
         300,6200,BANDESTRECHA,
         
         350,6280,BANDANCHA,
         370,6300,PINO,
         150,6400,MUNECO,
         310,6420,BANDANCHA,
         290,6510,BANDESTRECHA,
         240,6600,BANDANCHA,
         200,6800,BANDANCHA,
         300,6900,BANDANCHA,
         350,7000,BANDANCHA,
         400,7100,BANDANCHA,
         430,7200,BANDESTRECHA,
         400,7300,BANDESTRECHA,
         370,7400,BANDESTRECHA,
         350,7500,BANDANCHA,
         
         50,7800,METESTRECHA

         };

     NivelTemp = Nivel;
     while (NivelTemp > NivelMax)
           NivelTemp -= NivelMax;
     
     Punt = 0;                     
     for (n=0; n<NivelTemp; n++)                     
         Punt += NumObs[n]*3;

     FinPista = EndPista[NivelTemp];

     Inicial = new Obstaculo (ObjData[Punt],ObjData[Punt+1],ObjData[Punt+2]);
     Temp2 = Inicial;
     for (n=3;n<NumObs[NivelTemp]*3;n+=3) {
         Temp1 = new Obstaculo (ObjData[Punt+n],ObjData[Punt+n+1],ObjData[Punt+n+2]);
         Temp2->GrabarSiguiente (Temp1);
         Temp2 = Temp1;}

     YInd = 0;
     Margen = 200;
     Ind = Inicial; 
}

void Pista::DeInit () {
Temp1 = Inicial;
do {
    Temp2 = Temp1->VerSiguiente();
    delete Temp1;
    if (Temp2) Temp1 = Temp2;} while (Temp2);
}

bool Pista::Avanza (int Speed, int &Indice, int &Margin) {

     Indice = YInd;

     if (Margen) {             // Mientras haya margen, solo restalo, pero no avances ni hagas nada...
        Margen -= Speed;
        if (Margen < 0) {
           Speed = -Margen;
           Margen = 0;}
        Margin = Margen;
        return false;}        // ... y vu?vete ya 
        
     if (YInd >= FinPista - (ALTOPANTALLA - H->TextHeight())) return true; // Has llegado al final de la pista
     YInd += Speed;
     
     Temp1 = Ind;
     
     if (Ind->VerSiguiente() != NULL)                // Si no es el ?timo objeto...
        while (Ind->VerY(true) < YInd) {             // Mientras est?m? arriba del ?dice
              Temp1 = Ind;
              Ind = Temp1->VerSiguiente ();}         // Considera el siguiente al actual como nuevo ?dice

     return false;
}

void Pista::Dibuja () {
     Temp1 = Ind;
     while (Temp1) {
           Temp1->rDibuja(YInd, Margen);
           Temp1 = Temp1->VerSiguiente();}

}

void Pista::Borra () {
     Temp1 = Ind;
     while (Temp1) {
           Temp1->rBorra(YInd, Margen);
           Temp1 = Temp1->VerSiguiente();}
}
     
void Pista::SetMargin (int Margin) {
     Margen = Margin;}

Obstaculo* Pista::Indice () {
     return Ind;}
