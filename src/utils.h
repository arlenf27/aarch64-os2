#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stddef.h>

uint32_t be32tole32(uint32_t n);

size_t strlen(const char* s);

/* Returns 1 when strings are equal, 0 otherwise */
uint8_t strcmp(const char* a, const char* b);

#endif
