#include "maths.h"
#include "str.h"
#include "nic.h"
#include <stdlib.h>
#include <ctype.h>

int maxi(int a, int b) { return a > b ? a : b; }
float maxf(float a, float b) { return a > b ? a : b; }

int mini(int a, int b) { return a < b ? a : b; }
float minf(float a, float b) { return a < b ? a : b; }

int absi(int a) { return a < 0 ? -a : a; }
float absf(float a) { return a < 0 ? -a : a; }

int doop(int value, int lastV, int a, char op, char lastO)
{
    if(lastO == '*' || lastO == '/')
    {
        if(op == '*') return value * a;
        if(op == '/') return value / a;
    }
    switch(op)
    {
    case '+': return value + a;
    case '-': return value - a;
    case '*': return lastV + (value - lastV) * a;
    case '/': return lastV + (value - lastV) / a;
    }
    return 0;
}

int eval_(char** shards, int* i)
{
    int value = 0;
    int lastV = 0;
    char op = '+';
    char lastO = '+';

    while((*i)++ < nec_size(shards))
    {
        int j = *i - 1;
        int what = lastV;
        char huh = lastO;
        switch(shards[j][0])
        {
        case ')': return value;
        case '(':
            lastV = value;
            lastO = op;
            value = doop(value, what, eval_(shards, i), op, huh);
            break;
        case '+':
        case '-':
        case '*':
        case '/':
            op = shards[j][0];
            break;
        default:
            lastV = value;
            lastO = op;
            value = doop(value, what, atoi(shards[j]), op, huh);
        }
        nec_free(shards[j]);
    }

    return value;
}

// TODO this doesn't belong here
int eval(const char* str)
{
    char* real = 0;
    while(*str)
    {
        if(!isspace(*str++)) nec_push(real, *(str - 1));
    }
    nec_push(real, 0);
    char** shards = str_split(real, 1, 6, "+", "-", "*", "/", "(", ")");
    nec_free(real);

    int i = 0;
    const int value = eval_(shards, &i);
    nec_free(shards);
    return value;
}

