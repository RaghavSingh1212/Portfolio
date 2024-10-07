#include "rwlock.h"
#include <stdlib.h>
#include <pthread.h>

struct rwlock {
    pthread_mutex_t sync_mutex;
    pthread_cond_t cond_readers_allowed;
    pthread_cond_t cond_writers_allowed;
    int readers_active;
    int writers_active;
    int readers_queued;
    int writers_queued;
    int sequential_readers;
    PRIORITY lock_priority;
    int lock_n_way; // Used only for N_WAY priority
};

rwlock_t *rwlock_new(PRIORITY p, uint32_t n) {
    rwlock_t *lock = malloc(sizeof(rwlock_t));
    if (!lock)
        return NULL;
    pthread_mutex_init(&lock->sync_mutex, NULL);
    pthread_cond_init(&lock->cond_readers_allowed, NULL);
    pthread_cond_init(&lock->cond_writers_allowed, NULL);
    lock->readers_active = 0;
    lock->writers_active = 0;
    lock->readers_queued = 0;
    lock->writers_queued = 0;
    lock->sequential_readers = 0;
    lock->lock_priority = p;
    lock->lock_n_way = n;
    return lock;
}

void rwlock_delete(rwlock_t **lock) {
    pthread_mutex_lock(&(*lock)->sync_mutex);
    pthread_mutex_destroy(&(*lock)->sync_mutex);
    pthread_cond_destroy(&(*lock)->cond_readers_allowed);
    pthread_cond_destroy(&(*lock)->cond_writers_allowed);
    free(*lock);
    *lock = NULL;
}

void reader_lock(rwlock_t *lock) {
    pthread_mutex_lock(&lock->sync_mutex);
    lock->readers_queued++;
    while (lock->writers_active > 0 || (lock->lock_priority == WRITERS && lock->writers_queued > 0)
           || (lock->lock_priority == N_WAY && lock->sequential_readers >= lock->lock_n_way
               && lock->writers_queued > 0)) {
        pthread_cond_wait(&lock->cond_readers_allowed, &lock->sync_mutex);
    }
    lock->readers_queued--;
    lock->readers_active++;
    lock->sequential_readers++;
    pthread_mutex_unlock(&lock->sync_mutex);
}

void reader_unlock(rwlock_t *lock) {
    pthread_mutex_lock(&lock->sync_mutex);
    lock->readers_active--;
    if (lock->readers_active == 0 && lock->writers_queued > 0) {
        pthread_cond_signal(&lock->cond_writers_allowed);
    }
    pthread_mutex_unlock(&lock->sync_mutex);
}

void writer_lock(rwlock_t *lock) {
    pthread_mutex_lock(&lock->sync_mutex);
    lock->writers_queued++;
    while (lock->writers_active > 0 || lock->readers_active > 0
           || (lock->lock_priority == N_WAY && lock->sequential_readers < lock->lock_n_way
               && lock->readers_queued > 0)) {
        pthread_cond_wait(&lock->cond_writers_allowed, &lock->sync_mutex);
    }
    lock->writers_queued--;
    lock->writers_active++;
    lock->sequential_readers = 0; // Reset the counter after gaining writer lock
    pthread_mutex_unlock(&lock->sync_mutex);
}

void writer_unlock(rwlock_t *lock) {
    pthread_mutex_lock(&lock->sync_mutex);
    lock->writers_active--;
    if (lock->lock_priority == READERS || lock->lock_priority == N_WAY) {
        if (lock->readers_queued > 0) {
            pthread_cond_broadcast(&lock->cond_readers_allowed);
        } else if (lock->writers_queued > 0) {
            pthread_cond_signal(&lock->cond_writers_allowed);
        }
    } else {
        if (lock->writers_queued > 0) {
            pthread_cond_signal(&lock->cond_writers_allowed);
        } else if (lock->readers_queued > 0) {
            pthread_cond_broadcast(&lock->cond_readers_allowed);
        }
    }
    pthread_mutex_unlock(&lock->sync_mutex);
}
