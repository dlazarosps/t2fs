/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/2

  Testes desenvolvidos por Douglas Lázaro
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libs.h"

void test_parseSuperblock() {
  printf("\n--- Teste de parsing do bloco de boot ---\n\n");

  SECTOR_T Superblock;
  struct t2fs_superbloco config;

  readSector(0, &Superblock);

  config = parseSuperBlock(Superblock.at);

  printf("ID: %s\n", config.id);
  printf("Version: %d\n", config.version);
  printf("SuperBlock Size: %d\n", config.SuperBlockSize);
  printf("Num of Sector: %d\n", config.NofSectors);
  printf("Sector Per Cluster: %d\n", config.SectorsPerCluster);
  printf("FAT start: %d\n", config.pFATSectorStart);
  printf("ROOT dir Cluster: %d\n", config.RootDirCluster);
  printf("DATA Sector: %d\n", config.DataSectorStart);

  printf("\n--- Encerrou parsing do bloco de boot ---\n\n");
}

void test_parseDirectory() {
  printf("\n--- Teste de parsing de diretório ---\n\n");

  CLUSTER_T clusterBuffer;
  clusterBuffer.at = malloc(sizeof(unsigned char) * constants.CLUSTER_SIZE);
  struct t2fs_record records[constants.RECORD_PER_CLUSTER];

  if(readCluster(2, &clusterBuffer) == FALSE) {
    return;
  };

  parseDirectory(clusterBuffer, records);
  int i;
  for (i = 0; i < constants.RECORD_PER_CLUSTER; i++) {
    printRecord(records[i]);
    printf("\n");
  }
  printf("\n--- Encerrou parsing de diretório ---\n\n");
}

int main(int argc, char const *argv[]) {
  initConfig();

  // PARSE BOOT BLOCK
  //test_parseSuperblock();

  /* Parse Directory */
  test_parseDirectory();

  return 0;
}
