#ifndef __CCavalo_H__
#define __CCavalo_H__

#include "CPecas.h"

class CCavalo : public CPecas{
private:
	virtual char GetPeca();
	bool MovimentoQuadrantes(int,int,int,int, CPecas* [8][8]);
public:
	CCavalo(char);
	~CCavalo();
};
#endif
