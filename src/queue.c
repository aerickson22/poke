#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "queue.h"
#include "Constants.h"

struct queue_item_t {
    char* data;
    size_t data_size;
    struct queue_item_t* next;
};

struct queue_t {
    size_t size;
    struct queue_item_t* front;
};

queue_t* queue_init() {
    struct queue_t* out;
    if (!(out = malloc(sizeof(*out)))) {
        return NULL;
    }
    out->front = NULL;
    out->size = 0;
    return out;
}

int queue_destroy(struct queue_t* q) {
    struct queue_item_t* next;
    for (struct queue_item_t* i = q->front; i; i = next) {
        next = i->next;
        free(i->data);
        free(i);
    }
    free(q);
    return SUCCESS;
}

int queue_enqueue(void* item, size_t size, struct queue_t* q) {
    struct queue_item_t* newItem;
    if (!(newItem = malloc(sizeof(*newItem)))) {
        fprintf(stderr, "ERROR: Allocating memory for new queue item");
        return ERROR;
    }
    newItem->data_size = size;
    if (!(newItem->data = malloc(size))) {
        free(newItem);
        return ERROR;
    }
    memcpy(newItem->data, item, size);
    newItem->next = NULL;

    if (queue_is_empty(q)) {
        q->front = newItem;
    } else {
        struct queue_item_t* curr = q->front;
        while (curr->next) { curr = curr->next; }
        curr->next = newItem;
    }
    q->size++;
    return SUCCESS;
}

void* queue_dequeue(struct queue_t* q) {
    if (queue_is_empty(q)) {
        fprintf(stderr, "ERROR: Removing from empty queue");
        return NULL;
    }
    void* out;
    if (!(out = malloc(q->front->data_size))) {
        return NULL;
    }
    struct queue_item_t* to_be_deleted = q->front;
    memcpy(out, q->front->data, q->front->data_size);
    q->front = q->front->next;
    free(to_be_deleted->data);
    free(to_be_deleted);
    q->size--;
    return out;
}

void* queue_peek(struct queue_t* q) {
    return q->front->data;
}

int queue_is_empty(struct queue_t* q) {
    return q->size == 0;
}
