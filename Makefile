test : hello
	@build/hello

hello : hello.c
	@mkdir build
	@gcc hello.c -o build/hello

.PHONY : clean

clean :
	@rm -rf build/


