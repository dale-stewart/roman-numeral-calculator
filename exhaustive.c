#include "roman.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

int main(void)
{
    char result[ROMAN_SIZE];

    (void)strncpy(result, "I", sizeof(result) - 1);
    result[sizeof(result) - 1] = '\0';

    int16_t count = 3999;

    while (count != 0)
    {
        (void)printf("%s\n", result);
        (void)romanAdd(result, "I", result, sizeof(result));
        --count;
    }

    return 0;
}
