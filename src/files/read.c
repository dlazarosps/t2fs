/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/2
	
  Rodrigo Okido
*/

#include "libs.h"

int readFile(int handle, struct descritor descritor, char * buffer, unsigned int size) {
  
  char * tempBuffer;
  int return_value, bytes;
  
  if(descritor.currentPointer >= descritor.record.bytesFileSize) {
    // Ponteiro após o final do arquivo.
    return 0;
  }

  //Pega o indice onde inicia o cluster do descritor passado por parâmetro
  unsigned int descritorCluster = descritor.record.firstCluster;
  
  //aloca memoria definido com o tamanho para um cluster
  CLUSTER_T actualCluster;
  actualCluster.at = malloc(sizeof(unsigned char) * constants.CLUSTER_SIZE);
  
  //Le o cluster e testa para ver se houve sucesso
  if(readCluster(descritorCluster, &actualCluster) != TRUE){
	return FALSE;
  }
  
  
  unsigned int bytesRead = 0, nextCluster;
  unsigned int bytesLeft, cpySize;

  // Se size for maior que o tamanho do arquivo, atribui 
  // bytesLeft como o tamanho do arquivo. Caso contrário,
  // bytesLeft recebe size
  if(size > descritor.record.bytesFileSize) {
    bytesLeft = descritor.record.bytesFileSize;
  } else {
    bytesLeft = size;
  }
  
  cpySize = bytesLeft;
  
  unsigned int initialOffset = descritor.currentPointer % constants.CLUSTER_SIZE;
  
  //Pega o próximo cluster do arquivo a ser lido (se houver)
  nextCluster = getFAT(descritorCluster);
  
  tempBuffer = malloc(sizeof(char) * cpySize);
  
  //Arquivo só ocupa um cluster
  if (nextCluster < 0){
	  
	bytes = bytesLeft;

	//Copia tamanho de "bytes" que há na memoria do cluster atual para o tempBuffer
    memcpy(&tempBuffer[bytesRead], &actualCluster.at[initialOffset], bytes);
    bytesRead += bytes; //Aumenta o numero de bytes lido
    descritor.currentPointer += bytesRead; //Aumenta o currentPointer com a quantidade de bytes lida do arquivo
    updateLDAA(handle, TYPEVAL_REGULAR, descritor); //Atualiza o descritor após o currentPointer aumentar

	//Copia cpySize bytes do tempBuffer para o buffer.
    memcpy(buffer, tempBuffer, sizeof(char) * cpySize);
    bytesLeft = 0; //Como havia um cluster, chegou ao fim do arquivo. Nao ha mais bytes para ler
    return_value = bytesRead; //Devolve o numero de bytes lido do arquivo.

  
  } else {
	  
	  while(nextCluster > 0 && bytesLeft > (unsigned int) 0){
		  
		  switch(nextCluster){
			
			//Chegou no cluster onde termina o arquivo. Pois atingiu FAT_EOF
			case FAT_EOF:
			
				//Executa as mesmas operações explicadas quando o arquivo tem menos de um
				//cluster (Explicado no 'If' logo acima)
				bytes = bytesLeft;

				memcpy(&tempBuffer[bytesRead], &actualCluster.at[initialOffset], bytes);
				bytesRead += bytes;
				descritor.currentPointer += bytesRead;
				updateLDAA(handle, TYPEVAL_REGULAR, descritor);

				memcpy(buffer, tempBuffer, sizeof(char) * cpySize);
				bytesLeft = 0;
				return_value = bytesRead;
			
				break;
			
			default:
			
				//Caso o arquivo tenha mais de um cluster, todo o cluster é lido e passado ao tempBuffer.
				memcpy(&tempBuffer[bytesRead], &actualCluster.at[initialOffset], constants.CLUSTER_SIZE);
				bytesRead += constants.CLUSTER_SIZE; //Todo o cluster foi lido, portanto os bytes lidos será o tamanho do cluster (256 bytes * 4 setores)
				bytesLeft -= constants.CLUSTER_SIZE; //Bytes que restam será o que falta menos o tamanho do cluster.
				descritor.currentPointer += bytesRead; //currentPointer atualizado adicionando os bytesRead.
				updateLDAA(handle, TYPEVAL_REGULAR, descritor); //Atualiza o descritor após o currentPointer aumentar

				//memcpy(buffer, tempBuffer, sizeof(char) * cpySize); //ERRADO. Só pode mandar ao buffer após ler todo o arquivo.

				return_value = bytesRead; //Atualiza a quantidade de bytes lido para devolver ao final
				
				nextCluster = getFAT(nextCluster); //Verifica o proximo cluster na tabela de FAT
				
				if(readCluster(nextCluster, &actualCluster) != TRUE){
					return FALSE; //Cluster possivelmente defeituoso ou alguma operação errada de leitura do cluster ocorreu
				}

				break;
		  }
		  
		  initialOffset = 0;
		  
	  }
  }
  
  return return_value;

}
