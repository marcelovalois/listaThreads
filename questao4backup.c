#include <stdio.h>
#include <pthread.h> 
#include <stdlib.h>

#define BUFFER_SIZE 10
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


void *producer(void *params) {

    buffer aux = *((buffer *) params);

    
    pthread_exit(NULL);
}


void funexec(fatores a) {
    printf("%c %d\n", a.a, a.b);
}

void agendarExecucao(void (*func)(fatores), fatores p) {

    buffer aux;
    int rc;
    aux.funcao = func; aux.parametro = p;

    
    pthread_t insert_buffer;
    rc = pthread_create(&insert_buffer, NULL, producer, (void *) &aux);
    if (rc) {
        printf("ERRO");
        exit(-1);
    }

    pthread_exit(NULL);

    
}

void despachar() {
;
}


int main() {

    int choice = 0;

    fatores fators;
    fators.a = 2; fators.b = 3;

    agendarExecucao(funexec, fators);

    // do {

    //     printf("O que o programa deve fazer?\n");


    // } while (choice != 3);

    
    
    



   return 0;
}