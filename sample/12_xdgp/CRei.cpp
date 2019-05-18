#include "CRei.h"

CRei::CRei(char cor) : CPecas(cor){
}

char CRei::GetPeca(){
	return 'R';
}

bool CRei::MovimentoQuadrantes(int origemY, int origemX, int destinoY, int destinoX, CPecas* APObjetos[8][8]) {
		int vetorY = destinoY - origemY;
		int vetorX = destinoX - origemX;
		if (((vetorY >= -1) && (vetorY <= 1)) &&
			((vetorX >= -1) && (vetorX <= 1)))
		{
			return true;
		}
		return false;
}
