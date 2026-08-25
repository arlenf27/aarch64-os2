/* linker.ld - simple memory layout for QEMU virt */ 

ENTRY(_start)

/* Define ELF segments */
PHDRS{
	read_execute_seg PT_LOAD FLAGS(5); /* 5 = read (4) + execute (1) */
	read_write_seg PT_LOAD FLAGS(6); /* 6 = read(4) + write(2) */
}

SECTIONS{ 
	
	/* QEMU RAM base: 0x40000000, will reserve for DTB and leave 2 MiB */
	. = 0x40000000;
	__dtb_start = .;
	
	. = 0x40200000;
	
	.text : { *(.text) *(.text.*) } : read_execute_seg

	.rodata : { *(.rodata) *(.rodata.*) } : read_execute_seg

	.data : { *(.data) *(.data.*) } : read_write_seg

	__bss_start = .; 
	
	.bss (NOLOAD) : { *(.bss) *(.bss.*) *(COMMON) } : read_write_seg
	
	__bss_end = .;

	/* Reserve stack space at top of image */  
	
	. = ALIGN(8);  
	
	stack_top = . + 0x20000; /* 128 KiB stack */ 

	. = stack_top; 
}
