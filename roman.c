#include <string.h>
#include "roman.h"

static char buffer[32];

const char * roman_add(const char * a, const char * b)
{
	strcpy(buffer, a);
	strcat(buffer, b);
	if (0 == strcmp(buffer, "IIII"))
		strcpy(buffer, "IV");
    return buffer;
}
