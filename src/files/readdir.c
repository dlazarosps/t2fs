/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/2

Douglas Lázaro
*/

#include "libs.h"

int readDirectory(DIR2 handle, struct descritor descritor, DIRENT2 *dentry) {
  int return_value = -1;
  int index = 0;

  CLUSTER_T bufferCluster;
  bufferCluster.at = malloc(sizeof(unsigned char) * constants.CLUSTER_SIZE);

  struct t2fs_record list_records[constants.RECORD_PER_CLUSTER];
  DIRENT2 dirEntry_temp;

  // ler diretório do handle
  if(readCluster(descritor.record.firstCluster, &bufferCluster) != TRUE){
    return FALSE;
  }
  
  // parse Dir to records
  parseDirectory(bufferCluster, list_records);


  // se currentpointer é menor que o tamanho ocupado do diretório
  if (descritor.currentPointer < descritor.record.bytesFileSize)
  {
    
    index = (descritor.currentPointer / RECORD_SIZE) - 1;

    if(list_records[index].TypeVal == TYPEVAL_DIRETORIO || list_records[index].TypeVal == TYPEVAL_REGULAR) {

      // copia record para dentro do dentry
      strcpy(dirEntry_temp.name, list_records[index].name);
      dirEntry_temp.fileType = list_records[index].TypeVal;
      dirEntry_temp.fileSize = list_records[index].bytesFileSize;

      memcpy(dentry, &dirEntry_temp, sizeof(DIRENT2));

      return_value = 0;
      
      // Atualiza currentPointer na LDAA, para próximo arquivo.
      descritor.currentPointer += RECORD_SIZE;
      updateLDAA(handle, TYPEVAL_DIRETORIO, descritor);
    }
    else{
      return_value = READDIR_ERROR;
    }

  }
  else{
    return_value =  -END_OF_DIR;
  }

  // retorna valor (0 = OK, -1 = END, -11 ERRO RECORD)
  return return_value;
}
