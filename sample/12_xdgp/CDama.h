#ifndef __CDama_H__
#define __CDama_H__

#include "CPecas.h"

class CDama : public CPecas{
public:
	CDama(char);
	~CDama();
private:
	virtual char GetPeca();
	bool MovimentoQuadrantes(int,int,int,int,CPecas* [8][8]);
};

#endif
