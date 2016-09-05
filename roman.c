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

static int compare_roman(const void * a, const void * b)
{
	const char * ca = (const char *)a;
	const char * cb = (const char *)b;

	const char * order = "IVXLCDM";

	int index_a;
	int index_b;

	for (index_a = 0; index_a < strlen(order); ++index_a)
		if (order[index_a] == *ca)
			break;

	for (index_b = 0; index_b < strlen(order); ++index_b)
		if (order[index_b] == *cb)
			break;

	return index_b - index_a;
}

const char * roman_add(const char * a, const char * b)
{
	strcpy(op1, a);
	strcpy(op2, b);
	replace(op1, "IV", "IIII");
	replace(op2, "IV", "IIII");
	strcpy(buffer, op1);
	strcat(buffer, op2);
	qsort(buffer, strlen(buffer), sizeof(char), compare_roman);
	replace(buffer, "IIIII", "V");
	replace(buffer, "IIII", "IV");
    return buffer;
}
