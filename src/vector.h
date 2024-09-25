#pragma once
#ifndef _VECTOR_H
#define _VECTOR_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>



// Functions: vector_<T>_<func>
// Struct: Vector_<T>
#define MAKE_VECTOR(T)                                         \
                                                               \
typedef struct {                                               \
    size_t capacity, size;                                     \
    T *data;                                                   \
} Vector_##T;                                                  \
                                                               \
void vector_##T##_init(Vector_##T *v) {                        \
    v->size = 0;                                               \
    v->capacity = 5;                                           \
    v->data = malloc(sizeof(T));                               \
}                                                              \
                                                               \
void vector_##T##_append(Vector_##T *v, T data) {              \
                                                               \
    if (v->size+1 == v->capacity) {                            \
        v->capacity *= 2;                                      \
        v->data = realloc(v->data, v->capacity * sizeof(T));   \
    }                                                          \
                                                               \
    v->data[v->size++] = data;                                 \
}



#endif // _VECTOR_H
