#include "str.h"
#include "nec.h"
#include <ctype.h>

void str_append(char** out, const char* src)
{
    if(!src) return;
    char* string = *out;
    while(*src)
    {
        string[nec_size(string) - 1] = *src++;
        nec_push(string, 0);
    }
    *out = string;
}

char* str_cpy(const char* src)
{
    char* string = 0;
    nec_push(string, 0);
    str_append(&string, src);
    return string;
}

char* str_substr(const char* src, int start, int n)
{
    char* string = 0;
    for(int i = start; i < start + n; i++)
    {
        nec_push(string, src[i]);
    }
    nec_push(string, 0);
    return string;
}

char* str_trim_left(const char* src)
{
    while(*src && isspace(*src)) src++;
    char* trimmed = 0;
    while(*src) nec_push(trimmed, *src++);
    nec_push(trimmed, 0);
    return trimmed;
}

char* str_trim_right(const char* src)
{
    char* trimmed = 0;
    while(*src) nec_push(trimmed, *src++);
    while(trimmed && isspace(trimmed[nec_size(trimmed) - 1])) nec_pop(trimmed);
    nec_push(trimmed, 0);
    return trimmed;
}

char* str_trim(const char* src)
{
    char* left = str_trim_left(src);
    char* right = str_trim_right(left);
    nec_free(left);
    return right;
}
