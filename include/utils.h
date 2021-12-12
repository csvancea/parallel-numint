#ifndef UTILS_H
#define UTILS_H

#define _MIN(a, b) ((a) < (b) ? (a) : (b))

/**
 * @brief Number of available processors.
 * 
 * @return If NUMINT_NUM_PROCS environment variable is set,
 * this function will simply return that value, otherwise it
 * will return the number of available cores.
 */
unsigned get_num_of_procs();

#endif // UTILS_H
