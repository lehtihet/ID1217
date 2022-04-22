#ifndef _REENTRANT
#define _REENTRANT
#endif
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct arg_struct {
	char * fn;
	int thread_id;
};

pthread_cond_t cond;
pthread_mutex_t lock;
int turn = 1;
int fileEnd = 0;


void *printFile(void *arguments) {


	struct arg_struct *args = arguments;

	char *fileName = args -> fn; 
	int th_id = args -> thread_id;


	FILE *fptr;
	fptr = fopen(fileName, "r");

	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	while ((read = getline(&line, &len, fptr)) != -1) {
		if (fileEnd) {
			printf("%s", line);
			break;
		}
		pthread_mutex_lock(&lock);
		if (turn != th_id) {
			pthread_cond_wait(&cond, &lock);
		}
		printf("%s", line);
		turn = (th_id == 1) ? 2 : 1;
		pthread_mutex_unlock(&lock);
		pthread_cond_signal(&cond);
	}

	while ((read = getline(&line, &len, fptr)) != -1) {
		printf("%s", line);
	}


	pthread_mutex_lock(&lock);
	fileEnd = 1;
	pthread_mutex_unlock(&lock);
	pthread_cond_signal(&cond);

	printf("\n");

	return NULL;

}

int main (int argc, char *argv[]) {

	struct arg_struct t1_args;
	t1_args.fn = argv[1];
	t1_args.thread_id = 1;

	struct arg_struct t2_args;
	t2_args.fn = argv[2];
	t2_args.thread_id = 2;


	pthread_t tid[2];
	pthread_mutex_init(&lock, NULL);


	pthread_create(&tid[0], NULL, printFile, (void *)&t1_args);
	pthread_create(&tid[1], NULL, printFile, (void *)&t2_args);

	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);



	return 0;
}