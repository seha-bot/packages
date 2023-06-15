#ifndef SEHA_NEC
#define SEHA_NEC

#include <stddef.h>

#define nec_size(__nec_a) nec_size_((void**)&__nec_a)
#define nec_push(__nec_a, __nec_v) ( \
    nec_push_((void**)&__nec_a, sizeof(*__nec_a)), \
    __nec_a[nec_size(__nec_a) - 1] = __nec_v)
#define nec_pop(__nec_a) nec_pop_((void**)&__nec_a)
#define nec_free(__nec_a) (nec_free_((void**)&__nec_a), __nec_a = 0)

size_t nec_size_(void**);
void nec_push_(void**, size_t);
void nec_pop_(void**);
void nec_free_(void**);

#endif /* SEHA_NEC */

