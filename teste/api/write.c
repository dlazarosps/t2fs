/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Testes dos métodos de API: write

  Desenvolvido por Douglas Lázaro
*/

#include <time.h>

#include "libs.h"

#define IDENTIFY_SIZE 80

int read(int handle, int size, char* buffer) {
  if(handle < 0) {
    printf("%d: Erro abrindo arquivo.\n", handle);
    return -1;
  }

  int check = read2(handle, buffer, size);
  if(check < 0) {
    printf("%d: Erro lendo arquivo.\n", check);
    return -1;
  }

  return check;
}

int write(int handle, int size, char* buffer) {
  if(handle < 0) {
    printf("%d: Erro abrindo arquivo.\n", handle);
    return -1;
  }

  int check = write2(handle, buffer, size);
  if(check < 0) {
    printf("%d: Erro escrevendo arquivo.\n", check);
    return -1;
  }

  return check;
}

char *randstring(int length) {
    char *string = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    size_t stringLen = 26*2+10;
    char *randomString;

    randomString = malloc(sizeof(char) * (length +1));

    if (!randomString) {
        return (char*)0;
    }

    unsigned int key = 0;
    int n;
    for (n = 0;n < length;n++) {
        key = rand() % stringLen;
        randomString[n] = string[key];
    }

    randomString[length] = '\0';

    return randomString;
}

void readAndPrint(int handle, char* path, int size) {
  int offset = 0, check;
  char *newBuffer = malloc(sizeof(char) * size);
  strcpy(newBuffer, "");
  seek2(handle, 0);
  do {
    check = read(handle, 32, &newBuffer[offset]);
    if(check > 0) {
      offset += check;
    }
  } while(check != 0);

  newBuffer[offset] = '\0';
  printf("\nArquivo '%s' lido, com tamanho de '%d' bytes:\n", path, offset);
  printf("Pressione ENTER para imprimir o arquivo."); getchar();
  printf("%s", newBuffer);
}

void test_openFileAndWrite(char* path) {
  int handle = open2(path);
  seek2(handle, -1); // Ponteiro para o final do arquivo

  write(handle, 10, randstring(10));
  write(handle, 10, randstring(10));
  write(handle, 10, randstring(10));

  readAndPrint(handle, path, 30);

  close2(handle);
}

void test_writeRoot(char* path, int size, char* buffer) {
  int check;

  int handle = open2(path);
  seek2(handle, -1);

  check = write(handle, strlen(buffer), buffer);

  char newBuffer[size];
  seek2(handle, 0);
  check = read(handle, size, newBuffer);
  if(check > 0) {
    printf("\nImprimindo arquivo '%s', de '%d' bytes:\n", path, check);
    puts(newBuffer);
  }

  close2(handle);
}

void test_writeCluster(char* path) {
  int handle = open2(path);

  int i = 0, loops = 16, bytesPerWrite = 64;
  int size = loops * bytesPerWrite;
  for (i = 0; i < loops; i++) {
    write(handle, bytesPerWrite, randstring(bytesPerWrite));
  }

  readAndPrint(handle, path, size);

  printf("\n\n");
  CLUSTER_T cluster;
  cluster.at = malloc(sizeof(char) * constants.CLUSTER_SIZE);

  readCluster(2052, &cluster);
  printCluster(cluster.at);

  close2(handle);
}

void test_contiguousBlocks(char* path) {
  int handle = open2(path);

  int i = 0, loops = 40, bytesPerWrite = 64;
  int size = loops * bytesPerWrite;
  for (i = 0; i < loops; i++) {
    write(handle, bytesPerWrite, randstring(bytesPerWrite));
  }

  readAndPrint(handle, path, size);

  printf("\n\n");
  CLUSTER_T cluster1, cluster2, cluster3;
  cluster1.at = malloc(sizeof(unsigned char) * constants.CLUSTER_SIZE);
  cluster2.at = malloc(sizeof(unsigned char) * constants.CLUSTER_SIZE);
  cluster3.at = malloc(sizeof(unsigned char) * constants.CLUSTER_SIZE);

  /*
  readCluster(2052, &cluster1);
  readCluster(2053, &cluster2);
  readCluster(2054, &cluster3);
  printCluster(cluster1.at);
  printCluster(cluster2.at);
  printCluster(cluster3.at);
  */

  free(cluster1.at);
  free(cluster2.at);
  free(cluster3.at);

  close2(handle);
}

void test_contiguousBlocksThenMap(char* path, char* newFileName) {
  int handle = open2(path);

  int i = 0, loops = 40, bytesPerWrite = 64;
  int size = loops * bytesPerWrite;

  char *string = malloc(sizeof(char) * bytesPerWrite);
  sprintf(string, "$$%s##", randstring(bytesPerWrite - 4)); // String aleatório, com elementos indicando inicio e fim, para testes.

  for (i = 0; i < loops; i++) {
    write(handle, bytesPerWrite, string);
  }

  int handle2 = create2(newFileName); // Para ocupar um bloco e forçar map

  // Escreve novamente, e deverá indicar que não é possível alocar contiguo, e criará outra tupla
  for (i = 0; i < loops; i++) {
    write(handle, bytesPerWrite, string);
  }

  readAndPrint(handle, path, size*2);

  printf("\n\n");
  CLUSTER_T cluster1, cluster2, cluster3, cluster4, cluster5, cluster6;
  cluster1.at = malloc(sizeof(unsigned char) * constants.CLUSTER_SIZE);
  cluster2.at = malloc(sizeof(unsigned char) * constants.CLUSTER_SIZE);
  cluster3.at = malloc(sizeof(unsigned char) * constants.CLUSTER_SIZE);
  cluster4.at = malloc(sizeof(unsigned char) * constants.CLUSTER_SIZE);
  cluster5.at = malloc(sizeof(unsigned char) * constants.CLUSTER_SIZE);
  cluster6.at = malloc(sizeof(unsigned char) * constants.CLUSTER_SIZE);

  /*
  readCluster(2052, &cluster1);
  readCluster(2053, &cluster2);
  readCluster(2054, &cluster3);

  readCluster(2058, &cluster1);
  readCluster(2059, &cluster2);
  readCluster(2060, &cluster3);

  printCluster(cluster1.at);
  printCluster(cluster2.at);
  printCluster(cluster3.at);
  printCluster(cluster4.at);
  printCluster(cluster5.at);
  printCluster(cluster6.at);
  */

  free(cluster1.at);
  free(cluster2.at);
  free(cluster3.at);
  free(cluster4.at);
  free(cluster5.at);
  free(cluster6.at);

  close2(handle);
  close2(handle2);
}

void test_contiguousBlocksBlocksThenMapThenAditional(char* path) {
  int handle = open2(path);

  int bytesPerWrite = 1024, amountOfIterations = 40;
  int size = bytesPerWrite;

  char *string = malloc(sizeof(char) * bytesPerWrite);
  sprintf(string, "$$%s##", randstring(bytesPerWrite - 4)); // String aleatório, com elementos indicando inicio e fim, para testes.

  char *tempFileName = malloc(sizeof(char) * 20);

  int j = 0;
  for (j = 0; j < amountOfIterations; j++) {
    write(handle, bytesPerWrite, string);

    free(tempFileName);
    tempFileName = malloc(sizeof(char) * 20);
    sprintf(tempFileName, "/file_%s", randstring(5));
    create2(tempFileName); // Para ocupar um bloco e forçar map
  }

  readAndPrint(handle, path, size*amountOfIterations);
}

void test_write2() {
  /* WRITE FILE */
  printf("\n--- WRITE2 ---\n");
  char buffer[10];
  strcpy(buffer, "0123456789");

  //test_writeRoot("/file1", 100, buffer);

  //test_openFileAndWrite("/file2");

  //test_writeCluster("/file2");

  test_contiguousBlocks("/file2");

  //test_contiguousBlocksThenMap("/file2", "/file3");

  //test_contiguousBlocksBlocksThenMapThenAditional("/file2");

  printf("\n--- ENCERRANDO WRITE2 ---\n");
  return;
}

int main() {
  char identify[IDENTIFY_SIZE];

  identify2(identify, IDENTIFY_SIZE);
  printf("%s\n\n", identify);
  srand(time(NULL));
  initConfig();

  test_write2();

  return 0;
}
