#ifndef __roman_h__
#define __roman_h__

#include <stddef.h>

#define ROMAN_SIZE 32

const char * roman_add(const char * a, const char * b, char * result, size_t result_size);
const char * roman_subtract(const char * a, const char * b, char * result, size_t result_size);

#endif
