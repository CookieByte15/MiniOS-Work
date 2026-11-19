
#include "minios.h"
#include <stdio.h>
#include <errno.h>

int queue_init(job_queue_t *q)
{
    q->front = 0;
    q->rear = 0;

    sem_unlink("/minios_empty");
    sem_unlink("/minios_full");

    q->empty = sem_open("/minios_empty", O_CREAT | O_EXCL, 0600, QUEUE_SIZE);
    if (q->empty == SEM_FAILED)
        return -1;

    q->full = sem_open("/minios_full", O_CREAT | O_EXCL, 0600, 0);
    if (q->full == SEM_FAILED) {
        sem_close(q->empty);
        sem_unlink("/minios_empty");
        return -1;
    }

    if (pthread_mutex_init(&q->mutex, NULL) != 0) {
        sem_close(q->empty);
        sem_close(q->full);
        sem_unlink("/minios_empty");
        sem_unlink("/minios_full");
        return -1;
    }

    return 0;
}

int queue_push(job_queue_t *q, job_t job)
{
    if (sem_wait(q->empty) != 0)
        return -1;

    pthread_mutex_lock(&q->mutex);

    q->jobs[q->rear] = job;
    q->rear = (q->rear + 1) % QUEUE_SIZE;

    pthread_mutex_unlock(&q->mutex);

    sem_post(q->full);

    return 0;
}

int queue_pop(job_queue_t *q, job_t *job)
{
    if (sem_wait(q->full) != 0)
        return -1;

    pthread_mutex_lock(&q->mutex);

    *job = q->jobs[q->front];
    q->front = (q->front + 1) % QUEUE_SIZE;

    pthread_mutex_unlock(&q->mutex);

    sem_post(q->empty);

    return 0;
}

void queue_destroy(job_queue_t *q)
{
    sem_close(q->empty);
    sem_close(q->full);

    sem_unlink("/minios_empty");
    sem_unlink("/minios_full");

    pthread_mutex_destroy(&q->mutex);
}

