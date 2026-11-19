
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <errno.h>

pthread_mutex_t resource_mutex;

sem_t *low_has_lock;
sem_t *high_is_blocked;
sem_t *allow_low_release;

void wait_sem(sem_t *sem)
{
    while (sem_wait(sem) == -1) {
        if (errno != EINTR) {
            perror("sem_wait");
            exit(EXIT_FAILURE);
        }
    }
}

void *low_priority(void *arg)
{
    pthread_mutex_lock(&resource_mutex);

    printf("LOW: Acquired the resource lock.\n");

    sem_post(low_has_lock);

    printf("LOW: Holding the lock and waiting...\n");
    wait_sem(allow_low_release);

    printf("LOW: Releasing the resource lock.\n");
    pthread_mutex_unlock(&resource_mutex);

    return NULL;
}

void *high_priority(void *arg)
{
    wait_sem(low_has_lock);

    printf("HIGH: Trying to acquire the resource lock.\n");

    int result = pthread_mutex_trylock(&resource_mutex);

    if (result == EBUSY) {
        printf("HIGH: Resource is busy! I must wait.\n");

        sem_post(high_is_blocked);

        pthread_mutex_lock(&resource_mutex);
    } else if (result == 0) {
        printf("HIGH: Acquired the resource lock immediately.\n");
    } else {
        printf("HIGH: Mutex error: %d\n", result);
        return NULL;
    }

    printf("HIGH: Finally acquired the resource lock!\n");

    pthread_mutex_unlock(&resource_mutex);

    return NULL;
}

void *medium_priority(void *arg)
{
    wait_sem(high_is_blocked);

    printf("MEDIUM: Running while HIGH is waiting!\n");
    printf("MEDIUM: LOW cannot release the lock until it continues.\n");

    sem_post(allow_low_release);

    return NULL;
}

int main(void)
{
    pthread_t low, medium, high;

    const char *names[] = {
        "/minios_low_lock",
        "/minios_high_blocked",
        "/minios_allow_release"
    };

    sem_unlink(names[0]);
    sem_unlink(names[1]);
    sem_unlink(names[2]);

    low_has_lock = sem_open(names[0], O_CREAT | O_EXCL, 0600, 0);
    high_is_blocked = sem_open(names[1], O_CREAT | O_EXCL, 0600, 0);
    allow_low_release = sem_open(names[2], O_CREAT | O_EXCL, 0600, 0);

    if (low_has_lock == SEM_FAILED ||
        high_is_blocked == SEM_FAILED ||
        allow_low_release == SEM_FAILED) {
        perror("sem_open");
        return 1;
    }

    if (pthread_mutex_init(&resource_mutex, NULL) != 0) {
        printf("Mutex initialization failed.\n");
        return 1;
    }

    printf("=== PRIORITY INVERSION DEMONSTRATION ===\n\n");

    pthread_create(&low, NULL, low_priority, NULL);
    pthread_create(&high, NULL, high_priority, NULL);
    pthread_create(&medium, NULL, medium_priority, NULL);

    pthread_join(low, NULL);
    pthread_join(high, NULL);
    pthread_join(medium, NULL);

    printf("\nDemonstration completed successfully!\n");

    pthread_mutex_destroy(&resource_mutex);

    sem_close(low_has_lock);
    sem_close(high_is_blocked);
    sem_close(allow_low_release);

    sem_unlink(names[0]);
    sem_unlink(names[1]);
    sem_unlink(names[2]);

    return 0;
}

