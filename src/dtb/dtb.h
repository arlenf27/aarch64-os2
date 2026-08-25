#ifndef DTB_H
#define DTB_H

#include <stdint.h>

#define DTB_MAGIC_LITTLE_ENDIAN 0xEDFE0DD0
#define FDT_BEGIN_NODE 0x00000001
#define FDT_END_NODE 0x00000002
#define FDT_PROP 0x00000003
#define FDT_NOP 0x00000004
#define FDT_END 0x00000009

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

typedef struct dtb_node_state{
	uint32_t address_cells;
	uint32_t size_cells;
	const uint8_t* ranges_data;
	uint32_t ranges_len;
	uint8_t ranges_present;
	uint8_t compatible_match;
	const void* reg_data;
	uint32_t reg_len;
} dtb_node_state;

uint8_t dtb_correct_magic(const void* raw_dtb);

uint8_t find_device_reg(const void* raw_dtb, const char* target_compat, uint64_t* address);

#endif
