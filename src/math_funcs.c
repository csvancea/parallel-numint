#include <math.h>
#include <string.h>

#include "numint.h"


double mf_unit(__attribute__((__unused__)) double x)
{
    return 1.0;
}

double mf_identity(double x)
{
    return x;
}

double mf_square(double x)
{
    return x*x;
}

double mf_cube(double x)
{
    return x*x*x;
}

double mf_inverse(double x)
{
    return 1/x;
}

double mf_loge(double x)
{
    return log(x);
}

double mf_misc_0(double x)
{
    return 2 * pow(x, 1./3) - 1./(3*x) + sin(x)/2;
}


#define COMPARE_AND_RETURN_IF_MATCH(func) \
    if (strcmp(name, (#func)) == 0) return (mf_ ## func)

onedim_func_t get_test_func(const char *name)
{
    COMPARE_AND_RETURN_IF_MATCH(unit);
    COMPARE_AND_RETURN_IF_MATCH(identity);
    COMPARE_AND_RETURN_IF_MATCH(square);
    COMPARE_AND_RETURN_IF_MATCH(cube);
    COMPARE_AND_RETURN_IF_MATCH(inverse);
    COMPARE_AND_RETURN_IF_MATCH(loge);

    COMPARE_AND_RETURN_IF_MATCH(misc_0);

    /* Nasol! */
    return NULL;
}
