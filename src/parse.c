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

WORD parseFourBytesStructure(unsigned char* structure) {
  /* 00 00 00 08 => 0x08000000 => 134217728 */
  char temp[4] = "";

  return convertFourBytes(structure, 0, temp);
}

WORD parseTwoBytesStructure(unsigned char* structure) {
  /* 01 00 => 0x0001 => 1 */
  char temp[2] = "";

  return convertTwoBytes(structure, 0, temp);
}

void parseVersion(char* version, char* str) {
  unsigned char lowNibble = version[0] & 0x0f;
  unsigned char highNibble = (version[0] >> 4) & 0x0f;

  char temp[7] = "";
  sprintf(temp, "%d-%d", (version[1] << 4) | highNibble, lowNibble);
  temp[6] = '\0';

  strcpy(str, temp);
  return;
}

struct t2fs_superbloco parseSuperBlock(unsigned char* superbloco) {
  struct t2fs_superbloco config;

  char version[2] = "";
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
  parseVersion(version, config.version);

  // SUPER BLOCK SIZE
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




struct t2fs_4tupla parseRegister_tupla(unsigned char* buffer, int tuplaIndex) {
  struct t2fs_4tupla tupla;
  char temp[8];

  tupla.atributeType = convertFourBytes(buffer, tuplaIndex, temp);
  tupla.virtualBlockNumber = convertFourBytes(buffer, tuplaIndex + 4, temp);
  tupla.logicalBlockNumber = convertFourBytes(buffer, tuplaIndex + 8, temp);
  tupla.numberOfContiguosBlocks = convertFourBytes(buffer, tuplaIndex + 12, temp);

  return tupla;
}

int parseRegister(unsigned char* buffer, struct t2fs_4tupla * tuplas) {
  unsigned int i, offset;

  for (i = 0; i < constants.MAX_TUPLAS_REGISTER; ++i) {
    offset = constants.TUPLA_SIZE * i;

    tuplas[i] = parseRegister_tupla(&buffer[offset], 0);
  }

  return TRUE;
}



int parseRecord(BLOCK_T blockBuffer, struct t2fs_record * record, int offset) {
  char str[2];

  record->TypeVal = blockBuffer.at[RECORD_TYPE + offset];
  memcpy(record->name, &blockBuffer.at[RECORD_NAME + offset], MAX_FILE_NAME_SIZE * sizeof(char));
  record->blocksFileSize = convertFourBytes(blockBuffer.at, RECORD_BLOCK_FILE_SIZE + offset, str);
  record->bytesFileSize = convertFourBytes(blockBuffer.at, RECORD_BYTES_FILE_SIZE + offset, str);
  record->MFTNumber = convertFourBytes(blockBuffer.at, RECORD_MFT_NUMBER + offset, str);

  return TRUE;
}

int parseDirectory(BLOCK_T block, struct t2fs_record* records) {
  unsigned int i;
  unsigned int offset;

  for (i = 0; i < constants.RECORD_PER_BLOCK; i++) {
    offset = (i * RECORD_SIZE);

    parseRecord(block, &records[i], offset);
  }

  return TRUE;
}
