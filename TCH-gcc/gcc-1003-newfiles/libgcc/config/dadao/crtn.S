/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 */

# Provide last part of _init and _fini.

# The return address is stored in the topmost stored register in the
# register-stack.  We ignore the current value in rJ.  It is probably
# garbage because each fragment of _init and _fini may have their own idea
# of the current stack frame, if they're cut out from a "real" function
# like in gcc/crtstuff.c.

	.section .init,"ax",@progbits
	rb2rb	rb1, rb63, 0	

	.section .fini,"ax",@progbits
	rb2rb	rb1, rb63, 0
