#!/bin/bash

############################################################
### This script shall be run from root project directory ###
############################################################

path="./results/plot_input.txt"
scalability_output="./results/scalability.svg"
speedup_output="./results/speedup.svg"

threads=(1 2 4 8 16 20 32)
threads_string=$(printf ",%s" ${threads[@]})
threads_string="(${threads_string:1})"
xf=${threads[0]}
xl=${threads[-1]}

gnuplot -e "
    set term svg;
    set output '$scalability_output';
    set print \"-\";
    set xrange [$xf:$xl];
    set xtics $threads_string;
    set xlabel 'Number of threads/processes';
    set ylabel 'Time (ms)';
    plot '$path' using 1:2 title 'omp-time' smooth bezier, \
         '$path' using 1:3 title 'pthreads-time' smooth bezier, \
         '$path' using 1:4 title 'mpi-time' smooth bezier;"

gnuplot -e "
    set term svg;
    set output '$speedup_output';
    set print \"-\";
    set xrange [$xf:$xl];
    set xtics $threads_string;
    set xlabel 'Number of threads/processes';
    set ylabel 'Speedup';
    set y2tics;
    st_time = 0;
    first(x) = (\$0 > 0 ? st_time : st_time = x);
    plot y1 = 0, \
         y2 = 0, \
         y3 = 0, \
         '$path' using 1:((y1 == 0 ? (y1 = \$2, \$2) : y1) / \$2) title 'omp-speedup' smooth bezier, \
         '$path' using 1:((y2 == 0 ? (y2 = \$3, \$3) : y2) / \$3) title 'pthreads-speedup' smooth bezier, \
         '$path' using 1:((y3 == 0 ? (y3 = \$4, \$4) : y3) / \$4) title 'mpi-speedup' smooth bezier;"
