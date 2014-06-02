CC=g++
CFLAGS=-O2 -g `pkg-config --cflags opencv`
LDFLAGS=`pkg-config --libs opencv`
BIN=main.out

main.out : *cpp
	$(CC) $(CFLAGS) -o main.out *cpp $(LDFLAGS)
clean :
	rm -rf *.o *.out


