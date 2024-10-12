#pragma once
#ifndef _HASHTABLE_H
#define _HASHTABLE_H

#include <stddef.h>

#include "symboltable.h"

typedef struct {
    char identifier[BUFSIZE];
    uint64_t scope_level;
} ht_Key_t;

typedef ht_Key_t Key_t;
typedef SymbolTableEntry Value_t;

typedef struct {
    Key_t key;
    Value_t value;
} KVPair;

typedef struct Node {
    KVPair data;
    struct Node *next;
} Node;


typedef struct {
    size_t capacity,
           resize_diff, // diff triggering resize
           grow_factor; // factor for the size of the new table after resizing
    Node **buckets; // NULL if empty
} HashTable;


extern void    ht_init        (HashTable *ht, size_t cap, size_t resize_diff, size_t grow_factor);
extern int     ht_hash        (HashTable *ht, Key_t  key );
extern void    ht_delete      (HashTable *ht, Key_t  key );
extern Value_t ht_get         (HashTable *ht, Key_t  key );
extern bool    ht_exists      (HashTable *ht, Key_t  key );
extern size_t  ht_get_elements(HashTable *ht             );
extern void    ht_insert      (HashTable *ht, KVPair data);
extern void    ht_destroy     (HashTable *ht);
extern void    ht_print       (HashTable *ht);



#endif // _HASHTABLE_H
