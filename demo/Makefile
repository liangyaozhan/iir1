
prefix = $(PWD)/ins
prefix?=/usr/local

all: iirdemo

iirdemo: iirdemo.o wave_signal.o
	g++ -o  iirdemo iirdemo.o -Wl,-rpath,$(prefix)/lib -L$(prefix)/lib -liir -lrt wave_signal.o

iirdemo.o: iirdemo.cpp
	g++ -I. -g -O0 -I$(prefix)/include -c iirdemo.cpp

wave_signal.o:wave-signal.c wave-signal.h
	gcc -I. -g -O0 -c wave-signal.c -o $@

clean:
	rm -f *.dat *~ *.o iirdemo
