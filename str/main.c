#include <stdio.h>
#include "str.h"

int main()
{
    int val = eval("48 + 5 / 2 * 5 - (435 * 2 * 7) / ( (2 * -2 * 2) - (200 / (2 / 2)) ) * -(300)");
    printf("%d\n", val);
    val = 48 + 5 / 2 * 5 - (435 * 2 * 7) / ((2 * -2 * 2) - (200 / (2 / 2))) * -(300);
    printf("%d\n", val);
    return 0;
}

