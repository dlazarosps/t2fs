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
  unsigned char aux[8];
  char str[4];
	
	// Ler todos os setores do cluster 1
  for (i = 0; i < constants.SECTOR_PER_CLUSTER; i++) {
	
	//Caso erro na leitura do de algum setor do cluster da fat
    if(readSector(i, &sec) != TRUE) {
    	printf("Erro durante a leitura do setor %d da FAT \n", i);
		return;
    }
	
	//"caminha" de 4 em 4 bytes pelo setor e salva o valor de acordo com o indice na FAT
  	for(j = 0; j <= constants.FAT_SECTOR_SIZE-4; j += 4){
  		// 4 bytes = 8 nº Hexas
  		aux[0] = sec.at[j];
  		aux[1] = sec.at[j+1];
  		aux[2] = sec.at[j+2];
  		aux[3] = sec.at[j+3];
  		aux[4] = sec.at[j+4];
  		aux[5] = sec.at[j+5];
  		aux[6] = sec.at[j+6];
  		aux[7] = sec.at[j+7];
  		
  		int num = convertFourBytes(aux, 0, str); 
  		
  		/*
  			Função "temporária" com alguns testes aqui fiquei meio na duvida
  			de exatamente qual valor é retornado pelo convertFourBytes()
  		*/
  		int op;
		if(aux[0] == 'F' && aux[1] == 'F' && aux[2] == 'F' && aux[3] == 'F' && aux[4] == 'F' && aux[5] == 'F' && aux[6] == 'F'){
			if(aux[7] == 'F')
				op = FAT_EOF;
			else if(aux[7] == 'E')
				op = FAT_ERROR;
			else
				op = 0;
		}
		
		switch(op){
			case FAT_ERROR:
				config.indexFAT[index] = FAT_ERROR;
				// concatenar 0xFFFFFFFE no buffer
				break;
			case FAT_EOF:
				config.indexFAT[index] = FAT_EOF;
				// concatenar 0xFFFFFFFF no buffer
				break;
			default:
				if(num != 1)		
		  			config.indexFAT[index] = num;
		  		else
					config.indexFAT[index] = FAT_ERROR;
			break;
		}
	
  		index++;
  		//Garantia para não ocorrer Segmataton Fault no IndexFat
  		if(index > constants.DISK_CLUSTERS)
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
  strcpy(buffer.at, "");

  if(clusterIndex != 0){
    
    //Grava um unico setor da FAT no disco
    

    cluster = clusterIndex / FAT_PER_SECTOR;
    setorIndex = ceilnum(cluster);
    setorIndex = (clusterIndex % FAT_PER_SECTOR == 0) ?  setorIndex + 1 : sector;

    i =  FAT_PER_SECTOR * setorIndex; 

    for (i; i < i + FAT_PER_SECTOR ; ++i){
      //converte valores para gravar no setor
      concat_fat(&buffer.at, config.indexFAT[i]);    
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
        
        concat_fat(&buffer.at, config.indexFAT[i]);

      }

      writeSector(FAT_ROOT+j, buffer);

    }

  }
 return 0;
}

void concat_fat(char * buffer, int value){
  unsigned char little[32];
  int aux;

  switch(value){
    case FAT_ERROR:
      // concatenar 0xFFFFFFFE no buffer
      strcat(buffer,"fffffffe");
      break;
    case FAT_EOF:
      // concatenar 0xFFFFFFFF no buffer
      strcat(buffer,"ffffffff");
      break;
    default:
      // converter o valor em little-endian e concaternar no buffer
      //GAMBIARRA VIOLENTA
      aux = changed_endian(value);
      sprintf(little, "%x", aux);
      strcat(buffer, little[0]);
      strcat(buffer, little[1]);
      strcat(buffer, little[2]);
      strcat(buffer, little[3]);
      strcat(buffer, little[4]);
      strcat(buffer, little[5]);
      strcat(buffer, little[6]);
      strcat(buffer, little[7]);

      break;
  }
}

//swap little <-> big
int changed_endian(int num){
 int byte0, byte1, byte2, byte3;

 byte0 = (num & 0x000000FF) >> 0;
 byte1 = (num & 0x0000FF00) >> 8;
 byte2 = (num & 0x00FF0000) >> 16;
 byte3 = (num & 0xFF000000) >> 24;
 return((byte0 << 24) | (byte1 << 16) | (byte2 << 8) | (byte3 << 0));
}


int ceilnum(float num){
  int inum = (int) num;
  if (num == (float) inum){
    return inum;
  }
  
  return inum+1;
}

int deleteIndexFAT(int clusterIndex){
	if(clusterIndex < 2){
		printf("Acesso a index FAT [%d] negado, indice reservado\n", clusterIndex);
		return FALSE;
	}
	int index = getFAT(clusterIndex);
	
	if(setFAT(clusterIndex, FAT_LIVRE) != TRUE){
		printf("DeleteIndexFAT ERROR, indexFAT [%d] não foi possivel alterar o valor\n", clusterIndex);
		return FALSE;
	}
	int aux;
	while(index != FAT_EOF){
		aux = getFAT(index);
		if(setFAT(index, FAT_LIVRE) != TRUE){
			printf("DeleteIndexFAT ERROR, indexFAT [%d] não foi possivel alterar o valor\n", clusterIndex);
			return FALSE;
		}
		index = aux;
	}
	
	return TRUE;

}














