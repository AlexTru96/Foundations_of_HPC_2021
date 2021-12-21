#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>
#include <mpi.h>
#define SIZE_T_MAX ((size_t)(~0ULL))

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv); 
    int old_rank,size;
    int n_x=24,n_y=12,n_z=8;
    int original_dim[3],aumented_dim[3],local_dim[3];
    int p_x=0,p_y=0,p_z=0;
    int proc_dims[3];
    MPI_Comm_rank(MPI_COMM_WORLD,&old_rank);  
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    srand(old_rank*42+777); //seeds for random initialization

if(old_rank==0){

    //printf("insert dimensions (3D i)\n");
    scanf("%d %d", &n_x, &p_x);
    //printf("insert dimensions (3D i)\n");
    scanf("%d %d", &n_y, &p_y);
    //printf("insert dimensions (3D i)\n");
    scanf("%d %d", &n_z, &p_z);

    original_dim[0]=n_x;original_dim[1]=n_y;original_dim[2]=n_z;
    aumented_dim[0]=n_x;aumented_dim[1]=n_y;aumented_dim[2]=n_z;
    proc_dims[0]=p_x;proc_dims[1]=p_y;proc_dims[2]=p_z;
    
    if(p_x==0||p_y==0||p_z==0){
	MPI_Dims_create(size, 3, proc_dims);
    }

    if((p_x*p_y*p_z)!=size && (p_x*p_y*p_z)!=0){
	printf("mismatch of # processors, bye");
	MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    for(int i=0;i<3;++i){
	if(original_dim[i]%proc_dims[i]==0){
	        local_dim[i]=original_dim[i]/proc_dims[i];
	}
	else{
	        //In case that the dimensions of the matrix are not multiple of the size, I enlarge it to balance the work
		aumented_dim[i]=original_dim[i]+(proc_dims[i]-(original_dim[i]%proc_dims[i]));
	       	local_dim[i]=(aumented_dim[i]/proc_dims[i]);
	}
    }
    //printf("dimension array: %d %d %d \n", original_dim[0],original_dim[1],original_dim[2]);
    //printf("processor array: %d %d %d \n", proc_dims[0],proc_dims[1],proc_dims[2]); 
    //printf("local dimension array: %d %d %d \n", local_dim[0],local_dim[1],local_dim[2]); 
}
    MPI_Barrier(MPI_COMM_WORLD);  //do nothing till the master says go

    //SEnding all the information to the processors
    MPI_Bcast(proc_dims, 3, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(original_dim, 3, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(aumented_dim, 3, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(local_dim, 3, MPI_INT, 0, MPI_COMM_WORLD);
    p_x=proc_dims[0];p_y=proc_dims[1];p_z=proc_dims[2];
    n_x=original_dim[0];n_y=original_dim[1];n_z= original_dim[2];
    
    size_t m_x=local_dim[0];
    size_t m_y=local_dim[1];
    size_t m_z=local_dim[2];
    size_t aum_n_x=aumented_dim[0];
    size_t aum_n_y=aumented_dim[1];
    size_t aum_n_z=aumented_dim[2];
    typedef double slice_original[n_y][n_z];
    typedef double slice_partial[m_y][m_z];
    size_t aum_huge_size=aum_n_x*aum_n_y*aum_n_z;
    
    // Ask MPI to decompose our processes in a 2D cartesian grid for us
    // Make both dimensions periodic
    int periods[3] = {false, false,false}; 
    int reorder = true;

    MPI_Comm grid_communicator;
    MPI_Cart_create(MPI_COMM_WORLD, 3, proc_dims, periods, reorder, &grid_communicator);
    // My rank in the new communicator
    int my_rank;
    MPI_Comm_rank(grid_communicator, &my_rank);

    // Get my coordinates in the new communicator
    int my_coords[3];
    MPI_Cart_coords(grid_communicator, my_rank, 3, my_coords);
    
    slice_original *original_matrix_1,*original_matrix_2;
    slice_partial *partial_matrix_1,*partial_matrix_2;

    size_t hn_x=n_x;
    size_t hn_y=n_y;
    size_t hn_z=n_z;
    size_t huge_size=hn_x*hn_y*hn_z;

    double start_time = MPI_Wtime();

    if(my_rank==0){
    original_matrix_1 = (slice_original *)malloc((huge_size)*sizeof(double));
    original_matrix_2 = (slice_original *)malloc((huge_size)*sizeof(double));
    }
    size_t partial_huge_size= m_x*m_y*m_z;
    partial_matrix_1 = (slice_partial *)malloc((partial_huge_size)*sizeof(double));
    partial_matrix_2 = (slice_partial *)malloc((partial_huge_size)*sizeof(double));

//initialization through multiple cores, count down till find the nearest multiple of the size of the matrix 	

//int n_initialization_cores=(huge_size)%(size);
//if(n_initialization_cores==0){
//	n_initialization_cores=size;
//}
//else{
//	for(int i=size-1;i>0;--i){
//		if((huge_size)%(i)==0){
//			n_initialization_cores=i;
//		break;
//		}
//	}	
//}
//// initialized rows with float random numbers between 0 and 1
//     size_t init_size= (huge_size)/n_initialization_cores;
//     typedef double init_slice[1][1];
//     init_slice *partial_init_1;
//     init_slice *partial_init_2;
//     partial_init_1 = (init_slice*)malloc((init_size)*sizeof(double));
//     partial_init_2 = (init_slice*)malloc((init_size)*sizeof(double));
//
//
//if(my_rank<n_initialization_cores){
//     for (size_t i=0;i<init_size;++i){
//     	partial_init_1[0][0][i]= (rand() % 1001)/1000.0;
//     	partial_init_2[0][0][i]= (rand() % 1001)/1000.0;
//     }
//}
//
//MPI_Group world_group;
//MPI_Comm_group(MPI_COMM_WORLD, &world_group);
//MPI_Group init_group;
//int ranks[n_initialization_cores];
//
//for(int index=0; index<n_initialization_cores;index++){
//	ranks[index]=index;
//}
//
//MPI_Group_incl(world_group, n_initialization_cores, ranks, &init_group);
//MPI_Comm init_communicator;
//MPI_Comm_create(MPI_COMM_WORLD, init_group, &init_communicator);
//
//double t1_comm_1=MPI_Wtime();
////Gather all the strips in the main matrix
//if(my_rank<n_initialization_cores){
//MPI_Gather(&(partial_init_1[0][0][0]), init_size, MPI_DOUBLE, &(original_matrix_1[0][0][0]), init_size, MPI_DOUBLE, 0, init_communicator);
//MPI_Gather(&(partial_init_2[0][0][0]), init_size, MPI_DOUBLE, &(original_matrix_2[0][0][0]), init_size, MPI_DOUBLE, 0, init_communicator);
//}
//
//double t2_comm_1=MPI_Wtime();
//double t1_ov = t2_comm_1-t1_comm_1;
//initialization using one single core
if(my_rank==0){
	int c=0;
for (int i=0;i<n_x;++i){
	for(int j=0;j<n_y;++j){
		for(int k=0;k<n_z;++k){
	     		original_matrix_1[i][j][k]=(rand() % 1001)/1000.0;
			original_matrix_2[i][j][k]=(rand() % 1001)/1000.0; 
		}
	}
}
}

//Lets use scatterv to disperse the whole matrix into blocks instead of strips
MPI_Datatype block_partial,resized_block;
int starts[3]={0,0,0};
MPI_Type_create_subarray(3, aumented_dim, local_dim, starts, MPI_ORDER_C, MPI_DOUBLE, &block_partial);
MPI_Type_create_resized(block_partial, 0, m_z*sizeof(double), &resized_block); //This will indicate to ScatterV how many double size should jump every time this new data type is called
MPI_Type_commit(&resized_block);

int *counts = (int*)malloc((p_x*p_y*p_z)*sizeof(int));
int *displaces = (int*)malloc((p_x*p_y*p_z)*sizeof(int));

for (int l=0;l< (p_x*p_y*p_z);++l){
	counts[l]=1;
}

int jump_counter=0;
for(int i=0;i<p_x;++i){
	for(int j=0;j<p_y;++j){
		for(int k=0;k<p_z;++k){
			displaces[jump_counter]=i*(p_z*aum_n_y*m_x)+j*(p_z*m_y)+k;
			++jump_counter;
		}
	}
}

//realloc to increase size and send all the messages with no problems
if(my_rank==0){
original_matrix_1=realloc(original_matrix_1,(aum_huge_size)*sizeof(double));
original_matrix_2=realloc(original_matrix_2,(aum_huge_size)*sizeof(double));
}

double t1_comm_2=MPI_Wtime();
//Sending partial blocks to every available core
MPI_Scatterv(&(original_matrix_1[0][0][0]), counts, displaces, resized_block, &(partial_matrix_1[0][0][0]), partial_huge_size, MPI_DOUBLE, 0, grid_communicator);
MPI_Scatterv(&(original_matrix_2[0][0][0]), counts, displaces, resized_block, &(partial_matrix_2[0][0][0]), partial_huge_size, MPI_DOUBLE, 0, grid_communicator);

double t2_comm_2=MPI_Wtime();
double t2_ov=t2_comm_2-t1_comm_2;
//sum of partial matrices
for (int i=0;i<m_x;++i){
	for(int j=0;j<m_y;++j){
		for(int k=0;k<m_z;++k){
			 partial_matrix_1[i][j][k]+=partial_matrix_2[i][j][k];
	}}}

// NOW GATHER!
double t1_comm=MPI_Wtime();
MPI_Gatherv(&(partial_matrix_1[0][0][0]), partial_huge_size, MPI_DOUBLE, &(original_matrix_1[0][0][0]), counts, displaces, resized_block, 0, grid_communicator);
//Lets consider as communication time the elapsed time to gather all the blocks into the aumented matrix
double elapsed_time_comm=MPI_Wtime()-t1_comm+t2_ov;
double max_time_comm;

//free datatype, wont be used anymore
MPI_Type_free(&resized_block);
if(my_rank==0){
//back to original size
original_matrix_1=realloc(original_matrix_1, (huge_size)*sizeof(double));
//
//printf("\nhere is your matrix:\n");
//for(int i=0;i<n_x;++i){
//	for(int j=0;j<n_y;++j){
//		for(int k=0;k<n_z;++k){
//			printf("%5.3f ",original_matrix_1[i][j][k]);
//		}
//		printf("|");
//	}
//	printf("\n");
//}

free(&(original_matrix_1[0][0][0]));
free(&(original_matrix_2[0][0][0]));
}
free(&(partial_matrix_1[0][0][0]));
free(&(partial_matrix_2[0][0][0]));
//free(&(partial_init_1[0][0][0]));
//free(&(partial_init_2[0][0][0]));
free(counts);
free(displaces);
double end_time=MPI_Wtime();
double elapsed_time = end_time-start_time;
double max_time;
//printf ("\n # walltime on processor %i : %10.8f sec \n",my_rank, elapsed_time ) ;
MPI_Reduce(&elapsed_time, &max_time, 1,MPI_DOUBLE,MPI_MAX,0,grid_communicator);
MPI_Reduce(&elapsed_time_comm, &max_time_comm, 1,MPI_DOUBLE,MPI_MIN,0,grid_communicator);
if(my_rank==0){
// Max elapsed time on a processor
	printf("%10.8f ",max_time);
	printf("%10.8f ",max_time_comm);
        printf("%d %d %d ", original_dim[0],original_dim[1],original_dim[2]);
        printf("%d %d %d\n", proc_dims[0],proc_dims[1],proc_dims[2]); 
}
MPI_Finalize();
    return EXIT_SUCCESS;
}
