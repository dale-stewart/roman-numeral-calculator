#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "roman.h"

///////////////////////////////////////////////////////////////////////////////

static void denormalize(char * value);
static void normalize(char * value);
static bool subtract_symbol(char * value, char symbol);

///////////////////////////////////////////////////////////////////////////////

const char * roman_add(const char * a, const char * b, char * result)
{
    char rhs[ROMAN_SIZE];

    strcpy(result, a);
    denormalize(result);

    strcpy(rhs, b);
    denormalize(rhs);
    strcat(result, rhs);

    normalize(result);

    return result;
}

const char * roman_subtract(const char * a, const char * b, char * result)
{
    char rhs[ROMAN_SIZE];

    strcpy(result, a);
    denormalize(result);

    strcpy(rhs, b);
    denormalize(rhs);

    for (int i = 0; i < strlen(rhs); ++i)
    {
        if (!subtract_symbol(result, rhs[i]))
        {
            strcpy(result, "");
            return result;
        }
    }

    normalize(result);

    return result;
}

///////////////////////////////////////////////////////////////////////////////

typedef struct Translation
{
    const char * from;
    const char * to;
} Translation;

static void translate_all(char * value, const Translation * table, int table_size);
static int compare_roman(const void * a, const void * b);

#define countof(array) sizeof(array)/sizeof(array[0])

static void denormalize(char * value)
{
    static const Translation table[] =
    {
        { "IV", "IIII"  },
        { "IX", "VIIII" },
        { "XL", "XXXX"  },
        { "XC", "LXXXX" },
        { "CD", "CCCC"  },
        { "CM", "DCCCC" },
    };

    translate_all(value, table, countof(table));
}

static void normalize(char * value)
{
    static const Translation table[] =
    {
        { "IIIII",  "V"  },
        { "IIII",   "IV" },
        { "VIV",    "IX" },
        { "VV",     "X"  },
        { "XXXXX",  "L"  },
        { "XXXX",   "XL" },
        { "LXL",    "XC" },
        { "LL",     "C"  },
        { "CCCCC",  "D"  },
        { "CCCC",   "CD" },
        { "DCD",    "CM" },
        { "DD",     "M"  },
    };

    qsort(value, strlen(value), sizeof(char), compare_roman);

    translate_all(value, table, countof(table));
}

///////////////////////////////////////////////////////////////////////////////

static bool replace(char * value, const char * substring, const char * replacement);
static int roman_index(char c);

static void translate_all(char * value, const Translation * table, int table_size)
{
    for(int index = 0; index < table_size; ++index)
    {
        replace(value, table[index].from, table[index].to);
    }
}

static int compare_roman(const void * a, const void * b)
{
    return roman_index(*(const char *)b) - roman_index(*(const char *)a);
}

///////////////////////////////////////////////////////////////////////////////

static int roman_index(char c)
{
    static const char order[] = {'I','V','X','L','C','D','M'};

    int index;

    for (index = 0; index < countof(order); ++index)
        if (c == order[index])
            break;

    return index;
}

///////////////////////////////////////////////////////////////////////////////

static bool delete_symbol(char * value, char symbol);
static bool borrow(char * value, char symbol);

static bool subtract_symbol(char * value, char symbol)
{
    bool success = delete_symbol(value, symbol);

    if (!success)
        success = borrow(value, symbol);

    return success;
}

///////////////////////////////////////////////////////////////////////////////

static bool translate_first_match(char * value, const Translation * table, int table_size);

static bool delete_symbol(char * value, char symbol)
{
    char match[2];

    match[0] = symbol;
    match[1] = '\0';

    return replace(value, match, "");
}

static bool borrow(char * value, char symbol)
{
    static const Translation borrow_I[] =
    {
        { "V", "IIII"            },
        { "X", "VIIII"           },
        { "L", "XXXXVIIII"       },
        { "C", "LXXXXVIIII"      },
        { "D", "CCCCLXXXXVIIII"  },
        { "M", "DCCCCLXXXXVIIII" },
    };

    static const Translation borrow_V[] =
    {
        { "X", "V"           },
        { "L", "XXXXV"       },
        { "C", "LXXXXV"      },
        { "D", "CCCCLXXXXV"  },
        { "M", "DCCCCLXXXXV" },
    };

    static const Translation borrow_X[] =
    {
        { "L", "XXXX"       },
        { "C", "LXXXX"      },
        { "D", "CCCCLXXXX"  },
        { "M", "DCCCCLXXXX" },
    };

    static const Translation borrow_L[] =
    {
        { "C", "L"      },
        { "D", "CCCCL"  },
        { "M", "DCCCCL" },
    };

    static const Translation borrow_C[] =
    {
        { "D", "CCCC"  },
        { "M", "DCCCC" },
    };

    static const Translation borrow_D[] =
    {
        { "M", "D" },
    };

    static const struct
    {
        const Translation * translation;
        int size;
    }
    table[] =
    {
        { borrow_I, countof(borrow_I) },
        { borrow_V, countof(borrow_V) },
        { borrow_X, countof(borrow_X) },
        { borrow_L, countof(borrow_L) },
        { borrow_C, countof(borrow_C) },
        { borrow_D, countof(borrow_D) },
    };

    int index = roman_index(symbol);

    if (index < countof(table))
    {
        return translate_first_match(value, table[index].translation, table[index].size);
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////////

static bool translate_first_match(char * value, const Translation * table, int table_size)
{
    for(int index = 0; index < table_size; ++index)
    {
        if (replace(value, table[index].from, table[index].to))
            return true;
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////////

static bool replace(char * value, const char * substring, const char * replacement)
{
    char * substring_location = strstr(value, substring);
    if (substring_location)
    {
        int substring_offset = substring_location - value;
        int substring_length = strlen(substring);
        int replacement_length = strlen(replacement);
        int value_length = strlen(value);

        if (substring_length != replacement_length)
        {
            memmove(substring_location + replacement_length,
                    substring_location + substring_length,
                    value_length - substring_length - substring_offset + 1);
        }

        memcpy(substring_location, replacement, replacement_length);

        return true;
    }

    return false;
}
