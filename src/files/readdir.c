/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Douglas Lazaro
  Douglas Lázaro
*/

#include "libs.h"

int readDirectory(DIR2 handle, struct descritor descritor, DIRENT2 *dentry) {
  int return_value = -1;

  CLUSTER_T clusterBuffer;
  clusterBuffer.at = malloc(sizeof(unsigned char) * constants.CLUSTER_SIZE);

/*FAT*/
/*
  REGISTER_T reg;
  if(readRegister(descritor.record.MFTNumber, &reg) != TRUE) {
    return REGISTER_READ_ERROR;
  }

  struct t2fs_4tupla *tuplas = malloc(constants.MAX_TUPLAS_REGISTER * sizeof(struct t2fs_4tupla));
  parseRegister(reg.at, tuplas);

  unsigned int initialCluster = descritor.currentPointer / constants.CLUSTER_SIZE;
  unsigned int recordIndex = (descritor.currentPointer % constants.CLUSTER_SIZE) / RECORD_SIZE;
  unsigned int i = findOffsetTupla(tuplas, initialCluster, &reg);

  unsigned int currentCluster = tuplas[i].logicalBlockNumber + initialCluster;
  struct t2fs_record records[constants.RECORD_PER_CLUSTER];
  //printf("CP: %d, IB: %d, RI: %d, i: %d, CB: %d\n", descritor.currentPointer, initialCluster, recordIndex, i, currentCluster);

  if(readCluster(currentCluster, &clusterBuffer) == FALSE) {
    return FALSE;
  };

  parseDirectory(clusterBuffer, records);

  struct t2fs_record file = records[recordIndex];
  DIRENT2 dirEntry_temp;

  // Verifica se chegou ao final
  int k = 0, foundFile = FALSE;
  if(records[recordIndex].TypeVal != TYPEVAL_DIRETORIO && records[recordIndex].TypeVal != TYPEVAL_REGULAR) {
    // Verifica no restante do bloco se há mais arquivos
    for(k = recordIndex+1; k < constants.RECORD_PER_CLUSTER && foundFile != TRUE; k++) {
      if(records[k].TypeVal == TYPEVAL_DIRETORIO || records[k].TypeVal == TYPEVAL_REGULAR) {
        strcpy(dirEntry_temp.name, file.name);
        dirEntry_temp.fileType = file.TypeVal;
        dirEntry_temp.fileSize = file.bytesFileSize;

        memcpy(dentry, &dirEntry_temp, sizeof(DIRENT2));
        foundFile = TRUE;
        return_value = 0;
      }
    }

    currentCluster = initialCluster+1;
    // Verificar nos blocos contiguos se há mais arquivos
    while(currentCluster < tuplas[i].numberOfContiguosClusters && foundFile != TRUE){
        if(readCluster(currentCluster, &clusterBuffer) == FALSE) {
          return FALSE;
        };

        parseDirectory(clusterBuffer, records);
        recordIndex = (descritor.currentPointer % constants.CLUSTER_SIZE) / RECORD_SIZE;

        if(records[recordIndex].TypeVal != TYPEVAL_DIRETORIO && records[recordIndex].TypeVal != TYPEVAL_REGULAR) {
          // Verifica no restante do bloco se há mais arquivos
          for(k = recordIndex+1; k < constants.RECORD_PER_CLUSTER && foundFile != TRUE; k++) {
            if(records[k].TypeVal == TYPEVAL_DIRETORIO || records[k].TypeVal == TYPEVAL_REGULAR) {
              strcpy(dirEntry_temp.name, file.name);
              dirEntry_temp.fileType = file.TypeVal;
              dirEntry_temp.fileSize = file.bytesFileSize;

              memcpy(dentry, &dirEntry_temp, sizeof(DIRENT2));
              foundFile = TRUE;
              return_value = 0;
            }
        }
      }
      else {
          strcpy(dirEntry_temp.name, file.name);
          dirEntry_temp.fileType = file.TypeVal;
          dirEntry_temp.fileSize = file.bytesFileSize;

          memcpy(dentry, &dirEntry_temp, sizeof(DIRENT2));
          foundFile = TRUE;
          return_value = 0;
     }

     if(foundFile != TRUE)
      currentCluster++;
  }

    // Verifica nas tuplas seguintes se há mais arquivos
        // precisa achar alguma tupla seguinte que é map.
        // PS: SE tuplas[i+1] for FIM, não tem.]
   
    while(i < constants.MAX_TUPLAS_REGISTER && foundFile != TRUE){
        switch(tuplas[i+1].atributeType){
          case REGISTER_ADITIONAL:

            if(readRegister(tuplas[i+1].virtualBlockNumber, &reg) != TRUE) {
              return REGISTER_READ_ERROR;
            }
            free(tuplas);
            tuplas = malloc(constants.MAX_TUPLAS_REGISTER * sizeof(struct t2fs_4tupla));
            parseRegister(reg.at, tuplas);
            i = 0;
          
          case REGISTER_MAP:
               currentCluster = tuplas[i].logicalBlockNumber + initialCluster;

               if(readCluster(currentCluster, &clusterBuffer) == FALSE) {
                 return FALSE;
               };

               parseDirectory(clusterBuffer, records);
               recordIndex = (descritor.currentPointer % constants.CLUSTER_SIZE) / RECORD_SIZE;

               if(records[recordIndex].TypeVal != TYPEVAL_DIRETORIO && records[recordIndex].TypeVal != TYPEVAL_REGULAR) {
                 // Verifica no restante do bloco se há mais arquivos
                 for(k = recordIndex+1; k < constants.RECORD_PER_CLUSTER && foundFile != TRUE; k++) {
                   if(records[k].TypeVal == TYPEVAL_DIRETORIO || records[k].TypeVal == TYPEVAL_REGULAR) {
                     strcpy(dirEntry_temp.name, file.name);
                     dirEntry_temp.fileType = file.TypeVal;
                     dirEntry_temp.fileSize = file.bytesFileSize;

                     memcpy(dentry, &dirEntry_temp, sizeof(DIRENT2));
                     foundFile = TRUE;
                     return_value = 0;
                   }
               }
             }
             else {
                 strcpy(dirEntry_temp.name, file.name);
                 dirEntry_temp.fileType = file.TypeVal;
                 dirEntry_temp.fileSize = file.bytesFileSize;

                 memcpy(dentry, &dirEntry_temp, sizeof(DIRENT2));
                 foundFile = TRUE;
                 return_value = 0;
            }
            
            break;

          case REGISTER_FREE:
          case REGISTER_FIM:
          default:
            return -END_OF_DIR;
            break;
        }

        if(foundFile != TRUE)
          i++;
    }


    if(foundFile != TRUE) {
      return -END_OF_DIR;
    }
  } else {
    strcpy(dirEntry_temp.name, file.name);
    dirEntry_temp.fileType = file.TypeVal;
    dirEntry_temp.fileSize = file.bytesFileSize;

    memcpy(dentry, &dirEntry_temp, sizeof(DIRENT2));
    return_value = 0;
  }

  // Atualiza currentPointer na LDAA, para próximo arquivo.
  descritor.currentPointer += RECORD_SIZE;
  updateLDAA(handle, TYPEVAL_DIRETORIO, descritor);
*/
  return return_value;
}
