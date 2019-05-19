#include "hardware.h"
#include "constantes.h"
#include "sprmanager.h"
#include "partida.h"
#include "mensajes.h"
#include "menus.h"
#include "hiscore.h"
#include "sonido.h"

Hardware *H;
SprManager *M;
Partida *Game;
Mensajes *SMS;
Configuracion *Config;
Menus *Menu;
HiScore *Record;
Sonido *SFX;

int Dinero, Puntos, Vidas, ProximaVida, Nivel, ResMenu, Retorno;
const int Tamano = sizeof (char) * sizeof (word);
