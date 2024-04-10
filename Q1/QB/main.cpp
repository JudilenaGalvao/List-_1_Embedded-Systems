#include <cstdlib>
#include <ctime>
#include <iostream>
#include <pthread.h>

using namespace std;

#define n 100000
#define T 16

// Replit 8 cores
double total_sum = 0.0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *calcPartialPI(void *rank) {
  long my_rank = (long)rank; // typecast
  double factor;
  double pSum = 0.0;
  long long i;
  long long my_n = n / T;
  long long my_first_i = my_n * my_rank;
  long long my_last_i = my_first_i + my_n;

  if (my_first_i % 2 == 0) /* my first i is even */
    factor = 1.0;
  else /* my first i is odd */
    factor = -1.0;

  for (i = my_first_i; i < my_last_i; i++, factor = -factor) {
    double term = factor / (2.0 * i + 1);
    pSum += term; // Acumula o termo calculado em pSum
  }
  pthread_mutex_lock(&lock);
  total_sum += pSum;
  pthread_mutex_unlock(&lock);

  return NULL;
}

int main() {

  long thread;
  pthread_t *thread_handles;
  int err;
  srand(time(NULL));

  pthread_mutex_init(&lock, NULL);

  clock_t tstart, tend;
  tstart = clock();

  thread_handles = (pthread_t *)malloc(T * sizeof(pthread_t));
  for (thread = 0; thread < T; thread++)
    pthread_create(&thread_handles[thread], NULL, calcPartialPI,
                   (void *)thread);

  for (thread = 0; thread < T; thread++)
    pthread_join(thread_handles[thread], NULL);

  tend = clock();
  double trun;
  free(thread_handles);
  trun = (double)(tend - tstart) / (double)CLOCKS_PER_SEC;
  cout.precision(12);
  cout << fixed << "Time: " << trun << endl;
  cout << "Estimated value of PI: " << 4 * total_sum << endl;

  pthread_mutex_destroy(&lock);

  return 0;
}
