#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define CADEIRAS 5              /* Numero de cadeiras*/
#define TIMEWAIT_MAX 5          /* Tempo maximo de espera */

typedef enum {                  /* Definicao das etapas dos filosofos */    
    pensar,
    fome,
    comer,
} Filosofos;

int Filosofos_esquerda(int filo_n);     
int Filosofos_direita(int filo_n);
void *Filosofos_checkin(void *filo);

pthread_mutex_t forks;                  /* exclusao mutua para regioes cri­ticas */
Filosofos *FilosofosN;

int main(){

    pthread_t *thread;                              

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

    int i;
    for(i=0; i<CADEIRAS; i++){                                          /* Todos os filosofos em estado PENSANDO */
        FilosofosN[i] = pensar;
        pthread_create(&thread[i], NULL, Filosofos_checkin, (void *) i);
    }

    for(i=0; i<CADEIRAS; i++){                                          /* Todas as cadeiras com as threads */
        pthread_join(thread[i], NULL);
    }

    return 0;
}

int Filosofos_esquerda(int filo_n){
    return (filo_n + CADEIRAS - 1) % CADEIRAS;          /* Devolve o garfo esquerdo <<< a mesa */
}

int Filosofos_direita(int filo_n){
    return (filo_n + 1) % CADEIRAS;                     /* Devolve o garfo direito >>> a mesa */
}

void *Filosofos_checkin(void *filo){
    int filo_n = (int) filo;

    while(1){                                           /* Switch case para garantir que o filosofo soh ira comer se ambos os garfos estiverem liberados */
        pthread_mutex_lock(&forks);

        switch(FilosofosN[filo_n]){
            case pensar:
                FilosofosN[filo_n] = fome;
                pthread_mutex_unlock(&forks);           /* Destrava semafaro para o filosofo que está pensando para passar ao passo de COMER...*/

                printf("Filosofos #%d: pensando...\n", filo_n);

                sleep(random() % TIMEWAIT_MAX);
                break;

            case comer:
                FilosofosN[filo_n] = pensar;
                pthread_mutex_unlock(&forks);           /* Destrava semafaro para o filosofo que está comendo para passar ao passo de PENSAR...*/

                printf("Filosofos #%d: Parando de comer, voltando a pensar...\n", filo_n);

                sleep(random() % TIMEWAIT_MAX);
                break;

            case fome:
                printf("Filosofos #%d: estou com fome...\n", filo_n);

                if(FilosofosN[Filosofos_esquerda(filo_n)] == comer){
                    pthread_mutex_unlock(&forks);                           /* Destrava semafaro para o filosofo que está TENTANDO comer SE o garfo da ESQUERDA estiver liberado...*/
                    printf("Filosofos #%d:       Tentando comer mas o folosofo %d que esta na ESQUERDA esta comendo...\n", filo_n, Filosofos_esquerda(filo_n));
                } else if(FilosofosN[Filosofos_direita(filo_n)] == comer){
                    pthread_mutex_unlock(&forks);                           /* Destrava semafaro para o filosofo que está TENTANDO comer SE o garfo da DIREITA estiver liberado...*/
                    printf("Filosofos #%d:       Tentando comer mas o folosofo %d que esta na DIREITA esta comendo...\n", filo_n, Filosofos_direita(filo_n));
                }else{
                    FilosofosN[filo_n] = comer;
                    pthread_mutex_unlock(&forks);                           /* Destrava semafaro se os garfos estiverem ambos os garfos estiverem liberados... */
                    printf("Filosofos #%d:       Comendo!\n", filo_n);
                }

                sleep(random() % TIMEWAIT_MAX);
                break;
        }
    }
}