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

typedef struct
{
    size_t l, r;
    size_t hash;
    int height;
} nic;

size_t nic_find(const nic*, size_t, size_t);
int nic_set(nic**, size_t*, size_t);

size_t nic_hash(const char*);

// Set functions aren't currently included in nic_type
// because I don't like the implementation.
// nic_set should also have a compare function passed to it.
// If we had classes it would all be so much easier

#define nic_type(type) \
typedef struct { nic* nodes; type* values; size_t root; } nic_ ## type; \
nic_ ## type nic_init_ ## type() \
{ \
    return (nic_ ## type){ 0, 0, 0}; \
} \
int nic_map_ ## type(nic_ ## type * tree, const char* key, type value) \
{ \
    if(!nic_set(&tree->nodes, &tree->root, nic_hash(key))) return 0; \
    nec_push(tree->values, value); \
    return 1; \
} \
type* nic_map_find_ ## type(const nic_ ## type * tree, const char* key) \
{ \
    size_t id = nic_find(tree->nodes, tree->root, nic_hash(key)); \
    if(id == 0) return 0; \
    return &tree->values[id - 1]; \
} \
void nic_clear_ ## type(nic_ ## type * tree) \
{ \
    nec_free(tree->nodes); \
    nec_free(tree->values); \
    tree->root = 0; \
}

#endif /* SEHA_NIC */

