/*
 * Special .init and .fini section support for DADAO.
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */

/* crtn.S puts function epilogues in the .init and .fini sections
   corresponding to the prologues in crti.S. */

	.section .init,"ax",%progbits
	swym
	ret

	.section .fini,"ax",%progbits
	swym
	ret
