
#include <stdio.h>
#include <pthread.h>

#define THREADS 5
#define ITERATIONS 100000

int counter = 0;

void *increment(void *arg)
{
    for (int i = 0; i < ITERATIONS; i++)
    {
        counter++;
    }

    return NULL;
}

int main(void)
{
    pthread_t threads[THREADS];

    for (int i = 0; i < THREADS; i++)
    {
        pthread_create(&threads[i], NULL, increment, NULL);
    }

    for (int i = 0; i < THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("Final counter: %d\n", counter);
    printf("Expected counter: %d\n", THREADS * ITERATIONS);

    return 0;
}

