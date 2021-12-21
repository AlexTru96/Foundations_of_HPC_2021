#!/bin/bash
#PBS -l nodes=1:ppn=24
#PBS -l walltime=0:10:00
#PBS -q dssc

cd $PBS_O_WORKDIR

# parallel execution
module load openmpi-4.1.1+gnu-9.3.0
mpicc sum3Dmatrix_float.c -o sum3D.x
mpirun --mca coll ^tuned -np 24 ./sum3D.x <single_input 1>>single_output

exit


