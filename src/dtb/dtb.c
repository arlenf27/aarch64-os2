#include <stdint.h>
#include "dtb.h"
#include "utils.h"

uint8_t dtb_correct_magic(void* raw_dtb){
	dtb_header* dtb = (dtb_header*) raw_dtb;
	return dtb->magic == DTB_MAGIC_LITTLE_ENDIAN;
}
