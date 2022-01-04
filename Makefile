CC = gcc

BASEFLAGS = -Wall -Wextra -fopenmp
NODEBUG_FLAGS = -dNDEBUG 
DEBUG_FLAGS = -g

LDLIBS = -lm

OBJS = NearestNeighbourUpscale.o lodepng.o NearestNeighbourUpscaleDriver.o

EXE = NearestNeighbourUpscale

debug: CFLAGS = $(BASEFLAGS) $(DEBUG_FLAGS)
debug: $(EXE)

release: CFLAGS = $(BASEFLAGS) $(NODEBUG_FLAGS) 
release: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(EXE) $(LDLIBS)

NearestNeighbourUpscale.o: NearestNeighbourUpscale.c NearestNeighbourUpscale.h
	$(CC) $(CFLAGS) -c NearestNeighbourUpscale.c

NearestNeighbourUpscaleDriver.o: NearestNeighbourUpscaleDriver.c NearestNeighbourUpscaleDriver.h
	$(CC) $(CFLAGS) -c NearestNeighbourUpscaleDriver.c

lodepng.o: LODEPNG/lodepng.c LODEPNG/lodepng.h
	$(CC) $(CFLAGS) -c LODEPNG/lodepng.c

clean:
	-rm -f $(OBJS)
	-rm -f *~
	-rm -f $(EXE)
	-rm -f $(EXE)_d
