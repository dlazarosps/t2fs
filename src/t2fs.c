/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/2

Douglas Lázaro
*/

#include "libs.h"

struct Configuration config = { .initiated = FALSE };
struct Constants constants;

int identify2 (char *name, int size) {
  if (size == 0) {
    name[0] = '\0';

    return 0;
  }

  char info[] = "\n Douglas Lazaro S P Silva \t- 207305 \n Henrique de Souza La Porta \t- 273112 \n Rodrigo Okido \t 252745 \n";
  int length = strlen(info);

  if(size < length) {
    memcpy(name, info, size);
    name[size] = '\0';
  } else {
    memcpy(name, info, length);
    name[length] = '\0';
  }

  return 0;
};

FILE2 create2 (char *filename) {
  if (!config.initiated) {
    initConfig();
  }

  return createNewFile(filename, TYPEVAL_REGULAR);
};

int delete2 (char *filename) {
  if (!config.initiated) {
    initConfig();
  }

  struct t2fs_record file;
  int return_value = lookup(filename, &file);

  int handle, check;

  switch (return_value) {
    case PARSED_PATH_ERROR:
      printf("Path '%s' inválida.\n", filename);
      break;
    default:
      if(return_value >= 0) {
        // Remover da lista de arquivos abertos
        handle = findByNameLDAA(filename);
        if(handle >= 0) {
          removeLDAA(handle);
        }

        // Remover arquivo do disco
        check = deleteFileFromDisk(file, filename);
        if(check < 0) {
          return_value = check;
        } else {
          return_value = 0;
        }
      }
      break;
  }

  return return_value;
};

FILE2 open2 (char *filename) {
  if (!config.initiated) {
    initConfig();
  }

  return openFile(filename);
};

int close2 (FILE2 handle) {
  if (!config.initiated) {
    initConfig();
  }

  return closeFile(handle, TYPEVAL_REGULAR);
};

int read2 (FILE2 handle, char *buffer, int size) {
  if (!config.initiated) {
    initConfig();
  }

  int check, return_value;
  struct descritor descritor;

  check = searchLDAA(handle, TYPEVAL_REGULAR, &descritor);
  if(check == FALSE) {
    return_value = -1;
  } else {
    return_value = readFile(handle, descritor, buffer, size);
  }

  return return_value;
};

int write2 (FILE2 handle, char *buffer, int size) {
  if (!config.initiated) {
    initConfig();
  }

  int check, return_value;
  struct descritor descritor;

  check = searchLDAA(handle, TYPEVAL_REGULAR, &descritor);
  if(check == FALSE) {
    return_value = -1;
  } else {
    return_value = writeFile(handle, descritor, buffer, size);
  }

  return return_value;
};

int truncate2 (FILE2 handle) {
  if (!config.initiated) {
    initConfig();
  }

  int check, return_value;
  struct descritor descritor;
  check = searchLDAA(handle, TYPEVAL_REGULAR, &descritor);
  if(check == FALSE) {
    return_value = -1;
  } else {
    return_value = truncateFile(handle, descritor);
  }

  return return_value;
};

int seek2 (FILE2 handle, DWORD offset) {
  if (!config.initiated) {
    initConfig();
  }
  struct descritor descritor;
  if(searchLDAA(handle, TYPEVAL_REGULAR, &descritor) == TRUE) {
    if (offset == (unsigned int) EOF_OFFSET || offset > descritor.record.bytesFileSize)
      descritor.currentPointer = descritor.record.bytesFileSize;
    else
      descritor.currentPointer = offset;

  return updateLDAA(handle, TYPEVAL_REGULAR, descritor);
  }
  else
    return NOT_FOUND_LDAA;
};

int mkdir2 (char *pathname) {
  if (!config.initiated) {
    initConfig();
  }

  if(createNewFile(pathname, TYPEVAL_DIRETORIO) >= 0) {
    return 0;
  }
  return -1;
};

int rmdir2 (char *pathname) {
  if (!config.initiated) {
    initConfig();
  }

  if(delete2(pathname) == 0) {
    return 0;
  }
  return -1;
};

int chdir2 (char *pathname){
  if (!config.initiated) {
    initConfig();
  }
  DIR2 newCurrentPath;
  
  int check = getcwd2(pathname, sizeof(pathname));
  if(check!=TRUE) return -1;
  
  newCurrentPath = opendir2(pathname);
  if(newCurrentPath < 0) return -1;

  return 0;  // faltava retorno da função ????

};

int getcwd2 (char *pathname, int size){
  if (!config.initiated) {
    initConfig();
  }
  
  return getCurrentDirectory(pathname, size);
};

DIR2 opendir2 (char *pathname) {
  if (!config.initiated) {
    initConfig();
  }
	int aux = openFile(pathname);
	if(aux < 0 ) return -1;

  char absolutDir[MAX_FILE_NAME_SIZE * MAX_FILES_OPEN];

  strcpy(absolutDir, pathname);
	
	getCurrentDirectory(absolutDir, 0);

  strcpy(config.currentPath, absolutDir);

  return aux;
};

int readdir2 (DIR2 handle, DIRENT2 *dentry) {
  if (!config.initiated) {
    initConfig();
  }

  int check, return_value;
  struct descritor descritorDiretorio;
  check = searchLDAA(handle, TYPEVAL_DIRETORIO, &descritorDiretorio);
  if(check == FALSE) {
    return_value = -1;
  } else {
    return_value = readDirectory(handle, descritorDiretorio, dentry);
  }

  return return_value;
}

int closedir2 (DIR2 handle) {
  if (!config.initiated) {
    initConfig();
  }

  return closeFile(handle, TYPEVAL_DIRETORIO);
};