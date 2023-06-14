#ifndef SEHA_NIC
#define SEHA_NIC

#include "nec.h"

// NO INDEX CONTAINER
//
// Balanced binary tree.
// Made as a base for sets and maps.
// Conflict resolution hasn't been implemented yet.
// Keys under 10 characters will work fine.
// Supported key chars '!' - '~'

struct nicp
{
    size_t l, r, hash;
    int h;
};
typedef struct nicp nicp;

typedef struct
{
    size_t root;
    nicp* memo;
    void* data;
} nic;

size_t nic_hash(const char*);
size_t nic_find_hash(const nicp*, const size_t, const size_t);
size_t nic_insert_hash(nicp**, const size_t, size_t);
void nic_debug(const nicp*, const size_t, char*);

// Find definitions
#define nic_find(__nic_a, __nic_k) nic_find_hash(__nic_a.memo, __nic_a.root, __nic_k)

#define nic_imap_find(__nic_t, __nic_a, __nic_k) \
({ \
    const size_t __nic_r = nic_find(__nic_a, __nic_k); \
    __nic_r ? (__nic_t*)__nic_a.data + __nic_r - 1 : 0; \
})

#define nic_map_find(__nic_t, __nic_a, __nic_k) nic_imap_find(__nic_t, __nic_a, nic_hash(__nic_k))

// Insert definitions
#define nic_insert(__nic_a, __nic_k) \
({ \
    const size_t __nic_r = nic_insert_hash(&__nic_a.memo, __nic_a.root, __nic_k); \
    __nic_r ? __nic_a.root = __nic_r : 0; \
})

#define nic_imap(__nic_a, __nic_k, __nic_v) \
({ \
    const size_t __nic_r = nic_insert_hash(&__nic_a.memo, __nic_a.root, __nic_k); \
    if(__nic_r) \
    { \
        __nic_a.root = __nic_r; \
        typeof(__nic_v)* __nic_p = __nic_a.data; \
        nec_push(__nic_p, __nic_v); \
        __nic_a.data = __nic_p; \
    } \
    else ((typeof(__nic_v)*)__nic_a.data)[nic_find(__nic_a, __nic_k) - 1] = __nic_v; \
})

#define nic_map(__nic_a, __nic_k, __nic_v) nic_imap(__nic_a, nic_hash(__nic_k), __nic_v)

// Free definition
#define nic_free(__nic_a) (nec_free(__nic_a.memo), nec_free(__nic_a.data))

#endif /* SEHA_NIC */

