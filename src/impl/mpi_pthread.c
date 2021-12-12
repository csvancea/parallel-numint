#include <mpi.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>
#include "numint.h"
#include "utils.h"

#define MASTER 0

/* this enum is used for interprocess MPI comms only */
enum {
    SUM_ODDS = 0,
    SUM_EVENS,
    SUM_COUNT
};

/* these structs are used to pass data/context to threads */
typedef struct _numint_global_params {
    onedim_func_t f;
    double a;
    double h;
    unsigned mpi_start;
    unsigned mpi_end;
    unsigned nthreads;
} numint_global_params;

typedef struct _numint_specific_params
{
    const numint_global_params *global;
    pthread_t handle;
    unsigned tid;
    double sum_odds;
    double sum_evens;
} numint_specific_params;


static void *numint_thread_worker(void *arg)
{
    /*
     * Let N = num of inner points, M = num of MPI nodes, T = num of available threads on each MPI node
     * Each MPI node is responsible for N/M points (at most).
     * However, each MPI node divides the work further among T threads, so each thread processes N/M/T points (at most)
     *
     * Note that some threads/MPI nodes might process fewer points if there are not enough points
     */

    numint_specific_params *params = arg;
    onedim_func_t f    = params->global->f;
    double   a         = params->global->a;
    double   h         = params->global->h;
    unsigned mpi_start = params->global->mpi_start;
    unsigned mpi_end   = params->global->mpi_end;
    unsigned nthreads  = params->global->nthreads;
    unsigned tid       = params->tid;
    unsigned n         = mpi_end - mpi_start;

    /* there are n = (mpi_end - mpi_start) inner points which will be equally spread among the threads. */
    unsigned start     = mpi_start +     ((tid + 0) * n / nthreads   );
    unsigned end       = mpi_start + _MIN((tid + 1) * n / nthreads, n);

    double sum_odds    = 0.0;
    double sum_evens   = 0.0;

    /*
     * i must be an odd integer here.
     * This bit trickery sets i to the first odd integer greater or equal to start.
     */
    for (unsigned i = start | 1U; i < end; i += 2)
    {
        sum_odds += f(fma(i, h, a));
    }

    /*
     * i must be an even integer here.
     * This bit trickery sets i to the first even integer greater or equal to start.
     */
    for (unsigned i = (start + 1) & ~1U; i < end; i += 2)
    {
        sum_evens += f(fma(i, h, a));
    }

    params->sum_odds = sum_odds;
    params->sum_evens = sum_evens;
    return NULL;
}

static void numint_mpi_worker(int rank, int nproc, onedim_func_t f, double a, double h, unsigned n, double *local_sums)
{
    /* there are (n - 1) inner points which will be equally spread among the processes. */
    unsigned start =     ((rank + 0) * (n - 1) / nproc + 1   );
    unsigned end   = _MIN((rank + 1) * (n - 1) / nproc + 1, n);

    double sum_odds  = 0.0;
    double sum_evens = 0.0;

    /* initialize global context for threads */
    unsigned nthreads = get_num_of_procs();
    numint_specific_params *params = malloc(nthreads * sizeof(numint_specific_params));
    numint_global_params global = {
        .a = a,
        .f = f,
        .h = h,
        .mpi_start = start,
        .mpi_end = end,
        .nthreads = nthreads
    };

    /* initialize specific context for each thread and create the threads */
    for (unsigned i = 0; i < nthreads; ++i) {
        params[i].global = &global;
        params[i].tid = i;
        pthread_create(&params[i].handle, NULL, numint_thread_worker, &params[i]);
    }

    /* join the threads and sum the partial-partial sums */
    for (unsigned i = 0; i < nthreads; ++i) {
        pthread_join(params[i].handle, NULL);
        sum_odds  += params[i].sum_odds;
        sum_evens += params[i].sum_evens;
    }

    free(params);

    /* local_sums vector is received by the MASTER node when MPI_Reduce is called */
    local_sums[SUM_ODDS ] = sum_odds;
    local_sums[SUM_EVENS] = sum_evens;
}

double numint(onedim_func_t f, double a, double b, unsigned n)
{
    int rank, nproc;
    double local_sums[SUM_COUNT], global_sums[SUM_COUNT];
    double h = (b - a) / n;
    double result = 0.0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    numint_mpi_worker(rank, nproc, f, a, h, n, local_sums);
    MPI_Reduce(local_sums, global_sums, SUM_COUNT, MPI_DOUBLE, MPI_SUM, MASTER, MPI_COMM_WORLD);

    if (rank == MASTER) {
        /* compute f(a), f(b) and the overall sum/result */
        result = h / 3 * (fma(2, global_sums[SUM_EVENS], f(a)) + fma(4, global_sums[SUM_ODDS], f(b)));
    }

    return result;
}

void numint_init()
{
    MPI_Init(NULL, NULL);
}

void numint_end()
{
    MPI_Finalize();
}

int numint_is_master()
{
    int rank;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    return (rank == MASTER);
}
