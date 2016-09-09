TEST_linkflags = `pkg-config --libs check`

.PHONY : all

all: testtest

.PHONY : testtest

testtest : build/test
	@build/test

build/test : test.c roman.c
	@mkdir -p build
	@gcc -std=c99 test.c roman.c -o build/test $(TEST_linkflags)

build/exhaustive: exhaustive.c roman.c
	@mkdir -p build
	@gcc -std=c99 exhaustive.c roman.c -o build/exhaustive $(TEST_linkflags)

.PHONY : clean

clean :
	@rm -rf build/
