CC=gcc
CFLAGS=-std=c11 -Wall -ggdb -I"./inc"

all: test

test: src/test.c bin/bst.o
	$(CC) $(CFLAGS) -o $@ $+

bin/%.o: lib/%.c
	$(CC) $(CFLAGS) -o $@ $< -c

bin/%.tst: tst/%.c
	$(CC) $(CFLAGS) -o $@ $< -lcunit

clean:
	rm -f test ./bin/*
