#include <stdio.h>
#include <stdlib.h>
#include "queue_t.h"

#define MAX 40

void* producer_1();
void* producer_2();
void* consumer();

queue_t *q;
int producer_count_1 = 0;
int producer_count_2 = 0;
int consumer_count = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t data_not_produced = PTHREAD_COND_INITIALIZER;
pthread_cond_t data_not_consumed = PTHREAD_COND_INITIALIZER;

void* producer_1() {
    srand(time(NULL));

    while (1) {
        pthread_mutex_lock(&mutex);

        if ((producer_count_1 + producer_count_2) < MAX) {
            int num = rand() % 10 + 1;

            push(q, num);
            producer_count_1++;

            printf("Producer 1: %d\n", num);
	    fflush(stdout);

            pthread_cond_signal(&data_not_produced);
        }
        else if ((producer_count_1 + producer_count_2) >= MAX) {
            pthread_mutex_unlock(&mutex);
            return NULL;
        }
        else {
            printf("Producer 1 is in wait..\n");
            pthread_cond_wait(&data_not_consumed, &mutex);
        }

        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void* producer_2() {
    srand(time(NULL));

    while (1) {
        pthread_mutex_lock(&mutex);

        if ((producer_count_1 + producer_count_2) < MAX) {
            int num = rand() % 20 + 1;
	    push(q, num);
            producer_count_2++;

            printf("Producer 2: %d\n", num);
	    fflush(stdout);

	    pthread_cond_signal(&data_not_produced);
        }
        else if ((producer_count_1 + producer_count_2) >= MAX) {
            pthread_mutex_unlock(&mutex);
            return NULL;
        }
        else {
            printf("Producer 2 is in wait..\n");
            pthread_cond_wait(&data_not_consumed, &mutex);
        }

        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void* consumer() {
    while (1) {

	pthread_mutex_lock(&mutex);

	if (q->size > 0) {

	    int data = pop(q);

	    consumer_count++;

	    printf("Data consumed: %d\n", data);
	    fflush(stdout);

	    pthread_cond_broadcast(&data_not_consumed);
	}
	else if (consumer_count >= MAX) {
	    pthread_mutex_unlock(&mutex);
	    return NULL;
	}
	else {
	    printf("Consumer is in wait..\n");
	    pthread_cond_wait(&data_not_produced, &mutex);
	}

	pthread_mutex_unlock(&mutex);
    }
}

int main() {
    q = create();

    pthread_t producer_thread_1,
	      producer_thread_2,
	      consumer_thread;
    int ret_producer_1,
	ret_producer_2,
	ret_consumer;

    ret_producer_1 = pthread_create(&producer_thread_1, NULL, producer_1, NULL);
    ret_producer_2 = pthread_create(&producer_thread_2, NULL, producer_2, NULL);
    ret_consumer = pthread_create(&consumer_thread, NULL, consumer, NULL);

    if (!ret_producer_1)
        pthread_join(producer_thread_1, NULL);
    if (!ret_producer_2)
        pthread_join(producer_thread_2, NULL);
    if (!ret_consumer)
        pthread_join(consumer_thread, NULL);

    destroy(q);
    return 0;
}
