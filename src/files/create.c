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
  
	check = setFAT(fileLBN, FAT_EOF); // Set o cluster como ocupado

	if (check < 0) {
		return FAT_ERROR;
	}
 
	newFile.firstCluster = fileCluster; // set firtCluster do arquivo para o cluster corespondente

	return newFile;
}

int createNewFile(char * filename, BYTE typeVal) {
	struct t2fs_record file;
  
	int return_value = lookup(filename, &file);
  
	int check;

	if(return_value == TRUE){
		printf("Create file ERROR, o arquivo já existe \n");
		return FALSE;
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
			printf("Create file ERROR, onão é possivel mais adicionar aquivos a LDAA \n");
			return FALSE;
		}
		
	} else {
		printf("Create file ERROR, onão é possivel mais adicionar aquivos a LDAA \n");
		return FALSE;
	}

  return handle;
}

int addRecordToDirectory(struct t2fs_record record, char * filename, int updatingRecord) {
	int i;
	int recordIndex = -1;
	CLUSTER_T actualCluster;
 
	struct t2fs_record list_records[constants.RECORD_PER_CLUSTER];
	
	// Aqui não deve ser o root MODIFICAR
	if(readCluster(config.RootDirCluster, &actualCluster) != TRUE){
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
	
	//Escre o record no disco
	// Aqui não deve ser o root MODIFICAR
	if(writeRecord(config.RootDirCluster, recordIndex, record) != TRUE){
		printf("Create file ERROR durante a escrita do record\n");
		return FALSE;
	}
	
	return TRUE;
}
