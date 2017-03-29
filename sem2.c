#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>


sem_t semaphore;

void * th1(void *n){
	int id = *(int*)n;
	int i;
	while(1){
		for(i=0;i<5;i++){
			printf("%d\n", id);
			fflush(stdout);
			sleep(1);
		}
	}
}



int main(int argc, char **argv){
	pthread_t thread1, thread2, thread3;
	int thread_ids[4] = {0, 1, 2, 3};
	
	/* Inicializa Mutex */

	if(pthread_create(&thread1, NULL, th1, &thread_ids[0])) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}

	if(pthread_create(&thread2, NULL, th1, &thread_ids[1])) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}
	
	if(pthread_create(&thread3, NULL, th1, &thread_ids[2])) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}

	if(pthread_create(&thread3, NULL, th1, &thread_ids[3])) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}


	if(pthread_join(thread1, NULL)) {
		fprintf(stderr, "Error joining thread\n");
		return 1;
	}

	return 0;
}
