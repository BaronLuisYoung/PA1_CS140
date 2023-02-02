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

  int gap = 1;
  int sum = my_int;
  while(my_rank % (gap*2) == 0 && gap < comm_sz) {
    int other_sum;
    if(my_rank+gap < comm_sz) {
      MPI_Recv(&other_sum, 1, MPI_INT, my_rank+gap, 0, comm, MPI_STATUS_IGNORE);
      sum+=other_sum;
    }
    gap*=2;      
  }
  if(my_rank != 0) {
    MPI_Send(&sum, 1, MPI_INT, my_rank - gap, 0, comm);
  }
  return sum;
} /* Global_sum */
