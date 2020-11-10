#include <stdio.h>
#include <pthread.h> 
#include <stdlib.h>

#define NUM_ITEMS 500 // QUANTIDADE DE REQUISIÇÕES QUE SERÃO GERADAS
#define BUFFER_SIZE 20 // O TAMANHO DO BUFFER
#define N 5  //O NÚMERO MÁXIMO DE THREADS

typedef struct fatores {
    int a;
    int b;
    int id;
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

int ready[NUM_ITEMS] = {0};
int result[NUM_ITEMS];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t fill = PTHREAD_COND_INITIALIZER;
pthread_cond_t unavailable = PTHREAD_COND_INITIALIZER;
pthread_cond_t resultado = PTHREAD_COND_INITIALIZER;



int agendarExecucao(void* (*func)(void*), fatores p) {

    pthread_mutex_lock(&mutex);

    while(items == BUFFER_SIZE) pthread_cond_wait(&fill, &mutex); //CASO O BUFFER ESTEJA CHEIO

    buffer aux;
    aux.funcao = func; aux.parametro = p;
    buff[last] = aux;

    last++; total++; items++;

    if(last == BUFFER_SIZE) last = 0;
        
    if (items == 1) pthread_cond_broadcast(&empty); //VARIÁVEL DE CONDIÇÃO

    pthread_mutex_unlock(&mutex);
    
    return total - 1;
}


void* funexec(void* params) {  //FUNÇÃO GENÉRICA PEDIDA PELA QUESTÃO. ESSA RETORNA A SOMA DOS DOIS VALORES

    fatores dados = *((fatores *)params);

    ready[dados.id] = 1;

    result[dados.id] = dados.a + dados.b;

    printf("Dados recebidos na thread com id %d: %d %d\n", dados.id, dados.a, dados.b);

    free(params);

    pthread_mutex_lock(&mutex2);
    
    available++;

    pthread_cond_signal(&unavailable);
    pthread_cond_signal(&resultado);
    
    pthread_mutex_unlock(&mutex2);

    pthread_exit(NULL);
}



void *despachante() { // ESSA É A THREAD DESPACHANTE

    void* (*func)(void*);
    int i;

    for(i = 0; i < NUM_ITEMS; i++) {
        
        pthread_mutex_lock(&mutex);

        available--;

        while (items == 0) pthread_cond_wait(&empty, &mutex); //ESPERA ALGUM ITEM NO BUFFER

        while (available == 0) pthread_cond_wait(&unavailable, &mutex); //ESPERA ALGUMA THREAD DISPONÍVEL

        pthread_t consumer_thread;
        fatores* params = (fatores*) malloc(sizeof(fatores));  
        buffer aux = buff[first];

        func = aux.funcao; 
        params->a = aux.parametro.a;
        params->b = aux.parametro.b;
        params->id = i;  // ID DA THREAD

        pthread_create(&consumer_thread, NULL, func, (void*) params);  //CRIA AS THREADS QUE EXECUTAM FUNEXEC

        items--; first++;

        if(first == BUFFER_SIZE) first = 0;

        if(items == BUFFER_SIZE - 1) pthread_cond_signal(&fill);

        pthread_mutex_unlock(&mutex);

    }

    pthread_exit(NULL);

}

void pegarResultadoExecucao(int chave) {

    pthread_mutex_lock(&mutex2);

    while (ready[chave] == 0) pthread_cond_wait(&resultado, &mutex2); //ESPERA A THREAD ENTREGAR O RESULTADO

    if (ready[chave])
        printf("O resultado para a chave %d eh %d\n", chave, result[chave]);

    pthread_mutex_unlock(&mutex2);
}


int main() {

    pthread_t despachante_thread;
    int i, chave;

    fatores factors;
    
    pthread_create(&despachante_thread, NULL, despachante, NULL);

    for (i = 0; i < NUM_ITEMS; i++) {
        factors.a = rand() % 1000; factors.b = rand() % 1000;  //PEGA NÚMEROS ALEATÓRIOS DE 0 A 1000
        chave = agendarExecucao(funexec, factors);  //AGENDA EXECUÇÃO

        pegarResultadoExecucao(chave);
    }

    pthread_exit(NULL);

   return 0;
}

