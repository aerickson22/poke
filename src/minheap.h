#ifndef MINHEAP_H
#define MINHEAP_H

#include <stdio.h>
#include <stddef.h>

/**
 * @brief Opaque min-heap data structure.
 *
 * Stores elements of a fixed size in heap order, where the smallest
 * element (as defined by the comparator) is always at the root.
 * The internal layout is hidden; use the provided API to interact with it.
 */
typedef struct minheap_t minheap_t;

/**
 * @brief Initializes a new min-heap.
 *
 * Allocates and returns a new heap configured to store elements of
 * the given size. The heap starts empty with an initial capacity of 5,
 * and resizes automatically as elements are inserted.
 *
 * @param size  Size in bytes of each element.
 * @return      Pointer to the new heap, or NULL on allocation failure.
 */
minheap_t* minheap_init(size_t size);

/**
 * @brief Destroys a min-heap and frees all associated memory.
 *
 * Frees both the internal data buffer and the heap struct itself.
 * The pointer is invalid after this call.
 *
 * @param in  Heap to destroy.
 * @return    SUCCESS, or ERROR on failure.
 */
int minheap_destroy(minheap_t* in);

/**
 * @brief Returns the number of elements currently in the heap.
 *
 * @param in  Target heap.
 * @return    Number of elements.
 */
int minheap_size(minheap_t* in);

/**
 * @brief Returns whether the heap is empty.
 *
 * @param in  Target heap.
 * @return    1 if empty, 0 otherwise.
 */
int minheap_is_empty(minheap_t* in);

/**
 * @brief Inserts an element into the heap.
 *
 * Copies `data_size` bytes from `item` into the heap, then sifts up
 * to restore heap order. Resizes the heap if at capacity.
 *
 * @param item     Pointer to the element to insert.
 * @param in       Target heap.
 * @param compare  Comparator function. Should return negative if x < y,
 *                 zero if x == y, positive if x > y.
 * @return         SUCCESS, or ERROR on allocation failure.
 */
int minheap_insert(void* item, minheap_t* in, int(*compare)(void* x, void* y));

/**
 * @brief Removes and returns the minimum element from the heap.
 *
 * Copies the root element into a newly allocated buffer, replaces it
 * with the last element, then heapifies down to restore heap order.
 * The caller is responsible for freeing the returned pointer.
 *
 * @param in       Target heap.
 * @param compare  Comparator function. Should return negative if x < y,
 *                 zero if x == y, positive if x > y.
 * @return         Pointer to a heap-allocated copy of the minimum element,
 *                 or NULL if the heap is empty or on allocation failure.
 */
void* minheap_remove(minheap_t* in, int(*compare)(void* x, void* y));

/**
 * @brief Returns a pointer to the minimum element without removing it.
 *
 * The returned pointer points directly into the heap's internal buffer.
 * Do not free it. The pointer is invalidated by any subsequent insert,
 * remove, or resize operation.
 *
 * @param in  Target heap.
 * @return    Pointer to the minimum element, or NULL if the heap is empty.
 */
void* minheap_peek(minheap_t* in);

#endif
