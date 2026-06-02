CC      = gcc
CFLAGS  = -std=c99 -Wall -Wextra -Wpedantic -Wconversion -O3 -g
IFLAGS  = -Iinclude
SANFLAGS = -fsanitize=address,undefined

SRC     = src/params.c src/modular.c src/sampler.c src/keygen.c src/kem.c src/kdf.c
OBJ     = $(SRC:src/%.c=build/%.o)

.PHONY: all clean

all: build/test_kem

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

build/test_kem: tests/test_kem.c $(OBJ) | build
	$(CC) $(CFLAGS) $(IFLAGS) $(SANFLAGS) $^ -o $@ -lm -lcrypto

build:
	mkdir -p build

clean:
	rm -rf build/