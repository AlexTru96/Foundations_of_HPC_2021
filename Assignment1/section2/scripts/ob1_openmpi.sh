#!/bin/bash
#PBS -l nodes=ct1pt-tnode008:ppn=2+ct1pt-tnode009:ppn=2
#PBS -l walltime=0:15:00
#PBS -q dssc

cd $PBS_O_WORKDIR
# parallel execution
module purge
module load openmpi-4.1.1+gnu-9.3.0
make clean
make
mpirun --mca pml ob1 --mca btl self,tcp --map-by core -np 2 -report-bindings ./IMB-MPI1 PingPong -msglog 28 2>/dev/null |grep -v ^# | grep -v '^$'| tr -s '[:blank:]' ',' | sed '1d' | cut -c 2- >core_ob1_openmpi.csv
mpirun --mca pml ob1 --mca btl self,tcp --map-by socket -np 2 -report-bindings ./IMB-MPI1 PingPong -msglog 28 2>/dev/null |grep -v ^# | grep -v '^$'| tr -s '[:blank:]' ',' | sed '1d' | cut -c 2- >socket_ob1_openmpi.csv
mpirun --mca pml ob1 --mca btl self,tcp  -npernode 1 -np 2 -report-bindings ./IMB-MPI1 PingPong -msglog 28 2>/dev/null |grep -v ^# | grep -v '^$'| tr -s '[:blank:]' ',' | sed '1d' | cut -c 2- >node_ob1_openmpi.csv
exit
