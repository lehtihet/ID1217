
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int gridSize, numIters;
double** grid;
double** new;


double jacobi() {

  for (int n = 0; n < numIters; n++) {

    // Compute new values for all interior points
    for (int i = 1; i < gridSize-1; i++) {
      for (int j = 1; j < gridSize-1; j++) {
        new[i][j] = (grid[i-1][j] + grid[i+1][j] + grid[i][j-1] + grid[i][j+1]) * 0.25;
      }
    }

    // Compute values gain for interior points
    for (int i = 1; i < gridSize-1; i++) {
      for (int j = 1; j < gridSize-1; j++) {
        grid[i][j] = (new[i-1][j] + new[i+1][j] + new[i][j-1] + new[i][j+1]) * 0.25;
      }
    }


  }

  // Compute the maximum difference
  double *maxdiff = malloc(sizeof(double));
  *maxdiff = 0;
  double diff = 0;

  for (int i = 1; i < gridSize-1; i++) {
    for (int j = 1; j < gridSize-1; j++) {
      diff = fabs(grid[i][j]-new[i][j]);

      if (diff > *maxdiff) *maxdiff = diff;

    }
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

  FILE* fp = fopen("seqJacfiledata.out", "w");

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

  allocateGrids();
  initializeGrids();

  clock_t begin = clock();

  double finaldiff = jacobi();

  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  printGrid();
  printf("gridSize: %d, numIters: %d\n", gridSize-2, numIters);
  printf("Time: %g\n", time_spent);
  printf("MAX: %.10f\n", finaldiff);
  
	return 0;
}