#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "nic.h"
#include "nec.h"

int main()
{
    nic* nodes = 0;
    size_t root = 0;

    // time_t seed = time(0);
    srand(1687802094);
    int values = 0;
    // printf("Values: ");
    // nic_assign(&root, 1);
    for(int i = 0; i < 1000000; i++)
    {
        int hash = rand();
        // printf("%d ", hash);
        values += nic_insert_(&nodes, &root, hash);
        // printf("\nTree:\n");
        // nic_debug(nodes, root, 0);
    }

    // printf("\nTree:\n");
    // nic_debug(nodes, root, 0);
    // printf("SEED = %lu\n", seed);
    const char* status = "good";
    int discovered = 0;
    int result = test_tree(nodes, root, &discovered);
    if(result == 1) status = "unbalanced";
    else if(result == 2) status = "value mismatch";
    //if(discovered != values) status = "orphaned nodes";
    printf("\nTree status: %s.\n", status);
    printf("Created nodes: %d\n", values);
    printf("Discovered nodes: %d\n", discovered);
    nec_free(nodes);
    return 0;
}

