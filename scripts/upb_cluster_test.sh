#!/usr/bin/env bash

############################################################
### This script shall be run from root project directory ###
############################################################

OUTFILE=$1 # optional argument

set -e

module load compilers/gnu-5.4.0
module load libraries/openmpi-2.0.1-gcc-5.4.0

make clean
make

python3 -u ./scripts/scalability.py | tee $OUTFILE
