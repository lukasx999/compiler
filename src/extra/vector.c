#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>

#include "vector.h"



void
vec_init(vec_Vector *v, size_t element_size,
         size_t start_capacity, size_t growth_rate) {

    v->size         = 0;
    v->capacity     = start_capacity;
    v->element_size = element_size;
    v->growth_rate  = growth_rate;
    v->blob         = malloc(v->capacity * v->element_size);

}

void
vec_push(vec_Vector *v, void *value) {

    if (v->size+1 == v->capacity) {
        v->capacity *= v->growth_rate;
        v->blob = realloc(v->blob, v->capacity * v->element_size);
    }

    void *dest = v->blob + v->size * v->element_size;
    memcpy(dest, value, v->element_size);
    ++v->size;

}

void*
vec_get(vec_Vector *v, size_t index) {

    assert(index < v->size); // assertion will fail on negative indices (size_t is unsigned: -1 => very large number)

    return v->blob + index * v->element_size;

}

void
vec_set(vec_Vector *v, size_t index, void *value) {

    assert(index < v->size);

    void *dest = v->blob + index * v->element_size;
    memcpy(dest, value, v->element_size);

}

void
vec_delete(vec_Vector *v, size_t index) {

    assert(index < v->size);

    void *dest      = v->blob + index     * v->element_size;
    const void *src = v->blob + (index+1) * v->element_size;
    size_t n        = (v->size - index+1) * v->element_size;

    memmove(dest, src, n);
    --v->size;

}

void
vec_insert_before(vec_Vector *v, size_t index, void *value) {

    assert(index < v->size);

    ++v->capacity;
    v->blob = realloc(v->blob, v->capacity * v->element_size);

    void *dest      = v->blob + (index+1) * v->element_size;
    const void *src = v->blob + index     * v->element_size;
    size_t n        = (v->size - index+1) * v->element_size;

    memmove(dest, src, n);
    ++v->size;

    vec_set(v, index, value);

}

void
vec_insert_after(vec_Vector *v, size_t index, void *value) {

    assert(index < v->size);

    ++v->capacity;
    v->blob = realloc(v->blob, v->capacity * v->element_size);

    void *dest      = v->blob + (index+2) * v->element_size;
    const void *src = v->blob + (index+1)     * v->element_size;
    size_t n        = (v->size - index+1) * v->element_size;

    memmove(dest, src, n);
    ++v->size;

    vec_set(v, index+1, value);

}

void*
vec_pop(vec_Vector *v) {
    return v->blob + --v->size * v->element_size;
}
