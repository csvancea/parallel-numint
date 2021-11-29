CC          = gcc
CFLAGS      = -Wall -Wextra -Iinclude/
LDFLAGS     = -lm

OMP_CC      = gcc
OMP_CFLAGS  = $(CFLAGS)
OMP_LDFLAGS = $(LDFLAGS)

MPI_CC      = mpicc
MPI_CFLAGS  = $(CFLAGS)
MPI_LDFLAGS = $(LDFLAGS)


all: bin/serial

# Serial objs
bin/serial: bin/obj/serial.o bin/obj/main.o bin/obj/math_funcs.o
	$(CC) -o $@ $^ $(LDFLAGS)
bin/obj/serial.o: src/impl/serial.c
	$(CC) -o $@ -c $^ $(CFLAGS)

# General objs
bin/obj/main.o: src/main.c
	$(CC) -o $@ -c $^ $(CFLAGS)
bin/obj/math_funcs.o: src/math_funcs.c
	$(CC) -o $@ -c $^ $(CFLAGS)

clean:
	rm -f bin/serial bin/obj/*.o
