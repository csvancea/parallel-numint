#ifndef STOPWATCH_H
#define STOPWATCH_H

/* Opaque stopwatch handle */
typedef struct stopwatch_s* stopwatch_h;

/**
 * @brief Create and start a stopwatch
 *
 * @param handle Pointer to a variable where the handle will be stored
 * @return Status (1 = success, 0 = failure)
 */
int stopwatch_start(stopwatch_h* handle);

/**
 * @brief Stop and free resources of a stopwatch
 *
 * @param handle Handle to a previously started stopwatch.
 * @param ms Pointer to a variable where the time elapsed (in ms) will be stored
 * @return Status (1 = success, 0 = failure)
 */
int stopwatch_stop(stopwatch_h handle, int *ms);

#endif
