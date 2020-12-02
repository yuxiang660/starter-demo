all: foo bar

foo: foo.o
	gcc -shared -o libfoo.so foo.o

bar: bar.o
	gcc -shared -o bar bar.o

%.o: %.cpp
	gcc -c -Wall -Werror -fpic -c $< -o $@

.PHONY: all foo bar

clean:
	@rm -rf libfoo.so foo.o bar bar.o
