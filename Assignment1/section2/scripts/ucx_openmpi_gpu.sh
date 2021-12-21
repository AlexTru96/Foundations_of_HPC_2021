#!/bin/bash
#PBS -l nodes=ct1pg-gnode001:ppn=2+ct1pg-gnode003:ppn=2
#PBS -l walltime=0:15:00
#PBS -q dssc_gpu

cd $PBS_O_WORKDIR
# parallel execution
module purge
make clean
module load openmpi-4.1.1+gnu-9.3.0
make
mpirun -np 2 --map-by core -report-bindings ./IMB-MPI1 PingPong -msglog 28 2>/dev/null |grep -v ^# | grep -v '^$'| tr -s '[:blank:]' ',' | sed '1d' | cut -c 2- >core_ucx_openmpi_gpu.csv
mpirun -np 2 --map-by socket -report-bindings ./IMB-MPI1 PingPong -msglog 28 2>/dev/null |grep -v ^# | grep -v '^$'| tr -s '[:blank:]' ',' | sed '1d' | cut -c 2- >socket_ucx_openmpi_gpu.csv
mpirun -np 2 --map-by node -report-bindings ./IMB-MPI1 PingPong -msglog 28 2>/dev/null |grep -v ^# | grep -v '^$'| tr -s '[:blank:]' ',' | sed '1d' | cut -c 2- >node_ucx_openmpi_gpu.csv
exit
