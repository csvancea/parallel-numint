#!/usr/bin/env python3

############################################################
### This script shall be run from root project directory ###
############################################################

import subprocess
import time
import os

TESTS = {
    # serial must be the first test
    "serial": {
        "invoke": "{EXEFILE} {ARGS}",
        "num_processes": [1],
        "num_threads": [1]
    },
    "omp": {
        "invoke": "{EXEFILE} {ARGS}",
        "num_processes": [1],
        "num_threads": [1, 2, 4, 8, 16, 32],
        "env": {
            "OMP_NUM_THREADS": "{NTHREADS}"
        }
    },
    "pthread": {
        "invoke": "{EXEFILE} {ARGS}",
        "num_processes": [1],
        "num_threads": [1, 2, 4, 8, 16, 32],
        "env": {
            "NUMINT_NUM_PROCS": "{NTHREADS}"
        }
    },
    "mpi": {
        "invoke": "mpirun --oversubscribe -np {NPROCS} {EXEFILE} {ARGS}",
        "num_processes": [1, 2, 4, 8, 16, 32],
        "num_threads": [1]
    },
    "mpi_omp": {
        "invoke": "mpirun --oversubscribe -np {NPROCS} {EXEFILE} {ARGS}",
        "num_processes": [1, 2, 4, 8, 16, 32],
        "num_threads": [1, 2, 4, 8, 16, 32],
        "env": {
            "OMP_NUM_THREADS": "{NTHREADS}"
        }
    },
    "mpi_pthread": {
        "invoke": "mpirun --oversubscribe -np {NPROCS} {EXEFILE} {ARGS}",
        "num_processes": [1, 2, 4, 8, 16, 32],
        "num_threads": [1, 2, 4, 8, 16, 32],
        "env": {
            "NUMINT_NUM_PROCS": "{NTHREADS}"
        }
    }
}

ARGS = {
    "func": "misc_0",
    "a": 1,
    "b": 10,
    "n": 10**9
}


reference_output = b'INVALID'

for exefile, test in TESTS.items():
    for nprocs in test['num_processes']:
        for nthreads in test['num_threads']:

            # prepare subprocess command
            invoke = test['invoke'].format(
                EXEFILE  = f"./bin/{exefile}",
                ARGS     = f"{ARGS['func']} {ARGS['a']} {ARGS['b']} {ARGS['n']}",
                NPROCS   = nprocs,
                NTHREADS = nthreads
            )

            # prepare subprocess environment vars
            env = os.environ.copy()
            if 'env' in test:
                for k, v in test['env'].items():
                    env[k] = v.format(NPROCS = nprocs, NTHREADS = nthreads)

            # run the test
            start_time = time.time()
            output = subprocess.check_output(invoke, stderr=subprocess.DEVNULL, shell=True, env=env)
            end_time = time.time()

            if exefile == 'serial':
                # consider serial output as reference
                reference_output = output
            else:
                # make sure that output matches the reference
                # otherwise there is an error in our parallel implementation
                # TODO: rounding errors?
                assert output == reference_output

            elapsed_ms = int((end_time - start_time) * 1000)

            print(f"{exefile}_p{nprocs}_t{nthreads}: {elapsed_ms}")
