/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/2

Douglas Lázaro
*/

#include "libs.h"


int relativePath = FALSE;

int validPath(char* path) {
  int lastWasSlash = FALSE;

  int length = strlen(path);

  // Min Size || Path não começa no root ou diretorio corrente
  if(length == 0 || path[0] != '/' || path[0] != '.') {
    return FALSE;
  }
  
  if(path[0] == '.'){
	relativePath = TRUE;
  }

  // Dupla Barra
  int i;
  for(i = 0; i < length; i++) {
    if(path[i] == '/') {
      if (lastWasSlash == TRUE) {
        return FALSE;
      }

      lastWasSlash = TRUE;
    } else {
      lastWasSlash = FALSE;
    }
  }

  // Max Size
  char* temp = malloc(strlen(path));
  strcpy(temp, path);
  char * pch = strtok(temp, "/");

  while(pch != NULL) {
    if(strlen(pch) > MAX_FILE_NAME_SIZE) {
      return FALSE;
    }

    pch = strtok(NULL, "/");
  }

  return TRUE;
}

int isFileName(char* filename) {
  if(strlen(filename) == 0) {
    return -1;
  }

  char* temp = malloc(strlen(filename));
  strcpy(temp, filename);

  char * pch = strtok(temp, ".");

  if(strcmp(pch, filename)) { //nenhum ponto foi encontrado
    return FALSE; // é um nome de diretório
  }

  return TRUE; // is a directory
}

int getFileDirectory(char* path, char* directory) {
  char ** elements = malloc(sizeof(char) * MAX_FILE_NAME_SIZE);

  int count = parsePath(path, elements);
  if(count >= 2) {
    int i;
    sprintf(directory, "/%s", elements[1]);
    for (i = 2; i < count; i++) {
      sprintf(directory, "%s/%s", directory, elements[i]);
    }
  } else {
    strcpy(directory, "/");
  }

  return TRUE;
}

int parsePath(char* path, char ** elements) {
  if(validPath(path) == FALSE) {
    return FALSE;
  }

  char opPath[MAX_FILE_NAME_SIZE];
  strcpy(opPath, path);

  char * pch = strtok(opPath, "/");

  int i = 0;
  elements[i++] = "/";

  while(pch != NULL) {
    if(strlen(pch) > MAX_FILE_NAME_SIZE) {
      return FALSE;
    }

    elements[i] = (char*) malloc((strlen(pch) + 1) * sizeof(char));

    strcpy(elements[i++], pch);

    pch = strtok(NULL, "/");
  }

  return i -1;
}


int lookup(char* pathname, struct t2fs_record * fileRecord) {
  /* FAT */
  
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
  
  unsigned int i = 0, j = 1, k = 1;
  int found = FALSE, endReached = FALSE;
  
  if (relativePath){         // SE for PATH RELATIVO
    //ler FAT diretorio current   
	char* currentdir = getcwd2(pathname, sizeof(pathname)); //TEM QUE IMPLEMENTAR ESSA FUNÇÃO 
	char ** parsedCurrent = malloc(sizeof(char) * MAX_FILE_NAME_SIZE);
	int parseCurrentCount = parsePath(currentdir, parsedCurrent);
	
	while (i < parseCurrentCount){
		
		//Se chegou no limite ou tipo inválido, o arquivo desejado
		//não existe.
		if (j == constants.RECORD_PER_CLUSTER || j == -3){
			return FILE_NOT_FOUND;
		}
		
		//A partir da lista de records, se verifica o tipo.
		switch(list_records[j].typeVal){
			case TYPEVAL_INVALIDO:
				j = -3;
				i = parseCurrentCount;
				break;
			
			case TYPEVAL_DIRETORIO:
				if(strcmp(list_records[j].name, parsedCurrent[i]) == 0 && (list_records[i].TypeVal == TYPEVAL_DIRETORIO)) { // FILE NAME FOUND
					readCluster(list_records[i].firstCluster, &actualCluster);
					parseDirectory(actualCluster.at, list_records);
					i++;
					j = 0;
					break;
				} else {
					j++;
					break;
				}
				
			case TYPEVAL_REGULAR:			
				if(strcmp(list_records[j].name, parsedPath[1]) == 0 && (list_records[j].TypeVal == TYPEVAL_REGULAR )) { // FILE NAME FOUND
					memcpy((void*) fileRecord, (void*) &list_records[i], RECORD_SIZE);
					found = TRUE;
					i++;
					break;
				} else {
					j++;
					break;
				}
			default: 
					found = FALSE;
					break;
		}
	}
	

	// procurar arquivo com o nome "pathname"
    // SE achou 
    //  copia para fileRecord
    //  retorna TRUE
    // SENAO retorna FALSE
  }
  
  else {                         //SE for PATH ABSOLUTO
    //ler FAT diretorio root até chegar diretorio folha
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
				if(strcmp(list_records[j].name, parsedPath[i]) == 0 && (list_records[j].TypeVal == TYPEVAL_DIRETORIO)) { // FILE NAME FOUND
					readCluster(list_records[i].firstCluster, &actualCluster);
					parseDirectory(actualCluster.at, list_records);
					i++;
					j = 0;
					break;
				} else {
					j++;
					break;
				}
			case TYPEVAL_REGULAR:			
				if(strcmp(list_records[j].name, parsedPath[i]) == 0 && (list_records[j].TypeVal == TYPEVAL_REGULAR )) { // FILE NAME FOUND
					memcpy((void*) fileRecord, (void*) &list_records[i], RECORD_SIZE);
					found = TRUE;
					i++;
					break;
				} else {
					j++;
					break;
				}
			default: 
					found = FALSE;
					break;
		}
	}
	
	return found;	
	
	}
    // procurar arquivo com o nome "pathname"
    // SE achou 
    //  copia para fileRecord
    //  retorna TRUE
    // SENAO retorna FALSE
 }
  

struct t2fs_record initRecord(BYTE typeVal, char* name, DWORD clustersFileSize, DWORD bytesFileSize, DWORD MFTNumber) {
  struct t2fs_record record;

  memset(&record, 0, sizeof(record));

  record.TypeVal = typeVal;
  strcpy(record.name, name); // name é o último elemento parseado: /directory/directory/file1 -> name = file1
  record.bytesFileSize = bytesFileSize;

  return record;
}


DIRENT2 initDentry(struct t2fs_record record) {
  DIRENT2 dentry;

  strcpy(dentry.name, record.name);
  dentry.fileType = record.TypeVal;
  dentry.fileSize = record.bytesFileSize;

  return dentry;
}

