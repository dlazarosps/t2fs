/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/2

Douglas Lázaro
*/

#include "libs.h"

int deleteFileFromDisk(struct t2fs_record file, char* filename) {
	int recordIndex = -1;
	int clusterDir, i;
	CLUSTER_T actualCluster; 
	struct t2fs_record record;

	struct t2fs_record list_records[constants.RECORD_PER_CLUSTER];

	int check = getcwd2(filename, sizeof(filename));
	if(check!=TRUE) return FALSE;

	// Encontra o cluster referente ao diretório onde será criado o arquivo
	// função esta no create.c (LEMBRAR DE REVISAR) e/ou trocar de lugar
	if(file.TypeVal == TYPEVAL_DIRETORIO)
		clusterDir = findClusterDirectory(filename, 1);
	else
		clusterDir = findClusterDirectory(filename, 0);
		
	if(clusterDir <= 1){
		printf("Delete file ERROR, diretório não encontrado \n");
		return FALSE;
	}

	// Realiza a leitura do diretorio onde irá ser criado o arquivo
	if(readCluster(clusterDir, &actualCluster) != TRUE){
		printf("Delete file ERROR, erro durante a leitura do cluster do diretório \n");
		return FALSE;
	}
	
	parseDirectory(actualCluster, list_records);

	//Procura pelo record vazio do CLuster do diretorio
	for(i = 0; i < constants.RECORD_PER_CLUSTER; i++){
		if(list_records[i].TypeVal == RECORD_TYPE && recordIndex < 0) //record vazio
			recordIndex = i;
	}

	//Caso record não encontrado
	if(recordIndex < 0){
		printf("Delete file ERROR, record do arquivo não encontrado no diretório\n");
		return FALSE;
	}

	//Escreve o record vazio no disco
	if(writeRecord(clusterDir, recordIndex, record) != TRUE){
		printf("Delete file ERROR enquanto apagava o record do disco\n");
		return FALSE;
	}

	//Apaga registro da fat
	if(deleteIndexFAT(file.firstCluster) != TRUE){
		printf("Delete file ERROR não foi possivel apagar registro da indexFat\n");
		return FALSE;
	}

  return TRUE;
}
