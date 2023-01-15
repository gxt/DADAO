#define BOOTADDR_REG 0x4000

// TODO: SHOULD redefine following regs, but now reuse risc-v csr module
#define mstatus		0x300
#define misa		0x301
#define mideleg		0x303
#define mie		0x304
#define mtvec		0x305
#define mepc		0x341
#define mhartid		0xf14

// boot all cores (only hart 0) and jump to main program execution
.section .text.start, "ax", @progbits
.globl _start
_start:
	setzw	rb9, w1, 0x200		// base address of clint
	cprd	cp0, cr60, cr20, rd8	// mhartid: f14
	brnz	rd8, boot_core
	setzw	rb10, w1, 0x200
	addi	rb10, 4
	setzw	rd11, w0, 1
interrupt_loop:				// trigger MSIP interrupts for other harts so that they also boot
	stt	rd11, rb10, 0
	addi	rb10, 4
	ldtu	rd11, rb10, -4
	brnz	rd11, interrupt_loop
	jump	boot_core_hart0

// this boot ROM doesn't know about any boot devices, so it just spins,
// waiting for the serial interface to load the program and interrupt it
.section .text.hang, "ax", @progbits
.globl _hang
_hang:					// reset vector
	move	rd8, _start		// on MSIP interrupt, go to _start
	cpwr	cp0, cr12, cr5, rd8	// mtvec
	cprd	cp0, cr12, cr1, rd32	// misa	get MISA mask
	shrs	rd32, rd32, 'S' - 'A'	// get 'S' supervisor bit
	andi	rd32, rd32, 1
	brz	rd32, 1f		// only zero mideleg if supported
	cpwr	cp0, cr12, cr3, rd0	// mideleg: no delegation
1:
	setzw	rd8, w0, 8		// MIE or MSIP bit
	cpwr	cp0, cr12, cr4, rd8	// mie: set only MSIP in MIE CSR
	cprd	cp0, cr12, cr0, rd9	// mstatus
	orw	rd9, w0, 8
	cpwr	cp0, cr12, cr0, rd9	// set MIE in mstatus CSR

wfi_loop: // wait for MSIP interrupt to start program execution
	cpco	cp0, 0
	jump	wfi_loop

// jump to reset vector
.section .text.hang80, "ax", @progbits
.globl _hang80
_hang80:
	jump	_hang

// other harts must wait until MSIP of hart 0 is cleared to continue
boot_core:
	ldtu	rd32, rb9, 0		// load hart 0 MSIP
	brnz	rd32, boot_core		// block until hart 0 clears own MSIP
	shlu	rd8, rd8, 2		// offset for hart MSIP
	add	rd0, rd9, rd8, rd9
boot_core_hart0:			// begin executing code at DRAM_BASE
	stt	rd0, rb9, 0		// clear the interrupt
	move	rb8, BOOTADDR_REG	// program reset vector
	ldo	rd8, rb8, 0		// get boot address from bootaddr_reg SCR
	cpwr	cp0, cr13, cr1, rd8	// mepc: return from interrupt to start of user program
	cprd	cp0, cr60, cr20, rd8	// mhartid: f14, hartid for next level bootloader
	move	rd9, _dtb		// dtb address for next level bootloader
	cprd	cp0, cr12, cr0, rd10	// mstatus
	andnw	rd10, w0, 0x80		// set mstatus MPIE to 0
	cpwr	cp0, cr12, cr0, rd10	// mstatus
	ret	rd0, 0			// TODO: mret?

.align 3
_dtb:
