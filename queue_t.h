#ifndef QUEUE_T_H
#define QUEUE_T_H

#include <stdlib.h>
#include <pthread.h>

typedef struct queue_element_t {
    void *next;
    int value;
} queue_element_t;

typedef struct queue_t {
    queue_element_t *head;
    queue_element_t *tail;
    pthread_mutex_t *mutex;
    unsigned int size;
} queue_t;

extern queue_t* create();
extern void destroy(queue_t *q);
extern void push(queue_t *q, int elem);
extern int pop(queue_t *q);

#endif
