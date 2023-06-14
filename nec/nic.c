#include "nic.h"
#include "nec.h"
#include <stdio.h>

size_t nic_hash(const char* s)
{
    size_t hash = 0, offset = 1;
    while(*s)
    {
        hash += (*(s++) - 31) * offset;
        offset *= 96;
    }
    return hash;
}

size_t nic_find_hash(const nicp* memo, const size_t root, const size_t hash)
{
    if(!root) return 0;
    const nicp* rp = memo + root - 1;

    if(hash > rp->hash) return nic_find_hash(memo, rp->r, hash);
    else if(hash < rp->hash) return nic_find_hash(memo, rp->l, hash);

    return root;
}

size_t create(nicp** memo, const size_t hash)
{
    nicp n = { 0, 0, hash, 1 };
    nec_push(*memo, n);
    return nec_size(*memo);
}

void calc_height(const nicp* memo, nicp* root)
{
    int h = 0;
    root->h = (memo + root->r - 1)->h;
    if(root->l) h = (memo + root->l - 1)->h;
    if(root->r) h = h > root->h ? h : root->h;
    root->h = h + 1;
}

size_t rot(nicp* memo, nicp* root, size_t* chroot, size_t* child, const nicp* a, const nicp* b)
{
    nicp *chr = memo + *chroot - 1, *chi = memo + *child - 1;
    if(chi && chi->h == 1 && chr->h == 2 && a->hash < chi->hash && chi->hash < b->hash)
    {
        chi->l = a - memo + 1;
        chi->r = b - memo + 1;
        *child = *chroot = 0;
        root->h = chr->h = 1;
        chi->h = 2;
        return chi - memo + 1;
    }

    *child = root - memo + 1;
    *chroot = chi - memo + 1;
    calc_height(memo, root);
    calc_height(memo, chr);
    return chr - memo + 1;
}

size_t nic_insert_hash(nicp** memo, const size_t root, size_t hash)
{
    if(!root) return create(memo, hash);
    nicp* rp = *memo + root - 1;

    if(hash > rp->hash)
    {
        hash = nic_insert_hash(memo, rp->r, hash);
        if(!hash) return 0;
        rp = *memo + root - 1;
        rp->r = hash;
    }
    else if(hash < rp->hash)
    {
        hash = nic_insert_hash(memo, rp->l, hash);
        if(!hash) return 0;
        rp = *memo + root - 1;
        rp->l = hash;
    }
    else return 0;

    calc_height(*memo, rp);

    nicp *l = *memo + rp->l - 1, *r = *memo + rp->r - 1;
    int hdiff = rp->l ? l->h : 0;
    if(rp->r) hdiff -= r->h;
    if(abs(hdiff) < 2) return root;
    if(hdiff > 0) return rot(*memo, rp, &rp->l, &l->r, l, rp);
    return rot(*memo, rp, &rp->r, &r->l, rp, r);
}

void nic_debug(const nicp* memo, const size_t root, char* path)
{
    if(!root) return;
    const nicp* rpa = memo + root - 1;
    nec_push(path, '\0');
    printf("%s/%lu\n", path, rpa->hash);
    path[nec_size(path)-1] = 'l';
    nic_debug(memo, rpa->l, path);
    path[nec_size(path)-1] = 'r';
    nic_debug(memo, rpa->r, path);
    nec_pop(path);
}

