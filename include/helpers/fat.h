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
-----------------------------------------------------------------------------*/
  void initFAT();

/*-----------------------------------------------------------------------------
Função: Usada para imprimir a FAT
Entra: int begin -> índice inicial
	   int end -> índice final

Saída: print dos índices da FAT entre begin e end.
-----------------------------------------------------------------------------*/
  void printFAT(int begin, int end);

/*------------------------------------------------------------------------
	Retorna a informação de alocação do índice FAT “clusterIndex”.
Entra:
	clusterIndex -> numero do índice FAT cujo os 4 bytes deve ser retornado
Retorna:
	Sucesso: valor da alocacao: ZERO (livre) ou >0 (alocado)
	Erro: número negativo
------------------------------------------------------------------------*/
  int getFAT(int clusterIndex);

/*------------------------------------------------------------------------
	Seta a informação de alocação do índice FAT “clusterIndex”.
	Essa informação é passada no parâmetro “allocated”.
	Esse parâmetro pode ser:
Entra:
	clusterIndex -> numero do índice FAT cujo os bytes devem ser setados
	allocated -> valor a ser escrito
		==0 -> coloca em LIVRE (ZERO)
		!=0 -> coloca coloca o valor corespondente allocated
Retorna
	Sucesso: TRUE (1)
	Erro: número negativo
------------------------------------------------------------------------*/
  int setFAT(int clusterIndex, int allocated);

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
------------------------------------------------------------------------*/
  int searchFAT(int allocated);

#endif
