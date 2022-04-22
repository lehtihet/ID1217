#ifndef _REENTRANT
#define _REENTRANT
#endif
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_WIDTH 10
#define MAX_HEIGHT 10


pthread_mutex_t lock;
int num_threads;

int *matrix;
int matrix_width;
int matrix_height;
int matrix_length;

int totalZeroes = 0;
int symmetric = 1; // 1 at start important for &-updates

void fillMatrix() { // y*width+x
	*(matrix+0*matrix_width+0) = 0; *(matrix+0*matrix_width+1) = 2; *(matrix+0*matrix_width+2) = 5;
	*(matrix+1*matrix_width+0) = 2; *(matrix+1*matrix_width+1) = 7; *(matrix+1*matrix_width+2) = 0;
	*(matrix+2*matrix_width+0) = 5; *(matrix+2*matrix_width+1) = 0; *(matrix+2*matrix_width+2) = 4;

	printf("%d, %d, %d, \n%d, %d, %d, \n%d, %d, %d\n", 
		matrix[0], matrix[1], matrix[2], matrix[3], matrix[4], matrix[5], matrix[6], matrix[7], matrix[8]);

}

void *numZeroes(void *arg) {

	int* th_id = (int*)arg;
	int localZeroes = 0;
	int localSymmetri = 1;

	int element;
	int x;
	int y;

	for (int i = (*th_id)*matrix_length/num_threads; i < ((*th_id)+1)*matrix_length/num_threads; i++) {
		element = *(matrix+i);
		if (element == 0) {
			localZeroes++;
		}
		y = i / matrix_width;
		x = i % matrix_width;
		if (element != *(matrix+x*matrix_width+y)) { // Check transposition
			localSymmetri = 0;
		}
	}


	pthread_mutex_lock(&lock);
	totalZeroes += localZeroes;
	symmetric &= localSymmetri;
	pthread_mutex_unlock(&lock);

	return NULL;

}

int main (int argc, char *argv[]) {

	matrix_width = atoi(argv[1]) <= MAX_WIDTH ? atoi(argv[1]) : MAX_WIDTH;
	matrix_height = atoi(argv[2]) <= MAX_HEIGHT ? atoi(argv[2]) : MAX_HEIGHT;
	matrix_length = matrix_width*matrix_height;

	matrix = (int*)malloc(sizeof(int)*matrix_length);
	fillMatrix();

	num_threads = 3;
	int i, id[num_threads];


	pthread_t tid[num_threads];
	pthread_mutex_init(&lock, NULL);



	for (i = 0; i < num_threads; i++) {
		id[i] = i;
		pthread_create(&tid[i], NULL, numZeroes, (void*)(id + i));
	}
	
	for (i = 0; i < num_threads; i++) {
		pthread_join(tid[i], NULL);
	}

	printf("Symmetric (0=no,1=yes):		%d\nTotal amount of zeroes:		%d\n", symmetric, totalZeroes);

	free(matrix);

	return 0;
}