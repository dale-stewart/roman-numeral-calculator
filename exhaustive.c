#include "roman.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
	char result[ROMAN_SIZE];

	strcpy(result, "I");

	int count = 3999;

	while (count--)
	{
		printf("%s\n", result);
		roman_add(result, "I", result, sizeof(result));
	}

	return 0;
}
