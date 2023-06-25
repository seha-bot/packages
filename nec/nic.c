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

nic* create(size_t hash)
{
    nic node = { 0, 0, hash, 1 };
    nic* nodep = 0;
    nec_push(nodep, node);
    return nodep;
}

void calc_height(nic* root)
{
    int h1 = 0, h2 = 0;
    if(root->l) h1 = root->l->h;
    if(root->r) h2 = root->r->h;
    root->h = (h1 > h2 ? h1 : h2) + 1;
}

nic* rot(nic* root, nic** chroot, nic** child, nic* a, nic* b)
{
    if(*child && (*child)->h == 1 && (*chroot)->h == 2 && a->hash < (*child)->hash && (*child)->hash < b->hash)
    {
        (*child)->l = a;
        (*child)->r = b;
        root->h = (*chroot)->h = 1;
        (*child)->h = 2;
        nic* what = *child;
        *child = *chroot = 0;
        return what;
    }

    nic* tmpChild = *child;
    nic* tmpChroot = *chroot;
    *child = root;
    *chroot = tmpChild;
    calc_height(root);
    calc_height(tmpChroot);
    return tmpChroot;
}

int abs(int a)
{
    if(a < 0) return -a;
    return a;
}

nic* nic_insert_(nic** root, size_t hash)
{
    if(!*root) return create(hash);

    if(hash > (*root)->hash)
    {
        nic* node = nic_insert_(&(*root)->r, hash);
        if(!node) return 0;
        (*root)->r = node;
    }
    else if(hash < (*root)->hash)
    {
        nic* node = nic_insert_(&(*root)->l, hash);
        if(!node) return 0;
        (*root)->l = node;
    }
    else return 0;

    calc_height(*root);

    nic* l = (*root)->l;
    nic* r = (*root)->r;

    int heightDiff = 0;
    if(l) heightDiff += l->h;
    if(r) heightDiff -= r->h;

    if(abs(heightDiff) < 2) return *root;

    if(heightDiff > 0) return rot(*root, &(*root)->l, &l->r, l, *root);
    return rot(*root, &(*root)->r, &r->l, *root, r);
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

