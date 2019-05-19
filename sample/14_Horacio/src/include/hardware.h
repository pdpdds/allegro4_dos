#ifndef HARDWARE_H
#define HARDWARE_H

#include "constantes.h"
#include "configuracion.h"
#include <allegro.h>

class Hardware
{
	public:
		Hardware();
		~Hardware();
        int InitGraph ();
		int Init ();
		void DeInit ();
		const int VerTecla (int Sentido);
		void GrabarTecla (int Sentido, int Tecla);
        int Tecla ();
        int VerTecla ();
        void EsperaTecla ();
        void Rest(int Ms);
        void TextOut (const char *s, int x,int y, int color, int fondo=-1);
        void TextOutCentre (const char *s,int y, int color, int fondo=-1);
        void TextOutCentre_ex (const char *s,int x, int y, int color, int fondo=-1);
        void SetFont (char *s1, char *s2);
        void SetFont (int Numero);
        void DelFont ();
        void cls (int color);
        void DrawFondoBuffer (int x, int y, int LongX, int LongY, int xo, int yo);
        void VuelcaBuffer ();
        void RectFill (int x1, int y1, int x2, int y2, int color);
        void RectFill_ex (int x1, int y1, int x2, int y2, int color);
        int TextHeight ();
        int TextLength (char *String);
        void ClearKeyBuf ();
        void SetFondo (int NumFondo);
        void CopyFondo ();
        void ClrFondo (int Color);
        void Borra (int x, int y, int LongX, int LongY);
        void Dibuja (int x, int y, void *Spr);
        bool Esc ();
        void Interzona (int sentido);
        void Fundido (int Sentido);
        void SolidMode ();
        void TransMode (int n);

     private:
        BITMAP *Buffer, *Fondo;
        FONT *Original;
        DATAFILE *Archivo;
        int SpeccyColor [8];
        int ModoGraf;
        int Izquierda, Derecha, Arriba, Abajo, Selecc, Abort;
};

#endif

