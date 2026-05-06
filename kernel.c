/* kernel.c - freestanding minimal kernel */ 

#include <stdint.h> 
#include <stddef.h>

/* QEMU virt PL011 UART base. If nothing prints, try 0x09000000 or 0x09000000 (common). */ 

#define UART0_BASE 0x09000000UL 
#define UART0_DR (*(volatile uint32_t*) (UART0_BASE + 0x00)) 
#define UART0_FR (*(volatile uint32_t*) (UART0_BASE + 0x18)) 
#define UART_FR_TXFF (1u << 5)

static void uart_putc(char c) { 
	/* wait while TX FIFO full */ 
	while (UART0_FR & UART_FR_TXFF){
		asm volatile("nop"); 
	} 
	UART0_DR = (uint32_t) c; 
}

static void uart_puts(const char *s){ 
	for (size_t i = 0; s[i]; ++i){
		uart_putc(s[i]);
	}	
}

/* Exported symbol called by boot.S */ 
void kernel_main(void){  
	uart_puts("Hello from AArch64 bare-metal kernel!\n");  
	/* simple idle: wait for interrupts (low power) */ 
	for (;;){
		asm volatile("wfe");
	}

}
