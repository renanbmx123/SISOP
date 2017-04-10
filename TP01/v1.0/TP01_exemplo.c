#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define ESQUERDA(f) ((f + nfilosofo - 1) % nfilosofo)
#define DIREITA(f) ((f + 1) % nfilosofo)

#define MSLEEP 5
#define SLEEPTIME (random()%MSLEEP)

typedef enum {
    COMFOME, PENSANDO, COMENDO,
} Filosofo;

pthread_mutex_t garfos;
Filosofo *filosofos;

int nfilosofo;

void *ffuncao(void *f) {
    int fid = (int) f;

    while (1) {
        pthread_mutex_lock (&garfos);

        switch (filosofos[fid]) {
            case PENSANDO:
                filosofos[fid] = COMFOME;
                pthread_mutex_unlock(&garfos);

                fprintf(stdout, "F[%d]: Estou pensando...\n", fid);

                sleep(SLEEPTIME);
                break;

            case COMFOME:
                fprintf(stdout, "F[%d]: Estou com fome... Vou tentar pegar os garfos!\n", fid);

                if (filosofos[ESQUERDA(fid)] == COMENDO) {
                    pthread_mutex_unlock(&garfos);
                    fprintf (stdout, "\tFilosofo %d estah comendo...\n", ESQUERDA(fid));
                } else if (filosofos[DIREITA(fid)] == COMENDO) {
                    pthread_mutex_unlock(&garfos);
                    fprintf (stdout, "\tFilosofo %d estah comendo...\n", DIREITA(fid));
                } else {
                    filosofos[fid] = COMENDO;
                    pthread_mutex_unlock(&garfos);
                    fprintf (stdout, "\tAgora sim, vou comer!\n");
                }

                sleep(SLEEPTIME);
                break;

            case COMENDO:
                filosofos[fid] = PENSANDO;
                pthread_mutex_unlock(&garfos);

                fprintf(stdout, "F[%d]: Ja comido, voltando a pensar...\n", fid);

                sleep(SLEEPTIME);
                break;
        }
    }
}

int main (int argc, char **argv) {
    int i;
    pthread_t *t;

    if (argc > 1) {
        char **endptr = NULL;

        nfilosofo = (int) strtol(argv[1], endptr, 10);

        if (endptr) {
            fprintf (stderr, "Argumento invalido: %s\n", argv[1]);
            return 1;
        }
    } else {
        nfilosofo = 5;
    }

    filosofos = (Filosofo *) calloc (nfilosofo, sizeof(Filosofo));

    if (!filosofos) {
        fprintf(stderr, "OS filosofos estao cansados. Nao querem pensar hoje...\n");
        return 1;
    }

    t = (pthread_t *) calloc (nfilosofo, sizeof(pthread_t));

    if (!t) {
        fprintf (stderr, "Executar ou nao executar, eis a questao...\n");
        return 1;
    }

    pthread_mutex_init (&garfos, NULL);

    for (i = 0 ; i < nfilosofo ; i++) {
        filosofos[i] = PENSANDO;
        pthread_create (&t[i], NULL, ffuncao, (void *) i);
    }

    for (i = 0 ; i < nfilosofo ; i++) {
        pthread_join(t[i], NULL);
    }

    return 0;
}
