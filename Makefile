TEST_linkflags = `pkg-config --libs check`

.PHONY : all

all: hellotest testtest

.PHONY : testtest

testtest : build/test
	@build/test

build/test : test.c
	@mkdir -p build
	@gcc test.c -o build/test $(TEST_linkflags)

.PHONY : hellotest

hellotest : build/hello
	@build/hello

build/hello : hello.c
	@mkdir -p build
	@gcc hello.c -o build/hello

.PHONY : clean

clean :
	@rm -rf build/


