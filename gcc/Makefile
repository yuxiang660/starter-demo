all: main

main: main.cpp
	gcc -Wall -o $@ $< ./shared_lib/libfoo.so ./shared_lib/bar

.PHONY: all main

clean:
	@rm -rf main
