#include "queue.h"

#include <stdio.h>
#include <stdlib.h>

struct queue {
    Queue_elem* elems;

    int first;
    int last;

    int max_length;
};

Queue queue_create(int max_length) {
    if (max_length <= 0) {
        return NULL;
    }

    Queue queue = malloc(sizeof(struct queue));
    queue->elems = calloc(1, max_length * sizeof(Queue_elem));
    queue->first = 0;
    queue->last = -1;
    queue->max_length = max_length;

    return queue;
}

int queue_is_empty(Queue queue) {
    if (!queue) {
        return -1;
    }

    return queue->last == -1;
}

void queue_insert(Queue queue, Queue_elem elem) {
    if (!queue || (queue->first == 0 && queue->last == queue->max_length) || (queue->last != -1 && queue->first == (queue->last + 1) % queue->max_length)) {
        return;
    }
    
    if (queue_is_empty(queue)) {
        queue->first = 0;
        queue->last = 0;
    } else {
        ++queue->last;
        queue->last %= queue->max_length;
    }
    queue->elems[queue->last] = elem;
}

Queue_elem queue_remove(Queue queue) {
    if (!queue || queue_is_empty(queue)) {
        return NULL;
    }

    Queue_elem elem = queue->elems[queue->first];

    if (queue->first == queue->last) {
        queue->last = -1;
    }

    ++queue->first;
    queue->first %= queue->max_length;

    return elem;
}

void queue_destroy(Queue* queue) {
    if (!queue || !(*queue)) {
        return;
    }

    free((*queue)->elems);
    free(*queue);
    *queue = NULL;
}