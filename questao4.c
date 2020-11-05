#include <stdio.h>
#include <pthread.h> 
#include <stdlib.h>

#define BUFFER_SIZE 10
#define N 2

typedef struct pessoa {
    char genero;
    int idade;
} pessoa;

int buff[BUFFER_SIZE];

void funexec(pessoa a) {
    printf("%c %d\n", a.genero, a.idade);
}

void agendarExecucao(void (*func)(pessoa), pessoa p) {
    (*func)(p);
}




int main() {

    pessoa x;
    x.genero = 'm'; x.idade = 23;

    agendarExecucao(funexec, x);
    




   return 0;
}