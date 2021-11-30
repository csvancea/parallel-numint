#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "numint.h"
#include "math_funcs.h"


void master_printf(const char *fmt, ...)
{
    va_list arglist;

    if (numint_is_master()) {
        va_start(arglist, fmt);
        vprintf(fmt, arglist);
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

    if (f == NULL) {
        master_printf("Invalid func name provided.\n");
        goto cleanup;
    }

    if (n == 0) {
        master_printf("n must be non-zero.\n");
        goto cleanup;
    }

    master_printf("Integrating f=%s over [%f, %f] in %d steps.\n", func_name, a, b, n);
    double result = numint(f, a, b, n);
    master_printf("Result = %f\n", result);

cleanup:
    numint_end();
    return 0;
}
