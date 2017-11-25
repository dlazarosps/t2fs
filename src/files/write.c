/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/2

  Rodrigo Okido
*/

#include "libs.h"

int writeFile(int handle, struct descritor descritor, char * buffer, unsigned int size) {
   
   int readfile, return_value;
  if(descritor.currentPointer < descritor.record.bytesFileSize) {
	//Le o arquivo até o final antes de escrever novos dados.
	//Atualiza apenas o current pointer para o final do arquivo do descritor
	//para que seja possível continuar a nova escrita de bytes a partir do currentPointer
	readfile = readFile(handle, descritor, buffer, size); 
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
  
  //Variaveis de controle
  int rest;
  unsigned int bytesWritten = 0, bytesLeft = size;
  unsigned int initialOffset = descritor.currentPointer % constants.CLUSTER_SIZE;
	
  //Cluster atual
  CLUSTER_T actualCluster;
  //Aloca memoria com o tamanho de um cluster
  actualCluster.at = malloc(sizeof(unsigned char) * constants.CLUSTER_SIZE);

  
  //Se arquivo do descritor ocupa mais de 1 cluster, significa que
  //o nextCluster sera maior que 0.  
  if(nextCluster > 0){
	    while (nextCluster > 0){ //Enquanto for maior que zero, percorre tabela de FAT
		  nextCluster = getFAT(nextCluster);
		  filesize -= 1024; //Diminui o tamanho do arquivo com o tamanho de um cluster.
							//Quando chegar no cluster onde o arquivo acaba, sobrara apenas
							//os bytes que faltam naquele cluster do arquivo.
		  descritorClusterIndex = nextCluster;
	  }
	  
  }
  
  //Le o cluster e testa se houve sucesso
  if(readCluster(descritorClusterIndex, &actualCluster) != TRUE){
		return FALSE;
  }
	  
  //recebe o indice do cluster final do descritor
  unsigned int actualIndexCluster = descritorClusterIndex;
  
  
  //verifica o resto do cluster livre
  rest = 1024 - filesize;

  //Precisará alocar um novo cluster para escrever tudo que precisa
  if(rest < size) {
	  int sector_free = 0;
	  
	  //Checa qual setor daquele cluster esta livre para receber novos dados.
	  //Neste caso se o resto for > 768 e < 1024. Significa que apenas o setor 0
	  //tem dado no cluster. Por isto recebe sector_free = 1 (que seria o indice).
	  if (rest > 768 && rest < 1024) {
		sector_free = 1;
	  }
	  
	  if (rest > 512 && rest < 768) {
		sector_free = 2;
	  }
	  
	  if (rest > 256 && rest < 512) {
		sector_free = 3;
	  }
	  
	  //Se sector_free > 0, escreve dados novos no setor que está livre.
	  if (sector_free > 0 ){
		  int bytesToCluster = sector_free*SECTOR_SIZE;
		  //Preenche o que falta de setores no cluster com os novos dados
		  memcpy(&actualCluster.at[sector_free*SECTOR_SIZE], &buffer[bytesWritten], bytesToCluster);
		  writeCluster(actualIndexCluster, &actualCluster);
		  bytesWritten += bytesToCluster;
		  bytesLeft -= bytesWritten;
	  }
	  
	  // Loop para escrever o restante dos dados em novos clusters.
	  while(bytesLeft > 0){
		  
		  //PROCURA UM NOVO CLUSTER LIVRE
		  while(getFAT(searchFreeCluster) != FAT_LIVRE){
			if(searchFreeCluster > 8192){
				return -1; //Não tem como escrever mais. Pois não há cluster livre
			}
			searchFreeCluster++;
		  }
		  
		  //Le o cluster indicado pelo indice do proximo cluster livre.
		  if(readCluster(searchFreeCluster, &actualCluster) != TRUE){
			return FALSE;
		  }	
		  
		  //Se os bytes que restam escrever for menor que o tamanho todo do cluster
		  if (bytesLeft <= constants.CLUSTER_SIZE) {
			  // Escreve o resto dos dados do buffer no novo cluster
			  memcpy(&actualCluster.at[initialOffset], &buffer[bytesWritten], bytesLeft);
			  writeCluster(searchFreeCluster, &actualCluster);
			  // Chegou no final da escrita
			  bytesWritten += bytesLeft;
			  bytesLeft = 0;
			  
			  descritor.record.bytesFileSize += bytesWritten;
			  descritor.currentPointer += bytesWritten;
			  // descritor.record.clustersFileSize = (descritor.record.bytesFileSize / constants.CLUSTER_SIZE) + 1;
			  
			  
			  //Atualiza a tabela de FAT 
			  setFAT(actualIndexCluster, FAT_EOF);
			  
			  //Atualiza os bytes escritos 
			  return_value = bytesWritten;
			  
			  
		  } else { //Se nao, o cluster todo recebera dados e um outro cluster livre sera necessario escrever o resto dos dados
			// Escreve dados no buffer do bloco, e depois escreve no disco.
			memcpy(&actualCluster.at[initialOffset], &buffer[bytesWritten], constants.CLUSTER_SIZE);
			writeCluster(searchFreeCluster, &actualCluster);

			bytesWritten += constants.CLUSTER_SIZE;
			bytesLeft -= constants.CLUSTER_SIZE;
			
			//Atualiza o descritor
			descritor.record.bytesFileSize += bytesWritten;
			descritor.currentPointer += bytesWritten;
			// descritor.record.clustersFileSize = (descritor.record.bytesFileSize / constants.CLUSTER_SIZE) + 1;
			
			
			//Atualiza a tabela de FAT 
			setFAT(actualIndexCluster, searchFreeCluster);
			
			//Atualiza o indice do cluster atual
			actualIndexCluster = searchFreeCluster;
			
			//Atualiza os bytes escritos 
			return_value = bytesWritten;
		  }
		  
		  
		  initialOffset = 0;
	  }
	  
	  //Ao final atualiza o descritor na LDAA
	  updateLDAA(handle, TYPEVAL_REGULAR, descritor);
			   
	  // Atualiza record no diretório
	  addRecordToDirectory(descritor.record, descritor.name, TRUE);
	  
  } else {
  
  // Escreve dados no buffer do bloco, e depois escreve no disco.
  memcpy(&actualCluster.at[rest], &buffer[bytesWritten], bytesLeft);
  writeCluster(actualIndexCluster, &actualCluster);
  // Chegou no final da escrita
  bytesWritten += bytesLeft;
  bytesLeft = 0;
  
  
  //Atualiza o descritor	  
  descritor.record.bytesFileSize += bytesWritten;
  descritor.currentPointer += bytesWritten;
  // descritor.record.clustersFileSize = (descritor.record.bytesFileSize / constants.CLUSTER_SIZE) + 1;
  
  //Atualiza a tabela de FAT 
  setFAT(actualIndexCluster, FAT_EOF);
  
  //Atualiza o descritor na LDAA
  updateLDAA(handle, TYPEVAL_REGULAR, descritor);
  
  //Atualiza record no diretório
  addRecordToDirectory(descritor.record, descritor.name, TRUE);

  //Atualiza os bytes escritos 
  return_value = bytesWritten;
  }
  
  return return_value;
  
  
 } 
