/*
  INF01142 - Sistemas Operacionais I
  Prof: Sergio Cechin
  T2FS - 2017/2

  Douglas Lazaro
  Henrique la Porta
  Rodrigo Okido
*/

#include "libs.h"

/*-----------------------------------------------------------------------------
Funcoes: parseDiskSectorSize, parseTwoBytesStructure, parseFourBytesStructure, parseVersion
  
  Parsing de buffer do superbloco para encontrar valores de cada estrutura
  no t2fs_disk.dat. Os dados no superbloco estão armazenados na t2fs_disk 
  em LITTLE_ENDIAN. Estas funções convertem os valores lidos para HIGH_ENDIAN.

Entrada:
  Buffer com valor respectivo lido do bloco de boot.

Saida:
  Retorna valor numeral convertido do elemento.

Responsável: Rodrigo Okido
-----------------------------------------------------------------------------*/
DWORD parseDiskSectorSize(unsigned char* DiskSectorSize) {
  /* 00 80 00 00 => 0x00008000 => 32768 */
  char temp[8] = "";
  int num = 0;

  sprintf(temp, "%02X%02X%02X%02X", DiskSectorSize[2], DiskSectorSize[3], DiskSectorSize[1], DiskSectorSize[0]);
  sscanf(temp, "%x", &num);

  return num;
}

DWORD parseFourBytesStructure(unsigned char* structure) {
  /* 00 00 00 08 => 0x08000000 => 134217728 */
  char temp[4] = "";

  return convertFourBytes(structure, 0, temp);
}

WORD parseTwoBytesStructure(unsigned char* structure) {
  /* 01 00 => 0x0001 => 1 */
  char temp[2] = "";

  return convertTwoBytes(structure, 0, temp);
}
/*
void parseVersion(char* version, char* str) {
  unsigned char lowNibble = version[0] & 0x0f;
  unsigned char highNibble = (version[0] >> 4) & 0x0f;

  char temp[7] = "";
  sprintf(temp, "%d-%d", (version[1] << 4) | highNibble, lowNibble);
  temp[6] = '\0';

  strcpy(str, temp);
  return;
}*/

struct t2fs_superbloco parseSuperBlock(unsigned char* superbloco) {
  struct t2fs_superbloco config;

  unsigned char version[2] = "";
  unsigned char superBlockSize[2] = "";
  unsigned char diskSize[4] = "";
  unsigned char nofSectors[4] = "";
  unsigned char sectorsPerCluster[4] = "";
  unsigned char pFatSectorStart[4] = "";
  unsigned char rootDirCluster[4] = "";
  unsigned char dataSectorStart[4] = "";

  // ID
  memcpy(config.id, superbloco, 4 * sizeof(BYTE));
  config.id[4] = '\0';

  // VERSION
  memcpy(version, superbloco + 4, 2 * sizeof(BYTE));
  config.version = parseTwoBytesStructure(version);
  // parseVersion(version, config.version);

  // SUPER CLUSTER SIZE
  memcpy(superBlockSize, superbloco + 6, 2 * sizeof(BYTE));
  config.SuperBlockSize = parseTwoBytesStructure(superBlockSize);

  // DISK SIZE
  memcpy(diskSize, superbloco + 8, 4 * sizeof(BYTE));
  config.DiskSize = parseFourBytesStructure(diskSize);

  // NOF SECTORS
  memcpy(nofSectors, superbloco + 12, 4 * sizeof(BYTE));
  config.NofSectors = parseFourBytesStructure(nofSectors);
  
  // SECTORS PER CLUSTER
  memcpy(sectorsPerCluster, superbloco + 16, 4 * sizeof(BYTE));
  config.SectorsPerCluster = parseFourBytesStructure(sectorsPerCluster);
  
  // PFAT SECTOR START
  memcpy(pFatSectorStart, superbloco + 20, 4 * sizeof(BYTE));
  config.pFATSectorStart = parseFourBytesStructure(pFatSectorStart);
  
  // ROOT DIR CLUSTER
  memcpy(rootDirCluster, superbloco + 24, 4 * sizeof(BYTE));
  config.RootDirCluster = parseFourBytesStructure(rootDirCluster);
  
  // DATA SECTOR START
  memcpy(dataSectorStart, superbloco + 28, 4 * sizeof(BYTE));
  config.DataSectorStart = parseFourBytesStructure(dataSectorStart);

  return config;
}

int parseRecord(CLUSTER_T clusterBuffer, struct t2fs_record * record, int offset) {
  char str[2];

  record->TypeVal = clusterBuffer.at[RECORD_TYPE + offset];
  memcpy(record->name, &clusterBuffer.at[RECORD_NAME + offset], MAX_FILE_NAME_SIZE * sizeof(char));
  // record->clustersFileSize = convertFourBytes(clusterBuffer.at, RECORD_BLOCK_FILE_SIZE + offset, str);
  record->bytesFileSize = convertFourBytes(clusterBuffer.at, RECORD_BYTES_FILE_SIZE + offset, str);
  // record->MFTNumber = convertFourBytes(clusterBuffer.at, RECORD_MFT_NUMBER + offset, str);
  /* FAT */

  return TRUE;
}

int parseDirectory(CLUSTER_T cluster, struct t2fs_record* records) {
  unsigned int i;
  unsigned int offset;

  for (i = 0; i < constants.RECORD_PER_CLUSTER; i++) {
    offset = (i * RECORD_SIZE);

    parseRecord(cluster, &records[i], offset);
  }

  return TRUE;
}
