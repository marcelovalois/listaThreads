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

int agendarExecucao(void* (func), fatores p) {

    buffer aux;
    aux.funcao = func; aux.parametro = p;
    buff[last] = aux;


    items++; last++; total++;

    return total - 1;
}

void* funexec(void* params) {

    // pthread_mutex_lock(&mutex);
    printf("Thread criada\n");

    fatores a = *((fatores *)params);

    printf("%d %d\n", a.a, a.b);


    // pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

void *despachante() {
    
    pthread_t consumer_thread[20];
    void* (func);

    int i;

    for(i = 0; i < 20; i++) {
        fatores* params = (fatores*) malloc (sizeof(fatores));

        buffer aux = buff[first];

        printf("first: %d\n", first);

        func = aux.funcao; params[0] = aux.parametro;

        pthread_create(&consumer_thread[i], NULL, func, (void *)params);

        items--; first++;
    }

    // for(i = 0; i < 20; i++)
    //     pthread_join(consumer_thread[i], NULL);
}

int main() {

    int i, choice = 0;

    fatores factors;
    
    for (i = 0; i < 20; i++) {
        factors.a = i; factors.b = i; 
        agendarExecucao(funexec, factors);
    }

    despachante();

   return 0;
}