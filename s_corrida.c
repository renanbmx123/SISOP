#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

#define TOTAL 100000
unsigned int sum = 0;
pthread_mutex_t m;
int counter;


/* thread */
void *incr(){
	unsigned int i;
	counter += 1;

	printf("\n Lock\n");
	for(i=0;i<TOTAL;i++){
		pthread_mutex_lock(&m);
		sum += 1;
			printf("\n Job %i started\n", counter);
			fflush(stdout);
		pthread_mutex_unlock(&m);
	}
	
	printf("\n Unlock\n");
	return NULL;
}

int main(){

	pthread_t thread1;
	pthread_t thread2;

	pthread_mutex_init(&m, NULL);
	int i = 0;
    int err;

	if(pthread_create(&thread1, NULL, incr, NULL)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}
	
	if(pthread_create(&thread2, NULL, incr, NULL)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}


	if(pthread_join(thread1, NULL)) {
		fprintf(stderr, "Error joining thread\n");
		return 2;
	}

	if(pthread_join(thread2, NULL)) {
		fprintf(stderr, "Error joining thread\n");
		return 2;
	}


	printf("SUM: %d\n", sum);

	pthread_mutex_destroy(&m);

	return 0;
}