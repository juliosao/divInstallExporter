.PHONY: clean
CFLAGS=-std=gnu99 -g
LIBS=-lz
CFLAGS+=-I ./inc

bin/divInstallExporter: obj/main.o obj/div.o obj/divArchiver.o | bin
	gcc $(LIBS) obj/main.o obj/div.o obj/divArchiver.o -o bin/divInstallExporter

obj/main.o: src/main.c | obj
	gcc src/main.c $(CFLAGS) -c -o obj/main.o

obj/div.o: src/div.c | obj
	gcc src/div.c $(CFLAGS) -c -o obj/div.o

obj/divArchiver.o: src/divArchiver.c | obj
	gcc src/divArchiver.c $(CFLAGS) -c -o obj/divArchiver.o

bin:
	mkdir bin

obj:
	mkdir obj

lib:
	mkdir lib

Debug: bin/divInstallExporter

clean:
	-rm -r obj bin lib
	-find . -name '*~' -delete
