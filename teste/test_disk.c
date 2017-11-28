/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/2

  Testes dos métodos de src/disk.c

  Desenvolvido por Douglas Lázaro
*/

#include "libs.h"

void test_readSector() {
  printf("-- READ SECTOR --\n");
  SECTOR_T sectorBuffer;

  readSector(0, &sectorBuffer);
  printSector(sectorBuffer.at);

  printf("-- ENCERROU READ SECTOR --\n");
}

void test_readCluster() {
  printf("-- READ CLUSTER --\n");
  CLUSTER_T clusterBuffer;
  clusterBuffer.at = malloc(sizeof(unsigned char) * constants.CLUSTER_SIZE);

  readCluster(2, &clusterBuffer);
  printCluster(clusterBuffer.at);

  printf("-- ENCERROU READ CLUSTER --\n");
}

void test_showCluster() {
  printf("-- SHOW CLUSTER --\n");

  /* Mostrar blocos de importância */
  printf("-- ROOT CLUSTER --\n");
  showCluster(2);
  printf("-- ARQUIVO CLUSTER --\n");
  showCluster(3);
  /*printf("-- DATA CLUSTER --\n");
  showCluster(constants.DATA_CLUSTER);*/

  printf("-- ENCERROU SHOW CLUSTER --\n");
}

void test_writeSector() {
  printf("-- WRITE SECTOR --\n");

  SECTOR_T sectorBuffer2, sectorBufferBackup;
  readSector(0, &sectorBuffer2);
  readSector(1, &sectorBufferBackup);

  writeSector(1, &sectorBuffer2);

  // Mostra setor modificado
  readSector(1, &sectorBuffer2);
  printSector(sectorBuffer2.at);

  // Restaura setor modificado
  writeSector(1, &sectorBufferBackup);
  readSector(1, &sectorBuffer2);
  printSector(sectorBuffer2.at);

  printf("-- ENCERROU WRITE SECTOR --\n");
}

void test_writeCluster() {
  printf("-- WRITE CLUSTER --\n");

  CLUSTER_T clusterBuffer, clusterBufferBackup;
  clusterBuffer.at = malloc(sizeof(unsigned char) * constants.CLUSTER_SIZE);
  clusterBufferBackup.at = malloc(sizeof(unsigned char) * constants.CLUSTER_SIZE);

  readCluster(0, &clusterBuffer);
  readCluster(1, &clusterBufferBackup);

  writeCluster(1, &clusterBuffer);

  // Mostra bloco modificado
  readCluster(1, &clusterBuffer);
  printCluster(clusterBuffer.at);

  // Restaura bloco modificado
  writeCluster(1, &clusterBufferBackup);
  readCluster(1, &clusterBuffer);
  printCluster(clusterBuffer.at);

  printf("-- ENCERROU WRITE CLUSTER --\n");
}


void test_readRecord() {
  printf("-- READ RECORD --\n");

  struct t2fs_record record1, record2;

  readRecord(2, 3, &record1); // cluster  | indice do record | struct record
  readRecord(2, 4, &record2);

  printRecord(record1);
  printf("\n");
  printRecord(record2);

  printf("-- ENCERROU READ RECORD --\n");
}

void test_executeWriteRecord(int cluster, int index, struct t2fs_record record) {
  if (writeRecord(cluster, index, record) == FALSE) {
    printf("Erro no write record: ");

    if(index >= constants.RECORD_PER_CLUSTER) {
      printf("Index passado maior do que %d. ", constants.RECORD_PER_CLUSTER);
    }

    if(index < 0) {
      printf("Index passado menor do que %d. ", 0);
    }

    if(cluster >= constants.DISK_CLUSTERS) {
      printf("Número de bloco maior do que %d (acima do limite do disco). ", constants.DISK_CLUSTERS -1);
    }

    if(cluster < constants.DATA_CLUSTER) {
      printf("Número de bloco menor do que %d (antes do bloco de dados). ", constants.DATA_CLUSTER);
    }

    printf("\n");
  } else {
    printf("Sucesso no write record.\n");
  }
}

void test_writeRecord() {
  printf("-- WRITE RECORD --\n");

  printf("Lendo records...\n");
  struct t2fs_record record1, record2;
  readRecord(2050, 0, &record1);
  readRecord(2050, 1, &record2);

  printf("Alterando valores para teste...\n");
  strcpy(record1.name, "file3");
  // record1.MFTNumber = 6;

  strcpy(record2.name, "file4");
  // record2.MFTNumber = 7;

  printf("Backup de records...\n");
  struct t2fs_record record1_backup, record2_backup, record3_backup;
  readRecord(3000, 5, &record1_backup);
  readRecord(3000, 11, &record2_backup);
  readRecord(3000, 15, &record3_backup);

  printf("Escrevendo records modificados...\n");
  test_executeWriteRecord(3000, 5, record1);
  test_executeWriteRecord(3000, 11, record2);
  test_executeWriteRecord(3000, 15, record2);

  /* Erros */
  test_executeWriteRecord(3000, 16, record2);
  test_executeWriteRecord(3000, -1, record2);
  test_executeWriteRecord(constants.DATA_CLUSTER -1, 15, record2);
  test_executeWriteRecord(constants.DISK_CLUSTERS, 15, record2);

  printf("\nImprimindo diretório alterado...\n");
  CLUSTER_T clusterBuffer;
  clusterBuffer.at = malloc(sizeof(unsigned char) * constants.CLUSTER_SIZE);
  struct t2fs_record records[constants.RECORD_PER_CLUSTER];

  if(readCluster(3000, &clusterBuffer) == FALSE) {
    return;
  };
  parseDirectory(clusterBuffer, records);

  int i;
  for (i = 0; i < constants.RECORD_PER_CLUSTER; i++) {
    printRecord(records[i]);
    printf("\n");
  }

  printf("Restaurando valores antigos...\n");
  writeRecord(3000, 5, record1_backup);
  writeRecord(3000, 11, record2_backup);
  writeRecord(3000, 15, record3_backup);

  printf("-- ENCERROU WRITE RECORD --\n");
}

int main(int argc, char const *argv[]) {
	printf("\n\n---- Inciando testes do Test_disk.c ----\n-- INIT CONFIG --\n");
  initConfig();

  /* READ SECTOR */
  test_readSector();

  /* READ CLUSTER */
  test_readCluster();

  /* SHOW CLUSTER */
  test_showCluster();

  /* WRITE SECTOR */
  // test_writeSector();

  /* WRITE CLUSTER */
  // test_writeCluster();

  /* READ RECORD */
  test_readRecord();

  /* WRITE RECORD */
  // test_writeRecord();

  return 0;
}
