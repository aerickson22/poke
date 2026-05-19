#ifndef MINHEAP_H
#define MINHEAP_H

#include <stddef.h>

typedef struct minheap_t minheap_t;

minheap_t* minheap_init(size_t);

int minheap_destroy(minheap_t*);

int minheap_size(minheap_t*);

int minheap_is_empty(minheap_t*);

int minheap_insert(void*, minheap_t*, int(*)(void* x, void* y));

void* minheap_remove(minheap_t*, int(*)(void* x, void* y));

void* minheap_peek(minheap_t*);

#endif
