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

const char * roman_add(const char * a, const char * b)
{
	strcpy(op1, a);
	strcpy(op2, b);
	replace(op1, "IV", "IIII");
	replace(op2, "IV", "IIII");
	strcpy(buffer, op1);
	strcat(buffer, op2);
	replace(buffer, "IIIII", "V");
	replace(buffer, "IIII", "IV");
    return buffer;
}
