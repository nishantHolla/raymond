CC=g++
CFLAGS=-Wall -Wextra -pedantic -fopenmp
DEBUG_FLAGS=-ggdb -fsanitize=address
RELEASE_FLAGS=-O3
OUT=-o out/raymond
SRC=src/main.cpp
LIB=-lm
ARGS=

debug:
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(OUT) $(SRC) $(LIB)

release:
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) $(OUT) $(SRC) $(LIB)
