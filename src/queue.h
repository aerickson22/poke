#ifndef QUEUE_H
#define QUEUE_H

/**
 * @file queue.h
 * @brief Generic FIFO queue that stores arbitrary heap-allocated elements.
 */

#include <stdlib.h>

/**
 * @brief Opaque handle for a FIFO queue.
 *
 * Elements are stored internally as copies; ownership of dequeued memory
 * passes to the caller.
 */
typedef struct queue_t queue_t;

/**
 * @brief Allocates and initialises an empty queue.
 *
 * @return Pointer to the new queue, or NULL if allocation fails.
 */
queue_t* queue_init();

/**
 * @brief Frees all memory owned by the queue, including any remaining elements.
 *
 * @param q  Pointer to the queue to destroy. Must not be NULL.
 * @return   SUCCESS (0) on success, ERROR (-1) if @p q is NULL.
 */
int queue_destroy(queue_t* q);

/**
 * @brief Copies @p size bytes from @p data and appends them to the back of the queue.
 *
 * The queue stores an internal copy; the caller retains ownership of @p data.
 *
 * @param data  Pointer to the data to enqueue. Must not be NULL.
 * @param size  Number of bytes to copy from @p data.
 * @param q     Pointer to the target queue. Must not be NULL.
 * @return      SUCCESS (0) on success, ERROR (-1) on allocation failure or
 *              invalid arguments.
 */
int queue_enqueue(void* data, size_t size, queue_t* q);

/**
 * @brief Removes and returns the element at the front of the queue.
 *
 * Ownership of the returned memory transfers to the caller, who must free it.
 *
 * @param q  Pointer to the queue. Must not be NULL.
 * @return   Pointer to the dequeued data, or NULL if the queue is empty.
 */
void* queue_dequeue(queue_t* q);

/**
 * @brief Returns a pointer to the front element without removing it.
 *
 * The returned pointer is valid only until the next mutating queue operation.
 *
 * @param q  Pointer to the queue. Must not be NULL.
 * @return   Pointer to the front element, or NULL if the queue is empty.
 */
void* queue_peek(queue_t* q);

/**
 * @brief Tests whether the queue contains any elements.
 *
 * @param q  Pointer to the queue. Must not be NULL.
 * @return   Non-zero (true) if the queue is empty, 0 (false) otherwise.
 */
int queue_is_empty(queue_t* q);

#endif /* QUEUE_H */
