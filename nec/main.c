#include <stdio.h>
#include "nic.h"

int main()
{
    nic* root = 0;
    root = nic_insert_(&root, 33);
    root = nic_insert_(&root, 32);
    root = nic_insert_(&root, 31);

    printf("Tree:\n");
    nic_debug(root, 0);
    return 0;
}
