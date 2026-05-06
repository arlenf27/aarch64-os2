/* boot.S - Aarch64 entry */ 

.section .text 
.global _start 
.type _start, %function

_start: 

	/* load address of stack_top (from linker script) into x0 and set SP */ 
	ldr x0, =stack_top 
	mov sp, x0

	/* zero .bss: fill 8 bytes at a time from __bss_start to __bss_end */
	ldr    x1, =__bss_start      /* x1 = address of bss start */
	ldr    x2, =__bss_end        /* x2 = address of bss end */
1:	cmp x1, x2 /* while (x1 < x2) */  
	b.ge 2f  
	/* store 0 (xzr) at [x1]; x1 += 8 */
	str xzr, [x1], #8   
	b 1b
2:	bl kernel_main /* call C kernel entry */

 	
/* if kernel_main returns, loop forever */
_hang: 
	wfe 
	b _hang
