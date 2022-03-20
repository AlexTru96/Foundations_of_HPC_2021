CC = mpicc
EXE = binary_tree
OBJS = kdtree.o src/tools.o src/partial_sorting.o src/tree_builders.o

CFLAGS= -fopenmp -O3 -fexpensive-optimizations -ffast-math -msse3 -march=native -Iinclude -DDOUBLE_PRECISION
EXTRAFLAGS = -Wall -Wextra 
LFLAGS= -fopenmp
$(EXE): $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS)

%.o:%.c 
	$(CC) -c $< -o $@ $(CFLAGS) $(EXTRAFLAGS)

#prerequisites
kdtree.o: kdtree.c src/partial_sorting.c src/tools.c src/tree_builders.c include/tools.h
src/tools.o: include/tools.h
src/partial_sorting.c: include/tools.h
src/tree_builders.c: include/tools.h

#NOT A PHYSICAL FILE
.PHONY: clean default

#CLEAN OBJECTS AND EXECUTABLE FILE
clean:
	rm -f $(OBJS) $(EXE)


