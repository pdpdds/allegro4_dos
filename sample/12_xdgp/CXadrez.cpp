#include <iostream>
#include <cstdlib>
#include <allegro.h>

#include "CTabuleiro.h"
#include "CPecas.h"
#include "CPeao.h"
#include "CCavalo.h"
#include "CBispo.h"
#include "CTorre.h"
#include "CDama.h"
#include "CRei.h"
#include "CXadrez.h"

#define MAX_X         804
#define MAX_Y         504
#define V_MAX_X       0
#define V_MAX_Y       0
#define COLOR_BITS    32
#define VIDEO_CARD    GFX_AUTODETECT_WINDOWED
#define DIGI_CARD     DIGI_AUTODETECT
#define MIDI_CARD     MIDI_AUTODETECT
#define CONST_A       2+62.65

using namespace std;

CXadrez::CXadrez(){
	corjogador = 'B';
	velharodada = 0;
	nrodada = 1;
}
CXadrez::~CXadrez(){
}

bool CXadrez::Ainicia(){
    allegro_init();
    install_keyboard();
    install_mouse();
    install_timer();
    FONT *font1 = load_font("font1.pcx", NULL, NULL);
    set_window_title("Xadrez dos Garotos de Programa");

    set_color_depth(COLOR_BITS);
#ifdef _WIN32
    if (set_gfx_mode(VIDEO_CARD, MAX_X, MAX_Y, V_MAX_X, V_MAX_Y) < 0){
#else
	if (set_gfx_mode(GFX_AUTODETECT, MAX_X, MAX_Y, 0, 0) < 0) {
#endif
        cout<<"Erro ao inicializar o modo grafico";
        return false;
    }
    if (install_sound(DIGI_CARD, MIDI_CARD, NULL) < 0){
        cout<<"Erro ao inicializar o som";
        return false;
    }
    return true;
}

void CXadrez::Afinaliza(){
	remove_timer();
    remove_mouse();
    remove_sound();
    remove_keyboard();
    allegro_exit();
}

void CXadrez::Aabretabuleiro(){
    show_mouse(screen);

    BITMAP* buffer, *tela_inicial, *regra1, *regra2;
    buffer = create_bitmap(MAX_X, MAX_Y);
    clear_bitmap(buffer);

    tela_inicial = load_bitmap("Sprites/start.bmp", NULL);
    regra1 = load_bitmap("Sprites/regras1.bmp", NULL);
    regra2 = load_bitmap("Sprites/regras2.bmp", NULL);
    blit(tela_inicial, buffer, 0, 0, 0, 0, MAX_X, MAX_Y);
    blit(buffer, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
    bool flag = false, tela1 = true, tela2 = false, tela3 = false;
    MIDI *musica_inicio;
    SAMPLE *start;
    musica_inicio = load_midi("Songs/inicio.mid");
    start = load_sample("Songs/start.wav");
    play_midi(musica_inicio, TRUE);
    do{
        //cout<<"x: "<<mouse_x<<"y: "<<mouse_y<<endl;
       if((mouse_b & 1) && (mouse_x<= 498) && (mouse_x>=258) && (mouse_y<=427) && (mouse_y>=370) && tela1 == true){
            stop_midi();
            flag = true;
        }

       if((mouse_b & 1) && (mouse_x<= 483) && (mouse_x>=316) && (mouse_y<=339) && (mouse_y>=305) && tela1 == true){
            tela2 = true;
            tela1 = false;
            clear_bitmap(buffer);
            blit(regra1, buffer, 0, 0, 0, 0, MAX_X, MAX_Y);
            blit(buffer, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
        }
        if((mouse_b & 1) && (mouse_x<= 740) && (mouse_x>=698) && (mouse_y<=458) && (mouse_y>=415) && tela2 == true){
            tela2 = false;
            tela1 = false;
            tela3 = true;
            clear_bitmap(buffer);
            blit(regra2, buffer, 0, 0, 0, 0, MAX_X, MAX_Y);
            blit(buffer, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
        }
        if((mouse_b & 1) && (mouse_x<= 781) && (mouse_x>=747) && (mouse_y<=489) && (mouse_y>=462) && tela3 == true){
            tela2 = false;
            tela1 = true;
            tela3 = false;
            clear_bitmap(buffer);
            blit(tela_inicial, buffer, 0, 0, 0, 0, MAX_X, MAX_Y);
            blit(buffer, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
        }

    }while(!flag);
    play_sample(start, 255, 128, 1000, FALSE);
    int tempo = clock();
    while(clock() - tempo < 1500){};
}
int CXadrez::QuadranteX(){
    extern volatile int mouse_x;
    try{
        if( mouse_x == 1 || mouse_x == 2 || mouse_x == 64 || mouse_x ==127 || mouse_x ==189 ||
            mouse_x == 190 || mouse_x == 252 || mouse_x == 314 || mouse_x == 315 || mouse_x == 377 ||
            mouse_x == 440 || mouse_x == 502 || mouse_x == 503 || mouse_x == 504)
            throw(mouse_x);

        if (mouse_x >= 3 && mouse_x <= 63)
            return 0;
        if (mouse_x >= 65 && mouse_x <= 126)
            return 1;
        if (mouse_x >= 128 && mouse_x <= 188)
            return 2;
        if (mouse_x >= 191 && mouse_x <= 251)
            return 3;
        if (mouse_x >= 253 && mouse_x <= 313)
            return 4;
        if (mouse_x >= 316 && mouse_x <= 376)
            return 5;
        if (mouse_x >= 378 && mouse_x <= 439)
            return 6;
        if (mouse_x >= 441 && mouse_x <= 501)
            return 7;
    }
    catch(int mouse_x){
        return 8;
    }
}

int CXadrez::QuadranteY(){
    extern volatile int mouse_y;
    try{
        if( mouse_y == 1 || mouse_y == 2 || mouse_y == 64 || mouse_y ==127 || mouse_y ==189 ||
            mouse_y == 190 || mouse_y == 252 || mouse_y == 314 || mouse_y == 315 || mouse_y == 377 ||
            mouse_y == 440 || mouse_y == 502 || mouse_y == 503 || mouse_y == 504)
            throw(mouse_y);

        if (mouse_y >= 3 && mouse_y <= 63)
            return 7;
        if (mouse_y >= 65 && mouse_y <= 126)
            return 6;
        if (mouse_y >= 128 && mouse_y <= 188)
            return 5;
        if (mouse_y >= 191 && mouse_y <= 251)
            return 4;
        if (mouse_y >= 253 && mouse_y <= 313)
            return 3;
        if (mouse_y >= 316 && mouse_y <= 376)
            return 2;
        if (mouse_y >= 378 && mouse_y <= 439)
            return 1;
        if (mouse_y >= 441 && mouse_y <= 501)
            return 0;
    }
    catch(int mouse_y){
        return 8;
    }
}

void CXadrez::SelecSprite(int a, int b){
        BITMAP *selecao;
        selecao = load_bitmap("Sprites/selec.bmp", NULL);
        draw_sprite(screen, selecao, CONST_A*(a),CONST_A*(7-b));
}

void CXadrez::InicioXadrez(){
	do {
		GetMovimento(tabuleiro.ATObjetos);
		MudarRodada();
	} while (!FimdeJogo());
	tabuleiro.Print();
}

void CXadrez::GetMovimento(CPecas* AXObjetos[8][8]) {
    bool boolmovimento = false;
    char* msg = " ";
    FONT *font1 = load_font("font1.pcx", NULL, NULL);
    FONT *font2 = load_font("font2.pcx", NULL, NULL);
    FONT *font3 = load_font("font3.pcx", NULL, NULL);
    int cliqueX = 0, cliqueY = 0, clique2X = 0, clique2Y = 0;
	do {
        tabuleiro.Print();
        textprintf_ex(screen, font2, 525, 250, makecol(255, 255, 255),-1, msg);
        if (velharodada == 0){
            cout <<"#######Rodada das ";
            if (corjogador == 'B'){
                msg = " ";
                textprintf_ex(screen, font1, 545, 180, makecol(255, 255, 255),-1, "Vez do Jogador1");
                textprintf_ex(screen, font1, 580, 210, makecol(255, 255, 255),-1, "(BRANCAS)");
                cout<<"Brancas####### ";
            }
            else{
                 msg = " ";
                 textprintf_ex(screen, font1, 545, 180, makecol(255, 255, 255),-1, "Vez do Jogador2");
                 textprintf_ex(screen, font1, 585, 210, makecol(255, 255, 255),-1, "(PRETAS)");
                cout<<"Pretas####### ";
            }
            cout<<"n: "<<nrodada<<endl;
            if(tabuleiro.Cheque(corjogador)){
                textprintf_ex(screen, font3, 555, 305, makecol(255, 0, 0),-1, "CHEQUE, Proteja-se!");
                cout<<"[CHEQUE] Cuidado, defenda o Rei!"<<endl;
            }
        }
        while(!(mouse_b & 1)){
            if ((mouse_b & 1) && mouse_x>=576 && mouse_x<=722 && mouse_y<452 && mouse_y>410){
                Afinaliza();
            }
        }
        if ((mouse_b & 1) && mouse_x>=576 && mouse_x<=722 && mouse_y<452 && mouse_y>410){
            Afinaliza();
        }
        while(mouse_b & 1){
        }
        cliqueX = QuadranteX();
        cliqueY = QuadranteY();
        if (!(cliqueX == 8 || cliqueY == 8)){
            CPecas* pecaAtual = AXObjetos[cliqueX][cliqueY];
            if (AXObjetos[cliqueX][cliqueY] != 0){
                if (pecaAtual->GetCor() == corjogador){
                    SelecSprite(cliqueX, cliqueY);
                    while(!(mouse_b & 1)){
                    }
                    while(mouse_b & 1){
                    }
                    clique2X = QuadranteX();
                    clique2Y = QuadranteY();
                    if(tabuleiro.Roque(cliqueX, cliqueY, clique2X, clique2Y, corjogador)){
                        boolmovimento = true;
                        if (clique2X == 7)
                            clique2X = 6;
                        if (clique2X == 0)
                            clique2X = 2;
                    }
                    if (!(clique2X == 8 || clique2Y == 8) && !boolmovimento){
                        if (pecaAtual->MovimentoPossivel(cliqueY, cliqueX, clique2Y, clique2X, AXObjetos)) {
                            CPecas* objtemporario           = AXObjetos[clique2X][clique2Y];
                            AXObjetos[clique2X][clique2Y]   = AXObjetos[cliqueX][cliqueY];
                            AXObjetos[cliqueX][cliqueY]	= 0;
                            if (!tabuleiro.Cheque(corjogador)) {
                                velharodada = 0;
                                AXObjetos[clique2X][clique2Y]->addMovido();
                                delete objtemporario;
                                boolmovimento = true;
                            } else {
                                AXObjetos[cliqueX][cliqueY]   = AXObjetos[clique2X][clique2Y];
                                AXObjetos[clique2X][clique2Y] = objtemporario;
                                msg = "[ERRO] Ficara em Cheque!";
                                cout<<"[INVALIDO] Voce vai ficar em Cheque."<<endl;
                                velharodada = 1;
                            }
                        }else{
                            msg = "[ERRO]Movimento invalido";
                            cout<<"[INVALIDO] Movimento invalido para a Peca."<<endl;
                            velharodada = 1;
                        }
                    }
                }else{
                    msg = "  [ERRO] Peca invalida";
                    cout<<"[INVALIDO] Peca de cor errada."<<endl;
                    velharodada = 1;
                }
            }else{
                if (!(cliqueX == 8 || cliqueY == 8 || clique2X == 8 || clique2Y == 8)){
                    msg = "[ERRO] Quadrante Vazio";
                    cout<<"[INVALIDO] Quadrante vazio."<<endl;
                    velharodada = 1;
                }
            }
        }
        if (cliqueX == 8 || cliqueY == 8 || clique2X == 8 || clique2Y == 8){
            msg = "  [ERRO] Clique Invalido!";
            cout<<"[INVALIDO] Clique Invalido."<<endl;
            velharodada = 1;
        }
    } while (!boolmovimento);
    nrodada++;
    if(AXObjetos[clique2X][clique2Y]->GetPeca() == 'R')
        cout<<"Rei";
    if(AXObjetos[clique2X][clique2Y]->GetPeca() == 'D')
        cout<<"Dama";
    if(AXObjetos[clique2X][clique2Y]->GetPeca() == 'T')
        cout<<"Torre";
    if(AXObjetos[clique2X][clique2Y]->GetPeca() == 'C')
        cout<<"Cavalo";
    if(AXObjetos[clique2X][clique2Y]->GetPeca() == 'B')
        cout<<"Bispo";
    if(AXObjetos[clique2X][clique2Y]->GetPeca() == 'P')
        cout<<"Peao";

    if (corjogador == 'B')
        cout<<" Branco";
    else
        cout<<" Preto";

    cout<<" foi de ("<<cliqueX + 1<<","<<cliqueY + 1<<") para ("<<clique2X + 1<<","<<clique2Y + 1<<")."<<endl<<endl;
    if (AXObjetos[clique2X][clique2Y]->GetPeca() == 'P' && (clique2Y == 7 || clique2Y == 0))
        AXObjetos[clique2X][clique2Y] = tabuleiro.Promocao(clique2X, clique2Y);
}

void CXadrez::MudarRodada() {
		corjogador = (corjogador == 'B') ? 'P' : 'B';
}

bool CXadrez::FimdeJogo() {
        FONT *font1 = load_font("font1.pcx", NULL, NULL);
        FONT *font2 = load_font("font2.pcx", NULL, NULL);
        FONT *font3 = load_font("font3.pcx", NULL, NULL);
		bool boolpodemover;
		boolpodemover = tabuleiro.PodeMover(corjogador);
		if (!boolpodemover) {
            tabuleiro.Print();
			do{
                if (tabuleiro.Cheque(corjogador)) {
                    textprintf_ex(screen, font1, 580, 160, makecol(255, 0, 0),-1, "CHEQUEMATE!!!");
                    if (corjogador == 'B')
                        textprintf_ex(screen, font3, 530, 280, makecol(255, 255, 255),-1, "Jogador 2 (Pretas) Venceu!");
                    else
                        textprintf_ex(screen, font3, 530, 260, makecol(255, 255, 255),-1, "Jogador 1 (Brancas) Venceu!");
                } else {
                    textprintf_ex(screen, font1, 580, 180, makecol(255, 255, 255),-1, "EMPATE!!!");
                }
            }while(!((mouse_b & 1) && mouse_x>=576 && mouse_x<=722 && mouse_y<452 && mouse_y>410));
		}
		return !boolpodemover;
}
