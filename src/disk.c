/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/2

Douglas Lázaro
*/

#include "libs.h"

int readSector(int sector, SECTOR_T* buffer) {
  if(sector < 0 || sector > (int) constants.DISK_SECTORS) {
    return FALSE;
  }

  if (read_sector((unsigned int) sector, (unsigned char*) buffer) == 0) {
    return TRUE;
  }

  return FALSE;
}

int readCluster(int cluster, CLUSTER_T* buffer) {
  unsigned int i, sector;

  if (cluster < 0 || cluster > (int) constants.DISK_CLUSTERS) {
    return FALSE;
  }

  sector = cluster * constants.SECTOR_PER_CLUSTER;

  for(i = 0; i < constants.SECTOR_PER_CLUSTER; i++) {
    if (readSector(sector, (SECTOR_T*) &buffer->at[i * SECTOR_SIZE]) == FALSE) {
      return FALSE;
    }

    sector++;
  }

  return TRUE;
}

int readSuperblock(SECTOR_T* buffer) {
  if (read_sector(SUPERBLOCK, (unsigned char*) buffer) == 0) {
    return TRUE;
  }

  return FALSE;
}

int readRecord(int cluster, int index, struct t2fs_record * record) {
  if(cluster < (int) constants.DATA_CLUSTER || cluster >= (int) constants.DISK_CLUSTERS || index < 0 || index >= (int) constants.RECORD_PER_CLUSTER) {
    return FALSE;
  }

  int offset = (index * RECORD_SIZE);
  CLUSTER_T clusterBuffer;
  clusterBuffer.at = malloc(sizeof(unsigned char) * constants.CLUSTER_SIZE);

  if (readCluster(cluster, &clusterBuffer) == FALSE) {
    return FALSE;
  }

  parseRecord(clusterBuffer, record, offset);

  return TRUE;
}

int writeSector(int sector, SECTOR_T* buffer) {
  if (sector < 0 || sector > (int) constants.DISK_SECTORS) {
    return FALSE;
  }

  if (write_sector((unsigned int) sector, (unsigned char*) buffer) == 0) {
    return TRUE;
  }

  return FALSE;
}

int writeCluster(int cluster, CLUSTER_T* buffer) {
  unsigned int i, sector;

  if (cluster < 0 || cluster > (int) constants.DISK_CLUSTERS) {
    return FALSE;
  }

  sector = cluster * constants.SECTOR_PER_CLUSTER;

  for(i = 0; i < constants.SECTOR_PER_CLUSTER; i++){
    if (writeSector(sector++, (SECTOR_T*) &buffer->at[i*SECTOR_SIZE]) == FALSE) {
      return FALSE;
    }
  }

  return TRUE;
}

int writeRecord(int cluster, int index, struct t2fs_record record) {
  if(cluster < (int) constants.DATA_CLUSTER || cluster >= (int) constants.DISK_CLUSTERS || index < 0 || index >= (int) constants.RECORD_PER_CLUSTER) {
    return FALSE;
  }

  int sector_offset = index / constants.SECTOR_PER_CLUSTER;
  int record_offset = index % constants.SECTOR_PER_CLUSTER;
  int sector = cluster * constants.SECTOR_PER_CLUSTER + sector_offset;
  SECTOR_T sectorBuffer;

  if(readSector(sector, &sectorBuffer) == FALSE) {
    return FALSE;
  }

  memcpy(&sectorBuffer.at[RECORD_SIZE * record_offset], &record, RECORD_SIZE);

  writeSector(sector, &sectorBuffer);

  return TRUE;
}

int resetCluster(int cluster) {
  CLUSTER_T tempCluster;

  tempCluster.at = malloc(sizeof(unsigned char) * constants.CLUSTER_SIZE);
  memset(tempCluster.at, 0, sizeof(unsigned char) * constants.CLUSTER_SIZE);

  return writeCluster(cluster, &tempCluster);
}
