/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Douglas Lazaro
  Francisco Knebel

  Funções para leitura, escrita e reset de estruturas do disco.
*/

#ifndef __disk__
#define	__disk__
  #include "declarations.h"

  /*-----------------------------------------------------------------------------
  Função: readSector
    Leitura de um setor e armazenamento em um buffer.

  Entrada:
    sector: número do setor que será lido.
    buffer: endereço onde o buffer do setor lido será armazenado.

  Saida:
    Retorna TRUE se o setor foi lido corretamente.
    Retorna FALSE se houve um problema de leitura ou se o número de setor é inválido.

  Responsável: Francisco Knebel
  -----------------------------------------------------------------------------*/
  int readSector(int sector, SECTOR_T* buffer);

  /*-----------------------------------------------------------------------------
  Função: readBlock
    Leitura de um bloco e armazenamento em um buffer.

  Entrada:
    block: número do block que será lido.
    buffer: endereço onde o buffer do block lido será armazenado.

  Saida:
    Retorna TRUE se o bloco foi lido corretamente.
    Retorna FALSE se houve um problema de leitura ou se o número do bloco é inválido.

  Responsável: Francisco Knebel
  -----------------------------------------------------------------------------*/
  int readBlock(int block, BLOCK_T* buffer);

  /*-----------------------------------------------------------------------------
  Função: readBootBlock
    Leitura especial do bloco de boot. É a primeira leitura efetuada do T2FS,
    e será usado para definir os tamanhos dos elementos do disco, utilizados
    em outros métodos.

  Entrada:
    buffer: endereço onde o buffer do bloco de boot será armazenado.

  Saida:
    Retorna TRUE se o bloco foi lido corretamente.
    Retorna FALSE se houve um problema de leitura.

  Responsável: Francisco Knebel
  -----------------------------------------------------------------------------*/
  int readBootBlock(SECTOR_T* buffer);

  
  /*-----------------------------------------------------------------------------
  Função: readRecord
    Leitura de um descritor de arquivo.

  Entrada:
    block: bloco, de DATA_CLUSTER até DISK_CLUSTERS, que será lido.
    index: indice do descritor dentro do bloco de diretório.
    record: endereço onde será armazenado o descritor lido.

  Saida:
    Retorna FALSE se houve algum erro de leitura ou valor de entrada inválido.
    Retorna TRUE se o descritor foi lido e armazenado corretamente.


  Responsável: Francisco Knebel
  -----------------------------------------------------------------------------*/
  int readRecord(int block, int index, struct t2fs_record * record);

  /*-----------------------------------------------------------------------------
  Função: writeSector
    Escrita de um setor no disco.

  Entrada:
    sector: número do setor que será escrito.
    buffer: endereço do buffer de setor que será usado para a escrita.

  Saida:
    Retorna TRUE se o setor foi escrito corretamente.
    Retorna FALSE se houve um problema de escrita ou se o número de setor é inválido.

  Responsável: Francisco Knebel
  -----------------------------------------------------------------------------*/
  int writeSector(int sector, SECTOR_T* buffer);

  /*-----------------------------------------------------------------------------
  Função: writeBlock
    Escrita de um bloco no disco.

  Entrada:
    block: número do block que será escrito.
    buffer: endereço do buffer de bloco que será usado para a escrita.

  Saida:
    Retorna TRUE se o bloco foi escrito corretamente.
    Retorna FALSE se houve um problema de escrita ou se o número do bloco é inválido.

  Responsável: Francisco Knebel
  -----------------------------------------------------------------------------*/
  int writeBlock(int block, BLOCK_T* buffer);


  /*-----------------------------------------------------------------------------
  Função: writeRecord
    Escrita de um descritor de arquivo.

  Entrada:
    block: bloco, de DATA_CLUSTER até DISK_CLUSTERS, que será lido.
    index: indice do descritor dentro do bloco de diretório.
    record: descritor que será armazenado no disco.

  Saida:
    Retorna FALSE se houve algum erro de escrita ou valor de entrada inválido.
    Retorna TRUE se o descritor foi escrito corretamente.


  Responsável: Francisco Knebel
  -----------------------------------------------------------------------------*/
  int writeRecord(int block, int index, struct t2fs_record record);


  /*-----------------------------------------------------------------------------
  Função: resetBlock
    Reset de um bloco no disco com 0's.

  Entrada:
    block: indice do bloco que deve ser resetado.

  Saida:
    Retorna TRUE se o bloco foi escrito corretamente.
    Retorna FALSE se houve um problema de escrita ou se o número do bloco é inválido.

  Responsável: Francisco Knebel
  -----------------------------------------------------------------------------*/
  int resetBlock(int block);


#endif
