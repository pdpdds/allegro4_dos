#include "CCavalo.h"

CCavalo::CCavalo(char cor) : CPecas(cor){
}

char CCavalo::GetPeca(){
	return'C';
}


bool CCavalo::MovimentoQuadrantes(int origemY, int origemX, int destinoY, int destinoX, CPecas* APObjetos[8][8]){
	if ((origemX == destinoX + 1) || (origemX == destinoX - 1)) {
			if ((origemY == destinoY + 2) || (origemY == destinoY - 2)) {
				return true;
			}
	}
	if ((origemX == destinoX + 2) || (origemX == destinoX - 2)) {
		if ((origemY == destinoY + 1) || (origemY == destinoY - 1)) {
				return true;
		}
	}
		return false;
}
