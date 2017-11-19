/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/2

Douglas Lázaro
*/

#include "libs.h"

int printSector(unsigned char* buffer) {
  return printSector2(buffer, 0);
}

int printSector2(unsigned char* buffer, int currentSector) {
  unsigned int i, j;
  char str[20];

  for (i = 0; i < 16; ++i) {
    printf("%04X  ", (SECTOR_SIZE * currentSector) + 16 * i);
    for (j = 0; j < 16; ++j) {
      unsigned int index = 16*i+j;
      char c = buffer[index];

      if (c>=' ' && c<='z') {
        str[j] = c;
      } else {
        str[j] = ' ';
      }

      printf ("%02X ", c&0xFF);
    }

    str[16]='\0';
    printf (" *%s*\n", str);
  }

  return 0;
}

int printCluster(unsigned char* buffer) {
  unsigned int i;

  for(i = 0; i < constants.SECTOR_PER_CLUSTER; i++) {
    printSector2(&buffer[i * SECTOR_SIZE], i);
    printf("\n");
  }

  return 0;
}

void showCluster(int cluster) {
  CLUSTER_T buffer;
  buffer.at = malloc(sizeof(unsigned char) * constants.CLUSTER_SIZE);

  readCluster(cluster, &buffer);

  printCluster(buffer.at);

  return;
}

void printSuperblock() {
  printf("ID: %s\n", config.superbloco.id);
  printf("Version: %d\n", config.superbloco.version);
  printf("Disk Size: %d\n", config.superbloco.DiskSize);
  printf("Num of Sectors: %d\n", config.superbloco.NofSectors);
  printf("Sectors Per Cluster: %d\n", config.superbloco.SectorsPerCluster);
  printf("FAT Sector Start: %d\n", config.superbloco.pFATSectorStart);
  printf("Root Dir Cluster: %d\n", config.superbloco.RootDirCluster);
  printf("Data Sector Start: %d\n", config.superbloco.DataSectorStart);
}

void printRegister(unsigned char* buffer, int sector) {
  unsigned int i;

  for (i = 0; i < 2; i++) {
    printSector2(&buffer[i * SECTOR_SIZE], sector + i);
  }
}

void printRecord(struct t2fs_record record) {
  printf("TypeVal: %d\n", record.TypeVal);
  printf("Name: %s\n", record.name);
  printf("Bytes: %d\n", record.bytesFileSize);
}

void printDescritor(struct descritor file) {
  printf("FLAG: %d\n", file.flag);
  printf("Current Pointer: %d\n", file.currentPointer);

  printRecord(file.record);
}
