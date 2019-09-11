#include "roman.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

int main(void)
{
    char result[ROMAN_SIZE];

    strncpy(result, "I", sizeof(result) - 1);
    result[sizeof(result) - 1] = '\0';

    int16_t count = 3999;

    while (count != 0)
    {
        printf("%s\n", result);
        roman_add(result, "I", result, sizeof(result));
        --count;
    }

    return 0;
}
