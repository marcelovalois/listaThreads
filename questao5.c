#include <stdio.h>
#include <pthread.h> 
#include <stdlib.h>

#define I 4
#define P 10

pthread_barrier_t barrier;

int A[I][I] = {{20, 14, 5, 9},
              {12, 8, 6, 25},
              {3, 16, 36, 2},
              {4, 15, 35, 21}};

int B[I] = {2, 4, 12, 5};

float X[I] = {1, 1, 1, 1};



void* jacobi(void* i) {
    int id = *((int *) i);
    float aux[I], sum = 0;

    int k = 0, j = 0;

    while ( k < P ) {
        aux[0] = X[0], aux[1] = X[1], aux[2] = X[2], aux[3] = X[3];
        sum = 0;

        pthread_barrier_wait(&barrier);


        for (j = 0; j < I; j++) {
            if (j != id){
                sum += (float) A[id][j] * aux[j]; 
            }
        }

        X[id] = (B[id] - sum) / A[id][id];
        k = k + 1;

        printf("ID: %d  iteracao: %d  X: %f\n", id, k, X[id]);

        pthread_barrier_wait(&barrier);
        }

    pthread_exit(NULL);
    
}


int main() {

    int t, n, k=0;

    printf("Digite a quantidade de threads: ");
    scanf("%d", &n);

    pthread_t threads[n];
    int *tasksids[n];

    pthread_barrier_init(&barrier, NULL, n);

    for (t = 0; t < n; t++) {
        tasksids[t] = (int *) malloc(sizeof(int));
        *tasksids[t] = t % I;
        pthread_create(&threads[t], NULL, jacobi, (void*) tasksids[t]);
    }

    for(t = 0; t < n; t++) { pthread_join(threads[t], NULL); }

    printf("Resposta Final: x1: %f, x2: %f, x3: %f, x4: %f\n", X[0], X[1], X[2], X[3]);

    pthread_barrier_destroy(&barrier);

    pthread_exit(NULL);

    return 0;
}