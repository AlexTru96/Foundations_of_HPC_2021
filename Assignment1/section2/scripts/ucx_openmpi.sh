#!/bin/bash
#PBS -l nodes=ct1pt-tnode008:ppn=2+ct1pt-tnode009:ppn=2
#PBS -l walltime=0:15:00
#PBS -q dssc

cd $PBS_O_WORKDIR
# parallel execution
module purge
make clean
module load openmpi-4.1.1+gnu-9.3.0
make
mpirun -np 2 --map-by core -report-bindings ./IMB-MPI1 PingPong -msglog 28 2>/dev/null |grep -v ^# | grep -v '^$'| tr -s '[:blank:]' ',' | sed '1d' | cut -c 2- >core_ucx_openmpi.csv
mpirun -np 2 --map-by node -report-bindings ./IMB-MPI1 PingPong -msglog 28 2>/dev/null |grep -v ^# | grep -v '^$'| tr -s '[:blank:]' ',' | sed '1d' | cut -c 2- >node_ucx_openmpi.csv
mpirun -np 2 --map-by socket -report-bindings ./IMB-MPI1 PingPong -msglog 28 2>/dev/null |grep -v ^# | grep -v '^$'| tr -s '[:blank:]' ',' | sed '1d' | cut -c 2- >socket_ucx_openmpi.csv
exit
