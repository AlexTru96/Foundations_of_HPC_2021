#!/bin/bash
#PBS -l nodes=ct1pt-tnode008:ppn=2+ct1pt-tnode009:ppn=2
#PBS -l walltime=0:15:00
#PBS -q dssc

cd $PBS_O_WORKDIR
# parallel execution
module purge
module load intel/20.4
module load gnu/9.3.0
make clean
make
mpirun -n 2 -ppn 2 -genv I_MPI_PIN_PROCESSOR_LIST 0,2 ./IMB-MPI1 PingPong -msglog 28 2>/dev/null |grep -v ^# | grep -v '^$'| tr -s '[:blank:]' ',' | sed '1d' | cut -c 2- >core_intelmpi.csv
mpirun -n 2 -ppn 2 -genv I_MPI_PIN_PROCESSOR_LIST 0,1 ./IMB-MPI1 PingPong -msglog 28 2>/dev/null |grep -v ^# | grep -v '^$'| tr -s '[:blank:]' ',' | sed '1d' | cut -c 2- >socket_intelmpi.csv
mpirun -n 2 -ppn 1 -hosts ct1pt-tnode008,ct1pt-tnode009 ./IMB-MPI1 PingPong -msglog 28 2>/dev/null |grep -v ^# | grep -v '^$'| tr -s '[:blank:]' ',' | sed '1d' | cut -c 2- >node_intelmpi.csv
exit
