#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFERSZ	10
#define STRLEN		64
#define NCON		5

static char buffer[BUFFERSZ][STRLEN];
static int in, out;
static volatile int count;

static char format[] = "ping -c 1 -w 1 %s";
static char command[128];
static running = 1;

int ping(char *ip)
{
	char str[1024];
	FILE *fpipe;
	
	sprintf(command, format, ip);

	/* executa comando redirecionando a saida para o pipe. */
	if ( (fpipe = popen(command, "r"))<0){
		perror("popen");
	}
	
	/* Ignora primeira linha */
	/* PING 10.32.175.200 (10.32.175.200) 56(84) bytes of data. */
	fgets(str, 1023, fpipe);

	/* Verifica se teve sucesso no ping com a segunda linha */
	/* 64 bytes from 10.32.175.200: icmp_req=1 ttl=64 time=0.023 ms */
	fgets(str, 1023, fpipe);
	if(strcmp(str, "\n"))
		return 1;
	else
		return 0;

	fclose(fpipe);
}


void getFromBuf(char *str){
	while (count == 0);
	
	strncpy(str, buffer[out], STRLEN);
	out = (out + 1) % BUFFERSZ;
	count--;
}

void setToBuf(char *str){
	while(count == BUFFERSZ);
	
	strncpy(buffer[in], str, STRLEN);
	in = (in + 1) % BUFFERSZ;
	count++;
}

void * producer(){
	char str[STRLEN];
	int i = 0;
	char ipfor[] = "10.32.175.%i";
	
	for(i=0; i<255; i++){
		sprintf(str, ipfor, i);
		setToBuf(str);
	}
}

void * consumer(void * d){
	int id = *(int*)d;
	char str[STRLEN];
	int i = 0;
	
	while(running){
		//sprintf(str, "thread %d message %i", id, i);
		getFromBuf(str);
		printf("Thread %i -> %s %s\n", id, str, ping(str)? "sucesso": "falha");
		fflush(stdout);
	}
}


int main(int argc, char **argv){
	pthread_t prod, *con;
	//int ids[NCON] = {0, 1, 2, 3, 4};
	int *ids;
	int i, numthr;

	in = 0;
	out = 0;
	count = 0;
	memset(buffer, 0, sizeof(BUFFERSZ));
	
	if (argc < 2){
		printf("Uso: %s <num threads>\n", argv[0]);
		exit(0);
	}
	
	numthr = atoi(argv[1]);
	ids = (int *)malloc(numthr * sizeof(int));
	for(i=0; i<numthr; i++){
		ids[i] = i;
	}
	
	con = (pthread_t *)malloc(numthr * sizeof(pthread_t));

	if(pthread_create(&prod, NULL, producer, NULL)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}

	for(i=0; i<numthr; i++){
		if(pthread_create(&con[i], NULL, consumer, (void*)&ids[i])) {
			fprintf(stderr, "Error creating thread\n");
			return 2;
		}
	}

	if(pthread_join(prod, NULL)) {
		fprintf(stderr, "Error joining thread\n");
		return 3;
	}

	/* encerra execução das threads produtoras. */
	running = 0;
	
	for(i=0; i<numthr; i++){
		if(pthread_join(con[i], NULL)) {
			fprintf(stderr, "Error joining thread\n");
			return 4;
		}
	}
	
	return 0;
}
