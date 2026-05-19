#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>

typedef struct queue_t queue_t;

queue_t* queue_init();
int queue_destroy(queue_t*);
int queue_enqueue(void*, size_t, queue_t*);
void* queue_dequeue(queue_t*);
void* queue_peek(queue_t*);
int queue_is_empty(queue_t*);

#endif
