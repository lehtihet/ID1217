
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

int gridSize, numIters, numWorkers;
double** grid;
double** new;

double jacobi() {

  int i, j;
  for (int n = 0; n < numIters; n++) {

    #pragma omp parallel for private(j) 
      for (i = 1; i < gridSize-1; i++) { // Compute new values for all interior points
        for (j = 1; j < gridSize-1; j++) {
          new[i][j] = (grid[i-1][j] + grid[i+1][j] + grid[i][j-1] + grid[i][j+1]) * 0.25;
        }
      }
      // Have to wait for all threads to terminate so grid isn't 
      // updated while being used in the previous for loop

    #pragma omp parallel for private(j) 
      for (i = 1; i < gridSize-1; i++) { // Compute values gain for interior points
        for (j = 1; j < gridSize-1; j++) {
          grid[i][j] = (new[i-1][j] + new[i+1][j] + new[i][j-1] + new[i][j+1]) * 0.25;
        }
      }

  }

  // Compute the maximum difference
  double *maxdiff = malloc(sizeof(double)); // To be returned
  *maxdiff = 0;
  double diff;
  int id;
  double maxList[numWorkers];

  #pragma omp parallel private(diff, j, id)
  {
    id = omp_get_thread_num();
    maxList[id] = 0;
    diff = 0;

    #pragma omp for
      for (i = 1; i < gridSize-1; i++) {
        for (j = 1; j < gridSize-1; j++) {
          diff = fabs(grid[i][j]-new[i][j]);

          if (diff > maxList[id]) maxList[id] = diff;

        }
      }
  }

  for (i = 0; i < numWorkers; i++) {
    if (maxList[i] > *maxdiff) *maxdiff = maxList[i];
  }



  return *maxdiff;

}


void allocateGrids() {

  grid = malloc(gridSize * sizeof(double));
  new = malloc(gridSize * sizeof(double));
  for (int i = 0; i < gridSize; i++) {
    grid[i] = malloc(gridSize * sizeof(double));
    new[i] = malloc(gridSize * sizeof(double));
  }

}

void initializeGrids() {

  for (int i = 0; i < gridSize; i++) {
    for (int j = 0; j < gridSize; j++) {


      if (i == 0 || j == 0 || i == gridSize-1 || j == gridSize-1) {
        // Boundary
        grid[i][j] = 1;
        new[i][j] = 1;
      } else {
        // Interior
        grid[i][j] = 0;
        new[i][j] = 0;
      }

      
    }
  }
}

void printGrid() {

  FILE* fp = fopen("parJacfiledata.out", "w");

  for (int i = 0; i < gridSize; i++) {
    for (int j = 0; j < gridSize; j++) {
      fprintf(fp, "%.7f ", grid[i][j]);
    }
    fprintf(fp, "\n");
  }
}

int main(int argc, char *argv[]) {

  gridSize = atoi(argv[1]) + 2; // To include boundaries
  numIters = atoi(argv[2]);
  numWorkers = atoi(argv[3]);

  allocateGrids();
  initializeGrids();
  omp_set_num_threads(numWorkers);

  double begin = omp_get_wtime();

  double finaldiff = jacobi();

  double end = omp_get_wtime();
  double time_spent = (end - begin);

  printGrid();
  printf("gridSize: %d, numIters: %d, numWorkers: %d\n", gridSize-2, numIters, numWorkers);
  printf("Time: %g\n", time_spent);
  printf("MAX: %.10f\n", finaldiff);
  
	return 0;
}