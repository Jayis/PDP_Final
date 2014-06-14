CC=g++
CFLAGS=-O2 -g `pkg-config --cflags opencv`
LDFLAGS=`pkg-config --libs opencv`
BIN=main.out

main.out : 
	$(CC) $(CFLAGS) -o main.out *.cpp $(LDFLAGS)
run : 
	$(CC) $(CFLAGS) -o main.out *.cpp $(LDFLAGS)
	./main.out
clean :
	rm -rf *.o *.out

