/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/2

Douglas Lázaro
*/

#include "libs.h"

struct t2fs_record createRecord(char* pathname, BYTE typeVal) {
	struct t2fs_record newFile;

	char ** parsedPath = malloc(sizeof(char) * MAX_FILE_NAME_SIZE);
	int parseCount = parsePath(pathname, parsedPath);

	newFile = initRecord(typeVal, parsedPath[parseCount], 1, 0, -1);

	/* Encontrar um cluster Livre na FAT, . */
	int check;
	int fileCluster = searchFAT(FAT_LIVRE); // Encontra cluster de dados para o arquivo
  
	check = setFAT(fileCluster, FAT_EOF); // Set o cluster como ocupado

	if (check < 0) {
		return FAT_ERROR;
	}
 
	newFile.firstCluster = fileCluster; // set firtCluster do arquivo para o cluster corespondente

	return newFile;
}

int createNewFile(char * filename, BYTE typeVal) {
	struct t2fs_record file;
	int return_value;
	int check;

	check = lookup(filename, &file);
	if(check == TRUE){
		printf("Create file ERROR, o arquivo já existe \n");
		return FALSE;
	}
	
	if(check == PARSED_PATH_ERROR){
      printf("Path '%s' inválida.\n", filename);
      return FALSE
    }

	if(canAddToLDAA(filename)) { 
		
		//Cria o record para o arquivo e set um index da Fat (cluster) para ele
		file = createRecord(filename, typeVal);

		// salvar record no diretório 
		check = addRecordToDirectory(file, filename, FALSE);
		if(check < 0) {
			return check;
		}

		// adicionar para LDAA, e retornar valor do handle 
		int handle = insertLDAA(file, filename);

		if(handle < 0){
			printf("Create file ERROR, não foi possivel adicionar o aquivo a LDAA \n");
			return FALSE;
		}
		
	} else {
		printf("Create file ERROR, não é possivel adicionar mais aquivos a LDAA \n");
		return FALSE;
	}

  return handle;
}

int addRecordToDirectory(struct t2fs_record record, char * filename, int updatingRecord) {
	int i;
	int recordIndex = -1;
	CLUSTER_T actualCluster;
 
	struct t2fs_record list_records[constants.RECORD_PER_CLUSTER];
	
	// Encontra o cluster referente ao diretório onde será criado o arquivo
	int clusterDir = findClusterDirectory(filename);
	if(clusterDir <= 1){
		printf("Create file ERROR, diretório não encontrado \n");
		return FALSE;
	}
	
	// Realiza a leitura do diretorio onde irá ser criado o arquivo
	if(readCluster(clusterDir, &actualCluster) != TRUE){
		printf("Create file ERROR, erro durante a leitura do cluster do diretório \n");
		return FALSE;
	}
	
	parseDirectory(actualCluster.at, list_records);
	
	//Procura pelo record vazio do CLuster do diretorio
	for(i = 0; i < constants.RECORD_PER_CLUSTER; i++){
		if(list_records[i] == NULL && recordIndex < 0) //record vazio
			recordIndex = i;
	}
	
	//Caso diretório cheio nenhum record vazio
	if(recordIndex < 0){
		printf("Create file ERROR, dirertório sem espaço para adicionar arquivo\n");
		return FALSE;
	}
	
	//Escreve o record no disco
	if(writeRecord(clusterDir, recordIndex, record) != TRUE){
		printf("Create file ERROR durante a escrita do record\n");
		return FALSE;
	}
	
	return TRUE;
}

/*
	Retorna o cluster do ultimo diretório do pathname
	Agora a função funciona somente para diretórios absolutos
	Estrutura copiada da Lookup
*/
int findClusterDirectory(char* pathname) {

	char ** parsedPath = malloc(sizeof(char) * MAX_FILE_NAME_SIZE);
	unsigned int parseCount = parsePath(pathname, parsedPath);
  
	if(parseCount == FALSE) {
		return PARSED_PATH_ERROR;
	}
  
	CLUSTER_T actualCluster;
	actualCluster.at = malloc(sizeof(unsigned char) * constants.CLUSTER_SIZE);

	struct t2fs_record list_records[constants.RECORD_PER_CLUSTER];

	if(readCluster(config.RootDirCluster, &actualCluster) != TRUE){
		return FALSE;
	}

	parseDirectory(actualCluster.at, list_records);

	unsigned int i = 0, j = 1;
	int found = FALSE, endReached = FALSE;
	int cluster = 0;
	while (i < parseCount && endReached){
		
		//Se chegou no limite ou tipo inválido, o arquivo desejado
		//não existe.
		if (j == constants.RECORD_PER_CLUSTER || j == -3){
			return FILE_NOT_FOUND;
		}
		
		//A partir da lista de records, se verifica o tipo.
		switch(list_records[j].typeVal){
			case TYPEVAL_INVALIDO:
				j = -3;
				i = parseCount;
				break;
			
			case TYPEVAL_DIRETORIO:
				if(strcmp(list_records[j].name, parsedPath[i]) == 0 && (list_records[j].TypeVal == TYPEVAL_DIRETORIO)) { 
					readCluster(list_records[i].firstCluster, &actualCluster);
					parseDirectory(actualCluster.at, list_records);
					cluster = list_records[i].firstCluster;
					i++;
					j = 0;
					break;
				} else {
					j++;
					break;
				}
			default: 
					j++;
					break;
		}
	}
	
	if(i < parseCount)
		return FALSE;
	else
		return cluster;
}

