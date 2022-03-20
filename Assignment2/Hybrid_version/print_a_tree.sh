#!/bin/bash
#PBS -l nodes=1:ppn=8
#PBS -l walltime=1:00:00
#PBS -q dssc_gpu

cd $PBS_O_WORKDIR

# parallel execution
module load openmpi-4.1.1+gnu-9.3.0

make clean
make

mpirun -np 2 --map-by core ./binary_tree ./data/printing_toy_test 2 1 1>PRINTED_TREE 2>/dev/null
make clean
exit
