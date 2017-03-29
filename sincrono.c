#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


pthread_mutex_t mutex1, mutex2, mutex3;

void * th1(){
	while(1){
		pthread_mutex_lock(&mutex1);
		printf("1\n");
		fflush(stdout);
		sleep(1);
		pthread_mutex_unlock(&mutex2);
	}
}

void * th2(){
	while(1){
		pthread_mutex_lock(&mutex2);
		printf("2\n");
		fflush(stdout);
		sleep(2);
		pthread_mutex_unlock(&mutex3);
	}
}

void * th3(){
	while(1){
		pthread_mutex_lock(&mutex3);
		printf("3\n");
		fflush(stdout);
		sleep(3);
		pthread_mutex_unlock(&mutex1);
	}
}


int main(int argc, char **argv){
	pthread_t thread1, thread2, thread3;
	
	/* Inicializa Mutex */
	pthread_mutex_init(&mutex1, NULL);
	pthread_mutex_init(&mutex2, NULL);
	pthread_mutex_init(&mutex3, NULL);
	
	pthread_mutex_lock(&mutex2);
	pthread_mutex_lock(&mutex3);

	if(pthread_create(&thread1, NULL, th1, NULL)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}

	if(pthread_create(&thread2, NULL, th2, NULL)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}
	
	if(pthread_create(&thread3, NULL, th3, NULL)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}
	

	if(pthread_join(thread1, NULL)) {
		fprintf(stderr, "Error joining thread\n");
		return 1;
	}

	return 0;
}
