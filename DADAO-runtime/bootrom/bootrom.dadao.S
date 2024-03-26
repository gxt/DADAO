#define BOOTADDR_REG 0x4000

#include "encoding.h"

// boot all cores (only hart 0) and jump to main program execution
.section .text.start, "ax", @progbits
.globl _start
_start:
	setzw	rb9, w1, 0x200		// base address of clint
	CPRD_mhartid(rd8)
	brnz	rd8, boot_core
	setzw	rb10, w1, 0x200
	addi	rb10, rb10, 4
	setzw	rd11, w0, 1
interrupt_loop:				// trigger MSIP interrupts for other harts so that they also boot
	stt	rd11, rb10, 0
	addi	rb10, rb10, 4
	ldtu	rd11, rb10, -4
	brnz	rd11, interrupt_loop
	jump	boot_core_hart0

// this boot ROM doesn't know about any boot devices, so it just spins,
// waiting for the serial interface to load the program and interrupt it
.section .text.hang, "ax", @progbits
.globl _hang
_hang:					// reset vector
	setrd	rd8, _start		// on MSIP interrupt, go to _start
	CPWR_mtvec(rd8)
	CPRD_misa(rd32)
	shrs	rd32, rd32, 'S' - 'A'	// get 'S' supervisor bit
	andi	rd32, rd32, 1
	brz	rd32, 1f		// only zero mideleg if supported
	CPWR_mideleg(rd0)
1:
	setzw	rd8, w0, 8		// MIE or MSIP bit
	CPWR_mie(rd8)			// mie: set only MSIP in MIE CSR
	CPRD_mstatus(rd9)
	orw	rd9, w0, 8
	CPWR_mstatus(rd9)		// set MIE in mstatus CSR

wfi_loop: // wait for MSIP interrupt to start program execution
	cpco	cp0, 0
	jump	wfi_loop

// jump to reset vector
.section .text.hang90, "ax", @progbits
.globl _hang90
_hang90:
	jump	_hang

// other harts must wait until MSIP of hart 0 is cleared to continue
boot_core:
	ldtu	rd32, rb9, 0		// load hart 0 MSIP
	brnz	rd32, boot_core		// block until hart 0 clears own MSIP
	shlu	rd8, rd8, 2		// offset for hart MSIP
	add	rd0, rd9, rd8, rd9
boot_core_hart0:			// begin executing code at DRAM_BASE
	stt	rd0, rb9, 0		// clear the interrupt
	setrb	rb8, BOOTADDR_REG	// program reset vector
	ldo	rd8, rb8, 0		// get boot address from bootaddr_reg SCR
	CPWR_mepc(rd8)			// mepc: return from interrupt to start of user program
	CPRD_mhartid(rd16)		// hartid for next level bootloader
	setrd	rd17, _dtb		// dtb address for next level bootloader
	CPRD_mstatus(rd18)
	andnw	rd18, w0, 0x80		// set mstatus MPIE to 0
	CPWR_mstatus(rd18)
	escape	cp0, 0

.align 3
_dtb:
