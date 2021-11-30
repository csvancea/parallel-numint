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

/**
 * @brief Initialize numint library.
 * 
 * Needed by implementations with special startup/finalize requirements (eg: MPI)
 */
void numint_init();

/**
 * @brief Finalize numint library.
 * 
 * Needed by implementations with special startup/finalize requirements (eg: MPI)
 */
void numint_end();

/**
 * @brief Process type.
 * 
 * @returns true if the caller is the master process, otherwise false
 */
int  numint_is_master();

#endif // NUMINT_H
