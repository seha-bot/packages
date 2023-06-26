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

size_t nic_hash(const char*);
int nic_insert_(nic**, size_t*, size_t);

void nic_debug(nic*, size_t, char*);
int test_tree(nic* nodes, size_t nodeId, int* discovered);

#endif /* SEHA_NIC */

