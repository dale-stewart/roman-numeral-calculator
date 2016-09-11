TEST_linkflags = `pkg-config --libs check`
TEST_cflags = `pkg-config --cflags check` -g -std=c99
CC = gcc

.PHONY : all

all: unit_test exhaustive_test

.PHONY : unit_test

unit_test : build/test
	@build/test

.PHONY : exhaustive_test

exhaustive_test: build/exhaustive
	@build/exhaustive > exhaustive.txt
	@git diff exhaustive.txt

build/test: build/test.o build/roman.o
	@mkdir -p build
	@$(CC) $(TEST_cflags) build/test.o build/roman.o -o build/test $(TEST_linkflags)

build/exhaustive: build/exhaustive.o build/roman.o
	@mkdir -p build
	@$(CC) $(TEST_cflags) build/exhaustive.o build/roman.o -o build/exhaustive $(TEST_linkflags)

build/test.o: test.c
	@mkdir -p build
	@$(CC) $(TEST_cflags) -c test.c -o build/test.o

build/roman.o: roman.c
	@mkdir -p build
	@$(CC) $(TEST_cflags) -c roman.c -o build/roman.o

build/exhaustive.o: exhaustive.c
	@mkdir -p build
	@$(CC) $(TEST_cflags) -c exhaustive.c -o build/exhaustive.o

.PHONY : clean

clean :
	@rm -rf build/
