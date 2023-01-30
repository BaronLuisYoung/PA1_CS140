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

  int me = my_rank;
  int numProc = MPI_Comm_size(comm, &comm_sz);
  int sum = my_int;
  int other_sum = 0;
  MPI_Status status;
  if(me %2 !=0 &&me>0)
    MPI_Send(&sum, 1, MPI_INT, me -1, 0, comm);
  for(int gap=1; gap < numProc; gap =gap*2){
    if(me%(2*gap) ==0){
      MPI_Recv(&other_sum, 1, MPI_INT, me+gap, 0, comm, MPI_STATUS_IGNORE);
    sum= other_sum+sum;
    if( (me % (4*gap)) !=0 && me>=2*gap)
      MPI_Send(&sum, 1, MPI_INT, me -2*gap, 0, comm);
    }
  }
  if(numProc%2 == 1) {  // If odd num Processors, the last value is added afterwards
    MPI_Recv(&other_sum, 1, MPI_INT, numProc -1, 0, comm, MPI_STATUS_IGNORE);
    sum= other_sum+sum;
  }

  return my_sum;
} /* Global_sum */
