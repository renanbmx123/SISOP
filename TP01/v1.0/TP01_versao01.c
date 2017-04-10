#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

#define SEATS 5
#define MAX_SLEEP 5

typedef enum {
    think,
    hungry,
    eat,
} Phil;

int phil_left(int ph_id);
int phil_right(int ph_id);
void *phil_action(void *ph);

pthread_mutex_t forks;
Phil *phils;

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

    pthread_mutex_init(&forks, NULL);

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
        pthread_mutex_lock(&forks);

        switch(phils[ph_id]){
            case think:
                phils[ph_id] = hungry;
                pthread_mutex_unlock(&forks);

                printf("Phil #%d: Thinking\n", ph_id);

                sleep(random() % MAX_SLEEP);
                break;

            case eat:
                phils[ph_id] = think;
                pthread_mutex_unlock(&forks);

                printf("Phil #%d: Stopped eating, back to thinking\n", ph_id);

                sleep(random() % MAX_SLEEP);
                break;

            case hungry:
                printf("Phil #%d: Hungry...\n", ph_id);

                if(phils[phil_left(ph_id)] == eat){
                    pthread_mutex_unlock(&forks);
                    printf("Phil #%d:       Phil #%d (left) is eating...\n", ph_id, phil_left(ph_id));
                } else if(phils[phil_right(ph_id)] == eat){
                    pthread_mutex_unlock(&forks);
                    printf("Phil #%d:       Phil #%d (right) is eating...\n", ph_id, phil_right(ph_id));
                }else{
                    phils[ph_id] = eat;
                    pthread_mutex_unlock(&forks);
                    printf("Phil #%d:       Eating!\n", ph_id);
                }

                sleep(random() % MAX_SLEEP);
                break;
        }
    }
}
