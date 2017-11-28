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

    /* superbloco infos */
    unsigned int SUPERBLOCK_SIZE; //tamanho do superbloco em setores
    unsigned int SECTOR_PER_CLUSTER; //quantidade de setores por cluster

    unsigned int FAT_SECTOR; //primeiro setor de FAT
    unsigned int DATA_SECTOR; //setor onde começa a area de dados
    unsigned int SUPERBLOCK_CLUSTER; //número do cluster do root
    unsigned int DISK_SECTORS; //quantidade de setores
    
    /* infos calculadas */
    unsigned int DISK_CLUSTERS; //quantidade de clusters no disco

    unsigned int FAT_SECTORS; //quantidade de setores da área de FAT
    unsigned int DATA_SECTORS; //quantidade de setores da área de Dados
    
    unsigned int ROOT_SECTOR; //setor onde começa o cluster do root

    unsigned int CLUSTER_SIZE; //tamanho em bytes de um cluster
    unsigned int RECORD_PER_CLUSTER; //quantidade de records de um cluster

    unsigned int DATA_CLUSTER; //clusters reservados
    unsigned int FAT_CLUSTER; //clusters 1 reservados
  };


  /*
    Estrutura de configuração do T2FS,
    com valores necessários para execução do sistema.
  */
  struct Configuration {
    int initiated;

    struct t2fs_superbloco superbloco;

	/*
		O indexFAT talvez possa ser iniciado diretamente
		
		unsigned int indexFAT[DISK_CLUSTERS];
		
		Caso utilizar isto retirar o malloc do initConfig()
	*/

    //Lista de clusters representados na FAT
    DWORD* indexFAT;

    struct descritor LDAA[MAX_FILES_OPEN];
    
    DIR2 currentPath;
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
  } CLUSTER_T; //cluster


#endif
