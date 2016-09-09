#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "roman.h"

static char buffer[32];

static void denormalize(char * value);
static void normalize(char * value);
static int compare_roman(const void * a, const void * b);
static int roman_index(char c);
static void subtract_one_symbol(char * target, char symbol);
static bool replace(char * target, const char * substring, const char * replacement);

const char * roman_add(const char * a, const char * b)
{
    char rhs[32];

    strcpy(buffer, a);
    denormalize(buffer);

    strcpy(rhs, b);
    denormalize(rhs);
    strcat(buffer, rhs);

    normalize(buffer);

    return buffer;
}

const char * roman_subtract(const char * a, const char * b)
{
    char rhs[32];

    strcpy(buffer, a);
    denormalize(buffer);

    strcpy(rhs, b);
    denormalize(rhs);

    for (int i = 0; i < strlen(rhs); ++i)
        subtract_one_symbol(buffer, rhs[i]);

    normalize(buffer);

    return buffer;
}

static void denormalize(char * value)
{
    replace(value, "IV", "IIII");
    replace(value, "IX", "VIIII");
    replace(value, "XL", "XXXX");
    replace(value, "XC", "LXXXX");
    replace(value, "CD", "CCCC");
    replace(value, "CM", "DCCCC");
}

static void normalize(char * value)
{
    qsort(value, strlen(value), sizeof(char), compare_roman);

    replace(value, "IIIII", "V");
    replace(value, "IIII", "IV");
    replace(value, "VIV", "IX");
    replace(value, "VV", "X");
    replace(value, "XXXXX", "L");
    replace(value, "XXXX", "XL");
    replace(value, "LXL", "XC");
    replace(value, "LL", "C");
    replace(value, "CCCCC", "D");
    replace(value, "CCCC", "CD");
    replace(value, "DCD", "CM");
    replace(value, "DD", "M");
}

static int compare_roman(const void * a, const void * b)
{
    return roman_index(*(const char *)b) - roman_index(*(const char *)a);
}

static int roman_index(char c)
{
    static const char order[] = {'I','V','X','L','C','D','M'};
    static const int order_size = sizeof(order)/sizeof(order[0]);

    int index;

    for (index = 0; index < order_size; ++index)
        if (c == order[index])
            break;

    return index;
}

static void subtract_one_symbol(char * target, char symbol)
{
    char match[2];

    match[0] = symbol;
    match[1] = '\0';

    if (!replace(target, match, ""))
    {
        if ('I' == symbol)
        {
            replace(target, "V", "IIII") ||
            replace(target, "X", "VIIII") ||
            replace(target, "L", "XXXXVIIII") ||
            replace(target, "C", "LXXXXVIIII") ||
            replace(target, "D", "CCCCLXXXXVIIII") ||
            replace(target, "M", "DCCCCLXXXXVIIII");
        }
        else if ('V' == symbol)
        {
            replace(target, "X", "V") ||
            replace(target, "L", "XXXXV") ||
            replace(target, "C", "LXXXXV") ||
            replace(target, "D", "CCCCLXXXXV") ||
            replace(target, "M", "DCCCCLXXXXV");
        }
        else if ('X' == symbol)
        {
            replace(target, "L", "XXXX") ||
            replace(target, "C", "LXXXX") ||
            replace(target, "D", "CCCCLXXXX") ||
            replace(target, "M", "DCCCCLXXXX");
        }
        else if ('L' == symbol)
        {
            replace(target, "C", "L") ||
            replace(target, "D", "CCCCL") ||
            replace(target, "M", "DCCCCL");
        }
        else if ('C' == symbol)
        {
            replace(target, "D", "CCCC") ||
            replace(target, "M", "DCCCC");
        }
        else if ('D' == symbol)
        {
            replace(target, "M", "D");
        }
    }
}

static bool replace(char * target, const char * substring, const char * replacement)
{
    char * substring_location = strstr(target, substring);
    if (substring_location)
    {
        int substring_offset = substring_location - target;
        int substring_length = strlen(substring);
        int replacement_length = strlen(replacement);
        int target_length = strlen(target);

        memmove(substring_location + replacement_length,
                substring_location + substring_length,
                target_length - substring_length - substring_offset + 1);

        memcpy(substring_location, replacement, replacement_length);

        return true;
    }

    return false;
}
