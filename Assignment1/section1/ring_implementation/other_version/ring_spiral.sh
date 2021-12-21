#!/bin/bash
#PBS -l nodes=1:ppn=24
#PBS -l walltime=1:00:00
#PBS -q dssc

cd $PBS_O_WORKDIR
# parallel execution
module load openmpi-4.1.1+gnu-9.3.0
mpicc ring_spiral.c -o ring.x
for i in {1..24}
do
	mpirun -np $i ./ring.x 1>> output_ring
done
exit
