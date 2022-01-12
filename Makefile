CC          = gcc
MPI_CC      = mpicc

CFLAGS      = -Wall -Wextra -std=gnu99 -march=native -O0 -Iinclude/
LDFLAGS     = -lm


all: bin/serial bin/omp bin/mpi bin/pthread bin/mpi_omp bin/mpi_pthread

# Executables
bin/serial: bin/obj/serial.o bin/obj/main.o bin/obj/utils.o bin/obj/math_funcs.o bin/obj/stopwatch_unix.o
	$(CC) -o $@ $^ $(LDFLAGS)
bin/omp: bin/obj/omp.o bin/obj/main.o bin/obj/utils.o bin/obj/math_funcs.o bin/obj/stopwatch_unix.o
	$(CC) -o $@ $^ $(LDFLAGS) -fopenmp
bin/mpi: bin/obj/mpi.o bin/obj/main.o bin/obj/utils.o bin/obj/math_funcs.o bin/obj/stopwatch_unix.o
	$(MPI_CC) -o $@ $^ $(LDFLAGS)
bin/pthread: bin/obj/pthread.o bin/obj/main.o bin/obj/utils.o bin/obj/math_funcs.o bin/obj/stopwatch_unix.o
	$(CC) -o $@ $^ $(LDFLAGS) -lpthread
bin/mpi_omp: bin/obj/mpi_omp.o bin/obj/main.o bin/obj/utils.o bin/obj/math_funcs.o bin/obj/stopwatch_unix.o
	$(MPI_CC) -o $@ $^ $(LDFLAGS) -fopenmp
bin/mpi_pthread: bin/obj/mpi_pthread.o bin/obj/main.o bin/obj/utils.o bin/obj/math_funcs.o bin/obj/stopwatch_unix.o
	$(MPI_CC) -o $@ $^ $(LDFLAGS) -lpthread

# Serial objs
bin/obj/serial.o: src/impl/serial.c
	$(CC) -o $@ -c $^ $(CFLAGS)

# OpenMP objs
bin/obj/omp.o: src/impl/omp.c
	$(CC) -o $@ -c $^ $(CFLAGS) -fopenmp

# MPI objs
bin/obj/mpi.o: src/impl/mpi.c
	$(MPI_CC) -o $@ -c $^ $(CFLAGS)

# pthread objs
bin/obj/pthread.o: src/impl/pthread.c
	$(CC) -o $@ -c $^ $(CFLAGS)

# MPI + OpenMP objs
bin/obj/mpi_omp.o: src/impl/mpi_omp.c
	$(MPI_CC) -o $@ -c $^ $(CFLAGS) -fopenmp

# MPI + pthread objs
bin/obj/mpi_pthread.o: src/impl/mpi_pthread.c
	$(MPI_CC) -o $@ -c $^ $(CFLAGS)

# General objs
bin/obj/main.o: src/main.c
	$(CC) -o $@ -c $^ $(CFLAGS)
bin/obj/utils.o: src/utils.c
	$(CC) -o $@ -c $^ $(CFLAGS)
bin/obj/math_funcs.o: src/math_funcs.c
	$(CC) -o $@ -c $^ $(CFLAGS)
bin/obj/stopwatch_unix.o: src/stopwatch_unix.c
	$(CC) -o $@ -c $^ $(CFLAGS)

clean:
	rm -f bin/serial bin/omp bin/mpi bin/pthread bin/mpi_omp bin/mpi_pthread bin/obj/*.o
