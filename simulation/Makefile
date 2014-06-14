CC=g++
CFLAGS=-O2 -g `pkg-config --cflags opencv`
LDFLAGS=`pkg-config --libs opencv`
BIN=main.out

main.out : *cpp *c
	$(CC) $(CFLAGS) -o main.out *.cpp *.c $(LDFLAGS)
run : *cpp *c
	$(CC) $(CFLAGS) -o main.out *.cpp *.c $(LDFLAGS)
	./main.out
clean :
	rm -rf *.o *.out

