CC := gcc
INC_DIRS := -I"./inc"
CC_FLAGS := -std=c11 -Wall -ggdb $(INC_DIRS)

BIN_DIR := ./bin
LIB_DIR := ./lib
OBJ_DIR := ./obj
SRC_DIR := ./src
TST_DIR := ./tst
DOC_DIR := ./doc
CFG_DIR := ./cfg

OBJ_FILES := $(patsubst lib/%.c, %.o, $(wildcard lib/*.c))

all: $(OBJ_FILES)

prog.out: $(SRC_DIR)/prog.c bst.o
	$(CC) $(CC_FLAGS) -o $(BIN_DIR)/$@ $+

extractor.out: $(SRC_DIR)/extractor.c
	$(CC) $(CC_FLAGS) -o $(BIN_DIR)/$@ $+

test_wav.out: $(SRC_DIR)/test_wav.c $(OBJ_DIR)/wav.o
	$(CC) $(CC_FLAGS) -o $(BIN_DIR)/$@ $+



$(OBJ_DIR)/%.o: $(LIB_DIR)/%.c
	$(CC) $(CC_FLAGS) -o $(OBJ_DIR)/$(@F) $< -c

llist.o: $(OBJ_DIR)/llist.o
bst.o: $(OBJ_DIR)/bst.o
wav.o: $(OBJ_DIR)/wav.o

$(BIN_DIR)/llist.tst: $(TST_DIR)/llist_test.c $(OBJ_DIR)/llist.o
	$(CC) $(CC_FLAGS) -o $(BIN_DIR)/$@ $+ -lcunit

$(BIN_DIR)/bst.tst: $(TST_DIR)/bst_test.c $(OBJ_DIR)/bst.o
	$(CC) $(CC_FLAGS) -o $(BIN_DIR)/$@ $+ -lcunit

$(DOC_DIR)/%.doc: $(CFG_DIR)/%.cfg
	doxygen $< > /dev/null

clean:
	rm -rf ./bin/* ./obj/*.o ./doc/*
