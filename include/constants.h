/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/2

Douglas Lázaro

  Declarações de constantes usadas por todo o T2FS.
*/

#ifndef __constants__
#define	__constants__

  #define FALSE 0
  #define TRUE 1

  /* Disco */
  // #define SECTOR_SIZE 256 //t2fs.h
  #define SUPERBLOCK 0
  #define FAT_ROOT 1

  // MUDAR PARA VALORES DA FAT (xEFF , xFFF)
  #define REGISTER_FIM 0
  #define REGISTER_MAP 1
  #define REGISTER_ADITIONAL 2
  #define REGISTER_FREE -1

  #define REGISTER_READ_ERROR -5

  /* Record */
  #define RECORD_SIZE 64
  #define RECORD_TYPE 0
  #define RECORD_NAME 1
  #define RECORD_BYTES_FILE_SIZE 56
  #define RECORD_FIRST_CLUSTER 60
  #define RECORD_PER_CLUSTER 16

  /* Arquivo */
  // #define MAX_FILE_NAME_SIZE 55 //t2fs.h

  /* Lookup */
  #define FIND_REGISTER_NOTFOUND -1
  #define FIND_REGISTER_FIM -2
  #define FIND_REGISTER_ADITIONAL -3
  #define FIND_REGISTER_FREE -4
  #define FILE_NOT_FOUND -1

  /* LDAA */
  #define MAX_FILES_OPEN 40
  #define NOT_FOUND_LDAA -1
  #define ERRO_REMOVE_LDAA -2


  /* FAT */
  #define FAT_SECTORS 128   //num de setores de fat
  #define FAT_LIVRE 0       //0x00000000
  #define FAT_ERROR -2      //0xFFFFFFFE bad sector
  #define FAT_EOF -1        //0xFFFFFFFF end of fat
  #define FAT_PER_SECTOR 64 //qntos vetores fat de 4 bytes cabem num setor  256 / 4 = 64 


  /* API */
  #define FOUND_FILE_ERROR -1
  #define EOF_OFFSET -1
  #define DIRECTORY_NOT_FOUND -7
  #define FILE_LIMIT_REACHED -8
  #define PARSED_PATH_ERROR -9
  #define RECORD_WRITE_ERROR -10
  #define READDIR_ERROR -11

  /* Add to Directory */
  #define ADDDIR_CONTIGUOUS 200
  #define ADDDIR_ADITIONAL 201
  #define ADDDIR_MAP 202

#endif
