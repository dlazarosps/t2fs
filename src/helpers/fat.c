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
  SECTOR_T sec;
  unsigned int i, j, index = 0;
  unsigned char aux[4];
  char* str;
	
	// Ler todos os setores do cluster 1
  for (i = 0; i < constants.SECTOR_PER_CLUSTER; i++) {
	
	//Caso erro na leitura do de algum setor do cluster da fat
    if(readSector(i, &sec) != TRUE) {
    	printf("Erro durante a leitura de setores da FAT \n");
		return;
    }
	
	//"caminha" de 4 em 4 bytes pelo setor e salva o valor de acordo com o indice na FAT
  	for(j = 0; j <= FAT_SECTOR_SIZE-4; j += 4){
  		aux[0] = sec->at[j];
  		aux[1] = sec->at[j+1];
  		aux[2] = sec->at[j+2];
  		aux[3] = sec->at[j+3];
  		
  		int num = convertFourBytes(aux, 0, str); 
  		
  		if(num != 1)		
  			config.indexFAT[index] = num;
  		else
  			config.indexFAT[index] = FAT_ERROR;
  			
  		index++;
  		//Garantia para não ocorrer Segmataton Fault no IndexFat
  		if(index >= constants.DISK_CLUSTERS)
  			return;
  	}//End FOR 2
  }//End FOR 1
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
  for (i = 0; i < constants.DISK_CLUSTERS; ++i)
  {
    if (config.indexFAT[i] == allocated)
      return i;
  }
  return -1;
}

int saveFAT(int clusterIndex){
  int setorIndex;
  int i;
  float cluster;
  SECTOR_T buffer;

  if(clusterIndex != 0){
    
    //Grava um unico setor da FAT no disco
    

    cluster = clusterIndex / FAT_PER_SECTOR;
    setorIndex = ceilnum(cluster);
    setorIndex = (clusterIndex % FAT_PER_SECTOR == 0) ?  setorIndex + 1 : sector;

    i =  FAT_PER_SECTOR * setorIndex; 

    for (i; i < i + FAT_PER_SECTOR ; ++i){
      switch(config.indexFAT[i]){
        case FAT_ERROR:
          // concatenar 0xFFFFFFFE no buffer
          break;
        case FAT_EOF:
          // concatenar 0xFFFFFFFF no buffer
          break;
        default:
          // converter o valor em little-endian e concaternar no buffer
          break;

      }    
    }

    writeSector(setorIndex, buffer);

  }
  else{

    //Grava toda a FAT no disco
    i = 0;
    int j;
    for (j = 1; j <= FAT_SECTORS; ++j)
    {
      for (i; i < FAT_PER_SECTOR*j; ++i){
        switch(config.indexFAT[i]){
          case FAT_ERROR:
            // concatenar 0xFFFFFFFE no buffer
            break;
          case FAT_EOF:
            // concatenar 0xFFFFFFFF no buffer
            break;
          default:
            // converter o valor em little-endian e concaternar no buffer
            break;
        }    
      }

      writeSector(FAT_ROOT+j, buffer);

    }

  }
 return 0;
}

int ceilnum(float num){
  int inum = (int) num;
  if (num == (float) inum){
    return inum;
  }
  
  return inum+1;
}
