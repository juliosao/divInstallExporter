.PHONY: clean
CFLAGS=-std=gnu99

bin/divInstallExporter: obj/main.o | bin
	gcc obj/main.o -o bin/divInstallExporter

obj/main.o: src/main.c | obj
	gcc src/main.c $(CFLAGS) -c -o obj/main.o

bin:
	mkdir bin

obj:
	mkdir obj

clean:
	-rm -r obj bin
	-find . -name '*~' -delete
