#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "roman.h"

///////////////////////////////////////////////////////////////////////////////

static bool denormalize(char * value, size_t valueSize);
static bool normalize(char * value, size_t valueSize);
static bool subtractSymbol(char * value, size_t valueSize, char symbol);

static bool subtractAllSymbols(char * value,
                               size_t valueSize,
                               const char * rhs);

static bool copyString(char * destination,
                       size_t destinationSize,
                       const char * source);

static bool appendString(char * destination,
                         size_t destinationSize,
                         const char * source);

static void clearString(char * destination);

///////////////////////////////////////////////////////////////////////////////

const char * romanAdd(const char * a,
                      const char * b,
                      char * result,
                      size_t result_size)
{
    char rhs[ROMAN_SIZE];

    bool success;

    success = copyString(result, result_size, a);
    success = success && copyString(rhs, sizeof(rhs), b);
    success = success && denormalize(result, result_size);
    success = success && denormalize(rhs, sizeof(rhs));
    success = success && appendString(result, result_size, rhs);
    success = success && normalize(result, result_size);

    if (!success)
    {
        clearString(result);
    }

    return result;
}

const char * romanSubtract(const char * a,
                           const char * b,
                           char * result,
                           size_t result_size)
{
    char rhs[ROMAN_SIZE];

    bool success;

    success = copyString(result, result_size, a);
    success = success && copyString(rhs, sizeof(rhs), b);
    success = success && denormalize(result, result_size);
    success = success && denormalize(rhs, sizeof(rhs));
    success = success && subtractAllSymbols(result, result_size, rhs);
    success = success && normalize(result, result_size);

    if (!success)
    {
        clearString(result);
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////////

typedef struct Translation
{
    const char * from;
    const char * to;
} Translation;

static bool translateAll(char * value,
                         size_t valueSize,
                         const Translation * table,
                         size_t table_size);

static int compareRoman(const void * a, const void * b);

#define countof(array) sizeof(array)/sizeof(array[0])

static bool denormalize(char * value, size_t valueSize)
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

    return translateAll(value, valueSize, table, countof(table));
}

static bool normalize(char * value, size_t valueSize)
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

    qsort(value, strlen(value), sizeof(char), compareRoman);

    return translateAll(value, valueSize, table, countof(table));
}

///////////////////////////////////////////////////////////////////////////////

static bool copyString(char * destination,
                        size_t destinationSize,
                        const char * source)
{
    size_t source_size = strlen(source) + 1;

    if (source_size <= destinationSize)
    {
        memcpy(destination, source, source_size);
        return true;
    }

    return false;
}

static bool appendString(char * destination,
                          size_t destinationSize,
                          const char * source)
{
    size_t source_size = strlen(source) + 1;
    size_t destination_length = strlen(destination);

    if (source_size + destination_length <= destinationSize)
    {
        memcpy(destination + destination_length, source, source_size);
        return true;
    }

    return false;
}

static void clearString(char * destination)
{
    destination[0] = '\0';
}

///////////////////////////////////////////////////////////////////////////////

static bool isReplacementTooBig(char * value,
                                size_t valueSize,
                                const char * from,
                                const char * to);

static bool replace(char * value,
                    const char * substring,
                    const char * replacement);

static int romanIndex(char c);

static bool translateAll(char * value,
                         size_t valueSize,
                         const Translation * table,
                         size_t table_size)
{
    for(size_t index = 0; index < table_size; ++index)
    {
        const char * from = table[index].from;
        const char * to = table[index].to;

        if (isReplacementTooBig(value, valueSize, from, to))
        {
            return false;
        }

        replace(value, from, to);
    }

    return true;
}

static int compareRoman(const void * a, const void * b)
{
    return romanIndex(*(const char *)b) - romanIndex(*(const char *)a);
}

///////////////////////////////////////////////////////////////////////////////

static int romanIndex(char c)
{
    static const char order[] = {'I','V','X','L','C','D','M'};

    int index;

    for (index = 0; index < countof(order); ++index)
    {
        if (c == order[index])
        {
            break;
        }
    }

    return index;
}

///////////////////////////////////////////////////////////////////////////////

static bool delete_symbol(char * value, char symbol);
static bool borrow(char * value, size_t valueSize, char symbol);

static bool subtractSymbol(char * value, size_t valueSize, char symbol)
{
    bool success = delete_symbol(value, symbol);

    if (!success)
    {
        success = borrow(value, valueSize, symbol);
    }

    return success;
}

static bool subtractAllSymbols(char * value,
                                 size_t valueSize,
                                 const char * rhs)
{
    size_t rhs_length = strlen(rhs);

    for (size_t i = 0; i < rhs_length; ++i)
    {
        if (!subtractSymbol(value, valueSize, rhs[i]))
        {
            return false;
        }
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////

static bool translateFirstMatch(char * value,
                                size_t valueSize,
                                const Translation * table,
                                size_t table_size);

static bool delete_symbol(char * value, char symbol)
{
    char match[2];

    match[0] = symbol;
    match[1] = '\0';

    return replace(value, match, "");
}

static bool borrow(char * value, size_t valueSize, char symbol)
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

    size_t index = romanIndex(symbol);

    if (index < countof(table))
    {
        return translateFirstMatch(value,
                                   valueSize,
                                   table[index].translation,
                                   table[index].size);
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////////

static bool translateFirstMatch(char * value,
                                size_t valueSize,
                                const Translation * table,
                                size_t table_size)
{
    for(size_t index = 0; index < table_size; ++index)
    {
        const char * from = table[index].from;
        const char * to = table[index].to;

        if (isReplacementTooBig(value, valueSize, from, to))
        {
            return false;
        }

        if (replace(value, from, to))
        {
            return true;
        }
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////////

static bool isReplacementTooBig(char * value,
                                size_t valueSize,
                                const char * from,
                                const char * to)
{
    if (strstr(value, from))
    {
        size_t valueLength = strlen(value);
        size_t fromLength = strlen(from);
        size_t toLength = strlen(to);

        size_t sizeIncrease = toLength > fromLength ?
                              toLength - fromLength :
                              0;

        if (sizeIncrease > 0 && (valueLength + sizeIncrease) >= valueSize)
        {
            return true;
        }
    }

    return false;
}

static bool replace(char * value,
                    const char * substring,
                    const char * replacement)
{
    char * substringLocation = strstr(value, substring);
    if (substringLocation)
    {
        size_t substringOffset = substringLocation - value;
        size_t substringLength = strlen(substring);
        size_t replacementLength = strlen(replacement);
        size_t valueLength = strlen(value);

        if (substringLength != replacementLength)
        {
            memmove(substringLocation + replacementLength,
                    substringLocation + substringLength,
                    valueLength - substringLength - substringOffset + 1);
        }

        memcpy(substringLocation, replacement, replacementLength);

        return true;
    }

    return false;
}
