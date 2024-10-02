#pragma once
#ifndef _VECTOR_H
#define _VECTOR_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>



typedef char vec_Blob; // pointer arithmetic on voidptrs is undefined => increment by 1 byte via char

typedef struct {
    size_t capacity,
           size,
           element_size,
           growth_rate;

    vec_Blob *blob; // using char* instead of void* for pointer arith

} vec_Vector;

extern void  vec_init          (vec_Vector *v, size_t element_size, size_t start_capacity, size_t growth_rate);
extern void  vec_push          (vec_Vector *v, void *value);
extern void* vec_get           (vec_Vector *v, size_t index);
extern void  vec_set           (vec_Vector *v, size_t index, void *value);
extern void  vec_delete        (vec_Vector *v, size_t index);
extern void  vec_insert_before (vec_Vector *v, size_t index, void *value);
extern void  vec_insert_after  (vec_Vector *v, size_t index, void *value);
extern void* vec_pop           (vec_Vector *v);



#endif // _VECTOR_H
