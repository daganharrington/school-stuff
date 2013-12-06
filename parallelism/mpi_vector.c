/*********************************************************#
 ---------------------------------------------------------
 Dagan Harrington | Dr. Lyons | Parallel Computation | Fordham
 mpi_vector.c -> Vector summation using MPI_Scatter and MPI_Gather

 An array of size VECSIZE, type Int is populated.
 Process 0 scatters portions of the array to
 processes 1 to (p-1) where p is the total # of processes.
    Precondition : VECSIZE mod p == 0
                   for block partitioning
 Sums are then Gathered and pieced together (?)

 Using MPI_Wtime(), total computation time is tracked.

 *********************************************************/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#define VECSIZE 100

int main(int argc, char *argv[]) {
  
  int     *a1=NULL, *a2=NULL, *result=NULL, i,
    id /*process rank*/, p /*number of processes*/;

  /*MPI Initializations*/
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  printf("from P(%d)\n", id);
  
  /*process 0 populates entire arrays[VECSIZE]*/
  /*should probably free() here?*/
  if (id == 0){
    a1 = malloc(VECSIZE*sizeof(int));
    a2 = malloc(VECSIZE*sizeof(int));
    result = malloc(VECSIZE*sizeof(int));
    for (i=0; i < VECSIZE; i++){
      a1[i] = (i+1);
      a2[i] = (i+1)*10;
    }
    for (i=0;i<VECSIZE;i++)
      printf("a1[%d] = %d  || a2[%d] = %d\n",
	     i,a1[i],i,a2[i]);
  }
  /*local array initialization, scatter and gather occurs for all p*/
  /*---------------------------------------------------------------*/
  int local_a1[VECSIZE/p], local_a2[VECSIZE/p]; /*possiblywrongprobablyright*/
  int local_result[VECSIZE/p];
  
  MPI_Scatter(a1, VECSIZE/p, MPI_INT, &local_a1, VECSIZE/p, 
	      MPI_INT, 0, MPI_COMM_WORLD); 
  MPI_Scatter(a2, VECSIZE/p, MPI_INT, &local_a2, VECSIZE/p, 
	      MPI_INT, 0, MPI_COMM_WORLD); 

  for (i=0;i<VECSIZE/p;i++){
    /*perform the vector addition*/
    local_result[i] = local_a1[i] + local_a2[i];
    /*pre-gather print check on results*/
    printf("P(%d):a1[%d]=%d:a2[%d]=%d:r[%d]=%d\n",
	   id, i, local_a1[i], i, local_a2[i], i, local_result[i]);
  }
  
  MPI_Gather(&local_result, VECSIZE/p, MPI_INT,
	     result, VECSIZE/p, MPI_INT, 0, MPI_COMM_WORLD);

  if (id == 0){
    for (i=0;i<VECSIZE;i++)
      printf("%d ", result[i]);
    printf("\n");
  }

  fflush (stdout);
  MPI_Finalize(); 
  /*********************/  

  return 0;
}
