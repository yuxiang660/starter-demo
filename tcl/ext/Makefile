PROGRAM_NAME = main

CPPFLAGS = -I. -I/opt/ActiveTcl-8.6/include
LDFLAGS = -L/opt/ActiveTcl-8.6/lib
CC=/usr/bin/gcc

#CCDEBUGFLAG = -g


all:
	$(CC) -c $(CCDEBUGFLAG) -o tclext.o $(CPPFLAGS) $(CFLAGS) tclext.c
	$(CC) $(CCDEBUGFLAG) -o $(PROGRAM_NAME) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) main.c tclext.o -ltclstub8.6 -ltcl8.6

run:
	@export LD_LIBRARY_PATH=/opt/ActiveTcl-8.6/lib && ./$(PROGRAM_NAME)

clean:
	@rm -rf $(PROGRAM_NAME) tclext.o