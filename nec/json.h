#ifndef SEHA_JSON
#define SEHA_JSON

#include "nic.h"

// This is a very bad and slow way
// to store keys and values.
// Instead, nic should have a nic_entry
// which holds a key and a value.
typedef struct
{
    nic keys;
    nic ints;
    nic doubles;
    nic strings;
    nic objects;
} json;

json json_init(void);
char* json_truncate(const char*);

void json_set_int(json*, const char*, int);
void json_set_double(json*, const char*, double);
void json_set_string(json*, const char*, const char*);
void json_set_object(json*, const char*, json);
int* json_get_int(const json*, const char*);
double* json_get_double(const json*, const char*);
char** json_get_string(const json*, const char*);
json* json_get_object(const json*, const char*);

char* json_write(const json*);
json json_read(const char*);

#define json_free(__json_d) \
( \
    nic_free((__json_d).keys), \
    nic_free((__json_d).ints), \
    nic_free((__json_d).doubles), \
    nic_free((__json_d).strings), \
    nic_free((__json_d).objects) \
)

#endif /* SEHA_JSON */

