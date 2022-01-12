#include <mpi.h>
#include <omp.h>
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


static void numint_impl(int rank, int nproc, onedim_func_t f, double a, double h, unsigned n, double *local_sums)
{
    /* there are (n - 1) inner points which will be equally spread among the processes. */
    unsigned start =     ((rank + 0ULL) * (n - 1) / nproc + 1   );
    unsigned end   = _MIN((rank + 1ULL) * (n - 1) / nproc + 1, n);

    double sum_odds  = 0.0;
    double sum_evens = 0.0;

    /* create a parallel scope */
    #pragma omp parallel shared(a, h, n)
    {
        /*
         * the first for computes the sum for odd numbers, and the second does it for even numbers
         *
         * reduction is used to sum up the results
         *
         * nowait is used so threads won't have to wait until all the other threads
         * finish computing their odd numbers sum
         *
         *
         * i must be an odd integer here.
         * This bit trickery sets i to the first odd integer greater or equal to start.
         */

        #pragma omp for reduction(+:sum_odds) nowait
        for (unsigned i = start | 1U; i < end; i += 2)
        {
            sum_odds += f(fma(i, h, a));
        }

        /*
         * i must be an even integer here.
         * This bit trickery sets i to the first even integer greater or equal to start.
         */

        #pragma omp for reduction(+:sum_evens)
        for (unsigned i = (start + 1) & ~1U; i < end; i += 2)
        {
            sum_evens += f(fma(i, h, a));
        }
    }

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
    numint_impl(rank, nproc, f, a, h, n, local_sums);
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
