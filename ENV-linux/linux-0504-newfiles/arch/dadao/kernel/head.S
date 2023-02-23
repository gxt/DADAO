#include <asm/thread_info.h>
#include <asm/asm-offsets.h>
#include <linux/init.h>
#include <linux/linkage.h>
#include <asm/thread_info.h>
#include <asm/csr.h>

__INIT
ENTRY(_start)
	/*
	 * Image header expected by Linux boot-loaders. The image header data
	 * structure is described in asm/image.h.
	 * Do not modify it without modifying the structure and all bootloaders
	 * that expects this header format!!
	 */
	/* jump to start kernel */
	jump	_start_kernel
	/* reserved */
	.dd.t32	0
	.balign 8
	/* Image load offset(2MB) from start of RAM */
	.dd.o64 0x200000

	/* Effective size of kernel image */
/*	.dd.o64 _end - _start	*/

.global _start_kernel
_start_kernel:	/* FIXME */
	/* Mask all interrupts */

	/* Clear BSS for flat non-ELF images */

	/* Initialize page tables and relocate to virtual addresses */

	/* Restore C environment */

	/* Start the kernel */
	jump	start_kernel

END(_start)

__PAGE_ALIGNED_BSS
	/* Empty zero page */
	.balign PAGE_SIZE
