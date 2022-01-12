#include <stdlib.h>
#include <time.h>

#include "stopwatch.h"


struct stopwatch_s {
    struct timespec tp;
};

static int stopwatch_diff(const struct timespec *start, const struct timespec *stop)
{
    int ms = (stop->tv_sec - start->tv_sec) * 1000
           + (stop->tv_nsec - start->tv_nsec) / 1000000;

    return ms;
}

int stopwatch_start(stopwatch_h* handle)
{
    if (handle == NULL)
        return 0;

    *handle = malloc(sizeof(**handle));

    if (clock_gettime(CLOCK_MONOTONIC, &(*handle)->tp) < 0) {
        free(*handle);
        *handle = NULL;
        return 0;
    }

    return 1;
}

int stopwatch_stop(stopwatch_h handle, int *ms)
{
    struct timespec tp;

    if (clock_gettime(CLOCK_MONOTONIC, &tp) < 0)
        return 0;

    if (ms == NULL)
        return 0;

    *ms = stopwatch_diff(&handle->tp, &tp);

    free(handle);
    return 1;
}
