/*
Suponha que um grupo de N canibais come jantares a partir de uma grande travessa que comporta M porções. Quando alguém quer comer, ele(ela) se 
serve da travessa, a menos que ela esteja vazia. Se a travessa está vazia, o canibal acorda o cozinheiro e espera até que o cozinheiro coloque 
mais M porções na travessa. Desenvolva o código para as ações dos canibais e do cozinheiro – seserve (chamada por canibal) e enchetravessa 
(chamada por cozinheiro). A solução deve evitar deadlock e deve acordar o cozinheiro apenas quando a travessa estiver vazia. Suponha um longo 
jantar, onde cada canibal continuamente se serve e come, sem se preocupar com as demais na vida de um canibal. Dica: Use a primitiva tryacquire() 
para checar se um semaphoro esta ou não bloqueado.
*/

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>
#include <errno.h>
#include <unistd.h>


/* Faz as threads terminarem quando assume valor 1.*/
volatile int running = 1;
int numcanibais = 0;

/* Usado pelos canibais para se servirem. */
int seserve(){
	return 1;
}

/* Usado pelo cozinheiro para encher a travessa. */
void enchetravessa(){

}

/* Thread canibal */
void * canibal(void *param){
	int id = *(int*)param;
	
	while(running){
		/* Canibal serve-se. */
		if (seserve() == 1){
			printf("canibal comendo %d\n", id);
			fflush(stdout);
		}
	}

	return NULL;
}

/* Thread Cozinheiro */
void * cozinheiro(void *param){
	while(running){
		enchetravessa();
	}
	
	return NULL;
}



int main(int argc, char** argv){
	pthread_t cozinheirothr;
	pthread_t *canibais;
	int i;
	int * ids;
	int runtime;
	
	if(argc < 3){
		printf("Uso: %s <#canibais> <tempo execucao>\n", argv[0]);
		return 0;
	}
	
	/* Pega parâmetros do usuário. */
	numcanibais = atoi(argv[1]);
	runtime = atoi(argv[2]);
	
	/* Inicializa array com descritores dos canibais. */
	canibais = (pthread_t*)malloc(numcanibais * sizeof(pthread_t));
	/* Inicializa array com mutex dos canibais. */

	ids = (int*)malloc(numcanibais * sizeof(int));
	


	/* Dispara threads canibais. */
	for(i=0; i<numcanibais; i++){
		ids[i] = i;
		if(pthread_create(&canibais[i], NULL, canibal, (void*)&ids[i])) {
			fprintf(stderr, "Error creating thread\n");
			return 2;
		}
	}
	
	/* Dispara cozinheiro. */
	pthread_create(&cozinheirothr, NULL, cozinheiro, NULL);

	/* Espera o jantar acontecer pelo tempo definido pelo usuário. */
	sleep(runtime);
	
	/* Sinaliza o término do jantar.*/
	running = 0;
	
	/* Espera os canibais terminarem. */
	for(i=0; i<numcanibais; i++){
		if(pthread_join(canibais[i], NULL)) {
			fprintf(stderr, "Error joining thread\n");
			return 4;
		}
	}
	
	return 0;
}