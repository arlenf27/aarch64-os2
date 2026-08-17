#include <utils.h>

static uint32_t big_endian_32(uint32_t n){
	return ((n & 0xFF) << 24) | ((n & 0xFF00) << 8) | ((n & 0xFF0000) >> 8) | ((n & 0xFF000000) >> 24);
}
