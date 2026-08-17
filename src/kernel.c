/* kernel.c for Aarch64 */ 

#include <stdint.h> 
#include <stddef.h>
#include "dtb.h"

/* TODO: DELETE THESE UART DEFINES AFTER IMPLEMENTING DTB PARSER - QEMU virt PL011 UART base. See the VM's dtb file for more information on where this comes from */ 

#define UART0_BASE 0x09000000UL 
#define UART0_DR (*(volatile uint32_t*) (UART0_BASE + 0x00)) 
#define UART0_FR (*(volatile uint32_t*) (UART0_BASE + 0x18)) 
#define UART_FR_TXFF (1u << 5)

static void uart_putc(char c){ 
	/* wait while TX FIFO full */ 
	while (UART0_FR & UART_FR_TXFF){
		asm volatile("nop"); 
	} 
	UART0_DR = (uint32_t) c; 
}

static void uart_puts(const char* s){ 
	for (size_t i = 0; s[i] != '\0'; i++){
		uart_putc(s[i]);
	}	
}

/** Special function to read exception level */
static unsigned int read_el(void){
	unsigned long el; /* CurrentEL is a 64-bit register */
	/* Reads Aarch64 register CurrentEL and then places it into the C variable el */
	asm volatile("mrs %0, CurrentEL" : "=r"(el));
	/* CurrentEL register in Aarch64 is encoded as EL<<2 so shift right */
	return (unsigned int) ((el >> 2) & 0x3);
}

/* Exported symbol called by boot.S */ 
void kernel_main(void* dtb){
	// TODO: remove temporary debug code
	dtb_header* dtbh = (dtb_header*) dtb;
	uint32_t magic = dtbh->magic;
	if(magic == DTB_MAGIC_LITTLE_ENDIAN) uart_puts("Correct DTB Magic Number. \n");

	uart_puts("Hello from AArch64 bare-metal kernel!\n");  
	unsigned int el = read_el();
	uart_puts("Current exception level is: ");
	uart_putc('0' + (el & 0xF));
	uart_puts("\n");
	/* simple idle: wait for interrupts (low power) */ 
	while(1){
		asm volatile("wfe");
	}

}
