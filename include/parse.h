/*
  INF01142 - Sistemas Operacionais I
  Prof: Sergio Cechin
  T2FS - 2017/2

  Douglas Lazaro
  Henrique la Porta
  Rodrigo Okido

  Funções de parsing de elementos em outras estruturas.
*/

#ifndef __parse__
#define	__parse__
  #include "declarations.h"

    /*-----------------------------------------------------------------------------
  Função: parseSuperBlock
    Parsing do superbloco de boot para preenchimento da estrutura superbloco.
  Entrada:
    Buffer com valor lido diretamente do disco, no bloco de boot.
  Saida:
    Retorna estrutura convertida do superbloco.
  Responsável: Rodrigo Okido
  -----------------------------------------------------------------------------*/
  struct t2fs_superbloco parseSuperBlock(unsigned char* superbloco);


  /*-----------------------------------------------------------------------------
  Função: parseRecord
    Parsing de um buffer de bloco e conversão para um t2fs_record.

  Entrada:
    buffer: valor lido diretamente de um bloco, do bloco de dados.
    record: endereço onde será armazenado o descritor t2fs_record do arquivo requisitado.
    offset: offset do endereço inicial do buffer até o descritor requisitado.

  Saida:
    Retorna o descritor t2fs_record do arquivo.

  Responsável: Douglas Lázaro
  -----------------------------------------------------------------------------*/
  int parseRecord(CLUSTER_T clusterBuffer, struct t2fs_record * record, int offset);

  /*-----------------------------------------------------------------------------
  Função: parseDirectory
    Parsing de um buffer de bloco e conversão para lista de t2fs_records.

  Entrada:
    buffer: valor lido diretamente de um bloco, do bloco de dados.
    records: endereço onde será armazenado a lista de descritores t2fs_record dos arquivos.

  Saida:
    Retorna o bloco convertido em múltiplos t2fs_record.

  Responsável: Douglas Lázaro
  -----------------------------------------------------------------------------*/
  int parseDirectory(CLUSTER_T cluster, struct t2fs_record* records);

#endif
