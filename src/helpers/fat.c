/*
  Sistemas Operacionais II
  T2FS - 2017/2

  Douglas Lazaro
  Henrique La Porta
  Rodrigo Okido
*/

#include "libs.h"

/*
	ARQUIVO NÃO FINALIZADO - Apenas copiado do MFT
*/

void initFAT() {
  BLOCK_T reg;
  unsigned int i, j;

  /*
  	Reserva os primeiros registros de Clusters por ser o super bloco e FAT
  */
  for (i = 0; i <= FAT_SECTOR; i++) {
    config.indexFAT[i] = FAT_OCUPADO;
  }

  // VERIFICAR - Aqui os acessos para FAT
  for (i = FAT_SECTOR, j = REGISTER_REGULAR*2; j < constants.DISK_CLUSTERS; i++, j += 2) {
    int cluster = constants.FAT_SECTOR + j;

	//Caso erro na leitura para execução
    if(readBlock(cluster, &reg) != TRUE) {
      return;
    }

	//Verificar t2fs_4tupla
    struct t2fs_4tupla tuplaInicial = parseRegister_tupla(reg.at, 0);

	//VERIFICAR - Aqui o FAT deve ocupado registra valores > 1
    if(tuplaInicial.atributeType == (unsigned int) REGISTER_FREE) {
      config.indexFAT[i] = FAT_LIVRE; //livre
    } else {
      config.indexFAT[i] = FAT_OCUPADO; //ocupado
    }
  }
}

void printFAT(int begin, int end){
  int i;
  for (i = begin; i < end; ++i)
  {
    if (config.indexMFT[i] == MFT_BM_LIVRE)
      printf("Index MFT [%d] =  LIVRE \n",i);
    else
      printf("Index MFT [%d] =  OCUPADO \n",i);
  }

}

int getFAT(int registerIndex){
  return config.indexMFT[registerIndex];
}

int setMFT(int registerIndex, int allocated){

    config.indexMFT[registerIndex] = allocated;

    if (config.indexMFT[registerIndex] == allocated)
      return TRUE;
    else
      return -1;
}

int searchFAT(int allocated){
  unsigned int i;
  for (i = REGISTER_REGULAR; i < constants.MAX_REGISTERS; ++i)
  {
    if (config.indexMFT[i] == allocated)
      return i;
  }
  return -1;
}
