#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

int CADEIRAS;              /* Numero de cadeiras*/
int TIMEWAIT_MAX;          /* Tempo maxim  o de espera */
volatile int running = 1;  /* Se 0 finaliza as threads */

typedef enum {                  /* Definicao das etapas dos filosofos */    
    pensar,
    fome,
    comer,
} Filosofos;

void rel(int filosofo, int comeu,int pensou,int tentou);                 /* Exibe o relatório dos filosofos*/
int Filosofos_esquerda(int filo_n);     
int Filosofos_direita(int filo_n);
void *Filosofos_checkin(void *filo);
pthread_mutex_t forks;                  /* exclusao mutua para regioes cri­ticas */
Filosofos *FilosofosN;

int main(int argc, char **argv){

	int i; //contador
	
    pthread_t *thread;                              
	
    CADEIRAS = atoi(argv[1]);
    TIMEWAIT_MAX = atoi(argv[2]);    
   
    FilosofosN = (Filosofos *) calloc(CADEIRAS, sizeof(Filosofos));     /* Colocando os filosofos nas cadeiras */
    if(FilosofosN == 0){
        printf("\nErro ao alocar os filosofos!\n");                     /* Se nao tiver nenhum filosofo setado, retorna erro */
        return -1;
    }

    thread = (pthread_t *) calloc(CADEIRAS, sizeof(pthread_t));         /* Aloca uma thread para cada filosofo */
    if(thread == 0){
        printf("\nErro ao alocar as threads!\n");                       /* Se nao tiver nenhuma thread, retorna erro */
        return -1;
    }

    pthread_mutex_init(&forks, NULL);                                   /* Inicializa regiao cri­tica */

    for(i = 0; i<CADEIRAS; i++){                                      /*Todos os filosofos em estado PENSANDO */
       
        FilosofosN[i] = pensar;
        pthread_create(&thread[i], NULL, Filosofos_checkin, (void *) i);
        
    }

    /* Espera o jantar acontecer pelo tempo definido pelo usuário. */
   	sleep(TIMEWAIT_MAX);
   	
    /* Sinaliza o término do jantar.*/
	running = 0;
    
	/* Espera os canibais terminarem. */
	for(int n = 0; n<CADEIRAS; n++){
		if(pthread_join(thread[n], NULL)) {
			fprintf(stderr, "Error joining thread\n");
			return 1;
		}
	}
	pthread_mutex_destroy(&forks);
	free(FilosofosN);
    printf("Termino da execução... \n");
    return 0;
}

int Filosofos_esquerda(int filo_n){
    return (filo_n + CADEIRAS - 1) % CADEIRAS;          /* Devolve o garfo esquerdo <<< a mesa */
}

int Filosofos_direita(int filo_n){
    return (filo_n + 1) % CADEIRAS;                     /* Devolve o garfo direito >>> a mesa */
}

void *Filosofos_checkin(void *filo){
    int filo_n = (int*)filo;
	int comeu, tentou, pensou;
	comeu = 0;
	tentou = 0;
	pensou = 0;
    while(running){                                           /* Switch case para garantir que o filosofo soh ira comer se ambos os garfos estiverem liberados */
        pthread_mutex_lock(&forks);

        switch(FilosofosN[filo_n]){
            case pensar:
                FilosofosN[filo_n] = fome;
                pthread_mutex_unlock(&forks);           /* Destrava semafaro para o filosofo que está pensando para passar ao passo de COMER...*/
                printf("Filosofos #%d: pensando...\n", filo_n);
                pensou ++;
                sleep(5);
            break;

            case comer:
                FilosofosN[filo_n] = pensar;
                pthread_mutex_unlock(&forks);           /* Destrava semafaro para o filosofo que está comendo para passar ao passo de PENSAR...*/
				comeu ++;
                printf("Filosofos #%d: Parando de comer, voltando a pensar...\n", filo_n);
                usleep(2000);
            break;

            case fome:
                printf("Filosofos #%d: estou com fome...\n", filo_n);
                              
                if(FilosofosN[Filosofos_esquerda(filo_n)] == comer){
                    pthread_mutex_unlock(&forks);                           /* Destrava semafaro para o filosofo que está TENTANDO comer SE o garfo da ESQUERDA estiver liberado...*/       
                    tentou ++;
                    printf("Filosofos #%d:       Tentando comer mas o filosofo %d que esta na ESQUERDA esta comendo...\n", filo_n, Filosofos_esquerda(filo_n));
                } else if(FilosofosN[Filosofos_direita(filo_n)] == comer){
                    pthread_mutex_unlock(&forks);                           /* Destrava semafaro para o filosofo que está TENTANDO comer SE o garfo da DIREITA estiver liberado...*/       
                    tentou ++;
                    printf("Filosofos #%d:       Tentando comer mas o filosofo %d que esta na DIREITA esta comendo...\n", filo_n, Filosofos_direita(filo_n));
                }else{
                    FilosofosN[filo_n] = comer;
                    pthread_mutex_unlock(&forks);                           /* Destrava semafaro se os garfos estiverem ambos os garfos estiverem liberados... */
                    printf("Filosofos #%d:       Comendo!\n", filo_n);
                }
                
                sleep(random() % 3);
            break;
        }
        
    }
    rel(filo_n, comeu, pensou, tentou);
   	return NULL;
}

void rel(int filosofo, int comeu,int pensou,int tentou){
	
	printf("Filosofo [%d] - Pensou %d: - Comeu %d: - Tentou %d:\n",filosofo ,comeu ,pensou, tentou);
}
