#ifndef __CXadrez__
#define __CXadrez__

class CXadrez{
private:
    CTabuleiro tabuleiro;
	char corjogador;
    int velharodada;
    int nrodada;
public:
	CXadrez();
	~CXadrez();
    bool Ainicia();
    void Aabretabuleiro();
	void Afinaliza ();
	int QuadranteX();
	int QuadranteY();
	void SelecSprite(int,int);
	void InicioXadrez();
	void GetMovimento(CPecas* [8][8]);
	void MudarRodada();
	bool FimdeJogo();
};

#endif
