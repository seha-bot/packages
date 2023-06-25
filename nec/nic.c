#include "nic.h"
#include "nec.h"
#include <stdio.h>

size_t nic_hash(const char* str)
{
    size_t hash = 0, offset = 1;
    while(*str)
    {
        hash += (*(str++) - 31) * offset;
        offset *= 96;
    }
    return hash;
}

nic* nic_create(size_t hash)
{
    nic node = { 0, 0, hash, 1 };
    nic* nodep = 0;
    nec_push(nodep, node);
    return nodep;
}

void nic_calc_height(nic* root)
{
    int h1 = 0, h2 = 0;
    if(root->l) h1 = root->l->h;
    if(root->r) h2 = root->r->h;
    root->h = (h1 > h2 ? h1 : h2) + 1;
}

int abs(int a)
{
    if(a < 0) return -a;
    return a;
}

nic* nic_balance(nic* root)
{
    int heightDiff = 0;
    if(root->l) heightDiff += root->l->h;
    if(root->r) heightDiff -= root->r->h;

    if(abs(heightDiff) < 2) return root;

    nic** side;
    nic** down;
    if(heightDiff > 0)
    {
        side = &root->l;
        down = &root->l->r;
    }
    else
    {
        side = &root->r;
        down = &root->r->l;
    }

    nic *sideVal = *side;
    if(!down) return root;
    nic *downVal = *down;
    nic* min = sideVal->hash < root->hash ? sideVal : root;
    nic* max = sideVal->hash > root->hash ? sideVal : root;
    if(downVal && downVal->hash > min->hash && downVal->hash < max->hash)
    {
        min->r = downVal->l;
        max->l = downVal->r;
        downVal->l = nic_balance(min);
        downVal->r = nic_balance(max);
        root->h = sideVal->h = 1;
        downVal->h = 2;
        return downVal;
    }

    *side = downVal;
    *down = root;
    nic_calc_height(root);
    nic_calc_height(sideVal);
    return sideVal;
}

nic* nic_insert_(nic* root, size_t hash)
{
    if(!root) return nic_create(hash);

    if(hash > root->hash)
    {
        nic* node = nic_insert_(root->r, hash);
        if(!node) return 0;
        root->r = node;
    }
    else if(hash < root->hash)
    {
        nic* node = nic_insert_(root->l, hash);
        if(!node) return 0;
        root->l = node;
    }
    else return 0;

    nic_calc_height(root);
    return nic_balance(root);
}

void nic_debug(const nic* root, char* path)
{
    if(!root) return;
    nec_push(path, 0);
    printf("%s/%lu\n", path, root->hash);
    path[nec_size(path)-1] = 'l';
    nic_debug(root->l, path);
    path[nec_size(path)-1] = 'r';
    nic_debug(root->r, path);
    nec_pop(path);
}

