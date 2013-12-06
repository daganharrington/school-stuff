/* dgn - '13
  p-threads monte carlo pi estimate.
  usage : pthread_monte <numproc> <numpoints>
  will actually generate numproc*numpoints points.

  reentrant prng erand48_r() used..
  thought not sure about quality of prandomness??
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* Global variable : accessible to all threads */
int thread_count, t_entered_critical;
long long unsigned int num_points, points_in_circle=0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

/*prng associated variables, man drand48_r*/
unsigned short xsubi[3];
struct drand48_data buffer;
double result; //prng result

void *Pisum(void* rank); /* thread function */

int main(int argc, char** argv)
{
  long thread; /* Use long in case of a 64-bit system */
  pthread_t* thread_handles;
  
  /*error checking for argc*/
  if (argc!=3) {
    fprintf(stderr,"usage : pthreads_monte <num of threads> <num of points>\n"); 
    return -1;
  }
  /* Get # of threads from the command line */
  thread_count = strtol(argv[1], NULL, 10);
  num_points = strtol(argv[2], NULL, 10);
  
  printf("num_points = %llu\n",num_points);

  /*seed the prng_r*/
  seed48_r(xsubi,&buffer);

  /*allocate and create threads*/
  thread_handles = malloc (thread_count*sizeof(pthread_t));
  for (thread =  0; thread < thread_count; thread++)
    pthread_create(&thread_handles[thread], NULL,
		   Pisum, (void*) thread);
  
  for (thread = 0; thread < thread_count; thread++)
    pthread_join(thread_handles[thread], NULL);
  
  double pi_estimate = 
    4*(double)points_in_circle/(double)(num_points*thread_count);
  printf("(4 * %llu)/(%llu * %d) = %1.10f \n", points_in_circle,
	 num_points, thread_count, pi_estimate);
  
  
  pthread_mutex_destroy(&lock);
  pthread_mutex_destroy(&lock2);
  free(thread_handles);
  return 0;
}

void *Pisum(void* rank) {
  
  long my_rank = (long) rank;
  
  double x,y,distance_squared;
  long long unsigned int local_count=0;
  int i;
  
  printf("rank(%ld)\n",my_rank);
  /*  erand48 prng produces doubles on [0.0,1.0) = result
      need to renormalize on (-1,1)*/
  for (i=0;i<num_points;i++){
    erand48_r(xsubi, &buffer, &result);
    x = result*2-1;
    erand48_r(xsubi, &buffer, &result);
    y = result*2-1;

    distance_squared = x*x + y*y;
    //printf("distance_squared : %3.9f\n", distance_squared);
    if (distance_squared <= 1.0) local_count++;
  }
  
  printf("local count = %llu\n", local_count);
 
  pthread_mutex_lock(&lock);
  points_in_circle+=local_count;
  pthread_mutex_unlock(&lock);
  
  fflush(stdout);
  return NULL;
}
