/*
  INF01142 - Sistemas Operacionais I
  Prof: Sergio Cechin
  T2FS - 2017/2

  Douglas Lazaro
  Henrique la Porta
  Rodrigo Okido
*/

#include "libs.h"

struct Constants initConstants(struct t2fs_superbloco superblock) {
  struct Constants k;

  /* Disco */
  k.SECTOR_PER_CLUSTER = superblock.SectorsPerCluster;
  k.CLUSTER_SIZE = k.SECTOR_PER_CLUSTER * SECTOR_SIZE; // SECTOR_SIZE IS A DEFINED CONSTANT

  k.DISK_SECTORS = superblock.NofSectors;
  k.DISK_CLUSTERS = k.DISK_SECTORS / k.SECTOR_PER_CLUSTER;

  
  /* Partições do disco (Quantidade total de Cluster de cada partição) */
  k.SUPERBLOCK_CLUSTER_SIZE = 1;
  k.SUPERBLOCK_SECTOR_SIZE = SECTOR_SIZE;

  k.FAT_CLUSTER_SIZE = (superblock.DataSectorStart - superblock.pFATSectorStart)/ k.SECTOR_PER_CLUSTER;
  k.FAT_SECTOR_SIZE = k.FAT_CLUSTER_SIZE * k.SECTOR_PER_CLUSTER;

  k.DATA_CLUSTER_SIZE = k.DISK_CLUSTERS - (k.SUPERBLOCK_CLUSTER_SIZE + k.FAT_CLUSTER_SIZE);
  k.DATA_SECTOR_SIZE = k.DISK_SECTORS - (k.SUPERBLOCK_SECTOR_SIZE + k.FAT_SECTOR_SIZE);

  /* Cluster (Blocos) de inicio do disco */
  k.SUPERBLOCK_CLUSTER = 0;
  k.FAT_CLUSTER = k.SUPERBLOCK_CLUSTER + 1;
  k.DATA_CLUSTER = k.FAT_CLUSTER_SIZE + 1;
  
  /* Setores de inicio do disco */
  k.SUPERBLOCK_SECTOR = 0;
  k.FAT_SECTOR = superblock.pFATSectorStart;
  k.DATA_SECTOR = superblock.DataSectorStart;

  /* Records */
  k.RECORD_PER_CLUSTER = k.CLUSTER_SIZE / RECORD_SIZE;

  return k;
}


int initConfig() {
  printf("Inicializando file system..."); fflush(stdout);

  SECTOR_T superBlock;

  if(readBootBlock(&superBlock) == FALSE) {
    return FALSE;
  };

  struct t2fs_superbloco boot = parseSuperBlock(superBlock.at);
  constants = initConstants(boot);

  /*FAT*/

  initLDAA();

  config.initiated = TRUE;

  printf("\tInicialização encerrada.\n");
  return TRUE;
}

int convertTwoBytes(unsigned char* buffer, int index, char* str) {
  int num;

  sprintf(str, "%02X%02X", buffer[index + 1], buffer[index]);
  sscanf(str, "%x", &num);

  return num;
}

int convertFourBytes(unsigned char* buffer, int index, char* str) {
  int num;

  sprintf(str, "%02X%02X%02X%02X", buffer[index + 3], buffer[index + 2], buffer[index + 1], buffer[index]);
  sscanf(str, "%x", &num);

  return num;
}
