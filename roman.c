#include <string.h>
#include "roman.h"

static char buffer[32];

static void replace_in_buffer(const char * substring, const char * replacement)
{
	char * location = strstr(buffer, substring);
	if (location)
	{
		strcpy(location, replacement);
		strcat(location, location + strlen(substring));
	}
}

const char * roman_add(const char * a, const char * b)
{
	strcpy(buffer, a);
	strcat(buffer, b);
	replace_in_buffer("IIIII", "V");
	replace_in_buffer("IIII", "IV");
    return buffer;
}
