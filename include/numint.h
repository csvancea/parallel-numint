#ifndef NUMINT_H
#define NUMINT_H

typedef double (*onedim_func_t)(double x);

/**
 * @brief Composite Simpson's rule for numerical integration
 *
 * @param f one-dimensional mathematical function to be integrated
 * @param a lower limit of the integral
 * @param b upper limit of the integral
 * @param n number of subintervals
 * @return numerical integration result
 */
double numint(onedim_func_t f, double a, double b, unsigned n);

#endif // NUMINT_H
