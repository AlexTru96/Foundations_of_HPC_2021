#!/bin/bash
#PBS -l nodes=1:ppn=24
#PBS -l walltime=1:30:00
#PBS -q dssc

cd $PBS_O_WORKDIR

# parallel execution
module load openmpi-4.1.1+gnu-9.3.0
mpicc sum3Dmatrix_coreinit.c -o sum3D.x
while read i
do
	rm output_raw
	echo "$i" >tmp
	for i in {1..10}
	do
		mpirun --mca coll ^tuned -np 24 ./sum3D.x <tmp 1>>output_raw
	done
	awk 'BEGIN{x=0;y=0} {x+=$1;y+=$2} END{printf "%.6f %.6f %d %d %d %d %d %d\n",x/NR,y/NR,$3,$4,$5,$6,$7,$8}' <output_raw  >>output_sum3D
done <input

rm tmp
rm output_raw
exit


