CC=g++
CFLAGS=-Wall -Wextra -pedantic
DEBUG_FLAGS=-ggdb
RELEASE_FLAGS=-O3
OUT=-o out/raymond
SRC=src/main.cpp
LIB=-lm

debug:
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(OUT) $(SRC) $(LIB)

release:
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) $(OUT) $(SRC) $(LIB)
