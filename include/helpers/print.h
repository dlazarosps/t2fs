/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/2

  Douglas Lazaro
*/

#ifndef __helperaux__
#define	__helperaux__

  /*-----------------------------------------------------------------------------
  Função: printX
    Múltiplas funções para imprimir estruturas do sistema, para debuggação.
    Cada função tem o seu comportamento definido independentemente, e deverá ser
    visto cada implementação individualmente.
  -----------------------------------------------------------------------------*/
  int printSector(unsigned char* buffer);

  int printSector2(unsigned char* buffer, int currentSector);

  int printBlock(unsigned char* buffer);

  void showBlock(int block);

  void printBootBlock();

  void printConstants();

  void printRecord(struct t2fs_record record);

  void printDescritor(struct descritor file);

#endif
