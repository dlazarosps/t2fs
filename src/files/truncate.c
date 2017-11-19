/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/2

Douglas Lázaro
*/

#include "libs.h"

int truncateFile(FILE2 handle, struct descritor descritor) {
  
  int clusterIndex = descritor.record.firstCluster;
  int vectorFAT[MAX_FILES_OPEN];
  int aux, i;

  // inicializa o vetor de todos o cluster do arquivo
  for (i = 0; i < MAX_FILES_OPEN; ++i)
  {
    vectorFAT[i] = -1;
  }

  i = 0; 

  // acumula todos os clusters do arquivo a ser truncado
  do{
    switch(config.indexFAT[clusterIndex]){
      case FAT_ERROR:
      return FAT_ERROR;
        break;
      case FAT_EOF:
        vectorFAT[i] = clusterIndex;
        break;
      default:
        vectorFAT[i] = clusterIndex;
        aux =  config.indexFAT[clusterIndex];
        clusterIndex = aux;
        break;
    }
        i++;  
  }
  while(config.indexFAT[clusterIndex] != FAT_EOF);

  // libera todos os clusters depois do current pointer do arquivo
  unsigned int initialCluster = descritor.currentPointer / constants.CLUSTER_SIZE;
  i = initialCluster +1;

  do{
    if (vectorFAT[i] == -1)
      break;
    else{
      aux = vectorFAT[i];
      config.indexFAT[aux] = FAT_LIVRE;
      saveFAT(aux);
      i++;
    }
  }
  while(vectorFAT[i] != -1);


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
