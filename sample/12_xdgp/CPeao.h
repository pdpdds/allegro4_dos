#ifndef __CPeao_H__
#define __CPeao_H__

#include "CPecas.h"

class CPeao : public CPecas{
	private:
		virtual char GetPeca();
		bool MovimentoQuadrantes(int , int, int, int, CPecas* [8][8]);
	public:
		CPeao(char);
		~CPeao();
};
#endif
