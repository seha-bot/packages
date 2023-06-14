#ifndef SEHA_NEC
#define SEHA_NEC

#include <stdlib.h>

#define nec_potencial_null(__nec_a) (*((size_t*)__nec_a - 1))
#define nec_potencial(__nec_a) (__nec_a ? nec_potencial_null(__nec_a) : 32)

#define nec_size_null(__nec_a) (*((size_t*)__nec_a - 2))
#define nec_size(__nec_a) (__nec_a ? nec_size_null(__nec_a) : 0)

#define nec_start(__nec_a) (size_t*)__nec_a - ((__nec_a != 0) << 1)

#define nec_realloc(__nec_a, __nec_op) \
({ \
    const size_t __nec_p = nec_potencial(__nec_a) __nec_op 1; \
    __nec_a = (void*)((size_t*)realloc(nec_start(__nec_a), \
    (sizeof(size_t) << 1) + sizeof(*__nec_a) * __nec_p) + 2); \
    nec_potencial_null(__nec_a) = __nec_p; \
})

#define nec_push(__nec_a, __nec_v) \
({ \
    const size_t __nec_s = nec_size(__nec_a); \
    if(!__nec_s || __nec_s == nec_potencial(__nec_a)) nec_realloc(__nec_a, <<); \
    nec_size_null(__nec_a) = __nec_s + 1; \
    (__nec_a)[__nec_s] = __nec_v; \
})

#define nec_pop(__nec_a) \
({ \
    const size_t __nec_s = nec_size(__nec_a); \
    if(__nec_s < 2) nec_free(__nec_a); else \
    { \
        if(__nec_s - 1 == nec_potencial(__nec_a) >> 1) nec_realloc(__nec_a, >>); \
        nec_size_null(__nec_a) = __nec_s - 1; \
    } \
})

#define nec_free(__nec_a) (free(nec_start(__nec_a)), __nec_a = 0)

#endif /* SEHA_NEC */

