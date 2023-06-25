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

struct nic
{
    struct nic *l, *r;
    size_t hash;
    int h; // TODO rename to height
};
typedef struct nic nic;

size_t nic_hash(const char*);
nic* nic_insert_(nic*, size_t);

void nic_debug(const nic*, char*);

#endif /* SEHA_NIC */

