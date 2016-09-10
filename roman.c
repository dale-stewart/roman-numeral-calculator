#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "roman.h"

///////////////////////////////////////////////////////////////////////////////

static bool denormalize(char * value, size_t value_size);
static bool normalize(char * value, size_t value_size);
static bool subtract_symbol(char * value, char symbol);
static bool subtract_all_symbols(char * value, size_t value_size, const char * rhs);
static bool copy_string(char * destination, size_t destination_size, const char * source);
static bool append_string(char * destination, size_t destination_size, const char * source);
static void clear_string(char * destination);

///////////////////////////////////////////////////////////////////////////////

const char * roman_add(const char * a, const char * b, char * result, size_t result_size)
{
    char rhs[ROMAN_SIZE];

    bool success;

    success = copy_string(result, result_size, a);
    success = success && copy_string(rhs, sizeof(rhs), b);
    success = success && denormalize(result, result_size);
    success = success && denormalize(rhs, sizeof(rhs));
    success = success && append_string(result, result_size, rhs);
    success = success && normalize(result, result_size);

    if (!success)
        clear_string(result);

    return result;
}

const char * roman_subtract(const char * a, const char * b, char * result, size_t result_size)
{
    char rhs[ROMAN_SIZE];

    bool success;

    success = copy_string(result, result_size, a);
    success = success && copy_string(rhs, sizeof(rhs), b);
    success = success && denormalize(result, result_size);
    success = success && denormalize(rhs, sizeof(rhs));
    success = success && subtract_all_symbols(result, result_size, rhs);
    success = success && normalize(result, result_size);

    if (!success)
        clear_string(result);

    return result;
}

///////////////////////////////////////////////////////////////////////////////

typedef struct Translation
{
    const char * from;
    const char * to;
} Translation;

static bool translate_all(char * value, size_t value_size, const Translation * table, size_t table_size);
static int compare_roman(const void * a, const void * b);

#define countof(array) sizeof(array)/sizeof(array[0])

static bool denormalize(char * value, size_t value_size)
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

    return translate_all(value, value_size, table, countof(table));
}

static bool normalize(char * value, size_t value_size)
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

    return translate_all(value, value_size, table, countof(table));
}

///////////////////////////////////////////////////////////////////////////////

static bool copy_string(char * destination, size_t destination_size, const char * source)
{
    size_t source_size = strlen(source) + 1;

    if (source_size <= destination_size)
    {
        memcpy(destination, source, source_size);
        return true;
    }

    return false;
}

static bool append_string(char * destination, size_t destination_size, const char * source)
{
    size_t source_size = strlen(source) + 1;
    size_t destination_length = strlen(destination);

    if (source_size + destination_length <= destination_size)
    {
        memcpy(destination + destination_length, source, source_size);
        return true;
    }

    return false;
}

static void clear_string(char * destination)
{
    destination[0] = '\0';
}

///////////////////////////////////////////////////////////////////////////////

static bool replace(char * value, const char * substring, const char * replacement);
static int roman_index(char c);

static bool translate_all(char * value, size_t value_size, const Translation * table, size_t table_size)
{
    for(size_t index = 0; index < table_size; ++index)
    {
        size_t value_length = strlen(value);
        size_t from_length = strlen(table[index].from);
        size_t to_length = strlen(table[index].to);

        size_t size_increase = to_length > from_length ? to_length - from_length : 0;

        if (size_increase > 0 && (value_length + size_increase) >= value_size)
            return false;

        replace(value, table[index].from, table[index].to);
    }

    return true;
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

static bool subtract_all_symbols(char * value, size_t value_size, const char * rhs)
{
    size_t rhs_length = strlen(rhs);

    for (size_t i = 0; i < rhs_length; ++i)
    {
        if (!subtract_symbol(value, rhs[i]))
            return false;
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////

static bool translate_first_match(char * value, const Translation * table, size_t table_size);

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
        size_t size;
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

    size_t index = roman_index(symbol);

    if (index < countof(table))
    {
        return translate_first_match(value, table[index].translation, table[index].size);
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////////

static bool translate_first_match(char * value, const Translation * table, size_t table_size)
{
    for(size_t index = 0; index < table_size; ++index)
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
        size_t substring_offset = substring_location - value;
        size_t substring_length = strlen(substring);
        size_t replacement_length = strlen(replacement);
        size_t value_length = strlen(value);

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
