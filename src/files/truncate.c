/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

Douglas Lázaro
*/

#include "libs.h"

int truncateFile(FILE2 handle, struct descritor descritor) {
  // int registerIndex = descritor.record.MFTNumber;
  CLUSTER_T clusterBuffer;
  clusterBuffer.at = malloc(sizeof(unsigned char) * constants.CLUSTER_SIZE);
/*FAT*/  
/*
  REGISTER_T reg;
  if(readRegister(registerIndex, &reg) != TRUE) {
    return -1;
  }

  struct t2fs_4tupla *tuplas = malloc(constants.MAX_TUPLAS_REGISTER * sizeof(struct t2fs_4tupla));
  parseRegister(reg.at, tuplas);

  unsigned int initialCluster = descritor.currentPointer / constants.CLUSTER_SIZE;
  unsigned int i = findOffsetTupla(tuplas, initialCluster, &reg);

  unsigned int k = i+1, isDone = FALSE, notAditionalReg = TRUE;
  int contiguousLeft = 0;

  // DESALOCAR TODOS BLOCOS DAS TUPLAS POSTERIORES A ATUAL
  while(k < constants.MAX_TUPLAS_REGISTER && isDone != TRUE) {
    switch(tuplas[k].atributeType) {
      case REGISTER_MAP:
        if(k == i+1 && notAditionalReg == TRUE) {
          tuplas[k].atributeType = REGISTER_FIM;
        } else {
          tuplas[k].atributeType = REGISTER_FREE;
        }
        writeTupla(reg.at, &tuplas[k], k);

        contiguousLeft = tuplas[k].numberOfContiguosClusters;
        while(contiguousLeft > 0) {
          // Libera blocos ocupados
          setBitmap2(tuplas[k].logicalBlockNumber + --contiguousLeft, BM_LIVRE);
        }
        k++;
      case REGISTER_ADITIONAL:
        tuplas[k].atributeType = REGISTER_FREE;
        writeTupla(reg.at, &tuplas[k], k);
        writeRegister(registerIndex, &reg);

        registerIndex = tuplas[k].virtualBlockNumber;
        readRegister(registerIndex, &reg);

        setMFT(registerIndex, MFT_BM_LIVRE);
        parseRegister(reg.at, tuplas);
        k = 0;
        notAditionalReg = FALSE;
      case REGISTER_FIM:
        if(k == i+1 && notAditionalReg == TRUE) {
          tuplas[k].atributeType = REGISTER_FIM;
        } else {
          tuplas[k].atributeType = REGISTER_FREE;
        }

        writeTupla(reg.at, &tuplas[k], k);
        writeRegister(registerIndex, &reg);
      case REGISTER_FREE:
      default:
        isDone = TRUE;
        break;
    }
  }

  registerIndex = descritor.record.MFTNumber;
  if(readRegister(registerIndex, &reg) != TRUE) {
    return -1;
  }
  parseRegister(reg.at, tuplas);

  // Todos blocos depois da tupla atual estão liberados.
  // Apenas necessário liberar dentro da tupla.
  contiguousLeft = tuplas[i].numberOfContiguosClusters;

  // Libera blocos contiguos após o bloco atual
  if(initialCluster + 1 < tuplas[i].numberOfContiguosClusters) {
    contiguousLeft = tuplas[k].numberOfContiguosClusters - initialCluster + 1;
    while(contiguousLeft > 0) {
      // Libera blocos ocupados
      setBitmap2(tuplas[k].logicalBlockNumber + --contiguousLeft, BM_LIVRE);
    }
  }*/

  // Todos blocos depois do atual foram liberados. Apenas é necessário sinalizar o novo tamanho do arquivo.
  // Bytes posteriores ao tamanho do arquivo no disco serão ignorados na leitura.
  descritor.record.bytesFileSize = descritor.currentPointer;
  // descritor.record.clustersFileSize = (descritor.record.bytesFileSize / constants.CLUSTER_SIZE) + 1;

  // Atualiza descritor na LDAA
  updateLDAA(handle, TYPEVAL_REGULAR, descritor);

  // Atualiza record no diretório
  addRecordToDirectory(descritor.record, descritor.name, TRUE);


  return 0;
}
