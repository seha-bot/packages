#ifndef SEHA_NIC
#define SEHA_NIC

// NO INDEX CONTAINER
//
// Balanced binary tree.
// Made as a base for sets and maps.
// Conflict resolution hasn't been implemented yet.
// Keys under 10 characters will work fine.
// Supported key chars '!' - '~'

#include <stddef.h>
#include "nec.h"

typedef struct
{
    size_t l, r;
    size_t hash;
    int height;
} nic;

size_t nic_find(const nic*, size_t, size_t);
size_t nic_set(nic**, size_t*, size_t);

size_t nic_hash(const char*);

// Set functions aren't currently included in nic_type
// because I don't like the implementation.
// nic_set should also have a compare function passed to it.
// If we had classes it would all be so much easier

#define nic_define(type, name) \
typedef struct \
{ \
    nic* nodes; \
    size_t root; \
    type* values; \
    char** keys; \
} nic_ ## name; \
nic_ ## name nic_init_ ## name() \
{ \
    return (nic_ ## name){ 0, 0, 0, 0 }; \
} \
int nic_map_ ## name(nic_ ## name * tree, const char* key, type value) \
{ \
    size_t nodeId; \
    if((nodeId = nic_set(&tree->nodes, &tree->root, nic_hash(key)))) \
    { \
        tree->values[nodeId - 1] = value; \
        return 0; \
    } \
    nec_push(tree->values, value); \
    char* keyCopy = 0; \
    while(*key) nec_push(keyCopy, *key++); \
    nec_push(keyCopy, 0); \
    nec_push(tree->keys, keyCopy); \
    return 1; \
} \
type* nic_map_find_ ## name(const nic_ ## name * tree, const char* key) \
{ \
    size_t id = nic_find(tree->nodes, tree->root, nic_hash(key)); \
    if(id == 0) return 0; \
    return &tree->values[id - 1]; \
} \
void nic_clear_ ## name(nic_ ## name * tree) \
{ \
    nec_free(tree->nodes); \
    nec_free(tree->values); \
    for(int i = 0; i < nec_size(tree->keys); i++) \
    { \
        nec_free(tree->keys[i]); \
    } \
    nec_free(tree->keys); \
    tree->root = 0; \
}

#endif /* SEHA_NIC */

