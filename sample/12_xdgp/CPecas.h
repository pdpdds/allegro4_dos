#ifndef __CPecas_H__
#define __CPecas_H__

class CPecas
{
private:
	virtual bool MovimentoQuadrantes(int ,int, int, int,CPecas*[8][8]) = 0;
	int origemY,origemX,destinoY,destinoX;
	char corpeca;
    int movido;
public:
	CPecas(char);
	~CPecas();
	virtual char GetPeca() = 0;
	char GetCor();
    int getMovido();
    void addMovido();
	bool MovimentoPossivel(int, int , int, int, CPecas* [8][8]) ;
};
#endif
