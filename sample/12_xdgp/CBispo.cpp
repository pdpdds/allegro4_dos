#include "CBispo.h"

CBispo::CBispo(char cor) : CPecas(cor){
}

char CBispo::GetPeca(){
	return'B';
}


bool CBispo::MovimentoQuadrantes(int origemY, int origemX, int destinoY, int destinoX, CPecas* APObjetos[8][8]) {
		if ((destinoX - origemX == destinoY - origemY) || (destinoX - origemX == origemY - destinoY)) {
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
