CC = gcc

BASEFLAGS = -Wall -Wextra -fopenmp
NODEBUG_FLAGS = -dNDEBUG 
DEBUG_FLAGS = -g

LDLIBS = -lm

OBJS = NearestNeighborUpscale.o lodepng.o NearestNeighborUpscaleDriver.o

EXE = NearestNeighborUpscale

debug: CFLAGS = $(BASEFLAGS) $(DEBUG_FLAGS)
debug: $(EXE)

release: CFLAGS = $(BASEFLAGS) $(NODEBUG_FLAGS) 
release: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(EXE) $(LDLIBS)

NearestNeighborUpscale.o: NearestNeighborUpscale.c NearestNeighborUpscale.h
	$(CC) $(CFLAGS) -c NearestNeighborUpscale.c

NearestNeighborUpscaleDriver.o: NearestNeighborUpscaleDriver.c NearestNeighborUpscaleDriver.h
	$(CC) $(CFLAGS) -c NearestNeighborUpscaleDriver.c

lodepng.o: LODEPNG/lodepng.c LODEPNG/lodepng.h
	$(CC) $(CFLAGS) -c LODEPNG/lodepng.c

clean:
	-rm -f $(OBJS)
	-rm -f *~
	-rm -f $(EXE)
	-rm -f $(EXE)_d