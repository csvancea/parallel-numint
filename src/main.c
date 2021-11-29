#include <stdio.h>
#include <stdlib.h>

#include "numint.h"
#include "math_funcs.h"


int main(int argc, char **argv)
{
    if (argc < 5) {
        printf("Usage: %s [math-func-name] [a] [b] [n]\n", argv[0]);
        return 0;
    }

    char *func_name = argv[1];
    double a = atof(argv[2]);
    double b = atof(argv[3]);
    unsigned n = atoi(argv[4]);
    onedim_func_t f = get_test_func(func_name);

    if (f == NULL) {
        printf("Invalid func name provided.\n");
    }

    if (n == 0) {
        printf("n must be non-zero.\n");
        return 0;
    }

    printf("Integrating f=%s over [%f, %f] in %d steps.\n", func_name, a, b, n);
    double result = numint(f, a, b, n);
    printf("Result = %f\n", result);
    return 0;
}
