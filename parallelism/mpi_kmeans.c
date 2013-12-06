/*************************************************************
Dagan Harrington | Dr. Lyons | Parallel Computation | Fordham
mpi_kmeans.c -> parallel KMeans implementation
Alg :
  1-Scatter N/p points to p processors
  2-Broadcast K points to p processors
  3-Each processor : 
      a-Calculate K $localsums of points closest to each k <- K
      b-Maintain local $count for how many local points
          belong to each cluster.
      c-Reduce $localsums and $count to Global (note1)
      d-Calculate new means
  -If clusters same as last clusters, break
  -else, goto 3 (goto statement considered harmful)
****************************************************************/
/*
  note1-
    reductions : pairwise averaging commutes but is not associative:
    so ((a+b)/2 + (c+d)/2)/2 != (a+b+c+d)/4
    so cannot compute local averages ...
    need to keep local sums and local counts...reduce each separately,
    then for each cluster : 
      (add all sums) / (add all counts) = cluster centroid
  note2-
    not a generalized solution:
      preconditions :
        -N `mod` p == 0
      also not generalized for n-dimensions.
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/*block decomposition macros*/
#define BLOCK_LOW(id,p,n) ((id)*(n)/(p))
#define BLOCK_HIGH(id,p,n) (BLOCK_LOW((id)+1,p,n)-1)
#define BLOCK_SIZE(id,p,n) (BLOCK_LOW((id)+1,p,n)-BLOCK_LOW(id,p,n))
#define BLOCK_OWNER(index,p,n) ((p)*(index)+1)-1)/(n))
/****************************/

#define K 5 /*number of clusters*/
#define N 100000 /*number of random points to generate as problem space*/

#define _PRINTINITIAL 0 /*print initial space and means?*/
#define _DEBUG 0

/*return an array of points (x,y) | x,y <- INTS */
void Init_2space(double *space, int n);

/*return an empty array of (x,y) for initial changed(a1,a2) call*/
void Zero_space(double *space, int n);

/*copy a 2space*/
void Copy_2space(double *space, double *copyspace, int n);

/*initialize K random points as cluster means*/
void Init_means(double *means, double *space, int k);

/*recalculate means given a cluster
 modify summeans internally
 return counts*/
double *Recalc_means(double *space, double *means,
		     double *summeans, double *counts, int n, int k); 

/*return cluster mean with minimum euclidean distance from x,y*/
double *Min_euclid_dist(double *means, double *mincoords,
			int k, double x, double y);

/*have the clusters changed? 0 if no, 1 if yes:
  precondition : newclusters and oldclusters same size*/
int Changed(double *newclusters, double *oldclusters, int k);

/*char t is an identifier for the type of print*/
void Print(char t, double *space, int n);


int main(int argc, char *argv[]) {
  int id; /* process rank */
  int p; /* number of processes*/
  int i;

  /*allocate contiguous memory on all processes*/
  double *space     =   (double *)malloc(N*2*sizeof(double));  
  double *newclusters  =   (double *)malloc(K*2*sizeof(double));
  double *clusters     =   (double *)malloc(K*2*sizeof(double));
  double *summeans  =   (double *)malloc(K*2*sizeof(double));
  double *summeansG =   (double *)malloc(K*2*sizeof(double));
  double *counts    =   (double *)malloc(K*sizeof(double));
  double *countsG   =   (double *)malloc(K*sizeof(double));
  
  /*only measuring times within loop as they are dominant*/
  double ct1_start,ct1_end,ct_total=0; /*computation time*/
  double mt1_start,mt1_end,mt_total=0; /*message communication time*/

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  
  if (id == 0){
    printf("P(%d)\n",id);
    /*initialize random 2space and K mean values*/
    Init_2space(space,N);
    Init_means(newclusters,space,K);
   }
  
  if (_PRINTINITIAL)
    {
      if (id ==0) {
	Print('P',space,N);
	Print('C',newclusters,K);
	fflush(stdout);
      }
    }

  double local_space[N/p*2]; /*toheap or nottoheap*/
  
  MPI_Scatter(space, N/p*2, MPI_DOUBLE, 
	      &local_space, N/p*2, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  
  Zero_space(clusters,K);
  
  int steps=0;
  do {
    ct1_start = MPI_Wtime();
    Copy_2space(newclusters,clusters,K);
    ct1_end = MPI_Wtime();
    ct_total+=ct1_end - ct1_start;
    
    mt1_start = MPI_Wtime();
    MPI_Bcast(&clusters[0], K*2,
	      MPI_DOUBLE, 0, MPI_COMM_WORLD);
    mt1_end = MPI_Wtime();
    mt_total+=mt1_end - mt1_start;
    
    ct1_start = MPI_Wtime();
    /****************************************/
    if (_DEBUG){    
      Print('S',local_space,N/p);
      fflush(stdout);
      Print('M',clusters,K);
      fflush(stdout);
    }
    /****************************************/
    
    for (i=0;i<K*2;i++)
      summeans[i]=0.0;
    for (i=0;i<K;i++)
      counts[i]=0.0;
    
    counts=Recalc_means(local_space,clusters,summeans,counts,N/p,K);
    
    ct1_end = MPI_Wtime();
    ct_total+=ct1_end - ct1_start;

    mt1_start = MPI_Wtime();
    MPI_Reduce(&summeans[0], &summeansG[0], K*2, MPI_DOUBLE, MPI_SUM,
	       0, MPI_COMM_WORLD);
    MPI_Reduce(&counts[0],  &countsG[0], K, MPI_DOUBLE, MPI_SUM,
	       0, MPI_COMM_WORLD);
    mt1_end = MPI_Wtime();
    mt_total+=mt1_end - mt1_start;
    
    ct1_start = MPI_Wtime();
    if (id==0){
      steps++;
      for (i=0;i<K;i++){
	if (countsG[i] == 0) //no div by zero!
	  {
	    newclusters[i*2]=0.0;
	    newclusters[i*2+1]=0.0;
	  }
	else
	  {
	    newclusters[i*2]=summeansG[i*2]/countsG[i];
	    newclusters[i*2+1]=summeansG[i*2+1]/countsG[i];
	  }
      }
      /**************************************************************************/
      if (_DEBUG){
	Print('X',summeansG,K);
	for (i=0;i<K;i++){
	  printf("Both %11.3f and %11.3f div by ", summeansG[i*2],summeansG[i*2+1]);
	  printf("%3.1f\n",countsG[i]);
	  printf("equals %11.3f and %11.3f, respectively\n",summeansG[i*2]/countsG[i],
		 summeansG[i*2+1]/countsG[i]); 
	}
	
	Print('F',newclusters,K);
      }
      /***************************************************************************/
    }//END ID=0
    ct1_end = MPI_Wtime();
    ct_total+=ct1_end - ct1_start;
    
    mt1_start = MPI_Wtime();
    MPI_Bcast(&newclusters[0], K*2,
	      MPI_DOUBLE, 0, MPI_COMM_WORLD);
    mt1_end = MPI_Wtime();
    mt_total+=mt1_end - mt1_start;
    
    if (id == 0)
      printf("comp = %5.5f | mess = %5.5f\n", ct_total, mt_total);
    
  }while (Changed(newclusters,clusters,K));
  
  
  //free() all the things
  if (id ==0){
    printf("Final Means after %d steps:\n", steps-1);//-1 because of do/while
    Print('F',newclusters,K);
    fflush (stdout);}
  MPI_Finalize();
  
  return 0;
}

/********methods*********************************************/
void Init_2space(double *space, int n) {
  int i;
  /*every 2 indices spans a UNIT = (x,y)
   */
  srand(time(NULL)*2); /*2 to make seed differ from other srand call*/

  for (i=0;i<n;i++){
    space[2*i]   = (double)rand();
    space[2*i+1] = (double)rand();
    //space[2*i]   = (double)(i+1)*10;
    //space[2*i+1] = (double)(i+1)*3;
  }
}

void Zero_space(double *space, int n){
  /*zero a 2space*/
  int i;
  for (i=0;i<2*n;i++)
    space[i]   = 0.0;
}

void Copy_2space(double *space, double *copyspace, int n){
  //double *copyspace=malloc(n*2*sizeof(double));
  //n = n/p from function call
  int i;
  for (i=0;i<n;i++){
    copyspace   [2*i] = space[2*i];
    copyspace [2*i+1] = space[2*i+1];
  }
  //return copyspace;  
}

void Init_means(double *means,double *space, int k){
  int i;
  /* initialize first K means to be first K points of space */
  /* precondition...N >= 2*K */
  for (i=0;i<k;i++){
    means   [2*i] = space [2*i];
    means [2*i+1] = space [2*i+1];
    // means   [2*i] = (double) rand();
    //means [2*i+1] = (double) rand();
  }
}

double *Min_euclid_dist(double *means, double *mincoords,
			int k, double x, double y){
  /* which of the means provided is a minimum distance to x,y */
  int i;
  double dist, min;

  /*INIT first mean as the closest*/
  min = sqrt(pow(fabs(x-means[0]),2)+(pow(fabs(y-means[1]),2)));
  mincoords[0]=means[0];
  mincoords[1]=means[1]; 

  /*iterate through list of means to find minimum euclidean distance*/
  for (i=1;i<k;i++){
    dist=sqrt(pow((fabs(x-means[i*2])),2)+
      (pow(fabs(y-means[i*2+1]),2)));
    if (dist < min){
      min = dist;
      mincoords[0]=means[i*2];
      mincoords[1]=means[i*2+1];
    }
  }
  return mincoords;
}

double *Recalc_means(double *space, double *means,
		     double *summeans, double *counts, int n, int k){
  /*
     outer loop : iterate through means
       inner loop : for each mean M,
           pointwise sum all points
	   for which M <- means is a min
	   also maintain a count

	   replace current
	   return counts[K]
   */
  /*missing - a check that always K clusters*/
  int i,j;
  double sum_x=0,sum_y=0;
  int count;
  double *mincoords = malloc(2*sizeof(double));

  for (i=0;i<k;i++){//for each cluster mean
    sum_x=0.0;
    sum_y=0.0;
    count=0;
    for (j=0;j<n;j++){//for every point in the space
      mincoords=Min_euclid_dist(means,mincoords,k,
				space[j*2],space[j*2+1]);
      if (means[i*2] == mincoords[0] &&
	  means[i*2+1] == mincoords[1]){
	sum_x+=space[j*2];
	sum_y+=space[j*2+1];
	count++;
      }//end-if
    }//end for j
    summeans[i*2]   = sum_x;
    summeans[i*2+1] = sum_y;
    counts[i] = (double) count;

  }//end for i
  return counts;
}

int Changed(double *newclusters, double *oldclusters, int k){
  int i;
  for (i=0;i<k;i++){
    if (newclusters[2*i]   != oldclusters[2*i]   ||
	newclusters[2*i+1] != oldclusters[2*i+1] ){
      //flag++; /* this scans whole list .. only need to find one diff : modify */
      return 1;
    }
  }
  //return flag>0;
  return 0;
}

void Print(char t, double *space, int n){ /*print a 2space : x y*/
  int i;
  printf("print %c :\n", t);
  for (i=0;i<n;i++)
    printf("| %13.1f %13.1f |\n",
	   space[i*2],space[i*2+1]);
}
