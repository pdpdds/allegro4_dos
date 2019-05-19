#include "ambulancia.h"

Ambulancia::Ambulancia(int Speed)
{
 Tipo = AMBULANCIA;
 x = -150, y = 448; 
 VelX = Speed; VelY = 0;
 Sentido = 1;
 Actualiza();
}

Ambulancia::~Ambulancia()
{
}

int Ambulancia::Avanza () {
    x += VelX;
    if (x > ANCHOPANTALLA) return 1;
    return 0;
}
