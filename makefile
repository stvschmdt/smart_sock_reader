CC=gcc

default: sockreader

sockreader.o: sockreader.c sockreader.h
	$(CC) sockreader.c -o sockreader

sockreader: sockreader.o
	$(CC) sockreader.o -o sockreader

clean:
	rm sockreader.o
	rm sockreader
