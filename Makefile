MPICC=mpic++
CC=g++
CFLAGS=-O2 -g `pkg-config --cflags opencv`
LDFLAGS=`pkg-config --libs opencv`
LIBS=-L./ -lsimulation

main: simu
	$(MPICC) $(CFLAGS) main.cpp $(LIBS) $(LDFLAGS)
run: main 
	mpiexec -n 16 a.out
simu : *.o *.so
	$(CC) $(CFLAGS) -c -fPIC simulation/*.cpp $(LDFLAGS)
	$(CC) $(CFLAGS) -shared *.o -o libsimulation.so $(LDFLAGS)
clean :
	rm -rf *.o *.out *.so

