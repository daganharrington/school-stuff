/*****************
Dagan Harrington | Dr. Lyons | Parallel Computation | Fordham
mpi_monte.c -> monte carlo pi estimation using
MPI_Broadcast and MPI_Reduce.

# of generated points is GEN_BOUND

version : this version uses reduce() to collect
local_count and then derive  pi_estimate in id==0

 ****************/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "WELL1024a.h"

/*block decomposition macros*/
#define BLOCK_LOW(id,p,n) ((id)*(n)/(p))
#define BLOCK_HIGH(id,p,n) (BLOCK_LOW((id)+1,p,n)-1)
#define BLOCK_SIZE(id,p,n) (BLOCK_LOW((id)+1,p,n)-BLOCK_LOW(id,p,n))
#define BLOCK_OWNER(index,p,n) ((p)*(index)+1)-1)/(n))
/****************************/
#define GEN_BOUND 100000


int main(int argc, char *argv[]) {
  int id; /* process rank */
  int p; /* number of processes*/
  long long unsigned int n = 0, i=0,global_count=0;/*n=num of darts*/

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  if (id == 0){
    n = GEN_BOUND;/*a bit redundant but experimenting*/
    printf("id=0 and n = %llu\n",n);
    /*would like n=atoll(argv[1]) but having long long problems*/

  }

  MPI_Bcast(&n, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);

  /*srand() candidates : time(NULL), /dev/urandom, id*/
  srand(id);
  double x,y,distance_squared,local_pi;
  long long unsigned int local_count=0, inneriter=0;
  for (i=BLOCK_LOW(id,p,n);i<BLOCK_HIGH(id,p,n);i++){
    x = rand()  / (RAND_MAX*0.5) - 1.0;
    y = rand()  / (RAND_MAX*0.5) - 1.0;
    /*printf("%.5f %.5f\n",x,y);*/
    inneriter++;
    distance_squared = x*x + y*y;
    if (distance_squared <= 1.0) local_count++;
  }
  
  /*local_pi = 4*local_count/((double)BLOCK_SIZE(id,p,n));
    printf("P(%d) : iters(%llu) : area=%.9f\n",
    id,inneriter,local_pi);*/
   
  MPI_Reduce(&local_count, &global_count, 1, 
	     MPI_UNSIGNED_LONG, MPI_SUM,0,MPI_COMM_WORLD);

  if (id == 0)
    printf("full pi estimate given %d processes : %.9f\n",
	   p,4*global_count/((double)n));

  fflush (stdout);
  MPI_Finalize();

  
  return 0;
}
