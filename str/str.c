#include "str.h"
#include "nec.h"
#include <ctype.h>
#include <string.h>
#include <stdarg.h>


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

char** str_split(const char* str, int includePatterns, int patternCount, ...)
{
    struct pattern
    {
        char* text;
        size_t size;
    };
    struct pattern* patterns = 0;

    va_list args;
    va_start(args, patternCount);
    for(int i = 0; i < patternCount; i++)
    {
        struct pattern pattern;
        pattern.text = va_arg(args, char*);
        pattern.size = strlen(pattern.text);
        nec_push(patterns, pattern);
    }
    va_end(args);

    int i = 0;
    char* shard = 0;
    char** shards = 0;
    while(str[i] != 0)
    {
        int size = -1, j = 0;
        for(; j < nec_size(patterns); j++)
        {
            if(strncmp(str + i, patterns[j].text, patterns[j].size) == 0)
            {
                size = patterns[j].size;
                break;
            }
        }
        if(size != -1)
        {
            i += size;
            if(shard)
            {
                nec_push(shard, 0);
                nec_push(shards, shard);
                shard = 0;
            }
            if(includePatterns)
            {
                nec_push(shards, str_cpy(patterns[j].text));
            }
            continue;
        }
        nec_push(shard, str[i]);
        i++;
    }
    if(shard != 0) nec_push(shards, shard);

    nec_free(patterns);
    return shards;
}

