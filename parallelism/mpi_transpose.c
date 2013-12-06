/*transpose2.c : multiprocess transposition of mxn matrix
  dagan harrington | lyons '13

  approach : scatter rows to p processors (because storage is rowmajor)
  and gather in a different order (ie scatter rows => gather cols).
  ??important to use barrier to let all p read before writing.

 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*block decomposition macros*/
#define BLOCK_LOW(id,p,n) ((id)*(n)/(p))
#define BLOCK_HIGH(id,p,n) (BLOCK_LOW((id)+1,p,n)-1)
#define BLOCK_SIZE(id,p,n) (BLOCK_LOW((id)+1,p,n)-BLOCK_LOW(id,p,n))
#define BLOCK_OWNER(index,p,n) ((p)*(index)+1)-1)/(n))
/****************************/

#define ROWS 8 /*remember to test rect matrix*/
#define COLS 4

#define DEBUG

void print_mat(int *A, int m, int n); 
int *transpose(int *A, int *B, int m, int n);
int *transpose2(int *A, int *B, int m, int n);

int main(int argc, char *argv[]) {
  int i,j;
  int id; /* process rank */
  int p; /* number of processes*/
  int *A=NULL, *B=NULL, *local_A=NULL, *local_B=NULL;
  

  
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  
   if (id == 0){
     
     A = malloc(ROWS*COLS*sizeof(int));
     B = malloc(COLS*ROWS*sizeof(int));
     
     int r,c;
     for (r=0; r<ROWS*COLS; r++)
       A[r] = r;
     /*build matrix and send it somewhere
       for partial transpose*/
     
     print_mat(A,ROWS,COLS);
  
   }
   int sub_size = BLOCK_SIZE(id,p,ROWS);
   local_A = malloc(sub_size*COLS*sizeof(int));
   local_B = malloc(sub_size*COLS*sizeof(int));
   MPI_Scatter((void *)A, sub_size*COLS,MPI_INT,
	       (void *)local_A,sub_size*COLS,
	       MPI_INT,0,MPI_COMM_WORLD);
#ifdef DEBUG
   printf("P(%d) : %d blocksize\n",id,sub_size);  
   print_mat(local_A,sub_size,COLS);
#endif
   
   local_B = transpose2(local_A,local_B,sub_size,COLS);
   
   for (i=0;i<sub_size*COLS;i++)
     printf("%3d", local_B[i]);
   printf("\n");

   print_mat(local_B,COLS,sub_size);
   
   /*gather as columns..*/
   MPI_Gather(local_B, sub_size*COLS, MPI_INT,
	      B, sub_size*ROWS, MPI_INT, 0, MPI_COMM_WORLD);

   
   if (id == 0)
     print_mat(B,COLS,ROWS);
   
   fflush (stdout);
   MPI_Finalize(); 
   
   return 0;
}


void print_mat(int *A, int m, int n){
  int i,j;
  printf("----------------------\n");
  for (i=0; i<m; i++){
    for (j=0; j<n; j++)
      printf("%3d", A[i*n+j]);
    printf("\n");
  }
  printf("----------------------\n");
}

int *transpose(int *A, int *B, int m, int n){
  /*input : A (dim mxn)
    return : A^T (dim nxm)*/
  int i,j;

  for (i=0;i<m;i++)
    for(j=i+1;j<n;j++){
      /*lower triangular*/
      B[n*j+i] = A[n*i+j];
      /*upper triangular*/
      B[n*i+j] = A[n*j+i];      
    }
  return B;


}

int *transpose2(int *A, int *B, int m, int n){
  int i,j;
  for (i=0;i<m;i++)
    for (j=0;j<n;j++)
      B[j*n+i] = A[i*n+j];
  return B;

}
