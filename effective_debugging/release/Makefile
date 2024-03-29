CC=g++
RELEASE_OPTIONS=-O3 -Wall -DNDEBUG
DEBUG_OPTIONS=-O3 -g
SRCS=main.c

.PHONY: clean main main_d sym smy_d

all: clean sym sym_d main main_d

# https://sourceware.org/gdb/current/onlinedocs/gdb.html/Separate-Debug-Files.html
# https://stackoverflow.com/questions/30281766/need-to-load-debugging-symbols-for-shared-library-in-gdb

sym:
	$(CC) -fPIC -shared $(RELEASE_OPTIONS) -c mm_symbol.c -o mm_symbol.o
	$(CC) -fPIC -shared mm_symbol.o -o libsym.so

sym_d:
	$(CC) -fPIC -shared $(DEBUG_OPTIONS) -c mm_symbol.c -o mm_symbol_d.o
	$(CC) -fPIC -shared mm_symbol_d.o -o libsym_d.so
	objcopy --only-keep-debug libsym_d.so libsym.sym

main:
	$(CC) $(RELEASE_OPTIONS) -c main.c -o main.o
	$(CC) main.o -L. -lsym -o main -Wl,-rpath=.

main_d:
	$(CC) $(DEBUG_OPTIONS) -c main.c -o main_d.o
	$(CC) main_d.o -L. -lsym -o main_d -Wl,-rpath=.
	objcopy --only-keep-debug main_d main.sym

clean:
	@rm -rf main *.o *.i *.s *.so *.sym main_d

debug:
	gdb -q ./main \
	-ex 'run' \
	-ex 'ptype FreeBlock' \
	-ex 'p g_blocks' \
	-ex 'add-symbol-file mm_symbol_d.o' \
	-ex 'ptype FreeBlock' \
	-ex 'p g_blocks'

