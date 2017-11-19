/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/2

Douglas Lázaro
*/

#include "libs.h"

int validPath(char* path) {
  int lastWasSlash = FALSE;

  int length = strlen(path);

  // Min Size || Path não começa no root
  if(length == 0 || path[0] != '/') {
    return FALSE;
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
  
  if (parseCount <= 1){         // SE for PATH RELATIVO
    //ler FAT diretorio current
    // procurar arquivo com o nome "pathname"
    // SE achou 
    //  copia para fileRecord
    //  retorna TRUE
    // SENAO retorna FALSE
  }
  else{                         //SE for PATH ABSOLUTO
    //ler FAT diretorio root até chegar diretorio folha
    // procurar arquivo com o nome "pathname"
    // SE achou 
    //  copia para fileRecord
    //  retorna TRUE
    // SENAO retorna FALSE
  }
  
  return -1;
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

