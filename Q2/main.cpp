#include <iostream>
#include <cstdlib>
#include <ctime>
#include <pthread.h>

using namespace std;

#define m 8
#define n 8000000

double thread_count = 8;

double A[m][n];
double x[n];
double y[m];

void randomValues() {
    srand(time(NULL));

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            A[i][j] = (double)rand() / RAND_MAX * 5.0;
        }
    }

    for (int i = 0; i < n; i++) {
        x[i] = (double)rand() / RAND_MAX * 5.0;
    }
}

void* multMatrixVectorParallel(void* rank) {
    long my_rank = (long)rank;
    int i, j;
    int local_m = m / thread_count; // thread_count número total de threads
    int my_start = my_rank * local_m;
    int my_end = (my_rank + 1) * local_m;

    for (i = my_start; i < my_end; i++) {
        y[i] = 0.0;
        for (j = 0; j < n; j++)
            y[i] += A[i][j] * x[j];
    }

    return NULL;
}

int main() {
    randomValues();

    pthread_t* thread_handles;
    thread_handles = (pthread_t*)malloc(thread_count * sizeof(pthread_t));

    clock_t start_time, end_time;

    start_time = clock();
    multMatrixVectorParallel((void*)0);
    end_time = clock();
    double elapsed_time_serial = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    cout << "Tempo para 1 THREAD: " << elapsed_time_serial << " segundos" << endl;

    start_time = clock();
    for (long thread = 0; thread < thread_count; thread++) {
        pthread_create(&thread_handles[thread], NULL, multMatrixVectorParallel, (void*)thread);
    }
    for (long thread = 0; thread < thread_count; thread++) {
        pthread_join(thread_handles[thread], NULL);
    }
    end_time = clock();
    double elapsed_time_cores = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    cout << "Tempo para número de threads : " << elapsed_time_cores << " segundos" << endl;


    free(thread_handles);

    return 0;
}
