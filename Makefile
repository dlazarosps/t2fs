#
# Makefile ESQUELETO
#
# DEVE ter uma regra "all" para geração da biblioteca
# regra "clean" para remover todos os objetos gerados.
#
# NECESSARIO adaptar este esqueleto de makefile para suas necessidades.
#
# 
CC=gcc
CEXTRA=-Wextra
CFLAGS=-Wall -g
LIB_DIR=./lib
INC_DIR=./include
BIN_DIR=./bin
SRC_DIR=./src
TST_DIR=./teste

# GERAÇÂO DO T2FS #
FILES_OBJECTS=$(LIB_DIR)/files/create.o $(LIB_DIR)/files/delete.o $(LIB_DIR)/files/open.o $(LIB_DIR)/files/close.o $(LIB_DIR)/files/read.o $(LIB_DIR)/files/write.o $(LIB_DIR)/files/truncate.o $(LIB_DIR)/files/readdir.o $(LIB_DIR)/files/getcwd.o $(LIB_DIR)/files/chdir.o
HELPER_OBJECTS=$(LIB_DIR)/helpers/print.o $(LIB_DIR)/helpers/util.o $(LIB_DIR)/helpers/files.o $(LIB_DIR)/helpers/ldaa.o $(LIB_DIR)/helpers/fat.o
SRC_OBJECTS=$(LIB_DIR)/t2fs.o $(LIB_DIR)/disk.o $(LIB_DIR)/parse.o

LIB_GENERATED_OBJECTS=$(SRC_OBJECTS) $(FILES_OBJECTS) $(HELPER_OBJECTS)
LIB_OBJECTS=$(LIB_GENERATED_OBJECTS) $(LIB_DIR)/apidisk.o

all: $(LIB_OBJECTS)
	ar rcs $(LIB_DIR)/libt2fs.a $(LIB_OBJECTS)

$(LIB_DIR)/t2fs.o: $(SRC_DIR)/t2fs.c
	$(CC) $(CFLAGS) -c -o $(LIB_DIR)/t2fs.o -I$(INC_DIR) $(SRC_DIR)/t2fs.c

$(LIB_DIR)/disk.o: $(SRC_DIR)/disk.c
	$(CC) $(CFLAGS) -c -o $(LIB_DIR)/disk.o -I$(INC_DIR) $(SRC_DIR)/disk.c

$(LIB_DIR)/parse.o: $(SRC_DIR)/parse.c
	$(CC) $(CFLAGS) -c -o $(LIB_DIR)/parse.o -I$(INC_DIR) $(SRC_DIR)/parse.c

## FILES ##
$(LIB_DIR)/files/create.o: $(SRC_DIR)/files/create.c
	$(CC) $(CFLAGS) -c -o $(LIB_DIR)/files/create.o -I$(INC_DIR) $(SRC_DIR)/files/create.c

$(LIB_DIR)/files/delete.o: $(SRC_DIR)/files/delete.c
	$(CC) $(CFLAGS) -c -o $(LIB_DIR)/files/delete.o -I$(INC_DIR) $(SRC_DIR)/files/delete.c

$(LIB_DIR)/files/open.o: $(SRC_DIR)/files/open.c
	$(CC) $(CFLAGS) -c -o $(LIB_DIR)/files/open.o -I$(INC_DIR) $(SRC_DIR)/files/open.c

$(LIB_DIR)/files/close.o: $(SRC_DIR)/files/close.c
	$(CC) $(CFLAGS) -c -o $(LIB_DIR)/files/close.o -I$(INC_DIR) $(SRC_DIR)/files/close.c

$(LIB_DIR)/files/read.o: $(SRC_DIR)/files/read.c
	$(CC) $(CFLAGS) -c -o $(LIB_DIR)/files/read.o -I$(INC_DIR) $(SRC_DIR)/files/read.c

$(LIB_DIR)/files/write.o: $(SRC_DIR)/files/write.c
	$(CC) $(CFLAGS) -c -o $(LIB_DIR)/files/write.o -I$(INC_DIR) $(SRC_DIR)/files/write.c

$(LIB_DIR)/files/truncate.o: $(SRC_DIR)/files/truncate.c
	$(CC) $(CFLAGS) -c -o $(LIB_DIR)/files/truncate.o -I$(INC_DIR) $(SRC_DIR)/files/truncate.c

$(LIB_DIR)/files/readdir.o: $(SRC_DIR)/files/readdir.c
	$(CC) $(CFLAGS) -c -o $(LIB_DIR)/files/readdir.o -I$(INC_DIR) $(SRC_DIR)/files/readdir.c

$(LIB_DIR)/files/chdir.o: $(SRC_DIR)/files/chdir.c
	$(CC) $(CFLAGS) -c -o $(LIB_DIR)/files/chdir.o -I$(INC_DIR) $(SRC_DIR)/files/chdir.c

$(LIB_DIR)/files/getcwd.o: $(SRC_DIR)/files/getcwd.c
	$(CC) $(CFLAGS) -c -o $(LIB_DIR)/files/getcwd.o -I$(INC_DIR) $(SRC_DIR)/files/getcwd.c

## HELPERS ##
$(LIB_DIR)/helpers/print.o: $(SRC_DIR)/helpers/print.c
	$(CC) $(CFLAGS) -c -o $(LIB_DIR)/helpers/print.o -I$(INC_DIR) $(SRC_DIR)/helpers/print.c

$(LIB_DIR)/helpers/util.o: $(SRC_DIR)/helpers/util.c
	$(CC) $(CFLAGS) -c -o $(LIB_DIR)/helpers/util.o -I$(INC_DIR) $(SRC_DIR)/helpers/util.c

$(LIB_DIR)/helpers/files.o: $(SRC_DIR)/helpers/files.c
	$(CC) $(CFLAGS) -c -o $(LIB_DIR)/helpers/files.o -I$(INC_DIR) $(SRC_DIR)/helpers/files.c

$(LIB_DIR)/helpers/ldaa.o: $(SRC_DIR)/helpers/ldaa.c
	$(CC) $(CFLAGS) -c -o $(LIB_DIR)/helpers/ldaa.o -I$(INC_DIR) $(SRC_DIR)/helpers/ldaa.c

$(LIB_DIR)/helpers/fat.o: $(SRC_DIR)/helpers/fat.c
	$(CC) $(CFLAGS) -c -o $(LIB_DIR)/helpers/fat.o -I$(INC_DIR) $(SRC_DIR)/helpers/fat.c

# COMANDOS DE TESTE #
## código de avaliação testing program
avaliacao: all $(TST_DIR)/avaliacao.c
	$(CC) $(CFLAGS) -o $(BIN_DIR)/avaliacao $(TST_DIR)/avaliacao.c -L$(LIB_DIR) -lt2fs -I$(INC_DIR)
	$(BIN_DIR)/avaliacao

## teste de setores simplificado
main: $(TST_DIR)/main.c
	$(CC) -o $(BIN_DIR)/main $(TST_DIR)/main.c lib/apidisk.o $(CFLAGS)
	$(BIN_DIR)/main

## arquivos de teste
ifeq (teste,$(firstword $(MAKECMDGOALS)))
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(RUN_ARGS):;@:)
endif

TEST_FILES=$(BIN_DIR)/test_display $(BIN_DIR)/test_disk $(BIN_DIR)/test_parse $(BIN_DIR)/test_util  $(BIN_DIR)/test_files $(BIN_DIR)/test_ldaa $(BIN_DIR)/test_fat
teste: $(TEST_FILES)
	$(BIN_DIR)/$(RUN_ARGS)

$(BIN_DIR)/test_display: all $(TST_DIR)/test_display.c
	$(CC) $(CFLAGS) -o $(BIN_DIR)/test_display $(TST_DIR)/test_display.c -L$(LIB_DIR) -lt2fs -I$(INC_DIR)

$(BIN_DIR)/test_disk: all $(TST_DIR)/test_disk.c
	$(CC) $(CFLAGS) -o $(BIN_DIR)/test_disk $(TST_DIR)/test_disk.c -L$(LIB_DIR) -lt2fs -I$(INC_DIR)

$(BIN_DIR)/test_parse: all $(TST_DIR)/test_parse.c
	$(CC) $(CFLAGS) -o $(BIN_DIR)/test_parse $(TST_DIR)/test_parse.c -L$(LIB_DIR) -lt2fs -I$(INC_DIR)

$(BIN_DIR)/test_util: all $(TST_DIR)/test_util.c
	$(CC) $(CFLAGS) -o $(BIN_DIR)/test_util $(TST_DIR)/test_util.c -L$(LIB_DIR) -lt2fs -I$(INC_DIR)

$(BIN_DIR)/test_files: all $(TST_DIR)/test_files.c
	$(CC) $(CFLAGS) -o $(BIN_DIR)/test_files $(TST_DIR)/test_files.c -L$(LIB_DIR) -lt2fs -I$(INC_DIR)

$(BIN_DIR)/test_ldaa: all $(TST_DIR)/test_ldaa.c
	$(CC) $(CFLAGS) -o $(BIN_DIR)/test_ldaa $(TST_DIR)/test_ldaa.c -L$(LIB_DIR) -lt2fs -I$(INC_DIR)
	
$(BIN_DIR)/test_fat: all $(TST_DIR)/test_fat.c
	$(CC) $(CFLAGS) -o $(BIN_DIR)/test_fat $(TST_DIR)/test_fat.c -L$(LIB_DIR) -lt2fs -I$(INC_DIR)

## API ##
ifeq (api,$(firstword $(MAKECMDGOALS)))
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(RUN_ARGS):;@:)
endif

API=$(BIN_DIR)/api
API_FILES=$(API)/create $(API)/openclose $(API)/delete $(API)/read $(API)/write $(API)/seek $(API)/readdir $(API)/truncate $(API)/chdir $(API)/getcwd
API_SRC=$(TST_DIR)/api

api: $(API_FILES)
	$(API)/$(RUN_ARGS)

$(API)/create: all $(API_SRC)/create.c
	$(CC) $(CFLAGS) -o $(API)/create $(API_SRC)/create.c -L$(LIB_DIR) -lt2fs -I$(INC_DIR)

$(API)/openclose: all $(API_SRC)/openclose.c
	$(CC) $(CFLAGS) -o $(API)/openclose $(API_SRC)/openclose.c -L$(LIB_DIR) -lt2fs -I$(INC_DIR)

$(API)/delete: all $(API_SRC)/delete.c
	$(CC) $(CFLAGS) -o $(API)/delete $(API_SRC)/delete.c -L$(LIB_DIR) -lt2fs -I$(INC_DIR)

$(API)/read: all $(API_SRC)/read.c
	$(CC) $(CFLAGS) -o $(API)/read $(API_SRC)/read.c -L$(LIB_DIR) -lt2fs -I$(INC_DIR)

$(API)/write: all $(API_SRC)/write.c
	$(CC) $(CFLAGS) -o $(API)/write $(API_SRC)/write.c -L$(LIB_DIR) -lt2fs -I$(INC_DIR)

$(API)/seek: all $(API_SRC)/seek.c
	$(CC) $(CFLAGS) -o $(API)/seek $(API_SRC)/seek.c -L$(LIB_DIR) -lt2fs -I$(INC_DIR)

$(API)/readdir: all $(API_SRC)/readdir.c
	$(CC) $(CFLAGS) -o $(API)/readdir $(API_SRC)/readdir.c -L$(LIB_DIR) -lt2fs -I$(INC_DIR)

$(API)/truncate: all $(API_SRC)/truncate.c
	$(CC) $(CFLAGS) -o $(API)/truncate $(API_SRC)/truncate.c -L$(LIB_DIR) -lt2fs -I$(INC_DIR)

$(API)/chdir: all $(API_SRC)/chdir.c
	$(CC) $(CFLAGS) -o $(API)/chdir $(API_SRC)/chdir.c -L$(LIB_DIR) -lt2fs -I$(INC_DIR)

$(API)/getcwd: all $(API_SRC)/getcwd.c
	$(CC) $(CFLAGS) -o $(API)/getcwd $(API_SRC)/getcwd.c -L$(LIB_DIR) -lt2fs -I$(INC_DIR)

# LIMPEZA #
clean:
	clear
	rm -rf $(LIB_DIR)/*.a $(BIN_DIR)/*.o $(SRC_DIR)/*~ $(INC_DIR)/*~ *~
	rm -rf $(LIB_GENERATED_OBJECTS)
	rm -rf $(TEST_FILES)
	rm -rf $(API_FILES)
	rm -rf $(BIN_DIR)/avaliacao
