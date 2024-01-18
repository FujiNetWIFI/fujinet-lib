CC=cmoc
AR=lwar

libdw.a: dwread.o dwwrite.o
	$(AR) --create libdw.a dwread.o dwwrite.o

dwread.o: dwread.c dw.h
	$(CC) -o dwread.o -c dwread.c

dwwrite.o: dwwrite.c dw.h
	$(CC) -o dwwrite.o -c dwwrite.c

clean:
	$(RM) *.o *.a *~
