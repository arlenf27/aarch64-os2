/* exception_handlers.S - Aarch64 Exception Handlers */

/* TODO - NO handlers are functional or currently used! */

/* Everything below is placed in .text */
.section .text

.global synchronous_handler_sp0
.type synchronous_handler_sp0, @function
synchronous_handler_sp0:
	/* TODO */
	eret

.global irq_handler_sp0
.type irq_handler_sp0, @function
irq_handler_sp0:
	/* TODO */
	eret

.global fiq_handler_sp0
.type fiq_handler_sp0, @function
fiq_handler_sp0:
	/* TODO */
	eret

.global serror_handler_sp0
.type serror_handler_sp0, @function
serror_handler_sp0:
	/* TODO */
	eret

.global synchronous_handler_spx
.type synchronous_handler_spx, @function
synchronous_handler_spx:
	/* TODO */
	eret

.global irq_handler_spx
.type irq_handler_spx, @function
irq_handler_spx:
	/* TODO */
	eret

.global fiq_handler_spx
.type fiq_handler_spx, @function
fiq_handler_spx:
	/* TODO */
	eret

.global serror_handler_spx
.type serror_handler_spx, @function
serror_handler_spx:
	/* TODO */
	eret

.global synchronous_handler_64
.type synchronous_handler_64, @function
synchronous_handler_64:
	/* TODO */
	eret

.global irq_handler_64
.type irq_handler_64, @function
irq_handler_64:
	/* TODO */
	eret

.global fiq_handler_64
.type fiq_handler_64, @function
fiq_handler_64:
	/* TODO */
	eret

.global serror_handler_64
.type serror_handler_64, @function
serror_handler_64:
	/* TODO */
	eret

.global synchronous_handler_32
.type synchronous_handler_32, @function
synchronous_handler_32:
	/* TODO */
	eret

.global irq_handler_32
.type irq_handler_32, @function
irq_handler_32:
	/* TODO */
	eret

.global fiq_handler_32
.type fiq_handler_32, @function
fiq_handler_32:
	/* TODO */
	eret

.global serror_handler_32
.type serror_handler_32, @function
serror_handler_32:
	/* TODO */
	eret

