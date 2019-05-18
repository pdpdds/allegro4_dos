#ifndef __CTorre_H__
#define __CTorre_H__

#include "CPecas.h"

class CTorre : public CPecas{
private:
	virtual char GetPeca();
	bool MovimentoQuadrantes(int,int,int,int, CPecas* [8][8]);
public:
	CTorre(char);
	~CTorre() {}
};

#endif
