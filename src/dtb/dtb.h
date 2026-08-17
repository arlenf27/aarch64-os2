#include <stdint.h>

#define DTB_MAGIC_LITTLE_ENDIAN 0xEDFE0DD0

typedef struct dtb_header{
	uint32_t magic;
	uint32_t total_size;
	uint32_t off_dt_struct;
	uint32_t off_dt_strings;
	uint32_t off_mem_rsvmap;
	uint32_t version;
	uint32_t last_comp_version;
	uint32_t boot_cpuid_phys;
	uint32_t size_dt_strings;
	uint32_t size_dt_struct;
} dtb_header;
