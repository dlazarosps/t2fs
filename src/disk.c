/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Douglas Lazaro
  Francisco Knebel
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

int readBlock(int block, BLOCK_T* buffer) {
  unsigned int i, sector;

  if (block < 0 || block > (int) constants.DISK_CLUSTERS) {
    return FALSE;
  }

  sector = block * constants.SECTOR_PER_CLUSTER;

  for(i = 0; i < constants.SECTOR_PER_CLUSTER; i++) {
    if (readSector(sector, (SECTOR_T*) &buffer->at[i * SECTOR_SIZE]) == FALSE) {
      return FALSE;
    }

    sector++;
  }

  return TRUE;
}

int readBootBlock(SECTOR_T* buffer) {
  if (read_sector(SUPERBLOCK, (unsigned char*) buffer) == 0) {
    return TRUE;
  }

  return FALSE;
}

int readRecord(int block, int index, struct t2fs_record * record) {
  if(block < (int) constants.DATA_CLUSTER || block >= (int) constants.DISK_CLUSTERS || index < 0 || index >= (int) constants.RECORD_PER_CLUSTER) {
    return FALSE;
  }

  int offset = (index * RECORD_SIZE);
  BLOCK_T blockBuffer;
  blockBuffer.at = malloc(sizeof(unsigned char) * constants.CLUSTER_SIZE);

  if (readBlock(block, &blockBuffer) == FALSE) {
    return FALSE;
  }

  parseRecord(blockBuffer, record, offset);

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

int writeBlock(int block, BLOCK_T* buffer) {
  unsigned int i, sector;

  if (block < 0 || block > (int) constants.DISK_CLUSTERS) {
    return FALSE;
  }

  sector = block * constants.SECTOR_PER_CLUSTER;

  for(i = 0; i < constants.SECTOR_PER_CLUSTER; i++){
    if (writeSector(sector++, (SECTOR_T*) &buffer->at[i*SECTOR_SIZE]) == FALSE) {
      return FALSE;
    }
  }

  return TRUE;
}

int writeRecord(int block, int index, struct t2fs_record record) {
  if(block < (int) constants.DATA_CLUSTER || block >= (int) constants.DISK_CLUSTERS || index < 0 || index >= (int) constants.RECORD_PER_CLUSTER) {
    return FALSE;
  }

  int sector_offset = index / constants.SECTOR_PER_CLUSTER;
  int record_offset = index % constants.SECTOR_PER_CLUSTER;
  int sector = block * constants.SECTOR_PER_CLUSTER + sector_offset;
  SECTOR_T sectorBuffer;

  if(readSector(sector, &sectorBuffer) == FALSE) {
    return FALSE;
  }

  memcpy(&sectorBuffer.at[RECORD_SIZE * record_offset], &record, RECORD_SIZE);

  writeSector(sector, &sectorBuffer);

  return TRUE;
}

int resetBlock(int block) {
  BLOCK_T tempBlock;

  tempBlock.at = malloc(sizeof(unsigned char) * constants.CLUSTER_SIZE);
  memset(tempBlock.at, 0, sizeof(unsigned char) * constants.CLUSTER_SIZE);

  return writeBlock(block, &tempBlock);
}
