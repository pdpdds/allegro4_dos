#include "sprite.h"

extern Hardware *H;
extern SprManager *M;

Sprite::Sprite()
{
 Fotograma = 0;
 Frames = 0;
 KeyFrame = 20;
 FotoMax = 0;
}

Sprite::~Sprite()
{
}

void Sprite::PasaFrame () {
     if (FotoMax == 0) return;     
     if (++Frames > KeyFrame) Frames = 1;
     if (Frames == KeyFrame)
        if (++Fotograma>FotoMax) Fotograma=0;
     Dibujo = M->Sprite(Tipo,Sentido,Fotograma);
     Mascara = M->Mask(Tipo, Sentido, Fotograma);
}

void Sprite::Actualiza () {
     Fotograma = 0;
     Dibujo = M->Sprite(Tipo,Sentido,Fotograma);
     Mascara = M->Mask(Tipo, Sentido, Fotograma);
     FotoMax = Mascara->Fotog - 1;
}

void Sprite::Borra () {
     H->Borra (x,y,Mascara->LongX,Mascara->LongY);
}

void Sprite::Dibuja (bool actualiza) {
     if (actualiza) PasaFrame ();
     H->Dibuja(x,y,Dibujo);
}

const int Sprite::VerX (bool Final) {
    if (Final) return (x+(Mascara->LongX));
    return x;}

const int Sprite::VerY (bool Final) {
    if (Final) return (y+(Mascara->LongY));
    return y;}
    
const int Sprite::VerLongX () {
      return Mascara->LongX;}

const int Sprite::VerLongY () {
      return Mascara->LongY;}

const int Sprite::VerVelX () {
      return VelX;}

const int Sprite::VerVelY () {
      return VelY;}
      
const int Sprite::VerTipo () {
      return Tipo;}

void Sprite::SetVelX (int Vel) {VelX = Vel;}

void Sprite::SetVelY (int Vel) {VelY = Vel;}

void Sprite::SetY (int Y) {
     y = Y;}

void Sprite::SetX (int X) {
     x = X;}
     
void Sprite::SetTipo (int Tipo_) {
     Tipo = Tipo_;}

void Sprite::SetKF (int KF) {
     KeyFrame = KF;}


mask * Sprite::VerMascara () {
     return Mascara;}

bool Sprite::Colision (Sprite *Otro, bool CheckPP) {
    
       if (!Otro) return false;
       if (Otro == this) return false;
    
       int cx, cy, clx, cly;
       
       cx = Otro->VerX(false);
       clx = Otro->VerX(true);
       cy = Otro->VerY(false);
       cly = Otro->VerY(true);

        if ((VerX(true) > cx) && (VerX(false) < clx) &&
            (VerY(true) > cy) && (VerY(false) < cly)){  // Existe colision de bouncing box ...
            if (!CheckPP) return true; // Si tienes que mirar la Pixel Perfect...
            if (ColisionPP (Otro)) return true; // entonces compruebalo...
}
                 
        return false;
     }

bool Sprite::ColisionPP (Sprite *Otro) {

    int n,m;
    int x1, x2, y1, y2;
    int dx1, dx2, w1, w2, iw2, iw1, dy1, dy2;
    mask *Mask1, *Mask2;
    word Comp1, Comp2;
    Mask1 = VerMascara();
    Mask2 = Otro->VerMascara();
    w2 = 0; w1 = 0;
    x1 = VerX(false);
    x2 = Otro->VerX(false);
    y1 = VerY(false);
    y2 = Otro->VerY(false);
   
      if (x1>x2) {
           dx1 = 0;
           dx2 = (x1 - x2);
           while (dx2>=Tamano) {
                 dx2-=Tamano;
                 w2++;}
           }
        else {
           dx1 = (x2 - x1);
           dx2 = 0;
           while (dx1>=Tamano) {
                 dx1-=Tamano;
                 w1++;}
           }

       if (y1>y2) {
           dy1 = 0;         
           dy2 = y1 - y2;}
         else {
           dy1 = y2 - y1;
           dy2 = 0;}
        
       while ((dy1<Mask1->LongY) && (dy2<Mask2->LongY)) { // Bucle vertical

          iw1 = w1; iw2 = w2;      
          while   ((iw1<Mask1->Words) && (iw2<Mask2->Words)) { // Bucle horizontal
           
             Comp1 = Mask1->MaskData [dy1][iw1] >> dx1; // El comp del 1 es el 1 desplazado a la derecha
             Comp2 = Mask2->MaskData [dy2][iw2] >> dx2; // Id con el 2
          
             if ((dx1!=0) && (iw1<Mask1->Words-1)) // Si existe...
                Comp1 += Mask1->MaskData [dy1][iw1+1] << (Tamano-dx1); // A?dir la parte de la izqda

             if ((dx2!=0) && (iw2<Mask2->Words-1))
                Comp2 += Mask2->MaskData [dy2][iw2+1] << (Tamano-dx2);

             if (Comp1 & Comp2) return true;

             iw1++;
             iw2++;}
          dy1++;
          dy2++;}
   
     return false;
}

