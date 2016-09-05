TEST_linkflags = `pkg-config --libs check`

.PHONY : all

all: testtest

.PHONY : testtest

testtest : build/test
	@build/test

build/test : test.c roman.c
	@mkdir -p build
	@gcc test.c roman.c -o build/test $(TEST_linkflags)

.PHONY : clean

clean :
	@rm -rf build/
