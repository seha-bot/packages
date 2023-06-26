#include "nic.h"
#include "nec.h"
#include <stdio.h>
#include <stdint.h>

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

void nic_calc_height(nic* nodes, nic* node)
{
    if(!nodes)
    {
        printf("ERROR\n");
        return;
    }
    if(node->l > nec_size(nodes))
    {
        printf("Serious problem L\n");
        return;
    }
    if(node->r > nec_size(nodes))
    {
        printf("Serious problem R\n");
        return;
    }
    int h1 = 0, h2 = 0;
    if(node->l) h1 = nodes[node->l - 1].height;
    if(node->r) h2 = nodes[node->r - 1].height;
    node->height = (h1 > h2 ? h1 : h2) + 1;
}

int abs(int a)
{
    if(a < 0) return -a;
    return a;
}

nic* nic_balance(nic* nodes, nic* root)
{
    if(!nodes)
    {
        printf("ERROR\n");
        return 0;
    }
    int heightDiff = 0;
    if(root->l) heightDiff += nodes[root->l - 1].height;
    if(root->r) heightDiff -= nodes[root->r - 1].height;

    if(abs(heightDiff) < 2) return root;

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
    // root->height--;
    // side->height++;
    return side;
}

int nic_insert_(nic** nodes, size_t* nodeId, size_t hash)
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
    if(!nic_insert_(nodes, &nextId, hash)) return 0;
    node = &((*nodes)[*nodeId - 1]);
    if(hash > node->hash) node->r = nextId;
    else node->l = nextId;

    nic_calc_height(*nodes, node);
    *nodeId = nic_balance(*nodes, node) - *nodes + 1;
    return 1;
}

void nic_debug(nic* nodes, size_t nodeId, char* path)
{
    if(!nodeId) return;
    nec_push(path, 0);
    printf("%s/%lu\n", path, nodes[nodeId - 1].hash);
    path[nec_size(path)-1] = 'l';
    nic_debug(nodes, nodes[nodeId - 1].l, path);
    path[nec_size(path)-1] = 'r';
    nic_debug(nodes, nodes[nodeId - 1].r, path);
    nec_pop(path);
}

int test_tree(nic* nodes, size_t nodeId, int* discovered)
{
    nic node = nodes[nodeId - 1];
    *discovered = *discovered + 1;
    int h1 = 0, h2 = 0;
    if(node.l)
    {
        h1 = nodes[node.l - 1].height;
        if(nodes[node.l - 1].hash >= node.hash) return 2;
        int status = test_tree(nodes, node.l, discovered);
        if(status != 0) return status;
    }
    if(node.r)
    {
        h2 = nodes[node.r - 1].height;
        if(nodes[node.r - 1].hash <= node.hash) return 2;
        int status = test_tree(nodes, node.r, discovered);
        if(status != 0) return status;
    }

    int diff = abs(h2 - h1);
    if(diff >= 2) return 1;
    return 0;
}

