#include <sys/sysinfo.h>
#include <stdlib.h>


unsigned get_num_of_procs()
{
    int nproc = 0;
    char *env = getenv("NUMINT_NUM_PROCS");
    
    /* user specified value has priority over the number of available CPU cores */
    if (env) {
        nproc = atoi(env);
    }

    if (nproc < 1) {
        nproc = get_nprocs();
    }

    if (nproc < 1) {
        nproc = 1;
    }

    return nproc;
}
