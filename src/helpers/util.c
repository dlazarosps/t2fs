/*
  INF01142 - Sistemas Operacionais I
  Prof: Sergio Cechin
  T2FS - 2017/2

  Douglas Lazaro
  Henrique la Porta
  Rodrigo Okido
*/

#include "libs.h"

struct Constants initConstants(struct t2fs_superbloco supercluster) {
  struct Constants k;

  /*superbloco*/
  k.SUPERBLOCK_SIZE = supercluster.SuperBlockSize;
  k.SECTOR_PER_CLUSTER = supercluster.SectorsPerCluster;
  
  k.FAT_SECTOR = supercluster.pFATSectorStart;
  k.DATA_SECTOR = supercluster.DataSectorStart;
  k.SUPERBLOCK_CLUSTER = supercluster.RootDirCluster;
  k.DISK_SECTORS = supercluster.NofSectors;

  /* calculadas */
  k.DISK_CLUSTERS = k.DISK_SECTORS / k.SECTOR_PER_CLUSTER;

  k.FAT_SECTORS = k.DATA_SECTOR - k.FAT_SECTOR;
  k.DATA_SECTORS = k.DISK_SECTORS - k.FAT_SECTORS - k.SUPERBLOCK_SIZE;

  k.ROOT_SECTOR = k.DATA_SECTOR + (k.SUPERBLOCK_CLUSTER * k.SECTOR_PER_CLUSTER);

  k.CLUSTER_SIZE = SECTOR_SIZE * k.SECTOR_PER_CLUSTER;
  k.RECORD_PER_CLUSTER = k.CLUSTER_SIZE / RECORD_SIZE;

  /* pog */
  k.DATA_CLUSTER = k.SUPERBLOCK_CLUSTER - 1; // cluster 0 e 1
  k.FAT_CLUSTER = k.DATA_CLUSTER; // cluster 0 e 1

  return k;
}


int initConfig() {
  printf("Inicializando file system..."); fflush(stdout);

  SECTOR_T SuperBlock;

  if(readSuperblock(&SuperBlock) == FALSE) {
    return FALSE;
  };

  config.superbloco = parseSuperBlock(SuperBlock.at);
  constants = initConstants(config.superbloco);
  
  /*FAT*/
  config.indexFAT = malloc(sizeof(DWORD) * constants.DISK_CLUSTERS); //????
  initFAT();

  initLDAA();

	config.currentPath = openRoot("/");

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
