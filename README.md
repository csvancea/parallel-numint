# Parallel Numerical Integration - Cosmin-Răzvan VANCEA - 343C1

Algoritmul folosit pentru integrare numerică este [Composite Simpson's Rule](https://en.wikipedia.org/wiki/Simpson%27s_rule#Composite_Simpson's_rule).

## Implementări

1. Serială (pentru validarea rezultatelor)
2. OpenMP
3. MPI
4. pthread
5. MPI + OpenMP
6. MPI + pthread **(BONUS)**

Toate implementările se găsesc în directorul [src/impl/](src/impl/).

## Compilare

`make` va genera în directorul `bin/` câte un executabil pentru fiecare
dintre cele 6 implementări

## Rulare

Programele generate se execută din linia de comanda și așteaptă următorii parametri:

- `math-func-name`: numele funcției matematice care va fi integrată. Funcțiile disponibile
sunt definite în fișierul sursă [src/math_funcs.c](src/math_funcs.c).
- `a`: limita inferioară a integralei definite
- `b`: limita superioară a integralei definite
- `n`: numărul de subintervale folosite în calcularea integralei

De exemplu, pentru a integra funcția `f(x) = x` pe intervalul de integrare [0, 1], folosind
100 de subintervale și implementarea paralelă cu pthread, se va executa următoarea comandă:

```
$ ./bin/pthread identity 0 1 100
```

### Detalii specifice implementărilor

#### OpenMP

Pentru a specifica numărul de thread-uri folosite în calculul paralel, se va folosi variabila
de mediu `OMP_NUM_THREADS`. Exemplu pentru 4 thread-uri:

```
$ OMP_NUM_THREADS=4 ./bin/omp loge 1 1.5 1000
```

#### MPI

Programele MPI trebuie rulate prin executabilul `mpirun`. Parametrul `-np` determină numărul
de procese MPI folosite:

```
$ mpirun -np 4 ./bin/mpi loge 1 1.5 1000
```

#### pthread

Pentru a specifica numărul de thread-uri folosite în calculul paralel, se va folosi variabila
de mediu `NUMINT_NUM_PROCS`. Dacă nu se specifică o valoare anume, se vor folosi toate core-urile
disponibile. Exemplu pentru 4 thread-uri:

```
$ NUMINT_NUM_PROCS=4 ./bin/pthread loge 1 1.5 1000
```

#### MPI + OpenMP

Fiecarui proces MPI îi revine un număr egal de puncte ce trebuie calculate. Mai departe, fiecare
proces MPI va lansa T thread-uri folosind OpenMP și va împărți punctele sale acestor thread-uri.

Pentru a specifica numărul de procese MPI se folosește parametrul `-np` al lui `mpirun`, iar
numărul de thread-uri lansate de fiecare proces în parte se specifică folosind variabila de
mediu `OMP_NUM_THREADS`.

În următorul exemplu vor lansate 4 procese MPI, iar fiecare proces va lansa câte 8 thread-uri:

```
$ OMP_NUM_THREADS=8 mpirun -np 4 ./bin/mpi_omp loge 1 1.5 1000
```

#### MPI + pthread

Implementare asemănătoare cu cea de mai sus, diferențele fiind reprezentate de gestionarea
thread-urilor și de împărțirea manuală a punctelor pe thread-uri, ambele realizate acum explicit
de către programator.

În următorul exemplu vor lansate 4 procese MPI, iar fiecare proces va lansa câte 8 thread-uri
(a se observa folosirea variabilei `NUMINT_NUM_PROCS` spre deosebire de mai sus):

```
$ NUMINT_NUM_PROCS=8 mpirun -np 4 ./bin/mpi_pthread loge 1 1.5 1000
```
