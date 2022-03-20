#!/bin/bash
#PBS -l nodes=2:ppn=16
#PBS -l walltime=3:00:00
#PBS -q dssc_gpu

cd $PBS_O_WORKDIR

# parallel execution
module load openmpi-4.1.1+gnu-9.3.0

make clean
make

mpirun -np 8 --map-by socket ./binary_tree ./data/test_tree 2 0 1>time_result 2>/dev/null

make clean
exit
