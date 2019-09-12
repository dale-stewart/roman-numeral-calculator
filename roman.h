#ifndef roman_h_included
#define roman_h_included

#include <stddef.h>

#define ROMAN_SIZE 32

const char * romanAdd(const char * a,
                      const char * b,
                      char * result,
                      size_t resultSize);

const char * romanSubtract(const char * a,
                           const char * b,
                           char * result,
                           size_t resultSize);

#endif
