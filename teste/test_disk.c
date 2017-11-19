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

  readCluster(1, &clusterBuffer);
  printCluster(clusterBuffer.at);

  printf("-- ENCERROU READ CLUSTER --\n");
}

void test_showCluster() {
  printf("-- SHOW CLUSTER --\n");

  /* Mostrar blocos de importância */
  printf("-- BOOT CLUSTER --\n");
  showCluster(constants.SUPERBLOCK_CLUSTER);
  printf("-- MFT CLUSTER --\n");
  showCluster(constants.FAT_CLUSTER);
  printf("-- DATA CLUSTER --\n");
  showCluster(constants.DATA_CLUSTER);

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

void test_readRegister() {
  printf("-- READ REGISTER --\n");

  REGISTER_T reg;
  int registerIndex = 0;

  printf("Lendo registro %d\n", registerIndex);
  if(readRegister(registerIndex, &reg) != TRUE) {
    printf("Registro %d não existe, logo retorna erro.\n", registerIndex);
    return;
  }
  printRegister(reg.at, registerIndex);


  REGISTER_T reg2;
  int registerIndex2 = 4096;

  printf("\n\nLendo registro %d\n", registerIndex2);
  if(readRegister(registerIndex2, &reg2) != TRUE) {
    printf("Registro %d não existe, logo retorna erro.\n", registerIndex2);
    return;
  }

  printRegister(reg2.at, registerIndex2);

  printf("-- ENCERROU READ REGISTER --\n");
}

/* READ REGISTER */
void test_writeRegister() {
  printf("-- WRITE REGISTER --\n");

  REGISTER_T reg;

  REGISTER_T registerBefore, registerAfter;

  int registerRead = 5;   // Um dos registros definidos no disco
  int registerWrite = 20; // Um registro não definido qualquer

  /* SAVE PREVIOUS REGISTER */
  if(readRegister(registerWrite, &reg) != TRUE) {
    return;
  }
  memcpy((void*) &registerBefore.at, (void*) &reg, sizeof(REGISTER_T)); // Copiando _valor_ para registerBefore

  /* GET REGISTER TO COPY */
  if(readRegister(registerRead, &reg) != TRUE) {
    return;
  }

  /* WRITE NEW REGISTER */
  if(writeRegister(registerWrite, &reg) != TRUE) {
    return;
  };

  /* READ WRITTEN REGISTER */
  if(readRegister(registerWrite, &registerAfter) != TRUE) {
    return;
  }
  printf("------------------------------\n");
  printf("--------- Before \n");
  printRegister(registerBefore.at, registerWrite);
  printf("--------- After \n");
  printRegister(registerAfter.at, registerWrite);
  printf("------------------------------\n");

  /* WRITE OLD VALUE BACK ON REGISTER */
  if(writeRegister(registerWrite, &registerBefore) != TRUE) {
    return;
  };

  printf("-- ENCERROU WRITE REGISTER --\n");
}

void test_readRecord() {
  printf("-- READ RECORD --\n");

  struct t2fs_record record1, record2;

  readRecord(2050, 0, &record1);
  readRecord(2050, 1, &record2);

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
  record1.MFTNumber = 6;

  strcpy(record2.name, "file4");
  record2.MFTNumber = 7;

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

void test_freeRegister() {
  printf("-- FREE REGISTER --\n");

  REGISTER_T reg;
  readRegister(FAT_ROOT, &reg);
  printRegister(reg.at, 0);

  getchar();

  freeRegister(FAT_ROOT);
  readRegister(FAT_ROOT, &reg);
  printRegister(reg.at, 0);

  printf("-- ENCERROU FREE REGISTER --\n");
}

int main(int argc, char const *argv[]) {
  initConfig();

  /* READ SECTOR */
  test_readSector();

  /* READ CLUSTER */
  test_readCluster();

  /* SHOW CLUSTER */
  test_showCluster();

  /* WRITE SECTOR */
  test_writeSector();

  /* WRITE CLUSTER */
  test_writeCluster();

  /* READ REGISTER */
  test_readRegister();

  /* WRITE REGISTER */
  test_writeRegister();

  /* READ RECORD */
  test_readRecord();

  /* WRITE RECORD */
  test_writeRecord();

  /* FREE REGISTER */
  test_freeRegister();

  return 0;
}
