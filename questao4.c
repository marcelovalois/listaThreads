#include <stdio.h>
#include <pthread.h> 
#include <stdlib.h>

#define BUFFER_SIZE 20
#define N 5

typedef struct fatores {
    int a;
    int b;
} fatores;

typedef struct buffer {
    void* funcao;
    fatores parametro;
} buffer;

buffer buff[BUFFER_SIZE];
int available = N;
int items = 0;
int total = 0;
int first = 0;
int last = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t fill = PTHREAD_COND_INITIALIZER;



int agendarExecucao(void* (*func)(void*), fatores p) {

    printf("EN ");

    pthread_mutex_lock(&mutex);

    while(items == BUFFER_SIZE) pthread_cond_wait(&fill, &mutex);

    buffer aux;
    aux.funcao = func; aux.parametro = p;
    buff[last] = aux;

    last++; total++; items++;

    if(last == BUFFER_SIZE) last = 0;
        
    if (items == 1) pthread_cond_broadcast(&empty);

    pthread_mutex_unlock(&mutex);
    
    return total - 1;
}


void* funexec(void* params) {

    printf("Thread criada\n");

    fatores a = *((fatores *)params);

    printf("%d %d\n", a.a, a.b);

    int i = 0;
    for (i=0; i<1000000;i++){
        i+i;
    }

    free(params);

    pthread_mutex_lock(&mutex2);
    available++;
    pthread_mutex_unlock(&mutex2);

    pthread_exit(NULL);
}



void *despachante() {

    void* (*func)(void*);
    int i;

    printf("COMEÇOU DESPACHANTE\n");

    for(i = 0; i<500; i++) {
        
        pthread_mutex_lock(&mutex);

        available--;

        while (items == 0) pthread_cond_wait(&empty, &mutex);

        while (available == 0) {
        ;
        }

        pthread_t consumer_thread;
        fatores* params = (fatores*) malloc(sizeof(fatores));
        buffer aux = buff[first];

        func = aux.funcao; 
        params->a = aux.parametro.a;
        params->b = aux.parametro.b;


        printf("first: %d     %d %d\n", first, params->a, params->b);

        pthread_create(&consumer_thread, NULL, func, (void*) params);

        items--; first++;

        if(first == BUFFER_SIZE) first = 0;

        pthread_cond_signal(&fill);

        pthread_mutex_unlock(&mutex);

    }

    pthread_exit(NULL);


}


int main() {

    pthread_t despachante_thread;
    int i, teste;

    fatores factors;
    
    pthread_create(&despachante_thread, NULL, despachante, NULL);

    for (i = 0; i < 500; i++) {
        factors.a = i; factors.b = i;
        agendarExecucao(funexec, factors);
    }

    

    pthread_exit(NULL);

   return 0;
}

