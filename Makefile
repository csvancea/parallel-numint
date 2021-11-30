CC          = gcc
CFLAGS      = -Wall -Wextra -march=native -O0 -Iinclude/
LDFLAGS     = -lm

OMP_CC      = gcc
OMP_CFLAGS  = -fopenmp $(CFLAGS)
OMP_LDFLAGS = -fopenmp $(LDFLAGS)

MPI_CC      = mpicc
MPI_CFLAGS  = $(CFLAGS)
MPI_LDFLAGS = $(LDFLAGS)


all: bin/serial bin/omp bin/mpi

# Executables
bin/serial: bin/obj/serial.o bin/obj/main.o bin/obj/math_funcs.o
	$(CC) -o $@ $^ $(LDFLAGS)
bin/omp: bin/obj/omp.o bin/obj/main.o bin/obj/math_funcs.o
	$(OMP_CC) -o $@ $^ $(OMP_LDFLAGS)
bin/mpi: bin/obj/mpi.o bin/obj/main.o bin/obj/math_funcs.o
	$(MPI_CC) -o $@ $^ $(MPI_LDFLAGS)

# Serial objs
bin/obj/serial.o: src/impl/serial.c
	$(CC) -o $@ -c $^ $(CFLAGS)

# OpenMP objs
bin/obj/omp.o: src/impl/omp.c
	$(OMP_CC) -o $@ -c $^ $(OMP_CFLAGS)

# MPI objs
bin/obj/mpi.o: src/impl/mpi.c
	$(MPI_CC) -o $@ -c $^ $(MPI_CFLAGS)

# General objs
bin/obj/main.o: src/main.c
	$(CC) -o $@ -c $^ $(CFLAGS)
bin/obj/math_funcs.o: src/math_funcs.c
	$(CC) -o $@ -c $^ $(CFLAGS)

clean:
	rm -f bin/serial bin/obj/*.o
