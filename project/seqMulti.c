
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


void jacobi(double** grid, double** new, int gridSize, int numIters) {

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

}

void restriction(double** fine, double** coarse, int gridSize) {
  // Restrict from fine (large size) to coarse (smaller size). gridSize is of the course grid
  int i, j;
  for (int x = 1; x < gridSize-1; x++) {
    i = x*2;
    for (int y = 1; y < gridSize-1; y++) {
      j = y*2;
      coarse[x][y] = fine[i][j]*0.5 + (fine[i-1][j] + fine[i][j-1] + fine[i][j+1] + fine[i+1][j])*0.125;
    }
  }
}

void interpolation(double** fine, double** coarse, int gridSizeF, int gridSizeC) {
  int i, j;
  // First, assign coarse grid points to corresponding fine grid points
  for (int x = 1; x < gridSizeC-1; x++) {
    i = x*2;
    for (int y = 1; y < gridSizeC-1; y++) {
      j = y*2;
      fine[i][j] = coarse[x][y];
    }
  }

  // Second, update the other fine grid points in columns that were just updated
  for (i = 2; i < gridSizeF-1; i += 2) {
    for (j = 1; j < gridSizeF-1; j += 2) { 
      // Start at column 1 instead of 2 (y=1; j=y*2) as in the first update
      fine[i][j] = (fine[i-1][j] + fine[i+1][j])*0.5;
    }
  }

  // Finally, update the rest of the fine grid points
  for (i = 1; i < gridSizeF-1; i++) {
    for (j = 2; j < gridSizeF-1; j += 2) { 
      fine[i][j] = (fine[i][j-1] + fine[i][j+1])*0.5;
    }
  }
}

double calcMaxDiff(double** grid, double** new, int gridSize) {
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


double** allocateGrid(int size) {

  double** grid = malloc(size * sizeof(double));
  for (int i = 0; i < size; i++) {
    grid[i] = malloc(size * sizeof(double));
  }

  return grid;

}

void initializeGrid(double** grid, int size) {

  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {


      if (i == 0 || j == 0 || i == size-1 || j == size-1) {
        // Boundary
        grid[i][j] = 1;
      } else {
        // Interior
        grid[i][j] = 0;
      }

      
    }
  }
}

void printGrid(double** grid, int gridSize) {

  FILE* fp = fopen("seqMultifiledata.out", "w");

  for (int i = 0; i < gridSize; i++) {
    for (int j = 0; j < gridSize; j++) {
      fprintf(fp, "%.7f ", grid[i][j]);
    }
    fprintf(fp, "\n");
  }
}

int main(int argc, char *argv[]) {

  int gridSize = atoi(argv[1])+2; // +2 to include boundaries, base grid size
  int numIters = atoi(argv[2]);
  int fineIters = 4;

  int gs2 = (gridSize-2)*2+1+2;
  int gs3 = (gs2-2)*2+1+2;
  int gs4 = (gs3-2)*2+1+2;

  double** grid8h = allocateGrid(gridSize);
  double** new8h = allocateGrid(gridSize);
  double** grid4h = allocateGrid(gs2);
  double** new4h = allocateGrid(gs2);
  double** grid2h = allocateGrid(gs3);
  double** new2h = allocateGrid(gs3);
  double** grid1h = allocateGrid(gs4);
  double** new1h = allocateGrid(gs4);


  initializeGrid(grid8h, gridSize);
  initializeGrid(new8h, gridSize);
  initializeGrid(grid4h, gs2);
  initializeGrid(new4h, gs2);
  initializeGrid(grid2h, gs3);
  initializeGrid(new2h, gs3);
  initializeGrid(grid1h, gs4);
  initializeGrid(new1h, gs4);


  clock_t begin = clock();

  jacobi(grid1h, new1h, gs4, fineIters);
  restriction(grid1h, grid2h, gs3);
  jacobi(grid2h, new2h, gs3, fineIters);
  restriction(grid2h, grid4h, gs2);
  jacobi(grid4h, new4h, gs2, fineIters);
  restriction(grid4h, grid8h, gridSize);
  jacobi(grid8h, new8h, gridSize, numIters);

  interpolation(grid4h, grid8h, gs2, gridSize);
  jacobi(grid4h, new4h, gs2, fineIters);
  interpolation(grid2h, grid4h, gs3, gs2);
  jacobi(grid2h, new2h, gs3, fineIters);
  interpolation(grid1h, grid2h, gs4, gs3);
  jacobi(grid1h, new1h, gs4, fineIters);

  double finaldiff = calcMaxDiff(grid1h, new1h, gs4);
  

  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  printGrid(grid1h, gs4);
  printf("gridSize: %d, numIters: %d\n", gridSize-2, numIters);
  printf("Time: %g\n", time_spent);
  printf("MAX: %.10f\n", finaldiff);
  
	return 0;
}