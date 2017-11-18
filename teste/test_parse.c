/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Testes desenvolvidos por Douglas Lázaro
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libs.h"

void test_parseSuperblock() {
  printf("\n--- Teste de parsing do bloco de boot ---\n\n");

  SECTOR_T Superblock;
  struct Superblock config;

  readSector(0, &Superblock);

  config = parseSuperblock(Superblock.at);

  printf("ID: %s\n", config.id);
  printf("Version: %s\n", config.version);
  printf("Block Size: %d\n", config.clusterSize);
  printf("Disk Sector Size: %d\n", config.diskSectorSize);

  printf("\n--- Encerrou parsing do bloco de boot ---\n\n");
}

void test_parseRegister() {
  printf("\n--- Teste de parsing de registro ---\n\n");

  int registerIndex = 0;
  REGISTER_T reg;

  if(readRegister(registerIndex, &reg) != TRUE) {
    return;
  }

  struct t2fs_4tupla *tuplas = malloc(constants.MAX_TUPLAS_REGISTER * sizeof(struct t2fs_4tupla));
  parseRegister(reg.at, tuplas);

  int i;
  for (i = 0; i < constants.MAX_TUPLAS_REGISTER; i++) {
    printf("\n");
    printTupla(tuplas[i]);
  }

  printf("\n--- Encerrou parsing de registro ---\n\n");
}

void test_parseDirectory() {
  printf("\n--- Teste de parsing de diretório ---\n\n");

  CLUSTER_T clusterBuffer;
  clusterBuffer.at = malloc(sizeof(unsigned char) * constants.CLUSTER_SIZE);
  struct t2fs_record records[constants.RECORD_PER_CLUSTER];

  if(readCluster(2050, &clusterBuffer) == FALSE) {
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

  /* Parse Register */
  test_parseRegister();

  /* Parse Directory */
  //test_parseDirectory();

  return 0;
}
