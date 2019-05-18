#include "CPecas.h"

CPecas::CPecas(char cor){
	corpeca = cor;
    movido = 0;
}

CPecas::~CPecas(){}

char CPecas::GetCor(){
	return corpeca;
}

int CPecas::getMovido(){
    return movido;
}

void CPecas::addMovido(){
    movido++;
}

bool CPecas::MovimentoPossivel(int origemY, int origemX, int destinoY, int destinoX, CPecas* APObjetos[8][8])
{
	CPecas* destinopeca = APObjetos[destinoX][destinoY];
	if ((destinopeca == 0) || (corpeca != destinopeca->GetCor())) {
		return MovimentoQuadrantes(origemY, origemX, destinoY, destinoX, APObjetos);
	}
	return false;
}
