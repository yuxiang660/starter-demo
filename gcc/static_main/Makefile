.PHONY: all main run

all: main

main: main.cpp
	g++ -Wall -o $@ $< -L./../static_lib -lfooo

run:
	@./main

clean:
	@rm -rf main
