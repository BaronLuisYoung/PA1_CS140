
/*
 * File:     prog32_pi_mpi.c
 *
 * Purpose:  Estimate pi using a monte carlo method
 *
 * IPP book:      Programming Assignment 3.2
 */

#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

/*-------------------------------------------------------------------
 * Estimate Pi in parallel using a monte carlo method
 * Only process 0 returns the correct pi estimation
 * In args:
 * 	number_of_tosses: No of tosses.
 *      my_rank: my process rank
 *      no_proc: number of processes
 *      comm: MPI communication group
 * Return: estimated pi
 */

double parallel_pi(long long int number_of_tosses, int my_rank, int no_proc,
                   MPI_Comm comm) {
  double pi_estimate = 0.0;
 
  double x, y;
  double distance_squared;
  long long int number_in_circle = 0;
  long long int sum; 
  for (i = 0; i < (number_of_tosses/no_proc); i++) {
    x = 2 * random() / ((double)RAND_MAX) - 1.0;
    y = 2 * random() / ((double)RAND_MAX) - 1.0;
    distance_squared = x * x + y * y;
    if (distance_squared <= 1) {
      number_in_circle++;
    }
  }
  MPI_Reduce(&number_in_circle, &sum, 1, MPI_INT, MPI_SUM, 0, comm);

  pi_estimate = 4 * number_in_circle / ((double)number_of_tosses);
  return pi_estimate; 
}
