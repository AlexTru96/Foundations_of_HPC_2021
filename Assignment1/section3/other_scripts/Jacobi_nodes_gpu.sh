#!/bin/bash
#PBS -l nodes=ct1pg-gnode001:ppn=24+ct1pg-gnode003:ppn=24
#PBS -l walltime=0:15:00
#PBS -q dssc_gpu

cd $PBS_O_WORKDIR
# parallel execution
module purge
module load openmpi-4.1.1+gnu-9.3.0
mpirun -np 48 --map-by node ./Jacobi3D.x <input.1200 2>/dev/null 1>jacobi_nodes48_gpu
exit
