#include <math.h>
#include "numint.h"


double numint(onedim_func_t f, double a, double b, unsigned n)
{
    double h = (b - a) / n;
    double sum_odds = 0.0;
    double sum_evens = 0.0;

    /*
     * fma stands for fused multiply-add and it's basically equivalent to: 
     * fma(x, y, z) = x * y + z
     */
    for (unsigned i = 1; i < n; i += 2)
    {
        sum_odds += f(fma(i, h, a));
    }
    
    for (unsigned i = 2; i < n; i += 2)
    {
        sum_evens += f(fma(i, h, a));
    }

    return h / 3 * (fma(2, sum_evens, f(a)) + fma(4, sum_odds, f(b)));
}
