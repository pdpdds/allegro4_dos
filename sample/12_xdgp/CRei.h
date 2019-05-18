#ifndef __CRei_H__
#define __CRei_H__

#include "CPecas.h"

class CRei : public CPecas{
public:
	CRei(char);
	~CRei();
private:
	virtual char GetPeca();
	bool MovimentoQuadrantes(int ,int,int, int,CPecas* [8][8]);
};


#endif
