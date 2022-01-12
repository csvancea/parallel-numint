#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "numint.h"
#include "math_funcs.h"
#include "stopwatch.h"


#define master_printf(...) master_fprintf(stdout, __VA_ARGS__)
static void master_fprintf(FILE *stream, const char *fmt, ...)
{
    va_list arglist;

    if (numint_is_master()) {
        va_start(arglist, fmt);
        vfprintf(stream, fmt, arglist);
        va_end(arglist);
    }
}

int main(int argc, char **argv)
{
    numint_init();

    if (argc < 5) {
        master_printf("Usage: %s [math-func-name] [a] [b] [n]\n", argv[0]);
        goto cleanup;
    }

    char *func_name = argv[1];
    double a = atof(argv[2]);
    double b = atof(argv[3]);
    unsigned n = atoi(argv[4]);
    onedim_func_t f = get_test_func(func_name);
    stopwatch_h sw;
    int ms = 0;

    if (f == NULL) {
        master_printf("Invalid func name provided.\n");
        goto cleanup;
    }

    if (n == 0) {
        master_printf("n must be non-zero.\n");
        goto cleanup;
    }

    master_printf("Integrating f=%s over [%f, %f] in %d steps.\n", func_name, a, b, n);

    stopwatch_start(&sw);
    double result = numint(f, a, b, n);
    stopwatch_stop(sw, &ms);
    master_fprintf(stderr, "%d\n", ms);

    master_printf("Result = %f\n", result);

cleanup:
    numint_end();
    return 0;
}
