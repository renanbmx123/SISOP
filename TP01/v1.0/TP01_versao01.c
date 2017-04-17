#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define CADEIRAS 5
#define TIMEWAIT_MAX 5

typedef enum {
    pensar,
    fome,
    comer, eat
} Filosofos;

int Filosofos_esquerda(int filo_n);
int Filosofos_direita(int filo_n);
void *Filosofos_checkin(void *filo);

pthread_mutex_t forks;
Filosofos *FilosofosN;

int main(){

    pthread_t *thread;

    FilosofosN = (Filosofos *) calloc(CADEIRAS, sizeof(Filosofos));
    if(FilosofosN == 0){
        printf("\nErro ao alocar os filosofos!\n");
        return -1;
    }

    thread = (pthread_t *) calloc(CADEIRAS, sizeof(pthread_t));
    if(thread == 0){
        printf("\nErro ao alocar as threads!\n");
        return -1;
    }

    pthread_mutex_init(&forks, NULL);

    int i;
    for(i=0; i<CADEIRAS; i++){
        FilosofosN[i] = pensar;
        pthread_create(&thread[i], NULL, Filosofos_checkin, (void *) i);
    }

    for(i=0; i<CADEIRAS; i++){
        pthread_join(thread[i], NULL);
    }

    return 0;
}

int Filosofos_esquerda(int filo_n){
    return (filo_n + CADEIRAS - 1) % CADEIRAS;
}

int Filosofos_direita(int filo_n){
    return (filo_n + 1) % CADEIRAS;
}

void *Filosofos_checkin(void *filo){
    int filo_n = (int) filo;

    while(1){
        pthread_mutex_lock(&forks);

        switch(FilosofosN[filo_n]){
            case pensar:
                FilosofosN[filo_n] = fome;
                pthread_mutex_unlock(&forks);

                printf("Filosofos #%d: pensando...\n", filo_n);

                sleep(random() % TIMEWAIT_MAX);
                break;

            case comer:
                FilosofosN[filo_n] = pensar;
                pthread_mutex_unlock(&forks);

                printf("Filosofos #%d: Parando de comer, voltando a pensar...\n", filo_n);

                sleep(random() % TIMEWAIT_MAX);
                break;

            case fome:
                printf("Filosofos #%d: estou com fome...\n", filo_n);

                if(FilosofosN[Filosofos_esquerda(filo_n)] == comer){
                    pthread_mutex_unlock(&forks);
                    printf("Filosofos #%d:       Filosofos #%d (esquerda) esta comendo...\n", filo_n, Filosofos_esquerda(filo_n));
                } else if(FilosofosN[Filosofos_direita(filo_n)] == comer){
                    pthread_mutex_unlock(&forks);
                    printf("Filosofos #%d:       Filosofos #%d (direita) esta comendo...\n", filo_n, Filosofos_direita(filo_n));
                }else{
                    FilosofosN[filo_n] = comer;
                    pthread_mutex_unlock(&forks);
                    printf("Filosofos #%d:       Comendo!\n", filo_n);
                }

                sleep(random() % TIMEWAIT_MAX);
                break;
        }
    }
}