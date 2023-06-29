#ifndef SEHA_STR
#define SEHA_STR

void str_append(char**, const char*);
char* str_cpy(const char*);
char* str_substr(const char*, int, int);
char* str_trim_left(const char*);
char* str_trim_right(const char*);
char* str_trim(const char*);
char** str_split(const char*, int includePatterns, int patternCount, ...);
int eval(const char*);

#endif /* SEHA_STR */

