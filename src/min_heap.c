#include "min_heap.h"

#define DEFAULT_CAPACITY 10
#define MIN_ELEMENT 0

#define MIN_HEAP_OK 0
#define MIN_HEAP_ERROR -1

#define LEFT_CHILD(i) (2 * i + 1)
#define RIGHT_CHILD(i) (2 * i + 2)
#define PARENT(i) ((int)floor((i - 1) / 2))

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

struct minheap_t{
    char* data;
    size_t capacity;
    size_t size;
    size_t data_size;
    int(*compare)(void* p1, void* p2);
};

int _resize(size_t new_capacity, struct minheap_t* mh){
    void* tmp;
    if(!(tmp = realloc(mh->data, new_capacity * mh->data_size))){
        fprintf(stderr, "ERROR: INTERNAL DATA REALLOCATION FAILED.\n");
        return MIN_HEAP_ERROR;
    }
    mh->data = tmp;
    mh->capacity = new_capacity;
    return MIN_HEAP_OK;
}

int _swap(size_t i, size_t j, struct minheap_t* mh){
    void* temp;
    if(!(temp = malloc(mh->data_size))){
        return MIN_HEAP_ERROR;
    }
    memcpy(temp, mh->data + (mh->data_size * i), mh->data_size);
    memcpy(mh->data + (mh->data_size * i), mh->data + (mh->data_size * j), mh->data_size);
    memcpy(mh->data + (mh->data_size * j), temp, mh->data_size);
    free(temp);
    return MIN_HEAP_OK;
}

int _heapify_up(size_t i, struct minheap_t* mh){
    if(i > 0){
        size_t j = PARENT(i);
        if(mh->compare((mh->data + (mh->data_size * i)), (mh->data) + (mh->data_size * j)) < 0){
            if(_swap(i, j, mh) != MIN_HEAP_OK){
                return MIN_HEAP_ERROR;
            }
            return _heapify_up(j, mh);
        }
    }
    return MIN_HEAP_OK;
}

int _heapify_down(size_t i, minheap_t* mh){
    if(LEFT_CHILD(i) < mh->size){
        size_t left = LEFT_CHILD(i);
        size_t right = RIGHT_CHILD(i);
        size_t j = left;
        if(right < mh->size && mh->compare(mh->data + (mh->data_size * right), mh->data + (mh->data_size * left)) < 0){
            j = right;
        }
        if(mh->compare(mh->data + (mh->data_size * j), mh->data + (mh->data_size * i)) < 0){
            if(_swap(i, j, mh) != MIN_HEAP_OK){
                return MIN_HEAP_ERROR;
            }
            return _heapify_down(j, mh);
        }
    }
    return MIN_HEAP_OK;
}

struct minheap_t* minheap_init(size_t data_size, int(*compare)(void* p1, void* p2)){
    minheap_t* out;
    if(!(out = malloc(sizeof(*out)))){
        fprintf(stderr, "ERROR: MIH_HEAP ALLOCATION FAILED.\n");
        return NULL;
    }
    if(!(out->data = malloc(DEFAULT_CAPACITY * data_size))){
        free(out);
        fprintf(stderr, "ERROR: INTERNAL DATA ALLOCATION FAILED.\n");
        return NULL;
    }
    out->compare = compare;
    out->data_size = data_size;
    out->size = 0;
    out->capacity = DEFAULT_CAPACITY;
    return out;
}

struct minheap_t* minheap_make_heap(size_t data_size, void* array, size_t array_size, int(*compare)(void* p1, void* p2)){
    minheap_t* out;
    if(!(out = malloc(sizeof(*out)))){
        fprintf(stderr, "ERROR: MIH_HEAP ALLOCATION FAILED.\n");
        return NULL;
    }
    size_t capacity = array_size < DEFAULT_CAPACITY ? DEFAULT_CAPACITY : array_size;
    if(!(out->data = malloc(capacity * data_size))){
        free(out);
        fprintf(stderr, "ERROR: INTERNAL DATA ALLOCATION FAILED.\n");
        return NULL;
    }
    memcpy(out->data, array, array_size * data_size);
    out->compare = compare;
    out->data_size = data_size;
    out->size = array_size;
    out->capacity = capacity;
    size_t i = array_size / 2;
    while(i-- > 0){
        if(_heapify_down(i, out) != MIN_HEAP_OK){
            free(out->data);
            free(out);
            return NULL;
        }
    }
    return out;
}

void minheap_destroy(minheap_t* mh){
    free(mh->data);
    free(mh);
}

int minheap_insert(void* x, struct minheap_t* mh){
    if(mh->capacity <= mh->size){
        if(_resize(2 * mh->capacity, mh) != MIN_HEAP_OK){
            return MIN_HEAP_ERROR;
        }
    }
    memcpy(mh->data + (mh->data_size * mh->size), x, mh->data_size);
    mh->size++;
    if(_heapify_up(mh->size - 1, mh) != MIN_HEAP_OK){
        return MIN_HEAP_ERROR;
    }
    return MIN_HEAP_OK;
}

void* minheap_extract_min(struct minheap_t* mh){
    if(mh->size == 0){
        return NULL;
    }
    void* out = malloc(mh->data_size);
    if(!out){
        return NULL;
    }
    memcpy(out, mh->data + (mh->data_size * MIN_ELEMENT), mh->data_size);
    _swap(MIN_ELEMENT, mh->size - 1, mh);
    mh->size--;
    _heapify_down(MIN_ELEMENT, mh);
    return out;
}

void* minheap_peek(struct minheap_t* mh){
    if(mh->size == 0){
        return NULL;
    }
    return mh->data + (mh->data_size * MIN_ELEMENT);
}

int minheap_is_empty(struct minheap_t* mh){
    return mh->size == 0;
}

char* minheap_to_string(const minheap_t* mh, void (*elem_to_string)(const void* elem, char* buf, size_t buf_size)) {
    if (!mh) return NULL;

    if (mh->size == 0) {
        char* empty_str = malloc(3);
        if (empty_str) strcpy(empty_str, "[]");
        return empty_str;
    }

    size_t capacity = 128;
    char* str = malloc(capacity);
    if (!str) return NULL;

    str[0] = '[';
    str[1] = '\0';

    size_t i = 0;
    char elem_buf[64];

    while (i < mh->size) {
        const void* elem_ptr = mh->data + (i * mh->data_size);

        elem_to_string(elem_ptr, elem_buf, sizeof(elem_buf));

        size_t current_len = strlen(str);
        size_t needed = strlen(elem_buf) + (i < mh->size - 1 ? 2 : 1) + 1;

        while (current_len + needed >= capacity) {
            capacity *= 2;
            char* new_str = realloc(str, capacity);
            if (!new_str) {
                free(str);
                return NULL;
            }
            str = new_str;
        }

        strcat(str, elem_buf);
        if (i < mh->size - 1) {
            strcat(str, ", ");
        }

        i++;
    }

    strcat(str, "]");
    return str;
}


