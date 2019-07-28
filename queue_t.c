#include "queue_t.h"

queue_t* create() {
    queue_t *q = malloc(sizeof(*q));
    pthread_mutex_t *mutex = malloc(sizeof(*mutex));

    q->head = NULL;
    q->tail = NULL;
    q->mutex = mutex;
    q->size = 0;

    return q;
}

void destroy(queue_t *q) {
    free(q->mutex);
    free(q);
    q = NULL;
}

void push(queue_t *q, int elem) {
    queue_element_t *element = malloc(sizeof(*element));
    element->value = elem;
    element->next = NULL;

    pthread_mutex_lock(q->mutex);

    if (q->head == NULL) {
	q->head = element;
	q->tail = element;
    } else {
	queue_element_t *oldTail = q->tail;
	oldTail->next = element;
	q->tail = element;
    }

    q->size = q->size + 1;
    pthread_mutex_unlock(q->mutex);
}

int pop(queue_t *q) {
    pthread_mutex_lock(q->mutex);
    queue_element_t *head = q->head;

    if (head == NULL) {
	pthread_mutex_unlock(q->mutex);
	return -1;
    } else {
	q->head = head->next;

	int value = head->value;
	free(head);

	q->size = q->size - 1;
	pthread_mutex_unlock(q->mutex);
	return value;
    }
}

unsigned int get_size(queue_t *q) {
    return q->size;
}

