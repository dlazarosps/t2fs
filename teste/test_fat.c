#include "libs.h"

int test_fat() {
	initConfig();
	int status;
  
	printFAT(0, 15);
  
	status = getFAT(0);
	if(status != FAT_ERROR){ printf("\nERRO getFAT(0)\n"); return -1; }
	printf("---TEST getFAT(0)---\n indexFAT[0] = %d\n\n",status);
	
	status = getFAT(2);
	if(status != FAT_EOF){ printf("\nERRO getFAT(2)\n"); return -1; }
	printf("---TEST getFAT(2)---\n indexFAT[2] = %d\n\n",status);
  
	status = setFAT(2, 10);
	status = getFAT(2);
	if(status != 10){ printf("\nERRO setFAT(10)\n"); return -1; }
	printf("---TEST setFAT(2, 10)---\n indexFAT[2] = %d\n\n",status);
	
	printf("---TEST setFAT(0, 10)---\n\n");
	status = setFAT(0, 10);
	if(status >= 0){ printf("\nERRO setFAT(0,10)\n"); return -1; }
	
	printFAT(0, 2);
	status = searchFAT(0);
	int r = getFAT(status);
	if(r != 0){ printf("\nERRO searchFAT(0)\n"); return -1; }
	printf("---TEST searchFAT(0)---\n indexFAT[%d] = %d\n\n",status, r);
  
	return 0;
}

int main(int argc, char const *argv[]) {
  /* INIT configuration structure */
	if(test_fat() != 0){
		printf("---Terminou com ERRO\n\n");
	}else
		printf("---Terminou com SUCESSO\n\n");

  return 0;
}
