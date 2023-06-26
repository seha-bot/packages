#include "nic.h"
#include "nec.h"

size_t nic_find(const nic* nodes, size_t nodeId, size_t hash)
{
    nic node = nodes[nodeId - 1];
    if(hash > node.hash) return nic_find(nodes, node.r, hash);
    if(hash < node.hash) return nic_find(nodes, node.l, hash);
    return nodeId;
}

void nic_calc_height(nic* nodes, nic* node)
{
    int h1 = 0, h2 = 0;
    if(node->l) h1 = nodes[node->l - 1].height;
    if(node->r) h2 = nodes[node->r - 1].height;
    node->height = (h1 > h2 ? h1 : h2) + 1;
}

nic* nic_balance(nic* nodes, nic* root)
{
    int heightDiff = 0;
    if(root->l) heightDiff += nodes[root->l - 1].height;
    if(root->r) heightDiff -= nodes[root->r - 1].height;

    if(heightDiff < 2 && heightDiff > -2) return root;

    size_t* sideId;
    size_t* downId;
    if(heightDiff > 0)
    {
        sideId = &root->l;
        downId = &(nodes[root->l - 1].r);
    }
    else
    {
        sideId = &root->r;
        downId = &(nodes[root->r - 1].l);
    }

    nic* side = &(nodes[*sideId - 1]);
    nic* down = &(nodes[*downId - 1]);
    nic* min = side->hash < root->hash ? side : root;
    nic* max = side->hash > root->hash ? side : root;
    if(*downId && down->hash > min->hash && down->hash < max->hash)
    {
        min->r = down->l;
        max->l = down->r;
        nic_calc_height(nodes, min);
        nic_calc_height(nodes, max);
        down->l = nic_balance(nodes, min) - nodes + 1;
        down->r = nic_balance(nodes, max) - nodes + 1;
        nic_calc_height(nodes, down);
        return down;
    }

    *sideId = down - nodes + 1;
    *downId = root - nodes + 1;
    nic_calc_height(nodes, root);
    nic_calc_height(nodes, side);
    return side;
}

int nic_set(nic** nodes, size_t* nodeId, size_t hash)
{
    if(*nodeId == 0)
    {
        nic node = { 0, 0, hash, 1 };
        nec_push((*nodes), node);
        *nodeId = nec_size_((void**)nodes);
        return 1;
    }
    nic* node = &((*nodes)[*nodeId - 1]);
    if(hash == node->hash) return 0;

    size_t nextId = hash > node->hash ? node->r : node->l;
    if(!nic_set(nodes, &nextId, hash)) return 0;
    node = &((*nodes)[*nodeId - 1]);
    if(hash > node->hash) node->r = nextId;
    else node->l = nextId;

    nic_calc_height(*nodes, node);
    *nodeId = nic_balance(*nodes, node) - *nodes + 1;
    return 1;
}

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

