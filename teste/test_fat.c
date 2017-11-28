#include "libs.h"

void test_fat() {
	initConfig();
	int status;
  
	printFAT(0, 20);
  
	printf("---TEST getFAT(0)---\n indexFAT[0] = %d\n\n",getFAT(0));
	printf("---TEST getFAT(2)---\n indexFAT[2] = %d\n\n",getFAT(2));

  
	status = setFAT(2, 10);
	printf("---TEST setFAT(2, 10)---\n indexFAT[2] = %d\n\n",getFAT(2));
	
	printf("---TEST setFAT(0, 10)---\n\n");
	status = setFAT(0, 10);
	
	printFAT(0, 2);
	status = searchFAT(0);
	printf("---TEST searchFAT(0)---\n indexFAT[%d] = %d\n\n",status, getFAT(status));
  
}

int main(int argc, char const *argv[]) {
  /* INIT configuration structure */
  test_fat();


  return 0;
}
