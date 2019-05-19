#ifndef SPRITE_H
#define SPRITE_H

#include "hardware.h"
#include "sprmanager.h"
#include "constantes.h"
#include "typedefs.h"

extern const int Tamano;

class Sprite
{
	public:
		Sprite();
		~Sprite();
		void Actualiza ();
		void Borra ();
		void Dibuja (bool actualiza = true);
		const int VerX (bool Final = false);
		const int VerY (bool Final = false);
		const int VerLongX ();
		const int VerLongY ();
		const int VerVelX ();
		const int VerVelY ();
		const int VerTipo ();
		void SetVelY (int Vel);
		void SetVelX (int Vel);
		void SetY (int Y);
		void SetX (int X);
		void SetTipo (int Tipo_);
		void SetKF (int KF);
		mask * VerMascara ();		
        bool Colision (Sprite *Otro, bool CheckPP = true);
        bool ColisionPP (Sprite *Otro);
	protected:
        int x,y,VelY,VelX,Fotograma,FotoMax, Frames, KeyFrame;
        int Tipo, Sentido;
        void *Dibujo;
        mask *Mascara;
    private:
		void PasaFrame ();

};

#endif

