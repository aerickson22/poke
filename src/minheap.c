#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "minheap.h"
#include "Constants.h"

struct minheap_t{
    size_t capacity;
    char* data;
    size_t size;
    size_t data_size;
};

size_t _left_child_index(size_t parent_index){
    return 2 * parent_index + 1;
}

size_t _right_child_index(size_t parent_index){
    return 2 * parent_index + 2;
}

size_t _parent_index(size_t child_index){
    return (child_index - 1) / 2;
}

int _has_left_chid(size_t index, size_t size){
    return _left_child_index(index) < size;
}

int _has_right_child(size_t index, size_t size){
    return _right_child_index(index) < size;
}

int _has_parent(size_t index){
    return index != 0;
}

void* _left_child(int index, struct minheap_t* in){
    return (in->data) + (_left_child_index(index) * in->data_size);
}

void* _right_child(int index, struct minheap_t* in){
    return (in->data) + (_right_child_index(index) * in->data_size);
}

void* _parent(int index, struct minheap_t* in){
    return (in->data) + (_parent_index(index) * in->data_size);
}

int _resize(int new_capacity, struct minheap_t* in){
    char* new_data;
    if(!(new_data = malloc(in->data_size * new_capacity))){
        return ERROR;
    }
    memcpy(new_data, in->data, in->data_size * in->size);
    free(in->data);
    in->data = new_data;
    in->capacity = new_capacity;
    return SUCCESS;
}

void _heapify_down(struct minheap_t* in, int(*compare)(void*, void*)){
    int index = 0;
    while(_has_left_chid(index, in->size)){
        int smaller_child_index = _left_child_index(index);
        if(_has_right_child(index, in->size) && compare(_right_child(index, in), _left_child(index, in)) < 0){
            smaller_child_index = _right_child_index(index);
        }
        if(compare(in->data + (index * in->data_size), in->data + (smaller_child_index * in->data_size)) < 0){
            break;
        }else{
            char temp[in->data_size];
            memcpy(temp, in->data + (index * in->data_size), in->data_size);
            memcpy(in->data + (index * in->data_size), in->data + (smaller_child_index * in->data_size), in->data_size);
            memcpy(in->data + (smaller_child_index * in->data_size), temp, in->data_size);
        }
        index = smaller_child_index;
    }
}

void _sift_up(struct minheap_t* in, int(*compare)(void*, void*)){
    int index = in->size - 1;
    while(_has_parent(index) && compare(in->data + (index * in->data_size), in->data + (_parent_index(index) * in->data_size)) < 0){
        char temp[in->data_size];
        memcpy(temp, in->data + (_parent_index(index) * in->data_size), in->data_size);
        memcpy(in->data + (_parent_index(index) * in->data_size), in->data + (index * in->data_size), in->data_size);
        memcpy(in->data + (index * in->data_size), temp, in->data_size);
        index = _parent_index(index);
    }
}

struct minheap_t* minheap_init(size_t size){
    struct minheap_t* out;
    if(!(out = malloc(sizeof(*out)))){
        return NULL;
    }
    if(!(out->data = malloc(size * 5))){
        free(out);
        return NULL;
    }
    out->data_size = size;
    out->size = 0;
    out->capacity = 5;
    return out;
}

int minheap_destroy(struct minheap_t* in){
    free(in->data);
    free(in);
    return SUCCESS;
}

int minheap_size(struct minheap_t* in){
    return in->size;
}

int minheap_is_empty(struct minheap_t* in){
    return in->size == 0;
}

int minheap_insert(void* item, struct minheap_t* in, int(*compare)(void* x, void* y)){
    if(in->size >= in->capacity){
        if(_resize(in->capacity * 2, in) < 0){
            fprintf(stderr, "ERROR: FAILED TO RESIZE MIN HEAP");
            return ERROR;
        };
    }
    memcpy(in->data + (in->size * in->data_size), item, in->data_size);
    in->size++;
    _sift_up(in, compare);
    return SUCCESS;
}

void* minheap_remove(struct minheap_t* in, int(*compare)(void* x, void* y)){
    if(minheap_is_empty(in)){
        fprintf(stderr, "ERROR: Removing from empty heap");
        return NULL;
    }
    char* min_item;
    if(!(min_item = malloc(in->data_size))){
        fprintf(stderr, "ERROR fail to allocate min item");
        return NULL;
    }
    memcpy(min_item, in->data, in->data_size);
    memcpy(in->data, in->data + ((in->size - 1) * in->data_size), in->data_size);
    in->size--;
    _heapify_down(in, compare);
    return min_item;
}

void* minheap_peek(struct minheap_t* in){
    if(minheap_is_empty(in)){
        fprintf(stderr, "ERROR: Heap is empty");
        return NULL;
    }
    return in->data + (0 * in->data_size);
}

void minheap_do_something(struct minheap_t* in, void(*function)(void*)){
    for(size_t i = 0; i < in->size; i++){
        function(in->data + ((in->data_size) * i));
    }
}
