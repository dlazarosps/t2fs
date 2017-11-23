/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/2

  Rodrigo Okido
*/

#include "libs.h"

int writeFile(int handle, struct descritor descritor, char * buffer, unsigned int size) {
  
  char * tempBuffer;
  
  if(descritor.currentPointer < descritor.record.bytesFileSize) {
	//Le o arquivo até o final antes de escrever novos dados.
	//Atualiza apenas o current pointer para o final do arquivo do descritor.
	int readFile = readFile(handle, descritor, buffer, size); 
  }
    
  //Tamanho em bytes do arquivo.
  unsigned int filesize = descritor.record.bytesFileSize;
  
  //Numero do cluster onde se encontra o descritor
  unsigned int descritorClusterIndex = descritor.record.firstCluster;
  

  //Procura de novos clusters na FAT caso necessario 
  //Inicia em 2 pois os indices 0 e 1 sao reservados.
  unsigned int searchFreeCluster = 2;
  
  //Busca na tabela de FAT o próximo cluster do arquivo
  unsigned int nextCluster = getFAT(descritorClusterIndex);
  
  //
  int rest;
  unsigned int bytesWritten = 0, bytesLeft = size;
  unsigned int initialOffset = descritor.currentPointer % constants.CLUSTER_SIZE;
	
  CLUSTER_T actualCluster;
  actualCluster.at = malloc(sizeof(unsigned char) * constants.CLUSTER_SIZE);	

  
  //Se arquivo do descritor ocupa mais de 1 cluster	
  if(nextCluster > 0){
	    while (nextCluster > 0){
		  nextCluster = getFAT(nextCluster);
		  filesize -= 1024; 
		  descritorClusterIndex = nextCluster;
	  }
	  
  }
  
  if(readCluster(descritorClusterIndex, &actualCluster) != TRUE){
		return FALSE;
  }
	  
  unsigned int actualIndexCluster = descritorClusterIndex;
  
  
  //verifica o resto do cluster livre
  rest = 1024 - filesize;

  //Precisará alocar um novo cluster para escrever tudo que precisa
  if(rest < size) {
	  int sector_free = 0;
	  
	  if (rest > 768 && rest < 1024) {
		sector_free = 1;
	  }
	  
	  if (rest > 512 && rest < 768) {
		sector_free = 2;
	  }
	  
	  if (rest > 256 && rest < 512) {
		sector_free = 3;
	  }
	  
	  if (sector_free > 0 ){
		  int bytesToCluster = sector_free*SECTOR_SIZE;
		  //Preenche o que falta de setores no cluster com os novos dados
		  memcpy(&actualCluster.at[sector_free*SECTOR_SIZE], &buffer[bytesWritten], bytesToCluster);
		  writeCluster(descritorClusterIndex, &actualCluster);
		  // Chegou no final da escrita
		  bytesWritten += bytesToCluster;
		  bytesLeft -= bytesWritten;
	  }
	  
	  while(bytesLeft > 0){
		  
		  //PROCURA UM NOVO CLUSTER LIVRE
		  while(getFAT(searchFreeCluster) != FAT_LIVRE){
			if(searchFreeCluster > 8192){
				return -1; //Não tem como escrever mais. Pois não há cluster livre
			}
			searchFreeCluster++;
		  }
		  
		  if(readCluster(searchFreeCluster, &actualCluster) != TRUE){
			return FALSE;
		  }	
		  
		  if (bytesLeft <= constants.CLUSTER_SIZE) {
			  // Escreve o resto dos dados do buffer no novo cluster
			  memcpy(&actualCluster.at[initialOffset], &buffer[bytesWritten], bytesLeft);
			  writeCluster(searchFreeCluster, &actualCluster);
			  // Chegou no final da escrita
			  bytesWritten += bytesLeft;
			  bytesLeft = 0;
			  
			  descritor.record.bytesFileSize += bytesWritten;
			  descritor.currentPointer += bytesWritten;
			  descritor.record.clustersFileSize = (descritor.record.bytesFileSize / constants.CLUSTER_SIZE) + 1;
			  
			  
			  //Atualiza a tabela de FAT 
			  setFAT(actualIndexCluster, FAT_EOF);
			
			  return_value = bytesWritten;
			  
		  } else {
			// Escreve dados no buffer do bloco, e depois escreve no disco.
			memcpy(&clusterBuffer.at[initialOffset], &buffer[bytesWritten], constants.CLUSTER_SIZE);
			writeCluster(cluster, &clusterBuffer);

			bytesWritten += constants.CLUSTER_SIZE;
			bytesLeft -= constants.CLUSTER_SIZE;
			
			 
			descritor.record.bytesFileSize += bytesWritten;
			descritor.currentPointer += bytesWritten;
			descritor.record.clustersFileSize = (descritor.record.bytesFileSize / constants.CLUSTER_SIZE) + 1;
			
			
			//Atualiza a tabela de FAT 
			setFAT(actualIndexCluster, searchFreeCluster);
			
			//Atualiza o indice do cluster atual
			actualCluster = searchFreeCluster;
			return_value = bytesWritten;
		  }
		  
		  
		  initialOffset = 0;
	  }
	  
	  updateLDAA(handle, TYPEVAL_REGULAR, descritor);
			   
	  // Atualiza record no diretório
	  addRecordToDirectory(descritor.record, descritor.name, TRUE);
	  
  } else {
  
  // Escreve dados no buffer do bloco, e depois escreve no disco.
  memcpy(&actualCluster.at[rest], &buffer[bytesWritten], bytesLeft);
  writeCluster(cluster, &actualCluster);
  // Chegou no final da escrita
  bytesWritten += bytesLeft;
  bytesLeft = 0;
  
		  
  descritor.record.bytesFileSize += bytesWritten;
  descritor.currentPointer += bytesWritten;
  descritor.record.clustersFileSize = (descritor.record.bytesFileSize / constants.CLUSTER_SIZE) + 1;
  
  //Atualiza a tabela de FAT 
  setFAT(actualIndexCluster, FAT_EOF);
  
  
  updateLDAA(handle, TYPEVAL_REGULAR, descritor);
  
  // Atualiza record no diretório
  addRecordToDirectory(descritor.record, descritor.name, TRUE);

  return_value = bytesWritten;
  }
  
  return return_value;
  
  
 } 


  
/*  int registerIndex = descritor.record.MFTNumber;

  REGISTER_T reg;
  if(readRegister(registerIndex, &reg) != TRUE) {
    return FALSE;
  }

  struct t2fs_4tupla *tuplas = malloc(constants.MAX_TUPLAS_REGISTER * sizeof(struct t2fs_4tupla));
  parseRegister(reg.at, tuplas);

  CLUSTER_T clusterBuffer;
  clusterBuffer.at = malloc(sizeof(unsigned char) * constants.CLUSTER_SIZE);

  int allocated, fileClustersCounter = 0;
  unsigned int i = 0, cluster;
  unsigned int bytesWritten = 0, bytesLeft = size;
  unsigned int amountOfClustersRead = 0;

  // Achar tupla, bloco e offset inicial, de acordo com currentPointer.
  unsigned int bytesWrittenToCluster = 0;
  unsigned int initialCluster = descritor.currentPointer / constants.CLUSTER_SIZE;
  unsigned int initialOffset = descritor.currentPointer % constants.CLUSTER_SIZE;
  i = findOffsetTupla(tuplas, initialCluster, &reg);

  while (i < constants.MAX_TUPLAS_REGISTER && bytesLeft > (unsigned int) 0) {
    switch(tuplas[i].atributeType) {
      case REGISTER_MAP:
        amountOfClustersRead = initialCluster;
        initialCluster = 0;

        bytesWrittenToCluster = initialOffset;
        while(amountOfClustersRead < tuplas[i].numberOfContiguosClusters && bytesLeft > (unsigned int) 0) {
          cluster = tuplas[i].logicalBlockNumber + amountOfClustersRead;

          if(readCluster(cluster, &clusterBuffer) == FALSE) {
            return FALSE;
          };

          if(bytesLeft <= constants.CLUSTER_SIZE) {
            // Escreve dados no buffer do bloco, e depois escreve no disco.
            memcpy(&clusterBuffer.at[initialOffset], &buffer[bytesWritten], bytesLeft);
            writeCluster(cluster, &clusterBuffer);

            // Chegou no final da escrita
            bytesWritten += bytesLeft;
            bytesLeft = 0;

            // Atualiza descritor e record na LDAA
            // Se o ponteiro estiver antes do final, não deve incrementar o tamanho do arquivo, e sim apenas sobreescrever
            if(descritor.currentPointer < descritor.record.bytesFileSize) {
              unsigned int offBW = descritor.record.bytesFileSize - descritor.currentPointer;

              if(offBW < bytesWritten) {
                descritor.record.bytesFileSize += bytesWritten - offBW;
              }
            } else {
              descritor.record.bytesFileSize += bytesWritten;
            }
            descritor.currentPointer += bytesWritten;
            descritor.record.clustersFileSize = (descritor.record.bytesFileSize / constants.CLUSTER_SIZE) + 1;
            updateLDAA(handle, TYPEVAL_REGULAR, descritor);

            // Atualiza record no diretório
            addRecordToDirectory(descritor.record, descritor.name, TRUE);

            return_value = bytesWritten;
          } else {
            // Escreve dados no buffer do bloco, e depois escreve no disco.
            memcpy(&clusterBuffer.at[initialOffset], &buffer[bytesWritten], constants.CLUSTER_SIZE);
            writeCluster(cluster, &clusterBuffer);

            bytesWritten += constants.CLUSTER_SIZE;
            bytesLeft -= constants.CLUSTER_SIZE;
          }

          // Verificação se escreveu até o final do bloco. Se sim, incrementa o contador.
          bytesWrittenToCluster += bytesWritten;
          if(bytesWrittenToCluster >= constants.CLUSTER_SIZE) {
            bytesWrittenToCluster = 0;
            amountOfClustersRead++;
          }

          initialOffset = 0;
        }

        fileClustersCounter += amountOfClustersRead;

        if(bytesLeft > 0) {
          if(tuplas[i+1].atributeType == REGISTER_FIM) {
            // verificar se é possivel criar bloco contiguo na tupla atual
            cluster = tuplas[i].logicalBlockNumber + tuplas[i].numberOfContiguosClusters;
            allocated = getBitmap2(cluster);

            if(allocated < 0) {
              return BM_ERROR;
            }

            if(allocated == BM_LIVRE) {
              // Aloca bloco contíguo, atualizando o registro
              tuplas[i].numberOfContiguosClusters += 1;
              writeTupla(reg.at, &tuplas[i], i);
              setBitmap2(cluster, BM_OCUPADO);

              resetCluster(cluster);

              writeRegister(registerIndex, &reg);

              // Loop de tuplas começara novamente, partindo do novo bloco.
              initialCluster = tuplas[i].numberOfContiguosClusters - 1;
            } else {
              // próxima tupla está no final do registro
              if(i+1 == constants.MAX_TUPLAS_REGISTER - 1) {
                //próxima tupla vira uma REGISTER_ADITIONAL

                // Encontra indice para o novo registro. 
                int novoRegisterIndex = searchMFT(MFT_BM_LIVRE);
                int check = setMFT(novoRegisterIndex, MFT_BM_OCUPADO);
                if (check < 0) {
                  return MFT_BM_ERROR;
                }

                tuplas[i+1] = initTupla(REGISTER_ADITIONAL, novoRegisterIndex, 0, 0);
                writeTupla(reg.at, &tuplas[i+1], i+1);
                writeRegister(registerIndex, &reg);

                // Operações no novo registro
                int fileLBN;

                fileLBN = searchBitmap2(BM_LIVRE); // Encontra bloco de dados para o arquivo
                check = setBitmap2(fileLBN, BM_OCUPADO);
                if (check < 0) {
                  return BM_ERROR;
                }

                // Inicializa o novo registro.
                initNewRegister(novoRegisterIndex, fileClustersCounter, fileLBN);

                i = 0; // Reinicia o loop, no novo registro.
                if(readRegister(registerIndex, &reg) != TRUE) {
                  return FALSE;
                }
                parseRegister(reg.at, tuplas);
              } else { // próxima tupla vira um REGISTER_MAP
                int newLBN = searchBitmap2(BM_LIVRE); // Encontra bloco de dados para o arquivo

                // alocar novo bloco, setar como ocupado
                int check = setBitmap2(newLBN, BM_OCUPADO);
                if (check < 0) {
                  return BM_ERROR;
                }
                resetCluster(newLBN);

                // ATUALIZAÇÃO DO REGISTRO 
                readRegister(registerIndex, &reg);

                tuplas[i+1] = initTupla(REGISTER_MAP, fileClustersCounter, newLBN, 1);
                writeTupla(reg.at, &tuplas[i+1], i+1);

                tuplas[i+2] = initTupla(REGISTER_FIM, 0, 0, 0);
                writeTupla(reg.at, &tuplas[i+2], i+2);

                // Atualização do registro com novos valores.
                writeRegister(registerIndex, &reg);

                i++; // próxima iteração do loop ira escrever no novo bloco
              }
            }
          } else {
            // indica que o arquivo tem outras tuplas já alocadas
            i++;
          }
        }

        break;
      case REGISTER_ADITIONAL:
        // Ler novo registro e recomeçar a leitura.
        registerIndex = tuplas[i].virtualBlockNumber;

        if(readRegister(registerIndex, &reg) != TRUE) {
          return FALSE;
        }
        free(tuplas);
        tuplas = malloc(constants.MAX_TUPLAS_REGISTER * sizeof(struct t2fs_4tupla));

        parseRegister(reg.at, tuplas);
        i = 0; // reset i para 0, começar a ler tuplas novamente

      case REGISTER_FIM:
        bytesLeft = 0;
        return_value = bytesWritten;
        break;
      default:
        return_value = bytesWritten;
        break;
    }
  }
*/
 

