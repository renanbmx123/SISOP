#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void * th1(){
	while(1){
		printf("1\n");
		fflush(stdout);
		sleep(1);
	}
}

void * th2(){
	while(1){
		printf("2\n");
		fflush(stdout);
		sleep(2);
	}
}

void * th3(){
	while(1){
		printf("3\n");
		fflush(stdout);
		sleep(3);
	}
}


int main(int argc, char **argv){
	pthread_t thread1, thread2, thread3;
	

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
