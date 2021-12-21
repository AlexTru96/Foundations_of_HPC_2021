#!/bin/bash
#PBS -l nodes=ct1pt-tnode008:ppn=24+ct1pt-tnode009:ppn=24
#PBS -l walltime=0:15:00
#PBS -q dssc

cd $PBS_O_WORKDIR
# parallel execution
module purge
module load openmpi-4.1.1+gnu-9.3.0
mpirun -np 12 --map-by node ./Jacobi3D.x 2>/dev/null 1>jacobi_12_2nodes
exit
