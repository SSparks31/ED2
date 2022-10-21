CC = gcc
CFLAGS = -std=c99 -fstack-protector-all -Wall

ted: main.c geo.o qry.o list.o srbTree.o svg.o
	$(CC) $(CFLAGS) -g -o $@ $^ -lm

%.o: %.c %.h
	$(CC) $(CFLAGS) -g -c $<
	
.PHONY: clean test testmain

test: ted
	valgrind ./ted -e ../Testes -f 01-mar-pequeno-1par-naus.geo -o ../Output -q 01-mar-pequeno-1par-naus/mixed.qry
	valgrind ./ted -e ../Testes -f 02-mar-pequeno-3pares-naus.geo -o ../Output -q 02-mar-pequeno-3pares-naus/mixed.qry
	valgrind ./ted -e ../Testes -f 03-mar-medio.geo -o ../Output -q 03-mar-medio/mixed.qry
	valgrind ./ted -e ../Testes -f 04-mar-grande.geo -o ../Output -q 04-mar-grande/mixed.qry
	

testmain: testmain.c srbTree.o list.o geo.o qry.o data.o svg.o
	$(CC) $(CFLAGS) -g -o $@ $^ -lm
	./testmain

clean:
	-rm -rf *.o
	-rm -f ted
	-rm -rf ../Output/*.*