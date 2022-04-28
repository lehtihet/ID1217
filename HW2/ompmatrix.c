
#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

#define MAXSIZE 10000  /* maximum matrix size */
#define MAXWORKERS 8   /* maximum number of workers */

int numWorkers;
int size; 
int matrix[MAXSIZE][MAXSIZE];
double start_time, end_time;

/* read command line, initialize, and create threads */
int main(int argc, char *argv[]) {
  int i, j, totalZeroes=0, totalSymmetri=1;
  

  /* read command line args if any */
  size = (argc > 1)? atoi(argv[1]) : MAXSIZE;
  numWorkers = (argc > 2)? atoi(argv[2]) : MAXWORKERS;
  if (size > MAXSIZE) size = MAXSIZE;
  if (numWorkers > MAXWORKERS) numWorkers = MAXWORKERS;

printf("numworkers: %d\n", numWorkers);
  int localZeroes[numWorkers];
  int localSymmetri[numWorkers];

  omp_set_num_threads(numWorkers);

  // Verify program functionallity (symmetri with 3 zeroes):
  /*
  matrix[0][0] = 0; matrix[0][1] = 2; matrix[0][2] = 5;
  matrix[1][0] = 2; matrix[1][1] = 7; matrix[1][2] = 0;
  matrix[2][0] = 5; matrix[2][1] = 0; matrix[2][2] = 4;
  */

  /* initialize the matrix */
  for (i = 0; i < size; i++) {
      //printf("[ ");
	  for (j = 0; j < size; j++) {
      matrix[i][j] = rand()%99;
      //printf(" %d", matrix[i][j]);
	  }
	  	//printf(" ]\n");
  }

  start_time = omp_get_wtime();
#pragma omp parallel
{
  int id = omp_get_thread_num();
  localZeroes[id] = 0;
  localSymmetri[id] = 1;


#pragma omp for private(j)
  for (i = 0; i < size; i++)
    for (j = 0; j < size; j++){
      if (matrix[i][j] == 0)
        localZeroes[id]++;
      if (matrix[j][i] != matrix[i][j])
        localSymmetri[id] = 0;
    }
}

end_time = omp_get_wtime();


int k;
for (k = 0; k < numWorkers; k++) {
  totalZeroes += localZeroes[k];
  totalSymmetri &= localSymmetri[k];
}

  printf("Zeroes: %d\n", totalZeroes);
  printf("Symmetri: %d\n", totalSymmetri);
  printf("it took %g seconds\n", end_time - start_time);

}

