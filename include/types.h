/*
  INF01142 - Sistemas Operacionais I
  Prof: Sergio Cechin
  T2FS - 2017/2

  Douglas Lazaro
  Henrique la Porta
  Rodrigo Okido
*/

#ifndef __types__
#define	__types__

  #include "declarations.h"
  #include "t2fs.h"


  struct Constants {
    unsigned int SECTOR_PER_CLUSTER;
    unsigned int CLUSTER_SIZE;

    unsigned int DISK_SECTORS;
    unsigned int DISK_CLUSTERS;

    /* Partições do disco */
    unsigned int SUPERBLOCK_CLUSTER_SIZE;
    unsigned int SUPERBLOCK_SECTOR_SIZE;

    unsigned int FAT_CLUSTER_SIZE;
    unsigned int FAT_SECTOR_SIZE;

    unsigned int DATA_CLUSTER_SIZE;
    unsigned int DATA_SECTOR_SIZE;

    /* Cluster (Blocos) do disco */
    unsigned int SUPERBLOCK_CLUSTER;
    unsigned int FAT_CLUSTER;
    unsigned int DATA_CLUSTER;

    /* Setores de início do disco */
    unsigned int SUPERBLOCK_SECTOR;
    unsigned int FAT_SECTOR;
    unsigned int DATA_SECTOR;

    /* Records */
    unsigned int RECORD_PER_CLUSTER;
  };


  /*
    Estrutura de configuração do T2FS,
    com valores necessários para execução do sistema.
  */
  struct Configuration {
    int initiated;

    struct t2fs_superbloco superbloco;

    //vetor fat

    struct descritor LDAA[MAX_FILES_OPEN];
  };

  /*
    Estruturas usadas para a leitura de
    setores, blocos e registros, respectivamente.
  */
  typedef struct {
    unsigned char at[SECTOR_SIZE];
  } SECTOR_T;

  typedef struct {
    unsigned char * at;
  } BLOCK_T; //cluster


#endif
