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
                      size_t resultSize)
{
    char rhs[ROMAN_SIZE];

    bool success;

    success = copyString(result, resultSize, a);
    success = success && copyString(rhs, sizeof(rhs), b);
    success = success && denormalize(result, resultSize);
    success = success && denormalize(rhs, sizeof(rhs));
    success = success && appendString(result, resultSize, rhs);
    success = success && normalize(result, resultSize);

    if (!success)
    {
        clearString(result);
    }

    return result;
}

const char * romanSubtract(const char * a,
                           const char * b,
                           char * result,
                           size_t resultSize)
{
    char rhs[ROMAN_SIZE];

    bool success;

    success = copyString(result, resultSize, a);
    success = success && copyString(rhs, sizeof(rhs), b);
    success = success && denormalize(result, resultSize);
    success = success && denormalize(rhs, sizeof(rhs));
    success = success && subtractAllSymbols(result, resultSize, rhs);
    success = success && normalize(result, resultSize);

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
                         size_t tableSize);

static int compareRoman(const void * a, const void * b);

#define COUNTOF(array) (sizeof(array) / sizeof(array[0]))

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

    return translateAll(value, valueSize, table, COUNTOF(table));
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

    return translateAll(value, valueSize, table, COUNTOF(table));
}

///////////////////////////////////////////////////////////////////////////////

static bool copyString(char * destination,
                        size_t destinationSize,
                        const char * source)
{
    bool success = false;
    size_t sourceSize = strlen(source) + 1;

    if (sourceSize <= destinationSize)
    {
        memcpy(destination, source, sourceSize);
        success = true;
    }

    return success;
}

static bool appendString(char * destination,
                          size_t destinationSize,
                          const char * source)
{
    bool success = false;
    size_t sourceSize = strlen(source) + 1;
    size_t destinationLength = strlen(destination);

    if ((sourceSize + destinationLength) <= destinationSize)
    {
        memcpy(destination + destinationLength, source, sourceSize);
        success = true;
    }

    return success;
}

static void clearString(char * destination)
{
    destination[0] = '\0';
}

///////////////////////////////////////////////////////////////////////////////

static bool isReplacementTooBig(const char * value,
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
                         size_t tableSize)
{
    bool success = true;

    for(size_t index = 0;
        (index < tableSize) && success;
        ++index)
    {
        const char * from = table[index].from;
        const char * to = table[index].to;

        if (isReplacementTooBig(value, valueSize, from, to))
        {
            success = false;
        }
        else
        {
            replace(value, from, to);
        }
    }

    return success;
}

static int compareRoman(const void * a, const void * b)
{
    return romanIndex(*(const char *)b) - romanIndex(*(const char *)a);
}

///////////////////////////////////////////////////////////////////////////////

static int romanIndex(char c)
{
    static const char order[] = {'I','V','X','L','C','D','M'};

    int index = 0;

    while ((index < COUNTOF(order)) && (c != order[index]))
    {
        ++index;
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
    bool success = true;
    size_t rhsLength = strlen(rhs);

    for (size_t i = 0;
        (i < rhsLength) && success;
        ++i)
    {
        if (!subtractSymbol(value, valueSize, rhs[i]))
        {
            success = false;
        }
    }

    return success;
}

///////////////////////////////////////////////////////////////////////////////

static bool translateFirstMatch(char * value,
                                size_t valueSize,
                                const Translation * table,
                                size_t tableSize);

static bool delete_symbol(char * value, char symbol)
{
    char match[2];

    match[0] = symbol;
    match[1] = '\0';

    return replace(value, match, "");
}

static bool borrow(char * value, size_t valueSize, char symbol)
{
    bool success = false;

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
        { borrow_I, COUNTOF(borrow_I) },
        { borrow_V, COUNTOF(borrow_V) },
        { borrow_X, COUNTOF(borrow_X) },
        { borrow_L, COUNTOF(borrow_L) },
        { borrow_C, COUNTOF(borrow_C) },
        { borrow_D, COUNTOF(borrow_D) },
    };

    size_t index = romanIndex(symbol);

    if (index < COUNTOF(table))
    {
        success = translateFirstMatch(value,
                                      valueSize,
                                      table[index].translation,
                                      table[index].size);
    }

    return success;
}

///////////////////////////////////////////////////////////////////////////////

static bool translateFirstMatch(char * value,
                                size_t valueSize,
                                const Translation * table,
                                size_t tableSize)
{
    bool success = false;

    for(size_t index = 0;
        (index < tableSize) && (!success);
        ++index)
    {
        const char * from = table[index].from;
        const char * to = table[index].to;

        if ((!isReplacementTooBig(value, valueSize, from, to)) &&
            replace(value, from, to))
        {
            success = true;
        }
    }

    return success;
}

///////////////////////////////////////////////////////////////////////////////

static bool isReplacementTooBig(const char * value,
                                size_t valueSize,
                                const char * from,
                                const char * to)
{
    bool success = false;

    if (strstr(value, from) != 0)
    {
        size_t valueLength = strlen(value);
        size_t fromLength = strlen(from);
        size_t toLength = strlen(to);
        size_t sizeIncrease = 0;

        if (toLength > fromLength)
        {
            sizeIncrease = toLength - fromLength;
        }

        if ((sizeIncrease > 0) && ((valueLength + sizeIncrease) >= valueSize))
        {
            success = true;
        }
    }

    return success;
}

static bool replace(char * value,
                    const char * substring,
                    const char * replacement)
{
    bool success = false;
    char * substringLocation = strstr(value, substring);

    if (substringLocation != 0)
    {
        size_t substringOffset = substringLocation - value;
        size_t substringLength = strlen(substring);
        size_t replacementLength = strlen(replacement);
        size_t valueLength = strlen(value);

        if (substringLength != replacementLength)
        {
            memmove(substringLocation + replacementLength,
                    substringLocation + substringLength,
                    (valueLength - substringLength - substringOffset) + 1);
        }

        memcpy(substringLocation, replacement, replacementLength);

        success = true;
    }

    return success;
}
