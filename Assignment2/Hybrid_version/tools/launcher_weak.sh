#!/bin/bash
#PBS -l nodes=3:ppn=24
#PBS -l walltime=3:00:00
#PBS -q dssc

cd $PBS_O_WORKDIR

# parallel execution
module load openmpi-4.1.1+gnu-9.3.0

#export OMP_NUM_THREADS=16
make clean
make

#export OMP_PLACES=sockets
#export OMP_PROC_BIND=close
for i in 1 2 4 8 16
do
                let var=$i*4194304
                ./tools/random.x $var 1>test_tree
                mpirun -np $i --map-by socket ./binary_tree test_tree 2 0 1>>weak_scalability
#       awk 'BEGIN{x=0;y=0;w=0;z=0} {x+=$1;y+=$2;w+=$3;z+=$4} END{printf "%.6f %.6f %.6f %.6f\n",x/NR,y/NR,w/NR,z/NR}' <raw_result_2  >>outputdgemm8n_32k
#        rm raw_result_2
       	 	echo $i+mpi_proc >>weak_scalability
done
make clean
exit
