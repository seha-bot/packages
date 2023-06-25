#include "nec.h"
#include <stdlib.h>
#include <stdio.h>

const size_t PREFIX = sizeof(size_t) * 2;
#define STORAGE_LEFT(array, offset) *((size_t*)array - offset)
#define ARRAY_START(array) (size_t*)array - ((array != 0) << 1)

size_t nec_size_(void** array)
{
    if(*array) return STORAGE_LEFT(*array, 2);
    return 0;
}

int reallocate(void** array, size_t valueSize)
{
    const size_t potencial = *array ? STORAGE_LEFT(*array, 1) << 1 : 32;
    const size_t block = PREFIX + valueSize * potencial;
    *array = (size_t*)realloc(ARRAY_START(*array), block) + 2;
    if(*array == 0) return 1;
    STORAGE_LEFT(*array, 1) = potencial;
    return 0;
}

void nec_push_(void** array, size_t valueSize)
{
    const size_t size = nec_size_(array);
    if(size == 0 || size == STORAGE_LEFT(*array, 1))
    {
        if(reallocate(array, valueSize) != 0)
        {
            printf("ERROR\n");
        }
    }
    STORAGE_LEFT(*array, 2) = size + 1;
}

void nec_pop_(void** array)
{
    const size_t size = nec_size_(array);
    if(size == 0) return;
    if(size == 1) nec_free_(array);
    else STORAGE_LEFT(*array, 2) = size - 1;
}

void nec_free_(void** array)
{
    free(ARRAY_START(*array));
}

