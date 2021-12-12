#include <omp.h>
#include <math.h>
#include "numint.h"


double numint(onedim_func_t f, double a, double b, unsigned n)
{
    double h = (b - a) / n;
    double sum_odds = 0.0;
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
         */
        #pragma omp for reduction(+:sum_odds) nowait
        for (unsigned i = 1; i < n; i += 2)
        {
            sum_odds += f(fma(i, h, a));
        }

        #pragma omp for reduction(+:sum_evens)
        for (unsigned i = 2; i < n; i += 2)
        {
            sum_evens += f(fma(i, h, a));
        }
    }

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
