.PHONY: all main run

all: main

main: main.cpp
	gcc -Wall -o $@ $< -L./shared_lib -lfoo ./shared_lib/bar

run:
	@export LD_LIBRARY_PATH=./shared_lib && ./main

clean:
	@rm -rf main
