#include <stdio.h>
#include <stdbool.h>
#include <mpi.h>

int main( int argc,char **argv )
{
    int rank, value_right,value_left, size,message_counter=0;
    MPI_Status statusR,statusL;
    //communicator that allows periodicity
    MPI_Comm uroboros;
    MPI_Init( &argc, &argv );
    double start_time = MPI_Wtime();
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );

    int array_ordered[size];
    int periods[1] = {true};
    int reorder = true;
    int proc_dims[1] = {size};
    MPI_Cart_create(MPI_COMM_WORLD, 1, proc_dims, periods, reorder, &uroboros);

    //previous and next rank will be obtained as well
    int current_rank,next_rank,previous_rank;
    MPI_Comm_rank(uroboros ,&current_rank);
    MPI_Cart_shift(uroboros, 0, 1, &previous_rank,&next_rank);
    //printf("[MPI process, %d, move to %d] before I was (%d).\n", current_rank, next_rank, previous_rank);

    //why 1000000?, order of MB of course
for(int time_cons=0;time_cons<1000000;time_cons++){   
    //start of the iteration, free for all in every processor
    value_right=current_rank;
    value_left=-current_rank;
    //Each processor sends the message both directions
    MPI_Send( &value_right, 1, MPI_INT, next_rank, 10*current_rank, MPI_COMM_WORLD);
    MPI_Send( &value_left, 1, MPI_INT, previous_rank, 10*current_rank, MPI_COMM_WORLD );

    //As well each of them start to receive and send again and again
    for (int rank_iter=1; rank_iter<size;++rank_iter){

	    MPI_Recv( &value_right, 1, MPI_INT, previous_rank, MPI_ANY_TAG, MPI_COMM_WORLD, &statusR);
	    ++message_counter;
	    value_right+=current_rank;
	    MPI_Send( &value_right, 1, MPI_INT, next_rank, statusR.MPI_TAG, MPI_COMM_WORLD);

	    MPI_Recv( &value_left, 1, MPI_INT, next_rank, MPI_ANY_TAG, MPI_COMM_WORLD, &statusL);
	    ++message_counter;
	    value_left-=current_rank;
	    MPI_Send( &value_left, 1, MPI_INT, previous_rank, statusL.MPI_TAG, MPI_COMM_WORLD );
   }
   
   //Once the message traveled around all the processor, it will return to its creator
   MPI_Recv( &value_right, 1, MPI_INT, previous_rank, MPI_ANY_TAG, MPI_COMM_WORLD, &statusR);
   ++message_counter;
   MPI_Recv( &value_left, 1, MPI_INT, next_rank, MPI_ANY_TAG, MPI_COMM_WORLD, &statusL);
   ++message_counter;
}
   double end_time=MPI_Wtime();
   //You can comment this line , and you will receive only the elapsed time
   printf("I am process %d and i have received %d messages. My final messages have tag %d and value msg-left %d,msg-right %d \n",current_rank,message_counter,statusR.MPI_TAG,value_left,value_right);
   double elapsed_time= end_time-start_time;
   double max_time;
   MPI_Reduce(&elapsed_time,&max_time,1,MPI_DOUBLE,MPI_MAX,0,uroboros);

   if(current_rank==0){
   //printf ( "\n # max time on a processor: %10.8f \n", max_time );}

   printf ( " %10.8f \n", max_time );}
   MPI_Finalize( );
   return 0;
}
