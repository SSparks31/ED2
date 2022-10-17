CC = gcc
CFLAGS = -std=c99 -fstack-protector-all -Wall

ted: main.c list.o srbTree.o svg.o
	$(CC) $(CFLAGS) -g -o $@ $^ -lm

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<
	
PHONY: clean tedtest maintest

test: testmain.c srbTree.o list.o
	gcc -o testmain -g testmain.c srbTree.o list.o -lm
	./testmain

tedtest: ted
	./ted -e ../testes-t2 -o ../Output -f esquadra-2-alet-25.geo -q esquadra-2-alet-25/003-bombs.qry 

clean:
	-rm -rf *.o
	-rm -f ted
	-rm -rf ../Output/*.*