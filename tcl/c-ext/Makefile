TCLDIR = /opt/ActiveTcl-8.6
TCLINCDIR = $(TCLDIR)/include
TCLLIBDIR = $(TCLDIR)/lib
TCLLIB = $(TCLLIBDIR)/libtcl8.6.so

all: example.so

example.so : example.o
	g++ -shared -o example.so example.o $(TCLLIB)

example.o : example.cpp
	g++ -fPIC -I$(TCLINCDIR) example.cpp -c -o example.o
