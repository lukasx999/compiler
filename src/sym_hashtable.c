#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "sym_hashtable.h"

void
ht_init(HashTable *ht, size_t cap, size_t resize_diff, size_t grow_factor) {
    ht->capacity = cap;
    ht->buckets  = calloc(ht->capacity, sizeof(Node*));
    ht->resize_diff = resize_diff;
    ht->grow_factor = grow_factor;
}

static bool
compare_keys(Key_t first, Key_t second) {
    return (first.scope_level == second.scope_level) && (!strcmp(first.identifier, second.identifier));
}

int
ht_hash(HashTable *ht, Key_t key) {

    int sum = 0;

    // sum up the ascii representation of the characters of the string and multiply them by their position in the string
    for (size_t i = 0; i < strlen(key.identifier); ++i)
        sum += key.identifier[i] * i;

    sum *= key.scope_level+1; // no collisions on scope level 0

    return sum % ht->capacity;

}



static Node*
create_node(KVPair data) {
    Node *new = malloc(sizeof(Node));
    new->data = data;
    new->next = NULL;
    return new;
}



Value_t
ht_get(HashTable *ht, Key_t key) {
    int hsh = ht_hash(ht, key);

    assert((ht->buckets[hsh] != NULL) && "key does not exist!");

    Node *node = ht->buckets[hsh];

    if (!compare_keys(ht->buckets[hsh]->data.key, key))
        while (!compare_keys(node->data.key, key)) {
            node = node->next;
            assert((node != NULL) && "key not found");
        }


    return node->data.value;
}

bool
ht_exists(HashTable *ht, Key_t key) {
    return !!ht->buckets[ht_hash(ht, key)];
}


size_t
ht_get_elements(HashTable *ht) {
    // get amount of elements in the table
    // also considering collisions (linked list nodes)

    size_t counter = 0;

    for (size_t i = 0; i < ht->capacity; ++i) {
        if (ht->buckets[i] != NULL) {
            Node *current = ht->buckets[i];
            for (;;) {
                counter++;
                if (!current->next) break;
                current = current->next;
            }
        }
    }

    return counter;

}


static void
resize_table(HashTable *ht) {

    size_t elements = ht_get_elements(ht);

    KVPair *old_data = malloc(elements * sizeof(KVPair));
    size_t old_data_size = 0;

    // save the existing elements (kvpairs) of the table
    for (size_t i = 0; i < ht->capacity; ++i) {
        if (ht->buckets[i] == NULL) continue;

        Node *current = ht->buckets[i];
        for (;;) {
            old_data[old_data_size++] = current->data;
            if (current->next == NULL) break;
            current = current->next;
        }
    }

    // clear out buckets, and create new list of buckets
    ht_destroy(ht);
    ht_init(ht, ht->capacity * ht->grow_factor, ht->resize_diff, ht->grow_factor);

    // add old data to new table
    for (size_t i = 0; i < old_data_size; ++i)
        ht_insert(ht, old_data[i]);

    free(old_data);

}


void
ht_insert(HashTable *ht, KVPair data) {

    if (ht_get_elements(ht) >= ht->capacity - ht->resize_diff)
        resize_table(ht);

    int   hsh = ht_hash(ht, data.key);
    Node *new = create_node(data);

    if (ht->buckets[hsh] != NULL) { // hash collision
        Node *node = ht->buckets[hsh];
        while (node->next != NULL)
            node = node->next;
        node->next = new;
    }
    else
        ht->buckets[hsh] = new;

}



void
ht_print(HashTable *ht) {
    for (size_t i = 0; i < ht->capacity; ++i) {
        if (ht->buckets[i] == NULL) continue;

        printf("%lu: %s(%lu)", i,
               ht->buckets[i]->data.key.identifier,
               ht->buckets[i]->data.value.rbp_offset);

        if (ht->buckets[i]->next == NULL) { puts(""); continue; }
        Node *current = ht->buckets[i]->next;
            for (;;) {
                printf(" -> %s(%lu)", current->data.key.identifier, current->data.value.rbp_offset);
                if (current->next == NULL) break;
                current = current->next;
            }
        puts("");
    }
}


void
ht_destroy(HashTable *ht) {

    // free all of the nodes
    for (size_t i = 0; i < ht->capacity; ++i) {
        Node *current = ht->buckets[i];
        while (current != NULL) {
            Node *old = current;
            current = current->next;
            free(old); printf("free'd\n");
        }
    }

    free(ht->buckets);

}
