#include <pthread.h>
#include <sched.h>
#include <stdint.h>
#include <stdio.h>

#define ITER 400000
#define THREADS 4

static volatile int32_t counter_race = 0;
static int32_t counter_safe = 0;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

static void *worker_race(void *arg)
{
    int i;
    (void)arg;
    for (i = 0; i < ITER; ++i) {
        int32_t tmp = counter_race;
        tmp++;
        if ((i & 1023) == 0) {
            sched_yield();
        }
        counter_race = tmp;
    }
    return NULL;
}

static void *worker_safe(void *arg)
{
    int i;
    (void)arg;
    for (i = 0; i < ITER; ++i) {
        pthread_mutex_lock(&mtx);
        counter_safe++;
        pthread_mutex_unlock(&mtx);
    }
    return NULL;
}

int main(void)
{
    pthread_t t[THREADS];
    int i;

    for (i = 0; i < THREADS; ++i) {
        pthread_create(&t[i], NULL, worker_race, NULL);
    }
    for (i = 0; i < THREADS; ++i) {
        pthread_join(t[i], NULL);
    }

    for (i = 0; i < THREADS; ++i) {
        pthread_create(&t[i], NULL, worker_safe, NULL);
    }
    for (i = 0; i < THREADS; ++i) {
        pthread_join(t[i], NULL);
    }

    printf("Race counter   = %d (expected %d)\n", counter_race, THREADS * ITER);
    printf("Mutex counter  = %d (expected %d)\n", counter_safe, THREADS * ITER);
    printf("Interview point: race condition = shared state without synchronization.\n");
    return 0;
}
