#ifndef __CTabuleiro__
#define __CTabuleiro__

#include "CPecas.h"

class CTabuleiro{
public:
	CTabuleiro();
	~CTabuleiro();
	bool Cheque(char);
	bool PodeMover(char);
	void Print();
	bool Roque(int, int, int, int, char);
    CPecas* Promocao(int, int);
	CPecas* ATObjetos[8][8];
};

#endif
