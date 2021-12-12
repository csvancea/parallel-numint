#include <pthread.h>
#include <stdlib.h>
#include <math.h>
#include "numint.h"
#include "utils.h"

typedef struct _numint_global_params {
    onedim_func_t f;
    double a;
    double h;
    unsigned n;
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


static void *numint_impl(void *arg)
{
    numint_specific_params *params = arg;
    onedim_func_t f   = params->global->f;
    double   a        = params->global->a;
    double   h        = params->global->h;
    unsigned n        = params->global->n;
    unsigned nthreads = params->global->nthreads;
    unsigned tid      = params->tid;

    /* there are (n - 1) inner points which will be equally spread among the threads. */
    unsigned start    =     ((tid + 0) * (n - 1) / nthreads + 1   );
    unsigned end      = _MIN((tid + 1) * (n - 1) / nthreads + 1, n);

    double sum_odds   = 0.0;
    double sum_evens  = 0.0;

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

double numint(onedim_func_t f, double a, double b, unsigned n)
{
    double h = (b - a) / n;
    double sum_odds = 0.0;
    double sum_evens = 0.0;

    unsigned nthreads = get_num_of_procs();
    numint_specific_params *params = malloc(nthreads * sizeof(numint_specific_params));
    numint_global_params global = {
        .a = a,
        .f = f,
        .h = h,
        .n = n,
        .nthreads = nthreads
    };

    for (unsigned i = 0; i < nthreads; ++i) {
        params[i].global = &global;
        params[i].tid = i;
        pthread_create(&params[i].handle, NULL, numint_impl, &params[i]);
    }

    for (unsigned i = 0; i < nthreads; ++i) {
        pthread_join(params[i].handle, NULL);
        sum_odds  += params[i].sum_odds;
        sum_evens += params[i].sum_evens;
    }

    free(params);
    return h / 3 * (fma(2, sum_evens, f(a)) + fma(4, sum_odds, f(b)));
}

void numint_init()
{
}

void numint_end()
{
}

int numint_is_master()
{
    return 1;
}
