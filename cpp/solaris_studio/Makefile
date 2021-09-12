all:
	gcc -g main.c -lpthread

clean:
	@rm -rf a.out test.*.er

collector:
	/home/eugene/rd/tools/solstudio/bin/collect ./a.out

analyzer:
	/home/eugene/rd/tools/solstudio/bin/analyzer ./test.1.er
