#include <string.h>
#include "roman.h"

static char buffer[32];
static char op1[32];
static char op2[32];
static char buf2[32];

static void replace(char * target, const char * substring, const char * replacement)
{
	char * location = strstr(target, substring);
	if (location)
	{
		buf2[0] = '\0';
		if (location != target)
		{
			memcpy(buf2, target, location - target);
			buf2[location - target] = 0;
		}
		strcat(buf2, replacement);
		strcat(buf2, location + strlen(substring));
		strcpy(target, buf2);
	}
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

static int compare_roman(const void * a, const void * b)
{
	return roman_index(*(const char *)b) - roman_index(*(const char *)a);
}

void denormalize(char * value)
{
	replace(value, "IV", "IIII");
	replace(value, "IX", "VIIII");
	replace(value, "XL", "XXXX");
	replace(value, "XC", "LXXXX");
	replace(value, "CD", "CCCC");
	replace(value, "CM", "DCCCC");
}

void normalize(char * value)
{
	replace(value, "IIIII", "V");
	replace(value, "IIII", "IV");
	replace(value, "VV", "X");
	replace(value, "XXXXX", "L");
	replace(value, "XXXX", "XL");
	replace(value, "LL", "C");
	replace(value, "CCCCC", "D");
	replace(value, "CCCC", "CD");
	replace(value, "DD", "M");
}

const char * roman_add(const char * a, const char * b)
{
	strcpy(op1, a);
	strcpy(op2, b);
	denormalize(op1);
	denormalize(op2);
	strcpy(buffer, op1);
	strcat(buffer, op2);
	qsort(buffer, strlen(buffer), sizeof(char), compare_roman);
	normalize(buffer);
    return buffer;
}

const char * roman_subtract(const char * a, const char * b)
{
	return "";
}
