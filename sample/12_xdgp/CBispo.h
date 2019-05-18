#ifndef __CBispo_H__
#define __CBispo_H__

#include "CPecas.h"

class CBispo : public CPecas{
private:
	virtual char GetPeca();
	bool MovimentoQuadrantes(int,int, int, int, CPecas*[8][8]);
public:
	CBispo(char);
	~CBispo();
};
#endif
