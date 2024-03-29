/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/2

  Testes dos métodos do módulo LDAA

  Desenvolvido por Douglas Lázaro
*/

#include "libs.h"

void test_findByNameValidate(char* name) {
  int handle = findByNameLDAA(name);

  if(handle < 0) {
    printf("Arquivo '%s' não encontrado na LDAA.\n", name);
  } else {
    printf("Arquivo '%s' encontrado, com handle '%d'.\n", name, handle);

    printf("\tRemovendo '%s' e efetuando busca novamente.\n", name);
    removeLDAA(handle);

    handle = findByNameLDAA(name);
    if(handle < 0) {
      printf("\tArquivo '%s' não encontrado, como esperado.\n", name);
    } else {
      printf("\tArquivo '%s' não removido corretamente.\n", name);
    }
  }
}

void test_findByName() {
  char path0[30] = "/directory0/filename0";
  char path1[30] = "/directory1/filename1";
  char path2[30] = "/directory2/filename2";
  char path3[30] = "/directory3/filename3";
  char path4[30] = "/directory4/filename4";

  struct t2fs_record record0, record1, record2, record3, record4;
  record0 = initRecord(TYPEVAL_REGULAR, path0, 0);
  record1 = initRecord(TYPEVAL_REGULAR, path1, 0);
  record2 = initRecord(TYPEVAL_DIRETORIO, path2, 0);
  record3 = initRecord(TYPEVAL_REGULAR, path3, 0);
  record4 = initRecord(TYPEVAL_REGULAR, path4, 0);

  // Inserindo na LDAA
  insertLDAA(record0, path0);
  insertLDAA(record1, path1);
  insertLDAA(record2, path2);

  // Pesquisando na LDAA
  test_findByNameValidate(path0);
  test_findByNameValidate(path2);
  test_findByNameValidate(path4);

  test_findByNameValidate(path1);
  test_findByNameValidate(path3);
}

void test_insertSearch() {
  int i = getFreeLDAA();

  printf("Primeiro LDAA livre :: %d \n", i);

  printf("\t Inserindo no LDAA \n");

  struct t2fs_record record = {1, "Teste", 0, 2};

  if(insertLDAA(record, "Teste") != -1)
  printf("Inserido com sucesso \n");
  else
    printf("Erro ao inserir \n");

  printf("\t Procurando no LDAA \n");

  struct descritor result;
  if(searchLDAA(i, 1, &result) == TRUE){
    printf("Encontrado Handle %d \n", i);

    printf("\t Printando: \n");
    printf("typeVal = %d \n name = %s \n firstCluster = %d \n", result.record.TypeVal, result.record.name, result.record.firstCluster);

    printf("\t Original: \n");
    printf("typeVal = %d \n name = %s \n firstCluster = %d \n", config.LDAA[i].record.TypeVal, config.LDAA[i].record.name, config.LDAA[i].record.firstCluster);
  }
  else
  printf("Erro ao encontrar \n");
}

void test_insertUpdate() {
  int i = getFreeLDAA();
  struct t2fs_record record = {1, "Teste", 0, 3};


  printf("Inserindo no LDAA \n");
  if(insertLDAA(record, "Teste") != -1)
    printf("\t Inserido com sucesso \n");
  else
    printf("\t Erro ao inserir \n");

  printf("\t Original: \n");
  printf("typeVal = %d \n name = %s \n firstCluster = %d \n", config.LDAA[i].record.TypeVal, config.LDAA[i].record.name, config.LDAA[i].record.firstCluster);
  
  struct descritor update = {1, 5, "Teste", {1, "TesteEDIT", 1, 10}};
  printf("UPDATE no LDAA \n");

  if(updateLDAA(i, 1, update) != -1){
    printf("\t Atualizado: \n");
    printf("typeVal = %d \n name = %s \n firstCluster = %d \n", config.LDAA[i].record.TypeVal, config.LDAA[i].record.name, config.LDAA[i].record.firstCluster); 
  }
  else
    printf("\t Erro ao atualizar \n");

}

int main(int argc, char const *argv[]) {
  initConfig();

  printf(" ---------- TESTE test_insertSearch ---------- \n\n");

  test_insertSearch();

  printf("\n\n ---------- TESTE test_findByName ---------- \n\n");

  test_findByName();

  printf("\n\n ---------- TESTE test_insertUpdate ---------- \n\n");

  test_insertUpdate();

  return 0;
}
