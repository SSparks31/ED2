CC = gcc
CFLAGS = -std=c99 -fstack-protector-all -Wall

ted: main.c data.o geo.o qry.o list.o queue.o srbTree.o svg.o
	$(CC) $(CFLAGS) -g -o $@ $^ -lm

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<
	
.PHONY: clean test testmain

test: ted
	./ted -e ../Testes -f 01-mar-pequeno-1par-naus.geo -o ../Output -q 01-mar-pequeno-1par-naus/01-mv1-001-cmr.qry

testmain: testmain.c srbTree.o list.o
	$(CC) $(CFLAGS) -g -o $@ $^ -lm
	./testmain

clean:
	-rm -rf *.o
	-rm -f ted
	-rm -rf ../Output/*.*