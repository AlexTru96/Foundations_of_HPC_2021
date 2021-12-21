
# HOW TO COMPILE AND RUN

## RING PROBLEM
- Be sure to be in section1/ring_implementation directory, then you can compile and run with only one command thanks to bash:
>  `qsub ring.sh`

 - What does this script do? : Saves you a lot of time
 Well.. In fact calls PBS routines asking for 24 processors in a node (Default = 1 node), setting a walltime only for 15 minutes using the DSSC queue. 
 Then once it access to the requested node, it will access to the previous work directory, load the OPENMPI 4.1.1+GNU-9.3.0 module and compile the C program **ring.c** creating the executable **ring.x** .
By means of a for loop will iterate 24 times (default) the next comand :
`mpirun -np $i ./ring.x 1>> output_ring`
Where $i indicates the number of processors (1 to 24), appending the result to the ouput_ring file.
 - More information about the code and extra versions in the report
## SUM OF 3D MATRICES 

- Be sure to be in section1/matrix3D_implementation directory, then you have two ways to compile and run this program:

> Single run:   `qsub single_run.sh`
- This is the preferred way to run the code,  it will print in the single_output file the matrix (4x4x4 default using 24 processors) and total walltime, communication walltime, and the dimensions of the matrix and grid of processors. 
This script calls PBS routines asking for 24 processors in a node, setting a walltime only for 10 minutes using the DSSC queue. 
 Then once it access to the requested node, it will access to the previous work directory, load the OPENMPI 4.1.1+GNU-9.3.0 module and compile the C program **sum3Dmatrix_float.c** creating the executable **sum3D.x** .
It will run the executable using the next command line
`mpirun --mca coll ^tuned -np 24 ./sum3D.x <single_input 1>>single_output`
Where single_input is a file that contains the desired dimension of the matrix and processors, and the tuned option for collective algorithms is deactived to avoid problems with huge matrices (optional)
> Multiple run:  `qsub sum3D.sh`
- This should be called when you just require data collection (In this case the matrix print lines in the code should be commented).
It will follow the same steps as the single call script, but this time by means of a while loop will read the multiple_input file which can contain lines with different dimensions for several matrices as well as grid processors.
For each line the script will iterate 5 times (default) the next comand :
`mpirun --mca coll ^tuned -np 24 ./sum3D.x <tmp 1>>output_raw`
Where tmp is a file containing only the current read line from multiple_input and appends all the results in the output_raw file.
The next step is to obtain the mean of the total and gather communicatio walltime by using awk.
`awk 'BEGIN{x=0;y=0} {x+=$1;y+=$2} END{printf "%.6f %.6f\n",x/NR,y/NR}' <output_raw  >>output_sum3D`
It means for every line of multiple_input the script will run 5 times and will append the mean of the walltimes in the output_sum3D file.
 - More information about the code and extra versions in the report
