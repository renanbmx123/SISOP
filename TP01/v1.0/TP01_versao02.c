#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>

#define SEATS 5

typedef enum {
    think,
    hungry,
    eat,
} Phil;

int phil_left(int ph_id);
int phil_right(int ph_id);
void *phil_action(void *ph);
int print_table();

Phil *phils;
pthread_mutex_t table;

int main(){
    pthread_t *thread;

    phils = (Phil *) calloc(SEATS, sizeof(Phil));
    if(phils == 0){
        printf("\nErro ao alocar filosofos!\n");
        return -1;
    }

    thread = (pthread_t *) calloc(SEATS, sizeof(pthread_t));
    if(thread == 0){
        printf("\nErro ao alocar threads!\n");
        return -1;
    }

    pthread_mutex_init(&table, NULL);

    int i;
    for(i=0; i<SEATS; i++){
        phils[i] = think;
        pthread_create(&thread[i], NULL, phil_action, (void *) i);
    }

    for(i=0; i<SEATS; i++){
        pthread_join(thread[i], NULL);
    }

    return 0;
}

int phil_left(int ph_id){
    return (ph_id + SEATS - 1) % SEATS;
}

int phil_right(int ph_id){
    return (ph_id + 1) % SEATS;
}

void *phil_action(void *ph){
    int ph_id = (int) ph;

    while(1){
        pthread_mutex_lock(&table);

        switch(phils[ph_id]){
            case think:
                phils[ph_id] = hungry;
                print_table();
                pthread_mutex_unlock(&table);

                srand (time(NULL));
                sleep(rand() % 10 + 1);
                break;

            case eat:
                phils[ph_id] = think;
                print_table();
                pthread_mutex_unlock(&table);

                srand (time(NULL));
                sleep(rand() % 10 + 1);
                break;

            case hungry:
                if(phils[phil_left(ph_id)] != eat && phils[phil_right(ph_id)] != eat){
                    phils[ph_id] = eat;
                    print_table();
                }
                pthread_mutex_unlock(&table);

                srand (time(NULL));
                sleep(rand() % 10 + 1);
                break;
        }
    }
}

int print_table(){
    printf("\nTABLE: ");

    int i;
    for(i=0; i<SEATS; i++){
        if(phils[i] == eat){
            printf("->[ E ]<-");
        }else if(phils[i] == think){
            printf("   .T.   ");
        }else if(phils[i] == hungry){
            printf("  [ H ]  ");
        }else{
            printf("  [   ]  ");
        }
    }
}
