/*
 * File:     treesum_mpi.c
 *
 * Purpose:  Use tree-structured communication to find the global sum
 *           of a random collection of ints.  This version doesn't
 *           require that comm_sz be a power of 2.
 *
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

/*-------------------------------------------------------------------
 * Function:
 *  global_sum
 *
 * Purpose:
 *  Implement a global sum using tree-structured communication
 *
 * Notes:
 *  1.  The return value for global sum is only valid on process 0
 */
int global_sum(int my_int /* in */, int my_rank /* in */, int comm_sz /* in */,
               MPI_Comm comm /* in */) {
  int my_sum = my_int;

  /* Your solution */

  // // printf("I am proc #%d, and this is my value %d\n", my_rank, my_int);

  int numProc = comm_sz;
  int sum = my_int;
  int other_sum = 0;


  // printf("NUM PROC: %d\n", numProc);
  if(numProc%2 == 1 && my_rank == (numProc -1)) {  // If odd num Processors, the last value is added afterwards
      MPI_Send(&sum, 1, MPI_INT, 0, 0, comm);
    // printf("Sum %d\n", sum);
  }
  // printf("Sum %d\n", sum);
  // printf("MySum %d\n", my_sum);
  if(numProc%2 == 1 && my_rank == 0) {
    MPI_Recv(&other_sum, 1, MPI_INT, numProc -1, 0, comm, MPI_STATUS_IGNORE);
    sum= other_sum+sum;
  }

  // // printf("%d, this is my_rank!\n", my_rank);
  if(my_rank %2 !=0 &&my_rank>0) {
    MPI_Send(&sum, 1, MPI_INT, my_rank -1, 0, comm);
  }
  // printf("Sending 1\n");
  // printf("NumProc: %d", numProc);
  for(int gap=1; gap < numProc; gap =gap*2) {
    // printf("Condition %d\n", my_rank%(2*gap));
    if(my_rank%(2*gap) == 0 && my_rank+gap < (numProc -1)) {
      MPI_Recv(&other_sum, 1, MPI_INT, my_rank+gap, 0, comm, MPI_STATUS_IGNORE);
      // printf("Receiving 1\n");
      sum= other_sum+sum;
      if( (my_rank % (4*gap)) !=0 && my_rank>=2*gap) {
        MPI_Send(&sum, 1, MPI_INT, my_rank -2*gap, 0, comm);
      }
      // printf("Sending 2\n");
    }
  }


  return sum;
} /* Global_sum */
