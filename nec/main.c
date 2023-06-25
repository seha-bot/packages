#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "nic.h"

int absi(int a)
{
    if(a < 0) return -a;
    return a;
}

int test_tree(nic* node, int* discovered)
{
    *discovered = *discovered + 1;
    int h1 = 0, h2 = 0;
    if(node->l)
    {
        h1 = node->l->h;
        if(node->l->hash >= node->hash) return 2;
        if(test_tree(node->l, discovered)) return 1;
    }
    if(node->r)
    {
        h2 = node->r->h;
        if(node->r->hash <= node->hash) return 2;
        if(test_tree(node->r, discovered)) return 1;
    }

    int diff = absi(h2 - h1);
    if(diff > 2) return 1;
    return 0;
}

int nic_assign(nic** node, size_t hash)
{
    nic* newNode = nic_insert_(*node, hash);
    if(!newNode) return 0;
    *node = newNode;
    return 1;
}

int main()
{
    nic* root = 0;

    srand(time(0));
    int values = 0;
    printf("Values: ");
    // nic_assign(&root, 1);
    // nic_assign(&root, 0);
    // nic_assign(&root, 4);
    // nic_assign(&root, 2);
    // nic_assign(&root, 5);
    // nic_assign(&root, 3);
    for(int i = 0; i < 10000; i++)
    {
        int hash = rand();
        //printf("%d ", hash);
        values += nic_assign(&root, hash);
        //printf("\nTree:\n");
        //nic_debug(root, 0);
    }

    //printf("\nTree:\n");
    //nic_debug(root, 0);

    const char* status = "good";
    int discovered = 0;
    int result = test_tree(root, &discovered);
    if(result == 1) status = "unbalanced";
    else if(result == 2) status = "value mismatch";
    if(discovered != values) status = "orphaned nodes";
    printf("\nTree status: %s.\n", status);
    printf("Created nodes: %d\n", values);
    printf("Discovered nodes: %d\n", discovered);
    return 0;
}

