TEST_linkflags = `pkg-config --libs check`

.PHONY : all

all: unit_test exhaustive_test

.PHONY : unit_test

unit_test : build/test
	@build/test

.PHONY : exhaustive_test

exhaustive_test: build/exhaustive
	@build/exhaustive > exhaustive.txt
	@git diff exhaustive.txt

build/test : test.c roman.c
	@mkdir -p build
	@gcc -std=c99 test.c roman.c -o build/test $(TEST_linkflags)

build/exhaustive: exhaustive.c roman.c
	@mkdir -p build
	@gcc -std=c99 exhaustive.c roman.c -o build/exhaustive $(TEST_linkflags)

.PHONY : clean

clean :
	@rm -rf build/
