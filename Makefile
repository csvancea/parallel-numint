CC          = gcc
CFLAGS      = -Wall -Wextra -march=native -O0 -Iinclude/
LDFLAGS     = -lm

OMP_CC      = gcc
OMP_CFLAGS  = -fopenmp
OMP_LDFLAGS = -fopenmp

MPI_CC      = mpicc
MPI_CFLAGS  =
MPI_LDFLAGS =


all: bin/serial bin/omp bin/mpi bin/mpi_omp

# Executables
bin/serial: bin/obj/serial.o bin/obj/main.o bin/obj/utils.o bin/obj/math_funcs.o
	$(CC) -o $@ $^ $(LDFLAGS)
bin/omp: bin/obj/omp.o bin/obj/main.o bin/obj/utils.o bin/obj/math_funcs.o
	$(OMP_CC) -o $@ $^ $(LDFLAGS) $(OMP_LDFLAGS)
bin/mpi: bin/obj/mpi.o bin/obj/main.o bin/obj/utils.o bin/obj/math_funcs.o
	$(MPI_CC) -o $@ $^ $(LDFLAGS) $(MPI_LDFLAGS)
bin/mpi_omp: bin/obj/mpi_omp.o bin/obj/main.o bin/obj/utils.o bin/obj/math_funcs.o
	$(MPI_CC) -o $@ $^ $(LDFLAGS) $(MPI_LDFLAGS) $(OMP_LDFLAGS)

# Serial objs
bin/obj/serial.o: src/impl/serial.c
	$(CC) -o $@ -c $^ $(CFLAGS)

# OpenMP objs
bin/obj/omp.o: src/impl/omp.c
	$(OMP_CC) -o $@ -c $^ $(CFLAGS) $(OMP_CFLAGS)

# MPI objs
bin/obj/mpi.o: src/impl/mpi.c
	$(MPI_CC) -o $@ -c $^ $(CFLAGS) $(MPI_CFLAGS)

# MPI + OpenMP objs
bin/obj/mpi_omp.o: src/impl/mpi_omp.c
	$(MPI_CC) -o $@ -c $^ $(CFLAGS) $(MPI_CFLAGS) $(OMP_CFLAGS)

# General objs
bin/obj/main.o: src/main.c
	$(CC) -o $@ -c $^ $(CFLAGS)
bin/obj/utils.o: src/utils.c
	$(CC) -o $@ -c $^ $(CFLAGS)
bin/obj/math_funcs.o: src/math_funcs.c
	$(CC) -o $@ -c $^ $(CFLAGS)

clean:
	rm -f bin/serial bin/omp bin/mpi bin/mpi_omp bin/obj/*.o
