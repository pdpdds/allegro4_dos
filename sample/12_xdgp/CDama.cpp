#include "CDama.h"

CDama::CDama(char cor) : CPecas(cor){
}

char CDama::GetPeca(){
	return 'D';
}

bool CDama::MovimentoQuadrantes(int origemY, int origemX, int destinoY, int destinoX, CPecas* APObjetos[8][8]) {
		if (origemY == destinoY) {
			int vetorX = (destinoX - origemX > 0) ? 1 : -1;
			for (int contadorX = origemX + vetorX; contadorX !=  destinoX; contadorX = contadorX + vetorX) {
				if (APObjetos[contadorX][origemY] != 0) {
					return false;
				}
			}
			return true;
		} else if (destinoX == origemX) {
			int vetorY = (destinoY - origemY > 0) ? 1 : -1;
			for (int contadorY = origemY + vetorY; contadorY !=  destinoY; contadorY = contadorY + vetorY) {
				if (APObjetos[origemX][contadorY] != 0) {
					return false;
				}
			}
			return true;
		} else if ((destinoX - origemX == destinoY - origemY) || (destinoX - origemX == origemY - destinoY)) {
			int vetorY = (destinoY - origemY > 0) ? 1 : -1;
			int vetorX = (destinoX - origemX > 0) ? 1 : -1;
			int contadorY;
			int contadorX;
			for (contadorY = origemY + vetorY, contadorX = origemX + vetorX;
				contadorY !=  destinoY;
				contadorY = contadorY + vetorY, contadorX = contadorX + vetorX)
			{
				if (APObjetos[contadorX][contadorY] != 0) {
					return false;
				}
			}
			return true;
		}
		return false;
}
