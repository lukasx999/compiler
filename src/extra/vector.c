#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"

void
vec_init(vec_Vector *v, size_t element_size,
         size_t start_capacity, size_t growth_rate) {

    assert(v != NULL);
    assert( element_size != 0);

    v->size         = 0;
    v->capacity     = start_capacity;
    v->element_size = element_size;
    v->growth_rate  = growth_rate;
    v->blob         = malloc(v->capacity * v->element_size);

}

void
vec_push(vec_Vector *v, void *value) {

    assert(v != NULL);
    assert(value != NULL);

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

    assert(v != NULL);
    assert(index < v->size); // assertion will fail on negative indices (size_t is unsigned: -1 => very large number)

    return v->blob + index * v->element_size;

}

void
vec_set(vec_Vector *v, size_t index, void *value) {

    assert(value != NULL);
    assert(v != NULL);
    assert(index < v->size);

    void *dest = v->blob + index * v->element_size;
    memcpy(dest, value, v->element_size);

}

void
vec_delete(vec_Vector *v, size_t index) {

    assert(v != NULL);
    assert(index < v->size);

    void *dest      = v->blob + index     * v->element_size;
    const void *src = v->blob + (index+1) * v->element_size;
    size_t n        = (v->size - index+1) * v->element_size;

    memmove(dest, src, n);
    --v->size;

}

void
vec_insert_before(vec_Vector *v, size_t index, void *value) {

    assert(v != NULL);
    assert(value != NULL);
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
    assert(v != NULL);
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
    assert(v != NULL);

    return v->blob + --v->size * v->element_size;

}


void
vec_extend(vec_Vector *this, vec_Vector *other) {
    assert(this != NULL);
    assert(other != NULL);
    assert(this->element_size == other->element_size); // both vectors must have the same type

    for (size_t i=0; i<other->size; ++i)
        vec_push(this, vec_get(other, i));

}


void
vec_destroy(vec_Vector *v) {
    free(v->blob);
}
