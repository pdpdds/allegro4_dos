#include <fstream>
#include "hiscore.h"

HiScore::HiScore()
{                 
 fstream fich;

 for (n = 0; n < 10; n++) 
     GrabaPuntos(100 * (10 - n), n);
     GrabaNombre ("Luis",0);
     GrabaNombre ("Maria",1);
     GrabaNombre ("Nicolas",2);
     GrabaNombre ("Diego",3);
     GrabaNombre ("Valle",4);
     GrabaNombre ("Manuel",5);
     GrabaNombre ("Panik",6);
     GrabaNombre ("Beam",7);
     GrabaNombre ("William",8);
     GrabaNombre ("Tang",9);
             
 Valid = 0;

 fich.open ("scores.his",ios::in | ios::binary);
 fich.seekp(0, ios::beg);
 if (fich.good()) {                  
    fich.read (reinterpret_cast <char *> (&Valid), sizeof (Valid));    
    if (Valid == FICHEROOK) {
       for (n = 0; n < 10; n++) {
           fich.read (reinterpret_cast <char *> (&Nombre[n]), sizeof (Nombre[n]));
           fich.read (reinterpret_cast <char *> (&Score[n]), sizeof(Score[n]));
           }                                        // Del bucle for
       } // Del if Valid == 82
     }                                           // Del if (fich.good())
 fich.close();
 
 if (Valid!=FICHEROOK) Save();  // Si no has podido cargarlo graba lo que tienes para crearlo
}

HiScore::~HiScore()
{
}

int HiScore::LeePuntos (int Number) {
    return Score[Number];}
    
const char *HiScore::LeeNombre (int Number){
     return Nombre[Number];}

void HiScore::GrabaPuntos (int score, int Number){
    Score[Number] = score;}
    
void HiScore::GrabaNombre (char *name, int Number) {
    strcpy (Nombre[Number], name);}

int HiScore::Save () {
    int Ret;
    fstream fich;
    
    fich.open ("scores.his",ios::out | ios::trunc | ios::binary);
    fich.seekp(0, ios::beg);
 
    Ret = fich.good();
    Valid = FICHEROOK;
 
    if (Ret) {
       fich.write (reinterpret_cast <char *> (&Valid), sizeof (Valid));
       for (n = 0; n < 10; n++) {
           fich.write (reinterpret_cast <char *> (&Nombre[n]), sizeof (Nombre[n]));
           fich.write (reinterpret_cast <char *> (&Score[n]), sizeof(Score[n]));
       }
    }

    fich.close();
    return !Ret;
}
