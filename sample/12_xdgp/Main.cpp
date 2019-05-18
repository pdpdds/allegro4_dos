#include <iostream>
#include <cstdlib>
#include <allegro.h>

#include "CPecas.h"
#include "CPeao.h"
#include "CCavalo.h"
#include "CBispo.h"
#include "CTorre.h"
#include "CDama.h"
#include "CRei.h"
#include "CTabuleiro.h"
#include "CXadrez.h"

using namespace std;

int main (void){
    CXadrez jogo;
    jogo.Ainicia();
    jogo.Aabretabuleiro();

    MIDI *musica_inicio;
    musica_inicio = load_midi("Songs/game.mid");
    play_midi(musica_inicio, TRUE);
    jogo.InicioXadrez();
    /*while(!key[KEY_ESC]){
    }*/
    jogo.Afinaliza();
    exit(0);
}END_OF_MAIN();
