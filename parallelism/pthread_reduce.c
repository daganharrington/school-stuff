/* dgn - '13
   pthreads implementation of reduction over array data structure
   assumption #1 : a[N] where N is a power of 2
   assumption #2 : as many threads as necessary are available

   improvement :
      generic version - reduction takes a function pointer
      so reduce includes arbitrary binary associative ops.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define N 32768

/* Global variable : accessible to all threads */
long thread_count;
int a[N];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

struct Thread_params {
  long thread;
  int level;
};


void *Reduce(void* rank); /* thread function */

int main(int argc, char** argv)
{
  long thread; /* Use long in case of a 64-bit system */
  pthread_t* thread_handles;  
  int i;
  
  /* Get # of threads from the command line */
  thread_count = N / 2; //init
  int depth = log (N) / log (2);//height of binary tree
  
  /*populate array*/
  for (i=0;i<N;i++)
    a[i]=i+1;
  
  struct Thread_params *params=NULL;
  thread_handles = malloc (thread_count*sizeof(pthread_t));
  
  for (i=1; i<= depth; i++)
    {
      //printf("depth = %d\nnum proc needed = %ld\n",i-1,thread_count);
      
      for (thread =  0; thread < thread_count; thread++){
	params = malloc(sizeof(struct Thread_params));
	params->thread = thread;
	params->level = i;
	if (pthread_create(&thread_handles[thread], NULL,
			   Reduce, (void*) params))
	  {printf("nooooooo!!!\n"); return 42;}
      }
      for (thread = 0; thread < thread_count; thread++)
	pthread_join(thread_handles[thread], NULL);
      thread_count = thread_count / 2;
      free(params);
    }
  
  printf("\nsum [1..%d]=%d\n",N, a[0]);
  free(thread_handles); 
  pthread_mutex_destroy(&lock);
  
  return 0;
}

void *Reduce(void* params) {
  //struct Thread_params *my_params = params;
  struct Thread_params* my_params = (struct Thread_params*) params;
  int level = my_params->level;
  long thread = my_params->thread;
  
  /*remove mutex and get slower wall clock times=strange*/
  pthread_mutex_lock(&lock);
  a[thread*(int)(pow(2,level))] = a[thread*(int)(pow(2,level))] +
    a[thread*(int)(pow(2,level)) + (int)(pow(2,level-1))];
  pthread_mutex_unlock(&lock);
  
  fflush(stdout);
  return NULL;
}
