#include "tools.h"

int main(int argc, char** argv){

	int rank,size,provided,tid;
	size_t size_array;
	
 	//avoid MPI calls in parallel regions
	MPI_Init_thread(&argc, &argv,MPI_THREAD_FUNNELED,&provided);
	if(provided!=MPI_THREAD_FUNNELED) return EXIT_FAILURE;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);  
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int mpi_depth = get_depth(size); //gets the associated depth level according to the number of mpi processes

	FILE * f = fopen(argv[1],"r");
	int NumThr = atoi(argv[2]);  //set the number of threads per MPI processor also should be a power of 2
	int print_flag= atoi(argv[3]); //gets the associated depth according to the size

	if (!f){
		fprintf(stderr, "Error opening file"); /* FILE DOESNT EXIST*/
		return EXIT_FAILURE;	
  		}
	else{
		if(!fscanf(f,"%ld",&size_array)) return EXIT_FAILURE; /*FIRST NUMBER IN THE FILE IS THE SIZE */
		kpoint* array;
		array = malloc(sizeof(kpoint)*size_array);
		
		readArray(array,size_array,f);
		
		// OMP directives
		omp_set_dynamic(0); 
                omp_set_nested(0);
                omp_set_num_threads(NumThr);
		
		double time = MPI_Wtime();
		int initial_axis= 0; // 0 : axis X, 1 : axis Y
		kdnode* head_tree;
		
		//Creation of the master_tree
		#pragma omp parallel
   		{
   		// Only one single thread will spawn other threads
   		#pragma omp single nowait
			head_tree = build_master_tree(array, 0, size_array-1, initial_axis, mpi_depth);
		}
		
		if(rank==0){
			//Printing the master tree
			if(print_flag){	
				printf("\nThis is the master tree\n");
				print2DTree(head_tree,0,0,0,2);
			}
		}
		
		kdnode* binary_tree;
		size_t local_start=rank*size_array/size +1;
		size_t local_end=(rank+1)*size_array/size -1;
		initial_axis= (mpi_depth%2) ?  initial_axis : (initial_axis+1)%NDIM;
		
		int omp_depth = get_depth(NumThr);

		//Creation of the independent KDTrees by each OMP Thread
		#pragma omp parallel private(tid,binary_tree) shared(array,rank)
   		{
   			tid = omp_get_thread_num();

   			int *flags = encoder(tid, omp_depth);
   			int *flags_depths = depth_encoder(tid,omp_depth);
   			
			if(rank==0) binary_tree =  build_kdtree(array, local_start-1, local_end, initial_axis, flags, omp_depth);
			else binary_tree =  build_kdtree(array, local_start, local_end, initial_axis, flags, omp_depth);
		
		#pragma omp critical
		{
		//Printing the omp child trees
		if(print_flag){
					sleep(1*rank);
					printf("Child tree that belongs to rank %d , omp thread %d\n",rank,tid);
					print2DTree(binary_tree,flags,flags_depths,omp_depth,2);
					printf("\n \n");
 			      }
		}
				free(flags);
				free(flags_depths);
				free_tree(binary_tree);
		}
		
		time= MPI_Wtime() - time;
		double max_time;	
		MPI_Reduce(&time, &max_time, 1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);
		if(rank==0 && !print_flag) printf ("%10.5lf \n", max_time);
		
		free_tree(head_tree);
		free(array);
		fclose(f);
	}
	MPI_Finalize();

	return EXIT_SUCCESS;
}
