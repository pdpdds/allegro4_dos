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

#ifdef  _WIN32
#define MAX_X         804
#define MAX_Y         504
#else
#define MAX_X         320
#define MAX_Y         200
#endif //  _WIN32
#define V_MAX_X       0
#define V_MAX_Y       0
#define COLOR_BITS    32
#define VIDEO_CARD    GFX_AUTODETECT_WINDOWED
#define DIGI_CARD     DIGI_AUTODETECT
#define MIDI_CARD     MIDI_AUTODETECT
#define CONST_A       2+62.65

using namespace std;

CTabuleiro::CTabuleiro(){
	for (int Y = 0; Y < 8; ++Y) {
			for (int X = 0; X < 8; ++X) {
				ATObjetos[X][Y] = 0;
			}
	}

	for (int X = 0; X < 8; ++X) {
		ATObjetos[X][6] = new CPeao('P');
    }
	ATObjetos[0][7] = new CTorre('P');
	ATObjetos[1][7] = new CCavalo('P');
	ATObjetos[2][7] = new CBispo('P');
    ATObjetos[3][7] = new CDama('P');
	ATObjetos[4][7] = new CRei('P');
	ATObjetos[5][7] = new CBispo('P');
	ATObjetos[6][7] = new CCavalo('P');
	ATObjetos[7][7] = new CTorre('P');
	for (int X = 0; X < 8; ++X) {
		ATObjetos[X][1] = new CPeao('B');
	}
	ATObjetos[0][0] = new CTorre('B');
	ATObjetos[1][0] = new CCavalo('B');
	ATObjetos[2][0] = new CBispo('B');
	ATObjetos[3][0] = new CDama('B');
	ATObjetos[4][0] = new CRei('B');
	ATObjetos[5][0] = new CBispo('B');
	ATObjetos[6][0] = new CCavalo('B');
	ATObjetos[7][0] = new CTorre('B');
}

CTabuleiro::~CTabuleiro() {
	for (int Y = 0; Y < 8; ++Y) {
		for (int X = 0; X < 8; ++X) {
			delete ATObjetos[Y][X];
		}
	}
}
bool CTabuleiro::Roque(int cliqueX, int cliqueY, int clique2X, int clique2Y, char corjogador){
    int vetormovimento = (clique2X - cliqueX > 0) ? 1 : -1;
    CPecas* OBJtemporario;
    if (ATObjetos[cliqueX][cliqueY] != 0 && ATObjetos[clique2X][clique2Y] != 0){
        if(ATObjetos[cliqueX][cliqueY]->GetPeca() == 'R' && ATObjetos[cliqueX][cliqueY]->getMovido() == 0){
            if (ATObjetos[clique2X][clique2Y]->GetPeca() == 'T' &&  ATObjetos[clique2X][clique2Y]->getMovido() == 0){
                OBJtemporario = ATObjetos[clique2X][clique2Y];
                ATObjetos[clique2X][clique2Y] = 0;
                if(ATObjetos[cliqueX + vetormovimento][cliqueY] == 0 &&
                   ATObjetos[cliqueX + 2*vetormovimento][cliqueY] == 0 &&
                   ATObjetos[cliqueX + 3*vetormovimento][cliqueY] == 0){
                    for (int Y = 0; Y < 8; ++Y) {
                        for (int X = 0; X < 8; ++X) {
                            if (ATObjetos[X][Y] != 0) {
                                if (ATObjetos[X][Y]->GetCor() != corjogador) {
                                    if (!(ATObjetos[X][Y]->MovimentoPossivel(Y, X, clique2Y, cliqueX + vetormovimento, ATObjetos)) ||
                                        !(ATObjetos[X][Y]->MovimentoPossivel(Y, X, clique2Y, cliqueX + 2*vetormovimento, ATObjetos))){

                                        ATObjetos[cliqueX+2*vetormovimento][cliqueY] = ATObjetos[cliqueX][cliqueY];
                                        ATObjetos[cliqueX+vetormovimento][cliqueY] = OBJtemporario;
                                        ATObjetos[clique2X][clique2Y] = ATObjetos[cliqueX][cliqueY] = 0;
                                        return true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool CTabuleiro::Cheque(char cor) {
	int reiY;
	int reiX;
	for (int Y = 0; Y < 8; ++Y) {
		for (int X = 0; X < 8; ++X) {
			if (ATObjetos[X][Y] != 0) {
				if (ATObjetos[X][Y]->GetCor() == cor) {
					if (ATObjetos[X][Y]->GetPeca() == 'R') {
						reiY = Y;
						reiX = X;
					}
				}
			}
		}
	}
	for (int Y = 0; Y < 8; ++Y) {
		for (int X = 0; X < 8; ++X) {
			if (ATObjetos[X][Y] != 0) {
				if (ATObjetos[X][Y]->GetCor() != cor) {
					if (ATObjetos[X][Y]->MovimentoPossivel(Y, X, reiY, reiX, ATObjetos)) {
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool CTabuleiro::PodeMover(char cor) {
	for (int Y = 0; Y < 8; ++Y) {
		for (int X = 0; X < 8; ++X) {
			if (ATObjetos[X][Y] != 0) {
				if (ATObjetos[X][Y]->GetCor() == cor) {
					for (int movimentoY = 0; movimentoY < 8; ++movimentoY) {
						for (int movimentoX = 0; movimentoX < 8; ++movimentoX) {
							if (ATObjetos[X][Y]->MovimentoPossivel(Y, X, movimentoY, movimentoX, ATObjetos)) {
								CPecas* qpTemp	= ATObjetos[movimentoX][movimentoY];
								ATObjetos[movimentoX][movimentoY]	= ATObjetos[X][Y];
								ATObjetos[X][Y]			= 0;
								bool bpodemover = !Cheque(cor);
								ATObjetos[X][Y] = ATObjetos[movimentoX][movimentoY];
								ATObjetos[movimentoX][movimentoY] = qpTemp;
								if (bpodemover) {
									return true;
								}
							}
						}
					}
				}
			}
		}
	}
	return false;
}

void CTabuleiro::Print(){
    BITMAP* buffer;
    //buffer = create_bitmap(504, 504);
    buffer = create_bitmap(804, 504);
    clear_bitmap(buffer);
    BITMAP *tabuleiro1;
    tabuleiro1 = load_bitmap("Sprites/board5.bmp", NULL);
    blit(tabuleiro1, buffer, 0, 0, 0, 0, MAX_X, MAX_Y);
    for(int i=0;i<8;i++){
    for(int j=0;j<8;j++){
        if (ATObjetos[i][j] != 0){
            if(ATObjetos[i][j]->GetPeca()=='P'){
                if(ATObjetos[i][j]->GetCor()=='P')
                    draw_sprite(buffer,load_bitmap("Sprites/bp.bmp",NULL),CONST_A*(i),CONST_A*(7-j));
                else
                    draw_sprite(buffer,load_bitmap("Sprites/wp.bmp",NULL),CONST_A*(i),CONST_A*(7-j));
                    }
            if(ATObjetos[i][j]->GetPeca()=='B'){
                if(ATObjetos[i][j]->GetCor()=='P')
                    draw_sprite(buffer,load_bitmap("Sprites/bb.bmp",NULL),CONST_A*(i),CONST_A*(7-j));
                else
                    draw_sprite(buffer,load_bitmap("Sprites/wb.bmp",NULL),CONST_A*(i),CONST_A*(7-j));
                    }
            if(ATObjetos[i][j]->GetPeca()=='C'){
                if(ATObjetos[i][j]->GetCor()=='P')
                    draw_sprite(buffer,load_bitmap("Sprites/bn.bmp",NULL),CONST_A*(i),CONST_A*(7-j));
                else
                    draw_sprite(buffer,load_bitmap("Sprites/wn.bmp",NULL),CONST_A*(i),CONST_A*(7-j));
                    }
            if(ATObjetos[i][j]->GetPeca()=='D'){
                if(ATObjetos[i][j]->GetCor()=='P')
                    draw_sprite(buffer,load_bitmap("Sprites/bq.bmp",NULL),CONST_A*(i),CONST_A*(7-j));
                else
                    draw_sprite(buffer,load_bitmap("Sprites/wq.bmp",NULL),CONST_A*(i),CONST_A*(7-j));
                    }
            if(ATObjetos[i][j]->GetPeca()=='R'){
                if(ATObjetos[i][j]->GetCor()=='P')
                    draw_sprite(buffer,load_bitmap("Sprites/bk.bmp",NULL),CONST_A*(i),CONST_A*(7-j));
                else
                    draw_sprite(buffer,load_bitmap("Sprites/wk.bmp",NULL),CONST_A*(i),CONST_A*(7-j));
            }
            if(ATObjetos[i][j]->GetPeca()=='T'){
                if(ATObjetos[i][j]->GetCor()=='P')
                    draw_sprite(buffer,load_bitmap("Sprites/br.bmp",NULL),CONST_A*(i),CONST_A*(7-j));
                else
                    draw_sprite(buffer,load_bitmap("Sprites/wr.bmp",NULL),CONST_A*(i),CONST_A*(7-j));
            }
        }
    }
    }
    blit(buffer, screen, 0, 0, 0, 0, MAX_X, MAX_Y);
}

CPecas* CTabuleiro::Promocao(int clique2X, int clique2Y){
    CPecas* OBJpromocao;
    char pecapromocao, corpromocao = ATObjetos[clique2X][clique2Y]->GetCor();
    bool bsaida = false;
    while (!bsaida){
        FONT *font1 = load_font("font1.pcx", NULL, NULL);
        FONT *font3 = load_font("font3.pcx", NULL, NULL);
        textprintf_ex(screen, font1, 570, 140, makecol(255, 0, 0),-1, "PROMOCAO!!");
        textprintf_ex(screen, font3, 550, 310, makecol(255, 255, 255),-1, "Observe o prompt!");
        cout<<"Voce conseguiu promover um Peao!"<<endl<<"Digite a primeira letra da nova peca(Dama, Bispo, Cavalo, Torre, Peao): ";
        cin>>pecapromocao;
        try{
        if(pecapromocao != 'D' && pecapromocao != 'd' && pecapromocao != 'B' && pecapromocao != 'b' && pecapromocao != 'C' && pecapromocao != 'c' &&  pecapromocao != 'T' && pecapromocao != 't' && pecapromocao != 'P' && pecapromocao != 't'){
            throw(pecapromocao);
        }

        if(pecapromocao=='D' || pecapromocao=='d'){
            OBJpromocao = new CDama(corpromocao);
            bsaida = true;
            return OBJpromocao;
        }
        if(pecapromocao=='B'|| pecapromocao=='b'){
            OBJpromocao = new CBispo(corpromocao);
            bsaida = true;
            return OBJpromocao;
        }
        if(pecapromocao=='C'|| pecapromocao=='c'){
            OBJpromocao = new CCavalo(corpromocao);
            bsaida = true;
            return OBJpromocao;
        }
        if(pecapromocao=='T'|| pecapromocao=='t'){
            OBJpromocao = new CTorre(corpromocao);
            bsaida = true;
            return OBJpromocao;
        }
        if(pecapromocao=='P'|| pecapromocao=='p'){
            OBJpromocao = new CPeao(corpromocao);
            bsaida = true;
            return OBJpromocao;
        }
        }
        catch(int pecapromocao){
            cout<<"Entrada invalida"<<endl;
        }
        catch(float pecapromocao){
            cout<<"Entrada invalida"<<endl;
        }
        catch(char pecapromocao){
            cout<<"Entrada invalida"<<endl;
        }
        catch(string pecapromocao){
            cout<<"Entrada invalida"<<endl;
        }
    }
}
