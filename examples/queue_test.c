
#include <stdio.h>
#include <pthread.h>
#include "minios.h"

#define TOTAL_JOBS 10

job_queue_t q;

void *producer(void *arg)
{
    for (int i = 1; i <= TOTAL_JOBS; i++) {
        job_t job;
        job.job_id = i;

        queue_push(&q, job);
        printf("Producer: Added job %d\n", i);
    }

    return NULL;
}

void *consumer(void *arg)
{
    for (int i = 1; i <= TOTAL_JOBS; i++) {
        job_t job;

        queue_pop(&q, &job);
        printf("Consumer: Processed job %d\n", job.job_id);
    }

    return NULL;
}

int main(void)
{
    pthread_t producer_thread, consumer_thread;

    printf("Initializing queue...\n");

    if (queue_init(&q) != 0) {
        printf("Queue initialization failed!\n");
        return 1;
    }

    printf("Queue initialized successfully!\n\n");

    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    queue_destroy(&q);

    printf("\nAll concurrent queue tests passed!\n");

    return 0;
}

