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
  CLUSTER_T reg;
  unsigned int i, j;

  /*
  	Reserva os primeiros registros de Clusters por ser o super bloco e FAT
  */
  for (i = 0; i <= FAT_SECTOR; i++) {
    config.indexFAT[i] = FAT_OCUPADO;
  }

  for (i = FAT_SECTOR + 1, i < constants.DISK_CLUSTERS; i++) {
	
	//Caso erro na leitura do cluster (i)
    if(readCluster(i, &reg) != TRUE) {
      config.indexFAT[i] = FAT_ERROR;
    }
	
	/* 
		Aqui apenas para continuar funcionando
		Deve-se verificar se existe algo criado no disco
	*/
	config.indexFAT[i] = FAT_LIVRE;
	
  }
}

void printFAT(int begin, int end){
  int i;
  for (i = begin; i < end; ++i)
  {
    if (config.indexFAT[i] == FAT_LIVRE)
      printf("Index FAT [%d] =  LIVRE \n",i);
	else if (config.indexFAT[i] == FAT_ERROR)
      printf("Index FAT [%d] =  Cluster com ERRO \n",i);
    else
      printf("Index FAT [%d] =  OCUPADO \n",i);
  }

}

int getFAT(int clusterIndex){
  return config.indexFAT[clusterIndex];
}

int setFAT(int clusterIndex, int allocated){
	if(clusterIndex == SUPERBLOCK || clusterIndex == FAT_ROOT){
		printf("Acesso a index FAT [%d] negado, indice reservado\n", clusterIndex);
		return -1;
	}	

	if(config.indexFAT[clusterIndex] == FAT_ERROR){
		printf("Acesso a index FAT [%d] negado, cluster com ERRO\n", clusterIndex);
		return -1;
	}
	
    config.indexFAT[clusterIndex] = allocated;

    if (config.indexFAT[clusterIndex] == allocated)
      return TRUE;
    else
      return -1;
}

int searchFAT(int allocated){
  unsigned int i;
  for (i = REGISTER_REGULAR; i < constants.DISK_CLUSTERS; ++i)
  {
    if (config.indexFAT[i] == allocated)
      return i;
  }
  return -1;
}
