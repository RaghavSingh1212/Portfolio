#include "queue.h"
#include <stdlib.h>
#include <pthread.h>

/** @struct queue
 *  @brief This structure holds all the details for the queue.
 */
struct queue {
    void **elements; // Array to hold elements in the queue
    int size; // Maximum size of the queue
    int count; // Current number of elements in the queue
    int head; // Index of the head of the queue
    int tail; // Index of the tail of the queue
    pthread_mutex_t lock; // Mutex for synchronizing access
    pthread_cond_t not_full; // Condition variable to wait for space
    pthread_cond_t not_empty; // Condition variable to wait for elements
};

queue_t *queue_new(int size) {
    queue_t *q = malloc(sizeof(queue_t));
    if (!q)
        return NULL;

    q->elements = malloc(sizeof(void *) * size);
    if (!q->elements) {
        free(q);
        return NULL;
    }

    q->size = size;
    q->count = 0;
    q->head = 0;
    q->tail = 0;
    pthread_mutex_init(&q->lock, NULL);
    pthread_cond_init(&q->not_full, NULL);
    pthread_cond_init(&q->not_empty, NULL);

    return q;
}

void queue_delete(queue_t **q) {
    if (q == NULL || *q == NULL)
        return;

    pthread_mutex_lock(&(*q)->lock);
    free((*q)->elements);
    pthread_mutex_unlock(&(*q)->lock);

    pthread_mutex_destroy(&(*q)->lock);
    pthread_cond_destroy(&(*q)->not_full);
    pthread_cond_destroy(&(*q)->not_empty);

    free(*q);
    *q = NULL;
}

bool queue_push(queue_t *q, void *elem) {
    if (q == NULL)
        return false;

    pthread_mutex_lock(&q->lock);
    while (q->count == q->size) {
        pthread_cond_wait(&q->not_full, &q->lock);
    }

    q->elements[q->tail] = elem;
    q->tail = (q->tail + 1) % q->size;
    q->count++;
    pthread_cond_signal(&q->not_empty);
    pthread_mutex_unlock(&q->lock);

    return true;
}

bool queue_pop(queue_t *q, void **elem) {
    if (q == NULL || elem == NULL)
        return false;

    pthread_mutex_lock(&q->lock);
    while (q->count == 0) {
        pthread_cond_wait(&q->not_empty, &q->lock);
    }

    *elem = q->elements[q->head];
    q->head = (q->head + 1) % q->size;
    q->count--;
    pthread_cond_signal(&q->not_full);
    pthread_mutex_unlock(&q->lock);

    return true;
}
