
/*
 * File:     treesum_test_mpi.c
 *
 * Purpose:  test tree-structured communication to find the global sum
 *           of a random collection of ints.  This version doesn't
 *           require that no of processes  be a power of 2.
 *
 * Compile:  mpicc -Wall -o treesum_test_mpi  treesum_test_mpi.c treesum_mpi.c
 * minunit.c Run:      ibrun  -v ./treesum_test_mpi
 *
 * IPP:      Programming Assignment Text Book Problem 3.3 Page 148
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "minunit.h"

int my_rank, no_proc;
MPI_Comm comm;

int global_sum(int my_int, int my_rank, int no_proc, MPI_Comm comm);

/*-------------------------------------------------------------------
 * Test global_sum()
 * If successful, return NULL
 */
char *treesum_test1() {
  /* Your solution */
  int values[5] = {1, 1, 1, 1};

  MPI_Scatter(&values, 1, MPI_INT, &values, 1, MPI_INT, 0, comm);
  int result = global_sum(values[my_rank], my_rank, no_proc, comm);

  // MPI_Gather = (values[my_rank])
  // printf("\n\nResult: %d\n\n", result);
  int summation = 0;
  int i;
  for(i=0; i<no_proc;i++){
    summation+=values[i];
  }

  if(my_rank == 0) {
  // printf("sum: %d\n",summation);
  // printf("Result: %d\n", result);
  // printf("Condition: %d\n", result == summation);
  mu_assert("Failed Test Case!", result == summation);
  return 0;
  }
}

/*-------------------------------------------------------------------
 * Run all tests.  Ignore returned messages.
 */
void run_all_tests(void) { mu_run_test(treesum_test1); }

/*-------------------------------------------------------------------
 * The main entrance to run all tests.
 * Only Proc 0 prints the test summary
 */
int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_size(comm, &no_proc);
  MPI_Comm_rank(comm, &my_rank);

  run_all_tests();

  if (my_rank == 0) {
    mu_print_test_summary("Summary:");
  }
  MPI_Finalize();
  return 0;
}
