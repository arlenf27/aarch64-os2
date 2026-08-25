#include <stdint.h>
#include "dtb.h"
#include "utils.h"

#define DTB_CELLS_STACK_DEPTH 32
#define DTB_DEFAULT_ADDRESS_CELLS 2
#define DTB_DEFAULT_SIZE_CELLS 1
#define MAX_CURRENT_NODE_NAME_SIZE 256

uint8_t dtb_correct_magic(const void* raw_dtb){
	dtb_header* dtb = (dtb_header*) raw_dtb;
	return dtb->magic == DTB_MAGIC_LITTLE_ENDIAN;
}

static uint64_t dtb_read_cells(const uint32_t* data, uint32_t cells)
{
	if(cells == 1){
		return (uint64_t) be32tole32(data[0]);
	}else if(cells == 2){
		return ((uint64_t) be32tole32(data[0]) << 32) | 
			(uint64_t) be32tole32(data[1]);
	}
	return 0;
}

static uint8_t dtb_translate_one_range(const dtb_node_state* cstack, uint32_t bus_level, uint64_t child_address, uint64_t* parent_address){
	const dtb_node_state* bus = &cstack[bus_level];
	/* The parent of the bus determines the number of cells
	 * used for the ranges' parent address */
	const dtb_node_state* bus_parent = &cstack[bus_level - 1];
	uint32_t child_address_cells = bus->address_cells;
	uint32_t size_cells = bus->size_cells;
	uint32_t parent_address_cells = bus_parent->address_cells;
	if(!bus->ranges_present) return 0;
	if(bus->ranges_len == 0){
		*parent_address = child_address;
		return 1;
	}
	/* We currently support 64-bit addresses and sizes */
	if(child_address_cells > 2 || parent_address_cells > 2 || size_cells > 2){
		return 0;
	}
	uint32_t entry_cells = child_address_cells + parent_address_cells + size_cells;
	if(entry_cells == 0) return 0;
	uint32_t entry_bytes = entry_cells * 4;
	if(bus->ranges_len % entry_bytes != 0) return 0;
	const uint32_t* range = (const uint32_t*) bus->ranges_data;
	uint32_t entries = bus->ranges_len / entry_bytes;
	for(uint32_t i = 0; i < entries; i++){
		uint64_t child_base = dtb_read_cells(range, child_address_cells);
		range += child_address_cells;
		uint64_t parent_base = dtb_read_cells(range, parent_address_cells);
		range += parent_address_cells;
		uint64_t size = dtb_read_cells(range, size_cells);
		range += size_cells;
		/* Check whether the address lies inside this range */
		if(child_address >= child_base && child_address - child_base < size){
			*parent_address = parent_base + (child_address - child_base);
			return 1;
		}
	}
	return 0;
}

uint8_t find_device_reg(const void* raw_dtb, const char* target_compat, uint64_t* address){
	const dtb_header* header = (const dtb_header*) raw_dtb;
	const uint8_t* struct_start = (const uint8_t*) raw_dtb + be32tole32(header->off_dt_struct);
	const char* strings_start = (const char*) raw_dtb + be32tole32(header->off_dt_strings);
	const uint8_t* it = struct_start;
	dtb_node_state cstack[DTB_CELLS_STACK_DEPTH];
	uint32_t depth = 0;
	cstack[depth].address_cells = DTB_DEFAULT_ADDRESS_CELLS;
	cstack[depth].size_cells = DTB_DEFAULT_SIZE_CELLS;
	cstack[depth].ranges_data = NULL;
	cstack[depth].ranges_len = 0;
	cstack[depth].ranges_present = 0;
	cstack[depth].compatible_match = 0;
	cstack[depth].reg_data = NULL;
	cstack[depth].reg_len = 0;
	while(1){
		uint32_t token = be32tole32(*(const uint32_t*) it);
		it+=4;
		switch(token){
			case FDT_BEGIN_NODE:
			{
				const char* name = (const char*) it;
				size_t name_len = strlen(name);
				it += name_len + 1;
				/* After null terminator, skip past padding bytes for alignment */
				it = (const uint8_t*) (((uintptr_t) it + 3) & (~3));
				if(depth + 1 < DTB_CELLS_STACK_DEPTH){
					cstack[depth+1] = cstack[depth];
					cstack[depth+1].compatible_match = 0;
					cstack[depth+1].reg_data = NULL;
					cstack[depth+1].reg_len = 0;
					cstack[depth+1].ranges_data = NULL;
					cstack[depth+1].ranges_len = 0;
					cstack[depth+1].ranges_present = 0;
					depth++;
				}else{
					return 0;
				}
				break;
			}
			case FDT_END_NODE:
			{
				if(cstack[depth].compatible_match != 0 && cstack[depth].reg_data != NULL){
					uint32_t address_cells = cstack[depth].address_cells;
					uint32_t size_cells = cstack[depth].size_cells;
					if(address_cells == 0 || address_cells > 2 || size_cells > 2){
						return 0;
					}
					uint32_t tuple_cells = address_cells + size_cells;
					if(tuple_cells == 0 || cstack[depth].reg_len < tuple_cells * 4){
						return 0;
					}
					const uint32_t* reg = (const uint32_t*) cstack[depth].reg_data;
					uint64_t device_address = dtb_read_cells(reg, address_cells);
					for(uint32_t level = depth; level > 2; level--){
						uint64_t translated_address;
						if(!dtb_translate_one_range(cstack, level-1, device_address, &translated_address)){
							return 0;
						}
						device_address = translated_address;
					}
					*address = device_address;
					return 1;
				}
				if(depth > 0){
					depth--;
				}
				break;
			}
			case FDT_PROP:
			{
				uint32_t len = be32tole32(*(const uint32_t*) it);
				it += 4;
				uint32_t name_offset = be32tole32(*(const uint32_t*) it);
				it += 4;
				const void* prop_data = it;
				/* Padding */
				it += (len + 3) & (~3);
				const char* prop_name = strings_start + name_offset;
				/* Update local cell tracking (address, size) if properties define them */
				if(strcmp(prop_name, "#address-cells") && len == 4){
					cstack[depth].address_cells = be32tole32(*(const uint32_t*) prop_data);
				}else if(strcmp(prop_name, "#size-cells") && len == 4){
					cstack[depth].size_cells = be32tole32(*(const uint32_t*) prop_data);
				}else if(strcmp(prop_name, "compatible")){
					const char* compat_str = (const char*) prop_data;
					uint32_t left = len;
					/* List of null-terminated strings */
					while(left > 0){
						size_t str_len = strlen(compat_str);
						if(str_len >= left) break;
						if(strcmp(compat_str, target_compat)){
							cstack[depth].compatible_match = 1;
							break;
						}
						compat_str += str_len + 1;
						left -= str_len + 1;
					}
				}else if(strcmp(prop_name, "reg")){
					cstack[depth].reg_data = prop_data;
					cstack[depth].reg_len = len;
				}else if(strcmp(prop_name, "ranges")){
					cstack[depth].ranges_data = (const uint8_t*) prop_data;
					cstack[depth].ranges_len = len;
					cstack[depth].ranges_present = 1;
				}
				break;
			}
			case FDT_NOP:
			{
				break;
			}
			case FDT_END:
			{
				return 0;
			}
			default:
			{
				return 0;
			}

		}
	}
	return 0;
}
