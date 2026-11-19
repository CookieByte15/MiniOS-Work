
#ifndef MINIOS_H
#define MINIOS_H

#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

#define QUEUE_SIZE 5

typedef struct {
    int job_id;
} job_t;

typedef struct {
    job_t jobs[QUEUE_SIZE];
    int front;
    int rear;

    sem_t *empty;
    sem_t *full;

    pthread_mutex_t mutex;
} job_queue_t;

/* Queue functions */
int queue_init(job_queue_t *q);
int queue_push(job_queue_t *q, job_t job);
int queue_pop(job_queue_t *q, job_t *job);
void queue_destroy(job_queue_t *q);

#endif

