/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/2

*/

#include "libs.h"

int getCurrentDirectory(char *pathname, int size){
	
	if(pathname[0] == '/')	//pathname é absoluto não necssita mais nada
		return TRUE;

	int tipo;
	// int indexDir = -1;
	int relativo_sem_ponto_e_barra = 0;
	int relativo_um_ponto_e_barra = 1;
	int relativo_dois_ponto_e_barra = 2;
	
	char absolutDir[MAX_FILE_NAME_SIZE * MAX_FILES_OPEN];
	
	//Define o tipo do se "arq" ou "./arq" ou "../arq"
	if(pathname[0] == '.'){
		if(pathname[1] == '.' && pathname[2] == '/')
			tipo = relativo_dois_ponto_e_barra;
		else if(pathname[1] == '/')
			tipo = relativo_dois_ponto_e_barra;
		else
			tipo = -1;
	}else
		tipo = relativo_sem_ponto_e_barra;
	
	//Diretório não é valido
	if(tipo == -1) return FALSE;
	
	//Caso caminho relativo exemplo: "arquivo"
	if(tipo == relativo_sem_ponto_e_barra){
		strcpy(absolutDir, config.currentPath);
		strcat(absolutDir, pathname);
		strcpy(pathname, absolutDir);
	}
	
	//Caso caminho relativo exemplo: "./arquivo"
	else if(tipo == relativo_um_ponto_e_barra){
		strcpy(absolutDir, config.currentPath);

		// Concatena a o caminho relativo com o absoluto do diretório
		char * token = strtok(pathname, "/");
	  	token = strtok(NULL, "/");
		while (token != NULL){
			
			strcat(absolutDir, token);
			strcat(absolutDir, "/");
			token = strtok(NULL, "/");
		}
		strcpy(pathname, absolutDir);
	}
	
	//Caso caminho relativo exemplo: "../arquivo"
	else if(tipo == relativo_dois_ponto_e_barra){
		strcpy(absolutDir, config.currentPath);
	
		// tratamento para voltar um diretório acima
		char ** parsedPath = malloc(sizeof(char) * MAX_FILE_NAME_SIZE);
		int parseCount = parsePath(absolutDir, parsedPath);
		int i;
		while (i < parseCount -1){
			strcat(absolutDir, "/");
			strcat(absolutDir, parsedPath[i]);
			i++;
		}
		
		// Concatena a o caminho relativo com o absoluto do diretório
		char * token = strtok(pathname, "/");
	  	token = strtok(NULL, "/");
		while (token != NULL){
			strcat(absolutDir, token);
			strcat(absolutDir, "/");
			token = strtok(NULL, "/");
		}
		
		strcpy(pathname, absolutDir);
	}
	
	return TRUE;
}
