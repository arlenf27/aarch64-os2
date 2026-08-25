#include <stddef.h>
#include "utils.h"


uint32_t be32tole32(uint32_t n){
	return ((n & 0xFF) << 24) | ((n & 0xFF00) << 8) | ((n & 0xFF0000) >> 8) | ((n & 0xFF000000) >> 24);
}

size_t strlen(const char* s){
	if(s == NULL) return 0;
	size_t size = 0;
	while(*s != '\0'){
		size++;
		s++;
	}
	return size;
}

/* Returns 1 when strings are equal, 0 otherwise */
uint8_t strcmp(const char* a, const char* b){
	if(a == NULL || b == NULL) return 0;
	while(*a != '\0' && *b != '\0'){
		if(*a != *b) return 0;
		a++;
		b++;
	}
	return *a == '\0' && *b == '\0';
}
