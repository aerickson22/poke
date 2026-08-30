/**
 * @file min_heap.h
 * @brief Generic binary min-heap (priority queue) interface.
 *
 * This header declares an opaque, generic min-heap capable of storing
 * fixed-size elements of any type. The caller supplies a comparison
 * function appropriate for the element type at initialization time,
 * allowing the same implementation to be reused for any data type. The
 * heap root always holds the "smallest" element as determined by the
 * comparison function.
 *
 * Typical usage:
 * @code
 * minheap_t* mh = minheap_init(sizeof(int), int_compare);
 * int a = 5, b = 2, c = 8;
 * minheap_insert(&a, mh);
 * minheap_insert(&b, mh);
 * minheap_insert(&c, mh);
 * int* min = (int*)minheap_extract_min(mh); // -> points to value 2
 * minheap_destroy(mh);
 * @endcode
 */

#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/**
 * @brief Opaque min-heap type.
 *
 * The internal structure is not exposed to callers; all interaction with
 * the heap must go through the functions declared in this header.
 */
typedef struct minheap_t minheap_t;

/**
 * @brief Create and initialize a new, empty min-heap.
 *
 * @param data_size Size, in bytes, of a single element stored in the heap.
 * @param compare   Function that compares two elements (@p p1, @p p2) and
 *                   returns a negative value if @p p1 is smaller, zero if
 *                   they are equal, and a positive value if @p p1 is
 *                   larger. This ordering determines which element rises
 *                   to the root (the minimum).
 *
 * @return Pointer to a newly allocated, empty minheap_t, or NULL on
 *         allocation failure.
 *
 * @note The caller is responsible for eventually calling minheap_destroy()
 *       to free the returned heap.
 */
minheap_t* minheap_init(size_t data_size, int (*compare)(void* p1, void* p2));

/**
 * @brief Create a min-heap pre-populated from an existing array.
 *
 * Builds a new heap containing copies of the elements in @p array,
 * arranging them to satisfy the min-heap property (typically via a
 * linear-time "heapify" build process) rather than inserting them one
 * at a time.
 *
 * @param data_size  Size, in bytes, of a single element.
 * @param array      Pointer to the source array of elements to heapify.
 * @param array_size Number of elements in @p array.
 * @param compare    Comparison function, as described in minheap_init().
 *
 * @return Pointer to a newly allocated minheap_t containing the heapified
 *         elements, or NULL on allocation failure.
 *
 * @note The caller is responsible for eventually calling minheap_destroy()
 *       to free the returned heap. The original @p array is not modified
 *       unless the implementation documents otherwise.
 */
minheap_t* minheap_make_heap(size_t data_size, void* array, size_t array_size,
                              int (*compare)(void* p1, void* p2));

/**
 * @brief Destroy a min-heap and free all associated memory.
 *
 * Releases all internal storage used by the heap, including any stored
 * elements. After this call, @p mh must not be used again.
 *
 * @param mh Pointer to the heap to destroy.
 */
void minheap_destroy(minheap_t* mh);

/**
 * @brief Insert an element into the heap.
 *
 * Copies @p x (data_size bytes, as set at init) into the heap and restores
 * the min-heap property.
 *
 * @param x  Pointer to the element to insert.
 * @param mh Heap to insert into.
 *
 * @return Non-zero on success, 0 on failure (e.g. allocation failure while
 *         growing internal storage).
 */
int minheap_insert(void* x, minheap_t* mh);

/**
 * @brief Remove and return the minimum element from the heap.
 *
 * Removes the root element (the smallest, per the comparison function)
 * and restores the min-heap property among the remaining elements.
 *
 * @param mh Heap to extract from.
 *
 * @return Pointer to the removed minimum element, or NULL if the heap is
 *         empty.
 *
 * @warning Check the implementation to confirm the lifetime/ownership of
 *          the returned pointer (e.g. whether it is heap-allocated memory
 *          the caller must free, or a pointer into internal storage that
 *          becomes invalid after the next heap operation).
 */
void* minheap_extract_min(minheap_t* mh);

/**
 * @brief Retrieve the minimum element without removing it.
 *
 * @param mh Heap to inspect.
 *
 * @return Pointer to the minimum element, or NULL if the heap is empty.
 *
 * @warning The returned pointer refers to memory owned by the heap; do not
 *          free it directly, and treat it as invalid after any subsequent
 *          call that modifies or destroys the heap.
 */
void* minheap_peek(minheap_t* mh);

/**
 * @brief Check whether the heap contains no elements.
 *
 * @param mh Heap to check.
 *
 * @return Non-zero if the heap is empty, 0 otherwise.
 */
int minheap_is_empty(minheap_t* mh);

/**
 * @brief Produce a human-readable string representation of the heap.
 *
 * Formats the heap's contents into a newly allocated string, using
 * @p elem_to_string to render each individual element.
 *
 * @param mh             Heap to render. Not modified.
 * @param elem_to_string Callback that writes a textual representation of a
 *                        single element (@p elem) into @p buf, which is
 *                        @p buf_size bytes long. Implementations should
 *                        ensure they do not write past @p buf_size.
 *
 * @return Pointer to a newly heap-allocated, null-terminated string
 *         describing the heap's contents, or NULL on allocation failure.
 *
 * @note The caller is responsible for calling free() on the returned
 *       string once it is no longer needed.
 */
char* minheap_to_string(const minheap_t* mh,
                         void (*elem_to_string)(const void* elem, char* buf,
                                                 size_t buf_size));

#endif /* MIN_HEAP_H */
