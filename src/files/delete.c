/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/2

Douglas Lázaro
*/

#include "libs.h"

int removeFileFromDirectory(DWORD directoryMFTNumber, struct t2fs_record file) {
  int return_value;
/* FAT */
/*
  int registerIndex = directoryMFTNumber;
  REGISTER_T reg;

  if(readRegister(registerIndex, &reg) != TRUE) {
    return FALSE;
  }

  struct t2fs_4tupla *tuplas = malloc(constants.MAX_TUPLAS_REGISTER * sizeof(struct t2fs_4tupla));
  parseRegister(reg.at, tuplas);

  struct t2fs_record records[constants.RECORD_PER_CLUSTER];
  CLUSTER_T clusterBuffer;
  clusterBuffer.at = malloc(sizeof(unsigned char) * constants.CLUSTER_SIZE);

  unsigned int i = 0, amountOfClustersRead = 0, removedFile = FALSE, cluster;

  while (i < constants.MAX_TUPLAS_REGISTER && removedFile != TRUE) {
    switch (tuplas[i].atributeType) {
      case REGISTER_MAP:
        while(amountOfClustersRead < tuplas[i].numberOfContiguosClusters && removedFile != TRUE) {
          cluster = tuplas[i].logicalBlockNumber + amountOfClustersRead;
          amountOfClustersRead++;

          if(readCluster(cluster, &clusterBuffer) == FALSE) {
            return FALSE;
          };

          parseDirectory(clusterBuffer, records);

          unsigned int j;
          for (j = 0; j < constants.RECORD_PER_CLUSTER && removedFile != TRUE; j++) {
            if(strcmp(records[j].name, file.name) == 0 && (records[j].TypeVal == TYPEVAL_REGULAR || records[j].TypeVal == TYPEVAL_DIRETORIO)) {
              // Achou arquivo válido com o mesmo nome
              file.TypeVal = TYPEVAL_INVALIDO;

              if(writeRecord(tuplas[i].logicalBlockNumber, j, file) == FALSE) {
                return RECORD_WRITE_ERROR;
              };

              removedFile = TRUE;
              return_value = i;
            }
          }
        }

        amountOfClustersRead = 0;

        if(removedFile != TRUE) {
          i++;
        }

      break;
    case REGISTER_FIM:
      // CHEGOU AO FIM DAS TUPLAS E NÃO ACHOU
      return REGISTER_FIM;
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

      break;
    default:
      return_value = FIND_REGISTER_FREE;
      i++;
      break;
    }
  }*/

  return return_value;
}

int deleteFileFromDisk(struct t2fs_record file, char* filename) {
  /* Remover record do diretório */
/* FAT */
  int return_value = -1;
/* 
  char * directoryname = malloc(strlen(filename));

 getFileDirectory(filename, directoryname);

  if(strcmp("/", directoryname) == 0) { // remover da root
    return_value = removeFileFromDirectory(FAT_ROOT, file);
  } else {
    struct t2fs_record directory;

    return_value = lookup(directoryname, &directory);

    switch (return_value) {
      case REGISTER_READ_ERROR:
        printf("Erro crítico na leitura de um registro.\n");
        break;
      case FIND_REGISTER_ADITIONAL:
        printf("ERRO! Valor de retorno de lookup nunca deve ser FIND_REGISTER_ADITIONAL.\n");
        break;
      case FIND_REGISTER_NOTFOUND:
      case FIND_REGISTER_FIM:
      case FIND_REGISTER_FREE:
        printf("Diretório '%s' não encontrado.\n", directoryname);
        break;
      default:
        return_value = removeFileFromDirectory(directory.MFTNumber, file);
        break;
    }
  }

  // Desalocar blocos e registros do arquivo 
  removeFileFromMFT(file);
*/
  return return_value;
}
