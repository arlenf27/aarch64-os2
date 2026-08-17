/* linker.ld - simple memory layout for QEMU virt */ 

ENTRY(_start)

SECTIONS { 
	
	/* QEMU RAM base: 0x40000000, will reserve for DTB and leave 2 MiB */
	. = 0x40000000;
	__dtb_start = .;
	
	. = 0x40200000;
	
	.text : { *(.text) *(.text.*) }

	.rodata : { *(.rodata) *(.rodata.*) }

	.data : { *(.data) *(.data.*) }

	__bss_start = .; 
	
	.bss (NOLOAD) : { *(.bss) *(.bss.*) *(COMMON) } 
	
	__bss_end = .;

	/* Reserve stack space at top of image */  
	
	. = ALIGN(8);  
	
	stack_top = . + 0x20000; /* 128 KiB stack */ 

	. = stack_top; 
}
