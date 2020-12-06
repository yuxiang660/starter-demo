all: fooo

fooo: fooo.o
	ar rcs libfooo.a fooo.o

%.o: %.cpp
	gcc -c -Wall -Werror -fpic -c $< -o $@

.PHONY: all fooo

clean:
	@rm -rf libfooo.a fooo.o
