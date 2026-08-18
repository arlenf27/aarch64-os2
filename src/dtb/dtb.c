#include <stdint.h>
#include "dtb.h"
#include "utils.h"

uint8_t dtb_correct_magic(const void* raw_dtb){
	dtb_header* dtb = (dtb_header*) raw_dtb;
	return dtb->magic == DTB_MAGIC_LITTLE_ENDIAN;
}

uint8_t find_device_reg(const void* raw_dtb, const char* target_compat, uint64_t* address){
	const dtb_header* header = (const dtb_header*) raw_dtb;
	const uint8_t* struct_start = (const uint8_t*) raw_dtb + be32tole32(header->off_dt_struct);
	const char* strings_start = (const char*) raw_dtb + be32tole32(header->off_dt_strings);
	/* TODO */
	return 0;
}
