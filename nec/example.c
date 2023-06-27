#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "nic.h"
#include "nec.h"

// TODO: add nicer examples to the entire repository.
// This file is temporary

// void nic_debug(nic* nodes, size_t nodeId, char* path)
// {
//     if(!nodeId) return;
//     nec_push(path, 0);
//     printf("%s/%lu\n", path, nodes[nodeId - 1].hash);
//     path[nec_size(path)-1] = 'l';
//     nic_debug(nodes, nodes[nodeId - 1].l, path);
//     path[nec_size(path)-1] = 'r';
//     nic_debug(nodes, nodes[nodeId - 1].r, path);
//     nec_pop(path);
// }
//
//int test_tree(nic* nodes, size_t nodeId, int* discovered)
// {
//     nic node = nodes[nodeId - 1];
//     *discovered = *discovered + 1;
//     int h1 = 0, h2 = 0;
//     if(node.l)
//     {
//         h1 = nodes[node.l - 1].height;
//         if(nodes[node.l - 1].hash >= node.hash) return 2;
//         int status = test_tree(nodes, node.l, discovered);
//         if(status != 0) return status;
//     }
//     if(node.r)
//     {
//         h2 = nodes[node.r - 1].height;
//         if(nodes[node.r - 1].hash <= node.hash) return 2;
//         int status = test_tree(nodes, node.r, discovered);
//         if(status != 0) return status;
//     }
// 
//     int diff = abs(h2 - h1);
//     if(diff >= 2) return 1;
//     return 0;
// }

size_t nic_hash_double(double value)
{
    return value * 100000.0;
}

nic_define(double, real)

int main()
{
    nic_real map = nic_init_real();

    nic_map_real(&map, "Vitez", 33.64);
    nic_map_real(&map, "Travnik", 456.22);

    for(int i = 0; i < nec_size(map.keys); i++)
    {
        printf("Key: %s\n", map.keys[i]);
    }

    double* temp = nic_map_find_real(&map, "Vitez");
    if(temp) printf("TEMPERATURE = %f\n", *temp);

    printf("Map height: %d\n", map.nodes[map.root - 1].height);
    nic_clear_real(&map);
    return 0;
}

