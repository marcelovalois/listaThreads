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
int first = 0;
int last = 0;



void funexec(fatores a) {
    printf("%c %d\n", a.a, a.b);
}

void agendarExecucao(void (*func)(fatores), fatores p) {
    
    buff[last].funcao = func;
    buff[last].parametro = p;

    last++;
    items++;
    return items - 1;
}

void despachar() {

}


int main() {

    fatores a;
    a.a = 1; a.b = 5;

    agendarExecucao(funexec, a);
    
    



   return 0;
}