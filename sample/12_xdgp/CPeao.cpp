#include "CPeao.h"

CPeao::CPeao(char cor): CPecas(cor){
}

char CPeao::GetPeca(){
	return 'P';
}

bool CPeao::MovimentoQuadrantes(int origemY, int origemX, int destinoY, int destinoX, CPecas* APObjetos[8][8]) {
		CPecas* destinopeca = APObjetos[destinoX][destinoY];
		if (destinopeca == 0) {
			if (origemX == destinoX) {
				if (GetCor() == 'B') {
					if ((destinoY == origemY + 1) ||
                        (getMovido() == 0 && destinoY == origemY + 2 && APObjetos[origemX][origemY+1] == 0 )) {
						return true;
					}
				} else {
					if ((destinoY == origemY - 1) ||
                        (getMovido() == 0 && destinoY == origemY - 2 && APObjetos[origemX][origemY-1] == 0)) {
						return true;
					}
				}
			}
		} else {
			if ((origemX == destinoX + 1) || (origemX == destinoX - 1)) {
				if (GetCor() == 'B') {
					if (destinoY == origemY + 1) {
						return true;
					}
				} else {
					if (destinoY == origemY - 1) {
						return true;
					}
				}
			}
		}
		return false;
}
