/*
  Sistemas Operacionais II
  T2FS - 2017/2

  Douglas Lazaro
  Henrique La Porta
  Rodrigo Okido
*/

#ifndef __helperfat__
#define	__helperfat__

/*-----------------------------------------------------------------------------
Função: Usada para iniciar a FAT
	Essa função preenche o a FAT com o valor
	"0" se o cluster está livre na área do disco.
	ou
	"OutrosValores > 1" Se esta ocupada

Entra: void

Saída: void

Responsável: Henrique La Porta
-----------------------------------------------------------------------------*/
  void initFAT();

/*-----------------------------------------------------------------------------
Função: Usada para imprimir a FAT
Entra: int begin -> índice inicial
	   int end -> índice final

Saída: print dos índices da FAT entre begin e end.

Responsável: Henrique La Porta
-----------------------------------------------------------------------------*/
  void printFAT(int begin, int end);

/*------------------------------------------------------------------------
	Retorna a informação de alocação do índice FAT “registerIndex”.
Entra:
	registerIndex -> numero do índice FAT cujo os 4 bytes deve ser retornado
Retorna:
	Sucesso: valor da alocacao: ZERO (livre) ou >0 (alocado)
	Erro: número negativo

Responsável: Henrique La Porta
------------------------------------------------------------------------*/
  int getFAT(int registerIndex);

/*------------------------------------------------------------------------
	Seta a informação de alocação do índice FAT “registerIndex”.
	Essa informação é passada no parâmetro “allocated”.
	Esse parâmetro pode ser:
Entra:
	registerIndex -> numero do índice FAT cujo os bytes devem ser setados
	allocated -> valor a ser escrito
		==0 -> coloca bit em LIVRE (ZERO)
		!=0 -> coloca bit em OCUPADO
Retorna
	Sucesso: TRUE (1)
	Erro: número negativo

Responsável: Henrique La Porta
------------------------------------------------------------------------*/
  int setFAT(int registerIndex, int allocated);

/*------------------------------------------------------------------------
	Procura, a partir do índice FAT “0” (zero), no vetor de índices do FAT,
	um índice indicado por “allocated”.
Entra:
	allocated -> valor procurado
		“0” (zero), se deve procurar por um índice FAT desalocado (LIVRE)
		outros valores, se deve procurar por um índice FAT alocado (ALOCADO)
Retorna
	Sucesso: número do índice FAT encontrado (número >= 0)
	Erro: número negativo

Responsável: Henrique La Porta
------------------------------------------------------------------------*/
  int searchFAT(int allocated);

#endif
